@ Slice scenes 7-9: Haymarket, Widow Penn's seizure (Corwin's crime scene),
@ the case (granary, counting house, witness), the magistrate and Silas Crane.
@ Object, trigger and sign placement: docs/dialogue/events.md#haymarket

#ifndef LOCALID_HAYMARKET_PENN
#define LOCALID_HAYMARKET_PENN 1
#endif
#ifndef LOCALID_HAYMARKET_SEIZURE_CLERK_1
#define LOCALID_HAYMARKET_SEIZURE_CLERK_1 2
#endif
#ifndef LOCALID_HAYMARKET_SEIZURE_CLERK_2
#define LOCALID_HAYMARKET_SEIZURE_CLERK_2 3
#endif
#ifndef LOCALID_HAYMARKET_CORWIN
#define LOCALID_HAYMARKET_CORWIN 4
#endif
#ifndef LOCALID_HAYMARKET_TAMSIN
#define LOCALID_HAYMARKET_TAMSIN 5
#endif
#ifndef LOCALID_HAYMARKET_CRANE
#define LOCALID_HAYMARKET_CRANE 6
#endif
#ifndef LOCALID_COUNTING_HOUSE_PENN_BALL
#define LOCALID_COUNTING_HOUSE_PENN_BALL 1
#endif

@ Widow Penn's old POKéMON, seized by the guild.
#define PENN_MON SPECIES_CHANSEY

@ ---------------------------------------------------------------------------
@ Arrival (state 8 -> 9). Coord trigger where the market square comes into
@ view. On screen: guild clerks seize Widow Penn's POKéMON while the crowd
@ looks away. Then Corwin, on the fair stage, spots the player.
@ ---------------------------------------------------------------------------
Haymarket_EventScript_Arrival::
	lockall
	narrate Haymarket_Text_FirstLook
	playbgm MUS_ENCOUNTER_SUSPICIOUS, FALSE
	say Slice_Name_Clerk, Haymarket_Text_ClerkTwoFines
	applymovement LOCALID_HAYMARKET_PENN, Haymarket_Movement_PennProtest
	waitmovement 0
	say Slice_Name_Penn, Haymarket_Text_PennArithmetic
	say Slice_Name_Clerk, Haymarket_Text_ClerkPaymentInKind
	say Slice_Name_Penn, Haymarket_Text_PennPlease
	say Slice_Name_Clerk2, Haymarket_Text_Clerk2Sold
	closemessage
	applymovement LOCALID_HAYMARKET_SEIZURE_CLERK_1, Haymarket_Movement_ClerkReaches
	waitmovement 0
	narrate Haymarket_Text_PennGrabsSleeve
	closemessage
	playse SE_M_DOUBLE_SLAP
	call Slice_EventScript_ShakeScreenSmall
	playmoncry PENN_MON, CRY_MODE_WEAK
	fadescreen FADE_TO_BLACK
	waitmoncry
	playse SE_BALL
	waitse
	delay 30
	removeobject LOCALID_HAYMARKET_SEIZURE_CLERK_1
	removeobject LOCALID_HAYMARKET_SEIZURE_CLERK_2
	setflag FLAG_HIDE_HAYMARKET_SEIZURE_CLERKS
	turnobject LOCALID_HAYMARKET_PENN, DIR_SOUTH
	fadescreen FADE_FROM_BLACK
	narrate Haymarket_Text_CrowdLooksAway
	say Slice_Name_Penn, Haymarket_Text_PennThirtyYears
	closemessage
	delay 30
	playse SE_APPLAUSE
	say Slice_Name_Corwin, Haymarket_Text_CorwinPumpkins
	say Slice_Name_Crowd, Haymarket_Text_CrowdHurrah
	applymovement LOCALID_HAYMARKET_CORWIN, Common_Movement_ExclamationMark
	waitmovement 0
	applymovement LOCALID_HAYMARKET_CORWIN, Common_Movement_FacePlayer
	waitmovement 0
	say_g Slice_Name_Corwin, Haymarket_Text_CorwinFrog_M, Haymarket_Text_CorwinFrog_F
	narrate Haymarket_Text_CrowdLaughs
	say_g Slice_Name_Corwin, Haymarket_Text_CorwinBringMoney_M, Haymarket_Text_CorwinBringMoney_F
	narrate Haymarket_Text_YouTellCorwin
	say Slice_Name_Corwin, Haymarket_Text_CorwinMarketsHavePrices
	say Slice_Name_Corwin, Haymarket_Text_CorwinOnWhatGrounds
	say Slice_Name_Corwin, Haymarket_Text_CorwinEnjoyTheFair
	closemessage
	fadescreen FADE_TO_BLACK
	removeobject LOCALID_HAYMARKET_CORWIN
	setflag FLAG_HIDE_HAYMARKET_CORWIN_SQUARE
	clearflag FLAG_HIDE_HAYMARKET_TAMSIN
	addobject LOCALID_HAYMARKET_TAMSIN
	fadedefaultbgm
	fadescreen FADE_FROM_BLACK
	applymovement LOCALID_HAYMARKET_TAMSIN, Common_Movement_FacePlayer
	waitmovement 0
	say Slice_Name_Tamsin, Haymarket_Text_TamsinHeardThat
	say Slice_Name_Tamsin, Haymarket_Text_TamsinWeNeedProof
	closemessage
	setvar VAR_SLICE_STATE, 9
	releaseall
	end

Haymarket_Movement_PennProtest:
	walk_in_place_faster_left
	walk_in_place_faster_right
	step_end

@ Clerk 1 stands directly left of Penn, facing right.
Haymarket_Movement_ClerkReaches:
	walk_in_place_faster_right
	walk_in_place_faster_right
	step_end

@ ---------------------------------------------------------------------------
@ Tamsin in Haymarket: hints for the case, in any order.
@ ---------------------------------------------------------------------------
Haymarket_EventScript_Tamsin::
	lock
	faceplayer
	setspeaker Slice_Name_Tamsin
	goto_if_ge VAR_SLICE_STATE, 12, Haymarket_EventScript_TamsinAfterPact
	goto_if_set FLAG_HAYMARKET_TRIAL_GRANTED, Haymarket_EventScript_TamsinGoWin
	goto_if_unset FLAG_HAYMARKET_WEIGHTS_FOUND, Haymarket_EventScript_TamsinHintWeights
	goto_if_unset FLAG_HAYMARKET_WITNESS, Haymarket_EventScript_TamsinHintWitness
	msgbox Haymarket_Text_TamsinHintMagistrate, MSGBOX_DEFAULT
	release
	end

Haymarket_EventScript_TamsinHintWeights::
	msgbox Haymarket_Text_TamsinHintWeights, MSGBOX_DEFAULT
	release
	end

Haymarket_EventScript_TamsinHintWitness::
	msgbox Haymarket_Text_TamsinHintWitness, MSGBOX_DEFAULT
	release
	end

Haymarket_EventScript_TamsinGoWin::
	msgbox Haymarket_Text_TamsinGoWin, MSGBOX_DEFAULT
	release
	end

Haymarket_EventScript_TamsinAfterPact::
	msgbox Haymarket_Text_TamsinAfterPact, MSGBOX_DEFAULT
	release
	end

@ ---------------------------------------------------------------------------
@ Widow Penn: the witness. She testifies once her POKéMON is back.
@ ---------------------------------------------------------------------------
Haymarket_EventScript_Penn::
	lock
	faceplayer
	setspeaker Slice_Name_Penn
	goto_if_ge VAR_SLICE_STATE, 12, Haymarket_EventScript_PennAfterPact
	goto_if_set FLAG_HAYMARKET_WITNESS, Haymarket_EventScript_PennWillTestify
	goto_if_set FLAG_PENN_MON_RECOVERED, Haymarket_EventScript_PennReunion
	msgbox Haymarket_Text_PennNotUntilDaisy, MSGBOX_DEFAULT
	release
	end

Haymarket_EventScript_PennReunion::
	narrate Haymarket_Text_HandPennBall
	playse SE_BALL_OPEN
	playmoncry PENN_MON, CRY_MODE_NORMAL
	waitmoncry
	say Slice_Name_Penn, Haymarket_Text_PennMySillyGirl
	say Slice_Name_Penn, Haymarket_Text_PennIllTestify
	setflag FLAG_HAYMARKET_WITNESS
	release
	end

Haymarket_EventScript_PennWillTestify::
	msgbox Haymarket_Text_PennWillTestify, MSGBOX_DEFAULT
	release
	end

Haymarket_EventScript_PennAfterPact::
	msgbox Haymarket_Text_PennEggs, MSGBOX_DEFAULT
	release
	end

@ ---------------------------------------------------------------------------
@ Townsfolk. Most of Haymarket likes Corwin and blames the guild.
@ ---------------------------------------------------------------------------
Haymarket_EventScript_Farmer::
	lock
	faceplayer
	setspeaker Slice_Name_Farmer
	goto_if_ge VAR_SLICE_STATE, 12, Haymarket_EventScript_FarmerAfterPact
	msgbox Haymarket_Text_FarmerFair, MSGBOX_DEFAULT
	release
	end

Haymarket_EventScript_FarmerAfterPact::
	msgbox Haymarket_Text_FarmerLoaf, MSGBOX_DEFAULT
	release
	end

Haymarket_EventScript_Granny::
	lock
	faceplayer
	setspeaker Slice_Name_Granny
	goto_if_ge VAR_SLICE_STATE, 12, Haymarket_EventScript_GrannyAfterPact
	msgbox Haymarket_Text_GrannyGoodBoy, MSGBOX_DEFAULT
	release
	end

Haymarket_EventScript_GrannyAfterPact::
	msgbox Haymarket_Text_GrannyKnew, MSGBOX_DEFAULT
	release
	end

Haymarket_EventScript_RibbonSeller::
	lock
	faceplayer
	setspeaker Slice_Name_Merchant
	goto_if_ge VAR_SLICE_STATE, 12, Haymarket_EventScript_RibbonSellerAfterPact
	msgbox Haymarket_Text_RibbonTax, MSGBOX_DEFAULT
	release
	end

Haymarket_EventScript_RibbonSellerAfterPact::
	msgbox Haymarket_Text_RibbonNoTax, MSGBOX_DEFAULT
	release
	end

Haymarket_EventScript_Girl::
	lock
	faceplayer
	setspeaker Slice_Name_Girl
	goto_if_ge VAR_SLICE_STATE, 12, Haymarket_EventScript_GirlAfterPact
	msgbox Haymarket_Text_GirlPumpkin, MSGBOX_DEFAULT
	release
	end

Haymarket_EventScript_GirlAfterPact::
	msgbox Haymarket_Text_GirlPumpkinLost, MSGBOX_DEFAULT
	release
	end

Haymarket_EventScript_Boy::
	lock
	faceplayer
	setspeaker Slice_Name_Youngster
	goto_if_ge VAR_SLICE_STATE, 12, Haymarket_EventScript_BoyAfterPact
	msgbox Haymarket_Text_BoySweet, MSGBOX_DEFAULT
	release
	end

Haymarket_EventScript_BoyAfterPact::
	msgbox Haymarket_Text_BoyStillLoveCorwin, MSGBOX_DEFAULT
	release
	end

Haymarket_EventScript_NervousMan::
	lock
	faceplayer
	setspeaker Slice_Name_Townsfolk
	goto_if_ge VAR_SLICE_STATE, 12, Haymarket_EventScript_NervousManAfterPact
	msgbox Haymarket_Text_DontStare, MSGBOX_DEFAULT
	release
	end

Haymarket_EventScript_NervousManAfterPact::
	msgbox Haymarket_Text_StareAllYouLike, MSGBOX_DEFAULT
	release
	end

@ ---------------------------------------------------------------------------
@ The Gilt Pavilion's door guard. A coord trigger in front of the door turns
@ the player back until the Trial is granted.
@ ---------------------------------------------------------------------------
Haymarket_EventScript_PavilionGuard::
	lock
	faceplayer
	setspeaker Slice_Name_Guard
	goto_if_set FLAG_HAYMARKET_TRIAL_GRANTED, Haymarket_EventScript_PavilionGuardOpen
	msgbox Haymarket_Text_GuardClosed, MSGBOX_DEFAULT
	release
	end

Haymarket_EventScript_PavilionGuardOpen::
	msgbox Haymarket_Text_GuardOpen, MSGBOX_DEFAULT
	release
	end

@ Assumes the door is above the trigger.
Haymarket_EventScript_PavilionDoorTrigger::
	lockall
	goto_if_set FLAG_HAYMARKET_TRIAL_GRANTED, Haymarket_EventScript_PavilionDoorOpen
	say Slice_Name_Guard, Haymarket_Text_GuardClosed
	closemessage
	applymovement LOCALID_PLAYER, Common_Movement_WalkDown
	waitmovement 0
	releaseall
	end

Haymarket_EventScript_PavilionDoorOpen::
	releaseall
	end

@ ---------------------------------------------------------------------------
@ Silas Crane (state 10 -> 11). Coord trigger outside the Magistrate's Hall,
@ live while VAR_SLICE_STATE is 10. Crane is shown from the Trial grant on.
@ ---------------------------------------------------------------------------
Haymarket_EventScript_Crane::
	lockall
	goto_if_set FLAG_MET_CRANE, Haymarket_EventScript_CraneDone
	applymovement LOCALID_HAYMARKET_CRANE, Common_Movement_FacePlayer
	waitmovement 0
	say Slice_Name_Crane, Haymarket_Text_CraneWhatAPleasure
	say Slice_Name_Crane, Haymarket_Text_CraneBlessHim
	goto Haymarket_EventScript_CraneOffer
	end

Haymarket_EventScript_CraneOffer::
	setspeaker Slice_Name_Crane
	msgbox Haymarket_Text_CraneOffer, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, Haymarket_EventScript_CraneRefused
	narrate Haymarket_Text_YouThinkOfToft
	narrate Haymarket_Text_YouCantDoIt
	goto Haymarket_EventScript_CraneRefused
	end

Haymarket_EventScript_CraneRefused::
	say Slice_Name_Crane, Haymarket_Text_CranePity
	say_g Slice_Name_Crane, Haymarket_Text_CraneRestWell_M, Haymarket_Text_CraneRestWell_F
	narrate Haymarket_Text_CraneWalksAway
	closemessage
	fadescreen FADE_TO_BLACK
	removeobject LOCALID_HAYMARKET_CRANE
	setflag FLAG_HIDE_HAYMARKET_CRANE
	setflag FLAG_MET_CRANE
	setvar VAR_SLICE_STATE, 11
	fadescreen FADE_FROM_BLACK
	releaseall
	end

Haymarket_EventScript_CraneDone::
	releaseall
	end

@ ---------------------------------------------------------------------------
@ The Magistrate's Hall. Magistrate Lyle Arden grants the Trial when all three
@ pieces of evidence are in (state 9 -> 10).
@ ---------------------------------------------------------------------------
Haymarket_MagistrateHall_EventScript_Arden::
	lock
	faceplayer
	setspeaker Slice_Name_Arden
	goto_if_ge VAR_SLICE_STATE, 12, Haymarket_MagistrateHall_EventScript_ArdenAfterPact
	goto_if_set FLAG_HAYMARKET_TRIAL_GRANTED, Haymarket_MagistrateHall_EventScript_ArdenGoOn
	goto_if_unset FLAG_FOUND_BELL_RECEIPT, Haymarket_MagistrateHall_EventScript_ArdenNeedProof
	goto_if_unset FLAG_HAYMARKET_WEIGHTS_FOUND, Haymarket_MagistrateHall_EventScript_ArdenNeedProof
	goto_if_unset FLAG_HAYMARKET_WITNESS, Haymarket_MagistrateHall_EventScript_ArdenNeedProof
	goto Haymarket_MagistrateHall_EventScript_GrantTrial
	end

Haymarket_MagistrateHall_EventScript_ArdenNeedProof::
	msgbox Haymarket_Text_ArdenComplaint, MSGBOX_DEFAULT
	setspeaker Slice_Name_Arden
	msgbox Haymarket_Text_ArdenThreeThings, MSGBOX_DEFAULT
	setspeaker SP_NAME_NONE
	call_if_set FLAG_FOUND_BELL_RECEIPT, Haymarket_MagistrateHall_EventScript_HaveReceipt
	call_if_set FLAG_HAYMARKET_WEIGHTS_FOUND, Haymarket_MagistrateHall_EventScript_HaveWeights
	call_if_set FLAG_HAYMARKET_WITNESS, Haymarket_MagistrateHall_EventScript_HaveWitness
	release
	end

Haymarket_MagistrateHall_EventScript_HaveReceipt::
	msgbox Haymarket_Text_HaveReceipt, MSGBOX_DEFAULT
	return

Haymarket_MagistrateHall_EventScript_HaveWeights::
	msgbox Haymarket_Text_HaveWeights, MSGBOX_DEFAULT
	return

Haymarket_MagistrateHall_EventScript_HaveWitness::
	msgbox Haymarket_Text_HaveWitness, MSGBOX_DEFAULT
	return

Haymarket_MagistrateHall_EventScript_GrantTrial::
	msgbox Haymarket_Text_ArdenLetMeSee, MSGBOX_DEFAULT
	say Slice_Name_Arden, Haymarket_Text_ArdenThirtyOneYears
	say Slice_Name_Arden, Haymarket_Text_ArdenTheLawIsOld
	closemessage
	playse SE_BANG
	waitse
	narrate Haymarket_Text_StampsWrit
	say Slice_Name_Arden, Haymarket_Text_ArdenWatchYourBack
	setflag FLAG_HAYMARKET_TRIAL_GRANTED
	clearflag FLAG_HIDE_HAYMARKET_CRANE
	setvar VAR_SLICE_STATE, 10
	release
	end

Haymarket_MagistrateHall_EventScript_ArdenGoOn::
	msgbox Haymarket_Text_ArdenGoOn, MSGBOX_DEFAULT
	release
	end

Haymarket_MagistrateHall_EventScript_ArdenAfterPact::
	msgbox Haymarket_Text_ArdenAfterPact, MSGBOX_DEFAULT
	release
	end

Haymarket_MagistrateHall_EventScript_Clerk::
	npc Slice_Name_Townsfolk, Haymarket_Text_CourtClerk
	end

@ ---------------------------------------------------------------------------
@ The granary: the false weights. The foreman guards the scale room; the map
@ thread owns the push-the-sacks puzzle in front of it.
@ ---------------------------------------------------------------------------
Haymarket_Granary_EventScript_Foreman::
	trainerbattle_single TRAINER_HAYMARKET_FOREMAN, Haymarket_Text_ForemanIntro, Haymarket_Text_ForemanDefeat
	npc Slice_Name_Foreman, Haymarket_Text_ForemanPostBattle
	end

Haymarket_Granary_EventScript_Scale::
	lockall
	goto_if_set FLAG_HAYMARKET_WEIGHTS_FOUND, Haymarket_Granary_EventScript_ScaleAgain
	narrate Haymarket_Text_ScaleBrass
	narrate Haymarket_Text_ScaleCompare
	setflag FLAG_HAYMARKET_WEIGHTS_FOUND
	narrate Haymarket_Text_FoundWeights
	releaseall
	end

Haymarket_Granary_EventScript_ScaleAgain::
	sign Haymarket_Text_ScaleAgain
	end

Haymarket_Granary_EventScript_CrownCrate::
	sign Haymarket_Text_CrownCrate
	end

Haymarket_Granary_EventScript_Sacks::
	sign Haymarket_Text_GranarySacks
	end

Haymarket_Granary_EventScript_Worker::
	npc Slice_Name_Townsfolk, Haymarket_Text_GranaryWorker
	end

@ ---------------------------------------------------------------------------
@ The Gilded Scale counting house: the back room of "payments", and Crane's
@ office, which is empty after the pact.
@ ---------------------------------------------------------------------------
Haymarket_CountingHouse_EventScript_FrontClerk::
	lock
	faceplayer
	setspeaker Slice_Name_Clerk
	goto_if_ge VAR_SLICE_STATE, 12, Haymarket_CountingHouse_EventScript_FrontClerkAfter
	msgbox Haymarket_Text_FrontClerk, MSGBOX_DEFAULT
	release
	end

Haymarket_CountingHouse_EventScript_FrontClerkAfter::
	msgbox Haymarket_Text_FrontClerkAfter, MSGBOX_DEFAULT
	release
	end

Haymarket_CountingHouse_EventScript_Clerk1::
	trainerbattle_single TRAINER_HAYMARKET_CLERK_1, Haymarket_Text_Clerk1Intro, Haymarket_Text_Clerk1Defeat
	npc Slice_Name_Clerk, Haymarket_Text_Clerk1PostBattle
	end

Haymarket_CountingHouse_EventScript_Clerk2::
	trainerbattle_single TRAINER_HAYMARKET_CLERK_2, Haymarket_Text_Clerk2Intro, Haymarket_Text_Clerk2Defeat
	npc Slice_Name_Clerk, Haymarket_Text_Clerk2PostBattle
	end

@ Penn's POKé BALL on the back-room shelf.
Haymarket_CountingHouse_EventScript_PennBall::
	lockall
	narrate Haymarket_Text_PennBallLabel
	removeobject LOCALID_COUNTING_HOUSE_PENN_BALL
	setflag FLAG_HIDE_COUNTING_HOUSE_PENN_BALL
	setflag FLAG_PENN_MON_RECOVERED
	playfanfare MUS_OBTAIN_ITEM
	message Haymarket_Text_TookPennBall
	waitmessage
	waitfanfare
	releaseall
	end

Haymarket_CountingHouse_EventScript_PaymentShelves::
	sign Haymarket_Text_PaymentShelves
	end

Haymarket_CountingHouse_EventScript_CraneDesk::
	lockall
	goto_if_set FLAG_RECEIVED_SEALED_LETTER, Haymarket_CountingHouse_EventScript_CraneDeskEmpty
	goto_if_set FLAG_PACT_HAYMARKET, Haymarket_CountingHouse_EventScript_SealedLetter
	sign Haymarket_Text_CraneDeskBefore
	end

Haymarket_CountingHouse_EventScript_SealedLetter::
	narrate Haymarket_Text_OfficeClearedOut
	narrate Haymarket_Text_LetterOnDesk
	setspeaker SP_NAME_NONE
	giveitem ITEM_SEALED_LETTER
	setflag FLAG_RECEIVED_SEALED_LETTER
	narrate Haymarket_Text_DontOpenIt
	releaseall
	end

Haymarket_CountingHouse_EventScript_CraneDeskEmpty::
	sign Haymarket_Text_CraneDeskEmpty
	end

@ ---------------------------------------------------------------------------
@ Signs
@ ---------------------------------------------------------------------------
Haymarket_EventScript_TownSign::
	sign Haymarket_Text_TownSign
	end

Haymarket_EventScript_PavilionSign::
	sign Haymarket_Text_PavilionSign
	end

Haymarket_EventScript_CountingHouseSign::
	sign Haymarket_Text_CountingHouseSign
	end

Haymarket_EventScript_GranarySign::
	sign Haymarket_Text_GranarySign
	end

Haymarket_EventScript_MagistrateSign::
	sign Haymarket_Text_MagistrateSign
	end

@ ---------------------------------------------------------------------------
@ Text
@ ---------------------------------------------------------------------------

Haymarket_Text_FirstLook::
	.string "HAYMARKET. Bunting on every roof. Music\n"
	.string "from the fairground. The smell of hot\l"
	.string "bread and roasted nuts.\p"
	.string "Over by the egg stall, something is\n"
	.string "wrong.$"

Haymarket_Text_ClerkTwoFines::
	.string "WIDOW PENN. That's two fines unpaid.\n"
	.string "Two fines, for “disorderly complaint”.$"

Haymarket_Text_PennArithmetic::
	.string "I complained that your scales are\n"
	.string "wrong! That isn't disorder, it's\l"
	.string "arithmetic!$"

Haymarket_Text_ClerkPaymentInKind::
	.string "Here's some more arithmetic, then. Two\n"
	.string "fines, one POKéMON.\p"
	.string "The guild will take payment in kind.$"

Haymarket_Text_PennPlease::
	.string "No! Please! Not DAISY! She's old, she's\n"
	.string "no use to you!$"

Haymarket_Text_Clerk2Sold::
	.string "She's use to us if she's sold.$"

Haymarket_Text_PennGrabsSleeve::
	.string "The clerk reaches under the stall.\n"
	.string "WIDOW PENN grabs his sleeve.\p"
	.string "He shakes her off.$"

Haymarket_Text_CrowdLooksAway::
	.string "All around the square, people look at\n"
	.string "their shoes. At the bunting. At\l"
	.string "anything else.$"

Haymarket_Text_PennThirtyYears::
	.string "...Thirty years I've sold eggs in this\n"
	.string "square.$"

Haymarket_Text_CorwinPumpkins::
	.string "Friends! Neighbours! Valued customers!\n"
	.string "Who's ready for the PUMPKIN WEIGH-OFF?$"

Haymarket_Text_CrowdHurrah::
	.string "Hurrah! PRINCE CORWIN!$"

Haymarket_Text_CorwinFrog_M::
	.string "Wait, wait, hold the pumpkins. Look who's\n"
	.string "here!\p"
	.string "Everybody! It's the MARSH PRINCE! Has\n"
	.string "LOWMERE sent us its finest frog?$"

Haymarket_Text_CorwinFrog_F::
	.string "Wait, wait, hold the pumpkins. Look who's\n"
	.string "here!\p"
	.string "Everybody! It's the MARSH PRINCESS!\n"
	.string "Has LOWMERE sent us its finest frog?$"

Haymarket_Text_CrowdLaughs::
	.string "The crowd laughs. Somebody croaks.$"

Haymarket_Text_CorwinBringMoney_M::
	.string "What brings you up out of the puddle,\n"
	.string "little brother? Come to buy grain?\p"
	.string "We've got plenty! Bring money.$"

Haymarket_Text_CorwinBringMoney_F::
	.string "What brings you up out of the puddle,\n"
	.string "little sister? Come to buy grain?\p"
	.string "We've got plenty! Bring money.$"

Haymarket_Text_YouTellCorwin::
	.string "You tell CORWIN about the tithe wagon\n"
	.string "on the MIRE ROAD. About WIDOW PENN, a\l"
	.string "minute ago, in front of everyone.$"

Haymarket_Text_CorwinMarketsHavePrices::
	.string "Oh, don't start. The GILDED SCALE runs a\n"
	.string "market. Markets have prices. If LOWMERE\l"
	.string "can't pay them, that's LOWMERE's\l"
	.string "problem.\p"
	.string "And PENN knew the rules. Everyone\n"
	.string "knows the rules.$"

Haymarket_Text_CorwinOnWhatGrounds::
	.string "A TRIAL OF STANDING? Against me? On\n"
	.string "what grounds? That you're poor?\p"
	.string "No magistrate in HAYMARKET would grant\n"
	.string "it. And without a magistrate's writ, my\l"
	.string "doors stay shut.$"

Haymarket_Text_CorwinEnjoyTheFair::
	.string "Enjoy the fair! Try the nuts. They're\n"
	.string "overpriced.$"

Haymarket_Text_TamsinHeardThat::
	.string "I heard all that. I wanted to throw a\n"
	.string "pumpkin at him.$"

Haymarket_Text_TamsinWeNeedProof::
	.string "He's right about one thing, though. We\n"
	.string "need the MAGISTRATE. And a magistrate\l"
	.string "needs proof.\p"
	.string "We've got the receipt. The carter said\n"
	.string "the scales are rigged, so let's see them\l"
	.string "for ourselves. Try the GRANARY.\p"
	.string "And that old woman would make a good\n"
	.string "witness, if anyone would listen to her.$"

Haymarket_Text_TamsinHintWeights::
	.string "The carter said the guild keeps its\n"
	.string "scales in the GRANARY. If the weights\l"
	.string "are wrong, we need to see them.$"

Haymarket_Text_TamsinHintWitness::
	.string "WIDOW PENN saw everything. If anyone in\n"
	.string "this town will stand up and say so, it's\l"
	.string "her.$"

Haymarket_Text_TamsinHintMagistrate::
	.string "Receipt, weights, witness. That's a\n"
	.string "case. Take it to the MAGISTRATE'S HALL.$"

Haymarket_Text_TamsinGoWin::
	.string "A writ! An actual writ!\p"
	.string "Go on, then. Go and knock the smile off\n"
	.string "his face. I'll be in the gallery,\l"
	.string "shouting.$"

Haymarket_Text_TamsinAfterPact::
	.string "I'll go home ahead of the carts and tell\n"
	.string "TOFT. I want to see his face.$"

Haymarket_Text_PennNotUntilDaisy::
	.string "Testify? Against the guild? They've\n"
	.string "already taken everything I've got.\p"
	.string "...No. Not until I have DAISY back.\p"
	.string "They keep the “payments” in the back\n"
	.string "room of the COUNTING HOUSE, until they\l"
	.string "can sell them on.$"

Haymarket_Text_HandPennBall::
	.string "You give WIDOW PENN the POKé BALL. Her\n"
	.string "hands are shaking as she opens it.$"

Haymarket_Text_PennMySillyGirl::
	.string "Oh, my girl. My silly old girl.$"

Haymarket_Text_PennIllTestify::
	.string "All right, Your Highness. I'll tell the\n"
	.string "MAGISTRATE everything. The scales, the\l"
	.string "fines, all of it.\p"
	.string "And I'll tell it loud.$"

Haymarket_Text_PennWillTestify::
	.string "I'll say it to the MAGISTRATE, and I'll\n"
	.string "say it to anyone else who asks. Go on.$"

Haymarket_Text_PennEggs::
	.string "Eggs, Your Highness? Fair weight. I\n"
	.string "promise.$"

Haymarket_Text_FarmerFair::
	.string "PRINCE CORWIN pays for the fair out of\n"
	.string "his own pocket, you know! Well.\l"
	.string "Somebody's pocket.$"

Haymarket_Text_FarmerLoaf::
	.string "Honest weights! I bought a loaf this\n"
	.string "morning, and it was the size of a loaf!$"

Haymarket_Text_GrannyGoodBoy::
	.string "Prices up, wages down. It's that GILDED\n"
	.string "SCALE.\p"
	.string "The prince is a good boy. He just\n"
	.string "doesn't see it.$"

Haymarket_Text_GrannyKnew::
	.string "They say the prince knew about the\n"
	.string "scales all along. Signed for them\l"
	.string "himself.\p"
	.string "...I suppose I knew that. I just liked\n"
	.string "the fairs.$"

Haymarket_Text_RibbonTax::
	.string "I sell ribbons. The guild charges me a\n"
	.string "tax on ribbons. What for? “Scale\l"
	.string "maintenance.”\p"
	.string "I don't use a scale! I sell ribbons!$"

Haymarket_Text_RibbonNoTax::
	.string "No more scale tax! I've celebrated by\n"
	.string "buying a ribbon. From myself. At cost.$"

Haymarket_Text_GirlPumpkin::
	.string "Did you see the PUMPKIN WEIGH-OFF? The\n"
	.string "guild's pumpkin won.\p"
	.string "It always wins. Funny, that.$"

Haymarket_Text_GirlPumpkinLost::
	.string "They weighed the pumpkins again on the\n"
	.string "CROWN's scales. The guild's came fourth!$"

Haymarket_Text_BoySweet::
	.string "PRINCE CORWIN gave me a sweet once! I\n"
	.string "love PRINCE CORWIN!$"

Haymarket_Text_BoyStillLoveCorwin::
	.string "Mum says PRINCE CORWIN did a bad thing.\n"
	.string "He still gave me a sweet, though.$"

Haymarket_Text_DontStare::
	.string "Don't stare at the COUNTING HOUSE.\n"
	.string "People who stare at the COUNTING HOUSE\l"
	.string "get fined.$"

Haymarket_Text_StareAllYouLike::
	.string "I've been staring at the COUNTING\n"
	.string "HOUSE all morning. Nobody's fined me.\l"
	.string "It's wonderful.$"

Haymarket_Text_GuardClosed::
	.string "The GILT PAVILION is closed to\n"
	.string "petitioners. His Highness is receiving\l"
	.string "bidders only.$"

Haymarket_Text_GuardOpen::
	.string "A writ from the MAGISTRATE? ...So it is.\p"
	.string "Well. You'd better go in, then. The whole\n"
	.string "town will want to watch.$"

Haymarket_Text_CraneWhatAPleasure::
	.string "Your Highness! SILAS CRANE, master of\n"
	.string "the GILDED SCALE. What a pleasure. What\l"
	.string "a surprise.\p"
	.string "I do like surprises. When they're small.$"

Haymarket_Text_CraneBlessHim::
	.string "I hear you've been to see old ARDEN.\n"
	.string "Bless him. Thirty-one years in that\l"
	.string "chair, and he still believes in\l"
	.string "paperwork.$"

Haymarket_Text_CraneOffer::
	.string "Let me save us both some trouble.\n"
	.string "LOWMERE needs grain. I have grain.\p"
	.string "Withdraw your little writ, and I'll sell\n"
	.string "you all you need at a royal discount.\l"
	.string "Half price. Fair weight. Signed today.\p"
	.string "Will you withdraw the writ?$"

Haymarket_Text_YouThinkOfToft::
	.string "You think of TOFT's ledger. You think of\n"
	.string "WIDOW PENN's hands, shaking as she\l"
	.string "opened the POKé BALL.$"

Haymarket_Text_YouCantDoIt::
	.string "...You can't do it.$"

Haymarket_Text_CranePity::
	.string "No? A pity. It was a good price. It won't\n"
	.string "be offered again.$"

Haymarket_Text_CraneRestWell_M::
	.string "Then rest well tonight, little prince.\p"
	.string "HAYMARKET can be so noisy after dark.$"

Haymarket_Text_CraneRestWell_F::
	.string "Then rest well tonight, little princess.\p"
	.string "HAYMARKET can be so noisy after dark.$"

Haymarket_Text_CraneWalksAway::
	.string "He tips his hat and walks away. For a\n"
	.string "moment, under his collar, you catch a\l"
	.string "flash of something white.$"

Haymarket_Text_ArdenComplaint::
	.string "A complaint against PRINCE CORWIN?\n"
	.string "Your Highness, I don't doubt you. Doubt\l"
	.string "isn't the problem. Proof is.$"

Haymarket_Text_ArdenThreeThings::
	.string "Bring me something on paper. Bring me\n"
	.string "the scales, or whatever's wrong with\l"
	.string "them. And bring me a person who will\l"
	.string "stand up in this room and say it.\p"
	.string "Then we'll talk.$"

Haymarket_Text_HaveReceipt::
	.string "You have the BELL RECEIPT from the\n"
	.string "MIRE ROAD.$"

Haymarket_Text_HaveWeights::
	.string "You have seen the false weights in the\n"
	.string "GRANARY.$"

Haymarket_Text_HaveWitness::
	.string "WIDOW PENN has agreed to testify.$"

Haymarket_Text_ArdenLetMeSee::
	.string "Let me see.\p"
	.string "A tithe receipt. LOWMERE's own grain,\n"
	.string "delivered to the guild. Sealed with...\l"
	.string "what is this? A bell?\p"
	.string "The CROWN's weights, locked in a crate\n"
	.string "while lighter ones do the work. And a\l"
	.string "witness who will say so under oath.$"

Haymarket_Text_ArdenThirtyOneYears::
	.string "I have sat in this chair for thirty-one\n"
	.string "years, Your Highness. I've watched the\l"
	.string "guild buy this town, one fine at a time.\p"
	.string "I told myself I was too old to fight it.$"

Haymarket_Text_ArdenTheLawIsOld::
	.string "The law is old, Your Highness, but it is\n"
	.string "still the law.\p"
	.string "I authorise a TRIAL OF STANDING\n"
	.string "between {PLAYER} of LOWMERE and CORWIN\l"
	.string "of HAYMARKET.$"

Haymarket_Text_StampsWrit::
	.string "The MAGISTRATE stamps the writ. The\n"
	.string "sound echoes around the empty hall.$"

Haymarket_Text_ArdenWatchYourBack::
	.string "Take it to the GILT PAVILION. The\n"
	.string "guards will have to open the doors for\l"
	.string "you.\p"
	.string "And, Your Highness... watch your back in\n"
	.string "the square.$"

Haymarket_Text_ArdenGoOn::
	.string "The writ is signed. Go on, before I come\n"
	.string "to my senses.$"

Haymarket_Text_ArdenAfterPact::
	.string "Honest weights in the market, from\n"
	.string "today. I had them checked twice.\p"
	.string "I'd forgotten how much I liked this job.$"

Haymarket_Text_CourtClerk::
	.string "The MAGISTRATE hears petitions every\n"
	.string "day. Nobody's petitioned in two years.\l"
	.string "Funny, that.$"

Haymarket_Text_ForemanIntro::
	.string "Oi! The scale room is guild property.\n"
	.string "Nobody goes in without a chit from\l"
	.string "MASTER CRANE!$"

Haymarket_Text_ForemanDefeat::
	.string "That's not a chit!$"

Haymarket_Text_ForemanPostBattle::
	.string "Go on, then. Look all you like. They're\n"
	.string "just weights.$"

Haymarket_Text_ScaleBrass::
	.string "A great iron scale. On it sit shiny brass\n"
	.string "weights, stamped with the GILDED\l"
	.string "SCALE's mark.\p"
	.string "You lift one. It is much too light.$"

Haymarket_Text_ScaleCompare::
	.string "You set it on one pan and an old CROWN\n"
	.string "weight from the crate in the corner on\l"
	.string "the other.\p"
	.string "The scale drops hard toward the CROWN\n"
	.string "weight. The guild's “hundredweight” is\l"
	.string "short by a quarter.$"

Haymarket_Text_FoundWeights::
	.string "That's proof of the false weights.$"

Haymarket_Text_ScaleAgain::
	.string "The brass weights sit on the scale,\n"
	.string "shiny and light.$"

Haymarket_Text_CrownCrate::
	.string "A crate stamped with the CROWN's mark.\n"
	.string "Inside are iron weights, black with age.\p"
	.string "CROWN STANDARD. ONE HUNDREDWEIGHT.$"

Haymarket_Text_GranarySacks::
	.string "Grain sacks stacked to the rafters.\n"
	.string "Some of them are stamped LOWMERE.$"

Haymarket_Text_GranaryWorker::
	.string "I just carry the sacks. I don't weigh\n"
	.string "them. I don't want to know what they\l"
	.string "weigh.$"

Haymarket_Text_FrontClerk::
	.string "Welcome to the GILDED SCALE. Loans,\n"
	.string "fines, and payments. All payments are\l"
	.string "final.$"

Haymarket_Text_FrontClerkAfter::
	.string "MASTER CRANE? He's... away. On business.\n"
	.string "Please stop asking.$"

Haymarket_Text_Clerk1Intro::
	.string "This room is for staff and payments\n"
	.string "only. You don't look like staff.$"

Haymarket_Text_Clerk1Defeat::
	.string "I'm going to have to fine myself for\n"
	.string "that.$"

Haymarket_Text_Clerk1PostBattle::
	.string "We don't keep anything here we didn't\n"
	.string "take legally. Well. Legally enough.$"

Haymarket_Text_Clerk2Intro::
	.string "Royal or not, nobody walks out of the\n"
	.string "COUNTING HOUSE with the goods!$"

Haymarket_Text_Clerk2Defeat::
	.string "Fine, fine! Take what you came for!$"

Haymarket_Text_Clerk2PostBattle::
	.string "Every BALL on those shelves belonged\n"
	.string "to somebody who couldn't pay. Don't\l"
	.string "look at me like that. I just do the\l"
	.string "labels.$"

Haymarket_Text_PennBallLabel::
	.string "A POKé BALL sits on the shelf, with a\n"
	.string "label tied around it: PENN. 2 FINES. FOR\l"
	.string "RESALE.$"

Haymarket_Text_TookPennBall::
	.string "{PLAYER} took WIDOW PENN's POKé BALL!$"

Haymarket_Text_PaymentShelves::
	.string "Rows and rows of POKé BALLS, each one\n"
	.string "labelled with a name and a fine.\p"
	.string "There are dozens of them.$"

Haymarket_Text_CraneDeskBefore::
	.string "A big desk covered with ledgers. Every\n"
	.string "column adds up perfectly.\p"
	.string "That's what's wrong with them.$"

Haymarket_Text_OfficeClearedOut::
	.string "The office has been cleared out in a\n"
	.string "hurry. The ledgers are gone, and the\l"
	.string "drawers hang open.$"

Haymarket_Text_LetterOnDesk::
	.string "On the desk is a single envelope. It is\n"
	.string "addressed to no one. The seal is a pale\l"
	.string "bell, pressed so hard the wax has\l"
	.string "cracked.$"

Haymarket_Text_DontOpenIt::
	.string "Something tells you not to open it here.\n"
	.string "Whoever it was meant for will come\l"
	.string "looking for it.$"

Haymarket_Text_CraneDeskEmpty::
	.string "The desk is empty. Even the ink pot is\n"
	.string "gone.$"

Haymarket_Text_TownSign::
	.string "HAYMARKET\n"
	.string "Market town of the Crown.\l"
	.string "Honest weight since forever.$"

Haymarket_Text_PavilionSign::
	.string "THE GILT PAVILION\n"
	.string "Seat of PRINCE CORWIN.\l"
	.string "Bidding on the hour.$"

Haymarket_Text_CountingHouseSign::
	.string "GILDED SCALE COUNTING HOUSE\n"
	.string "All payments final.$"

Haymarket_Text_GranarySign::
	.string "HAYMARKET GRANARY\n"
	.string "Weighing by appointment only.$"

Haymarket_Text_MagistrateSign::
	.string "MAGISTRATE'S HALL\n"
	.string "Petitions heard daily.$"

