# Balance: vertical slice

Owned by the balance thread. Covers species settings, wild encounters, the level curve,
and every trainer team in the slice (Lowmere, the Mire Road, Haymarket, the Gilt Pavilion).
Story context is in the [story bible](story/vertical-slice.md).

## Species

Every species and form the expansion supports is already enabled in
`include/config/species_enabled.h`: Gens 1-9, Megas (including the Z-A ones), Primal,
Ultra Burst, Gigantamax, Tera forms, fusion forms, every regional form, cross-generation
evolutions and the Pikachu variants. Nothing is switched off. Keep it that way: if ROM space
ever runs out, disable unused families one by one in that file rather than whole
generations.

## Config changes

| Setting | File | Value | Why |
|---|---|---|---|
| `OW_TIME_OF_DAY_ENCOUNTERS` | `include/config/overworld.h` | `TRUE` | Routes get a separate night table, so each area holds more species. Unsuffixed tables cover morning, day and evening; `_Night` tables cover night. |
| `B_EXP_CAP_TYPE` | `include/config/caps.h` | `EXP_CAP_SOFT` | Pokémon at or above the level cap still gain experience, but much less. Keeps the legendary dog and over-grinders from flattening the Trials. |
| `B_LEVEL_CAP_TYPE` | `include/config/caps.h` | `LEVEL_CAP_FLAG_LIST` | The cap rises with each seal (badge flag). Before the first Trial it is **15**. |
| `B_RARE_CANDY_CAP` | `include/config/caps.h` | `TRUE` | Rare Candies can't push past the cap. |
| `B_LEVEL_CAP_EXP_UP` | `include/config/caps.h` | `TRUE` | Pokémon below the cap earn bonus experience: +12.5% one level under, rising to double at four or more under. Catches the slow-levelling dog and a freshly caught Pokémon up before the Trial. |

The cap per seal lives in `sLevelCapFlagMap` in `src/caps.c` (15, 19, 24, 29, 31, 33, 42,
46, 58). Only the first value matters for the slice; balance will retune the rest as each
Trial is built.

## Level curve

| Point in the slice | Player (expected) | Opponents |
|---|---|---|
| Starter from Bram | 5 | |
| Tamsin, first rival battle | 5-6 | 5 |
| King's gift dog | 5 | |
| Lowmere wild grass | | 2-4 |
| Mire Road wild grass | | 3-5 |
| Mire Road trainers and wagon clerk | 7-9 | 4-7 |
| Haymarket granary and counting house | 9-11 | 9-10 |
| Gilt Pavilion courtiers | 10-12 | 9-11 |
| **Corwin, Trial 1** | 11-13 (cap 15) | 10, 10, 11, ace 12 |

## The king's gift

Give the dog at **level 5**, the same as the starter. What keeps it from trivialising
Corwin:

- The dogs are on the Slow experience curve, so they level noticeably slower than the
  starter.
- The soft level cap (15) stops it running away before the first Trial.
- Corwin's Bunnelby carries Mud-Slap, which hits Entei and Raikou super-effectively.

The dog's slow curve turned out to hold it back too far in the first playtest (level 5 to
7 over two fights), so the below-cap experience bonus now helps it catch up.

At level 5 the dogs' default moves would include Extreme Speed (Raikou). Whoever writes the
gift script should give explicit moves with `givemon`:

| Dog | Level | Moves |
|---|---|---|
| Entei | 5 | Ember, Leer, Smokescreen |
| Raikou | 5 | Thunder Shock, Leer, Quick Attack |
| Suicune | 5 | Water Gun, Leer, Mist |

They learn Flame Wheel, Spark or Water Pulse at 6 and Bite at 12. Legendary perfect IVs
(3 × 31) stay on, as in the main series.

## Wild encounters

`src/data/wild_encounters.json`. Lowmere uses the Littleroot Town slot and only matters if
the map thread adds grass or water there. The Mire Road uses the Route 101 slot.

Land slot odds are 20, 20, 10, 10, 10, 10, 5, 5, 4, 4, 1, 1 (%).

**Lowmere, day** (2-4): Wooper, Lotad, Tympole, Surskit, Wingull, Bidoof, Ducklett,
Chewtle, Shellos, Paldean Wooper, Stunfisk (1%), Goomy (1%).

**Lowmere, night** (2-4): Wooper, Tympole, Spinarak, Gastly, Hoothoot, Oddish, Munna,
Croagunk, Stunky, Chewtle, Stunfisk (1%), Goomy (1%). Munna and Gastly are a quiet nod to
the village's bad dreams.

**Lowmere, surfing** (20-30, for later): Psyduck, Lotad, Buizel, Quagsire, Wishiwashi.

**Lowmere, fishing**: Old Rod Magikarp, Tympole; Good Rod Barboach, Goldeen, Chewtle;
Super Rod Whiscash, Stunfisk, Arrokuda, Corphish, Feebas (1%).

**Mire Road, day** (3-5): Lillipup, Tarountula, Wooper, Grubbin, Bidoof, Mudbray, Skwovet,
Yungoos, Croagunk, Rookidee, Pawmi (1%), Riolu (1%).

**Mire Road, night** (3-5): Hoothoot, Spinarak, Poochyena, Murkrow, Venipede, Croagunk,
Gastly, Noibat, Shuppet, Kricketot, Shroodle (1%), Impidimp (1%).

**Mire Road, fishing** (shallow pools): Old Rod Magikarp, Tympole; Good Rod Goldeen,
Chewtle, Wooper; Super Rod Basculin, Barboach, Stunfisk, Goomy, Dratini (1%).

That is 49 different species across the slice's two areas.

## Trainers

The expansion only has room for 9 new trainer IDs before trainer flags overflow, and the
slice needs 14. So the slice reuses vanilla trainer IDs from the matching story beat
(none of them have rematches). `include/constants/slice_balance.h` gives each one a story name (`TRAINER_CORWIN`, `TRAINER_TAMSIN_LOWMERE_0`, ...) for scripts to use, and defines `KINGS_GIFT_LEVEL`.
Classes and pics come from the features thread (docs/systems.md, "Trainer classes and pics").

| Constant | Who | Where | Class / pic | Team |
|---|---|---|---|---|
| `TRAINER_MAY_ROUTE_103_*`, `TRAINER_BRENDAN_ROUTE_103_*` | **Tamsin** | Edge of Lowmere | Friend / Tamsin | Starter that beats the player's, Lv 5, level-1 moves only |
| `TRAINER_RICK` | Clerk Hobb, Gilded Scale toll | Mire Road | Guild Clerk | Purrloin 5, Galarian Zigzagoon 5 |
| `TRAINER_TIANA` | Grisk, poacher | Mire Road | Poacher | Wooper 5, Croagunk 6 |
| `TRAINER_ALLEN` | Pip, youngster | Mire Road | Youngster | Bidoof 4, Lillipup 5 |
| `TRAINER_ANDREW` | Willem, grain carter | Mire Road | Carter | Mudbray 6, Skwovet 6 |
| `TRAINER_GRUNT_PETALBURG_WOODS` | Wagon clerk | Mire Road event | Guild Clerk | Poochyena 6, Galarian Meowth 7 |
| `TRAINER_GRUNT_RUSTURF_TUNNEL` | Guild foreman | Haymarket granary | Foreman | Mudbray 9, Timburr 10 |
| `TRAINER_GRUNT_MUSEUM_1` | Clerk | Counting house | Guild Clerk | Purrloin 9, Poochyena 9 |
| `TRAINER_GRUNT_MUSEUM_2` | Clerk | Counting house | Guild Clerk | Murkrow 9, Pawniard 10 |
| `TRAINER_JOSH` | Albrecht, bidder | Gilt Pavilion | Bidder / Rich Boy | Lillipup 9, Glameow 10 |
| `TRAINER_TOMMY` | Celeste, broker | Gilt Pavilion | Broker / Lady | Skitty 9, Minccino 10 |
| `TRAINER_MARC` | Fenwick, broker | Gilt Pavilion | Broker / Gentleman | Lechonk 10, Aipom 11 |
| `TRAINER_ROXANNE_1` | **Prince Corwin**, Trial 1 | Gilt Pavilion | Prince / Prince Corwin | See below |

Tamsin's constant suffix is the **player's** starter, as in vanilla: `_MUDKIP` means the
player chose Mudkip, so Tamsin has Treecko. Both the May and Brendan sets are identical, so
the vanilla gender check in the script can stay or go. The starters are still Treecko,
Torchic and Mudkip; if the story picks different partners, Tamsin's teams change with them.

### Corwin, Trial 1

Four "lots", one per round of the auction, with Type: Null unveiled last. Corwin carries no items, and the AI's `Ace Pokemon` flag keeps Type: Null back until it is the last Pokémon standing (without it, expansion's switching AI ignores party order).

| Lot | Pokémon | Lv | Ability / item | Moves | Role |
|---|---|---|---|---|---|
| 1 | Skwovet | 10 | Cheek Pouch, Oran Berry | Tackle, Bite, Tail Whip, Stuff Cheeks | Eats its berry to heal and raise Defence: Corwin's hoarding |
| 2 | Meowth | 10 | Technician | Fake Out, Feint, Scratch, Pay Day | Pays out coins all match |
| 3 | Bunnelby | 11 | Pickup | Mud-Slap, Quick Attack, Tackle, Leer | Ground coverage, the check on Entei and Raikou |
| 4 | **Type: Null** | 12 | Battle Armor | Tackle, Aerial Ace, Scary Face | The final lot. Bulky (95 HP / 95 Def / 95 SpD), no crits |

Type: Null sits two levels above the rest, like Roxanne's Nosepass, and holds no item (an Eviolite
would make it unbeatable at this level). Normal type has one weakness, Fighting, which
none of the starters have yet; Mudkip's Rock Smash and wild Riolu or Croagunk from the
Mire Road are the intended answers for players who want an edge.

### Playtest 1 changes (2026-10-09)

The first playtest arrived at Corwin with Mudkip and Suicune at level 10 and lost 5 times
out of 5. Corwin dropped two to three levels per Pokémon, lost his Potions (he had used one
on Type: Null) and now saves Type: Null for last. The courtiers dropped a level each,
the below-cap experience bonus is on, and Tamsin's starter only knows its level-1 moves
(her Treecko's Leafage beat a Tackle-only Mudkip 3 times in 4).

## Chapter 2: the Orchard Road, Thornfield and Isolde

The Orchard Road uses the Route 102 slot and Thornfield the Petalburg City slot. Isolde's
Trial is `TRAINER_NORMAN_1`. The level cap after Corwin's seal is **19** (`src/caps.c`).

### Level curve

| Point | Player (expected) | Opponents |
|---|---|---|
| Leaving Haymarket | 12-13 | |
| Orchard Road wild grass | | 9-12 |
| Orchard Road trainers | 13-14 | 11-13 |
| Thornfield bailiff | 14-15 | 13 |
| Isolde's gardeners and courtier | 15-16 | 13-15 |
| **Isolde, Trial 2** | 16-18 (cap 19) | 15, 15, 16, ace 18 |

### Wild encounters

**Orchard Road, day** (9-12): Starly, Seedot, Shroomish, Nincada, Sewaddle, Smoliv, Cutiefly,
Skiddo, Ralts, Pikipek, Applin (1%), Eevee (1%).

**Orchard Road, night** (9-12): Oddish, Zubat, Venonat, Rattata, Pumpkaboo, Morelull,
Hatenna, Spinarak, Litwick, Inkay, Zorua (1%), Rockruff (1%).

**Orchard Road, surfing** (20-35, for later): Marill, Azurill, Surskit, Dewpider, Masquerain.

**Orchard Road, fishing**: Old Rod Magikarp, Poliwag; Good Rod Poliwag, Goldeen, Corphish;
Super Rod Poliwhirl, Seaking, Corphish, Clauncher, Tynamo (1%).

**Thornfield terraces** (10-12, only if the map adds grass): Petilil, Bounsweet, Combee,
Flabébé, Hoppip, Sunkern, Cherubi, Budew, Deerling, Bulbasaur, Comfey (1%), Smoliv (1%).

**Thornfield, surfing and fishing**: garden-pond species (Lotad, Marill, Wooper, Poliwag,
Goldeen), with Bruxish on the Super Rod.

That adds 44 new species, so the first two chapters hold 93.

### Trainers

| Constant | Story name | Who | Class / pic | Team |
|---|---|---|---|---|
| `TRAINER_DAISY` | `TRAINER_ORCHARD_ROAD_PICKER` | Hazel, orchard hand | Picnicker | Combee 11, Bounsweet 12 |
| `TRAINER_RHETT` | `TRAINER_ORCHARD_ROAD_BAILIFF` | Bruno, debt bailiff | Bailiff | Machop 12, Meditite 12 |
| `TRAINER_MARCOS` | `TRAINER_ORCHARD_ROAD_POET` | Lionel, court poet | Gentleman | Flabébé 11, Kricketune 13 |
| `TRAINER_BERKE` | `TRAINER_THORNFIELD_BAILIFF` | Bailiff at the Steward's Office | Bailiff | Houndour 13, Grimer 13 |
| `TRAINER_RANDALL` | `TRAINER_PALACE_GARDENER_1` | Primrose, gardener | Gardener | Hoppip 13, Petilil 14 |
| `TRAINER_PARKER` | `TRAINER_PALACE_GARDENER_2` | Bryony, gardener | Gardener | Foongus 14, Sewaddle 14 |
| `TRAINER_GEORGE` | `TRAINER_PALACE_COURTIER` | Oswin, courtier | Gentleman | Sunkern 14, Skiploom 15 |
| `TRAINER_NORMAN_1` | `TRAINER_ISOLDE` | **Princess Isolde**, Trial 2 | Princess / Princess Isolde | See below |

None of these IDs have rematches or Match Call entries. Norman's gym rematches
(`NORMAN_2`-`5`) only trigger after the Champion, so they stay dormant.

### Isolde, Trial 2

Ace Pokemon AI flag, no items. Grass is weak to Fire, Ice, Flying, Bug and Poison; Entei,
Torchic and the Orchard Road's Starly, Pikipek and Sewaddle all answer it.

| Pokémon | Lv | Ability | Moves | Role |
|---|---|---|---|---|
| Gloom | 15 | Chlorophyll | Mega Drain, Acid, Poison Powder, Sweet Scent | Poison chip, resists Grass and Fighting |
| Skiddo | 15 | Sap Sipper | Razor Leaf, Leech Seed, Tail Whip, Tackle | Absorbs the player's Grass moves |
| Roselia | 16 | Natural Cure | Mega Drain, Stun Spore, Poison Sting, Leech Seed | Status and drain |
| **Shaymin** | 18 | Natural Cure | Magical Leaf, Leech Seed, Growth, Disarming Voice | The ace that "only flowers where land has been taken". Disarming Voice hits Fighting and Dragon types |

Shaymin is one level under the cap and holds no item, matching Corwin's ace at two above
his team.

## Chapter 3: Chain Road, Gallows Wood, Cragholt and Brannoc

Chain Road uses the Route 104 slot, Gallows Wood the Petalburg Woods slot and Cragholt the
Rustboro City slot. Brannoc's Trial is `TRAINER_BRAWLY_1`. The level cap after Isolde's seal
is **24** (`src/caps.c`).

### Level curve

| Point | Player (expected) | Opponents |
|---|---|---|
| Leaving Thornfield | 17-18 | |
| Chain Road and Gallows Wood grass | | 13-16 |
| Chain Road and wood trainers | 18-19 | 15-18 |
| Debt warden checkpoint (must fight) | 19 | 17, 18 |
| Ore office and Pithead Hall | 19-21 | 18-19 |
| **Brannoc, Trial 3** | 21-23 (cap 24) | 20, 20, 21, ace 23 |

### Wild encounters

**Chain Road, day** (13-16): Geodude, Roggenrola, Rolycoly, Nacli, Diglett, Pidove, Sandshrew,
Phanpy, Drilbur, Hippopotas, Larvitar (1%), Rhyhorn (1%).

**Chain Road, night** (13-16): Woobat, Gligar, Houndour, Purrloin, Sandile, Trapinch, Dwebble,
Nosepass, Onix, Aron, Sableye (1%), Deino (1%).

**Chain Road, surfing** (20-35, for later): Tentacool, Wingull, Finizen, Wailmer, Pelipper.

**Chain Road, fishing**: Old Rod Magikarp, Tentacool; Good Rod Remoraid, Shellder, Wailmer;
Super Rod Qwilfish, Remoraid, Chinchou, Wailmer, Skrelp (1%).

**Gallows Wood, day** (13-16): Wurmple, Caterpie, Weedle, Scatterbug, Blipbug, Nymble, Paras,
Pineco, Foongus, Joltik, Heracross (1%), Scyther (1%).

**Gallows Wood, night** (13-16): Duskull, Misdreavus, Sinistea, Phantump, Gothita, Natu, Nickit,
Greavard, Honedge, Yamask, Mimikyu (1%), Spiritomb (1%).

That adds 57 new species, so the first three chapters hold 150. Rock-heavy grass gives
Water, Grass and Fighting teams plenty of easy wins before Brannoc, and the night wood is
the first real Ghost source.

### Trainers

| Constant | Story name | Who | Class / pic | Team |
|---|---|---|---|---|
| `TRAINER_DARIAN` | `TRAINER_CHAIN_ROAD_FISHER` | Odo, fisher | Fisherman | Remoraid 16, Chinchou 16, Wailmer 17 |
| `TRAINER_IVAN` | `TRAINER_CHAIN_ROAD_COLLECTOR` | Aldric, Gilded Scale collector | Guild Clerk | Klefki 17, Mawile 18 |
| `TRAINER_LYLE` | `TRAINER_GALLOWS_WOOD_BUG_CATCHER` | Colm, bug catcher | Bug Catcher | Nincada 15, Joltik 16, Dwebble 16 |
| `TRAINER_GRUNT_WEATHER_INST_1` | `TRAINER_DEBT_WARDEN` | Debt warden, wood checkpoint | Bailiff | Pawniard 17, Mightyena 18 |
| `TRAINER_GRUNT_WEATHER_INST_2` | `TRAINER_ORE_OFFICE_CLERK` | Chancellery ore clerk | Guild Clerk | Nosepass 18, Bronzor 18 |
| `TRAINER_MIKE_2` | `TRAINER_PITHEAD_FOREMAN` | Durran, pithead foreman | Foreman | Rolycoly 18, Timburr 19 |
| `TRAINER_BRICE` | `TRAINER_PITHEAD_MINER_1` | Tobin, miner | Hiker | Roggenrola 18, Onix 19 |
| `TRAINER_CLARK` | `TRAINER_PITHEAD_MINER_2` | Maud, miner | Hiker / Picnicker | Drilbur 18, Nacli 19 |
| `TRAINER_BRAWLY_1` | `TRAINER_BRANNOC` | **Prince Brannoc**, Trial 3 | Prince / Leader Brawly (placeholder pic) | See below |

None of these IDs have rematches or Match Call entries. Brawly's gym rematches only trigger
after the Champion. The two Weather Institute grunts share their defeat flags with the
vanilla Route 119 battles, which the hack does not use.

### Brannoc, Trial 3

Ace Pokemon AI flag, no items. Rock is weak to Water, Grass, Fighting, Ground and Steel;
Mudkip, Treecko, Suicune and Isolde's region's Grass catches all answer it. Two Sturdy
leads punish players who rely on one big hit.

| Pokémon | Lv | Ability | Moves | Role |
|---|---|---|---|---|
| Onix | 20 | Sturdy | Rock Tomb, Smack Down, Bind, Screech | Lead, slows the player |
| Nosepass | 20 | Sturdy | Rock Tomb, Thunder Wave, Block, Rock Throw | Paralysis support |
| Carkol | 21 | Steam Engine | Smack Down, Incinerate, Rapid Spin, Smokescreen | Punishes Grass |
| **Regirock** | 23 | Clear Body | Rock Throw, Stomp, Charge Beam, Bulldoze | Ace, IVs 20 |
