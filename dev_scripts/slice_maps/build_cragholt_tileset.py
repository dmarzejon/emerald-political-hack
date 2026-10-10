#!/usr/bin/env python3
"""Build the Cragholt secondary tilesets: the town (data/tilesets/secondary/cragholt)
and the collapsed mine (data/tilesets/secondary/cragholt_mine).

Cragholt = Rustboro (unchanged, so every Rustboro metatile id is still valid)
         + mine-rail, ore-cart and ore-pile props drawn in palette slot 6,
           which Rustboro leaves unused

Rustboro already fills 498 of the 512 secondary tiles, so the props are drawn to
share tiles: a rail is one tile repeated and flipped, the cart and the ore pile
are left-right mirrors. Each prop is registered on both town paving and road dirt;
that costs metatiles but no tiles.

Cragholt mine = Rusturf Tunnel (unchanged) + the same rails, carts and ore, plus
               timber props, rubble and a crate, in palette slot 7

Writes the tileset files plus cragholt_ids.json (name -> metatile id). Run from the
repo root:  python3 dev_scripts/slice_maps/build_cragholt_tileset.py
"""
import json
import os

from PIL import Image, ImageDraw

from build_tileset import Builder
from gfxlib import path

OUT = path('data/tilesets/secondary/cragholt')
MINE_OUT = path('data/tilesets/secondary/cragholt_mine')
IDS = os.path.join(os.path.dirname(__file__), 'cragholt_ids.json')
PROP_SLOT = 6

MINE_PAL = [
    (24, 41, 82),     # 0 transparent
    (238, 230, 213),  # 1 highlight
    (197, 156, 98),   # 2 light wood
    (156, 115, 74),   # 3 wood
    (106, 74, 49),    # 4 dark wood
    (57, 41, 41),     # 5 outline
    (205, 205, 205),  # 6 bright iron
    (148, 148, 156),  # 7 iron
    (90, 90, 98),     # 8 dark iron
    (172, 98, 57),    # 9 rust
    (123, 65, 41),    # 10 dark rust
    (82, 82, 90),     # 11 coal light
    (41, 41, 49),     # 12 coal
    (238, 189, 74),   # 13 ore glint
    (164, 139, 115),  # 14 rock light
    (115, 98, 82),    # 15 rock
]

PAVING = 0x2BB      # Rustboro's town paving
DIRT = 0x121        # General road dirt (Route 104)
MINE_FLOOR = 0x201  # Rusturf Tunnel floor
MINE_WALL_FACE = 0x219
MINE_WALL_TOP = 0x209


def canvas(w, h):
    im = Image.new('P', (w, h), 0)
    im.putpalette([c for rgb in MINE_PAL for c in rgb])
    return im, ImageDraw.Draw(im)


def mirror(im):
    """Make the right half the mirror of the left, so both halves share tiles."""
    w, h = im.size
    px = im.load()
    for y in range(h):
        for x in range(w // 2):
            px[w - 1 - x, y] = px[x, y]
    return im


def vmirror(im):
    w, h = im.size
    px = im.load()
    for y in range(h // 2):
        for x in range(w):
            px[x, h - 1 - y] = px[x, y]
    return im


def draw_rail_h():
    """Two rails on sleepers every 8 px: every quadrant is the same tile or its flip."""
    im, d = canvas(16, 16)
    for x0 in (2, 10):
        d.rectangle([x0, 2, x0 + 3, 13], fill=4)
        d.line([x0, 2, x0, 13], fill=3)
    for y in (4, 11):
        d.line([0, y, 15, y], fill=7)
    d.line([0, 3, 15, 3], fill=6)
    d.line([0, 12, 15, 12], fill=6)
    return vmirror(mirror(im))


def draw_rail_v():
    im = draw_rail_h().transpose(Image.Transpose.TRANSPOSE)
    im.putpalette([c for rgb in MINE_PAL for c in rgb])
    return im


def draw_cart():
    """A rusty ore cart on the rails, heaped with ore."""
    im = draw_rail_h()
    d = ImageDraw.Draw(im)
    d.ellipse([3, 2, 12, 8], fill=11, outline=5)
    d.line([5, 3, 7, 3], fill=14)
    d.point((6, 5), fill=13)
    d.point((4, 6), fill=12)
    d.polygon([(0, 6), (15, 6), (14, 13), (1, 13)], fill=9, outline=5)
    d.line([1, 7, 14, 7], fill=1)
    d.line([2, 10, 13, 10], fill=10)
    d.rectangle([3, 13, 5, 15], fill=5)
    d.point((4, 14), fill=7)
    return mirror(im)


def draw_ore():
    """A heap of broken ore and rock."""
    im, d = canvas(16, 16)
    d.polygon([(0, 15), (3, 7), (6, 3), (8, 2), (10, 3), (13, 7), (15, 15)], fill=15, outline=5)
    d.polygon([(3, 9), (6, 4), (8, 3), (9, 6), (6, 10)], fill=14)
    d.line([2, 12, 6, 11], fill=5)
    d.line([7, 8, 9, 12], fill=5)
    for (x, y) in ((5, 7), (4, 13), (7, 11), (6, 5)):
        d.point((x, y), fill=13)
    d.point((7, 3), fill=1)
    return mirror(im)


def draw_headframe():
    """The pithead: a 2x3 winding frame over the shaft, with the pit bell under the
    sheave. Mirrored left-right, and the post and shaft rows repeat, so the whole
    frame costs 8 tiles."""
    im, d = canvas(32, 48)
    # posts, plain and uniform from the beam down so their tile rows repeat
    for x0 in (2, 26):
        d.rectangle([x0, 4, x0 + 3, 47], fill=3)
        d.line([x0, 4, x0, 47], fill=2)
        d.line([x0 + 3, 4, x0 + 3, 47], fill=5)
    # the cross-beam and the sheave wheel
    d.rectangle([0, 3, 31, 7], fill=4, outline=5)
    d.line([1, 4, 30, 4], fill=3)
    d.ellipse([10, 0, 21, 11], fill=8, outline=5)
    d.ellipse([13, 3, 18, 8], fill=7, outline=5)
    # the pit bell hanging under it
    d.polygon([(12, 20), (14, 14), (17, 14), (19, 20)], fill=13, outline=5)
    d.line([15, 11, 15, 13], fill=5)
    d.line([16, 11, 16, 13], fill=5)
    d.point((15, 21), fill=5)
    d.point((16, 21), fill=5)
    # the winding ropes, uniform down to the shaft
    for x in (11, 20):
        d.line([x, 8, x, 31], fill=8)
    # the shaft: a timber collar round a black pit, symmetric top to bottom
    d.rectangle([6, 32, 25, 47], fill=4, outline=5)
    d.rectangle([9, 35, 22, 44], fill=12, outline=5)
    d.line([7, 33, 24, 33], fill=3)
    d.line([7, 46, 24, 46], fill=3)
    for x in (11, 20):
        d.line([x, 35, x, 44], fill=8)
    return vmirror_rows(mirror(im), 32, 48)


def vmirror_rows(im, y0, y1):
    """Mirror rows y0..y1-1 top to bottom, so a band's lower tiles flip its upper ones."""
    px = im.load()
    w = im.size[0]
    for i in range((y1 - y0) // 2):
        for x in range(w):
            px[x, y1 - 1 - i] = px[x, y0 + i]
    return im


def draw_timber():
    """A pit prop: a timber post with a cross-beam, set against the wall."""
    im, d = canvas(16, 16)
    d.rectangle([5, 0, 10, 15], fill=3, outline=5)
    d.line([6, 1, 6, 14], fill=2)
    d.rectangle([0, 1, 15, 4], fill=4, outline=5)
    d.line([1, 2, 14, 2], fill=3)
    for y in (6, 11):
        d.point((8, y), fill=5)
    return mirror(im)


def draw_rubble():
    """A 2x2 fall of rock, with a broken prop sticking out of it."""
    im, d = canvas(32, 32)
    d.polygon([(0, 31), (2, 18), (7, 10), (12, 5), (16, 3), (20, 5), (25, 10), (30, 18), (31, 31)], fill=15, outline=5)
    for box in ([3, 18, 12, 28], [10, 9, 20, 19], [19, 18, 28, 28], [12, 21, 20, 30]):
        d.ellipse(box, fill=14, outline=5)
    d.line([6, 22, 9, 25], fill=15)
    d.line([14, 12, 17, 16], fill=15)
    for (x, y) in ((8, 14), (5, 26), (15, 25), (13, 7)):
        d.point((x, y), fill=13)
    return mirror(im)


def draw_rubble_low():
    """A 2x1 bank of fallen rock, low enough to show the wall behind it."""
    im, d = canvas(32, 16)
    d.polygon([(0, 15), (2, 7), (8, 3), (16, 1), (24, 3), (30, 7), (31, 15)], fill=15, outline=5)
    for box in ([2, 7, 11, 15], [10, 3, 21, 12], [20, 7, 29, 15]):
        d.ellipse(box, fill=14, outline=5)
    for (x, y) in ((6, 10), (14, 6), (13, 13)):
        d.point((x, y), fill=13)
    return mirror(im)


def draw_rubble_small():
    """One tile of fallen rock."""
    im, d = canvas(16, 16)
    d.polygon([(0, 15), (2, 6), (8, 2), (13, 6), (15, 15)], fill=15, outline=5)
    d.ellipse([2, 6, 9, 14], fill=14, outline=5)
    d.ellipse([7, 4, 13, 11], fill=14, outline=5)
    d.point((5, 9), fill=13)
    d.point((9, 6), fill=1)
    return im


def draw_crate():
    """A Chancellery supply crate, bound with iron."""
    im, d = canvas(16, 16)
    d.rectangle([1, 3, 14, 14], fill=3, outline=5)
    d.rectangle([1, 3, 14, 5], fill=2, outline=5)
    for x in (4, 11):
        d.line([x, 6, x, 13], fill=8)
    d.line([2, 9, 13, 9], fill=4)
    return mirror(im)


def build_mine():
    b = Builder('rusturf_tunnel', 7, MINE_PAL)
    while not any(b.ts.tiles[-1]):
        b.ts.tiles.pop()
    b.prop(draw_rail_h(), MINE_FLOOR, 'rail_h')
    b.prop(draw_rail_v(), MINE_FLOOR, 'rail_v')
    b.prop(draw_cart(), MINE_FLOOR, 'cart')
    b.prop(draw_ore(), MINE_FLOOR, 'ore')
    b.prop(draw_crate(), MINE_FLOOR, 'crate')
    b.prop(draw_rubble(), MINE_FLOOR, 'rubble')
    b.prop(draw_timber(), MINE_WALL_FACE, 'timber_face')
    b.prop(draw_timber(), MINE_WALL_TOP, 'timber_top')
    b.prop(draw_rubble_low(), MINE_FLOOR, 'rubble_low')
    b.prop(draw_rubble_small(), MINE_FLOOR, 'rubble_small')
    assert len(b.ts.tiles) <= 512 and len(b.ts.metatiles) <= 512
    b.ts.save(MINE_OUT)
    print('cragholt mine tileset: %d tiles, %d metatiles' % (len(b.ts.tiles), len(b.ts.metatiles)))
    return {'mine_' + k: v for k, v in b.ids.items()}, len(b.ts.tiles)


def main():
    mine_ids, mine_tiles = build_mine()
    b = Builder('rustboro', PROP_SLOT, MINE_PAL)
    while not any(b.ts.tiles[-1]):      # tiles.png pads Rustboro's 498 tiles out to 512
        b.ts.tiles.pop()
    for ground, suffix in ((PAVING, ''), (DIRT, '_dirt')):
        b.prop(draw_rail_h(), ground, 'rail_h' + suffix)
        b.prop(draw_rail_v(), ground, 'rail_v' + suffix)
        b.prop(draw_cart(), ground, 'cart' + suffix)
        b.prop(draw_ore(), ground, 'ore' + suffix)
    b.prop(draw_headframe(), PAVING, 'headframe')

    ntiles = len(b.ts.tiles)
    nmeta = len(b.ts.metatiles)
    assert ntiles <= 512, ntiles
    assert nmeta <= 512, nmeta
    b.ts.save(OUT)
    ids = dict(b.ids)
    ids.update(mine_ids)
    json.dump({'num_tiles': ntiles, 'num_metatiles': nmeta, 'mine_num_tiles': mine_tiles,
               'ids': {k: v for k, v in sorted(ids.items())}}, open(IDS, 'w'), indent=1)
    print('cragholt tileset: %d tiles, %d metatiles' % (ntiles, nmeta))


if __name__ == '__main__':
    main()
