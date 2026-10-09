#!/usr/bin/env python3
"""Build the Lowmere secondary tileset (data/tilesets/secondary/lowmere).

Lowmere = Petalburg (unchanged, so every Petalburg metatile id is still valid)
        + marsh, puddle and plank-bridge metatiles imported from Fortree
        + market-stall awnings imported from Slateport (palette 9 -> slot 11)
        + weathered roofs (slot 12) and boarded doors/windows for Stage 0 houses
        + hand-drawn wells, fences, sacks, crates, cart and lanterns (slot 7)

Writes the tileset files plus lowmere_ids.json (name -> metatile id), which
build_maps.py reads. Run from the repo root:  python3 dev_scripts/slice_maps/build_tileset.py
"""
import json
import os

from PIL import Image, ImageDraw

from gfxlib import (NUM_PRIMARY_METATILES, NUM_PRIMARY_TILES, Tileset, flip_tile, path)

OUT = path('data/tilesets/secondary/lowmere')
IDS = os.path.join(os.path.dirname(__file__), 'lowmere_ids.json')

LAYER_NORMAL, LAYER_COVERED = 0, 1
MB_NORMAL = 0x00

# Slot 7: hand-drawn props. Index 0 is transparent.
PROP_PAL = [
    (24, 41, 82),     # 0 transparent
    (246, 238, 205),  # 1 highlight
    (230, 197, 139),  # 2 light wood
    (197, 156, 98),   # 3 wood
    (156, 115, 74),   # 4 dark wood
    (106, 74, 49),    # 5 darker wood
    (57, 41, 41),     # 6 outline
    (222, 222, 213),  # 7 light stone
    (180, 180, 172),  # 8 stone
    (139, 139, 139),  # 9 dark stone
    (98, 98, 106),    # 10 darker stone
    (238, 222, 164),  # 11 sack light / lamp glow
    (205, 180, 115),  # 12 sack
    (49, 74, 106),    # 13 deep water
    (98, 139, 180),   # 14 water
    (115, 148, 82),   # 15 moss
]


def drab(c):
    """Weathered roof colours: desaturate and darken."""
    r, g, b = c
    grey = (r * 30 + g * 59 + b * 11) // 100
    mix = lambda v: int((v * 0.35 + grey * 0.65) * 0.85)
    return (mix(r), mix(g), mix(b))


class Builder:
    def __init__(self):
        self.ts = Tileset(path('data/tilesets/secondary/petalburg'))
        self.ts.pals[7] = list(PROP_PAL)
        self.ts.pals[11] = list(Tileset(path('data/tilesets/secondary/slateport')).pals[9])
        self.ts.pals[12] = [self.ts.pals[10][0]] + [drab(c) for c in self.ts.pals[10][1:]]
        self.primary = Tileset(path('data/tilesets/primary/general'))
        self.ids = {}
        self.src_cache = {}
        self.import_map = {}

    # ---- tiles -------------------------------------------------------
    def tile_pixels(self, entry, src=None):
        t = entry & 0x3FF
        if t < NUM_PRIMARY_TILES:
            return self.primary.tiles[t] if t < len(self.primary.tiles) else (0,) * 64
        ts = src or self.ts
        t -= NUM_PRIMARY_TILES
        return ts.tiles[t] if t < len(ts.tiles) else (0,) * 64

    def add_tile(self, pixels):
        """Return (tile index, hflip, vflip) for pixels, reusing an existing tile when possible."""
        if not any(pixels):
            return 0, 0, 0
        for hf in (0, 1):
            for vf in (0, 1):
                p = flip_tile(pixels, hf, vf)
                try:
                    i = self.ts.tiles.index(p)
                    return NUM_PRIMARY_TILES + i, hf, vf
                except ValueError:
                    pass
        self.ts.tiles.append(tuple(pixels))
        return NUM_PRIMARY_TILES + len(self.ts.tiles) - 1, 0, 0

    def add_metatile(self, entries, attr, name=None):
        mid = NUM_PRIMARY_METATILES + len(self.ts.metatiles)
        self.ts.metatiles.append(list(entries))
        self.ts.attrs.append(attr)
        if name:
            self.ids[name] = mid
        return mid

    # ---- imports -----------------------------------------------------
    def source(self, name):
        if name not in self.src_cache:
            self.src_cache[name] = Tileset(path('data/tilesets/secondary', name))
        return self.src_cache[name]

    def import_metatile(self, src_name, mid, palmap):
        key = '%s:%x' % (src_name, mid)
        if key in self.import_map:
            return self.import_map[key]
        src = self.source(src_name)
        out = []
        for v in src.metatiles[mid - NUM_PRIMARY_METATILES]:
            t, hf, vf, pal = v & 0x3FF, (v >> 10) & 1, (v >> 11) & 1, v >> 12
            if pal >= 6:
                if pal not in palmap:
                    raise ValueError('%s uses unmapped palette %d' % (key, pal))
                pal = palmap[pal]
            if t >= NUM_PRIMARY_TILES:
                nt, nhf, nvf = self.add_tile(flip_tile(self.tile_pixels(v, src), hf, vf))
                t, hf, vf = nt, nhf, nvf
            out.append(t | (hf << 10) | (vf << 11) | (pal << 12))
        new = self.add_metatile(out, src.attrs[mid - NUM_PRIMARY_METATILES])
        self.import_map[key] = new
        return new

    # ---- palette swaps and painted variants ---------------------------
    def variant(self, mid, name, palswap=None, paint=None, attr=None, paint_pal=9):
        """Copy metatile mid (Petalburg/Lowmere id), optionally swapping palettes and
        painting pixels. paint is a 16x16 list of rows (None = keep) in paint_pal indices,
        applied to the paint_pal tile of each quadrant."""
        entries = list(self.ts.metatiles[mid - NUM_PRIMARY_METATILES])
        if palswap:
            entries = [(v & 0x0FFF) | (palswap.get(v >> 12, v >> 12) << 12) for v in entries]
        if paint:
            for q in range(4):
                qx, qy = (q % 2) * 8, (q // 2) * 8
                quad = [paint[qy + y][qx + x] for y in range(8) for x in range(8)]
                if all(p is None for p in quad):
                    continue
                # prefer the top-layer tile in palette 9, then the bottom one, else an empty top slot
                slot = None
                for layer in (1, 0):
                    v = entries[layer * 4 + q]
                    if (v >> 12) == paint_pal and (v & 0x3FF):
                        slot = layer * 4 + q
                        break
                if slot is None:
                    slot = 4 + q
                    entries[slot] = paint_pal << 12
                v = entries[slot]
                base = flip_tile(self.tile_pixels(v), (v >> 10) & 1, (v >> 11) & 1) if v & 0x3FF else (0,) * 64
                px = [quad[i] if quad[i] is not None else base[i] for i in range(64)]
                t, hf, vf = self.add_tile(px)
                entries[slot] = t | (hf << 10) | (vf << 11) | (paint_pal << 12)
        a = self.ts.attrs[mid - NUM_PRIMARY_METATILES] if attr is None else attr
        return self.add_metatile(entries, a, name)

    # ---- hand-drawn props --------------------------------------------
    def prop(self, img, ground, name, layer=LAYER_COVERED, behavior=MB_NORMAL, above_rows=0):
        """img: 'P' image (palette-7 indices, 0 transparent), multiple of 16 px.
        ground: metatile id whose bottom layer shows under the prop.
        Registers name_X_Y for each 16x16 cell. The first above_rows rows are drawn
        over the player (layer type NORMAL), for things the player walks behind."""
        g = self.metatile_entries(ground)
        w, h = img.size
        px = img.load()
        for cy in range(h // 16):
            for cx in range(w // 16):
                top = []
                for q in range(4):
                    ox, oy = cx * 16 + (q % 2) * 8, cy * 16 + (q // 2) * 8
                    tile = tuple(px[ox + x, oy + y] for y in range(8) for x in range(8))
                    t, hf, vf = self.add_tile(tile)
                    top.append(t | (hf << 10) | (vf << 11) | (7 << 12) if t else 0)
                lt = LAYER_NORMAL if cy < above_rows else layer
                self.add_metatile(g[:4] + top, (lt << 12) | behavior, '%s_%d_%d' % (name, cx, cy))

    def metatile_entries(self, mid):
        if mid < NUM_PRIMARY_METATILES:
            return self.primary.metatiles[mid]
        return self.ts.metatiles[mid - NUM_PRIMARY_METATILES]


def canvas(w, h):
    im = Image.new('P', (w, h), 0)
    im.putpalette([c for rgb in PROP_PAL for c in rgb])
    return im, ImageDraw.Draw(im)


def draw_well(broken):
    im, d = canvas(32, 32)
    # cylinder base, body and rim
    d.ellipse([4, 24, 27, 31], fill=9, outline=6)
    d.rectangle([4, 18, 27, 28], fill=8, outline=6)
    for y in (21, 25):
        d.line([5, y, 26, y], fill=9)
    for x, y0, y1 in ((10, 18, 21), (20, 18, 21), (7, 22, 25), (15, 22, 25), (23, 22, 25), (11, 26, 28), (19, 26, 28)):
        d.line([x, y0, x, y1], fill=9)
    d.ellipse([3, 13, 28, 22], fill=7, outline=6)
    d.ellipse([7, 15, 24, 20], fill=6 if broken else 13, outline=10)
    if not broken:
        d.line([9, 18, 14, 18], fill=14)
        # posts, crossbar, rope, bucket, roof
        for x in (5, 24):
            d.rectangle([x, 5, x + 2, 18], fill=4, outline=6)
        d.rectangle([5, 7, 26, 9], fill=3, outline=6)
        d.line([16, 10, 16, 13], fill=12)
        d.rectangle([14, 13, 18, 16], fill=4, outline=6)
        d.polygon([(1, 6), (15, 0), (16, 0), (30, 6)], fill=5, outline=6)
        d.line([3, 6, 28, 6], fill=6)
        for x in (8, 13, 18, 23):
            d.line([x, 3, x, 5], fill=4)
    else:
        # broken rim: knock out the top right of the rim and drop rubble
        d.polygon([(18, 12), (29, 12), (29, 19), (24, 17), (20, 15)], fill=0)
        d.line([18, 13, 24, 17], fill=6)
        d.line([24, 17, 28, 18], fill=6)
        for (x, y) in ((27, 26), (24, 29), (29, 23), (1, 27), (3, 30)):
            d.rectangle([x, y, x + 2, y + 1], fill=8, outline=10)
        # one stump of a post, the other post lying in the mud
        d.rectangle([5, 9, 7, 18], fill=4, outline=6)
        d.line([5, 9, 7, 11], fill=0)
        d.polygon([(12, 30), (31, 25), (31, 27), (13, 31)], fill=4, outline=6)
        d.point([(9, 17), (12, 18), (22, 20)], fill=15)
    return im


def draw_fence(kind):
    im, d = canvas(16, 16)
    if kind == 'post':
        d.rectangle([6, 3, 9, 14], fill=4, outline=6)
        d.line([7, 4, 7, 13], fill=3)
        return im
    rails = (6, 10)
    if kind == 'broken':
        d.rectangle([1, 4, 4, 14], fill=4, outline=6)
        d.rectangle([0, rails[0], 6, rails[0] + 1], fill=3, outline=6)
        d.polygon([(7, 13), (15, 11), (15, 12), (8, 14)], fill=3, outline=6)  # fallen rail
        d.polygon([(11, 14), (13, 7), (15, 8), (13, 15)], fill=4, outline=6)  # leaning post
        return im
    for x in (1, 12):
        d.rectangle([x, 4, x + 3, 14], fill=4, outline=6)
    for y in rails:
        d.rectangle([0, y, 15, y + 1], fill=3)
        d.line([0, y + 2, 15, y + 2], fill=6)
    return im


def draw_sacks():
    im, d = canvas(16, 16)
    for (x, y) in ((0, 6), (7, 6), (3, 1)):
        d.ellipse([x, y, x + 9, y + 9], fill=12, outline=6)
        d.line([x + 3, y + 2, x + 6, y + 2], fill=4)
        d.point([(x + 3, y + 4), (x + 5, y + 5)], fill=11)
    return im


def draw_crate():
    im, d = canvas(16, 16)
    d.rectangle([1, 3, 14, 15], fill=3, outline=6)
    d.line([2, 4, 13, 4], fill=2)
    for y in (7, 11):
        d.line([2, y, 13, y], fill=4)
    d.line([2, 14, 13, 5], fill=5)
    return im


def draw_cart():
    """A grain cart from the side, stuck in mud (3x2 metatiles)."""
    im, d = canvas(48, 32)
    for (x, y) in ((4, 2), (14, 1), (24, 2), (9, 7), (19, 7)):
        d.ellipse([x, y, x + 11, y + 10], fill=12, outline=6)
        d.line([x + 4, y + 2, x + 7, y + 2], fill=4)
        d.point([(x + 3, y + 5), (x + 6, y + 6)], fill=11)
    d.rectangle([1, 15, 38, 21], fill=3, outline=6)
    d.line([2, 16, 37, 16], fill=2)
    d.line([2, 18, 37, 18], fill=4)
    d.polygon([(38, 17), (47, 21), (47, 23), (38, 19)], fill=4, outline=6)  # shaft
    for cx in (9, 29):
        d.ellipse([cx - 7, 17, cx + 7, 31], fill=5, outline=6)
        d.ellipse([cx - 4, 20, cx + 4, 28], fill=0, outline=4)
        d.line([cx, 18, cx, 30], fill=4)
        d.line([cx - 6, 24, cx + 6, 24], fill=4)
        d.rectangle([cx - 1, 23, cx + 1, 25], fill=6)
    d.rectangle([0, 28, 40, 31], fill=5)  # mud over the wheel bottoms
    for x in range(1, 40, 5):
        d.point((x, 28), fill=4)
    return im


def draw_lantern():
    im, d = canvas(16, 32)
    d.rectangle([7, 12, 8, 30], fill=5)
    d.line([7, 12, 7, 30], fill=6)
    d.rectangle([5, 29, 10, 31], fill=10, outline=6)
    d.rectangle([4, 3, 11, 11], fill=11, outline=6)
    d.line([7, 4, 7, 10], fill=1)
    d.polygon([(3, 3), (8, 0), (12, 3)], fill=5, outline=6)
    return im


def draw_stump():
    im, d = canvas(16, 16)
    d.ellipse([2, 9, 13, 15], fill=5, outline=6)
    d.rectangle([2, 5, 13, 12], fill=4, outline=6)
    d.ellipse([2, 3, 13, 8], fill=2, outline=6)
    d.ellipse([5, 4, 10, 7], outline=3)
    return im


def draw_reeds():
    im, d = canvas(16, 16)
    for x, top in ((2, 3), (5, 1), (8, 4), (11, 2), (13, 5)):
        d.line([x, top + 3, x, 15], fill=15)
        d.rectangle([x - 1, top, x, top + 4], fill=5)
    return im


def draw_mud(seed):
    im, d = canvas(16, 16)
    shapes = {0: [(1, 4, 13, 12), (6, 2, 14, 8)], 1: [(2, 6, 10, 14), (7, 3, 15, 10)]}[seed]
    for box in shapes:
        d.ellipse(box, fill=4)
    for box in shapes:
        x0, y0, x1, y1 = box
        d.ellipse((x0 + 2, y0 + 2, x1 - 2, y1 - 1), fill=5)
    d.point([(5, 7), (10, 6), (8, 10)], fill=3)
    return im


def draw_grave():
    im, d = canvas(16, 16)
    d.rectangle([4, 3, 11, 14], fill=8, outline=6)
    d.ellipse([4, 1, 11, 7], fill=8, outline=6)
    d.rectangle([5, 5, 10, 13], fill=8)
    d.line([7, 5, 8, 5], fill=10)
    d.line([6, 7, 9, 7], fill=10)
    d.rectangle([2, 13, 13, 15], fill=15)
    return im


def draw_tent():
    """A patched canvas tent, front on (2x2 metatiles): the evicted farmers' camp."""
    im, d = canvas(32, 32)
    d.polygon([(16, 1), (31, 30), (0, 30)], fill=12, outline=6)
    d.polygon([(16, 1), (24, 30), (8, 30)], fill=11, outline=6)
    d.polygon([(16, 12), (21, 30), (11, 30)], fill=6)             # the open flap
    d.line([16, 0, 16, 3], fill=4)
    d.rectangle([22, 18, 26, 22], fill=3, outline=5)               # a patch
    d.rectangle([4, 24, 8, 27], fill=4, outline=5)
    d.line([0, 31, 31, 31], fill=5)
    return im


def draw_campfire():
    im, d = canvas(16, 16)
    for (x, y) in ((1, 11), (5, 13), (10, 13), (13, 10), (3, 8)):
        d.ellipse([x, y, x + 3, y + 2], fill=9, outline=10)
    d.line([4, 12, 12, 9], fill=5, width=2)
    d.line([4, 9, 12, 12], fill=4, width=2)
    d.polygon([(8, 1), (12, 9), (8, 11), (4, 9)], fill=12, outline=4)
    d.polygon([(8, 4), (10, 9), (8, 10), (6, 9)], fill=11)
    d.point([(8, 7)], fill=1)
    return im


def draw_greenhouse():
    """Glass house with a stone base and plants inside (3x3 metatiles, door at +1,+2)."""
    im, d = canvas(48, 48)
    d.polygon([(0, 18), (24, 1), (47, 18)], fill=14, outline=6)    # glass roof
    for x in range(6, 44, 6):
        d.line([x, 18, 24 + (x - 24) // 2, 9 if x != 24 else 1], fill=9)
    d.line([24, 1, 24, 18], fill=9)
    d.line([9, 12, 17, 7], fill=1)
    d.rectangle([1, 18, 46, 41], fill=14, outline=6)               # glass walls
    for x in range(8, 46, 8):
        d.line([x, 19, x, 40], fill=9)
    d.line([2, 29, 45, 29], fill=9)
    for x in range(3, 44, 6):                                      # leaves behind the glass
        d.ellipse([x, 31, x + 6, 39], fill=15)
    for x in (4, 36):
        d.line([x, 21, x + 4, 25], fill=1)
    d.rectangle([0, 40, 47, 47], fill=8, outline=6)                # stone base
    for x in range(8, 47, 12):
        d.line([x, 41, x, 46], fill=9)
    d.rectangle([18, 26, 29, 47], fill=13, outline=6)              # door
    d.line([23, 27, 23, 46], fill=9)
    d.point([(27, 37)], fill=1)
    return im


def draw_flagstones():
    im, d = canvas(16, 16)
    for box in ((0, 0, 7, 6), (8, 0, 15, 4), (0, 7, 5, 15), (6, 7, 15, 11), (8, 5, 15, 6), (6, 12, 15, 15)):
        d.rectangle(box, fill=7, outline=8)
    d.point([(2, 2), (11, 2), (3, 10), (9, 9), (12, 14)], fill=9)
    return im


def draw_rowboat():
    """A moored rowboat, bow to the left (2x1 metatiles on water)."""
    im, d = canvas(32, 16)
    d.polygon([(0, 7), (6, 2), (29, 2), (31, 5), (29, 13), (6, 13)], fill=3, outline=6)
    d.polygon([(4, 7), (8, 4), (27, 4), (28, 7), (27, 11), (8, 11)], fill=5)
    for x in (13, 21):
        d.rectangle([x, 4, x + 2, 11], fill=2, outline=4)
    d.line([1, 7, 29, 7], fill=4)
    return im


def draw_notice():
    """A nailed foreclosure notice on a post (1x1)."""
    im, d = canvas(16, 16)
    d.rectangle([7, 9, 8, 15], fill=4, outline=6)
    d.rectangle([2, 1, 13, 10], fill=1, outline=6)
    for y in (3, 5, 7):
        d.line([4, y, 11, y], fill=9)
    d.point([(7, 2)], fill=6)
    d.rectangle([9, 7, 11, 9], fill=13)    # the seal
    return im


def boards(kind):
    """16x16 overlay in palette-9 indices for a boarded window (on 28f/27d) or door (27f/287)."""
    P, S, D = 3, 4, 8  # plank, shadow, dark
    grid = [[None] * 16 for _ in range(16)]

    def plank(y, x0, x1):
        for x in range(x0, x1 + 1):
            grid[y][x], grid[y + 1][x], grid[y + 2][x] = P, P, S
        for x in (x0 + 1, x1 - 1):
            grid[y + 1][x] = D

    if kind == 'window_low':     # 28f: lower half of the window
        plank(1, 0, 14)
        plank(6, 0, 14)
    elif kind == 'window_high':  # 27d: bottom strip of the window
        plank(12, 0, 14)
    elif kind == 'door_low':     # 287
        plank(1, 0, 11)
        plank(8, 0, 11)
    elif kind == 'door_high':    # 27f
        plank(11, 0, 11)
    return grid


def roof_hole():
    grid = [[None] * 16 for _ in range(16)]
    for y, x0, x1 in ((5, 6, 9), (6, 4, 10), (7, 5, 10), (8, 6, 8)):
        for x in range(x0, x1 + 1):
            grid[y][x] = 8
    return grid


def gild(c):
    """General palette 1's reds and oranges -> gold."""
    gold = {(255, 189, 131): (255, 238, 164), (238, 148, 115): (246, 213, 98),
            (222, 106, 98): (222, 172, 49), (205, 65, 82): (180, 131, 32)}
    return gold.get(c, c)


def build_haymarket():
    """Haymarket = Slateport + a gilded copy of the Battle Tent dome (palette 1 -> slot 12)."""
    ts = Tileset(path('data/tilesets/secondary/slateport'))
    general = Tileset(path('data/tilesets/primary/general'))
    ts.pals[12] = [gild(c) for c in general.pals[1]]
    ids = {}
    dome = [[0x371, 0x372, 0x373, 0x374, 0x375], [0x379, 0x37A, 0x37B, 0x37C, 0x37D],
            [0x381, 0x382, 0x383, 0x384, 0x385], [0x389, 0x38A, 0x38B, 0x38C, 0x38D],
            [0x391, 0x392, 0x393, 0x394, 0x395]]
    for y, row in enumerate(dome):
        for x, mid in enumerate(row):
            entries = [(v & 0x0FFF) | (12 << 12) if (v >> 12) == 1 else v
                       for v in ts.metatiles[mid - NUM_PRIMARY_METATILES]]
            ids['gilt_pavilion_%d_%d' % (x, y)] = NUM_PRIMARY_METATILES + len(ts.metatiles)
            ts.metatiles.append(entries)
            ts.attrs.append(ts.attrs[mid - NUM_PRIMARY_METATILES])
    assert len(ts.metatiles) <= 512
    ts.save(path('data/tilesets/secondary/haymarket'))
    json.dump({'num_tiles': len(ts.tiles), 'ids': ids}, open(os.path.join(os.path.dirname(__file__), 'haymarket_ids.json'), 'w'), indent=1)
    print('haymarket tileset: %d metatiles' % len(ts.metatiles))


# Slot 9 of the Gilt Pavilion tileset: gold lot plates for the auction-floor puzzle.
LOT_PAL = [(0, 0, 0), (106, 74, 24), (180, 131, 32), (222, 172, 49), (246, 213, 98),
           (255, 238, 164), (255, 250, 222), (74, 41, 32)] + [(0, 0, 0)] * 8
DIGITS = ['111101101101111', '010110010010111', '111001111100111', '111001111001111', '101101111001001',
          '111100111001111', '111100111101111', '111001001010010', '111101111101111', '111101111001111']
LOT_NUMBERS = list(range(1, 22))


def draw_lot(n):
    """16x16 gold plate with a raised rim and the lot number in 2x-scaled 3x5 digits."""
    px = [[5] * 16 for _ in range(16)]
    for i in range(16):
        px[0][i] = px[i][0] = 4
        px[15][i] = px[i][15] = 1
        px[1][i] = px[i][1] = 6 if 0 < i < 15 else px[1][i]
        px[14][i] = px[i][14] = 2 if 0 < i < 15 else px[14][i]
    text = str(n)
    w = len(text) * 7 - 1
    x0, y0 = (16 - w) // 2, 3
    for k, ch in enumerate(text):
        bits = DIGITS[int(ch)]
        for r in range(5):
            for c in range(3):
                if bits[r * 3 + c] == '1':
                    for dy in range(2):
                        for dx in range(2):
                            px[y0 + r * 2 + dy][x0 + k * 7 + c * 2 + dx] = 7
    return px


def build_gilt_pavilion():
    """Gilt Pavilion = Petalburg Gym + gold lot plates numbered 1-21 (palette slot 9)."""
    ts = Tileset(path('data/tilesets/secondary/petalburg_gym'))
    ts.pals[9] = list(LOT_PAL)
    ids = {}
    for n in LOT_NUMBERS:
        px = draw_lot(n)
        entries = []
        for q in range(4):
            qx, qy = (q % 2) * 8, (q // 2) * 8
            ts.tiles.append(tuple(px[qy + y][qx + x] for y in range(8) for x in range(8)))
            entries.append((NUM_PRIMARY_TILES + len(ts.tiles) - 1) | (9 << 12))
        ids['lot_%d' % n] = NUM_PRIMARY_METATILES + len(ts.metatiles)
        ts.metatiles.append(entries + [0, 0, 0, 0])
        ts.attrs.append((LAYER_NORMAL << 12) | MB_NORMAL)
    assert len(ts.tiles) <= 512 and len(ts.metatiles) <= 512
    ts.save(path('data/tilesets/secondary/gilt_pavilion'))
    json.dump({'num_tiles': len(ts.tiles), 'ids': ids},
              open(os.path.join(os.path.dirname(__file__), 'gilt_pavilion_ids.json'), 'w'), indent=1)
    print('gilt pavilion tileset: %d tiles, %d metatiles' % (len(ts.tiles), len(ts.metatiles)))


def main():
    build_haymarket()
    build_gilt_pavilion()
    b = Builder()
    swap = {10: 12}

    # Weathered Oldale-style house (4x4): roof rows, then front rows.
    house = [[0x26c, 0x26d, 0x26d, 0x26e],
             [0x274, 0x275, 0x275, 0x276],
             [0x27c, 0x27f, 0x27d, 0x27e],
             [0x284, 0x287, 0x28f, 0x286]]
    for y, row in enumerate(house):
        for x, mid in enumerate(row):
            b.variant(mid, 'drab_house_%d_%d' % (x, y), palswap=swap)
    # hole in the roof, boarded window and door (door no longer a warp)
    b.variant(0x275, 'drab_roof_hole', palswap=swap, paint=roof_hole(), paint_pal=12)
    b.variant(0x27d, 'boarded_window_high', palswap=swap, paint=boards('window_high'))
    b.variant(0x28f, 'boarded_window_low', paint=boards('window_low'))
    b.variant(0x27f, 'boarded_door_high', palswap=swap, paint=boards('door_high'))
    b.variant(0x287, 'boarded_door_low', paint=boards('door_low'), attr=(LAYER_COVERED << 12) | MB_NORMAL)

    # Marsh, puddles, berry soil and pond bridges from Fortree (primary palettes only).
    skipped = []
    for mid in range(0x270, 0x2a0):
        try:
            b.import_metatile('fortree', mid, {})
        except ValueError:
            skipped.append(hex(mid))
    print('skipped fortree metatiles that need Fortree palettes:', ' '.join(skipped))
    # Route 120's long plank bridge over deeper water.
    for mid in (0x2ef, 0x2f4, 0x2f5, 0x2f7, 0x30b, 0x30c, 0x313, 0x314, 0x2fb, 0x2fc, 0x303, 0x304):
        b.import_metatile('fortree', mid, {})
    # Market-stall awning from Slateport (palette 9 -> slot 11).
    for mid in (0x270, 0x271, 0x272, 0x278, 0x279, 0x27a, 0x263, 0x264):
        b.import_metatile('slateport', mid, {9: 11})

    grass, marsh = 0x001, b.import_map['fortree:291']
    b.prop(draw_well(False), grass, 'well')
    b.prop(draw_well(True), grass, 'well_broken')
    for kind in ('full', 'broken', 'post'):
        b.prop(draw_fence(kind), grass, 'fence_' + kind)
    b.prop(draw_sacks(), grass, 'sacks')
    b.prop(draw_crate(), grass, 'crate')
    b.prop(draw_cart(), marsh, 'cart')
    b.prop(draw_lantern(), grass, 'lantern', above_rows=1)
    b.prop(draw_stump(), grass, 'stump')
    b.prop(draw_stump(), marsh, 'stump_marsh')
    b.prop(draw_reeds(), marsh, 'reeds', behavior=0x02)  # MB_TALL_GRASS: wild encounters
    b.prop(draw_grave(), grass, 'grave')
    b.prop(draw_mud(0), marsh, 'mud_a', behavior=0x16)  # MB_PUDDLE
    b.prop(draw_mud(1), marsh, 'mud_b', behavior=0x16)
    # Chapter 2 (Thornfield and the Orchard Road). Append only: earlier ids must not move.
    b.prop(draw_tent(), grass, 'tent')
    b.prop(draw_campfire(), grass, 'campfire')
    b.prop(draw_greenhouse(), grass, 'greenhouse')
    b.prop(draw_flagstones(), grass, 'flagstones')
    b.prop(draw_rowboat(), 0x0D1, 'rowboat')
    b.prop(draw_notice(), grass, 'notice')
    b.prop(draw_flagstones(), grass, 'exit_mat', behavior=0x65)  # MB_SOUTH_ARROW_WARP

    ntiles = len(b.ts.tiles)
    nmeta = len(b.ts.metatiles)
    assert ntiles <= 512, ntiles
    assert nmeta <= 512, nmeta
    b.ts.save(OUT)
    ids = dict(b.ids)
    ids.update({k: v for k, v in b.import_map.items()})
    json.dump({'num_tiles': ntiles, 'num_metatiles': nmeta,
               'ids': {k: v for k, v in sorted(ids.items())}}, open(IDS, 'w'), indent=1)
    print('lowmere tileset: %d tiles, %d metatiles' % (ntiles, nmeta))


if __name__ == '__main__':
    main()
