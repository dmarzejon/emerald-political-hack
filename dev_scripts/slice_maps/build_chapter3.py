#!/usr/bin/env python3
"""Generate the chapter 3 layouts: Cragholt and the road to it.

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


def main():
    c = cragholt()
    d = path('data/layouts', 'Cragholt')
    c.save(os.path.join(d, 'map.bin'))
    Layout.load(path('data/layouts/RustboroCity/border.bin'), 2).save(os.path.join(d, 'border.bin'))
    register_layout('LAYOUT_CRAGHOLT', 'Cragholt', c.w, c.h, 'gTileset_Cragholt')
    layouts = json.load(open(path('data/layouts/layouts.json')))['layouts']
    for l in layouts:
        if l.get('id') in RETILED:
            register_layout(l['id'], l['name'][:-len('_Layout')], l['width'], l['height'], 'gTileset_Cragholt')
    print('wrote Cragholt and moved the road to the Cragholt tileset')


if __name__ == '__main__':
    main()
