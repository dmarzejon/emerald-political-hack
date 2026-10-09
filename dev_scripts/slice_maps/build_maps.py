#!/usr/bin/env python3
"""Generate the vertical-slice layouts: Lowmere stages 0-4, the Mire Road and Haymarket.

Run build_tileset.py first, then this, from the repo root:
    python3 dev_scripts/slice_maps/build_tileset.py
    python3 dev_scripts/slice_maps/build_maps.py

The generated .bin files are the source of truth once committed; edit them in Porymap
afterwards. Re-running this script overwrites hand edits to these layouts.
"""
import json
import os

from gfxlib import Layout, block, path

HERE = os.path.dirname(__file__)
LM = json.load(open(os.path.join(HERE, 'lowmere_ids.json')))['ids']


def fortree(m):
    return LM['fortree:%x' % m]


def slateport_l(m):
    """A Slateport metatile imported into the Lowmere tileset."""
    return LM['slateport:%x' % m]


# ---- metatile vocabulary (General primary + Lowmere secondary) ----------
GRASS = 0x001
FLOWERS = 0x004
BUSH = 0x00E          # small round tree, 1x1
SIGNPOST = 0x003
TALL_GRASS = 0x00D
SAND = {'tl': 0x1D0, 't': 0x1D1, 'tr': 0x1D2, 'l': 0x1D8, 'c': 0x1D9, 'r': 0x1DA,
        'bl': 0x1E0, 'b': 0x1E1, 'br': 0x1E2}
DIRT = {'tl': 0x118, 't': 0x119, 'tr': 0x11A, 'l': 0x120, 'c': 0x121, 'r': 0x122,
        'bl': 0x128, 'b': 0x129, 'br': 0x12A}
POND = {'tl': 0x0C8, 't': 0x0C9, 'tr': 0x0CA, 'l': 0x0D0, 'c': 0x0D1, 'r': 0x0D2,
        'bl': 0x0D8, 'b': 0x0D9, 'br': 0x0DA}
MARSH = {k: fortree(v) for k, v in {'tl': 0x288, 't': 0x289, 'tr': 0x28A, 'l': 0x290, 'c': 0x291,
                                     'r': 0x292, 'bl': 0x298, 'b': 0x299, 'br': 0x29A}.items()}
MARSH_POOL = [[fortree(0x274), fortree(0x275)], [fortree(0x27C), fortree(0x27D)]]
BERRY_SOIL = fortree(0x293)
BRIDGE = {'top': (fortree(0x285), fortree(0x287), fortree(0x286)),
          'bot': (fortree(0x28D), fortree(0x28F), fortree(0x28E))}
DEEP_WATER = fortree(0x277)

E_GROUND, E_WATER = 3, 1


def prop(name, w, h):
    return [[LM['%s_%d_%d' % (name, x, y)] for x in range(w)] for y in range(h)]


class Canvas(Layout):
    def put(self, x, y, mt, col=0, elev=E_GROUND):
        self.set(x, y, block(mt, col, elev))

    def fill(self, x0, y0, x1, y1, mt, col=0, elev=E_GROUND):
        for y in range(y0, y1 + 1):
            for x in range(x0, x1 + 1):
                self.put(x, y, mt, col, elev)

    def grid(self, x0, y0, rows, col=1, elev=0):
        for dy, row in enumerate(rows):
            for dx, mt in enumerate(row):
                if mt is not None:
                    self.put(x0 + dx, y0 + dy, mt, col, elev)

    def stamp(self, src, sx, sy, w, h, dx, dy, remap=None):
        for y in range(h):
            for x in range(w):
                v = src.get(sx + x, sy + y)
                if remap:
                    v = (v & ~0x3FF) | remap.get(v & 0x3FF, v & 0x3FF)
                self.set(dx + x, dy + y, v)

    def forest(self, x0, y0, x1, y1):
        """Dense trees. Rows alternate tree tops and bottoms from y0."""
        for y in range(y0, y1 + 1):
            for x in range(x0, x1 + 1):
                top = (y - y0) % 2 == 0
                left = (x - x0) % 2 == 0
                last = y == y1
                if top:
                    mt = 0x1D4 if left else 0x1D5
                elif last:
                    mt = 0x1E4 if left else 0x1E5
                else:
                    mt = 0x1DC if left else 0x1DD
                self.put(x, y, mt, 1, 0)

    def region(self, cells, tiles, col=0, elev=E_GROUND, joins=()):
        """Autotile a set of (x, y) cells with a 9-piece edge set. Cells in `joins` count
        as neighbours without being painted (a road running off the map or into a street)."""
        cells = set(cells)
        near = cells | set(joins)
        for (x, y) in cells:
            n, s = (x, y - 1) in near, (x, y + 1) in near
            w, e = (x - 1, y) in near, (x + 1, y) in near
            if not n:
                k = 'tl' if not w else 'tr' if not e else 't'
            elif not s:
                k = 'bl' if not w else 'br' if not e else 'b'
            else:
                k = 'l' if not w else 'r' if not e else 'c'
            self.put(x, y, tiles[k], col, elev)

    def pond(self, x0, y0, x1, y1):
        self.region(rect(x0, y0, x1, y1), POND, 0, E_WATER)


def rect(x0, y0, x1, y1):
    return [(x, y) for y in range(y0, y1 + 1) for x in range(x0, x1 + 1)]


# ---- buildings ----------------------------------------------------------
SLATEPORT = Layout.load(path('data/layouts/SlateportCity/map.bin'), 40)

# Blocks (metatile | collision | elevation) copied from vanilla Oldale and Littleroot,
# whose layouts this script replaces.
HOUSE = [[0x326C, 0x326D, 0x326D, 0x326E], [0x674, 0x675, 0x675, 0x676],
         [0x67C, 0x67F, 0x67D, 0x67E], [0x684, 0x687, 0x68F, 0x686]]      # door +1,+3
PC = [[0x3048, 0x3049, 0x304A, 0x304B], [0x450, 0x451, 0x452, 0x453],
      [0x458, 0x459, 0x45A, 0x45B], [0x460, 0x461, 0x462, 0x463]]         # door +1,+3
MART = [[0x3028, 0x3029, 0x3029, 0x302B], [0x430, 0x431, 0x432, 0x433],
        [0x438, 0x439, 0x43A, 0x43B], [0x460, 0x441, 0x442, 0x443]]       # door +1,+3
LODGE = [[0x3208, 0x3209, 0x3209, 0x3209, 0x320A], [0x610, 0x611, 0x611, 0x611, 0x612],
         [0x618, 0x619, 0x619, 0x619, 0x61A], [0x622, 0x632, 0x630, 0x640, 0x621],
         [0x62A, 0x63A, 0x638, 0x648, 0x629]]                             # door +3,+4


def raw(c, x, y, rows):
    for dy, row in enumerate(rows):
        for dx, v in enumerate(row):
            c.set(x + dx, y + dy, v)


def house(c, x, y, state='normal'):
    """Oldale-style cottage. state: normal | weathered | boarded."""
    raw(c, x, y, HOUSE)
    if state == 'normal':
        return
    for dy in range(4):
        for dx in range(4):
            v = c.get(x + dx, y + dy)
            c.set(x + dx, y + dy, (v & ~0x3FF) | LM['drab_house_%d_%d' % (dx, dy)])
    if state == 'boarded':
        swap = {(2, 1): 'drab_roof_hole', (2, 2): 'boarded_window_high', (2, 3): 'boarded_window_low',
                (1, 2): 'boarded_door_high', (1, 3): 'boarded_door_low'}
        for (dx, dy), name in swap.items():
            v = c.get(x + dx, y + dy)
            c.set(x + dx, y + dy, (v & ~0x3FF) | LM[name])


def stall(c, x, y, goods='sacks'):
    """3x3 market stall: awning, awning, legs with goods between."""
    c.grid(x, y, [[slateport_l(0x270), slateport_l(0x271), slateport_l(0x272)],
                  [slateport_l(0x278), slateport_l(0x279), slateport_l(0x27A)]], col=0, elev=E_GROUND)
    c.put(x, y + 2, slateport_l(0x263), 1, 0)
    c.put(x + 2, y + 2, slateport_l(0x264), 1, 0)
    c.put(x + 1, y + 2, LM['%s_0_0' % goods], 1, E_GROUND)
    # the awning's top row is walk-behind, the front row blocks
    for dx in range(3):
        v = c.get(x + dx, y + 1)
        c.set(x + dx, y + 1, v | (1 << 10))


# ---- Lowmere -----------------------------------------------------------
LOWMERE_W, LOWMERE_H = 32, 26
NORTH_EXIT = (15, 16)      # the two columns that lead onto the Mire Road

# Building positions (top-left). Doors: Oldale-style +1,+3; Lodge +3,+4.
LODGE_POS = (3, 3)
REEVE = (9, 3)
SHED = (19, 3)
HOUSE_A = (24, 3)
FORGE = (2, 11)
MART_LOT = (7, 11)
HESK = (24, 10)
HOUSE_B = (3, 18)
HOUSE_C = (9, 18)
SQUARE = (12, 10, 20, 16)
WELL = (15, 12)
POND_RECT = (19, 18, 29, 23)
JETTY_Y = 20
ROWBOATS = [(27, 20), (20, 22)]     # 2x1, from Stage 2
ORCHARD_Y, ORCHARD_XS = 17, (22, 24, 26, 28)
LM_SIGNS = {'town': (14, 9), 'shed': (18, 6), 'lodge': (8, 7), 'hesk': (23, 13)}


def lowmere(stage):
    c = Canvas(LOWMERE_W, LOWMERE_H, block(GRASS))
    # forest frame with the road north
    c.forest(0, 0, 31, 1)
    c.forest(0, 24, 31, 25)
    c.forest(0, 2, 1, 23)
    c.forest(30, 2, 31, 23)
    for x in NORTH_EXIT:
        c.fill(x, 0, x, 1, GRASS)
    # a few trees breaking up the edges
    for (x, y) in ((2, 2), (29, 2), (2, 23), (8, 9), (23, 8), (29, 16)):
        c.put(x, y, BUSH, 1, 0)

    # marsh: the ground the village sits in
    marsh = set(rect(2, 8, 8, 9)) | set(rect(13, 18, 18, 23)) | set(rect(2, 22, 12, 23)) | set(rect(21, 15, 28, 16))
    if stage >= 2:
        marsh -= set(rect(21, 15, 28, 16))     # drained for gardens
    c.region(marsh, MARSH)
    c.grid(15, 21, MARSH_POOL, col=0, elev=E_GROUND)
    for (x, y) in ((3, 22), (4, 22), (11, 23), (13, 19), (17, 18), (14, 23)):
        c.put(x, y, LM['reeds_0_0'], 0, E_GROUND)

    # roads and the square: mud at Stage 0, packed sand from Stage 1
    road = set(rect(NORTH_EXIT[0], 0, NORTH_EXIT[1], 9))      # north road
    road |= set(rect(5, 8, 14, 8)) | set(rect(9, 7, 14, 7))      # Lodge and Reeve
    road |= set(rect(17, 7, 26, 8))                               # Shed and house A
    road |= set(rect(*SQUARE))
    road |= set(rect(3, 15, 11, 16))                              # Forge
    road |= set(rect(21, 14, 26, 14))                             # Hesk
    if stage == 0:
        c.region(road, MARSH)
        for i, (x, y) in enumerate(sorted(road)):
            if (x * 7 + y * 13) % 9 == 0:
                c.put(x, y, LM['mud_a_0_0' if i % 2 else 'mud_b_0_0'], 0, E_GROUND)
    elif stage == 1:
        c.region(road, DIRT)
    else:
        # Thornfield's pact: the roads are laid with flagstones
        for (x, y) in road:
            c.put(x, y, LM['flagstones_0_0'], 0, E_GROUND)

    # the pond and the jetty
    c.pond(*POND_RECT)
    if stage >= 2:
        planks = range(19, 27)
    else:
        planks = (19, 20, 21, 24)              # collapsed: gaps in the middle, end missing
    for x in planks:
        i = 0 if x == 19 else 2 if x == max(planks) and stage >= 2 else 1
        c.put(x, JETTY_Y, BRIDGE['top'][i], 0, E_GROUND)
        c.put(x, JETTY_Y + 1, BRIDGE['bot'][i], 0, E_GROUND)
    if stage < 2:
        c.put(24, JETTY_Y, BRIDGE['top'][1], 1, 0)    # a stranded piece you can't reach
        c.put(24, JETTY_Y + 1, BRIDGE['bot'][1], 1, 0)
    else:
        for (x, y) in ROWBOATS:                        # moored at the rebuilt jetty
            c.grid(x, y, prop('rowboat', 2, 1), col=1, elev=0)

    # buildings
    raw(c, LODGE_POS[0], LODGE_POS[1], LODGE)
    house(c, *REEVE, state='weathered' if stage < 3 else 'normal')
    house(c, *SHED, state='weathered' if stage < 3 else 'normal')
    house(c, *HOUSE_A, state='boarded' if stage < 2 else 'weathered' if stage < 3 else 'normal')
    house(c, *FORGE, state='weathered' if stage < 3 else 'normal')
    house(c, *HESK, state='weathered' if stage < 3 else 'normal')
    house(c, *HOUSE_C, state='boarded' if stage < 1 else 'weathered' if stage < 3 else 'normal')
    if stage < 2:
        house(c, *HOUSE_B, state='boarded')
    else:
        raw(c, HOUSE_B[0], HOUSE_B[1], PC)
    if stage >= 3:
        raw(c, MART_LOT[0], MART_LOT[1], MART)

    # forge yard
    c.put(FORGE[0] + 4, FORGE[1] + 2, LM['stump_0_0'], 1, E_GROUND)
    c.put(FORGE[0] + 4, FORGE[1] + 3, LM['crate_0_0'], 1, E_GROUND)

    # the well
    c.grid(WELL[0], WELL[1], prop('well' if stage >= 1 else 'well_broken', 2, 2), col=1, elev=E_GROUND)

    # fences along the Lodge garden and the Shed
    lodge_fence = [(x, 9) for x in range(3, 8)]
    for i, (x, y) in enumerate(lodge_fence):
        kind = 'full' if stage >= 1 or i % 3 == 0 else 'broken' if i % 3 == 1 else None
        if kind:
            c.put(x, y, LM['fence_%s_0_0' % kind], 1, E_GROUND)
    # market: empty square at Stage 0, two stalls from Stage 1
    if stage == 0:
        for x in (12, 18):      # what's left of the old stalls
            c.put(x, 16, slateport_l(0x263), 1, 0)
            c.put(x + 2, 16, slateport_l(0x264), 1, 0)
    if stage >= 1:
        stall(c, 12, 14, 'sacks')
        stall(c, 18, 14, 'crate')
        c.put(13, 10, LM['sacks_0_0'], 1, E_GROUND)
        c.put(19, 10, LM['crate_0_0'], 1, E_GROUND)
    # gardens and berry plots once the marsh east of Hesk's is drained
    if stage >= 2:
        for x in range(22, 28):
            c.put(x, 16, BERRY_SOIL if x % 2 == 0 else fortree(0x294), 0, E_GROUND)
            c.put(x, 15, FLOWERS, 0, E_GROUND)
        for x in ORCHARD_XS:                           # fruit trees from Thornfield's nurseries
            c.put(x, ORCHARD_Y, BUSH, 1, 0)
    # lanterns on the road and the square
    if stage >= 3:
        for (x, y) in ((14, 5), (17, 5), (11, 12), (21, 12)):
            c.put(x, y - 1, LM['lantern_0_0'], 0, E_GROUND)
            c.put(x, y, LM['lantern_0_1'], 1, E_GROUND)
        for x in range(12, 21, 2):
            c.put(x, 17, FLOWERS, 0, E_GROUND)
    # town sign by the road, and name boards by the Lodge, the Shed and Hesk's door
    for (x, y) in LM_SIGNS.values():
        c.put(x, y, SIGNPOST, 1, E_GROUND)
    return c


def border(mts):
    """2x2 border block."""
    c = Canvas(2, 2)
    c.grid(0, 0, mts, col=1, elev=0)
    return c


def save_layout(c, name, layout_id, border_layout, secondary='gTileset_Lowmere', primary=None):
    d = path('data/layouts', name)
    c.save(os.path.join(d, 'map.bin'))
    border_layout.save(os.path.join(d, 'border.bin'))
    if primary is None:
        primary = 'gTileset_Building' if secondary == 'gTileset_PetalburgGym' else 'gTileset_General'
    register_layout(layout_id, name, c.w, c.h, secondary, primary)


def register_layout(layout_id, name, w, h, secondary, primary='gTileset_General'):
    """Add or update an entry in data/layouts/layouts.json, keeping its formatting."""
    p = path('data/layouts/layouts.json')
    data = json.load(open(p))
    entry = {
        'id': layout_id,
        'name': name + '_Layout',
        'width': w,
        'height': h,
        'primary_tileset': primary,
        'secondary_tileset': secondary,
        'border_filepath': 'data/layouts/%s/border.bin' % name,
        'blockdata_filepath': 'data/layouts/%s/map.bin' % name,
        'layout_version': 'emerald',
    }
    layouts = data['layouts']
    for i, l in enumerate(layouts):
        if l.get('id') == layout_id:
            layouts[i] = entry
            break
    else:
        layouts.append(entry)
    with open(p, 'w') as f:
        json.dump(data, f, indent=2)
        f.write('\n')


FOREST_BORDER = border([[0x1D4, 0x1D5], [0x1DC, 0x1DD]])


# ---- the Mire Road (Route 101 slot) ---------------------------------------
MIRE_W, MIRE_H = 24, 44
MIRE_SOUTH = (11, 12)      # meets Lowmere's NORTH_EXIT
MIRE_NORTH = (11, 12)      # meets Haymarket's SOUTH_EXIT
WAGON = (10, 16)           # 3x2 cart
MIRE_DRY = 7               # dry rows at the north end


def mire_road():
    c = Canvas(MIRE_W, MIRE_H, block(GRASS))
    # The marsh starts below MIRE_DRY rows of dry ground, so the tiles Haymarket draws
    # across the north seam are all General tiles (see HAY_ORIGIN).
    c.region(rect(2, MIRE_DRY, 21, MIRE_H - 1), MARSH)
    # forest frame and the chunks that make the road wind
    c.forest(0, 0, 1, MIRE_H - 1)
    c.forest(22, 0, 23, MIRE_H - 1)
    c.forest(2, 0, 10, 1)
    c.forest(13, 0, 21, 1)
    c.forest(2, 2, 7, 11)          # north-west
    c.forest(16, 2, 21, 9)         # north-east
    c.forest(2, 36, 9, 41)         # south-west
    c.forest(15, 38, 21, 41)       # south-east
    c.forest(2, 42, 10, 43)
    c.forest(13, 42, 21, 43)
    # the pond the old boardwalk crosses, and a deep pool on the east side
    c.pond(6, 22, 17, 25)
    for x in range(6, 18):
        i = 0 if x == 6 else 2 if x == 17 else 1
        c.put(x, 23, BRIDGE['top'][i], 0, E_GROUND)
        c.put(x, 24, BRIDGE['bot'][i], 0, E_GROUND)
    c.pond(14, 29, 21, 33)
    # reeds and grass: wild encounters
    for (x0, y0, x1, y1) in ((10, 33, 14, 37), (2, 13, 5, 20), (17, 11, 21, 19), (2, 27, 8, 32), (8, 3, 10, 9)):
        for (x, y) in rect(x0, y0, x1, y1):
            c.put(x, y, LM['reeds_0_0'] if y >= MIRE_DRY else TALL_GRASS, 0, E_GROUND)
    for (x, y) in rect(15, 34, 21, 36):
        c.put(x, y, TALL_GRASS, 0, E_GROUND)
    # small marsh pools for fishing
    c.grid(3, 23, MARSH_POOL, col=0, elev=E_GROUND)
    c.grid(19, 23, MARSH_POOL, col=0, elev=E_GROUND)
    # scattered trees and stumps
    for (x, y) in ((9, 12), (15, 13), (6, 34), (12, 28), (20, 26), (2, 25), (11, 19)):
        c.put(x, y, BUSH, 1, 0)
    for (x, y) in ((14, 20), (8, 30), (18, 27)):
        c.put(x, y, LM['stump_marsh_0_0'], 1, E_GROUND)
    # the Gilded Scale wagon, stuck in the mud
    for (x, y) in rect(WAGON[0] - 1, WAGON[1] + 1, WAGON[0] + 3, WAGON[1] + 2):
        c.put(x, y, LM['mud_a_0_0' if (x + y) % 2 else 'mud_b_0_0'], 0, E_GROUND)
    c.grid(WAGON[0], WAGON[1], prop('cart', 3, 2), col=1, elev=E_GROUND)
    # route sign at the south end
    c.put(13, 40, SIGNPOST, 1, E_GROUND)
    for x in MIRE_SOUTH:
        c.fill(x, MIRE_H - 2, x, MIRE_H - 1, MARSH['c'])
    c.region(rect(MIRE_NORTH[0], 0, MIRE_NORTH[-1], MIRE_DRY), DIRT,
             joins=rect(MIRE_NORTH[0], -1, MIRE_NORTH[-1], -1))
    return c


# ---- Haymarket (Oldale Town slot) ----------------------------------------
HM = json.load(open(os.path.join(HERE, 'haymarket_ids.json')))['ids']
# The town is drawn on a 40x32 grid, then framed by a strip of forest and country
# road (HAY_ORIGIN is where the town grid sits in the final map). The frame keeps
# Haymarket's own tiles out of the band the game draws across each seam: within
# about 7 rows or 8 columns of a seam, a neighbour's tiles are drawn with the
# tileset of whichever map you stand in, so they must be primary (General) tiles.
HAY_W, HAY_H = 40, 32
HAY_ORIGIN = (8, 4)
HAY_FULL_W, HAY_FULL_H = 48, 39
HAY_NORTH = (19, 20, 21, 22)   # Route 103 (columns 8-11 there)
HAY_SOUTH = (19, 20)           # the Mire Road
HAY_WEST = (14, 15)            # Route 102 (rows 10-11 there)
PAVE = {'tl': 0x208, 't': 0x209, 'tr': 0x20A, 'l': 0x210, 'c': 0x211, 'r': 0x212,
        'bl': 0x218, 'b': 0x219, 'br': 0x21A}

# Buildings: (top-left, door offset)
HAY_MAGISTRATE = (4, 4)    # Slateport fan club 5x5, door +2,+4
HAY_PC = (10, 4)           # 4x4, door +1,+3
HAY_PAVILION = (18, 3)     # gilded Battle Tent dome 5x5, door +2,+4
HAY_MART = (25, 4)         # 4x4, door +1,+3
HAY_COUNTING = (30, 4)     # Slateport harbour building 7x6, door +3,+5
HAY_MARKET = (12, 11)      # Slateport market enclosure 14x10
HAY_PENN = (4, 21)         # Slateport name rater's house 4x4, door +1,+3
HAY_GRANARY = (29, 20)     # Stern's shipyard 8x7, door +2,+6


def door(pos, off):
    return (pos[0] + off[0], pos[1] + off[1])


def hay(x, y):
    """Town-grid coordinates -> final map coordinates."""
    return (x + HAY_ORIGIN[0], y + HAY_ORIGIN[1])


def fix_forest_edges(c):
    """A tree bottom uses the grass-edge variant unless another tree top sits below it."""
    for y in range(c.h):
        for x in range(c.w):
            v = c.get(x, y)
            mt = v & 0x3FF
            if mt in (0x1DC, 0x1DD, 0x1E4, 0x1E5):
                below = c.get(x, y + 1) & 0x3FF if y + 1 < c.h else 0x1D4
                left = mt in (0x1DC, 0x1E4)
                if below in (0x1D4, 0x1D5):
                    mt = 0x1DC if left else 0x1DD
                else:
                    mt = 0x1E4 if left else 0x1E5
                c.set(x, y, (v & ~0x3FF) | mt)


def haymarket():
    ox, oy = HAY_ORIGIN
    c = Canvas(HAY_FULL_W, HAY_FULL_H, block(GRASS))
    c.forest(0, 0, HAY_FULL_W - 1, HAY_FULL_H - 1)
    c.stamp(haymarket_town(), 0, 0, HAY_W, HAY_H, ox, oy)
    # country roads out of town (General tiles only, see above)
    north = (HAY_NORTH[0] + ox, HAY_NORTH[-1] + ox)
    south = (HAY_SOUTH[0] + ox, HAY_SOUTH[-1] + ox)
    west = (HAY_WEST[0] + oy, HAY_WEST[-1] + oy)
    roads = set(rect(north[0], 0, north[1], oy + 2))
    roads |= set(rect(south[0], oy + 28, south[1], HAY_FULL_H - 1))
    roads |= set(rect(0, west[0], ox + 1, west[1]))
    # the roads run into the paved streets, and the south one on into the Mire Road's
    # (Routes 102 and 103 have no road at the seam, so those two end in a rounded cap)
    joins = set(rect(north[0], oy + 3, north[1], oy + 3)) | set(rect(ox + 2, west[0], ox + 2, west[1]))
    joins |= set(rect(south[0], oy + 27, south[1], oy + 27)) | set(rect(south[0], HAY_FULL_H, south[1], HAY_FULL_H))
    c.region(roads, DIRT, joins=joins)
    fix_forest_edges(c)
    return c


def haymarket_town():
    c = Canvas(HAY_W, HAY_H, block(GRASS))
    c.forest(0, 0, 39, 1)
    c.forest(0, 30, 39, 31)
    c.forest(0, 2, 1, 29)
    c.forest(38, 2, 39, 29)
    for x in HAY_NORTH:
        c.fill(x, 0, x, 1, GRASS)
    for x in HAY_SOUTH:
        c.fill(x, 30, x, 31, GRASS)
    for y in HAY_WEST:
        c.fill(0, y, 1, y, GRASS)

    # paved streets
    streets = set(rect(19, 3, 22, 10)) | set(rect(2, 9, 37, 10)) | set(rect(2, 13, 11, 16))
    streets |= set(rect(19, 21, 22, 27)) | set(rect(6, 25, 28, 27)) | set(rect(26, 11, 28, 27))
    streets |= set(rect(6, 17, 8, 24)) | set(rect(29, 27, 34, 28))
    c.region(streets, PAVE)

    # buildings
    c.stamp(SLATEPORT, 2, 22, 5, 5, *HAY_MAGISTRATE)
    raw(c, HAY_PC[0], HAY_PC[1], PC)
    c.stamp(SLATEPORT, 8, 8, 5, 5, *HAY_PAVILION)
    for dy in range(5):
        for dx in range(5):
            v = c.get(HAY_PAVILION[0] + dx, HAY_PAVILION[1] + dy)
            c.set(HAY_PAVILION[0] + dx, HAY_PAVILION[1] + dy, (v & ~0x3FF) | HM['gilt_pavilion_%d_%d' % (dx, dy)])
    raw(c, HAY_MART[0], HAY_MART[1], MART)
    c.stamp(SLATEPORT, 25, 7, 7, 6, *HAY_COUNTING)
    c.stamp(SLATEPORT, 1, 30, 14, 10, *HAY_MARKET)
    mx, my = HAY_MARKET
    c.put(mx, my + 10, 0x141, 1, 0)
    for x in range(mx + 1, mx + 13):
        c.put(x, my + 10, 0x149, 1, 0)
    c.put(mx + 13, my + 10, 0x14A, 1, 0)
    c.stamp(SLATEPORT, 4, 16, 4, 4, *HAY_PENN)
    c.stamp(SLATEPORT, 24, 32, 8, 7, *HAY_GRANARY)

    # flowers by the Pavilion, trees along the streets
    for (x, y) in ((17, 6), (17, 7), (23, 6), (23, 7), (10, 8), (13, 8), (25, 8), (28, 8)):
        c.put(x, y, FLOWERS, 0, E_GROUND)
    for (x, y) in ((2, 2), (37, 2), (2, 19), (2, 28), (37, 18), (37, 29), (9, 21), (14, 24), (24, 24)):
        c.put(x, y, BUSH, 1, 0)
    c.put(23, 22, SIGNPOST, 1, E_GROUND)
    c.put(23, 8, SIGNPOST, 1, E_GROUND)
    return c


# ---- the Gilt Pavilion (Corwin's palace) ----------------------------------
# Built from vanilla Petalburg Gym rooms, 9 wide, set into a 13-wide hall with
# spectator boxes (the galleries) down both sides. From the door up:
#   lobby -> call row A -> lot row A -> room A -> lot row B -> room B -> lot row C -> throne
# A call row is where the auctioneer calls a lot; the lot row above it is a row of
# numbered gold plates (gTileset_GiltPavilion) and only the called plate lets you on.
# The scripts are in data/maps/Haymarket_GiltPavilion/scripts.inc.
PETALBURG_GYM = Layout.load(path('data/layouts/PetalburgCity_Gym/map.bin'), 9)
LOTS = json.load(open(os.path.join(os.path.dirname(__file__), 'gilt_pavilion_ids.json')))['ids']
PAVILION_W, PAVILION_H = 13, 27
PAV_HALL_X = 2                       # gym column 0 lands here
PAV_THRONE = (0, 0, 7)               # (first gym row, first map row, rows)
PAV_ROOM_B = (16, 8, 5)              # orange mats
PAV_ROOM_A = (55, 14, 5)             # blue mats
PAV_LOBBY = (106, 21, 6)
PAV_CALL_ROW_A = 20
# lot rows: map row -> (lot numbers for x = 3..9, the called lot)
PAV_LOT_ROWS = {19: ([3, 11, 5, 9, 7, 1, 15], 7),
                13: ([2, 12, 8, 14, 19, 6, 10], 12),
                7: ([13, 17, 4, 18, 16, 21, 20], 20)}
BOX = [[0x26B, 0x26C], [0x273, 0x274]]
STATUE_TOP, STATUE_BASE, VOID, WOOD = 0x240, 0x248, 0x001, 0x201


def gilt_pavilion():
    c = Canvas(PAVILION_W, PAVILION_H)
    gym = lambda x, y: PETALBURG_GYM.get(x, y)
    hall = lambda src, y, rows: [c.set(PAV_HALL_X + x, y + r, gym(x, src + r))
                                 for r in range(rows) for x in range(9)]
    # throne room, widened to the full 13 columns
    src, y0, rows = PAV_THRONE
    hall(src, y0, rows)
    for x in (0, 1, 11):
        c.set(x, 0, gym(3, 0))
    c.set(0, 0, gym(0, 0))
    c.set(12, 0, gym(8, 0))
    for r in range(1, rows):
        c.set(0, r, gym(0, r))
        c.set(1, r, gym(8, r))
        c.set(11, r, gym(8, r))
        c.set(12, r, gym(8, r))
    for x in (1, 2, 11, 12):
        c.set(x, 1, gym(8, 3))
    c.set(2, 2, gym(8, 3))
    for r in range(3, rows):
        c.set(2, r, gym(8, r))
    # the two mat rooms, each a call row on top of its mats
    for src, y0, rows in (PAV_ROOM_B, PAV_ROOM_A):
        hall(src, y0, rows)
    for x in range(PAV_HALL_X, PAV_HALL_X + 9):
        c.set(x, PAV_CALL_ROW_A, gym(1 if x > PAV_HALL_X else 0, PAV_ROOM_A[0] + 4))
    # lobby, with the void either side
    src, y0, rows = PAV_LOBBY
    hall(src, y0, rows)
    for y in range(y0, y0 + rows):
        for x in (0, 1, 11, 12):
            c.set(x, y, block(VOID, 1, 0))
    # galleries: spectator boxes from the first lot row down to call row A
    for y in range(7, PAV_CALL_ROW_A + 1, 2):
        for gx in (0, 11):
            for dy in range(2):
                for dx in range(2):
                    c.set(gx + dx, y + dy, block(BOX[dy][dx], 1, 0))
    # lot rows, closed at each end by a statue
    for y, (nums, _) in PAV_LOT_ROWS.items():
        for i, n in enumerate(nums):
            c.put(3 + i, y, LOTS['lot_%d' % n])
        for x in (2, 10):
            c.set(x, y, block(STATUE_BASE, 1, 0))
            c.set(x, y - 1, block(STATUE_TOP, 1, 0))
    return c


# ---- the granary ------------------------------------------------------------
# Stern's Shipyard 1F with crate stacks added so the sack puzzle and the foreman
# can't be walked around: the only way east is a one-tile gap at (13,10) that a
# sack (a Strength boulder) fills, and the only way up to the scale room is a
# one-tile aisle at x=17 beside the foreman.
STERNS_1F = Layout.load(path('data/layouts/SlateportCity_SternsShipyard_1F/map.bin'), 21)
CRATE_TOP, CRATE, CRATE_BOTTOM = 0x21F, 0x227, 0x237
GRANARY_CRATES = [(16, 4), (16, 5), (18, 4), (18, 5),        # aisle to the scale room
                  (14, 9), (15, 9), (14, 11), (15, 11),      # lane the sack is pushed along
                  (13, 11), (13, 12), (13, 13)]              # wall below the gap
GRANARY_CRATE_TOPS = [(16, 3), (18, 3)]


def granary():
    c = Canvas(21, 15)
    c.b = list(STERNS_1F.b)
    for x, y in GRANARY_CRATES:
        c.set(x, y, block(CRATE, 1, 0))
    c.set(13, 14, block(CRATE_BOTTOM, 1, 0))
    for x, y in GRANARY_CRATE_TOPS:
        c.set(x, y, block(CRATE_TOP, 0, 3))
    return c


# ---- the Ranger's Shed ------------------------------------------------------
# Mr. Briney's cottage (wood beams, clay pots, shelves) with the mounted trophy from
# the Fossil Maniac's house hung on the back wall. Same tilesets, so ids carry over.
BRINEYS = Layout.load(path('data/layouts/Route104_MrBrineysHouse/map.bin'), 12)
FOSSIL_HOUSE = Layout.load(path('data/layouts/Route114_FossilManiacsHouse/map.bin'), 10)


def rangers_shed():
    c = Canvas(12, 9)
    c.b = list(BRINEYS.b)
    for dy in range(3):
        for dx in range(3):
            c.set(4 + dx, dy, FOSSIL_HOUSE.get(3 + dx, dy))
    c.set(6, 2, BRINEYS.get(5, 2))
    return c


def main():
    for s in range(5):
        save_layout(lowmere(s), 'LowmereStage%d' % s, 'LAYOUT_LOWMERE_STAGE%d' % s, FOREST_BORDER)
    print('wrote Lowmere stages 0-4')
    save_layout(mire_road(), 'Route101', 'LAYOUT_ROUTE101', FOREST_BORDER)
    save_layout(haymarket(), 'OldaleTown', 'LAYOUT_OLDALE_TOWN', FOREST_BORDER, 'gTileset_Haymarket')
    print('wrote the Mire Road and Haymarket')
    save_layout(granary(), 'Haymarket_Granary', 'LAYOUT_HAYMARKET_GRANARY',
                Layout.load(path('data/layouts/SlateportCity_SternsShipyard_1F/border.bin'), 2),
                'gTileset_Facility', 'gTileset_General')
    print('wrote the granary')
    save_layout(rangers_shed(), 'Lowmere_RangersShed', 'LAYOUT_LOWMERE_RANGERS_SHED',
                Layout.load(path('data/layouts/Route104_MrBrineysHouse/border.bin'), 2),
                'gTileset_GenericBuilding', 'gTileset_Building')
    print("wrote the Ranger's Shed")
    pav = gilt_pavilion()
    save_layout(pav, 'Haymarket_GiltPavilion', 'LAYOUT_HAYMARKET_GILT_PAVILION',
                border([[0x208, 0x208], [0x208, 0x208]]), 'gTileset_GiltPavilion', 'gTileset_Building')


if __name__ == '__main__':
    main()
