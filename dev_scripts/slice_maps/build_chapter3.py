#!/usr/bin/env python3
"""Generate the chapter 3 layouts: Cragholt, the road to it and the collapsed mine.

Run build_cragholt_tileset.py first, then this, from the repo root:
    python3 dev_scripts/slice_maps/build_cragholt_tileset.py
    python3 dev_scripts/slice_maps/build_chapter3.py

Cragholt is vanilla Rustboro City with a mine railway, ore carts and ore heaps added,
read from the untouched vanilla layout and written to a new layout, so this script can
be re-run safely. Route 104, Petalburg Woods and Route 116 keep their vanilla blocks
and only move to gTileset_Cragholt, which is Rustboro's tileset with the mine props
appended, so every vanilla Rustboro metatile still draws the same.
"""
import json
import os

from build_maps import Canvas, register_layout
from gfxlib import Layout, path

HERE = os.path.dirname(__file__)
CR = json.load(open(os.path.join(HERE, 'cragholt_ids.json')))['ids']
E_TOWN = 3                      # Rustboro's paving sits at elevation 3
PAVING = 0x2BB
SIGNBOARD = 0x003               # General's signboard; Rustboro sets it at elevation 0
VANILLA_RUSTBORO = Layout.load(path('data/layouts/RustboroCity/map.bin'), 40)
VANILLA_RUSTURF = Layout.load(path('data/layouts/RusturfTunnel/map.bin'), 36)

# Layouts that keep their blocks and only switch to the Cragholt tileset.
RETILED = ['LAYOUT_ROUTE104', 'LAYOUT_PETALBURG_WOODS', 'LAYOUT_ROUTE116']


def from_vanilla(src):
    c = Canvas(src.w, src.h)
    c.b = list(src.b)
    return c


# ---- Cragholt (Rustboro City slot) ----------------------------------------
# The ore line runs in from the mine road (Route 116, east) along the north street.
CG_ORE_LINE = (22, 35, 11)          # x0, x1, y
CG_ORE_CARTS = [(26, 11), (33, 11)]
# A siding in the Chancellery ore office's yard (Devon Corp), below its lamps.
CG_YARD_LINE = (7, 17, 22)
CG_YARD_CART = (10, 22)
CG_YARD_ORE = [(7, 16), (7, 17), (16, 21)]
# The fountain square becomes the quota board, with ore heaped either side.
CG_FOUNTAIN = (27, 38)              # 3x3
CG_QUOTA_BOARD = (28, 39)
CG_QUOTA_ORE = [(27, 39), (29, 39), (27, 40), (29, 40)]


def rail_line(c, x0, x1, y, carts):
    for x in range(x0, x1 + 1):
        if (x, y) in carts:
            c.put(x, y, CR['cart_0_0'], 1, E_TOWN)
        else:
            c.put(x, y, CR['rail_h_0_0'], 0, E_TOWN)


def cragholt():
    c = from_vanilla(VANILLA_RUSTBORO)
    rail_line(c, *CG_ORE_LINE, CG_ORE_CARTS)
    rail_line(c, *CG_YARD_LINE, [CG_YARD_CART])
    for (x, y) in CG_YARD_ORE + CG_QUOTA_ORE:
        c.put(x, y, CR['ore_0_0'], 1, E_TOWN)
    fx, fy = CG_FOUNTAIN
    for y in range(fy, fy + 3):
        for x in range(fx, fx + 3):
            if (x, y) not in CG_QUOTA_ORE:
                c.put(x, y, PAVING, 0, E_TOWN)
    c.put(*CG_QUOTA_BOARD, SIGNBOARD, 1, 0)
    return c


# ---- the collapsed mine (Rusturf Tunnel slot) -----------------------------
E_MINE = 3
MINE_FLOOR = 0x201
# The haulage way: the long corridor from the west entrance, rows 4-5.
MN_RAIL = (4, 19, 5)
MN_CART = (8, 5)
MN_ORE = [(5, 4), (13, 4)]
MN_TIMBERS = [10, 14, 18]           # props on the wall faces above (row 3) and below (row 6)
# The fall: rubble across the whole corridor, with the Chancellery crate in front of it.
MN_RUBBLE = (20, 4)                 # 2x2, covers (20-21, 4-5)
MN_CRATE = (19, 4)


def collapsed_mine():
    c = from_vanilla(VANILLA_RUSTURF)
    x0, x1, y = MN_RAIL
    for x in range(x0, x1 + 1):
        if (x, y) == MN_CART:
            c.put(x, y, CR['mine_cart_0_0'], 1, E_MINE)
        else:
            c.put(x, y, CR['mine_rail_h_0_0'], 0, E_MINE)
    for (x, y) in MN_ORE:
        c.put(x, y, CR['mine_ore_0_0'], 1, E_MINE)
    for x in MN_TIMBERS:
        c.put(x, 3, CR['mine_timber_face_0_0'], 1, E_MINE)
        c.put(x, 6, CR['mine_timber_top_0_0'], 1, E_MINE)
    rx, ry = MN_RUBBLE
    for dy in range(2):
        for dx in range(2):
            c.put(rx + dx, ry + dy, CR['mine_rubble_%d_%d' % (dx, dy)], 1, E_MINE)
    c.put(*MN_CRATE, CR['mine_crate_0_0'], 1, E_MINE)
    return c


def save(c, name, layout_id, border_from, secondary):
    d = path('data/layouts', name)
    c.save(os.path.join(d, 'map.bin'))
    Layout.load(path('data/layouts', border_from, 'border.bin'), 2).save(os.path.join(d, 'border.bin'))
    register_layout(layout_id, name, c.w, c.h, secondary)


def main():
    save(cragholt(), 'Cragholt', 'LAYOUT_CRAGHOLT', 'RustboroCity', 'gTileset_Cragholt')
    save(collapsed_mine(), 'Cragholt_Mine', 'LAYOUT_CRAGHOLT_MINE', 'RusturfTunnel', 'gTileset_CragholtMine')
    layouts = json.load(open(path('data/layouts/layouts.json')))['layouts']
    for l in layouts:
        if l.get('id') in RETILED:
            register_layout(l['id'], l['name'][:-len('_Layout')], l['width'], l['height'], 'gTileset_Cragholt')
    print('wrote Cragholt and the collapsed mine, and moved the road to the Cragholt tileset')


if __name__ == '__main__':
    main()
