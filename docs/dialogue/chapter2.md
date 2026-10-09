# Chapter 2: Thornfield, dialogue and script contract

Chapter 2 runs from the end of the slice (`VAR_SLICE_STATE` 13, Lowmere at Stage 1) to the
Thornfield pact. The player takes the Orchard Road west from Haymarket, watches a tenant
family evicted, builds the case against Princess Isolde, wins her Trial in the Glasshouse
and dictates the Pact of the Open Field. It follows the same contract as
[the slice](events.md): labels are named by area, each map's `scripts.inc` holds only its
header, and `map.json` points objects, triggers and signs at the labels below. Story source:
[characters.md](../story/characters.md) (Isolde), [acts.md](../story/acts.md) (Act 1) and
the crime-scene table in the [story bible README](../story/README.md).

## Where things live

| What | File |
| --- | --- |
| Trainer names, local-id fallbacks, speaker names | `data/scripts/chapter2/common.inc` |
| Orchard Road | `data/scripts/chapter2/orchard_road.inc` (`OrchardRoad_…`) |
| Thornfield and its interiors | `data/scripts/chapter2/thornfield.inc` (`Thornfield_…`) |
| Isolde's palace | `data/scripts/chapter2/glasshouse.inc` (`Glasshouse_…`) |

Until the map thread's `map.json` files define the `LOCALID_…` names below, `common.inc`
and `thornfield.inc` fall back to the ids in the tables. The same goes for the trainer
names, which fall back to vanilla trainers until balance defines them.

## Story progress

The Orchard Road opens at the end of the slice (`VAR_SLICE_STATE` 13). From there,
`VAR_THORNFIELD_STATE` tracks the chapter:

| Value | Meaning | Set by |
| --- | --- | --- |
| 0 | Not arrived yet | |
| 1 | Eviction seen; case open | `Thornfield_EventScript_Arrival` |
| 2 | Trial granted by Magistrate Fenwick | `Thornfield_MagistrateHall_EventScript_Fenwick` |
| 3 | Isolde beaten, pact signed | `Glasshouse_EventScript_Isolde` |

The case needs three things, as in Haymarket:

| Evidence | Flag | Key item | Where |
| --- | --- | --- | --- |
| The steward's ledger, with the pale bell inside its cover | `FLAG_FOUND_DEBT_LEDGER` | `ITEM_DEBT_LEDGER` | Steward's Office desk |
| The foreclosure notice signed by Isolde | `FLAG_FOUND_FORECLOSURE_WRIT` | `ITEM_FORECLOSURE_WRIT` | the Willows' boarded door |
| Old Mabry, Isolde's head gardener, agrees to testify | `FLAG_THORNFIELD_WITNESS` | | his cottage, once the player has the ledger |

Other flags: `FLAG_THORNFIELD_TRIAL_GRANTED` (Fenwick), `FLAG_THORNFIELD_GROVE_SEEN` (the
thorn gate is read), `FLAG_MET_ISOLDE` (her terrace talk), `FLAG_LOWMERE_GARDENERS_ARRIVED`
(set by Toft's Thornfield line in Lowmere), `FLAG_BADGE02_GET` (the Thornfield Seal) and
`FLAG_PACT_THORNFIELD`, set by `signpact PACT_THORNFIELD`. The features thread owns the flag
and item definitions (`docs/systems.md`). Scene objects use temp flags, set from each map's
on-transition helper, so they need no permanent flags.

The pact's fourth term changes later lines through `getpactterms PACT_THORNFIELD`:

| Term | What it adds | Lines that change |
| --- | --- | --- |
| Restitution | Isolde pays Lowmere orchard stock and a year's seed | |
| Reform | Every tenant's debt is cancelled; the ledgers are burned in the square | Mae Willow |
| Clemency | The land is returned as Isolde's own gift, in her name | The old man in town |

## Map-script headers

The map thread picks the slots. The headers these scripts need:

| Map | Header |
| --- | --- |
| Orchard Road (`Route102`) | On transition: `call_if_ge VAR_SLICE_STATE, 13, OrchardRoad_EventScript_HideWarden` |
| Thornfield (`PetalburgCity`) | On transition: `call Thornfield_EventScript_SetObjects`. On load, in states 1 and 2, board up the Willows' door (see `Thornfield_EventScript_BoardUpDoor`) |
| Steward's Office | On transition: `call_if_ge VAR_THORNFIELD_STATE, 3, Thornfield_StewardsOffice_EventScript_HideVoss` |
| Old Mabry's cottage, Magistrate's Hall, the Glasshouse | No scenes in the header |

`Thornfield_EventScript_BoardUpDoor` boards up the Willows' door (metatiles from the map
thread) and redraws the map. The eviction scene calls it under its fade, and the town's
on-load script calls it while `VAR_THORNFIELD_STATE` is 1 or 2.

## Placement per area

### Orchard Road

The road from Haymarket's west exit to Thornfield. Suggested slot: `Route102`. Orchards
line both sides.

| Kind | Label | Who / what | `local_id` | Flag | Notes |
| --- | --- | --- | --- | --- | --- |
| Object | `OrchardRoad_EventScript_Warden` | road warden across the road near the east end | `LOCALID_ORCHARD_ROAD_WARDEN` (1) | `FLAG_TEMP_1` | blocks the road until `VAR_SLICE_STATE` 13 |
| Object | `OrchardRoad_EventScript_Tenant` | an evicted tenant heading east | | | |
| Sign | `OrchardRoad_EventScript_TenantCart` | his handcart, beside him | | | |
| Trainer | `OrchardRoad_EventScript_Picker` | apple picker in the orchard | | | `TRAINER_ORCHARD_ROAD_PICKER` |
| Trainer | `OrchardRoad_EventScript_Bailiff` | Gilded Scale debt bailiff | | | `TRAINER_ORCHARD_ROAD_BAILIFF` |
| Trainer | `OrchardRoad_EventScript_Poet` | court poet on the way to Isolde's salon | | | `TRAINER_ORCHARD_ROAD_POET` |
| Signs | `OrchardRoad_EventScript_SignEast` / `_SignWest` / `_Orchard` | road signs at each end; an orchard tree | | | |

### Thornfield

Orchards and greenhouses, with flower terraces climbing the north side to the Glasshouse.
The terraces are built over old fields, so field walls show through the flowerbeds. The
Willows' house is a tenant cottage with a bean field beside it, in view of the road in
from the Orchard Road. A thorn gate behind the palace seals the old grove.

| Kind | Label | Who / what | `local_id` | Flag | Notes |
| --- | --- | --- | --- | --- | --- |
| Trigger | `Thornfield_EventScript_Arrival` | on the road in from the east, with the Willows' door within ~4 tiles | | | while `VAR_THORNFIELD_STATE` is 0 |
| Object | (none) | Steward Voss, facing the Willows' door | `LOCALID_THORNFIELD_VOSS` (1) | `FLAG_TEMP_1` | |
| Object | (none) | guard 1 and guard 2, either side of the family, facing up | `LOCALID_THORNFIELD_GUARD_1` (2) / `_GUARD_2` (3) | `FLAG_TEMP_1` | |
| Object | `Thornfield_EventScript_Willow` | Tom Willow at his door | `LOCALID_THORNFIELD_WILLOW` (4) | `FLAG_TEMP_3` | in the eviction scene; home again from state 3 |
| Object | `Thornfield_EventScript_Mae` | Mae Willow beside him | `LOCALID_THORNFIELD_MAE` (5) | `FLAG_TEMP_3` | as above |
| Object | `Thornfield_EventScript_Pip` | Pip, their son | `LOCALID_THORNFIELD_PIP` (6) | `FLAG_TEMP_3` | as above; tells the Shaymin story after the pact |
| Object | (none) | two gardeners in the bean field, facing down | `LOCALID_THORNFIELD_DIGGER_1` (7) / `_DIGGER_2` (8) | `FLAG_TEMP_1` | one is Old Mabry |
| Object | `Thornfield_EventScript_Tamsin` | Tamsin near the square | `LOCALID_THORNFIELD_TAMSIN` (9) | `FLAG_TEMP_5` | appears at the end of the arrival scene |
| Objects | `Thornfield_EventScript_CampWillow` / `_CampMae` / `_CampPip` | the Willows camped at the east edge of town | `LOCALID_THORNFIELD_CAMP_WILLOW` (10) / `_CAMP_MAE` (11) / `_CAMP_PIP` (12) | `FLAG_TEMP_2` | states 1-2 |
| Object | `Thornfield_EventScript_IsoldeTerrace` | Isolde on the terraces | | `FLAG_TEMP_4` | until state 2 |
| Object | `Thornfield_EventScript_PalaceGuard` | guard in front of the Glasshouse door | | `FLAG_TEMP_6` | blocks the door until state 2 |
| Objects | `Thornfield_EventScript_Woman` / `_OldMan` / `_Gardener` | townsfolk | | | |
| Sign | `Thornfield_EventScript_BoardedHouse` | the Willows' door | | | gives the writ |
| Sign | `Thornfield_EventScript_Terraces` | the terraces | | | |
| Sign | `Thornfield_EventScript_GroveGate` | the thorn gate behind the palace | | | the sealed grove (Xerneas, Act 2); sets `FLAG_THORNFIELD_GROVE_SEEN` |
| Signs | `Thornfield_EventScript_TownSign` / `_PalaceSign` | | | | |

Interiors:

| Map | Kind | Label | Notes |
| --- | --- | --- | --- |
| Steward's Office | Object | `Thornfield_StewardsOffice_EventScript_Voss` | Voss behind the desk, `local_id` `LOCALID_STEWARDS_OFFICE_VOSS` (1), flag `FLAG_TEMP_1` |
| Steward's Office | Trainer | `Thornfield_StewardsOffice_EventScript_Bailiff` | `TRAINER_THORNFIELD_BAILIFF`, in the way of the ledger desk |
| Steward's Office | Sign | `Thornfield_StewardsOffice_EventScript_Ledger` | the ledger; gives `ITEM_DEBT_LEDGER`. The pale bell is stamped inside its cover |
| Old Mabry's cottage | Object | `Thornfield_MabryCottage_EventScript_Mabry` | the witness once the player has the ledger page; sets `FLAG_THORNFIELD_WITNESS` |
| Old Mabry's cottage | Sign | `Thornfield_MabryCottage_EventScript_SeedShelf` | seed packets labelled with tenants' names |
| Magistrate's Hall | Object | `Thornfield_MagistrateHall_EventScript_Fenwick` | Magistrate Fenwick |
| Magistrate's Hall | Object | `Thornfield_MagistrateHall_EventScript_Clerk` | court clerk |

### The Glasshouse

Isolde's palace: one glass throne hall full of flowers, like the Gilt Pavilion, rather than
the 7-room Petalburg Gym.

| Kind | Label | Notes |
| --- | --- | --- |
| Object | `Glasshouse_EventScript_Herald` | by the door (the Gym guide's job) |
| Trainers | `Glasshouse_EventScript_Gardener1` / `_Gardener2` / `_Courtier` | `TRAINER_PALACE_GARDENER_1`, `TRAINER_PALACE_GARDENER_2`, `TRAINER_PALACE_COURTIER` |
| Object | `Glasshouse_EventScript_Servant` | a servant with rose water between the courtiers and Isolde; full heal before the Trial |
| Object | `Glasshouse_EventScript_Isolde` | Isolde on the throne, `TRAINER_ISOLDE` |
| Signs | `Glasshouse_EventScript_GlassWall` | the glass walls |

Trainer slide for Isolde's last Pokémon (Shaymin), for whoever adds trainer slides:
"My last bloom. Isn't it lovely?"

## Trainers

Balance owns the teams. Until balance defines these names, `common.inc` maps them to
vanilla trainers no other map uses:

| Name | Fallback | Who |
| --- | --- | --- |
| `TRAINER_ORCHARD_ROAD_PICKER` | `TRAINER_DAISY` | apple picker |
| `TRAINER_ORCHARD_ROAD_BAILIFF` | `TRAINER_RHETT` | Gilded Scale bailiff |
| `TRAINER_ORCHARD_ROAD_POET` | `TRAINER_MARCOS` | court poet |
| `TRAINER_THORNFIELD_BAILIFF` | `TRAINER_BERKE` | bailiff in the Steward's Office |
| `TRAINER_PALACE_GARDENER_1` / `_2` | `TRAINER_RANDALL` / `TRAINER_PARKER` | palace gardeners |
| `TRAINER_PALACE_COURTIER` | `TRAINER_GEORGE` | courtier poet |
| `TRAINER_ISOLDE` | `TRAINER_NORMAN_1` | Isolde, Grass, Shaymin as the ace |

## Lowmere after the pact

Reeve Toft has a new line from `VAR_THORNFIELD_STATE` 3 (`Lowmere_EventScript_ToftThornfield`): carts of
seed and two gardeners arrive from Thornfield. Lowmere stays at Stage 1 until the Cragholt
pact.
