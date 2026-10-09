# Slice events: dialogue and script contract

This page is the contract between the dialogue thread (scripts and text), the map thread
(maps, object and trigger placement), the features thread (flags, vars, items, the town
upgrade) and the balance thread (trainer teams and levels). Story source:
[vertical-slice.md](../story/vertical-slice.md) in the story bible.

## Where things live

| What | File |
| --- | --- |
| Slice scenes, NPCs and their text | `data/scripts/slice/*.inc` (one file per area) |
| Shared macros, speaker names, temporary constants | `data/scripts/slice/common.inc` |
| New-game narration (replaces Birch's speech) | `data/text/birch_speech.inc` |
| Court prologue hook | `data/maps/InsideOfTruck/scripts.inc` (on-frame, `VAR_SLICE_STATE == 0`) |

Labels are named by **area**, not by map folder (`Lowmere_…`, `MireRoad_…`, `Haymarket_…`,
`GiltPavilion_…`), so they work whichever map slot the map thread picks. A map's
`map.json` can point an object, trigger or sign straight at one of these labels. Each map's
own `scripts.inc` only needs the map-script header (on-frame and on-transition tables),
which the dialogue thread fills in once the map exists.

Names in a box use the engine's name box (`setspeaker`). Proper names are in capitals, as
in Emerald. Every line that names the player's title has a boy (`_M`) and girl (`_F`)
version.

## Temporary constants

Until the owning thread defines the real names, `common.inc` (and the top of each area
file) defines fallbacks under `#ifndef`. A fallback disappears as soon as the real
constant exists, so the owning thread can add it with no change here.

- **Features:** every `VAR_*` and `FLAG_*` below currently borrows a `VAR_UNUSED_*` or
  `FLAG_UNUSED_*` slot. Please define them for real (any slot), and keep the names or tell
  the dialogue thread the new ones.
- **Balance:** every `TRAINER_*` below borrows a vanilla early trainer so the battle runs.
  `KINGS_GIFT_LEVEL` (default 5) is the level of the king's gift.
- **Map:** every `LOCALID_*` below is a placeholder number. Give the object that
  `local_id` in `map.json` and it takes over.
- **Items:** the Silver Wing, Bell Receipt and Sealed Letter are flag-only for now (with
  an "obtained" message). Once features adds `ITEM_SILVER_WING`, `ITEM_BELL_RECEIPT` and
  `ITEM_SEALED_LETTER` as key items, the scripts switch to `giveitem`.

## Story progress: `VAR_SLICE_STATE`

| Value | Meaning | Set by |
| --- | --- | --- |
| 0 | New game, in the cart | |
| 1 | Prologue seen | `Prologue_EventScript_Apportionment` |
| 2 | Arrived in Lowmere, Bram greeted | `Lowmere_EventScript_Arrival` |
| 3 | Toft's ledger seen; go to the Ranger's Shed | `Lowmere_OldLodge_EventScript_ToftLedger` |
| 4 | Starter, Pokédex, running shoes, 5 Poké Balls received | `Lowmere_RangersShed_EventScript_ChooseStarter` |
| 5 | Silver Wing received; Tamsin waits on the north road | `Lowmere_HeskHouse_EventScript_Hesk` |
| 6 | Tamsin beaten; courier waits at the Old Lodge | `Lowmere_EventScript_TamsinRoad` |
| 7 | King's gift chosen; Mire Road open | `Lowmere_OldLodge_EventScript_KingsGift` |
| 8 | Tithe wagon event done, Bell Receipt found | `MireRoad_EventScript_WagonClerk` |
| 9 | Seizure seen, Corwin refused a Trial; case open | `Haymarket_EventScript_Arrival` |
| 10 | Trial granted by the magistrate | `Haymarket_MagistrateHall_EventScript_Arden` |
| 11 | Crane's offer refused | `Haymarket_EventScript_Crane` |
| 12 | Corwin beaten, pact signed | `GiltPavilion_EventScript_Corwin` |
| 13 | Back in Lowmere, Stage 1 (end of slice) | `Lowmere_EventScript_GrainArrives` |

Other vars: `VAR_LOWMERE_STAGE` (set to 1 by the return scene), `VAR_KINGS_GIFT_DOG`
(0 Entei, 1 Raikou, 2 Suicune).

Flags: `FLAG_RECEIVED_SILVER_WING`, `FLAG_RECEIVED_KINGS_GIFT`, `FLAG_FOUND_BELL_RECEIPT`,
`FLAG_HAYMARKET_WEIGHTS_FOUND`, `FLAG_PENN_MON_RECOVERED`, `FLAG_HAYMARKET_WITNESS`,
`FLAG_HAYMARKET_TRIAL_GRANTED`, `FLAG_MET_CRANE`, `FLAG_PACT_HAYMARKET`,
`FLAG_CORWIN_PRIVATE_TALK`, `FLAG_RECEIVED_SEALED_LETTER`, `FLAG_TALKED_TO_CARTER`,
`FLAG_TAMSIN_MIRE_ROAD_TALK`. Vanilla flags reused: `FLAG_SYS_POKEMON_GET`,
`FLAG_SYS_POKEDEX_GET`, `FLAG_SYS_B_DASH`, `FLAG_BADGE01_GET`.

## Placement per area

"Hide flag" means the object's `flag` in `map.json`: the object is hidden while the flag is
set. Unless noted, an object with a hide flag starts hidden only if the table says so
(features or the map's on-transition script sets the starting value).

Triggers that say "while state is N" are coord events with `var: VAR_SLICE_STATE`,
`var_value: N`. Where a trigger covers several values, add one coord event per value.

### Lowmere

Outside (Lowmere map):

| Kind | Label | Who / what | `local_id` | Hide flag | Notes |
| --- | --- | --- | --- | --- | --- |
| Map script, on frame | `Lowmere_EventScript_Arrival` | | | | when state is 1, right after stepping off the cart |
| Map script, on frame | `Lowmere_EventScript_GrainArrives` | | | | when state is 12 (first entry after the pact) |
| Object | `Lowmere_EventScript_BramAtCart` | Bram, beside the cart | `LOCALID_LOWMERE_BRAM` | `FLAG_HIDE_LOWMERE_ARRIVAL_BRAM` | visible at the start |
| Object | `Lowmere_EventScript_TamsinForge` | Tamsin at the forge | | `FLAG_HIDE_LOWMERE_TAMSIN_FORGE` | visible at the start |
| Object | `Lowmere_EventScript_TamsinRoad` | Tamsin on the north road, facing south | `LOCALID_LOWMERE_TAMSIN_ROAD` | `FLAG_HIDE_LOWMERE_TAMSIN_ROAD` | hidden at the start |
| Trigger | `Lowmere_EventScript_TamsinRoad` | one row in front of Tamsin | | | while state is 5 |
| Trigger | `Lowmere_EventScript_NorthRoadBlock` | across the north exit | | | while state is 2, 3, 4 or 6; steps the player back south |
| Object | `Lowmere_EventScript_Fisherman` | by the broken jetty | | | |
| Object | `Lowmere_EventScript_Mother` | | | | |
| Object | `Lowmere_EventScript_OldMan` | | | | |
| Object | `Lowmere_EventScript_Child` | | | | |
| Object | `Lowmere_EventScript_Stallkeeper` | Marta's stall, Stage 1+ | | | show only at Stage 1+ |
| Object | (none) | grain carts for the return scene | | `FLAG_HIDE_LOWMERE_GRAIN_CARTS` | optional set dressing |
| Sign | `Lowmere_EventScript_TownSign` | town sign | | | |
| Sign | `Lowmere_EventScript_OldLodgeSign` / `_RangersShedSign` / `_HeskHouseSign` | house signs | | | |
| Sign | `Lowmere_EventScript_Well` | the well | | | text changes at Stage 1 |
| Sign | `Lowmere_EventScript_GrainStore` | grain store door | | | text changes at Stage 1 |
| Sign | `Lowmere_EventScript_BoardedHouse` | any boarded door | | | |
| Sign | `Lowmere_EventScript_Jetty` | end of the jetty | | | |

The Old Lodge (replaces the player's house):

| Kind | Label | Who / what | `local_id` | Hide flag | Notes |
| --- | --- | --- | --- | --- | --- |
| Map script, on frame | `Lowmere_OldLodge_EventScript_ToftLedger` | | | | when state is 2 |
| Map script, on frame | `Lowmere_OldLodge_EventScript_KingsGift` | | | | when state is 6 |
| Object | `Lowmere_EventScript_Toft` | Reeve Toft at a desk | `LOCALID_OLD_LODGE_TOFT` | `FLAG_HIDE_OLD_LODGE_TOFT` | visible at the start |
| Object | `Lowmere_OldLodge_EventScript_Bram` | Bram | `LOCALID_OLD_LODGE_BRAM` | `FLAG_HIDE_OLD_LODGE_BRAM` | hidden at the start; shown from state 6 |
| Object | `Lowmere_OldLodge_EventScript_Courier` | royal courier, muddy | `LOCALID_OLD_LODGE_COURIER` | `FLAG_HIDE_OLD_LODGE_COURIER` | hidden at the start; shown at state 6 |
| Sign | `Lowmere_OldLodge_EventScript_Ledger` | ledger on the desk | | | |

The Ranger's Shed (replaces Birch's Lab): Bram, `Lowmere_RangersShed_EventScript_Bram`.
He gives the starter through `special ChooseStarter` (the bag menu), so no Poké Ball
objects are needed.

Mother Hesk's house (Elena's old house): Hesk, `Lowmere_HeskHouse_EventScript_Hesk`;
the bed, sign `Lowmere_HeskHouse_EventScript_ElenasBed`.

### Mire Road

| Kind | Label | Who / what | `local_id` | Hide flag | Notes |
| --- | --- | --- | --- | --- | --- |
| Object | `MireRoad_EventScript_Tamsin` | Tamsin at the foot of the road | `LOCALID_MIRE_ROAD_TAMSIN` | `FLAG_HIDE_MIRE_ROAD_TAMSIN` | hidden at the start; shown at state 7 |
| Trigger | `MireRoad_EventScript_TamsinJoins` | first rows of the route | | | while state is 7 |
| Object | `MireRoad_EventScript_WagonClerk` | guild clerk 1, by the wagon | `LOCALID_MIRE_ROAD_WAGON_CLERK` | `FLAG_HIDE_MIRE_ROAD_WAGON_CLERKS` | |
| Object | (none) | guild clerk 2, by the wagon | `LOCALID_MIRE_ROAD_WAGON_CLERK_2` | `FLAG_HIDE_MIRE_ROAD_WAGON_CLERKS` | |
| Trigger | `MireRoad_EventScript_WagonClerk` | row in front of the wagon, mid-route | | | while state is 7 |
| Object or sign | `MireRoad_EventScript_Wagon` | the stuck grain wagon | | | |
| Trainer | `MireRoad_EventScript_Clerk` | guild clerk | | | `TRAINER_MIRE_ROAD_CLERK` |
| Trainer | `MireRoad_EventScript_Poacher` | poacher | | | `TRAINER_MIRE_ROAD_POACHER` |
| Trainer | `MireRoad_EventScript_Youngster` | youngster | | | `TRAINER_MIRE_ROAD_YOUNGSTER` |
| Trainer | `MireRoad_EventScript_Carter` | grain carter | | | `TRAINER_MIRE_ROAD_CARTER`; gives the weights lead |
| Sign | `MireRoad_EventScript_RouteSign` | route sign | | | |
| Object | (none) | Tamsin beside the wagon, looking at the sacks | `LOCALID_MIRE_ROAD_TAMSIN_WAGON` | `FLAG_HIDE_MIRE_ROAD_TAMSIN_WAGON` | hidden at the start; shown at state 7 |

### Haymarket

| Kind | Label | Who / what | `local_id` | Hide flag | Notes |
| --- | --- | --- | --- | --- | --- |
| Trigger | `Haymarket_EventScript_Arrival` | where the square comes into view, within ~4 tiles of Penn's stall | | | while state is 8 |
| Object | `Haymarket_EventScript_Penn` | Widow Penn behind her egg stall, facing down | `LOCALID_HAYMARKET_PENN` | | |
| Object | (none) | seizure clerk 1, directly **left** of Penn, facing right | `LOCALID_HAYMARKET_SEIZURE_CLERK_1` | `FLAG_HIDE_HAYMARKET_SEIZURE_CLERKS` | |
| Object | (none) | seizure clerk 2, directly **right** of Penn, facing left | `LOCALID_HAYMARKET_SEIZURE_CLERK_2` | `FLAG_HIDE_HAYMARKET_SEIZURE_CLERKS` | |
| Object | (none) | Corwin on the fair stage | `LOCALID_HAYMARKET_CORWIN` | `FLAG_HIDE_HAYMARKET_CORWIN_SQUARE` | |
| Object | `Haymarket_EventScript_Tamsin` | Tamsin near the Pokémon Center | `LOCALID_HAYMARKET_TAMSIN` | `FLAG_HIDE_HAYMARKET_TAMSIN` | hidden until the arrival scene |
| Object | (none) | Silas Crane outside the Magistrate's Hall | `LOCALID_HAYMARKET_CRANE` | `FLAG_HIDE_HAYMARKET_CRANE` | hidden at the start |
| Trigger | `Haymarket_EventScript_Crane` | in front of the Magistrate's Hall door | | | while state is 10 |
| Object | `Haymarket_EventScript_PavilionGuard` | guard beside the Gilt Pavilion door | | | |
| Trigger | `Haymarket_EventScript_PavilionDoorTrigger` | the tile in front of the Pavilion door | | | while state is 8 or 9; steps the player back south |
| Objects | `Haymarket_EventScript_Farmer` / `_Granny` / `_RibbonSeller` / `_Girl` / `_Boy` / `_NervousMan` | townsfolk | | | |
| Signs | `Haymarket_EventScript_TownSign` / `_PavilionSign` / `_CountingHouseSign` / `_GranarySign` / `_MagistrateSign` | | | | |

Interiors:

| Map | Kind | Label | Notes |
| --- | --- | --- | --- |
| Magistrate's Hall | Object | `Haymarket_MagistrateHall_EventScript_Arden` | Magistrate Lyle Arden |
| Magistrate's Hall | Object | `Haymarket_MagistrateHall_EventScript_Clerk` | court clerk |
| Granary | Trainer | `Haymarket_Granary_EventScript_Foreman` | `TRAINER_HAYMARKET_FOREMAN`, guards the scale room past the sack puzzle |
| Granary | Sign | `Haymarket_Granary_EventScript_Scale` | the scale; sets `FLAG_HAYMARKET_WEIGHTS_FOUND` |
| Granary | Sign | `Haymarket_Granary_EventScript_CrownCrate` | crate of Crown weights in the corner |
| Granary | Sign | `Haymarket_Granary_EventScript_Sacks` | sacks |
| Granary | Object | `Haymarket_Granary_EventScript_Worker` | |
| Counting house | Object | `Haymarket_CountingHouse_EventScript_FrontClerk` | front desk |
| Counting house | Trainer | `Haymarket_CountingHouse_EventScript_Clerk1` / `_Clerk2` | back room, `TRAINER_HAYMARKET_CLERK_1` / `_2` |
| Counting house | Object | `Haymarket_CountingHouse_EventScript_PennBall` | Poké Ball sprite on the shelf, `local_id` `LOCALID_COUNTING_HOUSE_PENN_BALL`, hide flag `FLAG_HIDE_COUNTING_HOUSE_PENN_BALL` |
| Counting house | Sign | `Haymarket_CountingHouse_EventScript_PaymentShelves` | shelves of seized Poké Balls |
| Counting house | Sign | `Haymarket_CountingHouse_EventScript_CraneDesk` | Crane's desk (sealed letter after the pact) |

### Gilt Pavilion

| Kind | Label | Notes |
| --- | --- | --- |
| Object | `GiltPavilion_EventScript_Auctioneer` | the Gym guide's job, by the door |
| Trainers | `GiltPavilion_EventScript_Broker1` / `_Bidder` / `_Broker2` | courtiers, `TRAINER_PAVILION_BROKER_1`, `TRAINER_PAVILION_BIDDER`, `TRAINER_PAVILION_BROKER_2` |
| Object | `GiltPavilion_EventScript_Corwin` | Corwin on the throne, `local_id` `LOCALID_PAVILION_CORWIN`, `TRAINER_CORWIN` |
| Signs | `GiltPavilion_EventScript_Gallery` | the galleries along the walls |
| Callable | `GiltPavilion_EventScript_LotCallSeven` / `_LotCallTwelve` / `_LotCallTwenty` / `_WrongLot` | auctioneer lines for the lot-number floor puzzle; `call` them from the puzzle's triggers |

Trainer slide for Corwin's last Pokémon (Type: Null), for whoever adds trainer slides:
"And now, the final lot! Feast your eyes!"

### Court prologue

Plays in `InsideOfTruck` (the cart) over a black screen on a new game. If the map thread
builds a throne-room map, `Prologue_EventScript_Apportionment` can run there instead.

## Known gaps

- Scenes are written but not yet wired into maps (except the prologue). They go live as
  the map thread places the events above.
- The Lowmere return scene sets `VAR_LOWMERE_STAGE` to 1 behind a fade; the map refresh
  that shows the new layout is the features thread's API.
- The vanilla Littleroot intro (Mom, the clock, Birch) still runs after the prologue until
  the Lowmere map lands.
