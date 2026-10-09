@ Shared names and trainer fallbacks for chapter 3: the Chain Road, Gallows
@ Wood, Cragholt, the collapsed mine and Brannoc's palace, the Pithead Hall.
@ The say/narrate/npc/sign macros come from data/scripts/slice/common.inc.
@ Contract: docs/dialogue/chapter3.md.

@ ---------------------------------------------------------------------------
@ Trainers. Balance owns the teams and defines these names in
@ include/constants/slice_balance.h; the fallbacks match its picks.
@ ---------------------------------------------------------------------------
#ifndef TRAINER_CHAIN_ROAD_FISHER
#define TRAINER_CHAIN_ROAD_FISHER        TRAINER_DARIAN
#endif
#ifndef TRAINER_CHAIN_ROAD_COLLECTOR
#define TRAINER_CHAIN_ROAD_COLLECTOR     TRAINER_IVAN
#endif
#ifndef TRAINER_GALLOWS_WOOD_BUG_CATCHER
#define TRAINER_GALLOWS_WOOD_BUG_CATCHER TRAINER_LYLE
#endif
#ifndef TRAINER_DEBT_WARDEN
#define TRAINER_DEBT_WARDEN              TRAINER_GRUNT_WEATHER_INST_1
#endif
#ifndef TRAINER_ORE_OFFICE_CLERK
#define TRAINER_ORE_OFFICE_CLERK         TRAINER_GRUNT_WEATHER_INST_2
#endif
#ifndef TRAINER_PITHEAD_FOREMAN
#define TRAINER_PITHEAD_FOREMAN          TRAINER_MIKE_2
#endif
#ifndef TRAINER_PITHEAD_MINER_1
#define TRAINER_PITHEAD_MINER_1          TRAINER_BRICE
#endif
#ifndef TRAINER_PITHEAD_MINER_2
#define TRAINER_PITHEAD_MINER_2          TRAINER_CLARK
#endif
#ifndef TRAINER_BRANNOC
#define TRAINER_BRANNOC                  TRAINER_BRAWLY_1
#endif

@ ---------------------------------------------------------------------------
@ Scene object ids. The map thread's map.json files define these; until they
@ land, they fall back to the object order in docs/dialogue/chapter3.md.
@ ---------------------------------------------------------------------------
#ifndef LOCALID_CRAGHOLT_BRANNOC
#define LOCALID_CRAGHOLT_BRANNOC         1
#endif
#ifndef LOCALID_CRAGHOLT_GUARD
#define LOCALID_CRAGHOLT_GUARD           2
#endif
#ifndef LOCALID_CRAGHOLT_BONDED_1
#define LOCALID_CRAGHOLT_BONDED_1        3
#endif
#ifndef LOCALID_CRAGHOLT_BONDED_2
#define LOCALID_CRAGHOLT_BONDED_2        4
#endif
#ifndef LOCALID_CRAGHOLT_BONDED_3
#define LOCALID_CRAGHOLT_BONDED_3        5
#endif
#ifndef LOCALID_CRAGHOLT_TAMSIN
#define LOCALID_CRAGHOLT_TAMSIN          6
#endif
#ifndef LOCALID_COLLAPSED_MINE_RUBBLE
#define LOCALID_COLLAPSED_MINE_RUBBLE    1
#endif
#ifndef LOCALID_COLLAPSED_MINE_WIL
#define LOCALID_COLLAPSED_MINE_WIL       2
#endif
#ifndef LOCALID_COLLAPSED_MINE_MINER_1
#define LOCALID_COLLAPSED_MINE_MINER_1   3
#endif
#ifndef LOCALID_COLLAPSED_MINE_MINER_2
#define LOCALID_COLLAPSED_MINE_MINER_2   4
#endif
#ifndef LOCALID_OLD_LODGE_TAMSIN
#define LOCALID_OLD_LODGE_TAMSIN         11
#endif

@ ---------------------------------------------------------------------------
@ Speaker names. At most about 13 capitals.
@ ---------------------------------------------------------------------------
Ch3_Name_Brannoc::
	.string "BRANNOC$"
Ch3_Name_Penrose::
	.string "OLD PENROSE$"
Ch3_Name_Wil::
	.string "WIL PENROSE$"
Ch3_Name_Harrow::
	.string "NED HARROW$"
Ch3_Name_Bonded::
	.string "BONDED MAN$"
Ch3_Name_Keeper::
	.string "KEEPER$"
Ch3_Name_Hollen::
	.string "HOLLEN$"
Ch3_Name_Tallis::
	.string "WIDOW TALLIS$"
Ch3_Name_Miner::
	.string "MINER$"
Ch3_Name_Warden::
	.string "DEBT WARDEN$"
Ch3_Name_Collector::
	.string "COLLECTOR$"
Ch3_Name_OreClerk::
	.string "ORE CLERK$"
Ch3_Name_Cook::
	.string "CANTEEN COOK$"

@ ---------------------------------------------------------------------------
@ Where Tamsin stands for the Cragholt arrival scene: beside the player at the
@ arrival trigger. The map thread sets these to match the trigger.
@ ---------------------------------------------------------------------------
#ifndef CRAGHOLT_TAMSIN_SCENE_X
#define CRAGHOLT_TAMSIN_SCENE_X 0
#define CRAGHOLT_TAMSIN_SCENE_Y 0
#endif
