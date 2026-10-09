@ Chapter 3: Cragholt (the Rustboro City slot) and its interiors, and the
@ collapsed mine (Rusturf Tunnel). The pit collapse (Brannoc's crime scene),
@ the case (the bonding roll, the quota ledger and Wil Penrose), the bell
@ crate the Pale Choir left at the collapse face, and the town after the pact.
@ Object, trigger and sign placement: docs/dialogue/chapter3.md#cragholt


@ ---------------------------------------------------------------------------
@ ON_TRANSITION helper for the town map. Temp flags are cleared on every map
@ load, so each object group is shown or hidden from VAR_CRAGHOLT_STATE here.
@   FLAG_TEMP_1  collapse scene: Brannoc, a guard, three bonded men (state 0 only)
@   FLAG_TEMP_2  Tamsin (from state 1)
@   FLAG_TEMP_3  the bonded crew waiting at the pithead (until state 3)
@   FLAG_TEMP_4  freed miners in the square (from state 3)
@   FLAG_TEMP_5  the guard on the Pithead Hall door (until state 2)
@ ---------------------------------------------------------------------------
Cragholt_EventScript_SetObjects::
	call_if_ne VAR_CRAGHOLT_STATE, 0, Cragholt_EventScript_HideCollapse
	call_if_lt VAR_CRAGHOLT_STATE, 1, Cragholt_EventScript_HideTamsin
	call_if_ge VAR_CRAGHOLT_STATE, 3, Cragholt_EventScript_HideCrew
	call_if_lt VAR_CRAGHOLT_STATE, 3, Cragholt_EventScript_HideFreedMiners
	call_if_ge VAR_CRAGHOLT_STATE, 2, Cragholt_EventScript_HideHallGuard
	return

Cragholt_EventScript_HideCollapse::
	setflag FLAG_TEMP_1
	return

Cragholt_EventScript_HideTamsin::
	setflag FLAG_TEMP_2
	return

Cragholt_EventScript_HideCrew::
	setflag FLAG_TEMP_3
	return

Cragholt_EventScript_HideFreedMiners::
	setflag FLAG_TEMP_4
	return

Cragholt_EventScript_HideHallGuard::
	setflag FLAG_TEMP_5
	return

@ ---------------------------------------------------------------------------
@ Arrival (VAR_CRAGHOLT_STATE 0 -> 1). Coord trigger on the road into town,
@ with the pithead lift in view. On screen: the lift bell rings, the pit
@ comes down, the lift rises empty, and Brannoc sends the next crew down.
@ ---------------------------------------------------------------------------
Cragholt_EventScript_Arrival::
	lockall
	narrate Cragholt_Text_FirstLook
	closemessage
	playse SE_DING_DONG
	waitse
	delay 30
	playse SE_DING_DONG
	waitse
	delay 20
	playbgm MUS_ENCOUNTER_SUSPICIOUS, FALSE
	playse SE_M_EARTHQUAKE
	call Slice_EventScript_ShakeScreenSmall
	call Slice_EventScript_ShakeScreenSmall
	call Slice_EventScript_ShakeScreenSmall
	waitse
	narrate Cragholt_Text_Dust
	closemessage
	delay 40
	playse SE_ELEVATOR
	waitse
	narrate Cragholt_Text_LiftEmpty
	closemessage
	applymovement LOCALID_CRAGHOLT_BRANNOC, Cragholt_Movement_BrannocToLift
	waitmovement 0
	say Ch3_Name_Brannoc, Cragholt_Text_BrannocHowMany
	say Slice_Name_Guard, Cragholt_Text_GuardEleven
	say Ch3_Name_Brannoc, Cragholt_Text_BrannocRockfall
	say Slice_Name_Guard, Cragholt_Text_GuardNames
	say Ch3_Name_Brannoc, Cragholt_Text_BrannocNoNames
	say Ch3_Name_Brannoc, Cragholt_Text_BrannocNextCrew
	closemessage
	applymovement LOCALID_CRAGHOLT_BONDED_1, Cragholt_Movement_CrewToLift
	applymovement LOCALID_CRAGHOLT_BONDED_2, Cragholt_Movement_CrewToLift
	applymovement LOCALID_CRAGHOLT_BONDED_3, Cragholt_Movement_CrewToLift
	waitmovement 0
	say Ch3_Name_Bonded, Cragholt_Text_BondedStillDust
	say Slice_Name_Guard, Cragholt_Text_GuardIn
	closemessage
	fadescreen FADE_TO_BLACK
	removeobject LOCALID_CRAGHOLT_BONDED_1
	removeobject LOCALID_CRAGHOLT_BONDED_2
	removeobject LOCALID_CRAGHOLT_BONDED_3
	removeobject LOCALID_CRAGHOLT_GUARD
	playse SE_ELEVATOR
	waitse
	fadescreen FADE_FROM_BLACK
	narrate Cragholt_Text_LiftGoesDown
	closemessage
	applymovement LOCALID_CRAGHOLT_BRANNOC, Common_Movement_ExclamationMark
	waitmovement 0
	applymovement LOCALID_CRAGHOLT_BRANNOC, Common_Movement_FacePlayer
	waitmovement 0
	say_g Ch3_Name_Brannoc, Cragholt_Text_BrannocSeesYou_M, Cragholt_Text_BrannocSeesYou_F
	say Ch3_Name_Brannoc, Cragholt_Text_BrannocLiftSomething
	closemessage
	setflag FLAG_MET_BRANNOC
	fadescreen FADE_TO_BLACK
	removeobject LOCALID_CRAGHOLT_BRANNOC
	@ Tamsin's spot is by the quota board; for the scene she arrives on the
	@ road, beside the player at the trigger.
	clearflag FLAG_TEMP_2
	setobjectxyperm LOCALID_CRAGHOLT_TAMSIN, CRAGHOLT_TAMSIN_SCENE_X, CRAGHOLT_TAMSIN_SCENE_Y
	addobject LOCALID_CRAGHOLT_TAMSIN
	fadedefaultbgm
	fadescreen FADE_FROM_BLACK
	applymovement LOCALID_CRAGHOLT_TAMSIN, Common_Movement_FacePlayer
	waitmovement 0
	say Slice_Name_Tamsin, Cragholt_Text_TamsinToftSentMe
	say Slice_Name_Tamsin, Cragholt_Text_TamsinPlan
	closemessage
	setvar VAR_CRAGHOLT_STATE, 1
	releaseall
	end

Cragholt_Movement_BrannocToLift:
	walk_in_place_faster_right
	delay_16
	walk_in_place_faster_right
	step_end

Cragholt_Movement_CrewToLift:
	walk_in_place_faster_left
	step_end

@ ---------------------------------------------------------------------------
@ Tamsin: hints for the case, in any order.
@ ---------------------------------------------------------------------------
Cragholt_EventScript_Tamsin::
	lock
	faceplayer
	setspeaker Slice_Name_Tamsin
	goto_if_ge VAR_CRAGHOLT_STATE, 3, Cragholt_EventScript_TamsinAfterPact
	goto_if_set FLAG_CRAGHOLT_TRIAL_GRANTED, Cragholt_EventScript_TamsinGoWin
	goto_if_unset FLAG_FOUND_BONDING_ROLL, Cragholt_EventScript_TamsinHintRoll
	goto_if_unset FLAG_FOUND_QUOTA_LEDGER, Cragholt_EventScript_TamsinHintLedger
	goto_if_unset FLAG_RESCUED_WIL, Cragholt_EventScript_TamsinHintWil
	msgbox Cragholt_Text_TamsinHintMagistrate, MSGBOX_DEFAULT
	release
	end

Cragholt_EventScript_TamsinHintRoll::
	msgbox Cragholt_Text_TamsinHintRoll, MSGBOX_DEFAULT
	release
	end

Cragholt_EventScript_TamsinHintLedger::
	msgbox Cragholt_Text_TamsinHintLedger, MSGBOX_DEFAULT
	release
	end

Cragholt_EventScript_TamsinHintWil::
	msgbox Cragholt_Text_TamsinHintWil, MSGBOX_DEFAULT
	release
	end

Cragholt_EventScript_TamsinGoWin::
	msgbox Cragholt_Text_TamsinGoWin, MSGBOX_DEFAULT
	release
	end

Cragholt_EventScript_TamsinAfterPact::
	msgbox Cragholt_Text_TamsinAfterPact, MSGBOX_DEFAULT
	release
	end

@ ---------------------------------------------------------------------------
@ The pithead: the waiting crew and their guard (until the pact), and the
@ freed miners in the square (after it).
@ ---------------------------------------------------------------------------
Cragholt_EventScript_CrewGuard::
	npc Slice_Name_Guard, Cragholt_Text_CrewGuard
	end

Cragholt_EventScript_Crew1::
	npc Ch3_Name_Bonded, Cragholt_Text_Crew1
	end

Cragholt_EventScript_Crew2::
	npc Ch3_Name_Bonded, Cragholt_Text_Crew2
	end

Cragholt_EventScript_FreedMiner1::
	lock
	faceplayer
	setspeaker Ch3_Name_Miner
	getpactterms PACT_CRAGHOLT
	goto_if_eq VAR_RESULT, PACT_TERMS_REFORM, Cragholt_EventScript_FreedMinerReform
	msgbox Cragholt_Text_FreedMiner1, MSGBOX_DEFAULT
	release
	end

Cragholt_EventScript_FreedMinerReform::
	msgbox Cragholt_Text_FreedMinerReform, MSGBOX_DEFAULT
	release
	end

Cragholt_EventScript_FreedMiner2::
	npc Ch3_Name_Miner, Cragholt_Text_FreedMiner2
	end

Cragholt_EventScript_Lift::
	goto_if_ge VAR_CRAGHOLT_STATE, 3, Cragholt_EventScript_LiftAfterPact
	sign Cragholt_Text_Lift
	end

Cragholt_EventScript_LiftAfterPact::
	sign Cragholt_Text_LiftAfterPact
	end

@ The quota board in the square.
Cragholt_EventScript_QuotaBoard::
	goto_if_ge VAR_CRAGHOLT_STATE, 3, Cragholt_EventScript_QuotaBoardAfterPact
	sign Cragholt_Text_QuotaBoard
	end

Cragholt_EventScript_QuotaBoardAfterPact::
	sign Cragholt_Text_QuotaBoardAfterPact
	end

@ ---------------------------------------------------------------------------
@ Townsfolk and signs.
@ ---------------------------------------------------------------------------
Cragholt_EventScript_HallGuard::
	npc Slice_Name_Guard, Cragholt_Text_HallGuard
	end

Cragholt_EventScript_Woman::
	lock
	faceplayer
	setspeaker Slice_Name_Townsfolk
	goto_if_ge VAR_CRAGHOLT_STATE, 3, Cragholt_EventScript_WomanAfterPact
	msgbox Cragholt_Text_Woman, MSGBOX_DEFAULT
	release
	end

Cragholt_EventScript_WomanAfterPact::
	msgbox Cragholt_Text_WomanAfterPact, MSGBOX_DEFAULT
	release
	end

Cragholt_EventScript_OldMiner::
	lock
	faceplayer
	setspeaker Slice_Name_OldMan
	goto_if_ge VAR_CRAGHOLT_STATE, 3, Cragholt_EventScript_OldMinerAfterPact
	msgbox Cragholt_Text_OldMiner, MSGBOX_DEFAULT
	release
	end

Cragholt_EventScript_OldMinerAfterPact::
	@ Under clemency the deaths stay an accident in the record.
	getpactterms PACT_CRAGHOLT
	goto_if_eq VAR_RESULT, PACT_TERMS_CLEMENCY, Cragholt_EventScript_OldMinerClemency
	msgbox Cragholt_Text_OldMinerAfterPact, MSGBOX_DEFAULT
	release
	end

Cragholt_EventScript_OldMinerClemency::
	msgbox Cragholt_Text_OldMinerClemency, MSGBOX_DEFAULT
	release
	end

Cragholt_EventScript_Child::
	npc Slice_Name_Child, Cragholt_Text_Child
	end

Cragholt_EventScript_TownSign::
	sign Cragholt_Text_TownSign
	end

Cragholt_EventScript_HallSign::
	sign Cragholt_Text_HallSign
	end

Cragholt_EventScript_OfficeSign::
	sign Cragholt_Text_OfficeSign
	end

Cragholt_EventScript_BarracksSign::
	sign Cragholt_Text_BarracksSign
	end

Cragholt_EventScript_MineSign::
	sign Cragholt_Text_MineSign
	end

@ ---------------------------------------------------------------------------
@ The bonding barracks (the Trainer School slot). The roll of bonded men.
@ ---------------------------------------------------------------------------
Cragholt_Barracks_EventScript_Keeper::
	lock
	faceplayer
	setspeaker Ch3_Name_Keeper
	goto_if_ge VAR_CRAGHOLT_STATE, 3, Cragholt_Barracks_EventScript_KeeperAfterPact
	msgbox Cragholt_Text_Keeper, MSGBOX_DEFAULT
	release
	end

Cragholt_Barracks_EventScript_KeeperAfterPact::
	msgbox Cragholt_Text_KeeperAfterPact, MSGBOX_DEFAULT
	release
	end

Cragholt_Barracks_EventScript_Roll::
	goto_if_set FLAG_FOUND_BONDING_ROLL, Cragholt_Barracks_EventScript_RollAgain
	lockall
	narrate Cragholt_Text_RollOpen
	narrate Cragholt_Text_RollStruck
	narrate Cragholt_Text_RollHarrow
	setspeaker SP_NAME_NONE
	giveitem ITEM_BONDING_ROLL
	setflag FLAG_FOUND_BONDING_ROLL
	releaseall
	end

Cragholt_Barracks_EventScript_RollAgain::
	sign Cragholt_Text_RollAgain
	end

Cragholt_Barracks_EventScript_Sleeper::
	npc Ch3_Name_Bonded, Cragholt_Text_Sleeper
	end

Cragholt_Barracks_EventScript_Bunks::
	goto_if_ge VAR_CRAGHOLT_STATE, 3, Cragholt_Barracks_EventScript_BunksAfterPact
	sign Cragholt_Text_Bunks
	end

Cragholt_Barracks_EventScript_BunksAfterPact::
	sign Cragholt_Text_BunksAfterPact
	end

@ ---------------------------------------------------------------------------
@ The Chancellery ore office (the Devon Corp slot). The quota ledger.
@ ---------------------------------------------------------------------------
Cragholt_OreOffice_EventScript_Clerk::
	trainerbattle_single TRAINER_ORE_OFFICE_CLERK, Cragholt_Text_ClerkIntro, Cragholt_Text_ClerkDefeat
	npc Ch3_Name_OreClerk, Cragholt_Text_ClerkPostBattle
	end

Cragholt_OreOffice_EventScript_Ledger::
	goto_if_set FLAG_FOUND_QUOTA_LEDGER, Cragholt_OreOffice_EventScript_LedgerAgain
	lockall
	narrate Cragholt_Text_LedgerOpen
	narrate Cragholt_Text_LedgerBonuses
	narrate Cragholt_Text_LedgerVenn
	setspeaker SP_NAME_NONE
	giveitem ITEM_QUOTA_LEDGER
	setflag FLAG_FOUND_QUOTA_LEDGER
	releaseall
	end

Cragholt_OreOffice_EventScript_LedgerAgain::
	sign Cragholt_Text_LedgerAgain
	end

Cragholt_OreOffice_EventScript_Assayer::
	npc Slice_Name_Clerk, Cragholt_Text_Assayer
	end

Cragholt_OreOffice_EventScript_OreSamples::
	sign Cragholt_Text_OreSamples
	end

@ ---------------------------------------------------------------------------
@ The magistrate's hall. Magistrate Hollen needs the roll, the ledger and Wil.
@ Wil waits here once rescued, until the pact sends him home.
@ ---------------------------------------------------------------------------
Cragholt_MagistrateHall_EventScript_HideWil::
	setflag FLAG_TEMP_1
	return

Cragholt_MagistrateHall_EventScript_SetObjects::
	goto_if_unset FLAG_RESCUED_WIL, Cragholt_MagistrateHall_EventScript_HideWil
	call_if_ge VAR_CRAGHOLT_STATE, 3, Cragholt_MagistrateHall_EventScript_HideWil
	return

Cragholt_MagistrateHall_EventScript_Hollen::
	lock
	faceplayer
	setspeaker Ch3_Name_Hollen
	goto_if_ge VAR_CRAGHOLT_STATE, 3, Cragholt_MagistrateHall_EventScript_HollenAfterPact
	goto_if_set FLAG_CRAGHOLT_TRIAL_GRANTED, Cragholt_MagistrateHall_EventScript_HollenGoOn
	goto_if_unset FLAG_FOUND_BONDING_ROLL, Cragholt_MagistrateHall_EventScript_HollenNeedProof
	goto_if_unset FLAG_FOUND_QUOTA_LEDGER, Cragholt_MagistrateHall_EventScript_HollenNeedProof
	goto_if_unset FLAG_RESCUED_WIL, Cragholt_MagistrateHall_EventScript_HollenNeedProof
	goto Cragholt_MagistrateHall_EventScript_GrantTrial
	end

Cragholt_MagistrateHall_EventScript_HollenNeedProof::
	msgbox Cragholt_Text_HollenMiracle, MSGBOX_DEFAULT
	setspeaker Ch3_Name_Hollen
	msgbox Cragholt_Text_HollenNeed, MSGBOX_DEFAULT
	setspeaker SP_NAME_NONE
	call_if_set FLAG_FOUND_BONDING_ROLL, Cragholt_MagistrateHall_EventScript_HaveRoll
	call_if_set FLAG_FOUND_QUOTA_LEDGER, Cragholt_MagistrateHall_EventScript_HaveLedger
	call_if_set FLAG_RESCUED_WIL, Cragholt_MagistrateHall_EventScript_HaveWil
	release
	end

Cragholt_MagistrateHall_EventScript_HaveRoll::
	msgbox Cragholt_Text_HaveRoll, MSGBOX_DEFAULT
	return

Cragholt_MagistrateHall_EventScript_HaveLedger::
	msgbox Cragholt_Text_HaveLedger, MSGBOX_DEFAULT
	return

Cragholt_MagistrateHall_EventScript_HaveWil::
	msgbox Cragholt_Text_HaveWil, MSGBOX_DEFAULT
	return

Cragholt_MagistrateHall_EventScript_GrantTrial::
	msgbox Cragholt_Text_HollenReads, MSGBOX_DEFAULT
	say Ch3_Name_Wil, Cragholt_Text_WilTestifies
	say Ch3_Name_Hollen, Cragholt_Text_HollenTwoYears
	say_g Ch3_Name_Hollen, Cragholt_Text_HollenGrant_M, Cragholt_Text_HollenGrant_F
	closemessage
	playse SE_BANG
	waitse
	narrate Cragholt_Text_HollenStamps
	say Ch3_Name_Hollen, Cragholt_Text_HollenHeLikesStrength
	setflag FLAG_CRAGHOLT_TRIAL_GRANTED
	setvar VAR_CRAGHOLT_STATE, 2
	release
	end

Cragholt_MagistrateHall_EventScript_HollenGoOn::
	msgbox Cragholt_Text_HollenGoOn, MSGBOX_DEFAULT
	release
	end

Cragholt_MagistrateHall_EventScript_HollenAfterPact::
	msgbox Cragholt_Text_HollenAfterPact, MSGBOX_DEFAULT
	release
	end

Cragholt_MagistrateHall_EventScript_Wil::
	npc Ch3_Name_Wil, Cragholt_Text_WilInHall
	end

Cragholt_MagistrateHall_EventScript_Reports::
	sign Cragholt_Text_Reports
	end

@ ---------------------------------------------------------------------------
@ Widow Tallis's house.
@ ---------------------------------------------------------------------------
Cragholt_TallisHouse_EventScript_Tallis::
	lock
	faceplayer
	setspeaker Ch3_Name_Tallis
	goto_if_ge VAR_CRAGHOLT_STATE, 3, Cragholt_TallisHouse_EventScript_TallisAfterPact
	msgbox Cragholt_Text_Tallis, MSGBOX_DEFAULT
	release
	end

Cragholt_TallisHouse_EventScript_TallisAfterPact::
	getpactterms PACT_CRAGHOLT
	switch VAR_RESULT
	case PACT_TERMS_RESTITUTION, Cragholt_TallisHouse_EventScript_TallisRestitution
	case PACT_TERMS_CLEMENCY, Cragholt_TallisHouse_EventScript_TallisClemency
	msgbox Cragholt_Text_TallisAfterPact, MSGBOX_DEFAULT
	release
	end

Cragholt_TallisHouse_EventScript_TallisRestitution::
	msgbox Cragholt_Text_TallisRestitution, MSGBOX_DEFAULT
	release
	end

Cragholt_TallisHouse_EventScript_TallisClemency::
	msgbox Cragholt_Text_TallisClemency, MSGBOX_DEFAULT
	release
	end

Cragholt_TallisHouse_EventScript_Boots::
	sign Cragholt_Text_Boots
	end

@ The other house: a mason's family.
Cragholt_MasonHouse_EventScript_Mason::
	lock
	faceplayer
	setspeaker Slice_Name_Townsfolk
	goto_if_ge VAR_CRAGHOLT_STATE, 3, Cragholt_MasonHouse_EventScript_MasonAfterPact
	msgbox Cragholt_Text_Mason, MSGBOX_DEFAULT
	release
	end

Cragholt_MasonHouse_EventScript_MasonAfterPact::
	msgbox Cragholt_Text_MasonAfterPact, MSGBOX_DEFAULT
	release
	end

Cragholt_MasonHouse_EventScript_Girl::
	npc Slice_Name_Girl, Cragholt_Text_MasonGirl
	end

@ ---------------------------------------------------------------------------
@ The collapsed mine (the Rusturf Tunnel slot). Wil and two miners are
@ trapped behind a rubble fall, (20,4)-(21,5) on the haulage way. The bell
@ crate is at the collapse face.
@   FLAG_TEMP_1  the three trapped men (until Wil is out)
@ ---------------------------------------------------------------------------
CollapsedMine_EventScript_SetObjects::
	call_if_set FLAG_RESCUED_WIL, CollapsedMine_EventScript_HideTrapped
	return

@ On load once Wil is out, and in the rescue under the fade: the rubble
@ metatiles become floor (from the map thread).
CollapsedMine_EventScript_ClearRubble::
	setmetatile 20, 4, 0x201, FALSE
	setmetatile 21, 4, 0x201, FALSE
	setmetatile 20, 5, 0x201, FALSE
	setmetatile 21, 5, 0x201, FALSE
	special DrawWholeMapView
	return

CollapsedMine_EventScript_HideTrapped::
	setflag FLAG_TEMP_1
	return

@ Shouting through the rubble before it is cleared.
CollapsedMine_EventScript_Rubble::
	lockall
	narrate CollapsedMine_Text_RubbleVoices
	say Ch3_Name_Wil, CollapsedMine_Text_WilBehindRubble
	setspeaker SP_NAME_NONE
	msgbox CollapsedMine_Text_ClearRubble, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, CollapsedMine_EventScript_RubbleLeave
	closemessage
	fadescreen FADE_TO_BLACK
	playse SE_M_STRENGTH
	waitse
	delay 20
	playse SE_M_ROCK_THROW
	waitse
	call CollapsedMine_EventScript_ClearRubble
	delay 20
	fadescreen FADE_FROM_BLACK
	narrate CollapsedMine_Text_GapOpens
	closemessage
	applymovement LOCALID_COLLAPSED_MINE_WIL, Common_Movement_FacePlayer
	waitmovement 0
	say Ch3_Name_Wil, CollapsedMine_Text_WilThanks
	narrate CollapsedMine_Text_YouTellWil
	say Ch3_Name_Wil, CollapsedMine_Text_WilNets
	say Ch3_Name_Miner, CollapsedMine_Text_MinerNotRockfall
	say Ch3_Name_Wil, CollapsedMine_Text_WilWillTestify
	closemessage
	fadescreen FADE_TO_BLACK
	removeobject LOCALID_COLLAPSED_MINE_WIL
	removeobject LOCALID_COLLAPSED_MINE_MINER_1
	removeobject LOCALID_COLLAPSED_MINE_MINER_2
	setflag FLAG_RESCUED_WIL
	fadescreen FADE_FROM_BLACK
	narrate CollapsedMine_Text_TheyGoUp
	releaseall
	end

CollapsedMine_EventScript_RubbleLeave::
	releaseall
	end

@ The trapped men. They are behind the rubble until the rescue, which then
@ walks them out, so these only answer if the scene is skipped somehow.
CollapsedMine_EventScript_Wil::
	npc Ch3_Name_Wil, CollapsedMine_Text_WilThanks
	end

CollapsedMine_EventScript_TrappedMiner::
	npc Ch3_Name_Miner, CollapsedMine_Text_MinerNotRockfall
	end

@ The bell crate at the collapse face: the Pale Choir set the charges.
CollapsedMine_EventScript_BellCrate::
	goto_if_set FLAG_FOUND_BELL_CRATE, CollapsedMine_EventScript_BellCrateAgain
	lockall
	narrate CollapsedMine_Text_BellCrate
	narrate CollapsedMine_Text_BellCrateCharges
	setflag FLAG_FOUND_BELL_CRATE
	releaseall
	end

CollapsedMine_EventScript_BellCrateAgain::
	sign CollapsedMine_Text_BellCrateAgain
	end

CollapsedMine_EventScript_Props::
	sign CollapsedMine_Text_Props
	end

CollapsedMine_EventScript_Sign::
	sign CollapsedMine_Text_Sign
	end

@ ---------------------------------------------------------------------------
@ Text
@ ---------------------------------------------------------------------------

Cragholt_Text_FirstLook::
	.string "CRAGHOLT. Grey stone houses, grey\n"
	.string "stone streets, and above them all the\l"
	.string "black frame of the pithead lift.\p"
	.string "Men in rope and chain stand in a line\n"
	.string "beside it, waiting their turn.$"

Cragholt_Text_Dust::
	.string "A deep crack runs through the ground.\n"
	.string "Dust pours out of the shaft in a long\l"
	.string "grey breath.\p"
	.string "Then the bell on the lift rope goes\n"
	.string "quiet.$"

Cragholt_Text_LiftEmpty::
	.string "The lift cage comes up out of the dust,\n"
	.string "rattling on its chains.\p"
	.string "It is empty.$"

Cragholt_Text_BrannocHowMany::
	.string "How many?$"

Cragholt_Text_GuardEleven::
	.string "Eleven on that crew, Your Highness.\n"
	.string "Fourteen, counting the three in the\l"
	.string "side gallery.$"

Cragholt_Text_BrannocRockfall::
	.string "Write it as a rockfall. Act of the\n"
	.string "mountain.$"

Cragholt_Text_GuardNames::
	.string "And the names, sir? For the magistrate?$"

Cragholt_Text_BrannocNoNames::
	.string "No names. They were bought, not born\n"
	.string "here. The magistrate doesn't need\l"
	.string "names.$"

Cragholt_Text_BrannocNextCrew::
	.string "Clear the shaft and send the next crew\n"
	.string "down. The Chancellery's wagons come at\l"
	.string "the end of the month, and I'm not short\l"
	.string "a single ton.$"

Cragholt_Text_BondedStillDust::
	.string "The dust's still coming up...$"

Cragholt_Text_GuardIn::
	.string "In. All of you. Move.$"

Cragholt_Text_LiftGoesDown::
	.string "The cage drops into the dark with the\n"
	.string "new crew inside. The bell starts ringing\l"
	.string "again, slow and steady.$"

Cragholt_Text_BrannocSeesYou_M::
	.string "Little brother. I heard Father's pet was\n"
	.string "walking the roads. Come to watch real\l"
	.string "work?$"

Cragholt_Text_BrannocSeesYou_F::
	.string "Little sister. I heard Father's pet was\n"
	.string "walking the roads. Come to watch real\l"
	.string "work?$"

Cragholt_Text_BrannocLiftSomething::
	.string "CORWIN talks. ISOLDE pouts. I dig. This\n"
	.string "town feeds the crown its iron, and the\l"
	.string "crown's never once asked me how.\p"
	.string "If you want a Trial from me, prove you\n"
	.string "can lift something first. Then we'll\l"
	.string "talk.$"

Cragholt_Text_TamsinToftSentMe::
	.string "There you are! TOFT sent me for stone.\n"
	.string "We need it for the walls before winter.\p"
	.string "I've been here a day. I watched that\n"
	.string "happen twice yesterday, too.$"

Cragholt_Text_TamsinPlan::
	.string "Nobody here will say a word against\n"
	.string "BRANNOC. They're scared, and the bonded\l"
	.string "men can't.\p"
	.string "The barracks keeps a roll of every\n"
	.string "bonded man. The ore office tallies the\l"
	.string "quotas. And somebody in that pit must\l"
	.string "still be alive.$"

Cragholt_Text_TamsinHintRoll::
	.string "The bonded men sleep in the barracks by\n"
	.string "the school yard. There's a roll of names\l"
	.string "on the wall. Start there.$"

Cragholt_Text_TamsinHintLedger::
	.string "The Chancellery's ore office keeps the\n"
	.string "quota books. Somebody's paying for\l"
	.string "every ton. Find out who.$"

Cragholt_Text_TamsinHintWil::
	.string "I heard knocking from the old east\n"
	.string "tunnel. Three knocks, then three.\l"
	.string "Somebody's alive under there.$"

Cragholt_Text_TamsinHintMagistrate::
	.string "You've got it all. Take it to the\n"
	.string "magistrate's hall. HOLLEN's an old man,\l"
	.string "but he's not a bought one. I think.$"

Cragholt_Text_TamsinGoWin::
	.string "BRANNOC's in the PITHEAD HALL.\n"
	.string "Everyone knows his ROCK POKéMON are\l"
	.string "stronger than the walls.\p"
	.string "Bring something that can crack stone.$"

Cragholt_Text_TamsinAfterPact::
	.string "They're loading the first stone carts\n"
	.string "for LOWMERE now. Real masons, too. Paid\l"
	.string "ones!\p"
	.string "I'm going home with them. You should\n"
	.string "come and see it.$"

Cragholt_Text_CrewGuard::
	.string "Back off. This crew's for the next\n"
	.string "shift.\p"
	.string "Bought and paid for. Like all of them.$"

Cragholt_Text_Crew1::
	.string "My brother went down on the last crew. I\n"
	.string "watched the lift come up.\p"
	.string "I'm going down next. Same rope, same\n"
	.string "shaft.$"

Cragholt_Text_Crew2::
	.string "They tell us the pit only takes the\n"
	.string "careless.\p"
	.string "My father wasn't careless. He was just\n"
	.string "at the bottom.$"

Cragholt_Text_FreedMiner1::
	.string "Paid! Actually paid, in coin, for a day\n"
	.string "underground. I didn't know what to do\l"
	.string "with it. I bought bread and cried.$"

Cragholt_Text_FreedMinerReform::
	.string "We voted for a safety warden this\n"
	.string "morning. One of us. He can stop the lift\l"
	.string "if the props are bad.\p"
	.string "The prince said he'd go down first,\n"
	.string "every shift, until we trust it.$"

Cragholt_Text_FreedMiner2::
	.string "No rope today. I keep reaching for it.$"

Cragholt_Text_Lift::
	.string "The pithead lift. A brass bell hangs\n"
	.string "from the cage. It rings once for every\l"
	.string "crew that goes down.$"

Cragholt_Text_LiftAfterPact::
	.string "The pithead lift. A new notice on the\n"
	.string "cage reads: “No man goes down who has\l"
	.string "not been paid.”$"

Cragholt_Text_QuotaBoard::
	.string "QUOTA BOARD\p"
	.string "This month: 900 tons. Last month: 800\n"
	.string "tons. The month before: 700 tons.\p"
	.string "Someone has chalked a small cross under\n"
	.string "every number.$"

Cragholt_Text_QuotaBoardAfterPact::
	.string "QUOTA BOARD\p"
	.string "The numbers are scrubbed off. Chalked\n"
	.string "in their place, in a rough hand: “WE DIG\l"
	.string "WHAT IS SAFE.”$"

Cragholt_Text_HallGuard::
	.string "The PITHEAD HALL's closed. The prince\n"
	.string "sees nobody without the magistrate's\l"
	.string "leave.\p"
	.string "And the magistrate gives nobody leave.$"

Cragholt_Text_Woman::
	.string "My husband's a mason. He cuts the pit\n"
	.string "props.\p"
	.string "He says the props they buy now are half\n"
	.string "as thick as they used to be. Cheaper.\l"
	.string "The prince says it doesn't matter.$"

Cragholt_Text_WomanAfterPact::
	.string "They're buying proper timber for the\n"
	.string "props again. My husband actually smiled\l"
	.string "at dinner.$"

Cragholt_Text_OldMiner::
	.string "Fifty years I dug that mountain. We\n"
	.string "lost men, sure. Once a year, maybe.\p"
	.string "Now it's every week. And nobody's ever\n"
	.string "buried here. They're not from here, see.$"

Cragholt_Text_OldMinerAfterPact::
	.string "They read the names in the square this\n"
	.string "morning. All sixty-one. Took the\l"
	.string "magistrate an hour.\p"
	.string "Nobody left. Not one.$"

Cragholt_Text_OldMinerClemency::
	.string "They put up a plaque. “Lost in the\n"
	.string "rockfalls, by act of the mountain.”\p"
	.string "The mountain didn't buy them. But\n"
	.string "that's what it says.$"

Cragholt_Text_Child::
	.string "I'm going to be a miner when I grow up!\n"
	.string "The prince says the strong ones always\l"
	.string "come back up.$"

Cragholt_Text_TownSign::
	.string "CRAGHOLT\p"
	.string "The City of Stone. Iron for the crown\n"
	.string "since the first king.$"

Cragholt_Text_HallSign::
	.string "THE PITHEAD HALL\p"
	.string "Seat of Prince BRANNOC of CRAGHOLT.$"

Cragholt_Text_OfficeSign::
	.string "CHANCELLERY ORE OFFICE\p"
	.string "Quotas, assays and payments. By order\n"
	.string "of the CHANCELLOR.$"

Cragholt_Text_BarracksSign::
	.string "BONDED LABOUR BARRACKS\p"
	.string "Lights out at dusk. No visitors.$"

Cragholt_Text_MineSign::
	.string "EAST TUNNEL\p"
	.string "Closed after the rockfall. Keep out.$"

Cragholt_Text_Keeper::
	.string "I keep the roll. Names in when they come\n"
	.string "up the road. Names struck when they\l"
	.string "don't come up the shaft.\p"
	.string "I don't write why. Nobody's ever asked\n"
	.string "me to.$"

Cragholt_Text_KeeperAfterPact::
	.string "The roll's gone to the magistrate. I've\n"
	.string "got a new book now. Wages.\p"
	.string "I don't know what to do with a book that\n"
	.string "only goes up.$"

Cragholt_Text_RollOpen::
	.string "A long roll of names is nailed to the\n"
	.string "wall. THE BONDED LABOUR OF CRAGHOLT, in\l"
	.string "the keeper's square hand.$"

Cragholt_Text_RollStruck::
	.string "Two hundred and six names. Sixty-one\n"
	.string "struck through.\p"
	.string "No cause written. Just a line, and a\n"
	.string "date.$"

Cragholt_Text_RollHarrow::
	.string "Near the bottom, newly added: “HARROW,\n"
	.string "N. From THORNFIELD. Debt, two hundred\l"
	.string "and twelve.”\p"
	.string "You take down the roll.$"

Cragholt_Text_RollAgain::
	.string "Pale marks on the wall show where the\n"
	.string "roll was nailed.$"

Cragholt_Text_Sleeper::
	.string "...Is it my shift? No?\p"
	.string "Then let me sleep. It's the only time the\n"
	.string "rope comes off.$"

Cragholt_Text_Bunks::
	.string "Bunks stacked four high. Some have a\n"
	.string "few things on them: a carved stone, a\l"
	.string "letter, a child's ribbon.\p"
	.string "Some have nothing at all.$"

Cragholt_Text_BunksAfterPact::
	.string "The bunks are empty. Someone has left a\n"
	.string "note on one: “GONE HOME.”$"

Cragholt_Text_ClerkIntro::
	.string "The Chancellery's books are not for\n"
	.string "public reading. Least of all for royals.$"

Cragholt_Text_ClerkDefeat::
	.string "Fine! Read it! It's all perfectly legal!$"

Cragholt_Text_ClerkPostBattle::
	.string "The Chancellery sets the quota. The\n"
	.string "prince meets it. How he meets it is his\l"
	.string "business.\p"
	.string "That's what I was told to say, anyway.$"

Cragholt_Text_LedgerOpen::
	.string "A heavy ledger lies open on the clerk's\n"
	.string "desk. QUOTAS AND BONUSES, CRAGHOLT\l"
	.string "DIVISION.$"

Cragholt_Text_LedgerBonuses::
	.string "Every month the quota is met. Every\n"
	.string "month a bonus is paid to Prince\l"
	.string "BRANNOC.\p"
	.string "Beside each bonus, a smaller line:\n"
	.string "“Labour, purchased,” and a number.$"

Cragholt_Text_LedgerVenn::
	.string "The last page reads: “Quota raised\n"
	.string "again, by order of the CHANCELLERY.\l"
	.string "Signed, M. VENN.”\p"
	.string "You take the ledger.$"

Cragholt_Text_LedgerAgain::
	.string "The clerk's desk. A clean ledger lies\n"
	.string "open. Nothing is written in it yet.$"

Cragholt_Text_Assayer::
	.string "I test the ore. It's good ore. Best in\n"
	.string "the kingdom.\p"
	.string "I don't ask how it gets up here. I just\n"
	.string "test it.$"

Cragholt_Text_OreSamples::
	.string "Chunks of iron ore, labelled by tunnel.\n"
	.string "The east tunnel's tag has a black line\l"
	.string "through it.$"

Cragholt_Text_HollenMiracle::
	.string "Two years. Two years without a single\n"
	.string "death report from the pits.\p"
	.string "I was told it was a miracle. Better\n"
	.string "props. Better men.$"

Cragholt_Text_HollenNeed::
	.string "If you're telling me it's a lie, I need it\n"
	.string "in my hands. The names. The money. And\l"
	.string "someone who was down there.$"

Cragholt_Text_HaveRoll::
	.string "You have the bonding roll.$"

Cragholt_Text_HaveLedger::
	.string "You have the quota ledger.$"

Cragholt_Text_HaveWil::
	.string "WIL PENROSE will testify.$"

Cragholt_Text_HollenReads::
	.string "HOLLEN reads the roll. He reads it again.\n"
	.string "Then he opens the ledger and lays it\l"
	.string "beside the roll, line by line.$"

Cragholt_Text_WilTestifies::
	.string "Three crews a month went into the east\n"
	.string "tunnel. The props were thin as broom\l"
	.string "handles.\p"
	.string "We knocked on the walls when the dust\n"
	.string "came down. Nobody ever came.$"

Cragholt_Text_HollenTwoYears::
	.string "Sixty-one men. And I signed off every\n"
	.string "month that the pits were safe.\p"
	.string "I'm an old fool. But I'm not a liar, and I\n"
	.string "won't start now.$"

Cragholt_Text_HollenGrant_M::
	.string "Under the old law, I grant the heir from\n"
	.string "LOWMERE a Trial of Standing against\l"
	.string "BRANNOC of CRAGHOLT.\p"
	.string "His terms will bind the prince if he wins.$"

Cragholt_Text_HollenGrant_F::
	.string "Under the old law, I grant the heir from\n"
	.string "LOWMERE a Trial of Standing against\l"
	.string "BRANNOC of CRAGHOLT.\p"
	.string "Her terms will bind the prince if she\n"
	.string "wins.$"

Cragholt_Text_HollenStamps::
	.string "HOLLEN presses his seal into the wax.$"

Cragholt_Text_HollenHeLikesStrength::
	.string "BRANNOC respects only strength. Show\n"
	.string "him some.\p"
	.string "And bring those men home.$"

Cragholt_Text_HollenGoOn::
	.string "The PITHEAD HALL's guard has my order.\n"
	.string "Go.$"

Cragholt_Text_HollenAfterPact::
	.string "I read every name in the square.\n"
	.string "Sixty-one. I'll write a death report for\l"
	.string "each of them, if it takes me all winter.$"

Cragholt_Text_WilInHall::
	.string "The magistrate's given me a chair and a\n"
	.string "blanket. It's the first chair I've sat in\l"
	.string "for two years.$"

Cragholt_Text_Reports::
	.string "A shelf of pit reports. “No deaths. No\n"
	.string "deaths. No deaths.” The same signature\l"
	.string "on every page.$"

Cragholt_Text_Tallis::
	.string "My JORY went down the east tunnel last\n"
	.string "spring. They told me it was a fever. That\l"
	.string "he died in the barracks.\p"
	.string "He was never in the barracks. He was a\n"
	.string "free miner. He was paid.\p"
	.string "They bought the crew, and then they\n"
	.string "sent him down to lead it.$"

Cragholt_Text_TallisAfterPact::
	.string "They read JORY's name in the square.\n"
	.string "Out loud, with all the others.\p"
	.string "It doesn't bring him back. But it's true\n"
	.string "now. It's written down.$"

Cragholt_Text_TallisRestitution::
	.string "The prince came to my door himself. With\n"
	.string "a purse, and his hat in his hands.\p"
	.string "I took the purse. I didn't take his hand.\n"
	.string "Maybe next year.$"

Cragholt_Text_TallisClemency::
	.string "They say it was an accident now.\n"
	.string "Officially. The rockfall that took my\l"
	.string "JORY.\p"
	.string "I know what it was. So do you.$"

Cragholt_Text_Boots::
	.string "A pair of miner's boots by the door,\n"
	.string "polished and never worn since.$"

Cragholt_Text_Mason::
	.string "We cut stone for the crown. But we\n"
	.string "never see the crown's coin.\p"
	.string "The Chancellery takes it first, then\n"
	.string "the prince, then the guild. Then it's\l"
	.string "gone.$"

Cragholt_Text_MasonAfterPact::
	.string "LOWMERE's buying our stone now, and\n"
	.string "paying fair for it! My whole crew's going\l"
	.string "south.$"

Cragholt_Text_MasonGirl::
	.string "Papa says not to play near the lift. The\n"
	.string "bell's bad luck.$"

CollapsedMine_Text_RubbleVoices::
	.string "A wall of broken rock blocks the\n"
	.string "haulage way. Behind it, faintly, three\l"
	.string "knocks. Then three more.$"

CollapsedMine_Text_WilBehindRubble::
	.string "Hello? Is someone there? We're alive!\n"
	.string "Three of us!\p"
	.string "The air's going bad. Please!$"

CollapsedMine_Text_ClearRubble::
	.string "Clear the rubble with your POKéMON?$"

CollapsedMine_Text_GapOpens::
	.string "Rock by rock, your POKéMON pull the fall\n"
	.string "apart. A gap opens, and three dusty men\l"
	.string "squeeze through.$"

CollapsedMine_Text_WilThanks::
	.string "Air. Real air. Thank you. I'm WIL. WIL\n"
	.string "PENROSE.$"

CollapsedMine_Text_YouTellWil::
	.string "You tell WIL that his father says the\n"
	.string "nets are mended.$"

CollapsedMine_Text_WilNets::
	.string "...He said that? That means “come home.”\n"
	.string "He used to say it when I was out too\l"
	.string "late.\p"
	.string "I will. Once this is done.$"

CollapsedMine_Text_MinerNotRockfall::
	.string "That was no rockfall. I heard it, before.\n"
	.string "A crack like thunder, from the props\l"
	.string "near the entrance.\p"
	.string "Somebody blew that tunnel.$"

CollapsedMine_Text_WilWillTestify::
	.string "They'll write us down as dead. Struck\n"
	.string "off the roll, no name, no cause.\p"
	.string "Take me to the magistrate. I'll tell him\n"
	.string "what it's like down there.$"

CollapsedMine_Text_TheyGoUp::
	.string "The three men stagger toward the\n"
	.string "daylight. WIL will wait for you in the\l"
	.string "magistrate's hall.$"

CollapsedMine_Text_BellCrate::
	.string "An empty crate is wedged behind the\n"
	.string "fallen props. Stencilled on its side is a\l"
	.string "small pale bell.$"

CollapsedMine_Text_BellCrateCharges::
	.string "Inside: straw packing, and a smell like\n"
	.string "burnt matches. Blasting powder.\p"
	.string "The props here were broken from the\n"
	.string "outside in. Someone set charges at the\l"
	.string "mouth of the tunnel.$"

CollapsedMine_Text_BellCrateAgain::
	.string "The crate with the pale bell. Blasting\n"
	.string "powder has stained the straw black.$"

CollapsedMine_Text_Props::
	.string "Pit props, thin as broom handles,\n"
	.string "snapped in two.$"

CollapsedMine_Text_Sign::
	.string "EAST TUNNEL, LEVEL TWO\p"
	.string "Bonded crews only.$"

