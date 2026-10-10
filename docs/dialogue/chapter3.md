# Chapter 3: Cragholt, dialogue and script contract

Chapter 3 runs from the Thornfield pact (`VAR_THORNFIELD_STATE` 3) to the Cragholt pact and
the return to Lowmere at Stage 2. The player takes the Chain Road north from Thornfield,
through Gallows Wood, watches the east tunnel come down in Cragholt, builds the case against
Prince Brannoc, digs the survivors out of the collapsed mine, wins his Trial in the Pithead
Hall and dictates the Pact of the Open Shaft. Back in Lowmere, stone and masons arrive,
Tamsin is made envoy, and a letter with a pale-bell seal is left under the Old Lodge door.

It follows the same contract as [the slice](events.md) and [chapter 2](chapter2.md): labels
are named by area, each map's `scripts.inc` holds only its header, and `map.json` points
objects, triggers and signs at the labels below. Story source:
[characters.md](../story/characters.md) (Brannoc) and [acts.md](../story/acts.md) (Act 1).

## Where things live

| What | File |
| --- | --- |
| Trainer-name and local-id fallbacks, speaker names | `data/scripts/chapter3/common.inc` |
| The Chain Road, the Penrose cottage, Gallows Wood | `data/scripts/chapter3/chain_road.inc` (`ChainRoad_…`, `GallowsWood_…`) |
| Cragholt, its interiors and the collapsed mine | `data/scripts/chapter3/cragholt.inc` (`Cragholt_…`, `CollapsedMine_…`) |
| Brannoc's palace | `data/scripts/chapter3/pithead_hall.inc` (`PitheadHall_…`) |
| The act end in Lowmere | `data/scripts/slice/lowmere.inc` (`Lowmere_OldLodge_EventScript_StoneArrives`) |

The `LOCALID_…` names below fall back, in `common.inc`, to the object numbers given in
brackets until the map thread's `map.json` files define them.

## Story progress

`VAR_CRAGHOLT_STATE` tracks the chapter:

| Value | Meaning | Set by |
| --- | --- | --- |
| 0 | Not arrived yet | |
| 1 | Collapse seen; case open | `Cragholt_EventScript_Arrival` |
| 2 | Trial granted by Magistrate Hollen | `Cragholt_MagistrateHall_EventScript_Hollen` |
| 3 | Brannoc beaten, pact signed | `PitheadHall_EventScript_Brannoc` |
| 4 | Act end seen in the Old Lodge | `Lowmere_OldLodge_EventScript_StoneArrives` |

The case needs three things:

| Evidence | Flag | Key item | Where |
| --- | --- | --- | --- |
| The bonding roll: 206 names, 61 struck through, no cause written | `FLAG_FOUND_BONDING_ROLL` | `ITEM_BONDING_ROLL` | on the barracks wall |
| The quota ledger: Brannoc's bonuses, and the last quota rise signed "M. Venn" | `FLAG_FOUND_QUOTA_LEDGER` | `ITEM_QUOTA_LEDGER` | the ore office desk |
| Wil Penrose, dug out of the collapsed mine, agrees to testify | `FLAG_RESCUED_WIL` | | the collapsed mine |

Other flags: `FLAG_FOUND_BELL_CRATE` (the Pale Choir's powder crate at the collapse face;
optional, changes Brannoc's private talk), `FLAG_MET_BRANNOC` (the arrival scene),
`FLAG_CRAGHOLT_TRIAL_GRANTED` (Hollen), `FLAG_BADGE03_GET` (the Cragholt Pick),
`FLAG_PACT_CRAGHOLT` (set by `signpact PACT_CRAGHOLT`), `FLAG_TAMSIN_ENVOY` and
`FLAG_RECEIVED_PALE_LETTER` (the act end, which gives `ITEM_PALE_LETTER`). The features
thread owns the definitions (`docs/systems.md`). Scene objects use temp flags.

The pact's fixed terms free and pay every bonded worker, end bought labour in Cragholt and
send stone and masons to Lowmere. The fourth term changes later lines through
`getpactterms PACT_CRAGHOLT`:

| Term | What it adds | Lines that change |
| --- | --- | --- |
| Restitution | Brannoc pays the family of every dead man, and Lowmere gets double stone | Widow Tallis |
| Reform | Cragholt refuses the Chancellery's quotas; the miners elect a safety warden | the first freed miner |
| Clemency | The deaths go in the public record as an accident | Widow Tallis, the old miner |

## Map-script headers

| Map | Header |
| --- | --- |
| Chain Road south and north (`Route104`) | On transition: `call_if_ge VAR_CRAGHOLT_STATE, 3, ChainRoad_EventScript_HideCoffle` |
| Penrose cottage (`Route104_MrBrineysHouse`) | On transition: `call_if_lt VAR_CRAGHOLT_STATE, 3, ChainRoad_PenroseCottage_EventScript_HideWil` |
| Cragholt (`RustboroCity`) | On transition: `call Cragholt_EventScript_SetObjects` |
| Magistrate's hall | On transition: `call Cragholt_MagistrateHall_EventScript_SetObjects` |
| Collapsed mine (`RusturfTunnel`) | On transition: `call CollapsedMine_EventScript_SetObjects`. On load: `call_if_set FLAG_RESCUED_WIL, CollapsedMine_EventScript_ClearRubble` |
| Old Lodge (`LittlerootTown_BrendansHouse_1F`) | Done in this PR: on transition `Lowmere_OldLodge_EventScript_SetObjects`, on frame `Lowmere_OldLodge_EventScript_StoneArrives` at `VAR_CRAGHOLT_STATE` 3 |
| Lowmere (`LittlerootTown`) | No change: `Lowmere_EventScript_CheckReturn` now also plays the first sight of Stage 2 |
| Barracks, ore office, Tallis house, mason's house, Pithead Hall, Gallows Wood | No scenes in the header |

## Placement per area

### The Chain Road

The road north from Thornfield to Cragholt. Suggested slot: `Route104` (south and north
halves), with Gallows Wood (`PetalburgWoods`) between them.

| Kind | Label | Who / what | Flag | Notes |
| --- | --- | --- | --- | --- |
| Objects | `ChainRoad_EventScript_Harrow` / `_Bonded1` / `_Bonded2` | three bonded men roped together, resting by the beach path | `FLAG_TEMP_1` | Harrow is the Thornfield tenant from plot eleven; gone from state 3 |
| Object | `ChainRoad_EventScript_CoffleGuard` | their guard | `FLAG_TEMP_1` | as above |
| Sign | `ChainRoad_EventScript_Rope` | the rope between them | | |
| Trainer | `ChainRoad_EventScript_Fisher` | fisher on the beach | | `TRAINER_CHAIN_ROAD_FISHER` |
| Trainer | `ChainRoad_EventScript_Collector` | Gilded Scale collector | | `TRAINER_CHAIN_ROAD_COLLECTOR` |
| Signs | `ChainRoad_EventScript_SignSouth` / `_SignNorth` | road signs at each end | | |
| Trainer | `GallowsWood_EventScript_BugCatcher` | bug catcher in the wood | | `TRAINER_GALLOWS_WOOD_BUG_CATCHER` |
| Trainer | `GallowsWood_EventScript_DebtWarden` | debt warden at the north exit, must fight | | `TRAINER_DEBT_WARDEN` |
| Signs | `GallowsWood_EventScript_Sign` / `_Checkpoint` | wood sign; checkpoint notice by the warden | | |

Penrose cottage:

| Kind | Label | Flag | Notes |
| --- | --- | --- | --- |
| Object | `ChainRoad_PenroseCottage_EventScript_Penrose` | | Old Penrose; his son Wil was sold for debt |
| Object | `ChainRoad_PenroseCottage_EventScript_Wil` | `FLAG_TEMP_1` | home from state 3 |
| Sign | `ChainRoad_PenroseCottage_EventScript_Nets` | | mended nets |

### Cragholt

Grey stone town under a pithead lift. The square has the quota board; the bonded crew
waits in a line by the lift.

| Kind | Label | Who / what | `local_id` | Flag | Notes |
| --- | --- | --- | --- | --- | --- |
| Trigger | `Cragholt_EventScript_Arrival` | (12..19,54) across the south road in | | | while `VAR_CRAGHOLT_STATE` is 0 |
| Object | (none) | Brannoc at (18,52), facing right toward the headframe | `LOCALID_CRAGHOLT_BRANNOC` (1) | `FLAG_TEMP_1` | |
| Object | (none) | the lift guard | `LOCALID_CRAGHOLT_GUARD` (2) | `FLAG_TEMP_1` | |
| Objects | (none) | three bonded men east of the headframe, (21,51), (21,52) and one more, facing left | `LOCALID_CRAGHOLT_BONDED_1` (3) / `_2` (4) / `_3` (5) | `FLAG_TEMP_1` | turn to the lift and are removed under a fade |
| Object | `Cragholt_EventScript_Tamsin` | Tamsin by the quota board | `LOCALID_CRAGHOLT_TAMSIN` (6) | `FLAG_TEMP_2` | for the scene she is moved one tile ahead of the player, wherever they cross the trigger |
| Objects | `Cragholt_EventScript_Crew1` / `_Crew2` / `_CrewGuard` | the next crew and its guard by the lift | | `FLAG_TEMP_3` | until state 3 |
| Objects | `Cragholt_EventScript_FreedMiner1` / `_FreedMiner2` | freed miners in the square | | `FLAG_TEMP_4` | from state 3 |
| Object | `Cragholt_EventScript_HallGuard` | guard on the Pithead Hall door | | `FLAG_TEMP_5` | blocks the door until state 2 |
| Objects | `Cragholt_EventScript_Woman` / `_OldMiner` / `_Child` | townsfolk | | | |
| Signs | `Cragholt_EventScript_Lift` / `_QuotaBoard` | the pit bell at (19,52) or (20,52); the quota board at (28,39) | | | |
| Signs | `Cragholt_EventScript_TownSign` / `_HallSign` / `_OfficeSign` / `_BarracksSign` / `_MineSign` | | | | `_MineSign` at the collapsed mine entrance |

Interiors:

| Map | Kind | Label | Notes |
| --- | --- | --- | --- |
| Barracks (`RustboroCity_PokemonSchool`) | Object | `Cragholt_Barracks_EventScript_Keeper` | the roll keeper |
| Barracks | Object | `Cragholt_Barracks_EventScript_Sleeper` | a bonded man in a bunk |
| Barracks | Sign | `Cragholt_Barracks_EventScript_Roll` | the roll on the wall; gives `ITEM_BONDING_ROLL` |
| Barracks | Sign | `Cragholt_Barracks_EventScript_Bunks` | the bunks |
| Ore office (`RustboroCity_DevonCorp_1F`) | Trainer | `Cragholt_OreOffice_EventScript_Clerk` | `TRAINER_ORE_OFFICE_CLERK`, in the way of the desk |
| Ore office | Sign | `Cragholt_OreOffice_EventScript_Ledger` | the desk; gives `ITEM_QUOTA_LEDGER`. Starts the clerk's battle first if he hasn't been beaten |
| Ore office | Object, sign | `Cragholt_OreOffice_EventScript_Assayer`, `_OreSamples` | |
| Magistrate's hall (`RustboroCity_House1`) | Object | `Cragholt_MagistrateHall_EventScript_Hollen` | Magistrate Hollen |
| Magistrate's hall | Object | `Cragholt_MagistrateHall_EventScript_Wil` | flag `FLAG_TEMP_1`; there from the rescue until state 3 |
| Magistrate's hall | Sign | `Cragholt_MagistrateHall_EventScript_Reports` | the pit reports shelf |
| Widow Tallis's house (`RustboroCity_House2`) | Object, sign | `Cragholt_TallisHouse_EventScript_Tallis`, `_Boots` | |
| Mason's house (`RustboroCity_House3`) | Objects | `Cragholt_MasonHouse_EventScript_Mason`, `_Girl` | |

### The collapsed mine

The east tunnel (`RusturfTunnel`). A rubble fall of metatiles at (20,4)-(21,5) closes the
haulage way, with three men trapped behind it; the bell crate is a solid prop at (19,4). The
rescue turns the rubble into floor (`CollapsedMine_EventScript_ClearRubble`, metatile 0x201),
and the on-load script does the same once Wil is out.

| Kind | Label | Who / what | `local_id` | Flag | Notes |
| --- | --- | --- | --- | --- | --- |
| Sign | `CollapsedMine_EventScript_Rubble` | the rubble at (20,5), read from (19,5) facing right; also (20,4) | | | asks to clear it; the rescue sets `FLAG_RESCUED_WIL` |
| Object | `CollapsedMine_EventScript_Wil` | Wil at (23,4) | `LOCALID_COLLAPSED_MINE_WIL` (1) | `FLAG_TEMP_1` | |
| Objects | `CollapsedMine_EventScript_TrappedMiner` | miners at (24,5) and (26,4) | `LOCALID_COLLAPSED_MINE_MINER_1` (2) / `_2` (3) | `FLAG_TEMP_1` | |
| Sign | `CollapsedMine_EventScript_BellCrate` | the crate at (19,4) | | | sets `FLAG_FOUND_BELL_CRATE` |
| Signs | `CollapsedMine_EventScript_Props` / `_Sign` | snapped props; level sign | | | |

### The Pithead Hall

Brannoc's palace (`RustboroCity_Gym`): one bare stone hall with ore seams in the walls.

| Kind | Label | Notes |
| --- | --- | --- |
| Object | `PitheadHall_EventScript_Herald` | by the door, next to the cook |
| Trainers | `PitheadHall_EventScript_Foreman` / `_Miner1` / `_Miner2` | `TRAINER_PITHEAD_FOREMAN`, `TRAINER_PITHEAD_MINER_1`, `TRAINER_PITHEAD_MINER_2` |
| Object | `PitheadHall_EventScript_Cook` | the canteen cook at the guide's spot (3,18); full heal before the Trial |
| Object | `PitheadHall_EventScript_Brannoc` | Brannoc at (5,2), `TRAINER_BRANNOC` |
| Signs | `PitheadHall_EventScript_Seam` | ore seams in the walls |

Trainer slide for Brannoc's last Pokémon (Regirock): "I dug this one out myself."

### The Old Lodge

| Kind | Label | `local_id` | Flag | Notes |
| --- | --- | --- | --- | --- |
| Object | `Lowmere_OldLodge_EventScript_Tamsin` | `LOCALID_OLD_LODGE_TAMSIN` (11) | `FLAG_TEMP_1` | beside Toft; only while `VAR_CRAGHOLT_STATE` is 3 |

## Trainers

Balance defines these in `include/constants/slice_balance.h`; `common.inc` falls back to the
same vanilla trainers.

| Name | Fallback | Who |
| --- | --- | --- |
| `TRAINER_CHAIN_ROAD_FISHER` | `TRAINER_DARIAN` | fisher |
| `TRAINER_CHAIN_ROAD_COLLECTOR` | `TRAINER_IVAN` | Gilded Scale collector |
| `TRAINER_GALLOWS_WOOD_BUG_CATCHER` | `TRAINER_LYLE` | bug catcher |
| `TRAINER_DEBT_WARDEN` | `TRAINER_GRUNT_WEATHER_INST_1` | debt warden |
| `TRAINER_ORE_OFFICE_CLERK` | `TRAINER_GRUNT_WEATHER_INST_2` | ore clerk |
| `TRAINER_PITHEAD_FOREMAN` | `TRAINER_MIKE_2` | pithead foreman |
| `TRAINER_PITHEAD_MINER_1` / `_2` | `TRAINER_BRICE` / `TRAINER_CLARK` | free miners |
| `TRAINER_BRANNOC` | `TRAINER_BRAWLY_1` | Brannoc, Rock, Regirock as the ace |

## Lowmere at Stage 2

The Cragholt pact is the third, so `signpact` raises Lowmere to Stage 2. On the first sight
of the Stage 2 town, `Lowmere_EventScript_CheckReturn` plays the masons at work and Toft
waving. Entering the Old Lodge plays `Lowmere_OldLodge_EventScript_StoneArrives`: Toft
announces the stone, the sea wall and the new Pokémon Center, names Tamsin the town's envoy,
and a letter sealed with a pale bell is found on the step: "The tenth child should stay
small." From `VAR_CRAGHOLT_STATE` 4, Toft has a Stage 2 line
(`Lowmere_EventScript_ToftCragholt`).
