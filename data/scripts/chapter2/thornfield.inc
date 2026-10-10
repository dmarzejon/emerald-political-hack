@ Chapter 2: Thornfield (the Petalburg City slot). The Willows' eviction
@ (Isolde's crime scene), the case (the steward's ledger and Old Mabry), the
@ magistrate, Isolde on her terraces, and the town after the pact.
@ Object, trigger and sign placement: docs/dialogue/chapter2.md#thornfield


@ ---------------------------------------------------------------------------
@ ON_TRANSITION helper for the town map. Temp flags are cleared on every map
@ load, so each object group is shown or hidden from VAR_THORNFIELD_STATE here.
@   FLAG_TEMP_1  eviction scene: Voss, two guards, two diggers (state 0 only)
@   FLAG_TEMP_2  the Willows' camp at the town edge (states 1-2)
@   FLAG_TEMP_3  the Willows at their door (state 0, and home again from 3)
@   FLAG_TEMP_4  Isolde on the terraces (until state 2)
@   FLAG_TEMP_5  Tamsin (from state 1)
@   FLAG_TEMP_6  the palace door guard (until state 2)
@ ---------------------------------------------------------------------------
Thornfield_EventScript_SetObjects::
	call_if_ne VAR_THORNFIELD_STATE, 0, Thornfield_EventScript_HideEviction
	call_if_lt VAR_THORNFIELD_STATE, 1, Thornfield_EventScript_HideCamp
	call_if_ge VAR_THORNFIELD_STATE, 3, Thornfield_EventScript_HideCamp
	call_if_eq VAR_THORNFIELD_STATE, 1, Thornfield_EventScript_HideWillowsAtHome
	call_if_eq VAR_THORNFIELD_STATE, 2, Thornfield_EventScript_HideWillowsAtHome
	call_if_ge VAR_THORNFIELD_STATE, 2, Thornfield_EventScript_HideIsoldeAndGuard
	call_if_lt VAR_THORNFIELD_STATE, 1, Thornfield_EventScript_HideTamsin
	return

Thornfield_EventScript_HideEviction::
	setflag FLAG_TEMP_1
	return

Thornfield_EventScript_HideCamp::
	setflag FLAG_TEMP_2
	return

Thornfield_EventScript_HideWillowsAtHome::
	setflag FLAG_TEMP_3
	return

Thornfield_EventScript_HideIsoldeAndGuard::
	setflag FLAG_TEMP_4
	setflag FLAG_TEMP_6
	return

Thornfield_EventScript_HideTamsin::
	setflag FLAG_TEMP_5
	return

@ The Willows' door is boarded in states 1-2, until the pact. Called from the
@ map's on-load script and from the eviction scene, under the fade. Metatiles
@ from the map thread. The redraw is needed: the door is on screen as the
@ player walks in from the Orchard Road.
Thornfield_EventScript_BoardUpDoor::
	setmetatile 27, 15, 0x2A3, TRUE
	setmetatile 27, 16, 0x2A4, TRUE
	setmetatile 28, 15, 0x2A1, TRUE
	setmetatile 28, 16, 0x2A2, TRUE
	special DrawWholeMapView
	return

@ ---------------------------------------------------------------------------
@ Arrival (VAR_THORNFIELD_STATE 0 -> 1). Coord trigger on the road into town, with the
@ Willows' house in view. On screen: Steward Voss reads the foreclosure, the
@ guards march the family out, the door is boarded, and the gardeners start
@ digging up the bean field.
@ ---------------------------------------------------------------------------
Thornfield_EventScript_Arrival::
	lockall
	narrate Thornfield_Text_FirstLook
	closemessage
	playbgm MUS_ENCOUNTER_SUSPICIOUS, FALSE
	delay 20
	say Ch2_Name_Voss, Thornfield_Text_VossReads
	applymovement LOCALID_THORNFIELD_WILLOW, Thornfield_Movement_Protest
	waitmovement 0
	say Ch2_Name_Willow, Thornfield_Text_WillowWePaid
	say Ch2_Name_Voss, Thornfield_Text_VossInterest
	say Ch2_Name_Mae, Thornfield_Text_MaeWhereWillWeGo
	say Ch2_Name_Voss, Thornfield_Text_VossNotALodgingHouse
	say Ch2_Name_Pip, Thornfield_Text_PipBeans
	say Ch2_Name_Voss, Thornfield_Text_VossBelongsToTheLand
	closemessage
	applymovement LOCALID_THORNFIELD_GUARD_1, Thornfield_Movement_GuardStepIn
	applymovement LOCALID_THORNFIELD_GUARD_2, Thornfield_Movement_GuardStepIn
	waitmovement 0
	narrate Thornfield_Text_LedAway
	closemessage
	@ The guards march the family west along the road, in single file. Each
	@ starts a step after the one ahead so no one bumps into the next.
	applymovement LOCALID_THORNFIELD_GUARD_1, Thornfield_Movement_LeadAway0
	applymovement LOCALID_THORNFIELD_PIP, Thornfield_Movement_LeadAway1
	applymovement LOCALID_THORNFIELD_MAE, Thornfield_Movement_LeadAway2
	applymovement LOCALID_THORNFIELD_WILLOW, Thornfield_Movement_LeadAway3
	applymovement LOCALID_THORNFIELD_GUARD_2, Thornfield_Movement_LeadAway4
	waitmovement 0
	fadescreen FADE_TO_BLACK
	removeobject LOCALID_THORNFIELD_WILLOW
	removeobject LOCALID_THORNFIELD_MAE
	removeobject LOCALID_THORNFIELD_PIP
	removeobject LOCALID_THORNFIELD_GUARD_1
	removeobject LOCALID_THORNFIELD_GUARD_2
	delay 30
	playse SE_BANG
	call Slice_EventScript_ShakeScreenSmall
	playse SE_BANG
	call Slice_EventScript_ShakeScreenSmall
	playse SE_BANG
	call Slice_EventScript_ShakeScreenSmall
	call Thornfield_EventScript_BoardUpDoor
	delay 30
	fadescreen FADE_FROM_BLACK
	narrate Thornfield_Text_DoorBoarded
	closemessage
	applymovement LOCALID_THORNFIELD_DIGGER_1, Thornfield_Movement_Dig
	applymovement LOCALID_THORNFIELD_DIGGER_2, Thornfield_Movement_Dig
	playse SE_M_DIG
	waitmovement 0
	narrate Thornfield_Text_Digging
	closemessage
	applymovement LOCALID_THORNFIELD_VOSS, Common_Movement_ExclamationMark
	waitmovement 0
	applymovement LOCALID_THORNFIELD_VOSS, Common_Movement_FacePlayer
	waitmovement 0
	say_g Ch2_Name_Voss, Thornfield_Text_VossSeesYou_M, Thornfield_Text_VossSeesYou_F
	say Ch2_Name_Voss, Thornfield_Text_VossInOrder
	closemessage
	fadescreen FADE_TO_BLACK
	removeobject LOCALID_THORNFIELD_VOSS
	removeobject LOCALID_THORNFIELD_DIGGER_1
	removeobject LOCALID_THORNFIELD_DIGGER_2
	clearflag FLAG_TEMP_2
	addobject LOCALID_THORNFIELD_CAMP_WILLOW
	addobject LOCALID_THORNFIELD_CAMP_MAE
	addobject LOCALID_THORNFIELD_CAMP_PIP
	@ Tamsin's spot is by the square; for the scene she arrives on the road,
	@ beside the player at the trigger (29,17-19).
	clearflag FLAG_TEMP_5
	setobjectxyperm LOCALID_THORNFIELD_TAMSIN, 28, 19
	addobject LOCALID_THORNFIELD_TAMSIN
	fadedefaultbgm
	fadescreen FADE_FROM_BLACK
	applymovement LOCALID_THORNFIELD_TAMSIN, Common_Movement_FacePlayer
	waitmovement 0
	say Slice_Name_Tamsin, Thornfield_Text_TamsinThirdFamily
	say Slice_Name_Tamsin, Thornfield_Text_TamsinPlan
	closemessage
	setvar VAR_THORNFIELD_STATE, 1
	releaseall
	end

Thornfield_Movement_Protest:
	walk_in_place_faster_left
	walk_in_place_faster_right
	walk_in_place_faster_left
	step_end

Thornfield_Movement_GuardStepIn:
	walk_in_place_faster_up
	step_end

@ Row 17 is clear from x 14 to 29, so five steps west is safe for all five.
Thornfield_Movement_LeadAway0:
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	step_end

Thornfield_Movement_LeadAway1:
	face_left
	delay_16
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	step_end

Thornfield_Movement_LeadAway2:
	face_left
	delay_16
	delay_16
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	step_end

Thornfield_Movement_LeadAway3:
	face_left
	delay_16
	delay_16
	delay_16
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	step_end

Thornfield_Movement_LeadAway4:
	face_left
	delay_16
	delay_16
	delay_16
	delay_16
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	step_end

Thornfield_Movement_Dig:
	walk_in_place_down
	walk_in_place_down
	walk_in_place_down
	step_end

@ ---------------------------------------------------------------------------
@ Tamsin: hints for the case, in any order.
@ ---------------------------------------------------------------------------
Thornfield_EventScript_Tamsin::
	lock
	faceplayer
	setspeaker Slice_Name_Tamsin
	goto_if_ge VAR_THORNFIELD_STATE, 3, Thornfield_EventScript_TamsinAfterPact
	goto_if_set FLAG_THORNFIELD_TRIAL_GRANTED, Thornfield_EventScript_TamsinGoWin
	goto_if_unset FLAG_FOUND_DEBT_LEDGER, Thornfield_EventScript_TamsinHintLedger
	goto_if_unset FLAG_FOUND_FORECLOSURE_WRIT, Thornfield_EventScript_TamsinHintWrit
	goto_if_unset FLAG_THORNFIELD_WITNESS, Thornfield_EventScript_TamsinHintWitness
	msgbox Thornfield_Text_TamsinHintMagistrate, MSGBOX_DEFAULT
	release
	end

Thornfield_EventScript_TamsinHintLedger::
	msgbox Thornfield_Text_TamsinHintLedger, MSGBOX_DEFAULT
	release
	end

Thornfield_EventScript_TamsinHintWrit::
	msgbox Thornfield_Text_TamsinHintWrit, MSGBOX_DEFAULT
	release
	end

Thornfield_EventScript_TamsinHintWitness::
	msgbox Thornfield_Text_TamsinHintWitness, MSGBOX_DEFAULT
	release
	end

Thornfield_EventScript_TamsinGoWin::
	msgbox Thornfield_Text_TamsinGoWin, MSGBOX_DEFAULT
	release
	end

Thornfield_EventScript_TamsinAfterPact::
	msgbox Thornfield_Text_TamsinAfterPact, MSGBOX_DEFAULT
	release
	end

@ ---------------------------------------------------------------------------
@ The Willows. At their camp on the town edge (states 14-15), and back at
@ their door after the pact (state 3 on).
@ ---------------------------------------------------------------------------
Thornfield_EventScript_CampWillow::
	npc Ch2_Name_Willow, Thornfield_Text_CampWillow
	end

Thornfield_EventScript_CampMae::
	npc Ch2_Name_Mae, Thornfield_Text_CampMae
	end

Thornfield_EventScript_CampPip::
	npc Ch2_Name_Pip, Thornfield_Text_CampPip
	end

Thornfield_EventScript_Willow::
	npc Ch2_Name_Willow, Thornfield_Text_WillowHome
	end

Thornfield_EventScript_Mae::
	lock
	faceplayer
	setspeaker Ch2_Name_Mae
	getpactterms PACT_THORNFIELD
	goto_if_eq VAR_RESULT, PACT_TERMS_REFORM, Thornfield_EventScript_MaeNoDebt
	msgbox Thornfield_Text_MaeHome, MSGBOX_DEFAULT
	release
	end

Thornfield_EventScript_MaeNoDebt::
	msgbox Thornfield_Text_MaeNoDebt, MSGBOX_DEFAULT
	release
	end

@ Pip tells the player about the flowers in the bean rows: Shaymin, blooming
@ on land that was given back.
Thornfield_EventScript_Pip::
	lock
	faceplayer
	say Ch2_Name_Pip, Thornfield_Text_PipFlowers
	closemessage
	showmonpic SPECIES_SHAYMIN, 10, 3
	playmoncry SPECIES_SHAYMIN, CRY_MODE_NORMAL
	waitmoncry
	hidemonpic
	say Ch2_Name_Pip, Thornfield_Text_PipHedgehog
	release
	end

@ ---------------------------------------------------------------------------
@ Isolde on her terraces, before the Trial is granted.
@ ---------------------------------------------------------------------------
Thornfield_EventScript_IsoldeTerrace::
	lock
	faceplayer
	goto_if_set FLAG_MET_ISOLDE, Thornfield_EventScript_IsoldeAgain
	say_g Slice_Name_Isolde, Thornfield_Text_IsoldeYouCame_M, Thornfield_Text_IsoldeYouCame_F
	narrate Thornfield_Text_ShayminAtHerFeet
	closemessage
	showmonpic SPECIES_SHAYMIN, 10, 3
	playmoncry SPECIES_SHAYMIN, CRY_MODE_NORMAL
	waitmoncry
	say Slice_Name_Isolde, Thornfield_Text_IsoldeShaymin
	hidemonpic
	narrate Thornfield_Text_YouTellIsolde
	say Slice_Name_Isolde, Thornfield_Text_IsoldePlotNineteen
	say Slice_Name_Isolde, Thornfield_Text_IsoldeReturnedToBeauty
	say_g Slice_Name_Isolde, Thornfield_Text_IsoldeLoseGracefully_M, Thornfield_Text_IsoldeLoseGracefully_F
	setflag FLAG_MET_ISOLDE
	release
	end

Thornfield_EventScript_IsoldeAgain::
	npc Slice_Name_Isolde, Thornfield_Text_IsoldeAgain
	end

Thornfield_EventScript_Terraces::
	sign Thornfield_Text_Terraces
	end

@ ---------------------------------------------------------------------------
@ Townsfolk and signs.
@ ---------------------------------------------------------------------------
Thornfield_EventScript_Woman::
	lock
	faceplayer
	setspeaker Slice_Name_Townsfolk
	goto_if_ge VAR_THORNFIELD_STATE, 3, Thornfield_EventScript_WomanAfterPact
	msgbox Thornfield_Text_WomanShaymin, MSGBOX_DEFAULT
	release
	end

Thornfield_EventScript_WomanAfterPact::
	msgbox Thornfield_Text_WomanAfterPact, MSGBOX_DEFAULT
	release
	end

Thornfield_EventScript_OldMan::
	lock
	faceplayer
	setspeaker Slice_Name_OldMan
	goto_if_ge VAR_THORNFIELD_STATE, 3, Thornfield_EventScript_OldManAfterPact
	msgbox Thornfield_Text_OldManSteward, MSGBOX_DEFAULT
	release
	end

Thornfield_EventScript_OldManAfterPact::
	@ Under clemency the town hears it as Isolde's gift.
	getpactterms PACT_THORNFIELD
	goto_if_eq VAR_RESULT, PACT_TERMS_CLEMENCY, Thornfield_EventScript_OldManClemency
	msgbox Thornfield_Text_OldManAfterPact, MSGBOX_DEFAULT
	release
	end

Thornfield_EventScript_OldManClemency::
	msgbox Thornfield_Text_OldManClemency, MSGBOX_DEFAULT
	release
	end

Thornfield_EventScript_Gardener::
	lock
	faceplayer
	setspeaker Ch2_Name_Gardener
	goto_if_ge VAR_THORNFIELD_STATE, 3, Thornfield_EventScript_GardenerAfterPact
	msgbox Thornfield_Text_GardenerSoil, MSGBOX_DEFAULT
	release
	end

Thornfield_EventScript_GardenerAfterPact::
	msgbox Thornfield_Text_GardenerAfterPact, MSGBOX_DEFAULT
	release
	end

Thornfield_EventScript_PalaceGuard::
	npc Slice_Name_Guard, Thornfield_Text_PalaceGuard
	end

@ The foreclosure notice on the boarded door is the writ, in Isolde's hand.
Thornfield_EventScript_BoardedHouse::
	goto_if_ge VAR_THORNFIELD_STATE, 3, Thornfield_EventScript_HouseReopened
	goto_if_set FLAG_FOUND_FORECLOSURE_WRIT, Thornfield_EventScript_BoardedHouseBare
	lockall
	narrate Thornfield_Text_BoardedHouse
	narrate Thornfield_Text_TakeWrit
	setspeaker SP_NAME_NONE
	giveitem ITEM_FORECLOSURE_WRIT
	setflag FLAG_FOUND_FORECLOSURE_WRIT
	releaseall
	end

Thornfield_EventScript_BoardedHouseBare::
	sign Thornfield_Text_BoardedHouseBare
	end

Thornfield_EventScript_HouseReopened::
	sign Thornfield_Text_HouseReopened
	end

@ The sealed grove behind the palace. Xerneas sleeps here (Act 2).
Thornfield_EventScript_GroveGate::
	setflag FLAG_THORNFIELD_GROVE_SEEN
	goto_if_ge VAR_THORNFIELD_STATE, 3, Thornfield_EventScript_GroveGateAfter
	sign Thornfield_Text_GroveGate
	end

Thornfield_EventScript_GroveGateAfter::
	sign Thornfield_Text_GroveGateAfter
	end

Thornfield_EventScript_TownSign::
	sign Thornfield_Text_TownSign
	end

Thornfield_EventScript_PalaceSign::
	sign Thornfield_Text_PalaceSign
	end

@ ---------------------------------------------------------------------------
@ The Steward's Office. Voss, a bailiff, and the ledger.
@ ---------------------------------------------------------------------------
Thornfield_StewardsOffice_EventScript_HideVoss::
	setflag FLAG_TEMP_1
	return

Thornfield_StewardsOffice_EventScript_Voss::
	npc Ch2_Name_Voss, Thornfield_Text_VossOffice
	end

Thornfield_StewardsOffice_EventScript_Bailiff::
	trainerbattle_single TRAINER_THORNFIELD_BAILIFF, Thornfield_Text_OfficeBailiffIntro, Thornfield_Text_OfficeBailiffDefeat
	npc Ch2_Name_Bailiff, Thornfield_Text_OfficeBailiffPostBattle
	end

@ The desk can be reached without crossing the bailiff's sight, so reading
@ it starts his battle if he hasn't been beaten.
Thornfield_StewardsOffice_EventScript_Ledger::
	lockall
	goto_if_set FLAG_FOUND_DEBT_LEDGER, Thornfield_StewardsOffice_EventScript_LedgerAgain
	goto_if_defeated TRAINER_THORNFIELD_BAILIFF, Thornfield_StewardsOffice_EventScript_ReadLedger
	say Ch2_Name_Bailiff, Thornfield_Text_OfficeBailiffIntro
	closemessage
	trainerbattle_no_intro TRAINER_THORNFIELD_BAILIFF, Thornfield_Text_OfficeBailiffDefeat
	goto Thornfield_StewardsOffice_EventScript_ReadLedger
	end

Thornfield_StewardsOffice_EventScript_ReadLedger::
	lockall
	narrate Thornfield_Text_LedgerOpen
	narrate Thornfield_Text_LedgerWillow
	narrate Thornfield_Text_LedgerFortyOne
	narrate Thornfield_Text_LedgerBell
	goto_if_ge VAR_THORNFIELD_STATE, 3, Thornfield_StewardsOffice_EventScript_TakeLedger
	applymovement LOCALID_STEWARDS_OFFICE_VOSS, Common_Movement_ExclamationMark
	waitmovement 0
	say Ch2_Name_Voss, Thornfield_Text_VossPutThatDown
	goto Thornfield_StewardsOffice_EventScript_TakeLedger
	end

Thornfield_StewardsOffice_EventScript_TakeLedger::
	setspeaker SP_NAME_NONE
	giveitem ITEM_DEBT_LEDGER
	setflag FLAG_FOUND_DEBT_LEDGER
	releaseall
	end

Thornfield_StewardsOffice_EventScript_LedgerAgain::
	sign Thornfield_Text_LedgerAgain
	end

@ ---------------------------------------------------------------------------
@ Old Mabry's cottage. Isolde's head gardener becomes the witness once the
@ player has the ledger.
@ ---------------------------------------------------------------------------
Thornfield_MabryCottage_EventScript_Mabry::
	lock
	faceplayer
	setspeaker Ch2_Name_Mabry
	goto_if_ge VAR_THORNFIELD_STATE, 3, Thornfield_MabryCottage_EventScript_MabryAfterPact
	goto_if_set FLAG_THORNFIELD_WITNESS, Thornfield_MabryCottage_EventScript_MabryWillTestify
	goto_if_unset FLAG_FOUND_DEBT_LEDGER, Thornfield_MabryCottage_EventScript_MabryRefuses
	msgbox Thornfield_Text_MabryWhatsThat, MSGBOX_DEFAULT
	narrate Thornfield_Text_ShowMabryPage
	say Ch2_Name_Mabry, Thornfield_Text_MabryFortyOne
	say Ch2_Name_Mabry, Thornfield_Text_MabryBeans
	say Ch2_Name_Mabry, Thornfield_Text_MabryNeverCame
	say Ch2_Name_Mabry, Thornfield_Text_MabryIllTestify
	setflag FLAG_THORNFIELD_WITNESS
	release
	end

Thornfield_MabryCottage_EventScript_MabryRefuses::
	msgbox Thornfield_Text_MabryRefuses, MSGBOX_DEFAULT
	release
	end

Thornfield_MabryCottage_EventScript_MabryWillTestify::
	msgbox Thornfield_Text_MabryWillTestify, MSGBOX_DEFAULT
	release
	end

Thornfield_MabryCottage_EventScript_MabryAfterPact::
	msgbox Thornfield_Text_MabryAfterPact, MSGBOX_DEFAULT
	release
	end

Thornfield_MabryCottage_EventScript_SeedShelf::
	sign Thornfield_Text_SeedShelf
	end

@ ---------------------------------------------------------------------------
@ The Magistrate's Hall. Magistrate Fenwick grants the Trial when the player
@ has the ledger, the writ and a witness (state 1 -> 2).
@ ---------------------------------------------------------------------------
Thornfield_MagistrateHall_EventScript_Fenwick::
	lock
	faceplayer
	setspeaker Ch2_Name_Fenwick
	goto_if_ge VAR_THORNFIELD_STATE, 3, Thornfield_MagistrateHall_EventScript_FenwickAfterPact
	goto_if_set FLAG_THORNFIELD_TRIAL_GRANTED, Thornfield_MagistrateHall_EventScript_FenwickGoOn
	goto_if_unset FLAG_FOUND_DEBT_LEDGER, Thornfield_MagistrateHall_EventScript_FenwickNeedProof
	goto_if_unset FLAG_FOUND_FORECLOSURE_WRIT, Thornfield_MagistrateHall_EventScript_FenwickNeedProof
	goto_if_unset FLAG_THORNFIELD_WITNESS, Thornfield_MagistrateHall_EventScript_FenwickNeedProof
	goto Thornfield_MagistrateHall_EventScript_GrantTrial
	end

Thornfield_MagistrateHall_EventScript_FenwickNeedProof::
	msgbox Thornfield_Text_FenwickComplaint, MSGBOX_DEFAULT
	setspeaker Ch2_Name_Fenwick
	msgbox Thornfield_Text_FenwickTwoThings, MSGBOX_DEFAULT
	setspeaker SP_NAME_NONE
	call_if_set FLAG_FOUND_DEBT_LEDGER, Thornfield_MagistrateHall_EventScript_HaveLedger
	call_if_set FLAG_FOUND_FORECLOSURE_WRIT, Thornfield_MagistrateHall_EventScript_HaveWrit
	call_if_set FLAG_THORNFIELD_WITNESS, Thornfield_MagistrateHall_EventScript_HaveWitness
	release
	end

Thornfield_MagistrateHall_EventScript_HaveLedger::
	msgbox Thornfield_Text_HaveLedger, MSGBOX_DEFAULT
	return

Thornfield_MagistrateHall_EventScript_HaveWrit::
	msgbox Thornfield_Text_HaveWrit, MSGBOX_DEFAULT
	return

Thornfield_MagistrateHall_EventScript_HaveWitness::
	msgbox Thornfield_Text_HaveWitness, MSGBOX_DEFAULT
	return

Thornfield_MagistrateHall_EventScript_GrantTrial::
	msgbox Thornfield_Text_FenwickReads, MSGBOX_DEFAULT
	say Ch2_Name_Fenwick, Thornfield_Text_FenwickSonnet
	say_g Ch2_Name_Fenwick, Thornfield_Text_FenwickGrant_M, Thornfield_Text_FenwickGrant_F
	closemessage
	playse SE_BANG
	waitse
	narrate Thornfield_Text_FenwickStamps
	say Ch2_Name_Fenwick, Thornfield_Text_FenwickGoGently
	setflag FLAG_THORNFIELD_TRIAL_GRANTED
	setvar VAR_THORNFIELD_STATE, 2
	release
	end

Thornfield_MagistrateHall_EventScript_FenwickGoOn::
	msgbox Thornfield_Text_FenwickGoOn, MSGBOX_DEFAULT
	release
	end

Thornfield_MagistrateHall_EventScript_FenwickAfterPact::
	msgbox Thornfield_Text_FenwickAfterPact, MSGBOX_DEFAULT
	release
	end

Thornfield_MagistrateHall_EventScript_Clerk::
	npc Slice_Name_Townsfolk, Thornfield_Text_HallClerk
	end

@ ---------------------------------------------------------------------------
@ Text
@ ---------------------------------------------------------------------------

Thornfield_Text_FirstLook::
	.string "THORNFIELD. Orchards on every slope,\n"
	.string "greenhouses shining in the sun, and\l"
	.string "flower terraces climbing the hill to a\l"
	.string "palace made of glass.\p"
	.string "It's the most beautiful town you've\n"
	.string "ever seen.\p"
	.string "Then you hear someone shouting.$"

Thornfield_Text_VossReads::
	.string "By order of Her Royal Highness\n"
	.string "PRINCESS ISOLDE, the tenancy of plot\l"
	.string "nineteen is ended for debt.\p"
	.string "Amount owing: four hundred and twelve\n"
	.string "crowns. The land returns to the Crown,\l"
	.string "today.$"

Thornfield_Text_WillowWePaid::
	.string "We paid! Every quarter for nineteen\n"
	.string "years, we paid! Ask anyone!$"

Thornfield_Text_VossInterest::
	.string "You paid the rent, MASTER WILLOW.\n"
	.string "Nobody disputes the rent.\p"
	.string "You never paid the interest on the\n"
	.string "rent. Or the interest on that. It's all in\l"
	.string "the ledger.$"

Thornfield_Text_MaeWhereWillWeGo::
	.string "Then where are we supposed to go? We've\n"
	.string "got a child!$"

Thornfield_Text_VossNotALodgingHouse::
	.string "The Crown is not a lodging house,\n"
	.string "MISTRESS WILLOW.$"

Thornfield_Text_PipBeans::
	.string "Can I take my beans? I planted them.\n"
	.string "They're nearly ready.$"

Thornfield_Text_VossBelongsToTheLand::
	.string "What grows in the ground belongs to the\n"
	.string "land, child. And the land belongs to Her\l"
	.string "Highness.$"

Thornfield_Text_LedAway::
	.string "The guards take the WILLOWS by the\n"
	.string "arms and lead them away down the road.\p"
	.string "The boy keeps looking back at the field.$"

Thornfield_Text_DoorBoarded::
	.string "When the guards come back, they nail\n"
	.string "three boards across the WILLOWS' door.$"

Thornfield_Text_Digging::
	.string "Two gardeners in palace green start\n"
	.string "digging up the bean rows. Neat squares,\l"
	.string "ready for flowerbeds.\p"
	.string "One of them is an old man. He doesn't\n"
	.string "look up once.$"

Thornfield_Text_VossSeesYou_M::
	.string "Ah. The prince from LOWMERE. Her\n"
	.string "Highness said you might pass through.$"

Thornfield_Text_VossSeesYou_F::
	.string "Ah. The princess from LOWMERE. Her\n"
	.string "Highness said you might pass through.$"

Thornfield_Text_VossInOrder::
	.string "You'll find THORNFIELD's accounts are\n"
	.string "in perfect order, Your Highness. Every\l"
	.string "page signed by PRINCESS ISOLDE\l"
	.string "herself.\p"
	.string "Do enjoy the gardens. Everyone does.$"

Thornfield_Text_TamsinThirdFamily::
	.string "{PLAYER}! I came up for seed. Toft's\n"
	.string "orders.\p"
	.string "I've been here an hour. That's the third\n"
	.string "family they've thrown out today.$"

Thornfield_Text_TamsinPlan::
	.string "If those debts are made up, the proof's\n"
	.string "in that steward's books. His office is\l"
	.string "just off the square.\p"
	.string "And you'll need someone the magistrate\n"
	.string "will listen to. The WILLOWS are camped\l"
	.string "at the edge of town. Start there.$"

Thornfield_Text_TamsinHintLedger::
	.string "The STEWARD'S OFFICE, {PLAYER}.\n"
	.string "Whatever those debts really are, VOSS\l"
	.string "wrote them down somewhere.$"

Thornfield_Text_TamsinHintWrit::
	.string "That notice they nailed on the WILLOWS'\n"
	.string "door. Isolde signed it herself, didn't\l"
	.string "she? Somebody ought to keep it safe.$"

Thornfield_Text_TamsinHintWitness::
	.string "You've got the books. Now you need\n"
	.string "someone to swear to them.\p"
	.string "The WILLOWS say the magistrate won't\n"
	.string "hear a tenant. Pip said something about\l"
	.string "an old gardener crying, though.$"

Thornfield_Text_TamsinHintMagistrate::
	.string "Books, a writ and a witness. Go and see\n"
	.string "the magistrate, before VOSS finds out\l"
	.string "what you've got.$"

Thornfield_Text_TamsinGoWin::
	.string "The GLASSHOUSE is open. Go on. I'll be\n"
	.string "here, pretending to buy seed.$"

Thornfield_Text_TamsinAfterPact::
	.string "Two down, Your Highness. Seven to go.\p"
	.string "I'm taking the seed carts back to\n"
	.string "LOWMERE. Toft's going to cry. He'll say\l"
	.string "it's the pollen.\p"
	.string "BRAM says the road north goes through\n"
	.string "the woods to CRAGHOLT. Be careful up\l"
	.string "there.$"

Thornfield_Text_CampWillow::
	.string "We'll walk to my sister's in HAYMARKET.\n"
	.string "If she'll have three more mouths.\p"
	.string "The magistrate? FENWICK dines at the\n"
	.string "palace twice a week. He won't hear a\l"
	.string "word against her.$"

Thornfield_Text_CampMae::
	.string "The steward kept the book, and we never\n"
	.string "saw it. He'd read us out a number, and\l"
	.string "we'd pay it.\p"
	.string "The number only ever went up.$"

Thornfield_Text_CampPip::
	.string "The old gardener was crying when he dug\n"
	.string "up my beans. I saw.\p"
	.string "He lives in the cottage with all the\n"
	.string "seed packets.$"

Thornfield_Text_WillowHome::
	.string "Her Highness came down to the field\n"
	.string "herself. In her good shoes.\p"
	.string "She stood in the mud for a long time and\n"
	.string "didn't say anything. Then she said\l"
	.string "sorry. I didn't know princesses could.$"

Thornfield_Text_MaeHome::
	.string "We've got the land back. The debt's still\n"
	.string "on paper somewhere, but nobody's come\l"
	.string "to collect it.\p"
	.string "We'll manage. We always have.$"

Thornfield_Text_MaeNoDebt::
	.string "They burned the ledgers in the square!\n"
	.string "Every debt in THORNFIELD, gone up in\l"
	.string "smoke.\p"
	.string "I stood there and watched the whole\n"
	.string "thing. Best fire I ever saw.$"

Thornfield_Text_PipFlowers::
	.string "Look! There are flowers in my bean rows!\n"
	.string "White ones. They weren't there\l"
	.string "yesterday.\p"
	.string "And there was something sleeping in\n"
	.string "them.$"

Thornfield_Text_PipHedgehog::
	.string "A little green hedgehog! It looked just\n"
	.string "like the one on the terraces.\p"
	.string "Everyone says it only flowers where\n"
	.string "land's been taken. But we got ours back.$"

Thornfield_Text_IsoldeYouCame_M::
	.string "Oh! You must be our little brother. From\n"
	.string "LOWMERE. You came all this way.\p"
	.string "Corwin wrote to me about you. He was\n"
	.string "very rude. I'm sure you're not as bad as\l"
	.string "he says.$"

Thornfield_Text_IsoldeYouCame_F::
	.string "Oh! You must be our little sister. From\n"
	.string "LOWMERE. You came all this way.\p"
	.string "Corwin wrote to me about you. He was\n"
	.string "very rude. I'm sure you're not as bad as\l"
	.string "he says.$"

Thornfield_Text_ShayminAtHerFeet::
	.string "A small POKéMON is curled up among the\n"
	.string "flowers at her feet.$"

Thornfield_Text_IsoldeShaymin::
	.string "This is SHAYMIN. Father gave it to me at\n"
	.string "my Apportionment. It only blooms here,\l"
	.string "on my terraces.\p"
	.string "Isn't it the loveliest thing you ever\n"
	.string "saw?$"

Thornfield_Text_YouTellIsolde::
	.string "You tell her about the WILLOWS, and\n"
	.string "plot nineteen.$"

Thornfield_Text_IsoldePlotNineteen::
	.string "Plot nineteen? I don't know the plots\n"
	.string "by number. STEWARD VOSS sees to all\l"
	.string "that. I only sign what he brings me.$"

Thornfield_Text_IsoldeReturnedToBeauty::
	.string "The land was tired, and the tenants\n"
	.string "were tired. Now look at it. I've returned\l"
	.string "it to beauty.\p"
	.string "People will remember these gardens long\n"
	.string "after they've forgotten whose beans\l"
	.string "were here.$"

Thornfield_Text_IsoldeLoseGracefully_M::
	.string "You'll want a TRIAL, I suppose. Ask the\n"
	.string "magistrate. If he allows it, I'll gladly\l"
	.string "give you an afternoon.\p"
	.string "Losing gracefully is an art, little\n"
	.string "brother. I'd be happy to teach you.$"

Thornfield_Text_IsoldeLoseGracefully_F::
	.string "You'll want a TRIAL, I suppose. Ask the\n"
	.string "magistrate. If he allows it, I'll gladly\l"
	.string "give you an afternoon.\p"
	.string "Losing gracefully is an art, little\n"
	.string "sister. I'd be happy to teach you.$"

Thornfield_Text_IsoldeAgain::
	.string "Do stay for my reading this evening.\n"
	.string "I've written one about LOWMERE. It's\l"
	.string "mostly about the fog.$"

Thornfield_Text_Terraces::
	.string "The terraces climb the hill in neat\n"
	.string "green steps. Here and there, the old\l"
	.string "field walls still show through the\l"
	.string "flowerbeds.$"

Thornfield_Text_WomanShaymin::
	.string "Have you seen Her Highness's SHAYMIN?\n"
	.string "It flowers all over the terraces.\p"
	.string "My gran says it only blooms where land\n"
	.string "has been taken. Don't tell anyone I said\l"
	.string "that.$"

Thornfield_Text_WomanAfterPact::
	.string "The terraces are coming down, one plot\n"
	.string "at a time. They're giving the land back!\p"
	.string "Funny thing is, the flowers keep coming\n"
	.string "up in the bean fields too.$"

Thornfield_Text_OldManSteward::
	.string "VOSS came two years ago. The GILDED\n"
	.string "SCALE sent him, as a favour to Her\l"
	.string "Highness.\p"
	.string "Before that, the old steward let you\n"
	.string "pay late when the harvest was bad. VOSS\l"
	.string "doesn't believe in bad harvests.$"

Thornfield_Text_OldManAfterPact::
	.string "VOSS is gone, and the land's going back.\n"
	.string "They're saying the heir from LOWMERE\l"
	.string "made her do it.\p"
	.string "I never thought I'd see anybody make a\n"
	.string "princess do anything.$"

Thornfield_Text_OldManClemency::
	.string "Her Highness is giving the land back! A\n"
	.string "gift, she called it. In her own name.\p"
	.string "Generous of her. To give back what she\n"
	.string "took.$"

Thornfield_Text_GardenerSoil::
	.string "Her Highness pays us well. The best\n"
	.string "wages in the kingdom.\p"
	.string "I try not to think too much about where\n"
	.string "the soil came from.$"

Thornfield_Text_GardenerAfterPact::
	.string "We're digging up the flowerbeds and\n"
	.string "putting the fields back. Same spades,\l"
	.string "other direction.\p"
	.string "Half of us are going to LOWMERE, with\n"
	.string "the seed. I hear there's a lot of work\l"
	.string "there.$"

Thornfield_Text_PalaceGuard::
	.string "The GLASSHOUSE is closed to\n"
	.string "petitioners. If you want a TRIAL, bring\l"
	.string "a writ from the magistrate.$"

Thornfield_Text_BoardedHouse::
	.string "The door is boarded shut. A notice is\n"
	.string "nailed over it:\p"
	.string "FORECLOSED. PLOT 19, WILLOW. BY ORDER\n"
	.string "OF H.R.H. PRINCESS ISOLDE. And below, in\l"
	.string "green ink, her signature.$"

Thornfield_Text_TakeWrit::
	.string "You pull the notice off the nail.$"

Thornfield_Text_BoardedHouseBare::
	.string "The door is boarded shut. There's a pale\n"
	.string "square on the wood where the notice\l"
	.string "was.$"

Thornfield_Text_HouseReopened::
	.string "The boards are gone. Somebody has\n"
	.string "chalked on the door: WILLOW. PLOT 19.\l"
	.string "HOME.$"

Thornfield_Text_GroveGate::
	.string "A gate of woven thorn, taller than a\n"
	.string "house. Behind it, the trees are older\l"
	.string "than the town.\p"
	.string "A notice on the gate: SEALED BY ROYAL\n"
	.string "ORDER. NO ENTRY.$"

Thornfield_Text_GroveGateAfter::
	.string "A gate of woven thorn. Behind it, the\n"
	.string "trees are older than the town.\p"
	.string "ISOLDE's own seal now hangs on the\n"
	.string "gate, beside one much older.$"

Thornfield_Text_TownSign::
	.string "THORNFIELD\p"
	.string "The Garden of the Realm$"

Thornfield_Text_PalaceSign::
	.string "THE GLASSHOUSE\p"
	.string "Court of Her Royal Highness PRINCESS\n"
	.string "ISOLDE$"

Thornfield_Text_VossOffice::
	.string "Can I help you, Your Highness? No?\p"
	.string "Then do mind the ledgers. They're very\n"
	.string "old, and very private.$"

Thornfield_Text_OfficeBailiffIntro::
	.string "Nobody touches the steward's books!\n"
	.string "GILDED SCALE rules.$"

Thornfield_Text_OfficeBailiffDefeat::
	.string "All right, all right! It's only paper!$"

Thornfield_Text_OfficeBailiffPostBattle::
	.string "I just stand here. I don't read them. I'm\n"
	.string "not even sure I can.$"

Thornfield_Text_LedgerOpen::
	.string "A heavy ledger lies open on the desk.\n"
	.string "Every page is a family.$"

Thornfield_Text_LedgerWillow::
	.string "Plot nineteen, WILLOW. Rent paid every\n"
	.string "quarter for nineteen years.\p"
	.string "Interest on rent: 400 crowns. Interest\n"
	.string "on interest: 12 crowns. Never paid.\l"
	.string "Never asked for.\p"
	.string "FORECLOSED. Signed in green ink:\n"
	.string "ISOLDE.$"

Thornfield_Text_LedgerFortyOne::
	.string "You turn the pages. Forty-one plots, all\n"
	.string "foreclosed in the last two years. Every\l"
	.string "one is signed ISOLDE, in the same green\l"
	.string "ink.$"

Thornfield_Text_LedgerBell::
	.string "On the inside of the cover, stamped\n"
	.string "very small, is a pale bell.$"

Thornfield_Text_VossPutThatDown::
	.string "Put that down! That's...\p"
	.string "Oh, take it, Your Highness. It's all\n"
	.string "perfectly legal. Her Highness signed\l"
	.string "every page.$"

Thornfield_Text_LedgerAgain::
	.string "The desk is bare, except for a pot of\n"
	.string "green ink.$"

Thornfield_Text_MabryRefuses::
	.string "I'm a gardener, Your Highness. I plant\n"
	.string "what I'm told, where I'm told.\p"
	.string "Leave an old man to his seeds.$"

Thornfield_Text_MabryWhatsThat::
	.string "What's that you've got there? Not the\n"
	.string "steward's book...$"

Thornfield_Text_ShowMabryPage::
	.string "You show OLD MABRY the steward's\n"
	.string "ledger, open at plot nineteen.$"

Thornfield_Text_MabryFortyOne::
	.string "Forty-one. I didn't count them. I didn't\n"
	.string "let myself.\p"
	.string "I dug every one of those fields under,\n"
	.string "Your Highness. I made the terraces out\l"
	.string "of them.$"

Thornfield_Text_MabryBeans::
	.string "The WILLOWS' beans were up to my knee.\n"
	.string "Good beans. I dug them under this\l"
	.string "morning, while the boy watched.$"

Thornfield_Text_MabryNeverCame::
	.string "Her Highness has never once come down\n"
	.string "to the fields. She sees the flowers from\l"
	.string "the palace windows, and she thinks\l"
	.string "that's all there is.$"

Thornfield_Text_MabryIllTestify::
	.string "I'll tell the magistrate. A tenant's word\n"
	.string "is worth nothing in that hall.\p"
	.string "The head gardener of the GLASSHOUSE,\n"
	.string "though. That might be worth something.$"

Thornfield_Text_MabryWillTestify::
	.string "I'll be at the magistrate's when you\n"
	.string "need me. I've put my good coat on. I'm\l"
	.string "not taking it off.$"

Thornfield_Text_MabryAfterPact::
	.string "I'm putting the field walls back up,\n"
	.string "stone by stone. My back hates you.\p"
	.string "The rest of me doesn't.$"

Thornfield_Text_SeedShelf::
	.string "Shelves of seed packets, every one\n"
	.string "labelled with a family name. WILLOW.\l"
	.string "HARROW. FENN. COBB.\p"
	.string "Someone has been keeping their seed\n"
	.string "safe.$"

Thornfield_Text_FenwickComplaint::
	.string "A complaint against Her Highness?\n"
	.string "Against PRINCESS ISOLDE?\p"
	.string "Oh dear. Oh dear, oh dear.$"

Thornfield_Text_FenwickTwoThings::
	.string "If I'm to hear it, the law asks three\n"
	.string "things. The steward's own accounts. An\l"
	.string "order in Her Highness's own hand. And a\l"
	.string "witness this court can hear.\p"
	.string "The tenants themselves, I'm afraid,\n"
	.string "don't count. A tenant's word against\l"
	.string "the Crown is worth one tenth of a word.\l"
	.string "I didn't write the law.$"

Thornfield_Text_HaveLedger::
	.string "You have the steward's DEBT LEDGER.$"

Thornfield_Text_HaveWrit::
	.string "You have the FORECLOSURE WRIT from\n"
	.string "the WILLOWS' door.$"

Thornfield_Text_HaveWitness::
	.string "OLD MABRY has agreed to testify.$"

Thornfield_Text_FenwickReads::
	.string "The ledger. Her own signature on the\n"
	.string "writ. And MABRY, of all people. Let me\l"
	.string "see...$"

Thornfield_Text_FenwickSonnet::
	.string "I wrote her a sonnet last spring, you\n"
	.string "know. About the terraces. “Where once\l"
	.string "was toil, there now is only bloom.”\p"
	.string "I thought it was a good line. I see now\n"
	.string "what the toil was.$"

Thornfield_Text_FenwickGrant_M::
	.string "Very well. Under the old law of the\n"
	.string "realm, I grant the prince of LOWMERE a\l"
	.string "TRIAL OF STANDING against PRINCESS\l"
	.string "ISOLDE.$"

Thornfield_Text_FenwickGrant_F::
	.string "Very well. Under the old law of the\n"
	.string "realm, I grant the princess of LOWMERE\l"
	.string "a TRIAL OF STANDING against PRINCESS\l"
	.string "ISOLDE.$"

Thornfield_Text_FenwickStamps::
	.string "FENWICK stamps the writ, and then sits\n"
	.string "looking at it for a long time.$"

Thornfield_Text_FenwickGoGently::
	.string "Go to the GLASSHOUSE. And go gently,\n"
	.string "Your Highness. I don't think she knows.\l"
	.string "I think she made very sure not to know.$"

Thornfield_Text_FenwickGoOn::
	.string "The writ is granted. The GLASSHOUSE is\n"
	.string "at the top of the terraces.$"

Thornfield_Text_FenwickAfterPact::
	.string "I've written a new poem. It's about mud.\n"
	.string "Her Highness says it's the best thing\l"
	.string "I've ever written.$"

Thornfield_Text_HallClerk::
	.string "The magistrate hasn't heard a case\n"
	.string "against the palace in eleven years. He's\l"
	.string "had to look up how.$"

