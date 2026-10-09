#!/usr/bin/env python3
"""Place chapter 3's people, signs and triggers (docs/dialogue/chapter3.md).

    python3 dev_scripts/slice_maps/wire_chapter3.py

Rebuilds each map's events from scratch, so it can be re-run. It keeps warps, connections,
berry trees, item balls, hidden items and cut trees, drops everyone else and the vanilla
story triggers, and adds the chapter 3 events pointing at the dialogue thread's labels. The maps' scripts.inc files become map headers only.
"""
import os

from build_events import load_map, save_map, obj, coord, sign
from gfxlib import Layout, path
import json

LAYOUTS = {l['id']: l for l in json.load(open(path('data/layouts/layouts.json')))['layouts'] if 'id' in l}
CS = 'VAR_CRAGHOLT_STATE'
KEEP_GFX = {'OBJ_EVENT_GFX_ITEM_BALL', 'OBJ_EVENT_GFX_BERRY_TREE', 'OBJ_EVENT_GFX_CUTTABLE_TREE'}


def keep_objects(m):
    return [o for o in m['object_events'] if o['graphics_id'] in KEEP_GFX]


def keep_bgs(m, signs=()):
    return [b for b in m['bg_events'] if b['type'] != 'sign' or b['script'] in signs]


class Placer:
    """Builds objects at the elevation of the tile they stand on."""

    def __init__(self, m):
        l = LAYOUTS[m['layout']]
        self.lay = Layout.load(path(l['blockdata_filepath']), l['width'])

    def elev(self, x, y):
        e = self.lay.get(x, y) >> 12
        return e if 1 <= e <= 14 else 3

    def obj(self, gfx, x, y, script, *a, **kw):
        kw.setdefault('elevation', self.elev(x, y))
        return obj(gfx, x, y, script, *a, **kw)

    def trainer(self, gfx, x, y, script, face, sight, **kw):
        return self.obj(gfx, x, y, script, move='MOVEMENT_TYPE_FACE_' + face, trainer=True, sight=sight, **kw)


def face(d):
    return 'MOVEMENT_TYPE_FACE_' + d


def header(name, comment, entries, bodies=''):
    """A scripts.inc holding only the map header."""
    lines = ['@ ' + c for c in comment] + ['%s_MapScripts::' % name]
    lines += ['\tmap_script %s, %s' % e for e in entries] + ['\t.byte 0', '']
    with open(path('data/maps', name, 'scripts.inc'), 'w') as f:
        f.write('\n'.join(lines) + bodies)


def chain_road():
    name = 'Route104'
    m = load_map(name)
    p = Placer(m)
    s = 'ChainRoad_EventScript_'
    # The coffle rests on the grass above the beach path; the rope signs sit between the men.
    m['object_events'] = [
        p.obj('OBJ_EVENT_GFX_MAN_5', 23, 52, s + 'Harrow', move=face('DOWN'), flag='FLAG_TEMP_1'),
        p.obj('OBJ_EVENT_GFX_HIKER', 25, 52, s + 'Bonded1', move=face('DOWN'), flag='FLAG_TEMP_1'),
        p.obj('OBJ_EVENT_GFX_MAN_4', 27, 52, s + 'Bonded2', move=face('DOWN'), flag='FLAG_TEMP_1'),
        p.obj('OBJ_EVENT_GFX_POKEFAN_M', 22, 52, s + 'CoffleGuard', move=face('RIGHT'), flag='FLAG_TEMP_1'),
        p.trainer('OBJ_EVENT_GFX_FISHERMAN', 15, 59, s + 'Fisher', 'LEFT', 2),
        p.trainer('OBJ_EVENT_GFX_RICH_BOY', 21, 25, s + 'Collector', 'DOWN', 3),
    ] + keep_objects(m)
    m['coord_events'] = []
    m['bg_events'] = [
        sign(24, 52, s + 'Rope'), sign(26, 52, s + 'Rope'),
        sign(27, 66, s + 'SignSouth'), sign(23, 5, s + 'SignNorth'),
    ] + keep_bgs(m, {'Route104_EventScript_FlowerShopSign'})
    save_map(name, m)
    header(name, ['The Chain Road. Its people are in the dialogue thread\'s chapter 3 scripts',
                  '(docs/dialogue/chapter3.md); this file only holds the map header.'],
           [('MAP_SCRIPT_ON_TRANSITION', 'ChainRoad_OnTransition')],
           '\nChainRoad_OnTransition:\n\tcall_if_ge %s, 3, ChainRoad_EventScript_HideCoffle\n\tend\n' % CS)
    # The flower shop is still vanilla and its sign label lived in this file.
    with open(path('data/maps', name, 'scripts.inc'), 'a') as f:
        f.write('\nRoute104_EventScript_FlowerShopSign::\n\tmsgbox Route104_Text_PrettyPetalFlowShop, MSGBOX_SIGN\n'
                '\tend\n\nRoute104_Text_PrettyPetalFlowShop:\n\t.string "PRETTY PETAL FLOWER SHOP$"\n')


def penrose_cottage():
    name = 'Route104_MrBrineysHouse'
    m = load_map(name)
    p = Placer(m)
    s = 'ChainRoad_PenroseCottage_EventScript_'
    m['object_events'] = [
        p.obj('OBJ_EVENT_GFX_OLD_MAN', 5, 3, s + 'Penrose', move=face('DOWN')),
        p.obj('OBJ_EVENT_GFX_HIKER', 6, 3, s + 'Wil', move=face('DOWN'), flag='FLAG_TEMP_1'),
    ]
    m['bg_events'] = [sign(9, 5, s + 'Nets')]
    save_map(name, m)
    header(name, ['Old Penrose\'s cottage on the Chain Road; see docs/dialogue/chapter3.md.'],
           [('MAP_SCRIPT_ON_TRANSITION', 'ChainRoad_PenroseCottage_OnTransition')],
           '\nChainRoad_PenroseCottage_OnTransition:\n'
           '\tcall_if_lt %s, 3, ChainRoad_PenroseCottage_EventScript_HideWil\n\tend\n' % CS)


def gallows_wood():
    name = 'PetalburgWoods'
    m = load_map(name)
    p = Placer(m)
    s = 'GallowsWood_EventScript_'
    # Row 7 from x 12 to 17 is the only way to the north exit at (14-15,5): the warden
    # stands at its west end and sees the whole row.
    m['object_events'] = [
        p.trainer('OBJ_EVENT_GFX_AQUA_MEMBER_M', 12, 7, s + 'DebtWarden', 'RIGHT', 5),
        p.obj('OBJ_EVENT_GFX_BUG_CATCHER', 7, 32, s + 'BugCatcher',
              move='MOVEMENT_TYPE_FACE_DOWN_LEFT_AND_RIGHT', trainer=True, sight=3),
    ] + keep_objects(m)
    m['coord_events'] = []
    m['bg_events'] = [sign(11, 8, s + 'Checkpoint', 3), sign(14, 32, s + 'Sign', 3)] + keep_bgs(m)
    save_map(name, m)
    header(name, ['Gallows Wood. Its people are in the dialogue thread\'s chapter 3 scripts',
                  '(docs/dialogue/chapter3.md); this file only holds the map header.'], [])


def cragholt():
    name = 'RustboroCity'
    m = load_map(name)
    m['layout'] = 'LAYOUT_CRAGHOLT'
    p = Placer(m)
    s = 'Cragholt_EventScript_'
    m['object_events'] = [
        # The arrival scene: Brannoc west of the headframe, the bonded men east of it.
        p.obj('OBJ_EVENT_GFX_BLACK_BELT', 18, 52, '0x0', 'LOCALID_CRAGHOLT_BRANNOC', face('RIGHT'), 'FLAG_TEMP_1'),
        p.obj('OBJ_EVENT_GFX_POKEFAN_M', 22, 52, '0x0', 'LOCALID_CRAGHOLT_GUARD', face('LEFT'), 'FLAG_TEMP_1'),
        p.obj('OBJ_EVENT_GFX_MAN_5', 21, 51, '0x0', 'LOCALID_CRAGHOLT_BONDED_1', face('LEFT'), 'FLAG_TEMP_1'),
        p.obj('OBJ_EVENT_GFX_HIKER', 21, 52, '0x0', 'LOCALID_CRAGHOLT_BONDED_2', face('LEFT'), 'FLAG_TEMP_1'),
        p.obj('OBJ_EVENT_GFX_MAN_4', 21, 53, '0x0', 'LOCALID_CRAGHOLT_BONDED_3', face('LEFT'), 'FLAG_TEMP_1'),
        p.obj('OBJ_EVENT_GFX_TAMSIN', 30, 40, s + 'Tamsin', 'LOCALID_CRAGHOLT_TAMSIN', face('LEFT'), 'FLAG_TEMP_2'),
        p.obj('OBJ_EVENT_GFX_MAN_5', 24, 52, s + 'Crew1', move=face('LEFT'), flag='FLAG_TEMP_3'),
        p.obj('OBJ_EVENT_GFX_HIKER', 25, 52, s + 'Crew2', move=face('LEFT'), flag='FLAG_TEMP_3'),
        p.obj('OBJ_EVENT_GFX_POKEFAN_M', 26, 51, s + 'CrewGuard', move=face('LEFT'), flag='FLAG_TEMP_3'),
        p.obj('OBJ_EVENT_GFX_HIKER', 25, 41, s + 'FreedMiner1', move='MOVEMENT_TYPE_LOOK_AROUND', flag='FLAG_TEMP_4'),
        p.obj('OBJ_EVENT_GFX_MAN_4', 30, 42, s + 'FreedMiner2', move='MOVEMENT_TYPE_LOOK_AROUND', flag='FLAG_TEMP_4'),
        p.obj('OBJ_EVENT_GFX_POKEFAN_M', 27, 20, s + 'HallGuard', 'LOCALID_CRAGHOLT_HALL_GUARD', face('DOWN'), 'FLAG_TEMP_5'),
        p.obj('OBJ_EVENT_GFX_WOMAN_5', 22, 34, s + 'Woman', move='MOVEMENT_TYPE_WANDER_UP_AND_DOWN', ry=1),
        p.obj('OBJ_EVENT_GFX_OLD_MAN', 19, 27, s + 'OldMiner', move=face('DOWN')),
        p.obj('OBJ_EVENT_GFX_TWIN', 21, 46, s + 'Child', move='MOVEMENT_TYPE_WANDER_UP_AND_DOWN', ry=1),
    ] + keep_objects(m)
    # The road in is eight tiles wide at row 54 and nothing else leads into town from the south.
    m['coord_events'] = [coord(x, 54, CS, 0, s + 'Arrival') for x in range(12, 20)]
    m['bg_events'] = [
        sign(19, 52, s + 'Lift'), sign(20, 52, s + 'Lift'),
        sign(28, 39, s + 'QuotaBoard'), sign(19, 49, s + 'TownSign'),
        sign(23, 19, s + 'HallSign'), sign(17, 20, s + 'OfficeSign'),
        sign(25, 35, s + 'BarracksSign'), sign(30, 8, s + 'MineSign'),
    ] + keep_bgs(m, {'Common_EventScript_ShowPokemartSign', 'Common_EventScript_ShowPokemonCenterSign'})
    save_map(name, m)
    header(name, ['Cragholt. Its people and scenes are in the dialogue thread\'s chapter 3 scripts',
                  '(docs/dialogue/chapter3.md); this file only holds the map header.'],
           [('MAP_SCRIPT_ON_TRANSITION', 'Cragholt_OnTransition')],
           '\nCragholt_OnTransition:\n\tsetflag FLAG_VISITED_RUSTBORO_CITY\n'
           '\tcall Cragholt_EventScript_SetObjects\n\tend\n')


def interior(name, objects, bgs, comment, transition=None):
    m = load_map(name)
    p = Placer(m)
    m['object_events'] = [o if isinstance(o, dict) else p.obj(*o[0], **o[1]) for o in objects]
    m['coord_events'] = []
    m['bg_events'] = bgs
    save_map(name, m)
    entries = []
    body = ''
    if transition:
        label = name + '_OnTransition'
        entries = [('MAP_SCRIPT_ON_TRANSITION', label)]
        body = '\n%s:\n\t%s\n\tend\n' % (label, transition)
    header(name, [comment], entries, body)
    return p


def o(*a, **kw):
    return (a, kw)


def interiors():
    s = 'Cragholt_Barracks_EventScript_'
    interior('RustboroCity_PokemonSchool', [
        o('OBJ_EVENT_GFX_MAN_3', 5, 3, s + 'Keeper', move=face('DOWN')),
        o('OBJ_EVENT_GFX_MAN_5', 3, 8, s + 'Sleeper', move=face('UP')),
    ], [sign(x, 2, s + 'Roll') for x in (4, 6, 7)] + [sign(3, 5, s + 'Bunks')],
        'The bonded men\'s barracks in Cragholt; see docs/dialogue/chapter3.md.')

    s = 'Cragholt_OreOffice_EventScript_'
    m = load_map('RustboroCity_DevonCorp_1F')
    p = Placer(m)
    interior('RustboroCity_DevonCorp_1F', [
        # The clerk stands in front of the desk; the assayer keeps the stairs.
        p.trainer('OBJ_EVENT_GFX_AQUA_MEMBER_M', 5, 5, s + 'Clerk', 'DOWN', 3),
        o('OBJ_EVENT_GFX_SCIENTIST_1', 14, 2, s + 'Assayer', 'LOCALID_ORE_OFFICE_ASSAYER', face('DOWN')),
    ], [sign(x, 4, s + 'Ledger') for x in (4, 5, 6)] + [sign(3, 2, s + 'OreSamples'), sign(8, 2, s + 'OreSamples')],
        'The Chancellery ore office in Cragholt; see docs/dialogue/chapter3.md.')

    s = 'Cragholt_MagistrateHall_EventScript_'
    interior('RustboroCity_House1', [
        o('OBJ_EVENT_GFX_GENTLEMAN', 9, 2, s + 'Hollen', move=face('DOWN')),
        o('OBJ_EVENT_GFX_HIKER', 6, 4, s + 'Wil', move=face('LEFT'), flag='FLAG_TEMP_1'),
    ], [sign(3, 1, s + 'Reports')],
        'Magistrate Hollen\'s hall in Cragholt; see docs/dialogue/chapter3.md.',
        'call Cragholt_MagistrateHall_EventScript_SetObjects')

    s = 'Cragholt_TallisHouse_EventScript_'
    interior('RustboroCity_House2', [
        o('OBJ_EVENT_GFX_WOMAN_4', 4, 4, s + 'Tallis', move=face('DOWN')),
    ], [sign(2, 1, s + 'Boots')], 'Widow Tallis\'s house in Cragholt; see docs/dialogue/chapter3.md.')

    s = 'Cragholt_MasonHouse_EventScript_'
    interior('RustboroCity_House3', [
        o('OBJ_EVENT_GFX_MAN_4', 4, 5, s + 'Mason', move=face('RIGHT')),
        o('OBJ_EVENT_GFX_LITTLE_GIRL', 7, 5, s + 'Girl', move=face('LEFT')),
    ], [], 'The mason\'s house in Cragholt; see docs/dialogue/chapter3.md.')

    s = 'PitheadHall_EventScript_'
    m = load_map('RustboroCity_Gym')
    p = Placer(m)
    interior('RustboroCity_Gym', [
        o('OBJ_EVENT_GFX_BLACK_BELT', 5, 2, s + 'Brannoc', move=face('DOWN')),
        p.trainer('OBJ_EVENT_GFX_HIKER', 1, 6, s + 'Foreman', 'DOWN', 3),
        p.trainer('OBJ_EVENT_GFX_MAN_4', 3, 9, s + 'Miner1', 'LEFT', 3),
        p.trainer('OBJ_EVENT_GFX_MAN_5', 5, 13, s + 'Miner2', 'DOWN', 2),
        o('OBJ_EVENT_GFX_WOMAN_5', 3, 18, s + 'Cook', move=face('DOWN')),
        o('OBJ_EVENT_GFX_MAN_3', 4, 18, s + 'Herald', move=face('RIGHT')),
    ], [sign(2, 18, s + 'Seam'), sign(8, 18, s + 'Seam')],
        'The Pithead Hall, Prince Brannoc\'s palace; see docs/dialogue/chapter3.md.')
    # src/field_control_avatar.c still names this vanilla label. Roxanne's first-call flag
    # is never set now, so it never runs.
    with open(path('data/maps/RustboroCity_Gym/scripts.inc'), 'a') as f:
        f.write('\nRustboroCity_Gym_EventScript_RegisterRoxanne::\n'
                '\tclearflag FLAG_ENABLE_ROXANNE_FIRST_CALL\n\tend\n')


def collapsed_mine():
    name = 'RusturfTunnel'
    m = load_map(name)
    m['layout'] = 'LAYOUT_CRAGHOLT_MINE'
    p = Placer(m)
    s = 'CollapsedMine_EventScript_'
    m['object_events'] = [
        p.obj('OBJ_EVENT_GFX_HIKER', 23, 4, s + 'Wil', 'LOCALID_COLLAPSED_MINE_WIL', face('LEFT'), 'FLAG_TEMP_1'),
        p.obj('OBJ_EVENT_GFX_MAN_5', 24, 5, s + 'TrappedMiner', 'LOCALID_COLLAPSED_MINE_MINER_1', face('LEFT'), 'FLAG_TEMP_1'),
        p.obj('OBJ_EVENT_GFX_MAN_4', 26, 4, s + 'TrappedMiner', 'LOCALID_COLLAPSED_MINE_MINER_2', face('DOWN'), 'FLAG_TEMP_1'),
    ] + keep_objects(m)
    m['coord_events'] = []
    m['bg_events'] = [
        sign(20, 5, s + 'Rubble'), sign(20, 4, s + 'Rubble'), sign(19, 4, s + 'BellCrate'),
        sign(14, 3, s + 'Props'), sign(18, 3, s + 'Props'), sign(7, 10, s + 'Sign'),
    ] + keep_bgs(m)
    save_map(name, m)
    header(name, ['The collapsed mine east of Cragholt; see docs/dialogue/chapter3.md.'],
           [('MAP_SCRIPT_ON_TRANSITION', 'CollapsedMine_OnTransition'),
            ('MAP_SCRIPT_ON_LOAD', 'CollapsedMine_OnLoad')],
           '\nCollapsedMine_OnTransition:\n\tcall CollapsedMine_EventScript_SetObjects\n\tend\n'
           '\nCollapsedMine_OnLoad:\n\tcall_if_set FLAG_RESCUED_WIL, CollapsedMine_EventScript_ClearRubble\n\tend\n')


def old_lodge():
    """Tamsin beside Toft for the act end. Appended, so she is object 11."""
    name = 'LittlerootTown_BrendansHouse_1F'
    f = path('data/maps', name, 'map.json')
    m = json.load(open(f))
    m['object_events'] = [e for e in m['object_events'] if e.get('local_id') != 'LOCALID_OLD_LODGE_TAMSIN']
    m['object_events'].append(obj('OBJ_EVENT_GFX_TAMSIN', 6, 7, 'Lowmere_OldLodge_EventScript_Tamsin',
                                  'LOCALID_OLD_LODGE_TAMSIN', face('LEFT'), 'FLAG_TEMP_1'))
    assert len(m['object_events']) == 11
    save_map(name, m)


def main():
    chain_road()
    penrose_cottage()
    gallows_wood()
    cragholt()
    interiors()
    collapsed_mine()
    old_lodge()
    print('wired chapter 3')


if __name__ == '__main__':
    main()
