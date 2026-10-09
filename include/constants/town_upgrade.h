#ifndef GUARD_CONSTANTS_TOWN_UPGRADE_H
#define GUARD_CONSTANTS_TOWN_UPGRADE_H

// Lowmere's upgrade stages, stored in VAR_LOWMERE_STAGE.
#define LOWMERE_STAGE_0     0 // Start: broken well, boarded houses
#define LOWMERE_STAGE_1     1 // Pact with Haymarket (Trial 1)
#define LOWMERE_STAGE_2     2 // Three pacts (through Cragholt)
#define LOWMERE_STAGE_3     3 // Six pacts (through Lumenhall)
#define LOWMERE_STAGE_4     4 // Eight pacts (through Duskmoor) and the siege survived
#define LOWMERE_STAGE_COUNT 5

// Pacts signed needed to reach each stage. Stage 4 also needs FLAG_LOWMERE_SIEGE_SURVIVED.
#define LOWMERE_STAGE_1_PACTS 1
#define LOWMERE_STAGE_2_PACTS 3
#define LOWMERE_STAGE_3_PACTS 6
#define LOWMERE_STAGE_4_PACTS 8

// The nine sibling towns, in Trial order. Passed to the pact specials in VAR_0x8004.
#define PACT_HAYMARKET  0 // Corwin, Normal
#define PACT_THORNFIELD 1 // Isolde, Grass
#define PACT_CRAGHOLT   2 // Brannoc, Rock
#define PACT_SALTMERE   3 // Marisol, Water
#define PACT_VOLTAINE   4 // Teodor, Electric
#define PACT_LUMENHALL  5 // Seraphine, Psychic
#define PACT_EMBERFORT  6 // Garrick, Fire
#define PACT_DUSKMOOR   7 // Vespera, Dark
#define PACT_HIGHCREST  8 // Aurelian, Dragon
#define PACT_TOWN_COUNT 9

// The stance the player takes when dictating a pact after a Trial of Standing.
// Every pact sends resources to Lowmere; the terms decide what else it does.
// Passed to SignPact in VAR_0x8005 and read back with GetPactTerms.
#define PACT_TERMS_RESTITUTION 0 // The sibling pays: extra goods for Lowmere
#define PACT_TERMS_REFORM      1 // The sibling's town is fixed: its people benefit
#define PACT_TERMS_CLEMENCY    2 // The sibling keeps face: goodwill, a possible ally
#define PACT_TERMS_COUNT       3
#define PACT_TERMS_NONE        0xFF // No pact signed with that town yet

// Values of VAR_KINGS_GIFT_DOG.
#define KINGS_GIFT_ENTEI   0
#define KINGS_GIFT_RAIKOU  1
#define KINGS_GIFT_SUICUNE 2

#endif // GUARD_CONSTANTS_TOWN_UPGRADE_H
