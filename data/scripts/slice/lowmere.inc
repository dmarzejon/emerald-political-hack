@ Slice scenes 2-5 and 12: Lowmere, the Old Lodge, the Ranger's Shed,
@ Mother Hesk's house, the first rival battle, the king's gift and the return.
@ Object, trigger and sign placement: docs/dialogue/events.md#lowmere

@ ---------------------------------------------------------------------------
@ Arrival (VAR_SLICE_STATE 1 -> 2). Runs on frame when the player steps off
@ the cart. Bram stands next to the cart.
@ ---------------------------------------------------------------------------
Lowmere_EventScript_Arrival::
	lockall
	applymovement LOCALID_LOWMERE_BRAM, Common_Movement_FacePlayer
	waitmovement 0
	say Slice_Name_Bram, Lowmere_Text_BramHereWeAre
	narrate Lowmere_Text_FirstLook
	say Slice_Name_Bram, Lowmere_Text_BramDontMakeThatFace
	closemessage
	@ Bram heads off to his shed, so he isn't in two places at once.
	applymovement LOCALID_LOWMERE_BRAM, Lowmere_Movement_BramLeavesCart
	waitmovement 0
	removeobject LOCALID_LOWMERE_BRAM
	setflag FLAG_HIDE_LOWMERE_ARRIVAL_BRAM
	setvar VAR_SLICE_STATE, 2
	releaseall
	end

Lowmere_Movement_BramLeavesCart:
	walk_right
	walk_right
	walk_right
	walk_right
	step_end

@ Bram by the cart, if talked to before the scene clears him away.
Lowmere_EventScript_BramAtCart::
	npc Slice_Name_Bram, Lowmere_Text_BramSeeToCart
	end

@ ---------------------------------------------------------------------------
@ The Old Lodge. Reeve Toft's ledger (state 2 -> 3), on frame on entry.
@ ---------------------------------------------------------------------------
Lowmere_OldLodge_EventScript_ToftLedger::
	lockall
	applymovement LOCALID_OLD_LODGE_TOFT, Common_Movement_ExclamationMark
	waitmovement 0
	applymovement LOCALID_OLD_LODGE_TOFT, Common_Movement_FacePlayer
	waitmovement 0
	say Slice_Name_Toft, Lowmere_Text_ToftWelcome
	narrate Lowmere_Text_ToftOpensLedger
	say Slice_Name_Toft, Lowmere_Text_ToftElevenDays
	say Slice_Name_Toft, Lowmere_Text_ToftLastGovernor
	setspeaker Slice_Name_Toft
	msgbox Lowmere_Text_ToftWillYouStay, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, Lowmere_OldLodge_EventScript_ToftNotLeaving
	say Slice_Name_Toft, Lowmere_Text_ToftThankYou
	goto Lowmere_OldLodge_EventScript_ToftGoal
	end

Lowmere_OldLodge_EventScript_ToftNotLeaving::
	say Slice_Name_Toft, Lowmere_Text_ToftYouHaventMoved
	goto Lowmere_OldLodge_EventScript_ToftGoal
	end

Lowmere_OldLodge_EventScript_ToftGoal::
	say Slice_Name_Toft, Lowmere_Text_ToftGetLowmereFed
	say Slice_Name_Toft, Lowmere_Text_ToftGoSeeBram
	closemessage
	setvar VAR_SLICE_STATE, 3
	releaseall
	end

@ Reeve Toft, talked to. Lines follow the story.
Lowmere_EventScript_Toft::
	lock
	faceplayer
	setspeaker Slice_Name_Toft
	goto_if_ge VAR_LOWMERE_STAGE, 1, Lowmere_EventScript_ToftStage1
	goto_if_ge VAR_SLICE_STATE, 12, Lowmere_EventScript_ToftPactSigned
	goto_if_ge VAR_SLICE_STATE, 7, Lowmere_EventScript_ToftHaymarket
	goto_if_eq VAR_SLICE_STATE, 6, Lowmere_EventScript_ToftCourier
	goto_if_ge VAR_SLICE_STATE, 4, Lowmere_EventScript_ToftDaysLeft
	msgbox Lowmere_Text_ToftGoSeeBramAgain, MSGBOX_DEFAULT
	release
	end

Lowmere_EventScript_ToftDaysLeft::
	msgbox Lowmere_Text_ToftDaysLeft, MSGBOX_DEFAULT
	release
	end

Lowmere_EventScript_ToftCourier::
	msgbox Lowmere_Text_ToftCourier, MSGBOX_DEFAULT
	release
	end

Lowmere_EventScript_ToftHaymarket::
	msgbox Lowmere_Text_ToftHaymarket, MSGBOX_DEFAULT
	release
	end

Lowmere_EventScript_ToftPactSigned::
	msgbox Lowmere_Text_ToftPactSigned, MSGBOX_DEFAULT
	release
	end

Lowmere_EventScript_ToftStage1::
	msgbox Lowmere_Text_ToftStage1, MSGBOX_DEFAULT
	release
	end

@ ---------------------------------------------------------------------------
@ The Ranger's Shed. Bram gives the starter (state 3 -> 4) and heals the
@ party afterwards while Lowmere has no Pokémon Center.
@ ---------------------------------------------------------------------------
Lowmere_RangersShed_EventScript_Bram::
	lock
	faceplayer
	goto_if_ge VAR_SLICE_STATE, 4, Lowmere_EventScript_BramHeal
	goto_if_eq VAR_SLICE_STATE, 3, Lowmere_RangersShed_EventScript_ChooseStarter
	npc Slice_Name_Bram, Lowmere_Text_BramNotYet
	end

Lowmere_RangersShed_EventScript_ChooseStarter::
	say Slice_Name_Bram, Lowmere_Text_BramSitDown
	say Slice_Name_Bram, Lowmere_Text_BramOldHabit
	say Slice_Name_Bram, Lowmere_Text_BramHaveALook
	closemessage
	setflag FLAG_SYS_POKEMON_GET
	goto Lowmere_RangersShed_EventScript_PickStarter
	end

@ Bram's three mended POKéMON. A script menu instead of special ChooseStarter,
@ which brings Birch's text and the wild Zigzagoon battle with it.
@ VAR_STARTER_MON keeps the vanilla order (0 grass, 1 fire, 2 water) for
@ Tamsin's team.
Lowmere_RangersShed_EventScript_PickStarter::
	setspeaker SP_NAME_NONE
	message Lowmere_Text_WhichStarter
	waitmessage
	dynmultichoice 0, 0, TRUE, 3, 0, DYN_MULTICHOICE_CB_NONE, Lowmere_Text_Treecko, Lowmere_Text_Torchic, Lowmere_Text_Mudkip
	switch VAR_RESULT
	case 0, Lowmere_RangersShed_EventScript_LookTreecko
	case 1, Lowmere_RangersShed_EventScript_LookTorchic
	case 2, Lowmere_RangersShed_EventScript_LookMudkip
	end

Lowmere_RangersShed_EventScript_LookTreecko::
	setvar VAR_STARTER_MON, 0
	showmonpic SPECIES_TREECKO, 10, 3
	playmoncry SPECIES_TREECKO, CRY_MODE_NORMAL
	setspeaker SP_NAME_NONE
	msgbox Lowmere_Text_AboutTreecko, MSGBOX_YESNO
	goto Lowmere_RangersShed_EventScript_ConfirmStarter
	end

Lowmere_RangersShed_EventScript_LookTorchic::
	setvar VAR_STARTER_MON, 1
	showmonpic SPECIES_TORCHIC, 10, 3
	playmoncry SPECIES_TORCHIC, CRY_MODE_NORMAL
	setspeaker SP_NAME_NONE
	msgbox Lowmere_Text_AboutTorchic, MSGBOX_YESNO
	goto Lowmere_RangersShed_EventScript_ConfirmStarter
	end

Lowmere_RangersShed_EventScript_LookMudkip::
	setvar VAR_STARTER_MON, 2
	showmonpic SPECIES_MUDKIP, 10, 3
	playmoncry SPECIES_MUDKIP, CRY_MODE_NORMAL
	setspeaker SP_NAME_NONE
	msgbox Lowmere_Text_AboutMudkip, MSGBOX_YESNO
	goto Lowmere_RangersShed_EventScript_ConfirmStarter
	end

Lowmere_RangersShed_EventScript_ConfirmStarter::
	waitmoncry
	hidemonpic
	goto_if_eq VAR_RESULT, NO, Lowmere_RangersShed_EventScript_PickStarter
	switch VAR_STARTER_MON
	case 0, Lowmere_RangersShed_EventScript_GiveTreecko
	case 1, Lowmere_RangersShed_EventScript_GiveTorchic
	case 2, Lowmere_RangersShed_EventScript_GiveMudkip
	end

Lowmere_RangersShed_EventScript_GiveTreecko::
	givemon SPECIES_TREECKO, 5
	goto Lowmere_RangersShed_EventScript_ReceivedStarter
	end

Lowmere_RangersShed_EventScript_GiveTorchic::
	givemon SPECIES_TORCHIC, 5
	goto Lowmere_RangersShed_EventScript_ReceivedStarter
	end

Lowmere_RangersShed_EventScript_GiveMudkip::
	givemon SPECIES_MUDKIP, 5
	goto Lowmere_RangersShed_EventScript_ReceivedStarter
	end

Lowmere_RangersShed_EventScript_ReceivedStarter::
	bufferleadmonspeciesname STR_VAR_1
	setspeaker SP_NAME_NONE
	message Lowmere_Text_ReceivedStarter
	waitmessage
	playfanfare MUS_OBTAIN_ITEM
	waitfanfare
	msgbox gText_NicknameThisPokemon, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, Lowmere_RangersShed_EventScript_AfterStarter
	setvar VAR_0x8004, 0
	call Common_EventScript_NameReceivedPartyMon
	goto Lowmere_RangersShed_EventScript_AfterStarter
	end

Lowmere_RangersShed_EventScript_AfterStarter::
	say Slice_Name_Bram, Lowmere_Text_BramYourMother
	say Slice_Name_Bram, Lowmere_Text_BramRegister
	setspeaker SP_NAME_NONE
	playfanfare MUS_OBTAIN_ITEM
	message Lowmere_Text_ReceivedPokedex
	waitfanfare
	setflag FLAG_SYS_POKEDEX_GET
	special SetUnlockedPokedexFlags
	setflag FLAG_RECEIVED_POKEDEX_FROM_BIRCH
	setflag FLAG_ADVENTURE_STARTED
	say Slice_Name_Bram, Lowmere_Text_BramBoots
	setspeaker SP_NAME_NONE
	playfanfare MUS_OBTAIN_ITEM
	message Lowmere_Text_ReceivedBoots
	waitfanfare
	setflag FLAG_SYS_B_DASH
	setflag FLAG_RECEIVED_RUNNING_SHOES
	narrate Lowmere_Text_BootsExplain
	say Slice_Name_Bram, Lowmere_Text_BramBalls
	giveitem ITEM_POKE_BALL, 5
	say Slice_Name_Bram, Lowmere_Text_BramGoSeeHesk
	setvar VAR_SLICE_STATE, 4
	release
	end

@ After a whiteout the player wakes in the Old Lodge (replaces vanilla Mom's
@ EventScript_AfterWhiteOutMomHeal). Text only, so it works whoever is home.
Lowmere_OldLodge_EventScript_WhiteOutHeal::
	lockall
	say Slice_Name_Toft, Lowmere_Text_ToftWhiteOut
	call Common_EventScript_OutOfCenterPartyHeal
	say Slice_Name_Toft, Lowmere_Text_ToftWhiteOutRest
	fadedefaultbgm
	releaseall
	end

@ Bram anywhere after the starter: heals the party, then a line that follows
@ the story. Used in the Ranger's Shed and the Old Lodge.
Lowmere_EventScript_BramHeal::
	say Slice_Name_Bram, Lowmere_Text_BramLetMeLook
	closemessage
	call Common_EventScript_OutOfCenterPartyHeal
	setspeaker Slice_Name_Bram
	goto_if_ge VAR_SLICE_STATE, 13, Lowmere_EventScript_BramThornfield
	goto_if_ge VAR_SLICE_STATE, 7, Lowmere_EventScript_BramMireRoad
	goto_if_eq VAR_SLICE_STATE, 6, Lowmere_EventScript_BramCourier
	goto_if_eq VAR_SLICE_STATE, 5, Lowmere_EventScript_BramTamsinWaiting
	msgbox Lowmere_Text_BramRightAsRain, MSGBOX_DEFAULT
	release
	end

Lowmere_EventScript_BramTamsinWaiting::
	msgbox Lowmere_Text_BramTamsinWaiting, MSGBOX_DEFAULT
	release
	end

Lowmere_EventScript_BramCourier::
	msgbox Lowmere_Text_BramCourier, MSGBOX_DEFAULT
	release
	end

Lowmere_EventScript_BramMireRoad::
	msgbox Lowmere_Text_BramMireRoad, MSGBOX_DEFAULT
	release
	end

Lowmere_EventScript_BramThornfield::
	msgbox Lowmere_Text_BramThornfield, MSGBOX_DEFAULT
	release
	end

@ ---------------------------------------------------------------------------
@ Mother Hesk, in Elena's old house. The Silver Wing (state 4 -> 5).
@ ---------------------------------------------------------------------------
Lowmere_HeskHouse_EventScript_Hesk::
	lock
	faceplayer
	setspeaker Slice_Name_Hesk
	goto_if_ge VAR_LOWMERE_STAGE, 1, Lowmere_HeskHouse_EventScript_HeskStage1
	goto_if_set FLAG_RECEIVED_SILVER_WING, Lowmere_HeskHouse_EventScript_HeskDreams
	goto_if_eq VAR_SLICE_STATE, 4, Lowmere_HeskHouse_EventScript_GiveSilverWing
	msgbox Lowmere_Text_HeskSeeBramFirst, MSGBOX_DEFAULT
	release
	end

Lowmere_HeskHouse_EventScript_GiveSilverWing::
	msgbox Lowmere_Text_HeskLetMeLook, MSGBOX_DEFAULT
	say Slice_Name_Hesk, Lowmere_Text_HeskHerHouse
	say Slice_Name_Hesk, Lowmere_Text_HeskOneDay
	setspeaker SP_NAME_NONE
	giveitem ITEM_SILVER_WING
	setflag FLAG_RECEIVED_SILVER_WING
	say Slice_Name_Hesk, Lowmere_Text_HeskNeverCold
	say Slice_Name_Hesk, Lowmere_Text_HeskBellDream
	say Slice_Name_Hesk, Lowmere_Text_HeskTamsinPacing
	setvar VAR_SLICE_STATE, 5
	setflag FLAG_HIDE_LOWMERE_TAMSIN_FORGE
	clearflag FLAG_HIDE_LOWMERE_TAMSIN_ROAD
	release
	end

Lowmere_HeskHouse_EventScript_HeskDreams::
	msgbox Lowmere_Text_HeskDreams, MSGBOX_DEFAULT
	release
	end

Lowmere_HeskHouse_EventScript_HeskStage1::
	msgbox Lowmere_Text_HeskDreamsWorse, MSGBOX_DEFAULT
	release
	end

Lowmere_HeskHouse_EventScript_ElenasBed::
	sign Lowmere_Text_ElenasBed
	end

@ ---------------------------------------------------------------------------
@ Tamsin. At the forge before the Silver Wing; on the north road for the first
@ rival battle (state 5 -> 6); back at the forge after the slice.
@ ---------------------------------------------------------------------------
Lowmere_EventScript_TamsinForge::
	lock
	faceplayer
	setspeaker Slice_Name_Tamsin
	goto_if_ge VAR_LOWMERE_STAGE, 1, Lowmere_EventScript_TamsinStage1
	goto_if_ge VAR_SLICE_STATE, 6, Lowmere_EventScript_TamsinAfterBattle
	msgbox Lowmere_Text_TamsinForge, MSGBOX_DEFAULT
	release
	end

Lowmere_EventScript_TamsinAfterBattle::
	msgbox Lowmere_Text_TamsinGoSeeCourier, MSGBOX_DEFAULT
	release
	end

Lowmere_EventScript_TamsinStage1::
	msgbox Lowmere_Text_TamsinOneDown, MSGBOX_DEFAULT
	checkplayergender
	goto_if_eq VAR_RESULT, FEMALE, Lowmere_EventScript_TamsinStage1Female
	msgbox Lowmere_Text_TamsinIron_M, MSGBOX_DEFAULT
	release
	end

Lowmere_EventScript_TamsinStage1Female::
	msgbox Lowmere_Text_TamsinIron_F, MSGBOX_DEFAULT
	release
	end

@ Tamsin on the north road: talked to, or her coord trigger on the road.
Lowmere_EventScript_TamsinRoad::
	lockall
	applymovement LOCALID_LOWMERE_TAMSIN_ROAD, Common_Movement_ExclamationMark
	waitmovement 0
	applymovement LOCALID_LOWMERE_TAMSIN_ROAD, Common_Movement_FacePlayer
	waitmovement 0
	say Slice_Name_Tamsin, Lowmere_Text_TamsinTookYourTime
	say_g Slice_Name_Tamsin, Lowmere_Text_TamsinWorthFollowing_M, Lowmere_Text_TamsinWorthFollowing_F
	closemessage
	switch VAR_STARTER_MON
	case 0, Lowmere_EventScript_BattleTamsin0
	case 1, Lowmere_EventScript_BattleTamsin1
	case 2, Lowmere_EventScript_BattleTamsin2
	end

Lowmere_EventScript_BattleTamsin0::
	trainerbattle_no_intro TRAINER_TAMSIN_LOWMERE_0, Lowmere_Text_TamsinDefeated
	goto Lowmere_EventScript_AfterTamsinBattle
	end

Lowmere_EventScript_BattleTamsin1::
	trainerbattle_no_intro TRAINER_TAMSIN_LOWMERE_1, Lowmere_Text_TamsinDefeated
	goto Lowmere_EventScript_AfterTamsinBattle
	end

Lowmere_EventScript_BattleTamsin2::
	trainerbattle_no_intro TRAINER_TAMSIN_LOWMERE_2, Lowmere_Text_TamsinDefeated
	goto Lowmere_EventScript_AfterTamsinBattle
	end

Lowmere_EventScript_AfterTamsinBattle::
	special HealPlayerParty
	say Slice_Name_Tamsin, Lowmere_Text_TamsinNotUseless
	say Slice_Name_Tamsin, Lowmere_Text_TamsinManAtLodge
	closemessage
	fadescreen FADE_TO_BLACK
	removeobject LOCALID_LOWMERE_TAMSIN_ROAD
	setflag FLAG_HIDE_LOWMERE_TAMSIN_ROAD
	clearflag FLAG_HIDE_LOWMERE_TAMSIN_FORGE
	clearflag FLAG_HIDE_OLD_LODGE_COURIER
	clearflag FLAG_HIDE_OLD_LODGE_BRAM
	setvar VAR_SLICE_STATE, 6
	fadescreen FADE_FROM_BLACK
	releaseall
	end

@ Coord trigger across the north exit to the Mire Road. Turns the player back
@ until the king's gift is chosen. Assumes the exit is at the top of the map.
Lowmere_EventScript_NorthRoadBlock::
	lockall
	goto_if_ge VAR_SLICE_STATE, 7, Lowmere_EventScript_NorthRoadOpen
	setspeaker SP_NAME_NONE
	goto_if_eq VAR_SLICE_STATE, 6, Lowmere_EventScript_NorthRoadCourier
	goto_if_eq VAR_SLICE_STATE, 4, Lowmere_EventScript_NorthRoadHesk
	goto_if_ge VAR_SLICE_STATE, 3, Lowmere_EventScript_NorthRoadNoMon
	msgbox Lowmere_Text_NorthRoadSeeToft, MSGBOX_DEFAULT
	goto Lowmere_EventScript_NorthRoadTurnBack
	end

Lowmere_EventScript_NorthRoadNoMon::
	msgbox Lowmere_Text_NorthRoadNoMon, MSGBOX_DEFAULT
	goto Lowmere_EventScript_NorthRoadTurnBack
	end

Lowmere_EventScript_NorthRoadHesk::
	msgbox Lowmere_Text_NorthRoadHesk, MSGBOX_DEFAULT
	goto Lowmere_EventScript_NorthRoadTurnBack
	end

Lowmere_EventScript_NorthRoadCourier::
	msgbox Lowmere_Text_NorthRoadCourier, MSGBOX_DEFAULT
	goto Lowmere_EventScript_NorthRoadTurnBack
	end

Lowmere_EventScript_NorthRoadTurnBack::
	closemessage
	applymovement LOCALID_PLAYER, Common_Movement_WalkDown
	waitmovement 0
	releaseall
	end

Lowmere_EventScript_NorthRoadOpen::
	releaseall
	end

@ ---------------------------------------------------------------------------
@ The king's gift (state 6 -> 7). On frame when the player enters the Old
@ Lodge after beating Tamsin. The courier and Bram are inside.
@ ---------------------------------------------------------------------------
Lowmere_OldLodge_EventScript_KingsGift::
	lockall
	applymovement LOCALID_OLD_LODGE_COURIER, Common_Movement_ExclamationMark
	waitmovement 0
	applymovement LOCALID_OLD_LODGE_COURIER, Common_Movement_FacePlayer
	waitmovement 0
	say Slice_Name_Courier, Lowmere_Text_CourierThankFounders
	say Slice_Name_Courier, Lowmere_Text_CourierFromThePalace
	narrate Lowmere_Text_LetterAndBalls
	say Slice_Name_Courier, Lowmere_Text_CourierStoppedAtGate
	say Slice_Name_Courier, Lowmere_Text_CourierFrightened
	narrate Lowmere_Text_BreakTheSeal
	checkplayergender
	call_if_eq VAR_RESULT, MALE, Lowmere_OldLodge_EventScript_LetterOpeningMale
	call_if_eq VAR_RESULT, FEMALE, Lowmere_OldLodge_EventScript_LetterOpeningFemale
	narrate Lowmere_Text_Letter
	say Slice_Name_Courier, Lowmere_Text_CourierWhichOne
	goto Lowmere_OldLodge_EventScript_ChooseDog
	end

Lowmere_OldLodge_EventScript_LetterOpeningMale::
	narrate Lowmere_Text_LetterOpening_M
	return

Lowmere_OldLodge_EventScript_LetterOpeningFemale::
	narrate Lowmere_Text_LetterOpening_F
	return

Lowmere_OldLodge_EventScript_ChooseDog::
	setspeaker SP_NAME_NONE
	message Lowmere_Text_ChooseWhichOne
	waitmessage
	dynmultichoice 0, 0, TRUE, 3, 0, DYN_MULTICHOICE_CB_NONE, Lowmere_Text_Entei, Lowmere_Text_Raikou, Lowmere_Text_Suicune
	switch VAR_RESULT
	case 0, Lowmere_OldLodge_EventScript_LookEntei
	case 1, Lowmere_OldLodge_EventScript_LookRaikou
	case 2, Lowmere_OldLodge_EventScript_LookSuicune
	end

Lowmere_OldLodge_EventScript_LookEntei::
	setvar VAR_KINGS_GIFT_DOG, 0
	showmonpic SPECIES_ENTEI, 10, 3
	playmoncry SPECIES_ENTEI, CRY_MODE_NORMAL
	setspeaker SP_NAME_NONE
	msgbox Lowmere_Text_AboutEntei, MSGBOX_YESNO
	goto Lowmere_OldLodge_EventScript_ConfirmDog
	end

Lowmere_OldLodge_EventScript_LookRaikou::
	setvar VAR_KINGS_GIFT_DOG, 1
	showmonpic SPECIES_RAIKOU, 10, 3
	playmoncry SPECIES_RAIKOU, CRY_MODE_NORMAL
	setspeaker SP_NAME_NONE
	msgbox Lowmere_Text_AboutRaikou, MSGBOX_YESNO
	goto Lowmere_OldLodge_EventScript_ConfirmDog
	end

Lowmere_OldLodge_EventScript_LookSuicune::
	setvar VAR_KINGS_GIFT_DOG, 2
	showmonpic SPECIES_SUICUNE, 10, 3
	playmoncry SPECIES_SUICUNE, CRY_MODE_NORMAL
	setspeaker SP_NAME_NONE
	msgbox Lowmere_Text_AboutSuicune, MSGBOX_YESNO
	goto Lowmere_OldLodge_EventScript_ConfirmDog
	end

Lowmere_OldLodge_EventScript_ConfirmDog::
	waitmoncry
	hidemonpic
	goto_if_eq VAR_RESULT, NO, Lowmere_OldLodge_EventScript_ChooseDog
	switch VAR_KINGS_GIFT_DOG
	case 0, Lowmere_OldLodge_EventScript_GiveEntei
	case 1, Lowmere_OldLodge_EventScript_GiveRaikou
	case 2, Lowmere_OldLodge_EventScript_GiveSuicune
	end

Lowmere_OldLodge_EventScript_GiveEntei::
	givemon SPECIES_ENTEI, KINGS_GIFT_LEVEL, move1=MOVE_EMBER, move2=MOVE_LEER, move3=MOVE_SMOKESCREEN
	goto Lowmere_OldLodge_EventScript_ReceivedDog
	end

Lowmere_OldLodge_EventScript_GiveRaikou::
	givemon SPECIES_RAIKOU, KINGS_GIFT_LEVEL, move1=MOVE_THUNDER_SHOCK, move2=MOVE_LEER, move3=MOVE_QUICK_ATTACK
	goto Lowmere_OldLodge_EventScript_ReceivedDog
	end

Lowmere_OldLodge_EventScript_GiveSuicune::
	givemon SPECIES_SUICUNE, KINGS_GIFT_LEVEL, move1=MOVE_WATER_GUN, move2=MOVE_LEER, move3=MOVE_MIST
	goto Lowmere_OldLodge_EventScript_ReceivedDog
	end

@ Moves set by the balance thread so the dogs arrive with starter-level moves.
@ The party has at most two Pokémon at this point, so the gift always goes to
@ the party.
Lowmere_OldLodge_EventScript_ReceivedDog::
	setflag FLAG_RECEIVED_KINGS_GIFT
	switch VAR_KINGS_GIFT_DOG
	case 0, Lowmere_OldLodge_EventScript_BufferEntei
	case 1, Lowmere_OldLodge_EventScript_BufferRaikou
	case 2, Lowmere_OldLodge_EventScript_BufferSuicune
	end

Lowmere_OldLodge_EventScript_BufferEntei::
	bufferspeciesname STR_VAR_1, SPECIES_ENTEI
	goto Lowmere_OldLodge_EventScript_AnnounceDog
	end

Lowmere_OldLodge_EventScript_BufferRaikou::
	bufferspeciesname STR_VAR_1, SPECIES_RAIKOU
	goto Lowmere_OldLodge_EventScript_AnnounceDog
	end

Lowmere_OldLodge_EventScript_BufferSuicune::
	bufferspeciesname STR_VAR_1, SPECIES_SUICUNE
	goto Lowmere_OldLodge_EventScript_AnnounceDog
	end

Lowmere_OldLodge_EventScript_AnnounceDog::
	setspeaker SP_NAME_NONE
	playfanfare MUS_OBTAIN_ITEM
	message Lowmere_Text_ReceivedDog
	waitmessage
	waitfanfare
	msgbox gText_NicknameThisPokemon, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, Lowmere_OldLodge_EventScript_CourierLeaves
	call Common_EventScript_GetGiftMonPartySlot
	call Common_EventScript_NameReceivedPartyMon
	goto Lowmere_OldLodge_EventScript_CourierLeaves
	end

Lowmere_OldLodge_EventScript_CourierLeaves::
	say Slice_Name_Courier, Lowmere_Text_CourierGoodLuck
	closemessage
	fadescreen FADE_TO_BLACK
	removeobject LOCALID_OLD_LODGE_COURIER
	setflag FLAG_HIDE_OLD_LODGE_COURIER
	fadescreen FADE_FROM_BLACK
	applymovement LOCALID_OLD_LODGE_BRAM, Common_Movement_FacePlayer
	waitmovement 0
	say Slice_Name_Bram, Lowmere_Text_BramLetMeSee
	narrate Lowmere_Text_BramReadsLetter
	say Slice_Name_Bram, Lowmere_Text_BramHisHand
	say Slice_Name_Bram, Lowmere_Text_BramMadMen
	say Slice_Name_Bram, Lowmere_Text_BramTamsinComing
	closemessage
	@ Bram goes back to his shed, and Tamsin leaves the forge for the road.
	fadescreen FADE_TO_BLACK
	removeobject LOCALID_OLD_LODGE_BRAM
	setflag FLAG_HIDE_OLD_LODGE_BRAM
	setflag FLAG_HIDE_LOWMERE_TAMSIN_FORGE
	fadescreen FADE_FROM_BLACK
	setvar VAR_SLICE_STATE, 7
	clearflag FLAG_HIDE_MIRE_ROAD_TAMSIN
	clearflag FLAG_HIDE_MIRE_ROAD_TAMSIN_WAGON
	releaseall
	end

@ Bram in the Old Lodge (shown from the courier scene on).
Lowmere_OldLodge_EventScript_Bram::
	lock
	faceplayer
	goto Lowmere_EventScript_BramHeal
	end

@ The courier, if talked to before the scene (should not normally happen).
Lowmere_OldLodge_EventScript_Courier::
	npc Slice_Name_Courier, Lowmere_Text_CourierThankFounders
	end

@ ---------------------------------------------------------------------------
@ Return to Lowmere (state 12 -> 13). signpact already raised
@ VAR_LOWMERE_STAGE, so the town loads in its Stage 1 layout. The first time
@ the player sees it, the carts are heard and Toft waves from the Old Lodge
@ (this marks the stage as seen; see docs/systems.md, "Upgrade scene"). The
@ scene with Toft plays on entering the Lodge.
@ ---------------------------------------------------------------------------
Lowmere_EventScript_CheckReturn::
	setvar VAR_TEMP_1, 1
	goto_if_ne VAR_SLICE_STATE, 12, Lowmere_EventScript_CheckReturnEnd
	goto_if_lowmere_stage_new Lowmere_EventScript_ReturnSeen
Lowmere_EventScript_CheckReturnEnd::
	end

Lowmere_EventScript_ReturnSeen::
	lockall
	narrate Lowmere_Text_Cartwheels
	narrate Lowmere_Text_ToftWaving
	closemessage
	lowmere_mark_stage_seen
	releaseall
	end

Lowmere_OldLodge_EventScript_GrainArrives::
	lockall
	applymovement LOCALID_OLD_LODGE_TOFT, Common_Movement_ExclamationMark
	waitmovement 0
	applymovement LOCALID_OLD_LODGE_TOFT, Common_Movement_FacePlayer
	waitmovement 0
	say Slice_Name_Toft, Lowmere_Text_ToftCarts
	say Slice_Name_Toft, Lowmere_Text_ToftCountedFourTimes
	say Slice_Name_Toft, Lowmere_Text_ToftEveryWeek
	say Slice_Name_Toft, Lowmere_Text_ToftLeaveItWithMe
	closemessage
	setvar VAR_SLICE_STATE, 13
	setflag FLAG_HIDE_LOWMERE_GRAIN_CARTS
	clearflag FLAG_HIDE_LOWMERE_TAMSIN_FORGE
	lowmere_mark_stage_seen
	releaseall
	end

@ ---------------------------------------------------------------------------
@ Villagers. Each has a Stage 0 complaint and a Stage 1 line.
@ ---------------------------------------------------------------------------
Lowmere_EventScript_Fisherman::
	lock
	faceplayer
	setspeaker Slice_Name_Fisher
	goto_if_ge VAR_LOWMERE_STAGE, 1, Lowmere_EventScript_FishermanStage1
	msgbox Lowmere_Text_FishermanJetty, MSGBOX_DEFAULT
	release
	end

Lowmere_EventScript_FishermanStage1::
	msgbox Lowmere_Text_FishermanBread, MSGBOX_DEFAULT
	release
	end

Lowmere_EventScript_Mother::
	lock
	faceplayer
	setspeaker Slice_Name_Mother
	goto_if_ge VAR_LOWMERE_STAGE, 1, Lowmere_EventScript_MotherStage1
	msgbox Lowmere_Text_MotherNoHealer, MSGBOX_DEFAULT
	release
	end

Lowmere_EventScript_MotherStage1::
	msgbox Lowmere_Text_MotherCoughBetter, MSGBOX_DEFAULT
	release
	end

Lowmere_EventScript_OldMan::
	lock
	faceplayer
	setspeaker Slice_Name_OldMan
	goto_if_ge VAR_LOWMERE_STAGE, 1, Lowmere_EventScript_OldManStage1
	msgbox Lowmere_Text_OldManNoShop, MSGBOX_DEFAULT
	release
	end

Lowmere_EventScript_OldManStage1::
	msgbox Lowmere_Text_OldManString, MSGBOX_DEFAULT
	release
	end

Lowmere_EventScript_Child::
	lock
	faceplayer
	setspeaker Slice_Name_Child
	goto_if_ge VAR_LOWMERE_STAGE, 1, Lowmere_EventScript_ChildStage1
	msgbox Lowmere_Text_ChildBellDream, MSGBOX_DEFAULT
	release
	end

Lowmere_EventScript_ChildStage1::
	msgbox Lowmere_Text_ChildSisterToo, MSGBOX_DEFAULT
	release
	end

@ Marta's stall: Stage 1 and later.
Lowmere_EventScript_Stallkeeper::
	lock
	faceplayer
	setspeaker Slice_Name_Stallkeeper
	message Lowmere_Text_StallWelcome
	waitmessage
	pokemart Lowmere_Mart_Stall
	setspeaker Slice_Name_Stallkeeper
	msgbox Lowmere_Text_StallComeAgain, MSGBOX_DEFAULT
	release
	end

	.align 2
Lowmere_Mart_Stall:
	.2byte ITEM_POTION
	.2byte ITEM_POKE_BALL
	.2byte ITEM_ANTIDOTE
	.2byte ITEM_PARALYZE_HEAL
	pokemartlistend

@ ---------------------------------------------------------------------------
@ Signs and examinable objects.
@ ---------------------------------------------------------------------------
Lowmere_EventScript_TownSign::
	sign Lowmere_Text_TownSign
	end

Lowmere_EventScript_OldLodgeSign::
	sign Lowmere_Text_OldLodgeSign
	end

Lowmere_EventScript_RangersShedSign::
	sign Lowmere_Text_RangersShedSign
	end

Lowmere_EventScript_HeskHouseSign::
	sign Lowmere_Text_HeskHouseSign
	end

Lowmere_EventScript_Well::
	goto_if_ge VAR_LOWMERE_STAGE, 1, Lowmere_EventScript_WellFixed
	sign Lowmere_Text_WellBroken
	end

Lowmere_EventScript_WellFixed::
	sign Lowmere_Text_WellFixed
	end

Lowmere_EventScript_GrainStore::
	goto_if_ge VAR_LOWMERE_STAGE, 1, Lowmere_EventScript_GrainStoreFull
	sign Lowmere_Text_GrainStoreEmpty
	end

Lowmere_EventScript_GrainStoreFull::
	sign Lowmere_Text_GrainStoreFull
	end

Lowmere_EventScript_BoardedHouse::
	sign Lowmere_Text_BoardedHouse
	end

Lowmere_EventScript_Jetty::
	sign Lowmere_Text_Jetty
	end

Lowmere_OldLodge_EventScript_Ledger::
	goto_if_ge VAR_LOWMERE_STAGE, 1, Lowmere_OldLodge_EventScript_LedgerStage1
	sign Lowmere_Text_LedgerStage0
	end

Lowmere_OldLodge_EventScript_LedgerStage1::
	sign Lowmere_Text_LedgerStage1
	end

@ ---------------------------------------------------------------------------
@ Text
@ ---------------------------------------------------------------------------

Lowmere_Text_BramHereWeAre::
	.string "Well. Here we are, {PLAYER}.\p"
	.string "LOWMERE.$"

Lowmere_Text_FirstLook::
	.string "Six houses, and three of them boarded\n"
	.string "up. A well with no bucket. A jetty that\l"
	.string "stops halfway out over the water.\p"
	.string "Marsh in every direction, as far as you\n"
	.string "can see.$"

Lowmere_Text_BramDontMakeThatFace::
	.string "Don't make that face. The court would\n"
	.string "pay good money to see it.\p"
	.string "This is yours now. Every leaking roof of\n"
	.string "it.\p"
	.string "The OLD LODGE is your royal residence.\n"
	.string "REEVE TOFT is waiting for you there. Go\l"
	.string "on. You'll find me at my shed.$"

Lowmere_Text_BramSeeToCart::
	.string "The OLD LODGE. TOFT's waiting. I'll see\n"
	.string "to the cart.$"

Lowmere_Text_ToftWelcome::
	.string "Your Highness! Welcome, welcome! ABEL\n"
	.string "TOFT, reeve of LOWMERE. Which is to say,\l"
	.string "the only official it has.\p"
	.string "I'd hoped to have the place swept for\n"
	.string "you. I did sweep it. The marsh came back\l"
	.string "in.\p"
	.string "Forgive me for starting with business,\n"
	.string "but you'll want to see this.$"

Lowmere_Text_ToftOpensLedger::
	.string "TOFT opens a heavy ledger. Most of the\n"
	.string "columns are empty.$"

Lowmere_Text_ToftElevenDays::
	.string "Grain in the town store: eleven days.\n"
	.string "Twelve, if nobody's hungry on Sundays.\p"
	.string "We buy our grain from HAYMARKET, up the\n"
	.string "MIRE ROAD. Their prices have tripled\l"
	.string "this year.\p"
	.string "We used to sell them our own grain, Your\n"
	.string "Highness. Now we can't afford to buy it\l"
	.string "back.$"

Lowmere_Text_ToftLastGovernor::
	.string "The last royal governor came out here\n"
	.string "once. He looked at this ledger, and he\l"
	.string "went straight home.\p"
	.string "I won't blame you if you do the same.$"

Lowmere_Text_ToftWillYouStay::
	.string "So, Your Highness. Will you stay?$"

Lowmere_Text_ToftThankYou::
	.string "Oh, thank goodness. I mean... very good,\n"
	.string "Your Highness.$"

Lowmere_Text_ToftYouHaventMoved::
	.string "Ha! The last governor said that too. He\n"
	.string "was halfway out of the door when he\l"
	.string "said it.\p"
	.string "You haven't moved an inch. I'll take\n"
	.string "that as a yes.$"

Lowmere_Text_ToftGetLowmereFed::
	.string "Then the first thing LOWMERE needs is\n"
	.string "very simple, Your Highness.\p"
	.string "It needs to eat. Get LOWMERE fed, and\n"
	.string "everything else can wait.$"

Lowmere_Text_ToftGoSeeBram::
	.string "Oh! BRAM said to send you to the\n"
	.string "RANGER'S SHED when we were done. He has\l"
	.string "something for you.\p"
	.string "It's the shed with the antlers over the\n"
	.string "door. You can't miss it. Many have tried.$"

Lowmere_Text_ToftGoSeeBramAgain::
	.string "BRAM's waiting for you at the RANGER'S\n"
	.string "SHED, Your Highness. The one with the\l"
	.string "antlers.$"

Lowmere_Text_ToftDaysLeft::
	.string "I've checked the grain store again,\n"
	.string "Your Highness. It hasn't grown. I keep\l"
	.string "hoping.$"

Lowmere_Text_ToftCourier::
	.string "There's a man from the city waiting for\n"
	.string "you, Your Highness. He won't give his\l"
	.string "letter to anyone else.\p"
	.string "He won't even give it to me, and I'm the\n"
	.string "reeve.$"

Lowmere_Text_ToftHaymarket::
	.string "HAYMARKET's up the MIRE ROAD, Your\n"
	.string "Highness. Mind your purse. Mind it twice.$"

Lowmere_Text_ToftPactSigned::
	.string "Is it true? A pact? With HAYMARKET? I\n"
	.string "won't believe it until I've weighed it.$"

Lowmere_Text_ToftStage1::
	.string "Grain for a month! A whole month!\p"
	.string "I've started a new ledger, Your\n"
	.string "Highness. It has numbers in it!$"

Lowmere_Text_BramNotYet::
	.string "TOFT first. He's been waiting all\n"
	.string "morning. He's been waiting all year,\l"
	.string "really.$"

Lowmere_Text_BramSitDown::
	.string "There you are. Sit down. No, not there.\n"
	.string "That chair's had it.$"

Lowmere_Text_BramOldHabit::
	.string "Thirty years a royal RANGER, and I still\n"
	.string "can't walk past a hurt POKéMON on the\l"
	.string "road.\p"
	.string "These three came to me that way.\n"
	.string "They're mended now. What they need is\l"
	.string "somebody to go out with.$"

Lowmere_Text_BramHaveALook::
	.string "And you're going to need a partner, if\n"
	.string "you mean to do anything about that\l"
	.string "ledger.\p"
	.string "Go on. Have a look at them.$"

Lowmere_Text_ToftWhiteOut::
	.string "Easy, Your Highness. Easy. The\n"
	.string "fishermen found you on the road and\l"
	.string "carried you in.$"

Lowmere_Text_ToftWhiteOutRest::
	.string "Your POKéMON have been seen to. Rest a\n"
	.string "while before you go back out there.$"

Lowmere_Text_WhichStarter::
	.string "Three POKé BALLS sit on Bram's table.\n"
	.string "Which one will you open?$"

Lowmere_Text_Treecko::
	.string "TREECKO$"

Lowmere_Text_Torchic::
	.string "TORCHIC$"

Lowmere_Text_Mudkip::
	.string "MUDKIP$"

Lowmere_Text_AboutTreecko::
	.string "TREECKO. Bram found it stuck in a snare\n"
	.string "in the reeds. It bit him twice while he\l"
	.string "cut it free.\p"
	.string "Will you choose TREECKO?$"

Lowmere_Text_AboutTorchic::
	.string "TORCHIC. It wandered into the shed one\n"
	.string "winter, half frozen, and slept by the\l"
	.string "stove until spring.\p"
	.string "Will you choose TORCHIC?$"

Lowmere_Text_AboutMudkip::
	.string "MUDKIP. It was pulled out of a drained\n"
	.string "fish pond, too weak to swim. It swims\l"
	.string "just fine now.\p"
	.string "Will you choose MUDKIP?$"

Lowmere_Text_ReceivedStarter::
	.string "{PLAYER} received the {STR_VAR_1}!$"

Lowmere_Text_BramYourMother::
	.string "Huh.\p"
	.string "Your mother would have made you pick\n"
	.string "the one that was afraid of you. I'm not\l"
	.string "her.\p"
	.string "You picked the one you liked. Good.$"

Lowmere_Text_BramRegister::
	.string "Take this as well. A RANGER's POKéDEX.\n"
	.string "Every POKéMON you meet goes in here.\p"
	.string "The Crown used to pay for that\n"
	.string "information. Maybe one day it will again.$"

Lowmere_Text_ReceivedPokedex::
	.string "{PLAYER} received the POKéDEX!$"

Lowmere_Text_BramBoots::
	.string "And these. My old RANGER's boots.\n"
	.string "They'll be big on you. Stuff the toes\l"
	.string "with moss.$"

Lowmere_Text_ReceivedBoots::
	.string "{PLAYER} received the RANGER'S BOOTS!$"

Lowmere_Text_BootsExplain::
	.string "{PLAYER} pulled on the boots.\p"
	.string "Hold the B Button while walking to run.$"

Lowmere_Text_BramBalls::
	.string "And some POKé BALLS. The marsh is full\n"
	.string "of POKéMON, and most of them want a\l"
	.string "better life than the marsh.$"

Lowmere_Text_BramGoSeeHesk::
	.string "Now. MOTHER HESK has been asking after\n"
	.string "you since the cart came in.\p"
	.string "She's in ELENA's old house, down by the\n"
	.string "reeds. Don't keep her waiting. She\l"
	.string "doesn't hold with waiting.$"

Lowmere_Text_BramLetMeLook::
	.string "Let me have a look at them.$"

Lowmere_Text_BramRightAsRain::
	.string "There. Right as rain. Marsh rain, anyway.$"

Lowmere_Text_BramTamsinWaiting::
	.string "There. Right as rain.\p"
	.string "TAMSIN's out on the north road, wearing\n"
	.string "a groove in it. You'd best go and see\l"
	.string "what she wants.$"

Lowmere_Text_BramCourier::
	.string "There. Right as rain.\p"
	.string "A courier came in from the capital while\n"
	.string "you were out. He's at the OLD LODGE. He\l"
	.string "looks like he's had a bad week.$"

Lowmere_Text_BramMireRoad::
	.string "There. Right as rain.\p"
	.string "HAYMARKET's up the MIRE ROAD. Keep\n"
	.string "your eyes open on the way. And keep\l"
	.string "TAMSIN out of trouble. Or the other way\l"
	.string "round.$"

Lowmere_Text_BramThornfield::
	.string "There. Right as rain.\p"
	.string "One pact, and TOFT's been singing all\n"
	.string "morning. I've never heard TOFT sing\l"
	.string "before. It's awful.\p"
	.string "Past HAYMARKET, the west road goes to\n"
	.string "THORNFIELD. PRINCESS ISOLDE's town.\l"
	.string "She'll be kinder to you than CORWIN was.\p"
	.string "Be careful of the kind ones.$"

Lowmere_Text_HeskSeeBramFirst::
	.string "Go and see BRAM first, child. He's been\n"
	.string "fussing over those POKéMON for weeks.\l"
	.string "Come back when you've got one.$"

Lowmere_Text_HeskLetMeLook::
	.string "Let me look at you.\p"
	.string "...Yes. ELENA's eyes. Everything else is\n"
	.string "your father's, more's the pity.$"

Lowmere_Text_HeskHerHouse::
	.string "This was her house. I've kept it as she\n"
	.string "left it. Mostly. The roof had its own\l"
	.string "ideas.$"

Lowmere_Text_HeskOneDay::
	.string "She left something for you. She said\n"
	.string "you'd want it one day.\p"
	.string "I've decided one day is today. I'm old,\n"
	.string "and I don't trust “one day” anymore.$"

Lowmere_Text_Entei::
	.string "ENTEI$"

Lowmere_Text_Raikou::
	.string "RAIKOU$"

Lowmere_Text_Suicune::
	.string "SUICUNE$"

Lowmere_Text_HeskNeverCold::
	.string "A feather, she called it, when she\n"
	.string "brought it home from the sea. It's no\l"
	.string "feather I've ever seen.\p"
	.string "It's never once been cold. Not in twelve\n"
	.string "winters. Keep it close.$"

Lowmere_Text_HeskBellDream::
	.string "And sleep with a candle lit, while you're\n"
	.string "here.\p"
	.string "Folk have been dreaming badly. The same\n"
	.string "dream, some of them. A bell that rings\l"
	.string "and makes no sound.\p"
	.string "Don't laugh. I'm too old to be laughed\n"
	.string "at.$"

Lowmere_Text_HeskTamsinPacing::
	.string "Off you go. TAMSIN's been pacing up and\n"
	.string "down the north road for an hour.\p"
	.string "That girl has never waited for anything\n"
	.string "in her life.$"

Lowmere_Text_HeskDreams::
	.string "Keep that feather close, child. And\n"
	.string "keep a candle lit.$"

Lowmere_Text_HeskDreamsWorse::
	.string "The dreams are worse since you came.\n"
	.string "The bell, every night now, in every\l"
	.string "house.\p"
	.string "Don't look like that. I don't mean it's\n"
	.string "your fault.\p"
	.string "I mean someone has noticed you.$"

Lowmere_Text_ElenasBed::
	.string "A narrow bed with a patched quilt.\n"
	.string "Someone still airs it every week.$"

Lowmere_Text_TamsinForge::
	.string "So it's true. They gave you LOWMERE.\p"
	.string "Don't look at me like that. I'm glad\n"
	.string "you're back. I'm just not glad about\l"
	.string "why.\p"
	.string "Pa says the court sent you here to fail\n"
	.string "quietly. I told him you've never done\l"
	.string "anything quietly in your life.$"

Lowmere_Text_TamsinGoSeeCourier::
	.string "Go on, go and see what your muddy man\n"
	.string "wants. I'll be here. Hitting things.$"

Lowmere_Text_TamsinOneDown::
	.string "One down. Eight to go, Your Highness.\p"
	.string "...I'm being sarcastic. Mostly.$"

Lowmere_Text_TamsinIron_M::
	.string "Pa says if you bring him iron, he'll mend\n"
	.string "every fence in town. I told him you're a\l"
	.string "prince, not a mule.\p"
	.string "Bring him iron anyway.$"

Lowmere_Text_TamsinIron_F::
	.string "Pa says if you bring him iron, he'll mend\n"
	.string "every fence in town. I told him you're a\l"
	.string "princess, not a mule.\p"
	.string "Bring him iron anyway.$"

Lowmere_Text_TamsinTookYourTime::
	.string "There you are. Took your time.\p"
	.string "Everyone in town's talking about you.\n"
	.string "Half of them think you're going to save\l"
	.string "us. The other half think you're a spy.$"

Lowmere_Text_TamsinWorthFollowing_M::
	.string "So here's what I think. If a prince is\n"
	.string "going up that road to sort out\l"
	.string "HAYMARKET, I want to know they're worth\l"
	.string "following.\p"
	.string "Pa's been training this one at the forge\n"
	.string "all spring. Let's see what yours can do!$"

Lowmere_Text_TamsinWorthFollowing_F::
	.string "So here's what I think. If a princess is\n"
	.string "going up that road to sort out\l"
	.string "HAYMARKET, I want to know they're worth\l"
	.string "following.\p"
	.string "Pa's been training this one at the forge\n"
	.string "all spring. Let's see what yours can do!$"

Lowmere_Text_TamsinDefeated::
	.string "...Huh. Fine. You're not useless.$"

Lowmere_Text_TamsinNotUseless::
	.string "Don't get a big head about it. I went\n"
	.string "easy on you.\p"
	.string "...I didn't go easy on you.$"

Lowmere_Text_TamsinManAtLodge::
	.string "Oh, and there's a man at your lodge. City\n"
	.string "clothes, mud up to the knees. Looks like\l"
	.string "he's been crying the whole way here.\p"
	.string "You should probably go and see what he\n"
	.string "wants.$"

Lowmere_Text_NorthRoadSeeToft::
	.string "The road north leads up into the marsh.\n"
	.string "REEVE TOFT is waiting at the OLD LODGE\l"
	.string "first.$"

Lowmere_Text_NorthRoadNoMon::
	.string "The MIRE ROAD is no place to walk\n"
	.string "without a POKéMON. BRAM is waiting at\l"
	.string "the RANGER'S SHED.$"

Lowmere_Text_NorthRoadHesk::
	.string "BRAM said MOTHER HESK wanted to see\n"
	.string "you first. Her house is down by the\l"
	.string "reeds.$"

Lowmere_Text_NorthRoadCourier::
	.string "TAMSIN said there's a man waiting for\n"
	.string "you at the OLD LODGE.$"

Lowmere_Text_CourierThankFounders::
	.string "Your... Your Highness? Oh, thank the\n"
	.string "Founders.\p"
	.string "They told me the marsh road was bad.\n"
	.string "They didn't tell me it was a lake.$"

Lowmere_Text_CourierFromThePalace::
	.string "I'm from the palace. The king's own\n"
	.string "household, not the Chancellery.\p"
	.string "I'm to put this into your hands, and\n"
	.string "nobody else's.$"

Lowmere_Text_LetterAndBalls::
	.string "The courier holds out a letter. The wax\n"
	.string "is pressed with a ring. Not the\l"
	.string "Chancellery's seal, but the king's own\l"
	.string "signet.\p"
	.string "On the table behind him sit three POKé\n"
	.string "BALLS, wrapped in a royal blanket.$"

Lowmere_Text_CourierStoppedAtGate::
	.string "The CHANCELLOR's men stopped me at the\n"
	.string "city gate. They opened the bag. They\l"
	.string "looked at the BALLS for a long time.\p"
	.string "Then the CHANCELLOR came down himself,\n"
	.string "and he said, “The king's gift is the\l"
	.string "king's affair.” And he let me go. Just\l"
	.string "like that.$"

Lowmere_Text_CourierFrightened::
	.string "I don't know why that frightened me\n"
	.string "more than if he'd kept them.$"

Lowmere_Text_BreakTheSeal::
	.string "You break the seal. The handwriting is\n"
	.string "shaky, and every line slopes downhill,\l"
	.string "as if the hand that wrote it was very\l"
	.string "tired.$"

Lowmere_Text_LetterOpening_M::
	.string "“My dear son,”$"

Lowmere_Text_LetterOpening_F::
	.string "“My dear daughter,”$"

Lowmere_Text_Letter::
	.string "“I am sorry. I could not stop the\n"
	.string "Apportionment. I am not well, and there\l"
	.string "are days I do not know where I am. Today\l"
	.string "I do.”\p"
	.string "“They have refused you what every\n"
	.string "child of mine was given. So I am giving it\l"
	.string "to you myself.”\p"
	.string "“These three are from the royal\n"
	.string "kennels. Their keeper loved your\l"
	.string "mother, as I did. Choose the one that\l"
	.string "suits you.”\p"
	.string "“Your mother always said I chose badly\n"
	.string "for myself. Do better than me.”\p"
	.string "“ALDRIC.”$"

Lowmere_Text_CourierWhichOne::
	.string "Well, Your Highness? Which one will it\n"
	.string "be?$"

Lowmere_Text_ChooseWhichOne::
	.string "Which POKé BALL will you open?$"

Lowmere_Text_AboutEntei::
	.string "ENTEI. A beast of fire, born, they say,\n"
	.string "on the day the first volcano erupted.\l"
	.string "It does not back down.\p"
	.string "Will you choose ENTEI?$"

Lowmere_Text_AboutRaikou::
	.string "RAIKOU. A beast of thunder, that runs\n"
	.string "like lightning and roars like the storm.\p"
	.string "Will you choose RAIKOU?$"

Lowmere_Text_AboutSuicune::
	.string "SUICUNE. A beast of the north wind, said\n"
	.string "to make clean any water it touches.\p"
	.string "Will you choose SUICUNE?$"

Lowmere_Text_ReceivedDog::
	.string "{PLAYER} received {STR_VAR_1}, the\n"
	.string "king's gift!$"

Lowmere_Text_CourierGoodLuck::
	.string "I'll take the other two back to the\n"
	.string "kennels. The keeper will want to know\l"
	.string "which one you chose.\p"
	.string "Good luck, Your Highness. I mean that.$"

Lowmere_Text_BramLetMeSee::
	.string "Let me see that.$"

Lowmere_Text_BramReadsLetter::
	.string "BRAM reads the letter over your\n"
	.string "shoulder. He reads it twice. Then he is\l"
	.string "quiet for a long time.$"

Lowmere_Text_BramHisHand::
	.string "He wrote this himself. That's his hand.\p"
	.string "He hasn't written anything himself in\n"
	.string "years. Everything with his name on it\l"
	.string "comes out of the Chancellery.$"

Lowmere_Text_BramMadMen::
	.string "They say he's mad.\p"
	.string "Mad men don't apologise.$"

Lowmere_Text_BramTamsinComing::
	.string "...Well. You've still got a ledger to fix.\n"
	.string "HAYMARKET's up the MIRE ROAD.\p"
	.string "And it seems TAMSIN has decided she's\n"
	.string "going with you. I wouldn't argue with\l"
	.string "her. I never win either.$"

Lowmere_Text_Cartwheels::
	.string "As you come down the MIRE ROAD, you\n"
	.string "hear it before you see it. Cartwheels. A\l"
	.string "lot of cartwheels.$"

Lowmere_Text_ToftWaving::
	.string "REEVE TOFT is at the door of the OLD\n"
	.string "LODGE, waving both arms.$"

Lowmere_Text_ToftCarts::
	.string "Your Highness! Your Highness! Did you\n"
	.string "see the carts? From HAYMARKET!\p"
	.string "Grain, at fair weight, and the bill's a\n"
	.string "third of last month's!$"

Lowmere_Text_ToftCountedFourTimes::
	.string "I've counted it four times. Four! I keep\n"
	.string "thinking I've made a mistake. I haven't!\l"
	.string "For once!$"

Lowmere_Text_ToftEveryWeek::
	.string "And the carters say there'll be more\n"
	.string "every week, by the terms of the pact.\l"
	.string "Every week, Your Highness!$"

Lowmere_Text_ToftLeaveItWithMe::
	.string "We can mend the well. We can open the\n"
	.string "grain store. Old MARTA says she'll open\l"
	.string "a stall, if there's anything to sell.\p"
	.string "Go and look, Your Highness! We've\n"
	.string "already started!$"

Lowmere_Text_FishermanJetty::
	.string "The jetty fell in last winter. Nobody\n"
	.string "came to mend it.\p"
	.string "Now I fish off the bank, like a child.$"

Lowmere_Text_FishermanBread::
	.string "Bread, three days running! I'd\n"
	.string "forgotten what it tasted like.\p"
	.string "The jetty next, eh, Your Highness?$"

Lowmere_Text_MotherNoHealer::
	.string "My boy's had a cough since the spring.\n"
	.string "The nearest healer's in HAYMARKET, and\l"
	.string "she wants paying first.$"

Lowmere_Text_MotherCoughBetter::
	.string "The cough's better. Proper food does\n"
	.string "that.\p"
	.string "Thank you, Your Highness.$"

Lowmere_Text_OldManNoShop::
	.string "A shop? Ha! The last shop in LOWMERE\n"
	.string "closed when I still had hair.$"

Lowmere_Text_OldManString::
	.string "MARTA's stall is open! She sells\n"
	.string "POTIONS! And string!\p"
	.string "I've wanted string for years.$"

Lowmere_Text_ChildBellDream::
	.string "I keep dreaming about a bell. It rings\n"
	.string "and rings, but it doesn't make any\l"
	.string "sound.\p"
	.string "Mum says it's only a dream.$"

Lowmere_Text_ChildSisterToo::
	.string "I still have the bell dream.\p"
	.string "Now my sister has it too.$"

Lowmere_Text_StallWelcome::
	.string "Not much on the stall yet, Your\n"
	.string "Highness, but it's honest. Fair weight,\l"
	.string "too. I checked.$"

Lowmere_Text_StallComeAgain::
	.string "Come again. I'll have more when the next\n"
	.string "cart comes in.$"

Lowmere_Text_TownSign::
	.string "LOWMERE\p"
	.string "Where the road ends.$"

Lowmere_Text_OldLodgeSign::
	.string "THE OLD LODGE\p"
	.string "Royal residence, by order of the\n"
	.string "Chancellery.$"

Lowmere_Text_RangersShedSign::
	.string "RANGER'S SHED\p"
	.string "Knock loudly.$"

Lowmere_Text_HeskHouseSign::
	.string "MOTHER HESK\p"
	.string "Herbs, remedies and opinions.$"

Lowmere_Text_WellBroken::
	.string "The well's winch is broken, and the\n"
	.string "bucket is long gone.$"

Lowmere_Text_WellFixed::
	.string "The well has a new winch and a new\n"
	.string "bucket. The water is cold and clean.$"

Lowmere_Text_GrainStoreEmpty::
	.string "The town grain store. A sign on the door\n"
	.string "says: 11 DAYS.$"

Lowmere_Text_GrainStoreFull::
	.string "The grain store is full. Someone has\n"
	.string "crossed out the 11 DAYS on the sign and\l"
	.string "written FULL in big letters.$"

Lowmere_Text_BoardedHouse::
	.string "The door is boarded up. A note nailed to\n"
	.string "it says GONE TO HAYMARKET. The ink has\l"
	.string "nearly washed away.$"

Lowmere_Text_Jetty::
	.string "The jetty ends in broken planks and\n"
	.string "black water.$"

Lowmere_Text_LedgerStage0::
	.string "TOFT's ledger. GRAIN IN STORE: 11 DAYS.\n"
	.string "Every other column is empty.$"

Lowmere_Text_LedgerStage1::
	.string "TOFT's new ledger. GRAIN IN STORE: 30\n"
	.string "DAYS. Someone has drawn a small smiling\l"
	.string "face in the margin.$"

