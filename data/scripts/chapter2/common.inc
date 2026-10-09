@ Shared names and constants for chapter 2: the Orchard Road, Thornfield and
@ Isolde's palace, the Glasshouse. The say/narrate/npc/sign macros come from
@ data/scripts/slice/common.inc. Contract: docs/dialogue/chapter2.md.

@ ---------------------------------------------------------------------------
@ Trainers. Balance owns the teams; until it adds these names, they fall back
@ to vanilla trainers no other map uses.
@ ---------------------------------------------------------------------------
#ifndef TRAINER_ORCHARD_ROAD_PICKER
#define TRAINER_ORCHARD_ROAD_PICKER     TRAINER_DAISY
#endif
#ifndef TRAINER_ORCHARD_ROAD_BAILIFF
#define TRAINER_ORCHARD_ROAD_BAILIFF    TRAINER_RHETT
#endif
#ifndef TRAINER_ORCHARD_ROAD_POET
#define TRAINER_ORCHARD_ROAD_POET       TRAINER_MARCOS
#endif
#ifndef TRAINER_THORNFIELD_BAILIFF
#define TRAINER_THORNFIELD_BAILIFF      TRAINER_BERKE
#endif
#ifndef TRAINER_PALACE_GARDENER_1
#define TRAINER_PALACE_GARDENER_1       TRAINER_RANDALL
#endif
#ifndef TRAINER_PALACE_GARDENER_2
#define TRAINER_PALACE_GARDENER_2       TRAINER_PARKER
#endif
#ifndef TRAINER_PALACE_COURTIER
#define TRAINER_PALACE_COURTIER         TRAINER_GEORGE
#endif
#ifndef TRAINER_ISOLDE
#define TRAINER_ISOLDE                  TRAINER_NORMAN_1
#endif

@ ---------------------------------------------------------------------------
@ Speaker names. At most about 13 capitals (64 px in the small font).
@ ---------------------------------------------------------------------------
Ch2_Name_Voss::
	.string "STEWARD VOSS$"
Ch2_Name_Willow::
	.string "TOM WILLOW$"
Ch2_Name_Mae::
	.string "MAE WILLOW$"
Ch2_Name_Pip::
	.string "PIP$"
Ch2_Name_Mabry::
	.string "OLD MABRY$"
Ch2_Name_Fenwick::
	.string "FENWICK$"
Ch2_Name_Warden::
	.string "ROAD WARDEN$"
Ch2_Name_Bailiff::
	.string "BAILIFF$"
Ch2_Name_Gardener::
	.string "GARDENER$"
Ch2_Name_Picker::
	.string "PICKER$"
Ch2_Name_Poet::
	.string "POET$"
Ch2_Name_Tenant::
	.string "TENANT$"
Ch2_Name_Courtier::
	.string "COURTIER$"
