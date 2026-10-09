# Slice maps

The vertical slice runs Lowmere → the Mire Road → Haymarket, ending at Corwin's palace, the Gilt Pavilion.
The maps reuse vanilla map slots so that vanilla scripts, flags and the map-group tables keep working.

| Place | Map (slot) | Layout | Size | Secondary tileset |
|---|---|---|---|---|
| Lowmere | `MAP_LITTLEROOT_TOWN` | `LAYOUT_LOWMERE_STAGE0`…`4` | 32×26 | `gTileset_Lowmere` |
| The Mire Road | `MAP_ROUTE101` | `LAYOUT_ROUTE101` | 24×44 | `gTileset_Lowmere` |
| Haymarket | `MAP_OLDALE_TOWN` | `LAYOUT_OLDALE_TOWN` | 48×39 | `gTileset_Haymarket` |
| Gilt Pavilion | `MAP_HAYMARKET_GILT_PAVILION` (new) | `LAYOUT_HAYMARKET_GILT_PAVILION` | 13×27 | `gTileset_GiltPavilion` |
| The Orchard Road | `MAP_ROUTE102` | `LAYOUT_ORCHARD_ROAD` | 50×20 | `gTileset_Lowmere` |
| Thornfield | `MAP_PETALBURG_CITY` | `LAYOUT_THORNFIELD` | 30×30 | `gTileset_Lowmere` |
| Isolde's palace garden | `MAP_THORNFIELD_PALACE` (new) | `LAYOUT_THORNFIELD_PALACE` | 16×22 | `gTileset_Lowmere` |

No wild Pokémon appear while the party is empty (`StandardWildEncounter` in `src/wild_encounter.c`), so Lowmere's reeds are safe before the Ranger's Shed hands out a starter.

The map name popups come from `src/data/region_map/region_map_sections.json`: LOWMERE, MIRE ROAD, HAYMARKET, ORCHARD ROAD and THORNFIELD.

## Lowmere's upgrade stages

`MAP_LITTLEROOT_TOWN` names `LAYOUT_LOWMERE_STAGE0`. The engine hook from the features work (described in `docs/systems.md`) swaps in `LAYOUT_LOWMERE_STAGE<n>` from `VAR_LOWMERE_STAGE` on every load.
All five layouts are the same size and keep every door, exit and NPC spot in the same place. A stage only changes what you see and which doors open.

| Stage | What changes |
|---|---|
| 0 | Marsh paths with mud, a broken well, two empty stall frames, broken fences, houses A, B and C boarded up, and a collapsed jetty. |
| 1 | Dirt roads, the well repaired, two working stalls with sacks and crates, fences mended, house C reopened, and two grain carts from Haymarket, parked at (10,9) and (21,9). |
| 2 | Flagstone roads, a full jetty with two rowboats, berry plots, flowers and a row of fruit trees, a Pokémon Center on house B's lot, and house A reopened. |
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
- **The Ranger's Shed** (`LAYOUT_LOWMERE_RANGERS_SHED`): Mr. Briney's beamed cottage with the Fossil Maniac's mounted trophy on the back wall. Bram stands at (5,4). The shelf is the field guides, the clay pots are the trap cages, the table holds the logbook and the cabinet at (8,1) is the PC. Three Poké Balls (`LOCALID_SHED_BALL_1`…`3`) sit on the table at (7,4), (8,4) and (7,5). They run Bram's script and are hidden by `FLAG_SYS_POKEMON_GET` once the starter is taken.
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

Magistrate Arden stands in front of his desk at (8,3) inside House 2, facing down onto open floor, with his clerk at the table. Penn's house (House 1) is empty for now.

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
  - A servant (`GiltPavilion_EventScript_Servant`) at (3,5), facing down, heals the party before Corwin.
  - **The lot puzzle.** Three rows of numbered gold lots cross the hall, at rows 19, 13 and 7. On the row below each one (rows 20, 14 and 8), the auctioneer calls lot 7, then 12, then 20. Only the called lot lets you on. A wrong lot gets the auctioneer's "wrong lot" line and steps you back. A solved row stays open until you leave the Pavilion, and the floor goes quiet after the Trial. The scripts are in `data/maps/Haymarket_GiltPavilion/scripts.inc`, and they call the auctioneer's lines in `data/scripts/slice/gilt_pavilion.inc`.
- **Counting house:** this reuses the Devon Corp 1F layout. The front clerk is at the desk, and stairs lead to a back room.
- **Counting house back room:** this reuses Devon Corp 2F. Penn's Poké Ball is in the corner at (0,8) (`LOCALID_COUNTING_HOUSE_PENN_BALL`). You can only take it from (0,7) or (1,8). `Clerk2` at (0,4), facing down, watches the first, and `Clerk1` at (4,8), facing left, watches the second, so taking the ball always starts a clerk battle. Crane's desk is a sign at (2,4), and the payment shelves are signs at (10,4) and (11,4).
- **Granary** (`LAYOUT_HAYMARKET_GRANARY`): Stern's Shipyard 1F with crate stacks added. The only way east is a one-tile gap at (13,10), plugged by a sack (a Strength boulder) that you push along a crate-lined lane. The only way up to the scale room is a one-tile aisle at x=17, and the foreman at (16,6), facing right, sees anyone who steps into it. The scale, the crown crate and the sacks are signs. The granary's map script turns on Strength, so the sacks can be pushed. Leaving and coming back puts the sacks back.

## Chapter 2: the Orchard Road, Thornfield and Isolde's palace

The road west from Haymarket (Route 102) leads to Thornfield (Petalburg). Both keep their vanilla layout, with props added by `build_chapter2.py`, and both now use `gTileset_Lowmere`, which draws every Petalburg metatile the same. The Petalburg Gym's door now leads to a new map, Isolde's palace, which the text calls the Glasshouse. The vanilla gym map is no longer reachable.

Every event points at the dialogue thread's chapter 2 scripts (`docs/dialogue/chapter2.md`, `data/scripts/chapter2/`). The vanilla Route 102, Petalburg, Wally's house and house 1 and 2 scripts are replaced by map headers only, and their vanilla people are gone.

### The Orchard Road (`MAP_ROUTE102`)

| Event | Where | Script |
|---|---|---|
| Warden, `LOCALID_ORCHARD_ROAD_WARDEN`, `FLAG_TEMP_1`, faces right | (47,10) | `OrchardRoad_EventScript_Warden` |
| Tenant (man), faces left, beside his cart | (40,15) | `OrchardRoad_EventScript_Tenant` |
| Tenant's family (old woman, girl) | (36,15), (41,12) | `OrchardRoad_EventScript_Tenant` |
| Tenant's cart (signs) | (37–39,15) | `OrchardRoad_EventScript_TenantCart` |
| Picker, trainer, faces left, sight 3 | (19,4) | `OrchardRoad_EventScript_Picker` |
| Bailiff, trainer, faces left, sight 4 | (33,14) | `OrchardRoad_EventScript_Bailiff` |
| Poet, trainer, faces down, sight 3 | (8,7) | `OrchardRoad_EventScript_Poet` |
| Signs | (40,9), (17,2), (21,2) | `_SignEast`, `_SignWest`, `_Orchard` |

- The warden and a tree at (47,11) close the road's east end until the header hides him (`VAR_SLICE_STATE` 13 or more).
- The tenant camp sits in the middle of the road: tents at (36,12) and (38,12), a campfire at (41,13) and the cart at (37,15). Row 14 stays open, because it is the only way between the two halves of the road.
- A boarded cottage at (11,2) and a few orchard trees are scenery.
- The road's west edge at (0,6) is a tree, because Thornfield's side of that seam is the Willows' cottage.

### Thornfield (`MAP_PETALBURG_CITY`)

**The eviction.** The Willows' cottage is at (26,13), right by the road in from the Orchard Road, with its door at (27,16) and its bean field at (23–24,13–15). `Thornfield_EventScript_Arrival` is a trigger on (29,17), (29,18) and (29,19), the first tiles you step on from the road, while `VAR_THORNFIELD_STATE` is 0. It fires on the step across the seam (checked in mGBA).

| Person | Local id | Flag | Where | Facing | Script |
|---|---|---|---|---|---|
| Voss | `LOCALID_THORNFIELD_VOSS` | `FLAG_TEMP_1` | (27,18) | up | none |
| Guard 1 | `LOCALID_THORNFIELD_GUARD_1` | `FLAG_TEMP_1` | (24,17) | up | none |
| Guard 2 | `LOCALID_THORNFIELD_GUARD_2` | `FLAG_TEMP_1` | (28,17) | up | none |
| Tom Willow | `LOCALID_THORNFIELD_WILLOW` | `FLAG_TEMP_3` | (27,17) | down | `Thornfield_EventScript_Willow` |
| Mae Willow | `LOCALID_THORNFIELD_MAE` | `FLAG_TEMP_3` | (26,17) | down | `Thornfield_EventScript_Mae` |
| Pip Willow | `LOCALID_THORNFIELD_PIP` | `FLAG_TEMP_3` | (25,17) | down | `Thornfield_EventScript_Pip` |
| Digger 1 | `LOCALID_THORNFIELD_DIGGER_1` | `FLAG_TEMP_1` | (23,14) | down | none |
| Digger 2 | `LOCALID_THORNFIELD_DIGGER_2` | `FLAG_TEMP_1` | (24,14) | down | none |
| Tamsin | `LOCALID_THORNFIELD_TAMSIN` | `FLAG_TEMP_5` | (13,11) | down | `Thornfield_EventScript_Tamsin` |
| Willow at the camp | `LOCALID_THORNFIELD_CAMP_WILLOW` | `FLAG_TEMP_2` | (24,20) | up | `Thornfield_EventScript_CampWillow` |
| Mae at the camp | `LOCALID_THORNFIELD_CAMP_MAE` | `FLAG_TEMP_2` | (25,20) | up | `Thornfield_EventScript_CampMae` |
| Pip at the camp | `LOCALID_THORNFIELD_CAMP_PIP` | `FLAG_TEMP_2` | (23,20) | right | `Thornfield_EventScript_CampPip` |
| Isolde on the terraces | `LOCALID_THORNFIELD_ISOLDE` | `FLAG_TEMP_4` | (17,25) | down | `Thornfield_EventScript_IsoldeTerrace` |
| Palace guard, on the door | `LOCALID_THORNFIELD_PALACE_GUARD` | `FLAG_TEMP_6` | (15,9) | down | `Thornfield_EventScript_PalaceGuard` |
| Woman | | | (16,18) | left | `Thornfield_EventScript_Woman` |
| Old man | | | (20,10) | down | `Thornfield_EventScript_OldMan` |
| Gardener | | | (19,26) | left | `Thornfield_EventScript_Gardener` |

Signs: `_BoardedHouse` on the Willows' door (27,16), `_Terraces` at (14,26) and (24,26), `_GroveGate` at (18,2), `_TownSign` at (17,16) and `_PalaceSign` at (17,10).

**Boarding up the Willows' door.** `Thornfield_EventScript_BoardUpDoor` (called on load in states 1 and 2) should be:

```
	setmetatile 27, 15, 0x2A3, TRUE   @ boarded_door_high
	setmetatile 27, 16, 0x2A4, TRUE   @ boarded_door_low
	setmetatile 28, 15, 0x2A1, TRUE   @ boarded_window_high
	setmetatile 28, 16, 0x2A2, TRUE   @ boarded_window_low
	special DrawWholeMapView
	return
```

The `DrawWholeMapView` is needed: walking in from the Orchard Road, the door is already on screen before the map loads, and without a redraw it stays unboarded until you leave and come back by a warp. Checked in mGBA.

**Other places in town.**
- **Isolde's palace** is the old gym building. Its door is warp 2 at (15,8). The GYM plate on its front is now a window, and the sign is plain.
- **The sealed grove** is behind the palace. A one-tile path runs up between the palace and the pond, from (18,8) to the thorn gate at (18,2). The grove map itself is for Act 2.
- **Old Mabry's cottage** is house 2 (warp 4, door (20,24)). Its vegetable plot is a glasshouse at (23,21), and the yard south of it, rows 25 to 27, is the flower terraces.
- **The Steward's Office** is Wally's house (warp 1, door (7,5)), and **the Magistrate's Hall** is house 1 (warp 0, door (10,19)).
- The Mart's two wall signs are gone, because the Willows' roof is in front of them.

### Thornfield interiors

| Map | Event | Where | Script |
|---|---|---|---|
| Steward's Office (`PETALBURG_CITY_WALLYS_HOUSE`) | Voss behind the table, `LOCALID_STEWARDS_OFFICE_VOSS`, `FLAG_TEMP_1`, faces down | (5,2) | `Thornfield_StewardsOffice_EventScript_Voss` |
| | Bailiff, trainer, faces left, sight 3: the way to the table passes him | (6,5) | `Thornfield_StewardsOffice_EventScript_Bailiff` |
| | Ledger on the table (signs) | (5,4), (6,4) | `Thornfield_StewardsOffice_EventScript_Ledger` |
| Mabry's cottage (`PETALBURG_CITY_HOUSE2`) | Mabry, faces left | (7,5) | `Thornfield_MabryCottage_EventScript_Mabry` |
| | Seed shelf (signs) | (8,1), (9,1) | `Thornfield_MabryCottage_EventScript_SeedShelf` |
| Magistrate's Hall (`PETALBURG_CITY_HOUSE1`) | Fenwick behind the table, faces down | (4,2) | `Thornfield_MagistrateHall_EventScript_Fenwick` |
| | Clerk, faces left | (7,4) | `Thornfield_MagistrateHall_EventScript_Clerk` |

### Isolde's palace, the Glasshouse (`MAP_THORNFIELD_PALACE`)

A walled garden with three terraces of hedges, climbing to Isolde's flagstone dais in front of a glass wall.

| Event | Where | Script |
|---|---|---|
| Isolde on the dais, `LOCALID_PALACE_ISOLDE`, faces down | (7,3) | `Glasshouse_EventScript_Isolde` |
| Herald by the entrance, faces right | (6,20) | `Glasshouse_EventScript_Herald` |
| Gardener 1, trainer, faces right, sight 8 | (2,16) | `Glasshouse_EventScript_Gardener1` |
| Gardener 2, trainer, faces left, sight 8 | (13,13) | `Glasshouse_EventScript_Gardener2` |
| Courtier, trainer, faces right, sight 8 | (2,10) | `Glasshouse_EventScript_Courtier` |
| Servant, heals the party, faces left | (13,7) | `Glasshouse_EventScript_Servant` |
| Glass wall (signs) | (2–13,1) | `Glasshouse_EventScript_GlassWall` |
| Exit mats (warps 0 and 1 to Thornfield warp 2) | (7,21), (8,21) | |

Each terrace has a hedge row with a gap at one end, and a flower planter that leaves only one lane open. You go up through the gap at x 12–13 in row 17, along row 16 to the gap at x 2–3, along row 13 to x 12–13, along row 10 to x 2–3, and up to the dais. Each trainer stands at the far end of one lane and looks down it, so you cannot pass without a battle.

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
  - chapter 2 props: tents, a campfire, a 3×3 glasshouse, flagstones, a rowboat, a notice post, and a flagstone exit mat (south-arrow warp behaviour) for the palace garden
  - names for the added metatiles, in `dev_scripts/slice_maps/lowmere_ids.json`
- **`gTileset_Haymarket`** (`data/tilesets/secondary/haymarket`) is Slateport plus a gilded Battle Tent dome. The dome uses palette 12.
- **`gTileset_GiltPavilion`** (`data/tilesets/secondary/gilt_pavilion`) is Petalburg Gym plus gold lot plates numbered 1 to 21 in palette 9. Their ids are in `dev_scripts/slice_maps/gilt_pavilion_ids.json`.

## Regenerating

The scripts in `dev_scripts/slice_maps/` built everything above. Run them from the repository root:

```sh
python3 dev_scripts/slice_maps/build_tileset.py   # tilesets (only if the art changes)
python3 dev_scripts/slice_maps/build_maps.py      # layouts
python3 dev_scripts/slice_maps/build_chapter2.py  # chapter 2 layouts (from the vanilla Route 102 and Petalburg)
python3 dev_scripts/slice_maps/validate.py        # reachability, seam and sprite checks
```

`build_events.py` placed the first events, and `wire_events.py` then pointed them at the dialogue scripts. Both have been run, and `map.json` is now the source of truth. Change events by hand or in Porymap. Do not re-run `build_events.py` or `wire_events.py`, because they would overwrite the wiring and the later rework. `build_maps.py` and `build_chapter2.py` only write layouts, so they are safe to re-run.
