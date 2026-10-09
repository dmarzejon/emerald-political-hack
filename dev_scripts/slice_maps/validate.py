#!/usr/bin/env python3
"""Sanity-check the slice maps: can the player reach every door, NPC, sign and exit?

    python3 dev_scripts/slice_maps/validate.py

Walkable = no collision bit and not water (elevation 1). Every object event blocks its
tile, as it does in game. Starting points are the map's warps and connection edges.
Prints one line per problem and exits non-zero if there are any.
"""
import json
import sys
from collections import deque

from gfxlib import Layout, Tileset, path, tileset_dir_for

MAPS = ['LittlerootTown', 'Route101', 'OldaleTown', 'Haymarket_GiltPavilion', 'Haymarket_CountingHouse',
        'Haymarket_CountingHouse_BackRoom', 'Haymarket_Granary', 'Lowmere_Forge', 'Lowmere_ReevesHouse',
        'Lowmere_ReopenedHouse', 'OldaleTown_House1', 'OldaleTown_House2', 'LittlerootTown_MaysHouse_1F',
        'LittlerootTown_MaysHouse_2F', 'LittlerootTown_BrendansHouse_1F', 'LittlerootTown_ProfessorBirchsLab']
LOWMERE_STAGES = ['LAYOUT_LOWMERE_STAGE%d' % i for i in range(5)]
# Objects whose spot is meant to block (a guard on a door, a boulder) or that only appear in a cutscene.
BLOCKING_OK = {'LOCALID_HAYMARKET_PAVILION_GUARD'}

# The FRLG overworld sprites draw nothing in this build, so a person using one is invisible.
# These vanilla objects are hidden for good but still named by vanilla scripts.
FRLG_GFX_OK = {'RivalsHouse_1F_EventScript_RivalSibling'}


def frlg_gfx():
    names, on = set(), False
    for line in open(path('include/constants/event_objects.h')):
        name = line.strip().split(',')[0]
        if name == 'OBJ_EVENT_GFX_RED_NORMAL':
            on = True
        if name == 'NUM_OBJ_EVENT_GFX':
            break
        if on and name.startswith('OBJ_EVENT_GFX_'):
            names.add(name)
    return names


FRLG_GFX = frlg_gfx()

# How far the game draws into a neighbouring map. Tiles this close to a seam are drawn with
# whichever map you are standing in, so they must look the same under both maps' tilesets.
SEAM_DEPTH_ROWS, SEAM_DEPTH_COLS, SEAM_SPREAD = 7, 8, 8
_tilesets = {}

LAYOUTS = {l['id']: l for l in json.load(open(path('data/layouts/layouts.json')))['layouts'] if 'id' in l}


def load_layout(lid):
    l = LAYOUTS[lid]
    return Layout.load(path(l['blockdata_filepath']), l['width'])


def walkable(lay, x, y):
    if not (0 <= x < lay.w and 0 <= y < lay.h):
        return False
    v = lay.get(x, y)
    return ((v >> 10) & 3) == 0 and (v >> 12) != 1


def check(mapname, layout_id, problems):
    m = json.load(open(path('data/maps', mapname, 'map.json')))
    lay = load_layout(layout_id)
    tag = '%s (%s)' % (mapname, layout_id)
    objs = {(o['x'], o['y']): o for o in m['object_events']}
    # cutscene-only objects (no script) are hidden in normal play
    # Strength boulders (the granary sacks) can be pushed out of the way.
    blocked = {p for p, o in objs.items()
               if o['script'] != '0x0' and o['graphics_id'] != 'OBJ_EVENT_GFX_PUSHABLE_BOULDER'}

    starts = []
    for w in m['warp_events']:
        x, y = w['x'], w['y']
        if walkable(lay, x, y):
            starts.append((x, y))           # floor warps (mats, stairs)
        elif walkable(lay, x, y + 1):
            starts.append((x, y + 1))       # doors: you arrive below them
        else:
            problems.append('%s: warp at (%d,%d) to %s has no walkable tile below it' % (tag, x, y, w['dest_map']))
    for c in m.get('connections') or []:
        d = c['direction']
        edge = {'up': [(x, 0) for x in range(lay.w)], 'down': [(x, lay.h - 1) for x in range(lay.w)],
                'left': [(0, y) for y in range(lay.h)], 'right': [(lay.w - 1, y) for y in range(lay.h)]}[d]
        open_edge = [p for p in edge if walkable(lay, *p)]
        if not open_edge:
            problems.append('%s: no walkable tile on the %s edge for %s' % (tag, d, c['map']))
        starts += open_edge

    seen = set()
    q = deque(p for p in starts if p not in blocked)
    seen.update(q)
    while q:
        x, y = q.popleft()
        for dx, dy in ((1, 0), (-1, 0), (0, 1), (0, -1)):
            n = (x + dx, y + dy)
            if n not in seen and n not in blocked and walkable(lay, *n):
                seen.add(n)
                q.append(n)

    def near(x, y):
        return any((x + dx, y + dy) in seen for dx, dy in ((1, 0), (-1, 0), (0, 1), (0, -1)))

    guarded = {(o['x'], o['y'] - 1) for o in m['object_events'] if o.get('local_id') in BLOCKING_OK}
    for w in m['warp_events']:
        x, y = w['x'], w['y']
        if (x, y) not in seen and not near(x, y) and (x, y) not in guarded:
            problems.append('%s: warp at (%d,%d) to %s is unreachable' % (tag, x, y, w['dest_map']))
    for (x, y), o in objs.items():
        name = o.get('local_id') or o['script']
        # spectators (no script) may stand on the gallery boxes
        if not walkable(lay, x, y) and o['graphics_id'] != 'OBJ_EVENT_GFX_TRUCK' and o['script'] != '0x0':
            problems.append('%s: %s stands on a wall at (%d,%d)' % (tag, name, x, y))
        if o['graphics_id'] in FRLG_GFX and o['script'] not in FRLG_GFX_OK:
            problems.append('%s: %s uses %s, an FRLG sprite that draws nothing' % (tag, name, o['graphics_id']))
        if not near(x, y) and name not in BLOCKING_OK and o['script'] != '0x0':
            problems.append('%s: nobody can reach %s at (%d,%d)' % (tag, name, x, y))
    for b in m['bg_events']:
        if not near(b['x'], b['y']) and (b['x'], b['y']) not in seen:
            problems.append('%s: sign %s at (%d,%d) is unreachable' % (tag, b.get('script'), b['x'], b['y']))
    for c in m['coord_events']:
        if (c['x'], c['y']) not in seen:
            problems.append('%s: trigger %s at (%d,%d) is unreachable' % (tag, c['script'], c['x'], c['y']))
    return lay


def secondary(name):
    if name not in _tilesets:
        _tilesets[name] = Tileset(tileset_dir_for(name))
    return _tilesets[name]


def same_metatile(mid, sa, sb):
    """Does metatile mid render the same with secondary tilesets sa and sb?"""
    if sa == sb or mid < 0x200:
        return True
    a, b = secondary(sa), secondary(sb)
    i = mid - 0x200
    if i >= len(a.metatiles) or i >= len(b.metatiles) or a.metatiles[i] != b.metatiles[i]:
        return False
    for e in a.metatiles[i]:
        t, pal = (e & 0x3FF) - 0x200, e >> 12
        if t >= 0 and (t >= len(a.tiles) or t >= len(b.tiles) or a.tiles[t] != b.tiles[t]):
            return False
        if pal >= 6 and a.pals[pal] != b.pals[pal]:
            return False
    return True


def seam_problems(name, a, la, other, b, lb, d, o, pairs, problems):
    """Near a seam, each map's tiles must render the same with the other map's tileset."""
    sa, sb = la['secondary_tileset'], lb['secondary_tileset']
    if sa == sb:
        return
    open_a = [p for p, q in pairs if walkable(a, *p) and walkable(b, *q)]
    bad = set()
    for (x, y) in open_a:
        if d in ('up', 'down'):
            ys_a = range(0, SEAM_DEPTH_ROWS) if d == 'up' else range(a.h - SEAM_DEPTH_ROWS, a.h)
            ys_b = range(b.h - SEAM_DEPTH_ROWS, b.h) if d == 'up' else range(0, SEAM_DEPTH_ROWS)
            cells = [(a, la, cx, cy, name) for cx in range(x - SEAM_SPREAD, x + SEAM_SPREAD + 1) for cy in ys_a]
            cells += [(b, lb, cx - o, cy, other) for cx in range(x - SEAM_SPREAD, x + SEAM_SPREAD + 1) for cy in ys_b]
        else:
            xs_a = range(0, SEAM_DEPTH_COLS) if d == 'left' else range(a.w - SEAM_DEPTH_COLS, a.w)
            xs_b = range(b.w - SEAM_DEPTH_COLS, b.w) if d == 'left' else range(0, SEAM_DEPTH_COLS)
            cells = [(a, la, cx, cy, name) for cy in range(y - SEAM_SPREAD, y + SEAM_SPREAD + 1) for cx in xs_a]
            cells += [(b, lb, cx, cy - o, other) for cy in range(y - SEAM_SPREAD, y + SEAM_SPREAD + 1) for cx in xs_b]
        for lay, l, cx, cy, mapname in cells:
            if 0 <= cx < lay.w and 0 <= cy < lay.h:
                mid = lay.get(cx, cy) & 0x3FF
                if not same_metatile(mid, sa, sb):
                    bad.add((mapname, cx, cy, mid))
    for mapname in sorted({m for m, *_ in bad}):
        cells = sorted((x, y, m) for n, x, y, m in bad if n == mapname)
        problems.append('%s near the %s seam with %s: %d tile(s) use %s-only metatiles, e.g. %s' % (
            mapname, d, other if mapname == name else name, len(cells),
            (la if mapname == name else lb)['secondary_tileset'], ['(%d,%d)=0x%x' % c for c in cells[:6]]))


def main():
    problems = []
    for name in MAPS:
        m = json.load(open(path('data/maps', name, 'map.json')))
        layouts = LOWMERE_STAGES if name == 'LittlerootTown' else [m['layout']]
        sizes = set()
        for lid in layouts:
            lay = check(name, lid, problems)
            sizes.add((lay.w, lay.h))
        if len(sizes) > 1:
            problems.append('%s: stage layouts differ in size: %s' % (name, sorted(sizes)))
    # connections must meet open ground on both sides
    for name in ('LittlerootTown', 'Route101', 'OldaleTown', 'Route102', 'Route103'):
        m = json.load(open(path('data/maps', name, 'map.json')))
        a = load_layout(m['layout'])
        for c in m['connections'] or []:
            other = c['map'].replace('MAP_', '')
            om = next(json.load(open(path('data/maps', n, 'map.json'))) for n in
                      ('LittlerootTown', 'Route101', 'OldaleTown', 'Route102', 'Route103', 'PetalburgCity', 'Route110')
                      if json.load(open(path('data/maps', n, 'map.json')))['id'] == c['map'])
            b = load_layout(om['layout'])
            la, lb = LAYOUTS[m['layout']], LAYOUTS[om['layout']]
            o, d = c['offset'], c['direction']
            pairs = []
            if d in ('up', 'down'):
                ay, by = (0, b.h - 1) if d == 'up' else (a.h - 1, 0)
                pairs = [((x, ay), (x - o, by)) for x in range(a.w) if 0 <= x - o < b.w]
            else:
                ax, bx = (0, b.w - 1) if d == 'left' else (a.w - 1, 0)
                pairs = [((ax, y), (bx, y - o)) for y in range(a.h) if 0 <= y - o < b.h]
            ok = [p for p, q in pairs if walkable(a, *p) and walkable(b, *q)]
            dangling = [p for p, q in pairs if walkable(a, *p) != walkable(b, *q)]
            if not ok:
                problems.append('%s -> %s (%s): no shared open tiles' % (name, other, d))
            if dangling:
                problems.append('%s -> %s (%s): edge tiles open on one side only: %s' % (name, other, d, dangling))
            if name in ('LittlerootTown', 'Route101', 'OldaleTown'):
                seam_problems(name, a, la, other, b, lb, d, o, pairs, problems)
    for p in problems:
        print(p)
    print('%d problem(s)' % len(problems))
    sys.exit(1 if problems else 0)


if __name__ == '__main__':
    main()
