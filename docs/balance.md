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
| Gilt Pavilion courtiers | 11-13 | 10-12 |
| **Corwin, Trial 1** | 12-14 (cap 15) | 12, 12, 13, ace 15 |

## The king's gift

Give the dog at **level 5**, the same as the starter. What keeps it from trivialising
Corwin:

- The dogs are on the Slow experience curve, so they level noticeably slower than the
  starter.
- The soft level cap (15) stops it running away before the first Trial.
- Corwin's Bunnelby carries Mud-Slap and Mud Shot, which hit Entei and Raikou
  super-effectively.

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
(none of them have rematches). Map and dialogue threads should use these constants.
Names, classes and sprites are placeholders where noted until features adds proper
trainer classes and pics.

| Constant | Who | Where | Class / pic (placeholder?) | Team |
|---|---|---|---|---|
| `TRAINER_MAY_ROUTE_103_*`, `TRAINER_BRENDAN_ROUTE_103_*` | **Tamsin** | Edge of Lowmere | Rival / May (placeholder pic) | Starter that beats the player's, Lv 5 |
| `TRAINER_RICK` | Clerk Hobb, Gilded Scale toll | Mire Road | Gentleman (placeholder) | Purrloin 5, Galarian Zigzagoon 5 |
| `TRAINER_TIANA` | Grisk, poacher | Mire Road | Hiker (placeholder) | Wooper 5, Croagunk 6 |
| `TRAINER_ALLEN` | Pip, youngster | Mire Road | Youngster | Bidoof 4, Lillipup 5 |
| `TRAINER_ANDREW` | Willem, grain carter | Mire Road | Fisherman (placeholder) | Mudbray 6, Skwovet 6 |
| `TRAINER_GRUNT_PETALBURG_WOODS` | Wagon clerk | Mire Road event | Team Aqua (placeholder) | Poochyena 6, Galarian Meowth 7 |
| `TRAINER_GRUNT_RUSTURF_TUNNEL` | Guild foreman | Haymarket granary | Team Aqua (placeholder) | Mudbray 9, Timburr 10 |
| `TRAINER_GRUNT_MUSEUM_1` | Clerk | Counting house | Team Aqua (placeholder) | Purrloin 9, Poochyena 9 |
| `TRAINER_GRUNT_MUSEUM_2` | Clerk | Counting house | Team Aqua (placeholder) | Murkrow 9, Pawniard 10 |
| `TRAINER_JOSH` | Albrecht, bidder | Gilt Pavilion | Rich Boy | Lillipup 10, Glameow 11 |
| `TRAINER_TOMMY` | Celeste, bidder | Gilt Pavilion | Lady | Skitty 10, Minccino 11 |
| `TRAINER_MARC` | Fenwick, broker | Gilt Pavilion | Gentleman | Lechonk 11, Aipom 12 |
| `TRAINER_ROXANNE_1` | **Prince Corwin**, Trial 1 | Gilt Pavilion | Leader / Roxanne (placeholder pic) | See below |

Tamsin's constant suffix is the **player's** starter, as in vanilla: `_MUDKIP` means the
player chose Mudkip, so Tamsin has Treecko. Both the May and Brendan sets are identical, so
the vanilla gender check in the script can stay or go. The starters are still Treecko,
Torchic and Mudkip; if the story picks different partners, Tamsin's teams change with them.

### Corwin, Trial 1

Four "lots", one per round of the auction, with Type: Null unveiled last. Holds 2 Potions.

| Lot | Pokémon | Lv | Ability / item | Moves | Role |
|---|---|---|---|---|---|
| 1 | Skwovet | 12 | Cheek Pouch, Oran Berry | Tackle, Bite, Tail Whip, Stuff Cheeks | Eats its berry to heal and raise Defence: Corwin's hoarding |
| 2 | Meowth | 12 | Technician | Fake Out, Feint, Scratch, Pay Day | Pays out coins all match |
| 3 | Bunnelby | 13 | Pickup | Mud-Slap, Quick Attack, Mud Shot, Leer | Ground coverage, the check on Entei and Raikou |
| 4 | **Type: Null** | 15 | Battle Armor | Tackle, Aerial Ace, Scary Face, Double Hit | The final lot. Bulky (95 HP / 95 Def / 95 SpD), no crits |

Type: Null is at the pre-Trial cap, like Roxanne's Nosepass, and holds no item (an Eviolite
would make it unbeatable at this level). Normal type has one weakness, Fighting, which
none of the starters have yet; Mudkip's Rock Smash and wild Riolu or Croagunk from the
Mire Road are the intended answers for players who want an edge.
