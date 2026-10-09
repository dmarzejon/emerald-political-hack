# Slice maps

The vertical slice runs Lowmere → the Mire Road → Haymarket, ending at Corwin's palace, the Gilt Pavilion.
The maps reuse vanilla map slots so that vanilla scripts, flags and the map-group tables keep working.

| Place | Map (slot) | Layout | Size | Secondary tileset |
|---|---|---|---|---|
| Lowmere | `MAP_LITTLEROOT_TOWN` | `LAYOUT_LOWMERE_STAGE0`…`4` | 32×26 | `gTileset_Lowmere` |
| The Mire Road | `MAP_ROUTE101` | `LAYOUT_ROUTE101` | 24×44 | `gTileset_Lowmere` |
| Haymarket | `MAP_OLDALE_TOWN` | `LAYOUT_OLDALE_TOWN` | 48×39 | `gTileset_Haymarket` |
| Gilt Pavilion | `MAP_HAYMARKET_GILT_PAVILION` (new) | `LAYOUT_HAYMARKET_GILT_PAVILION` | 9×26 | `gTileset_PetalburgGym` |

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
| 2 | The shed, Birch's lab slot (20,6) | `MAP_LITTLEROOT_TOWN_PROFESSOR_BIRCHS_LAB` |
| 3 | The forge (3,14) | `MAP_LOWMERE_FORGE` (new) |
| 4 | The reeve's house (10,6) | `MAP_LOWMERE_REEVES_HOUSE` (new) |
| 5 | House C (10,21), open from stage 1 | `MAP_LOWMERE_REOPENED_HOUSE` (new) |

The north exit is at columns 15–16 and leads onto the Mire Road.

### Lowmere people

These are new `LOCALID_LOWMERE_*` objects: `BRAM` (9,9), `TOFT` (11,8), `TAMSIN` (5,15), `COURIER` (8,8), `JETTY_VILLAGER` (18,20), `HEALER_VILLAGER` (13,12), `SHOP_VILLAGER` (19,12), `DREAMS_VILLAGER` (23,14), `GRAIN_SELLER` (13,17) and `STALL_KEEPER` (19,17).
`LOCALID_LOWMERE_HESK` stands at (4,4) inside Hesk's house.
The vanilla objects (Mom, the trucks, the rival, Birch, the twin, the fat man and the boy) keep their local ids and were moved to fit the new streets.

## The Mire Road

This is a winding marsh road. The reeds and the tall grass at the north end give wild encounters.

- **North end:** seven rows of dry ground, so the seam with Haymarket draws cleanly (see below).
- **Middle:** the Gilded Scale wagon is stuck in the mud at (10,16). The `LOCALID_MIRE_ROAD_WAGON_CLERK` trainer stands at (11,18) and `LOCALID_MIRE_ROAD_RECEIPT_CLERK` at (13,17).
- **Below the wagon:** a boardwalk crosses the pond at rows 23–24.
- **South end:** the vanilla Birch-rescue objects and triggers.

| Trainer | Slot | Spot |
|---|---|---|
| Hobb | `TRAINER_RICK` | (11,6) |
| Grisk | `TRAINER_TIANA` | (5,21) |
| Willem | `TRAINER_ANDREW` | (16,17) |
| Pip | `TRAINER_ALLEN` | (9,33) |
| Wagon clerk | `TRAINER_GRUNT_PETALBURG_WOODS` | (11,18) |

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

These are the town's objects:

- **At the fair:** `LOCALID_HAYMARKET_CORWIN` (29,14), fairgoers A, B and C (26,14), (32,14) and (28,13), and `CRANE` (16,13).
- **The seizure:** `PENN` (28,20) between `SEIZING_CLERK_A` (27,20) and `SEIZING_CLERK_B` (29,20).
- **At the Pavilion door:** `LOCALID_HAYMARKET_PAVILION_GUARD` stands on (28,12) and blocks the door. A script has to move him or hide him behind a flag.
- **Unnamed townsfolk:** a townsman, a townswoman and a grain buyer.

`LOCALID_HAYMARKET_PENN_HOME` is inside House 1 at (6,3), and `LOCALID_HAYMARKET_ARDEN` is inside House 2 at (6,3).

These are the exits:

| Exit | Leads to |
|---|---|
| North, columns 27–30 | Route 103 |
| South, columns 27–28 | The Mire Road |
| West, rows 18–19 | Route 102 |

### Haymarket interiors

- **Gilt Pavilion:** Corwin sits on the throne at (4,4), with the auctioneer at (6,5). The auction floor holds Fenwick (`TRAINER_MARC`), Celeste (`TRAINER_TOMMY`) and Albrecht (`TRAINER_JOSH`). Spectators and a steward fill the room, and the music is `MUS_GYM`.
- **Counting house:** this reuses the Devon Corp 1F layout. A clerk trainer (`TRAINER_GRUNT_MUSEUM_1`) stands guard, and stairs lead to a back room.
- **Counting house back room:** this reuses Devon Corp 2F. It holds a clerk trainer (`TRAINER_GRUNT_MUSEUM_2`), Penn's Pokémon as an item ball (`LOCALID_COUNTING_HOUSE_PENNS_POKEMON`), and Crane's desk as a sign.
- **Granary:** this reuses Stern's Shipyard 1F. It holds the foreman (`TRAINER_GRUNT_RUSTURF_TUNNEL`), four Strength boulders as stacked sacks, and the weights crate and the scale as signs. The boulders need Strength or the features thread's sack push.

## What other work owns

- **Dialogue** owns `scripts.inc`. Every new object and sign points at a placeholder script at the end of its map's `scripts.inc`, below the `Map-thread placeholders` marker. Dialogue can rename those scripts and set the `flag` and `script` fields in `map.json`.
- **Vanilla movement scripts** still assume the old Littleroot and Oldale layouts. These include Mom and the rival in Littleroot and the Mart employee's walk to the Center in Oldale. They need rewriting or cutting.
- **Heal locations** in `src/data/heal_locations.json` were moved to the new door spots for the Lodge, Hesk's house and the Haymarket Pokémon Center.
- **Trainer slots** are listed above. The balance work owns the parties.

## Seams between maps with different tilesets

When you stand near a map edge, the game draws the neighbouring map's tiles with the tileset of the map you are in. Those tiles stay drawn after you cross. So within about 7 rows or 8 columns of a seam, both maps must use metatiles that look the same in both tilesets. In practice that means the primary (General) set: trees, grass, dirt and sand roads.

Haymarket's frame and the Mire Road's dry north end exist for this reason. Lowmere and the Mire Road share a tileset, so their seam has no limit. `validate.py` checks every seam the slice maps touch.

## Tilesets

- **`gTileset_Lowmere`** (`data/tilesets/secondary/lowmere`) is Petalburg with additions, so every Petalburg metatile id still works. It adds:
  - weathered and boarded houses
  - marsh, boardwalk and berry soil from Fortree
  - Slateport stall awnings
  - hand-drawn props: the well and broken well, fences, sacks, crates, the cart, lanterns, stumps, reeds (tall-grass behaviour), graves and mud (puddle behaviour)
  - names for the added metatiles, in `dev_scripts/slice_maps/lowmere_ids.json`
- **`gTileset_Haymarket`** (`data/tilesets/secondary/haymarket`) is Slateport plus a gilded Battle Tent dome. The dome uses palette 12.

## Regenerating

The scripts in `dev_scripts/slice_maps/` built everything above. Run them from the repository root:

```sh
python3 dev_scripts/slice_maps/build_tileset.py   # tilesets (only if the art changes)
python3 dev_scripts/slice_maps/build_maps.py      # layouts
python3 dev_scripts/slice_maps/build_events.py    # map.json events, new interiors, placeholder scripts
python3 dev_scripts/slice_maps/validate.py        # reachability and seam checks
```

`build_events.py` starts from origin/main's `map.json` and overwrites the events of every map it touches. Once dialogue edits those maps, change them by hand, or in Porymap, rather than re-running it. `build_maps.py` only writes layouts, so it is safe to re-run.
