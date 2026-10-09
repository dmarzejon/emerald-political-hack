# Slice maps

The vertical slice runs Lowmere → the Mire Road → Haymarket, ending at Corwin's palace, the Gilt Pavilion.
The maps reuse vanilla map slots so that vanilla scripts, flags and the map-group tables keep working.

| Place | Map (slot) | Layout | Size | Secondary tileset |
|---|---|---|---|---|
| Lowmere | `MAP_LITTLEROOT_TOWN` | `LAYOUT_LOWMERE_STAGE0`…`4` | 32×26 | `gTileset_Lowmere` |
| The Mire Road | `MAP_ROUTE101` | `LAYOUT_ROUTE101` | 24×44 | `gTileset_Lowmere` |
| Haymarket | `MAP_OLDALE_TOWN` | `LAYOUT_OLDALE_TOWN` | 48×39 | `gTileset_Haymarket` |
| Gilt Pavilion | `MAP_HAYMARKET_GILT_PAVILION` (new) | `LAYOUT_HAYMARKET_GILT_PAVILION` | 13×27 | `gTileset_GiltPavilion` |

No wild Pokémon appear while the party is empty (`StandardWildEncounter` in `src/wild_encounter.c`), so Lowmere's reeds are safe before the Ranger's Shed hands out a starter.

Map name popups still say LITTLEROOT TOWN, ROUTE 101 and OLDALE TOWN until the `MAPSEC_*` names are renamed.

## Lowmere's upgrade stages

`MAP_LITTLEROOT_TOWN` names `LAYOUT_LOWMERE_STAGE0`. The engine hook from the features work (described in `docs/systems.md`) swaps in `LAYOUT_LOWMERE_STAGE<n>` from `VAR_LOWMERE_STAGE` on every load.
All five layouts are the same size and keep every door, exit and NPC spot in the same place. A stage only changes what you see and which doors open.

| Stage | What changes |
|---|---|
| 0 | Marsh paths with mud, a broken well, two empty stall frames, broken fences, houses A, B and C boarded up, and a collapsed jetty. |
| 1 | Dirt roads, the well repaired, two working stalls with sacks and crates, fences mended, and house C reopened. |
| 2 | A full jetty, berry plots and flowers, a Pokémon Center on house B's lot, and house A reopened. |
| 3 | Lanterns, restored roofs, a Poké Mart on the empty lot, and more flowers. |
| 4 | The same as stage 3 for now. |

A boarded door has no door behaviour, so its warp does nothing until a later stage swaps in a real door. The stage 2 Pokémon Center and stage 3 Mart do not have warps or interiors yet.

### Lowmere doors (warp ids)

| Id | Door | Leads to |
|---|---|---|
| 0 | Hesk's house (25,13) | `MAP_LITTLEROOT_TOWN_MAYS_HOUSE_1F` |
| 1 | The Lodge, the player's home (6,7) | `MAP_LITTLEROOT_TOWN_BRENDANS_HOUSE_1F` |
| 2 | The Ranger's Shed, Birch's lab slot (20,6) | `MAP_LITTLEROOT_TOWN_PROFESSOR_BIRCHS_LAB` |
| 3 | The forge (3,14) | `MAP_LOWMERE_FORGE` (new) |
| 4 | The reeve's house (10,6) | `MAP_LOWMERE_REEVES_HOUSE` (new) |
| 5 | House C (10,21), open from stage 1 | `MAP_LOWMERE_REOPENED_HOUSE` (new) |

The north exit is at columns 15–16 and leads onto the Mire Road.

### Lowmere events

Every object, sign and trigger runs a script from `data/scripts/slice/`. The labels and flags are listed in [the dialogue contract](dialogue/events.md).

- **Outside:**
  - Bram is at (9,9) (`LOCALID_LOWMERE_BRAM`).
  - Tamsin works at the forge, (5,15).
  - On the north road, Tamsin waits at (16,0) (`LOCALID_LOWMERE_TAMSIN_ROAD`).
  - The fisherman, mother, old man, child and stallkeeper are townsfolk.
  - Signs mark the town, the Lodge, the Shed, Hesk's house, the well, the grain store (house A's door), the boarded house (house C's door) and the jetty.
- **North road triggers:** `TamsinRoad` (state 5) and `NorthRoadBlock` (states 2, 3, 4 and 6) both sit on row 1, at (15,1) and (16,1). Row 1 is the only way out.
- **The Old Lodge** (Brendan's house 1F):
  - Toft is at (5,7), Bram at (7,6) and the courier at (8,6).
  - The ledger sign is on the table at (4,6).
- **The Ranger's Shed** (`LAYOUT_LOWMERE_RANGERS_SHED`): Mr. Briney's beamed cottage with the Fossil Maniac's mounted trophy on the back wall. Bram stands at (5,4). The shelf is the field guides, the clay pots are the trap cages, the table holds the logbook and the cabinet at (8,1) is the PC.
- **Hesk's house:** Hesk is at (4,4) downstairs, and Elena's bed is a sign at (7,5) upstairs.
- **Vanilla objects still there:** Mom, the Twin, the trucks, the rival and Birch keep their spots, because vanilla scripts still name their local ids. The dialogue work removes them together with those scripts.

## The Mire Road

This is a winding marsh road. The reeds and the tall grass at the north end give wild encounters.

- **North end:** seven rows of dry ground, so the seam with Haymarket draws cleanly (see below).
- **Middle:** the Gilded Scale wagon is stuck in the mud at (10,16).
  - The two guild clerks stand at (11,18) and (13,17) (`LOCALID_MIRE_ROAD_WAGON_CLERK`, `_2`).
  - Tamsin's spot by the sacks is (9,18).
  - The `WagonClerk` trigger (state 7) covers every open tile of row 20.
- **Below the wagon:** a boardwalk crosses the pond at rows 23–24.
- **South end:** Tamsin waits at (13,41). The `TamsinJoins` trigger (state 7) covers the south edge, row 43.

| Trainer | Script | Spot |
|---|---|---|
| Guild clerk | `MireRoad_EventScript_Clerk` | (11,6) |
| Poacher | `MireRoad_EventScript_Poacher` | (5,21) |
| Grain carter | `MireRoad_EventScript_Carter` | (16,17) |
| Youngster | `MireRoad_EventScript_Youngster` | (9,33) |

## Haymarket

The town is a market square under the Gilt Pavilion's gold dome. A ring of forest and dirt road frames it on the west, north and south. That ring keeps Haymarket's own tiles away from the map seams (see below).

### Haymarket doors (warp ids)

| Id | Door | Leads to |
|---|---|---|
| 0 | Widow Penn's house (13,28) | `MAP_OLDALE_TOWN_HOUSE1` |
| 1 | The magistrate's hall (14,12) | `MAP_OLDALE_TOWN_HOUSE2` |
| 2 | Pokémon Center (19,11) | `MAP_OLDALE_TOWN_POKEMON_CENTER_1F` |
| 3 | Poké Mart (34,11) | `MAP_OLDALE_TOWN_MART` |
| 4 | Gilt Pavilion (28,11) | `MAP_HAYMARKET_GILT_PAVILION` (new) |
| 5 | Counting house (41,13) | `MAP_HAYMARKET_COUNTING_HOUSE` (new) |
| 6 | Granary (39,30) | `MAP_HAYMARKET_GRANARY` (new) |

These are the town's events:

- **The seizure:** Penn is at (28,20) inside the market, with seizure clerk 1 at (27,20) and seizure clerk 2 at (29,20).
- **Hidden until their scenes:**
  - Corwin on the fair stage, (29,14)
  - Crane outside the Magistrate's Hall, (16,13)
  - Tamsin by the Pokémon Center, (21,13)
- **The Pavilion guard:** he stands beside the door at (27,12). The `PavilionDoorTrigger` (states 8 and 9) is in front of it at (28,12).
- **`Arrival` trigger (state 8):** this is where the dirt road enters town, (27,35) and (28,35). It is the only way in from the Mire Road.
- **`Crane` trigger (state 10):** this is in front of the Magistrate's door, (14,13).
- **Townsfolk:** the ribbon seller, nervous man, girl, granny, farmer and boy.
- **Signs:** the town, the Pavilion, the Magistrate's Hall, the counting house and the granary.

Magistrate Arden is at (6,3) inside House 2, with his clerk. Penn's house (House 1) is empty for now.

These are the exits:

| Exit | Leads to |
|---|---|
| North, columns 27–30 | Route 103 |
| South, columns 27–28 | The Mire Road |
| West, rows 18–19 | Route 102 |

### Haymarket interiors

- **Gilt Pavilion:** Petalburg Gym rooms set into a 13-wide hall, with spectator boxes down both sides (the galleries). Six townsfolk watch from the boxes, and four box faces are the `Gallery` signs. The music is `MUS_GYM`.
  - Corwin stands below the throne at (6,4) (`LOCALID_PAVILION_CORWIN`), and the auctioneer by the door at (5,25).
  - The courtiers are `Broker1` at (3,10), `Bidder` at (9,15) and `Broker2` at (3,17), each with a sight range of 4.
  - **The lot puzzle.** Three rows of numbered gold lots cross the hall, at rows 19, 13 and 7. On the row below each one (rows 20, 14 and 8), the auctioneer calls lot 7, then 12, then 20. Only the called lot lets you on. A wrong lot gets the auctioneer's "wrong lot" line and steps you back. A solved row stays open until you leave the Pavilion, and the floor goes quiet after the Trial. The scripts are in `data/maps/Haymarket_GiltPavilion/scripts.inc`, and they call the auctioneer's lines in `data/scripts/slice/gilt_pavilion.inc`.
- **Counting house:** this reuses the Devon Corp 1F layout. The front clerk is at the desk, and stairs lead to a back room.
- **Counting house back room:** this reuses Devon Corp 2F. Penn's Poké Ball is in the corner at (0,8) (`LOCALID_COUNTING_HOUSE_PENN_BALL`). You can only take it from (0,7) or (1,8). `Clerk2` at (0,4), facing down, watches the first, and `Clerk1` at (4,8), facing left, watches the second, so taking the ball always starts a clerk battle. Crane's desk is a sign at (2,4), and the payment shelves are signs at (10,4) and (11,4).
- **Granary** (`LAYOUT_HAYMARKET_GRANARY`): Stern's Shipyard 1F with crate stacks added. The only way east is a one-tile gap at (13,10), plugged by a sack (a Strength boulder) that you push along a crate-lined lane. The only way up to the scale room is a one-tile aisle at x=17, and the foreman at (16,6), facing right, sees anyone who steps into it. The scale, the crown crate and the sacks are signs. The granary's map script turns on Strength, so the sacks can be pushed. Leaving and coming back puts the sacks back.

## What other work owns

- **Dialogue** owns the scripts and the map-script headers (on-frame and on-transition tables).
- **Vanilla movement scripts** still assume the old Littleroot and Oldale layouts. These include Mom and the rival in Littleroot and the Mart employee's walk to the Center in Oldale. The dialogue work cuts them, along with their objects.
- **Heal locations** in `src/data/heal_locations.json` point at the new door spots for the Lodge, Hesk's house and the Haymarket Pokémon Center.
- **Balance** owns the trainer parties.
- **Not wired yet:** the forge, the reeve's house and house C have no scenes, so they are empty. The stallkeeper has no flag to hide her before stage 1.

## Seams between maps with different tilesets

When you stand near a map edge, the game draws the neighbouring map's tiles with the tileset of the map you are in. Those tiles stay drawn after you cross. So within about 7 rows or 8 columns of a seam, both maps must use metatiles that look the same in both tilesets. In practice that means the primary (General) set: trees, grass, dirt and sand roads.

Haymarket's frame and the Mire Road's dry north end exist for this reason. Lowmere and the Mire Road share a tileset, so their seam has no limit. `validate.py` checks every seam the slice maps touch.

## People's sprites

The FRLG overworld sprites (everything from `OBJ_EVENT_GFX_RED_NORMAL` on, such as the balding man and the policeman) are only compiled into FireRed and LeafGreen builds. In this Emerald build they draw nothing, so a person using one is invisible but still talks. Use Emerald sprites. `validate.py` flags any slice object that uses an FRLG sprite.

## Tilesets

- **`gTileset_Lowmere`** (`data/tilesets/secondary/lowmere`) is Petalburg with additions, so every Petalburg metatile id still works. It adds:
  - weathered and boarded houses
  - marsh, boardwalk and berry soil from Fortree
  - Slateport stall awnings
  - hand-drawn props: the well and broken well, fences, sacks, crates, the cart, lanterns, stumps, reeds (tall-grass behaviour), graves and mud (puddle behaviour)
  - names for the added metatiles, in `dev_scripts/slice_maps/lowmere_ids.json`
- **`gTileset_Haymarket`** (`data/tilesets/secondary/haymarket`) is Slateport plus a gilded Battle Tent dome. The dome uses palette 12.
- **`gTileset_GiltPavilion`** (`data/tilesets/secondary/gilt_pavilion`) is Petalburg Gym plus gold lot plates numbered 1 to 21 in palette 9. Their ids are in `dev_scripts/slice_maps/gilt_pavilion_ids.json`.

## Regenerating

The scripts in `dev_scripts/slice_maps/` built everything above. Run them from the repository root:

```sh
python3 dev_scripts/slice_maps/build_tileset.py   # tilesets (only if the art changes)
python3 dev_scripts/slice_maps/build_maps.py      # layouts
python3 dev_scripts/slice_maps/validate.py        # reachability, seam and sprite checks
```

`build_events.py` placed the first events, and `wire_events.py` then pointed them at the dialogue scripts. Both have been run, and `map.json` is now the source of truth. Change events by hand or in Porymap. Do not re-run `build_events.py` or `wire_events.py`, because they would overwrite the wiring and the later rework. `build_maps.py` only writes layouts, so it is safe to re-run.
