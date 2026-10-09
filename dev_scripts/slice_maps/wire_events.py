#!/usr/bin/env python3
"""Point the slice maps at the dialogue thread's scripts (docs/dialogue/events.md).

    python3 dev_scripts/slice_maps/wire_events.py

Edits the current map.json files in place: repoints objects and signs at the area-named
labels in data/scripts/slice/, adopts the dialogue thread's local ids and hide flags, adds
the VAR_SLICE_STATE triggers and the objects the scenes need, and drops the vanilla
townsfolk and triggers the slice replaces. Vanilla objects that vanilla scripts still name
by local id stay until those scripts are cut. Finally it removes map-thread placeholder
scripts that nothing points at any more.

Safe to re-run: every step looks events up by local id, script or position first.
"""
import json
import os
import re

from build_events import load_map, save_map, obj, coord, sign, STUB_HEADER  # noqa: F401
from gfxlib import Layout, path

LAYOUTS = {l['id']: l for l in json.load(open(path('data/layouts/layouts.json')))['layouts'] if 'id' in l}
ST = 'VAR_SLICE_STATE'


def current(name):
    return json.load(open(path('data/maps', name, 'map.json')))


def layout_of(m, lid=None):
    l = LAYOUTS[lid or m['layout']]
    return Layout.load(path(l['blockdata_filepath']), l['width'])


def walkable(lay, x, y):
    if not (0 <= x < lay.w and 0 <= y < lay.h):
        return False
    v = lay.get(x, y)
    return ((v >> 10) & 3) == 0 and (v >> 12) != 1


def row_cells(lay, y):
    """Every walkable tile in row y: a trigger across it cannot be walked around."""
    return [x for x in range(lay.w) if walkable(lay, x, y)]


class Map:
    def __init__(self, name):
        self.name = name
        self.m = current(name)
        self.lay = layout_of(self.m)

    def find(self, **kw):
        hits = [o for o in self.m['object_events'] if all(o.get(k) == v for k, v in kw.items())]
        assert len(hits) == 1, (self.name, kw, len(hits))
        return hits[0]

    def has(self, **kw):
        return any(all(o.get(k) == v for k, v in kw.items()) for o in self.m['object_events'])

    def set(self, match, **fields):
        """Update one object, found by a dict of fields (it may already be updated)."""
        o = None
        for cand in (match, {k: fields[k] for k in ('local_id', 'script') if k in fields}):
            if cand and self.has(**cand):
                o = self.find(**cand)
                break
        assert o is not None, (self.name, match)
        new_lid = fields.pop('local_id', o.get('local_id'))
        o.update(fields)
        if new_lid:
            # keep local_id as the first key, as the other generators write it
            items = [(k, v) for k, v in o.items() if k != 'local_id']
            o.clear()
            o['local_id'] = new_lid
            o.update(items)
        elif 'local_id' in o:
            del o['local_id']
        return o

    def drop(self, **kw):
        self.m['object_events'] = [o for o in self.m['object_events']
                                   if not all(o.get(k) == v for k, v in kw.items())]

    def add(self, o):
        key = {'local_id': o['local_id']} if o.get('local_id') else {'script': o['script'], 'x': o['x'], 'y': o['y']}
        self.drop(**key)
        self.m['object_events'].append(o)

    def triggers(self, cells, values, script):
        """One coord event per cell per value of VAR_SLICE_STATE."""
        self.m['coord_events'] = [c for c in self.m['coord_events'] if c.get('script') != script]
        for v in values:
            for (x, y) in cells:
                assert walkable(self.lay, x, y), (self.name, script, x, y)
                self.m['coord_events'].append(coord(x, y, ST, v, script))

    def drop_vanilla_triggers(self):
        self.m['coord_events'] = [c for c in self.m['coord_events'] if c.get('var') == ST]

    def signs(self, spots):
        """{(x, y): script}. Replaces whatever sign is on that tile."""
        bg = [b for b in self.m['bg_events'] if (b['x'], b['y']) not in spots]
        for (x, y), script in spots.items():
            assert not walkable(self.lay, x, y), (self.name, 'sign on open ground', x, y, script)
            bg.append(sign(x, y, script))
        self.m['bg_events'] = bg

    def save(self):
        save_map(self.name, self.m)


def npc(gfx, x, y, script, local_id=None, flag='0', move='MOVEMENT_TYPE_FACE_DOWN', **kw):
    return obj(gfx, x, y, script, local_id, move=move, flag=flag, **kw)


# ---- Lowmere ---------------------------------------------------------------
def lowmere():
    t = Map('LittlerootTown')
    # every stage keeps the same open tiles at the north exit; check them on stage 0
    # the north exit runs between trees for rows 0-1; Tamsin waits on row 0, and both
    # north-road triggers sit on row 1, the only way out
    for y in (0, 1):
        assert row_cells(t.lay, y) == [15, 16], (y, row_cells(t.lay, y))

    t.set({'local_id': 'LOCALID_LOWMERE_BRAM'}, script='Lowmere_EventScript_BramAtCart',
          flag='FLAG_HIDE_LOWMERE_ARRIVAL_BRAM')
    t.set({'local_id': 'LOCALID_LOWMERE_TAMSIN'}, local_id=None, script='Lowmere_EventScript_TamsinForge',
          flag='FLAG_HIDE_LOWMERE_TAMSIN_FORGE')
    t.add(npc('OBJ_EVENT_GFX_RIVAL_MAY_NORMAL', 16, 0, 'Lowmere_EventScript_TamsinRoad',
              'LOCALID_LOWMERE_TAMSIN_ROAD', 'FLAG_HIDE_LOWMERE_TAMSIN_ROAD'))
    t.set({'local_id': 'LOCALID_LOWMERE_JETTY_VILLAGER'}, local_id=None, script='Lowmere_EventScript_Fisherman',
          graphics_id='OBJ_EVENT_GFX_FISHERMAN')
    t.set({'local_id': 'LOCALID_LOWMERE_HEALER_VILLAGER'}, local_id=None, script='Lowmere_EventScript_Mother')
    t.set({'local_id': 'LOCALID_LOWMERE_DREAMS_VILLAGER'}, local_id=None, script='Lowmere_EventScript_OldMan',
          graphics_id='OBJ_EVENT_GFX_OLD_MAN')
    t.set({'local_id': 'LOCALID_LOWMERE_SHOP_VILLAGER'}, local_id=None, script='Lowmere_EventScript_Child',
          graphics_id='OBJ_EVENT_GFX_LITTLE_BOY')
    t.set({'local_id': 'LOCALID_LOWMERE_STALL_KEEPER'}, local_id=None, script='Lowmere_EventScript_Stallkeeper')
    # Toft and the courier appear inside the Old Lodge; the grain seller has no scene
    for lid in ('LOCALID_LOWMERE_TOFT', 'LOCALID_LOWMERE_COURIER', 'LOCALID_LOWMERE_GRAIN_SELLER'):
        t.drop(local_id=lid)
    # vanilla townsfolk with no local id
    for s in ('LittlerootTown_EventScript_FatMan', 'LittlerootTown_EventScript_Boy'):
        t.drop(script=s)

    t.drop_vanilla_triggers()
    t.triggers([(15, 1), (16, 1)], [5], 'Lowmere_EventScript_TamsinRoad')
    t.triggers([(15, 1), (16, 1)], [2, 3, 4, 6], 'Lowmere_EventScript_NorthRoadBlock')

    t.m['bg_events'] = [b for b in t.m['bg_events'] if not b['script'].startswith('LittlerootTown_')]
    t.signs({
        (14, 9): 'Lowmere_EventScript_TownSign',
        (8, 7): 'Lowmere_EventScript_OldLodgeSign',
        (18, 6): 'Lowmere_EventScript_RangersShedSign',
        (23, 13): 'Lowmere_EventScript_HeskHouseSign',
        (15, 13): 'Lowmere_EventScript_Well',
        (16, 13): 'Lowmere_EventScript_Well',
        (25, 6): 'Lowmere_EventScript_GrainStore',     # house A, boarded until stage 2
        (10, 21): 'Lowmere_EventScript_BoardedHouse',  # house C, boarded at stage 0
        (19, 19): 'Lowmere_EventScript_Jetty',
    })
    t.save()

    # The Old Lodge (the player's house)
    t = Map('LittlerootTown_BrendansHouse_1F')
    t.add(npc('OBJ_EVENT_GFX_FAT_MAN', 5, 7, 'Lowmere_EventScript_Toft', 'LOCALID_OLD_LODGE_TOFT',
              'FLAG_HIDE_OLD_LODGE_TOFT', move='MOVEMENT_TYPE_FACE_LEFT'))
    t.add(npc('OBJ_EVENT_GFX_EXPERT_M', 7, 6, 'Lowmere_OldLodge_EventScript_Bram', 'LOCALID_OLD_LODGE_BRAM',
              'FLAG_HIDE_OLD_LODGE_BRAM', move='MOVEMENT_TYPE_FACE_LEFT'))
    t.add(npc('OBJ_EVENT_GFX_MAN_3', 8, 6, 'Lowmere_OldLodge_EventScript_Courier', 'LOCALID_OLD_LODGE_COURIER',
              'FLAG_HIDE_OLD_LODGE_COURIER', move='MOVEMENT_TYPE_FACE_LEFT'))
    t.m['bg_events'] = [b for b in t.m['bg_events'] if b['script'] != 'Lowmere_OldLodge_EventScript_Ledger']
    t.signs({(4, 6): 'Lowmere_OldLodge_EventScript_Ledger'})
    t.save()

    # The Ranger's Shed (Birch's lab)
    t = Map('LittlerootTown_ProfessorBirchsLab')
    t.add(npc('OBJ_EVENT_GFX_EXPERT_M', 5, 4, 'Lowmere_RangersShed_EventScript_Bram'))
    t.save()

    # The forge, the reeve's house and house C have no scenes yet, so they stay empty
    # rather than show placeholder lines
    for name in ('Lowmere_Forge', 'Lowmere_ReevesHouse', 'Lowmere_ReopenedHouse'):
        t = Map(name)
        t.drop(script=name + '_EventScript_Resident')
        t.save()

    # Mother Hesk's house (May's house)
    t = Map('LittlerootTown_MaysHouse_1F')
    t.set({'local_id': 'LOCALID_LOWMERE_HESK'}, script='Lowmere_HeskHouse_EventScript_Hesk')
    t.save()
    t = Map('LittlerootTown_MaysHouse_2F')
    t.signs({(7, 5): 'Lowmere_HeskHouse_EventScript_ElenasBed'})
    t.save()


# ---- the Mire Road ----------------------------------------------------------
def mire_road():
    t = Map('Route101')
    t.set({'script': 'Route101_EventScript_Hobb'}, script='MireRoad_EventScript_Clerk',
          graphics_id='OBJ_EVENT_GFX_AQUA_MEMBER_M')
    t.set({'script': 'Route101_EventScript_Grisk'}, script='MireRoad_EventScript_Poacher')
    t.set({'script': 'Route101_EventScript_Pip'}, script='MireRoad_EventScript_Youngster')
    t.set({'script': 'Route101_EventScript_Willem'}, script='MireRoad_EventScript_Carter',
          graphics_id='OBJ_EVENT_GFX_MAN_4')
    t.set({'local_id': 'LOCALID_MIRE_ROAD_WAGON_CLERK'}, script='MireRoad_EventScript_WagonClerk',
          flag='FLAG_HIDE_MIRE_ROAD_WAGON_CLERKS', trainer_type='TRAINER_TYPE_NONE',
          trainer_sight_or_berry_tree_id='0')
    t.set({'local_id': 'LOCALID_MIRE_ROAD_RECEIPT_CLERK'}, local_id='LOCALID_MIRE_ROAD_WAGON_CLERK_2',
          script='0x0', flag='FLAG_HIDE_MIRE_ROAD_WAGON_CLERKS')
    t.add(npc('OBJ_EVENT_GFX_RIVAL_MAY_NORMAL', 13, 41, 'MireRoad_EventScript_Tamsin', 'LOCALID_MIRE_ROAD_TAMSIN',
              'FLAG_HIDE_MIRE_ROAD_TAMSIN', move='MOVEMENT_TYPE_FACE_LEFT'))
    t.add(npc('OBJ_EVENT_GFX_RIVAL_MAY_NORMAL', 9, 18, '0x0', 'LOCALID_MIRE_ROAD_TAMSIN_WAGON',
              'FLAG_HIDE_MIRE_ROAD_TAMSIN_WAGON', move='MOVEMENT_TYPE_FACE_UP'))
    for s in ('Route101_EventScript_Youngster', 'Route101_EventScript_BirchsBag',
              'ProfBirch_EventScript_RatePokedexOrRegister', 'Route101_EventScript_Boy'):
        t.drop(script=s)

    t.drop_vanilla_triggers()
    t.triggers([(x, t.lay.h - 1) for x in row_cells(t.lay, t.lay.h - 1)], [7], 'MireRoad_EventScript_TamsinJoins')
    t.triggers([(x, 20) for x in row_cells(t.lay, 20)], [7], 'MireRoad_EventScript_WagonClerk')

    t.m['bg_events'] = []
    t.signs({(13, 40): 'MireRoad_EventScript_RouteSign',
             (10, 17): 'MireRoad_EventScript_Wagon', (12, 17): 'MireRoad_EventScript_Wagon'})
    t.save()


# ---- Haymarket -------------------------------------------------------------
def haymarket():
    t = Map('OldaleTown')
    t.set({'local_id': 'LOCALID_HAYMARKET_PENN'}, script='Haymarket_EventScript_Penn')
    t.set({'local_id': 'LOCALID_HAYMARKET_SEIZING_CLERK_A'}, local_id='LOCALID_HAYMARKET_SEIZURE_CLERK_1',
          script='0x0', flag='FLAG_HIDE_HAYMARKET_SEIZURE_CLERKS')
    t.set({'local_id': 'LOCALID_HAYMARKET_SEIZING_CLERK_B'}, local_id='LOCALID_HAYMARKET_SEIZURE_CLERK_2',
          script='0x0', flag='FLAG_HIDE_HAYMARKET_SEIZURE_CLERKS')
    t.set({'local_id': 'LOCALID_HAYMARKET_CORWIN'}, script='0x0', flag='FLAG_HIDE_HAYMARKET_CORWIN_SQUARE')
    t.set({'local_id': 'LOCALID_HAYMARKET_CRANE'}, script='0x0', flag='FLAG_HIDE_HAYMARKET_CRANE')
    t.set({'local_id': 'LOCALID_HAYMARKET_PAVILION_GUARD'}, local_id=None, script='Haymarket_EventScript_PavilionGuard',
          x=27, y=12, movement_type='MOVEMENT_TYPE_FACE_RIGHT')
    t.add(npc('OBJ_EVENT_GFX_RIVAL_MAY_NORMAL', 21, 13, 'Haymarket_EventScript_Tamsin', 'LOCALID_HAYMARKET_TAMSIN',
              'FLAG_HIDE_HAYMARKET_TAMSIN'))
    for old, new, gfx in (('LOCALID_HAYMARKET_FAIRGOER_A', 'Haymarket_EventScript_RibbonSeller', None),
                          ('LOCALID_HAYMARKET_FAIRGOER_B', 'Haymarket_EventScript_NervousMan', None),
                          ('LOCALID_HAYMARKET_FAIRGOER_C', 'Haymarket_EventScript_Girl', None)):
        t.set({'local_id': old}, local_id=None, script=new)
    t.set({'script': 'OldaleTown_EventScript_Townsman'}, script='Haymarket_EventScript_Granny',
          graphics_id='OBJ_EVENT_GFX_OLD_WOMAN')
    t.set({'script': 'OldaleTown_EventScript_Townswoman'}, script='Haymarket_EventScript_Farmer',
          graphics_id='OBJ_EVENT_GFX_MAN_4')
    t.set({'script': 'OldaleTown_EventScript_GrainBuyer'}, script='Haymarket_EventScript_Boy',
          graphics_id='OBJ_EVENT_GFX_BOY_1')
    t.drop(script='OldaleTown_EventScript_Girl')

    t.drop_vanilla_triggers()
    south = row_cells(t.lay, 35)
    assert south == [27, 28], south   # the dirt road in from the Mire Road
    t.triggers([(x, 35) for x in south], [8], 'Haymarket_EventScript_Arrival')
    t.triggers([(14, 13)], [10], 'Haymarket_EventScript_Crane')
    t.triggers([(28, 12)], [8, 9], 'Haymarket_EventScript_PavilionDoorTrigger')

    t.m['bg_events'] = [b for b in t.m['bg_events'] if b['script'].startswith('Common_')]
    t.signs({
        (31, 26): 'Haymarket_EventScript_TownSign',
        (31, 12): 'Haymarket_EventScript_PavilionSign',
        (13, 12): 'Haymarket_EventScript_MagistrateSign',
        (40, 13): 'Haymarket_EventScript_CountingHouseSign',
        (38, 30): 'Haymarket_EventScript_GranarySign',
    })
    t.save()

    # Penn's house: no scene inside yet
    t = Map('OldaleTown_House1')
    t.drop(script='OldaleTown_House1_EventScript_Woman')
    t.drop(local_id='LOCALID_HAYMARKET_PENN_HOME')
    t.save()

    # The Magistrate's Hall
    t = Map('OldaleTown_House2')
    t.set({'local_id': 'LOCALID_HAYMARKET_ARDEN'}, script='Haymarket_MagistrateHall_EventScript_Arden')
    t.set({'script': 'OldaleTown_House2_EventScript_Woman'}, script='Haymarket_MagistrateHall_EventScript_Clerk',
          graphics_id='OBJ_EVENT_GFX_MAN_2')
    t.drop(script='OldaleTown_House2_EventScript_Man')
    t.save()

    # Counting house: front desk downstairs, the clerks and Penn's Pokémon upstairs
    t = Map('Haymarket_CountingHouse')
    t.set({'script': 'Haymarket_CountingHouse_EventScript_Receptionist'},
          script='Haymarket_CountingHouse_EventScript_FrontClerk')
    t.drop(script='Haymarket_CountingHouse_EventScript_Clerk')
    t.save()
    t = Map('Haymarket_CountingHouse_BackRoom')
    t.set({'script': 'Haymarket_CountingHouse_BackRoom_EventScript_Clerk'},
          script='Haymarket_CountingHouse_EventScript_Clerk1')
    if not t.has(script='Haymarket_CountingHouse_EventScript_Clerk2'):
        t.add(npc('OBJ_EVENT_GFX_AQUA_MEMBER_M', 13, 6, 'Haymarket_CountingHouse_EventScript_Clerk2',
                  move='MOVEMENT_TYPE_FACE_LEFT', trainer=True, sight=3))
    t.set({'local_id': 'LOCALID_COUNTING_HOUSE_PENNS_POKEMON'}, local_id='LOCALID_COUNTING_HOUSE_PENN_BALL',
          script='Haymarket_CountingHouse_EventScript_PennBall', flag='FLAG_HIDE_COUNTING_HOUSE_PENN_BALL')
    t.m['bg_events'] = []
    t.signs({(2, 4): 'Haymarket_CountingHouse_EventScript_CraneDesk',
             (10, 4): 'Haymarket_CountingHouse_EventScript_PaymentShelves',
             (11, 4): 'Haymarket_CountingHouse_EventScript_PaymentShelves'})
    t.save()

    t = Map('Haymarket_Granary')
    if not t.has(script='Haymarket_Granary_EventScript_Worker'):
        t.add(npc('OBJ_EVENT_GFX_MAN_4', 17, 7, 'Haymarket_Granary_EventScript_Worker',
                  move='MOVEMENT_TYPE_FACE_LEFT'))
    t.m['bg_events'] = [b for b in t.m['bg_events'] if b['script'] != 'Haymarket_Granary_EventScript_WeightsCrate']
    t.signs({(19, 4): 'Haymarket_Granary_EventScript_CrownCrate',
             (13, 8): 'Haymarket_Granary_EventScript_Sacks'})
    t.save()

    # The Gilt Pavilion
    t = Map('Haymarket_GiltPavilion')
    t.set({'local_id': 'LOCALID_GILT_PAVILION_CORWIN'}, local_id='LOCALID_PAVILION_CORWIN',
          script='GiltPavilion_EventScript_Corwin')
    t.set({'local_id': 'LOCALID_GILT_PAVILION_AUCTIONEER'}, local_id=None, script='GiltPavilion_EventScript_Auctioneer',
          x=3, y=24, movement_type='MOVEMENT_TYPE_FACE_RIGHT')
    t.set({'script': 'Haymarket_GiltPavilion_EventScript_Fenwick'}, script='GiltPavilion_EventScript_Broker1')
    t.set({'script': 'Haymarket_GiltPavilion_EventScript_Celeste'}, script='GiltPavilion_EventScript_Bidder')
    t.set({'script': 'Haymarket_GiltPavilion_EventScript_Albrecht'}, script='GiltPavilion_EventScript_Broker2')
    for s in ('SpectatorA', 'SpectatorB', 'SpectatorC', 'Steward'):
        t.drop(script='Haymarket_GiltPavilion_EventScript_' + s)
    t.m['bg_events'] = []
    t.signs({(1, 24): 'GiltPavilion_EventScript_Gallery', (7, 24): 'GiltPavilion_EventScript_Gallery'})
    t.save()


# ---- placeholder scripts ------------------------------------------------------
def referenced_labels():
    """Every script label a map.json or an assembly file outside the stub blocks names."""
    refs = set()
    for d in os.listdir(path('data/maps')):
        p = path('data/maps', d, 'map.json')
        if os.path.exists(p):
            refs |= set(re.findall(r'"script": "([A-Za-z0-9_]+)"', open(p).read()))
    return refs


def remove_dead_stubs():
    refs = referenced_labels()
    for d in sorted(os.listdir(path('data/maps'))):
        p = path('data/maps', d, 'scripts.inc')
        if not os.path.exists(p):
            continue
        s = open(p).read()
        if STUB_HEADER not in s:
            continue
        head, stubs = s.split(STUB_HEADER, 1)
        # blocks start at a "Label::" line
        blocks = re.split(r'\n(?=[A-Za-z0-9_]+::\n)', stubs.strip('\n'))
        keep, labels = [], {}
        for b in blocks:
            m = re.match(r'([A-Za-z0-9_]+)::', b)
            if m:
                labels[m.group(1)] = b
        # a block is live if a map points at it or a live block's text/movement lives in it
        live = {l for l in labels if l in refs}
        for l, b in labels.items():
            if l in live:
                live |= set(re.findall(r'\b([A-Za-z0-9_]+_Text_[A-Za-z0-9_]+)\b', b))
        keep = [b for l, b in labels.items() if l in live]
        if keep:
            out = head.rstrip('\n') + '\n\n' + STUB_HEADER + '\n' + '\n\n'.join(keep) + '\n'
        else:
            out = head.rstrip('\n') + '\n'
        if out != s:
            open(p, 'w').write(out)
            print('%s: kept %d of %d placeholder blocks' % (d, len(keep), len(labels)))


def main():
    lowmere()
    mire_road()
    haymarket()
    remove_dead_stubs()
    print('wired the slice maps to the dialogue scripts')


if __name__ == '__main__':
    main()
