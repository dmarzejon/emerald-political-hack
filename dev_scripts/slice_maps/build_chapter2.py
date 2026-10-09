#!/usr/bin/env python3
"""Generate the chapter 2 layouts: the Orchard Road, Thornfield and Isolde's palace garden.

Run build_tileset.py first, then this, from the repo root:
    python3 dev_scripts/slice_maps/build_tileset.py
    python3 dev_scripts/slice_maps/build_chapter2.py

The Orchard Road and Thornfield are vanilla Route 102 and Petalburg City with props
added, read from the untouched vanilla layouts and written to new layouts, so this
script can be re-run safely. All three use gTileset_Lowmere, which is Petalburg's
tileset with the Lowmere props appended, so every vanilla Petalburg metatile still
draws the same.
"""
from build_maps import (BERRY_SOIL, BUSH, DIRT, FLOWERS, FOREST_BORDER, GRASS, LM, SIGNPOST, Canvas, fortree, house,
                        prop, rect, save_layout)
from gfxlib import Layout, block, path

E_GROUND = 3
VANILLA_ROUTE102 = Layout.load(path('data/layouts/Route102/map.bin'), 50)
VANILLA_PETALBURG = Layout.load(path('data/layouts/PetalburgCity/map.bin'), 30)


def from_vanilla(src):
    c = Canvas(src.w, src.h)
    c.b = list(src.b)
    return c


# ---- the Orchard Road (Route 102 slot) -----------------------------------
ROAD_COTTAGE = (11, 2)                  # seized tenant cottage 4x4, boarded door +1,+3
ROAD_ORCHARD = [(19, 2), (21, 2), (23, 2), (27, 3), (26, 2)]
# The camp stays 8+ columns clear of the Haymarket seam (x 42-49), where only General tiles may go.
# Row 14 stays clear: it is the only way between the two halves of the road.
ROAD_TENTS = [(36, 12), (38, 12)]       # 2x2 each
ROAD_FIRE = (41, 13)
ROAD_CART = (37, 15)                    # 3x2, the tenant's cart
# The warden stands at (47,10); a tree beside him closes the road's east end.
ROAD_WARDEN_TREE = (47, 11)


def orchard_road():
    c = from_vanilla(VANILLA_ROUTE102)
    house(c, *ROAD_COTTAGE, state='boarded')
    # the cottage's fenced plot, gone to seed
    for x in (8, 9, 10):
        c.put(x, 6, LM['fence_broken_0_0' if x == 9 else 'fence_full_0_0'], 1, E_GROUND)
    for (x, y) in ROAD_ORCHARD:
        c.put(x, y, BUSH, 1, 0)
    # the evicted farmers' camp by the Haymarket end of the road
    for (x, y) in ROAD_TENTS:
        c.grid(x, y, prop('tent', 2, 2), col=1, elev=E_GROUND)
    c.put(*ROAD_FIRE, LM['campfire_0_0'], 1, E_GROUND)
    c.grid(*ROAD_CART, prop('cart', 3, 2), col=1, elev=E_GROUND)
    c.put(40, 12, LM['sacks_0_0'], 1, E_GROUND)
    c.put(*ROAD_WARDEN_TREE, BUSH, 1, 0)
    # Thornfield's east edge at row 16 is the Willows' cottage now, so close the road's side too
    c.put(0, 6, BUSH, 1, 0)
    return c


# ---- Thornfield (Petalburg City slot) ------------------------------------
TF_PALACE_DOOR = (15, 8)        # vanilla gym door
TF_GROVE_PATH = 18              # the column behind the palace that leads to the grove gate
TF_GROVE_GATE = (18, 2)
TF_PALACE_SIGN = (17, 10)
TF_GREENHOUSE = (23, 21)        # 3x3, in the head gardener's vegetable plot; door +1,+2
TF_WILLOWS = (26, 13)           # the Willows' cottage, 4x4, door +1,+3 = (27,16), by the east road in
TF_BEANS = rect(23, 13, 24, 15)  # their bean field
TF_TERRACES = rect(14, 25, 24, 27)


def thornfield():
    c = from_vanilla(VANILLA_PETALBURG)
    # a path squeezed between the palace and the pond, up to the sealed grove's gate
    c.region(rect(TF_GROVE_PATH, TF_GROVE_GATE[1] + 1, TF_GROVE_PATH, 8), DIRT,
             joins=[(TF_GROVE_PATH - 1, 8)])
    c.put(*TF_GROVE_GATE, LM['fence_full_0_0'], 1, E_GROUND)
    # no more GYM: the plate left of the door becomes a second window, the gym sign a plain one
    c.set(12, 7, VANILLA_PETALBURG.get(16, 7))
    c.set(13, 7, VANILLA_PETALBURG.get(17, 7))
    c.put(*TF_PALACE_SIGN, SIGNPOST, 1, E_GROUND)
    # flower beds either side of the palace door
    for (x, y) in ((12, 8), (13, 8), (12, 9), (17, 9)):
        c.put(x, y, FLOWERS, 0, E_GROUND)
    # the Willows' cottage and bean field, the first thing you see coming in from the Orchard Road
    house(c, *TF_WILLOWS, state='weathered')
    for dx in range(4):     # nothing to walk behind: the roof backs onto the Mart and the trees
        c.set(TF_WILLOWS[0] + dx, TF_WILLOWS[1], c.get(TF_WILLOWS[0] + dx, TF_WILLOWS[1]) | (1 << 10))
    for (x, y) in TF_BEANS:
        c.put(x, y, BERRY_SOIL if y == 14 else fortree(0x294), 0, E_GROUND)
    # the steward's office (Wally's house) has no name board
    c.put(8, 9, GRASS, 0, E_GROUND)
    # seized farmland: the gardener's vegetable plot is a glasshouse, the south yard flower terraces
    c.grid(*TF_GREENHOUSE, prop('greenhouse', 3, 3), col=1, elev=E_GROUND)
    for (x, y) in TF_TERRACES:
        if y != 26:
            c.put(x, y, FLOWERS, 0, E_GROUND)
    for (x, y) in ((14, 26), (24, 26)):
        c.put(x, y, LM['notice_0_0'], 1, E_GROUND)
    return c


# ---- Isolde's palace garden (replaces the Petalburg Gym) ------------------
PAL_W, PAL_H = 16, 22
PAL_DAIS = rect(4, 2, 11, 4)
PAL_ISOLDE = (7, 3)
PAL_EXITS = ((7, 21), (8, 21))
# Hedge rows (y, first x, last x) and planters that narrow each lane to one row.
PAL_HEDGES = [(17, 2, 11), (14, 4, 13), (11, 2, 11), (8, 4, 13)]
PAL_PLANTERS = [(15, 4, 11), (12, 4, 11), (9, 4, 11)]
# Gardeners: (x, y, facing). Each one looks down the only lane through its terrace.
PAL_GARDENERS = [(2, 16, 'right'), (13, 13, 'left'), (2, 10, 'right')]


def palace():
    c = Canvas(PAL_W, PAL_H, block(GRASS))
    c.forest(0, 0, PAL_W - 1, 1)
    # the glass hall behind the dais that gives the palace its name, the Glasshouse
    for x in range(2, PAL_W - 2):
        c.grid(x, 0, prop('glass_wall', 1, 2), col=1, elev=0)
    c.forest(0, 2, 1, PAL_H - 1)
    c.forest(PAL_W - 2, 2, PAL_W - 1, PAL_H - 1)
    for x in range(2, PAL_W - 2):
        c.put(x, PAL_H - 1, BUSH, 1, 0)
    for (y, x0, x1) in PAL_HEDGES:
        c.fill(x0, y, x1, y, BUSH, 1, 0)
    for (y, x0, x1) in PAL_PLANTERS:
        c.fill(x0, y, x1, y, BUSH, 1, 0)
        c.fill(x0 + 1, y, x1 - 1, y, FLOWERS, 1, E_GROUND)
    # Isolde's terrace: a flagstone dais in a bed of flowers
    c.fill(2, 2, PAL_W - 3, 6, FLOWERS, 0, E_GROUND)
    for (x, y) in PAL_DAIS:
        c.put(x, y, LM['flagstones_0_0'], 0, E_GROUND)
    for x in range(5, 11):
        c.put(x, 5, LM['flagstones_0_0'], 0, E_GROUND)
    # entrance court and the path in
    for (x, _) in PAL_EXITS:
        for y in range(18, PAL_H - 1):
            c.put(x, y, LM['flagstones_0_0'], 0, E_GROUND)
    for (x, y) in PAL_EXITS:
        c.put(x, y, LM['exit_mat_0_0'], 0, E_GROUND)
    return c


def main():
    save_layout(orchard_road(), 'OrchardRoad', 'LAYOUT_ORCHARD_ROAD',
                Layout.load(path('data/layouts/Route102/border.bin'), 2))
    save_layout(thornfield(), 'Thornfield', 'LAYOUT_THORNFIELD',
                Layout.load(path('data/layouts/PetalburgCity/border.bin'), 2))
    save_layout(palace(), 'Thornfield_Palace', 'LAYOUT_THORNFIELD_PALACE', FOREST_BORDER)
    print('wrote the Orchard Road, Thornfield and the palace garden')


if __name__ == '__main__':
    main()
