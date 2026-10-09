#!/usr/bin/env python3
"""Write map.json events, new interior maps and placeholder scripts for the slice.

Run after build_maps.py, from the repo root:
    python3 dev_scripts/slice_maps/build_events.py

One-shot generator: once merged, map.json and scripts.inc are edited by hand (and by the
dialogue thread). Re-running it overwrites the events of the maps it touches.
"""
import json
import subprocess
import os
import re

from gfxlib import path

import build_maps as M

STUB_HEADER = '@ ---- Map-thread placeholders. The dialogue thread replaces these. ----\n'


def load_map(name):
    """The vanilla map.json from origin/main when there is one, so re-running is idempotent."""
    try:
        out = subprocess.run(['git', 'show', 'origin/main:data/maps/%s/map.json' % name], cwd=path(),
                             capture_output=True, check=True, text=True).stdout
        return json.loads(out)
    except subprocess.CalledProcessError:
        return json.load(open(path('data/maps', name, 'map.json')))


def save_map(name, data):
    d = path('data/maps', name)
    os.makedirs(d, exist_ok=True)
    with open(os.path.join(d, 'map.json'), 'w') as f:
        json.dump(data, f, indent=2)
        f.write('\n')


def obj(gfx, x, y, script, local_id=None, move='MOVEMENT_TYPE_FACE_DOWN', flag='0',
        trainer=None, sight=0, rx=0, ry=0, elevation=3):
    o = {}
    if local_id:
        o['local_id'] = local_id
    o.update({
        'graphics_id': gfx, 'x': x, 'y': y, 'elevation': elevation, 'movement_type': move,
        'movement_range_x': rx, 'movement_range_y': ry,
        'trainer_type': 'TRAINER_TYPE_NORMAL' if trainer else 'TRAINER_TYPE_NONE',
        'trainer_sight_or_berry_tree_id': str(sight), 'script': script, 'flag': flag,
    })
    return o


def warp(x, y, dest, dest_id, elevation=0):
    return {'x': x, 'y': y, 'elevation': elevation, 'dest_map': dest, 'dest_warp_id': str(dest_id)}


def coord(x, y, var, value, script, elevation=3):
    return {'type': 'trigger', 'x': x, 'y': y, 'elevation': elevation, 'var': var,
            'var_value': str(value), 'script': script}


def sign(x, y, script, elevation=0):
    return {'type': 'sign', 'x': x, 'y': y, 'elevation': elevation,
            'player_facing_dir': 'BG_EVENT_PLAYER_FACING_ANY', 'script': script}


def move(events, key, value, **fields):
    """Update the event whose `key` equals `value`."""
    hits = [e for e in events if e.get(key) == value]
    assert hits, (key, value)
    for e in hits:
        e.update(fields)


class Stubs:
    """Collects placeholder scripts for one map's scripts.inc."""

    def __init__(self, mapname):
        self.mapname = mapname
        self.blocks = []

    def npc(self, label, who):
        text = label.replace('_EventScript_', '_Text_')
        self.blocks.append('%s::\n\tmsgbox %s, MSGBOX_NPC\n\tend\n\n%s:\n\t.string "%s: (placeholder)$"\n'
                           % (label, text, text, who))
        return label

    def sign(self, label, what):
        text = label.replace('_EventScript_', '_Text_')
        self.blocks.append('%s::\n\tmsgbox %s, MSGBOX_SIGN\n\tend\n\n%s:\n\t.string "%s$"\n'
                           % (label, text, text, what))
        return label

    def trainer(self, label, trainer, who):
        base = label.replace('_EventScript_', '_Text_')
        self.blocks.append(
            '%s::\n\ttrainerbattle_single %s, %sIntro, %sDefeat\n\tmsgbox %sPostBattle, MSGBOX_AUTOCLOSE\n\tend\n\n'
            '%sIntro:\n\t.string "%s: (placeholder intro)$"\n\n'
            '%sDefeat:\n\t.string "%s: (placeholder defeat)$"\n\n'
            '%sPostBattle:\n\t.string "%s: (placeholder after battle)$"\n'
            % (label, trainer, base, base, base, base, who, base, who, base, who))
        return label

    def write(self, new_map=False):
        p = path('data/maps', self.mapname, 'scripts.inc')
        old = open(p).read() if os.path.exists(p) else ''
        if STUB_HEADER in old:
            old = old[:old.index(STUB_HEADER)].rstrip('\n') + '\n'
        if new_map or not old:
            old = '%s_MapScripts::\n\t.byte 0\n' % self.mapname
        body = old.rstrip('\n') + '\n\n' + STUB_HEADER + '\n' + '\n'.join(self.blocks) if self.blocks else old
        with open(p, 'w') as f:
            f.write(body)


def interior(name, map_id, layout, music, mapsec, warps, objects=(), bgs=()):
    return {
        'id': map_id, 'name': name, 'layout': layout, 'music': music, 'region': 'REGION_HOENN',
        'region_map_section': mapsec, 'requires_flash': False, 'weather': 'WEATHER_NONE',
        'map_type': 'MAP_TYPE_INDOOR', 'allow_cycling': False, 'allow_escaping': False,
        'allow_running': False, 'show_map_name': False, 'battle_scene': 'MAP_BATTLE_SCENE_NORMAL',
        'connections': None, 'object_events': list(objects), 'warp_events': list(warps),
        'coord_events': [], 'bg_events': list(bgs),
    }


def add_to_group(group, names):
    p = path('data/maps/map_groups.json')
    data = json.load(open(p))
    for n in names:
        if n not in data[group]:
            data[group].append(n)
    with open(p, 'w') as f:
        json.dump(data, f, indent=2)
        f.write('\n')


def include_scripts(after, names):
    p = path('data/event_scripts.s')
    s = open(p).read()
    for n in reversed(names):
        line = '\t.include "data/maps/%s/scripts.inc"\n' % n
        if line in s:
            continue
        anchor = '\t.include "data/maps/%s/scripts.inc"\n' % after
        s = s.replace(anchor, anchor + line, 1)
    open(p, 'w').write(s)


# Lowmere doors (shared by every stage)
LM_DOORS = {
    'hesk': (M.HESK[0] + 1, M.HESK[1] + 3),
    'lodge': (M.LODGE_POS[0] + 3, M.LODGE_POS[1] + 4),
    'shed': (M.SHED[0] + 1, M.SHED[1] + 3),
    'forge': (M.FORGE[0] + 1, M.FORGE[1] + 3),
    'reeve': (M.REEVE[0] + 1, M.REEVE[1] + 3),
    'house_c': (M.HOUSE_C[0] + 1, M.HOUSE_C[1] + 3),
}


def lowmere():
    name = 'LittlerootTown'
    m = load_map(name)
    st = Stubs(name)
    m['layout'] = 'LAYOUT_LOWMERE_STAGE0'
    m['connections'] = [{'map': 'MAP_ROUTE101', 'offset': M.NORTH_EXIT[0] - M.MIRE_SOUTH[0], 'direction': 'up'}]

    ev = m['object_events']
    # vanilla objects the intro scripts still use, moved onto the new map
    move(ev, 'script', 'LittlerootTown_EventScript_Twin', x=16, y=10)
    move(ev, 'script', 'LittlerootTown_EventScript_FatMan', x=17, y=11)
    move(ev, 'script', 'LittlerootTown_EventScript_Boy', x=14, y=17)
    move(ev, 'local_id', 'LOCALID_LITTLEROOT_MOM', x=5, y=8)
    move(ev, 'flag', 'FLAG_HIDE_LITTLEROOT_TOWN_BRENDANS_HOUSE_TRUCK', x=2, y=10)
    move(ev, 'flag', 'FLAG_HIDE_LITTLEROOT_TOWN_MAYS_HOUSE_TRUCK', x=11, y=10)
    move(ev, 'local_id', 'LOCALID_LITTLEROOT_RIVAL', x=14, y=11)
    move(ev, 'local_id', 'LOCALID_LITTLEROOT_BIRCH', x=14, y=10)
    p = 'LittlerootTown_EventScript_'
    ev += [
        obj('OBJ_EVENT_GFX_EXPERT_M', 9, 9, st.npc(p + 'Bram', 'BRAM'), 'LOCALID_LOWMERE_BRAM',
            move='MOVEMENT_TYPE_FACE_UP'),
        obj('OBJ_EVENT_GFX_FAT_MAN', 11, 8, st.npc(p + 'Toft', 'REEVE TOFT'), 'LOCALID_LOWMERE_TOFT'),
        obj('OBJ_EVENT_GFX_TAMSIN', 5, 15, st.npc(p + 'Tamsin', 'TAMSIN'), 'LOCALID_LOWMERE_TAMSIN',
            move='MOVEMENT_TYPE_FACE_UP'),
        obj('OBJ_EVENT_GFX_MAN_3', 8, 8, st.npc(p + 'Courier', 'COURIER'), 'LOCALID_LOWMERE_COURIER',
            move='MOVEMENT_TYPE_FACE_LEFT'),
        obj('OBJ_EVENT_GFX_FISHERMAN', 18, 20, st.npc(p + 'JettyVillager', 'VILLAGER (rotten jetty)'),
            'LOCALID_LOWMERE_JETTY_VILLAGER', move='MOVEMENT_TYPE_FACE_RIGHT'),
        obj('OBJ_EVENT_GFX_WOMAN_2', 13, 12, st.npc(p + 'HealerVillager', 'VILLAGER (no healer)'),
            'LOCALID_LOWMERE_HEALER_VILLAGER', move='MOVEMENT_TYPE_WANDER_AROUND', rx=1, ry=1),
        obj('OBJ_EVENT_GFX_MAN_1', 19, 12, st.npc(p + 'ShopVillager', 'VILLAGER (no shop)'),
            'LOCALID_LOWMERE_SHOP_VILLAGER', move='MOVEMENT_TYPE_LOOK_AROUND'),
        obj('OBJ_EVENT_GFX_OLD_MAN', 23, 14, st.npc(p + 'DreamsVillager', 'VILLAGER (bad dreams)'),
            'LOCALID_LOWMERE_DREAMS_VILLAGER', move='MOVEMENT_TYPE_FACE_LEFT'),
        obj('OBJ_EVENT_GFX_WOMAN_5', 13, 17, st.npc(p + 'GrainSeller', 'GRAIN SELLER (Stage 1+)'),
            'LOCALID_LOWMERE_GRAIN_SELLER', move='MOVEMENT_TYPE_FACE_DOWN'),
        obj('OBJ_EVENT_GFX_MAN_4', 19, 17, st.npc(p + 'StallKeeper', 'STALL KEEPER (Stage 1+)'),
            'LOCALID_LOWMERE_STALL_KEEPER', move='MOVEMENT_TYPE_FACE_DOWN'),
    ]

    # Warp order is fixed: interiors return to these ids.
    m['warp_events'] = [
        warp(*LM_DOORS['hesk'], 'MAP_LITTLEROOT_TOWN_MAYS_HOUSE_1F', 1),
        warp(*LM_DOORS['lodge'], 'MAP_LITTLEROOT_TOWN_BRENDANS_HOUSE_1F', 1),
        warp(*LM_DOORS['shed'], 'MAP_LITTLEROOT_TOWN_PROFESSOR_BIRCHS_LAB', 0),
        warp(*LM_DOORS['forge'], 'MAP_LOWMERE_FORGE', 0),
        warp(*LM_DOORS['reeve'], 'MAP_LOWMERE_REEVES_HOUSE', 0),
        warp(*LM_DOORS['house_c'], 'MAP_LOWMERE_REOPENED_HOUSE', 0),
    ]
    north = M.NORTH_EXIT
    ce = m['coord_events']
    move(ce, 'script', 'LittlerootTown_EventScript_NeedPokemonTriggerLeft', x=north[0], y=1)
    move(ce, 'script', 'LittlerootTown_EventScript_NeedPokemonTriggerRight', x=north[1], y=1)
    move(ce, 'script', 'LittlerootTown_EventScript_GoSaveBirchTrigger', x=north[1], y=1)
    move(ce, 'script', 'LittlerootTown_EventScript_GiveRunningShoesTrigger0', x=north[0], y=2)
    move(ce, 'script', 'LittlerootTown_EventScript_GiveRunningShoesTrigger1', x=north[1], y=2)
    for i, (x, y) in zip((4, 5, 2, 3), ((12, 7), (12, 8), (13, 7), (13, 8))):
        move(ce, 'script', 'LittlerootTown_EventScript_GiveRunningShoesTrigger%d' % i, x=x, y=y)

    bg = m['bg_events']
    for label, (x, y) in (('TownSign', M.LM_SIGNS['town']), ('BirchsLabSign', M.LM_SIGNS['shed']),
                          ('BrendansHouseSign', M.LM_SIGNS['lodge']), ('MaysHouseSign', M.LM_SIGNS['hesk'])):
        move(bg, 'script', 'LittlerootTown_EventScript_' + label, x=x, y=y, elevation=0)
    save_map(name, m)
    st.write()

    # New Lowmere interiors
    houses = [('Lowmere_Forge', 'MAP_LOWMERE_FORGE', 'LAYOUT_HOUSE1', 3, (3, 8), 'forge', 'SMITH (Tamsin\'s forge)'),
              ('Lowmere_ReevesHouse', 'MAP_LOWMERE_REEVES_HOUSE', 'LAYOUT_HOUSE2', 4, (3, 7), 'reeve', 'TOFT\'S WIFE'),
              ('Lowmere_ReopenedHouse', 'MAP_LOWMERE_REOPENED_HOUSE', 'LAYOUT_HOUSE1', 5, (3, 8), 'house_c', 'RETURNED VILLAGER')]
    for name, map_id, layout, warp_id, (dx, dy), _, who in houses:
        st = Stubs(name)
        o = [obj('OBJ_EVENT_GFX_WOMAN_4' if 'Reeve' in name else 'OBJ_EVENT_GFX_MAN_2', 8, 4,
                 st.npc(name + '_EventScript_Resident', who), move='MOVEMENT_TYPE_FACE_DOWN', elevation=3)]
        w = [warp(dx, dy, 'MAP_LITTLEROOT_TOWN', warp_id), warp(dx + 1, dy, 'MAP_LITTLEROOT_TOWN', warp_id)]
        save_map(name, interior(name, map_id, layout, 'MUS_LITTLEROOT', 'MAPSEC_LITTLEROOT_TOWN', w, o))
        st.write(new_map=True)
    names = [h[0] for h in houses]
    add_to_group('gMapGroup_IndoorLittleroot', names)
    include_scripts('LittlerootTown_ProfessorBirchsLab', names)

    # Mother Hesk lives in Elena's old house (vanilla May's house)
    name = 'LittlerootTown_MaysHouse_1F'
    m = load_map(name)
    st = Stubs(name)
    if not any(o.get('local_id') == 'LOCALID_LOWMERE_HESK' for o in m['object_events']):
        m['object_events'].append(obj('OBJ_EVENT_GFX_EXPERT_F', 4, 4, st.npc(name + '_EventScript_Hesk', 'MOTHER HESK'),
                                      'LOCALID_LOWMERE_HESK', move='MOVEMENT_TYPE_FACE_DOWN'))
    else:
        st.npc(name + '_EventScript_Hesk', 'MOTHER HESK')
    save_map(name, m)
    st.write()


def mire_road():
    name = 'Route101'
    m = load_map(name)
    st = Stubs(name)
    m['connections'] = [
        {'map': 'MAP_OLDALE_TOWN', 'offset': M.MIRE_NORTH[0] - M.hay(*M.HAY_SOUTH)[0], 'direction': 'up'},
        {'map': 'MAP_LITTLEROOT_TOWN', 'offset': M.MIRE_SOUTH[0] - M.NORTH_EXIT[0], 'direction': 'down'},
    ]
    ev = m['object_events']
    # vanilla Birch rescue, kept at the south end until the dialogue thread replaces it
    move(ev, 'script', 'Route101_EventScript_Youngster', x=14, y=12)
    move(ev, 'local_id', 'LOCALID_ROUTE101_BIRCH', x=11, y=39)
    move(ev, 'script', 'Route101_EventScript_BirchsBag', x=13, y=41)
    move(ev, 'local_id', 'LOCALID_ROUTE101_ZIGZAGOON', x=12, y=39)
    move(ev, 'script', 'ProfBirch_EventScript_RatePokedexOrRegister', x=12, y=30)
    move(ev, 'script', 'Route101_EventScript_Boy', x=3, y=21)
    p = 'Route101_EventScript_'
    wx, wy = M.WAGON
    ev += [
        obj('OBJ_EVENT_GFX_GENTLEMAN', 11, 6, st.trainer(p + 'Hobb', 'TRAINER_RICK', 'CLERK HOBB'),
            trainer=True, sight=3, move='MOVEMENT_TYPE_FACE_DOWN'),
        obj('OBJ_EVENT_GFX_HIKER', 5, 21, st.trainer(p + 'Grisk', 'TRAINER_TIANA', 'GRISK'),
            trainer=True, sight=3, move='MOVEMENT_TYPE_FACE_RIGHT'),
        obj('OBJ_EVENT_GFX_YOUNGSTER', 9, 33, st.trainer(p + 'Pip', 'TRAINER_ALLEN', 'PIP'),
            trainer=True, sight=4, move='MOVEMENT_TYPE_FACE_RIGHT'),
        obj('OBJ_EVENT_GFX_FISHERMAN', 16, 17, st.trainer(p + 'Willem', 'TRAINER_ANDREW', 'WILLEM'),
            trainer=True, sight=2, move='MOVEMENT_TYPE_FACE_LEFT'),
        obj('OBJ_EVENT_GFX_AQUA_MEMBER_M', wx + 1, wy + 2, st.trainer(p + 'WagonClerk', 'TRAINER_GRUNT_PETALBURG_WOODS', 'WAGON CLERK'),
            'LOCALID_MIRE_ROAD_WAGON_CLERK', trainer=True, sight=2, move='MOVEMENT_TYPE_FACE_DOWN'),
        obj('OBJ_EVENT_GFX_AQUA_MEMBER_M', wx + 3, wy + 1, st.npc(p + 'ReceiptClerk', 'CLERK (drops the bell receipt)'),
            'LOCALID_MIRE_ROAD_RECEIPT_CLERK', move='MOVEMENT_TYPE_FACE_LEFT'),
    ]
    sx = M.MIRE_SOUTH
    ce = m['coord_events']
    hy = M.MIRE_H
    for e in ce:
        s = e['script']
        if s.endswith('StartBirchRescue'):
            e['x'], e['y'] = (sx[0] if e['x'] == 10 else sx[1]), hy - 2
        elif s.endswith('PreventExitSouth'):
            e['x'], e['y'] = (sx[0] if e['x'] == 10 else sx[1]), hy - 3
        elif s.endswith('PreventExitWest'):
            e['x'], e['y'] = 10, 38 + (e['y'] - 15)
        elif s.endswith('PreventExitNorth'):
            e['x'], e['y'] = 12, 37
    move(m['bg_events'], 'script', 'Route101_EventScript_RouteSign', x=13, y=40)
    save_map(name, m)
    st.write()


HAY_WARPS = [
    ('penn', M.door(M.HAY_PENN, (1, 3)), 'MAP_OLDALE_TOWN_HOUSE1', 0),
    ('magistrate', M.door(M.HAY_MAGISTRATE, (2, 4)), 'MAP_OLDALE_TOWN_HOUSE2', 0),
    ('pc', M.door(M.HAY_PC, (1, 3)), 'MAP_OLDALE_TOWN_POKEMON_CENTER_1F', 0),
    ('mart', M.door(M.HAY_MART, (1, 3)), 'MAP_OLDALE_TOWN_MART', 0),
    ('pavilion', M.door(M.HAY_PAVILION, (2, 4)), 'MAP_HAYMARKET_GILT_PAVILION', 0),
    ('counting', M.door(M.HAY_COUNTING, (3, 5)), 'MAP_HAYMARKET_COUNTING_HOUSE', 0),
    ('granary', M.door(M.HAY_GRANARY, (2, 6)), 'MAP_HAYMARKET_GRANARY', 0),
]


def haymarket():
    name = 'OldaleTown'
    m = load_map(name)
    st = Stubs(name)
    m['connections'] = [
        {'map': 'MAP_ROUTE103', 'offset': M.hay(*M.HAY_NORTH[:2])[0] - 8, 'direction': 'up'},
        {'map': 'MAP_ROUTE101', 'offset': M.hay(*M.HAY_SOUTH)[0] - M.MIRE_NORTH[0], 'direction': 'down'},
        {'map': 'MAP_ROUTE102', 'offset': M.hay(0, M.HAY_WEST[0])[1] - 10, 'direction': 'left'},
    ]
    ev = m['object_events']
    move(ev, 'script', 'OldaleTown_EventScript_Girl', x=24, y=12)
    move(ev, 'local_id', 'LOCALID_OLDALE_MART_EMPLOYEE', x=29, y=8)
    move(ev, 'local_id', 'LOCALID_FOOTPRINTS_MAN', x=8, y=12)
    move(ev, 'local_id', 'LOCALID_OLDALE_RIVAL', x=20, y=29)
    p = 'OldaleTown_EventScript_'
    pav = M.door(M.HAY_PAVILION, (2, 4))
    ev += [
        obj('OBJ_EVENT_GFX_RICH_BOY', 21, 10, st.npc(p + 'CorwinAtFair', 'CORWIN'), 'LOCALID_HAYMARKET_CORWIN'),
        obj('OBJ_EVENT_GFX_WOMAN_5', 18, 10, st.npc(p + 'FairgoerA', 'FAIRGOER'), 'LOCALID_HAYMARKET_FAIRGOER_A',
            move='MOVEMENT_TYPE_FACE_RIGHT'),
        obj('OBJ_EVENT_GFX_MAN_1', 24, 10, st.npc(p + 'FairgoerB', 'FAIRGOER'), 'LOCALID_HAYMARKET_FAIRGOER_B',
            move='MOVEMENT_TYPE_FACE_LEFT'),
        obj('OBJ_EVENT_GFX_LITTLE_GIRL', 20, 9, st.npc(p + 'FairgoerC', 'FAIRGOER'), 'LOCALID_HAYMARKET_FAIRGOER_C',
            move='MOVEMENT_TYPE_FACE_DOWN'),
        obj('OBJ_EVENT_GFX_WOMAN_2', 20, 16, st.npc(p + 'WidowPenn', 'WIDOW PENN'), 'LOCALID_HAYMARKET_PENN',
            move='MOVEMENT_TYPE_FACE_DOWN'),
        obj('OBJ_EVENT_GFX_AQUA_MEMBER_M', 19, 16, st.npc(p + 'SeizingClerkA', 'GUILD CLERK'),
            'LOCALID_HAYMARKET_SEIZING_CLERK_A', move='MOVEMENT_TYPE_FACE_RIGHT'),
        obj('OBJ_EVENT_GFX_AQUA_MEMBER_M', 21, 16, st.npc(p + 'SeizingClerkB', 'GUILD CLERK'),
            'LOCALID_HAYMARKET_SEIZING_CLERK_B', move='MOVEMENT_TYPE_FACE_LEFT'),
        obj('OBJ_EVENT_GFX_GENTLEMAN', 8, 9, st.npc(p + 'Crane', 'SILAS CRANE'), 'LOCALID_HAYMARKET_CRANE',
            move='MOVEMENT_TYPE_FACE_LEFT'),
        obj('OBJ_EVENT_GFX_BLACK_BELT', pav[0], pav[1] + 1, st.npc(p + 'PavilionGuard', 'PALACE GUARD'),
            'LOCALID_HAYMARKET_PAVILION_GUARD'),
        obj('OBJ_EVENT_GFX_OLD_MAN', 14, 26, st.npc(p + 'Townsman', 'TOWNSMAN'), move='MOVEMENT_TYPE_WANDER_AROUND',
            rx=2, ry=1),
        obj('OBJ_EVENT_GFX_WOMAN_4', 33, 15, st.npc(p + 'Townswoman', 'TOWNSWOMAN'), move='MOVEMENT_TYPE_WANDER_AROUND',
            rx=1, ry=2),
        obj('OBJ_EVENT_GFX_FAT_MAN', 8, 18, st.npc(p + 'GrainBuyer', 'GRAIN BUYER'), move='MOVEMENT_TYPE_FACE_RIGHT'),
    ]
    m['warp_events'] = [warp(x, y, dest, did) for _, (x, y), dest, did in HAY_WARPS]
    ce = m['coord_events']
    wy = M.HAY_WEST[0]
    move(ce, 'script', 'OldaleTown_EventScript_BlockedPath', x=0, y=wy)
    for i, x in zip((1, 2, 3), (M.HAY_SOUTH[0], M.HAY_SOUTH[1], M.HAY_SOUTH[1] + 1)):
        move(ce, 'script', 'OldaleTown_EventScript_RivalTrigger%d' % i, x=x, y=28)
    bg = m['bg_events']
    pcx, pcy = M.HAY_PC
    mx, my = M.HAY_MART
    m['bg_events'] = [
        sign(23, 8, 'OldaleTown_EventScript_TownSign'),
        sign(pcx + 2, pcy + 3, 'Common_EventScript_ShowPokemonCenterSign'),
        sign(pcx + 3, pcy + 3, 'Common_EventScript_ShowPokemonCenterSign'),
        sign(mx + 2, my + 3, 'Common_EventScript_ShowPokemartSign'),
        sign(mx + 3, my + 3, 'Common_EventScript_ShowPokemartSign'),
        sign(23, 22, st.sign(p + 'NoticeBoard', 'GILDED SCALE NOTICE (placeholder)')),
    ]
    # everything above is in town-grid coordinates; place it in the framed map
    for e in m['object_events'] + m['warp_events'] + m['coord_events'] + m['bg_events']:
        e['x'], e['y'] = M.hay(e['x'], e['y'])
    save_map(name, m)
    st.write()

    # Vanilla interiors reused: Penn's house and the magistrate's hall
    for name, who, local in (('OldaleTown_House1', 'WIDOW PENN (at home)', 'LOCALID_HAYMARKET_PENN_HOME'),
                             ('OldaleTown_House2', 'MAGISTRATE ARDEN', 'LOCALID_HAYMARKET_ARDEN')):
        m = load_map(name)
        st = Stubs(name)
        label = st.npc(name + '_EventScript_' + ('PennAtHome' if 'Penn' in who else 'Arden'), who)
        if not any(o.get('local_id') == local for o in m['object_events']):
            gfx = 'OBJ_EVENT_GFX_WOMAN_2' if 'Penn' in who else 'OBJ_EVENT_GFX_GENTLEMAN'
            m['object_events'].append(obj(gfx, 6, 3, label, local, elevation=3))
        save_map(name, m)
        st.write()

    # Gilt Pavilion
    name = 'Haymarket_GiltPavilion'
    st = Stubs(name)
    p = name + '_EventScript_'
    h = sum(n for _, n in M.PAVILION_PARTS)
    o = [
        obj('OBJ_EVENT_GFX_RICH_BOY', 4, 4, st.npc(p + 'Corwin', 'CORWIN (Trial 1)'), 'LOCALID_GILT_PAVILION_CORWIN'),
        obj('OBJ_EVENT_GFX_GENTLEMAN', 6, 5, st.npc(p + 'Auctioneer', 'AUCTIONEER'), 'LOCALID_GILT_PAVILION_AUCTIONEER',
            move='MOVEMENT_TYPE_FACE_LEFT'),
        obj('OBJ_EVENT_GFX_GENTLEMAN', 4, 10, st.trainer(p + 'Fenwick', 'TRAINER_MARC', 'BROKER FENWICK'),
            trainer=True, sight=3),
        obj('OBJ_EVENT_GFX_LASS', 6, 15, st.trainer(p + 'Celeste', 'TRAINER_TOMMY', 'BIDDER CELESTE'),
            trainer=True, sight=3, move='MOVEMENT_TYPE_FACE_LEFT'),
        obj('OBJ_EVENT_GFX_RICH_BOY', 2, 17, st.trainer(p + 'Albrecht', 'TRAINER_JOSH', 'BIDDER ALBRECHT'),
            trainer=True, sight=3, move='MOVEMENT_TYPE_FACE_RIGHT'),
        obj('OBJ_EVENT_GFX_WOMAN_5', 1, 9, st.npc(p + 'SpectatorA', 'SPECTATOR'), move='MOVEMENT_TYPE_FACE_RIGHT'),
        obj('OBJ_EVENT_GFX_MAN_1', 7, 12, st.npc(p + 'SpectatorB', 'SPECTATOR'), move='MOVEMENT_TYPE_FACE_LEFT'),
        obj('OBJ_EVENT_GFX_OLD_MAN', 7, 18, st.npc(p + 'SpectatorC', 'SPECTATOR'), move='MOVEMENT_TYPE_FACE_LEFT'),
        obj('OBJ_EVENT_GFX_BLACK_BELT', 3, 21, st.npc(p + 'Steward', 'STEWARD'), move='MOVEMENT_TYPE_FACE_DOWN'),
    ]
    w = [warp(4, h - 1, 'MAP_OLDALE_TOWN', 4), warp(5, h - 1, 'MAP_OLDALE_TOWN', 4)]
    save_map(name, interior(name, 'MAP_HAYMARKET_GILT_PAVILION', 'LAYOUT_HAYMARKET_GILT_PAVILION', 'MUS_GYM',
                            'MAPSEC_OLDALE_TOWN', w, o))
    st.write(new_map=True)

    # Counting house: front office and Crane's back office upstairs
    name = 'Haymarket_CountingHouse'
    st = Stubs(name)
    p = name + '_EventScript_'
    o = [obj('OBJ_EVENT_GFX_AQUA_MEMBER_M', 11, 6, st.trainer(p + 'Clerk', 'TRAINER_GRUNT_MUSEUM_1', 'GUILD CLERK'),
             trainer=True, sight=3, move='MOVEMENT_TYPE_FACE_LEFT'),
         obj('OBJ_EVENT_GFX_WOMAN_5', 5, 3, st.npc(p + 'Receptionist', 'RECEPTIONIST'), move='MOVEMENT_TYPE_FACE_DOWN')]
    w = [warp(5, 8, 'MAP_OLDALE_TOWN', 5), warp(6, 8, 'MAP_OLDALE_TOWN', 5),
         warp(14, 1, 'MAP_HAYMARKET_COUNTING_HOUSE_BACK_ROOM', 0)]
    save_map(name, interior(name, 'MAP_HAYMARKET_COUNTING_HOUSE', 'LAYOUT_RUSTBORO_CITY_DEVON_CORP_1F', 'MUS_OLDALE',
                            'MAPSEC_OLDALE_TOWN', w, o))
    st.write(new_map=True)

    name = 'Haymarket_CountingHouse_BackRoom'
    st = Stubs(name)
    p = name + '_EventScript_'
    o = [obj('OBJ_EVENT_GFX_AQUA_MEMBER_M', 9, 6, st.trainer(p + 'Clerk', 'TRAINER_GRUNT_MUSEUM_2', 'GUILD CLERK'),
             trainer=True, sight=3, move='MOVEMENT_TYPE_FACE_RIGHT'),
         obj('OBJ_EVENT_GFX_ITEM_BALL', 1, 7, st.npc(p + 'PennsPokemon', 'WIDOW PENN\'S POKEMON'),
             'LOCALID_COUNTING_HOUSE_PENNS_POKEMON')]
    bgs = [sign(2, 4, st.sign(p + 'CranesDesk', 'CRANE\'S DESK (sealed letter, placeholder)'))]
    w = [warp(14, 1, 'MAP_HAYMARKET_COUNTING_HOUSE', 2)]
    save_map(name, interior(name, 'MAP_HAYMARKET_COUNTING_HOUSE_BACK_ROOM', 'LAYOUT_RUSTBORO_CITY_DEVON_CORP_2F',
                            'MUS_OLDALE', 'MAPSEC_OLDALE_TOWN', w, o, bgs))
    st.write(new_map=True)

    # Granary: push the sacks to reach the scale room
    name = 'Haymarket_Granary'
    st = Stubs(name)
    p = name + '_EventScript_'
    o = [obj('OBJ_EVENT_GFX_HIKER', 14, 11, st.trainer(p + 'Foreman', 'TRAINER_GRUNT_RUSTURF_TUNNEL', 'GUILD FOREMAN'),
             'LOCALID_GRANARY_FOREMAN', trainer=True, sight=3, move='MOVEMENT_TYPE_FACE_LEFT')]
    for i, (x, y) in enumerate(((9, 9), (10, 11), (16, 10), (8, 12))):
        o.append(obj('OBJ_EVENT_GFX_PUSHABLE_BOULDER', x, y, 'EventScript_StrengthBoulder',
                     move='MOVEMENT_TYPE_LOOK_AROUND'))
    bgs = [sign(19, 4, st.sign(p + 'WeightsCrate', 'CROWN WEIGHTS CRATE (placeholder)')),
           sign(15, 4, st.sign(p + 'Scale', 'THE SCALE (placeholder)'))]
    w = [warp(2, 14, 'MAP_OLDALE_TOWN', 6), warp(3, 14, 'MAP_OLDALE_TOWN', 6)]
    save_map(name, interior(name, 'MAP_HAYMARKET_GRANARY', 'LAYOUT_SLATEPORT_CITY_STERNS_SHIPYARD_1F', 'MUS_OLDALE',
                            'MAPSEC_OLDALE_TOWN', w, o, bgs))
    st.write(new_map=True)

    names = ['Haymarket_GiltPavilion', 'Haymarket_CountingHouse', 'Haymarket_CountingHouse_BackRoom', 'Haymarket_Granary']
    add_to_group('gMapGroup_IndoorOldale', names)
    include_scripts('OldaleTown_Mart', names)


def neighbours():
    """Route 102 and Route 103 keep meeting Haymarket where their roads end."""
    for name, direction, offset in (('Route102', 'right', 10 - M.hay(0, M.HAY_WEST[0])[1]),
                                    ('Route103', 'down', 8 - M.hay(M.HAY_NORTH[0], 0)[0])):
        m = load_map(name)
        for c in m['connections']:
            if c['map'] == 'MAP_OLDALE_TOWN':
                c['offset'] = offset
                assert c['direction'] == direction
        save_map(name, m)


def main():
    lowmere()
    mire_road()
    haymarket()
    neighbours()
    print('wrote events for Lowmere, the Mire Road, Haymarket and their interiors')


if __name__ == '__main__':
    main()
