@ Slice scene 6: the Mire Road (route between Lowmere and Haymarket).
@ Object, trigger and sign placement: docs/dialogue/events.md#mire-road

#ifndef LOCALID_MIRE_ROAD_TAMSIN
#define LOCALID_MIRE_ROAD_TAMSIN 1
#endif
#ifndef LOCALID_MIRE_ROAD_TAMSIN_WAGON
#define LOCALID_MIRE_ROAD_TAMSIN_WAGON 4
#endif
#ifndef LOCALID_MIRE_ROAD_WAGON_CLERK
#define LOCALID_MIRE_ROAD_WAGON_CLERK 2
#endif
#ifndef LOCALID_MIRE_ROAD_WAGON_CLERK_2
#define LOCALID_MIRE_ROAD_WAGON_CLERK_2 3
#endif

@ ---------------------------------------------------------------------------
@ Tamsin at the foot of the road. Coord trigger on the first steps of the
@ route while VAR_SLICE_STATE is 7 and she hasn't spoken yet. She then goes on
@ ahead; a second Tamsin object waits by the wagon.
@ ---------------------------------------------------------------------------
MireRoad_EventScript_TamsinJoins::
	lockall
	goto_if_set FLAG_TAMSIN_MIRE_ROAD_TALK, MireRoad_EventScript_TamsinJoinsDone
	applymovement LOCALID_MIRE_ROAD_TAMSIN, Common_Movement_FacePlayer
	waitmovement 0
	say Slice_Name_Tamsin, MireRoad_Text_TamsinWhatWasInLetter
	narrate MireRoad_Text_ShowTamsinBall
	say Slice_Name_Tamsin, MireRoad_Text_TamsinYourFather
	say Slice_Name_Tamsin, MireRoad_Text_TamsinBeforeDark
	closemessage
	setflag FLAG_TAMSIN_MIRE_ROAD_TALK
	fadescreen FADE_TO_BLACK
	removeobject LOCALID_MIRE_ROAD_TAMSIN
	setflag FLAG_HIDE_MIRE_ROAD_TAMSIN
	fadescreen FADE_FROM_BLACK
	releaseall
	end

MireRoad_EventScript_TamsinJoinsDone::
	releaseall
	end

@ Tamsin talked to directly before the trigger fires.
MireRoad_EventScript_Tamsin::
	lockall
	goto MireRoad_EventScript_TamsinJoins
	end

@ ---------------------------------------------------------------------------
@ The tithe wagon (state 7 -> 8). Coord trigger in front of the wagon, or
@ talking to the first clerk. Tamsin is already there, looking at the sacks.
@ ---------------------------------------------------------------------------
MireRoad_EventScript_WagonClerk::
	lockall
	goto_if_ge VAR_SLICE_STATE, 8, MireRoad_EventScript_WagonDone
	narrate MireRoad_Text_WagonStuck
	say Slice_Name_Tamsin, MireRoad_Text_TamsinLookAtSacks
	narrate MireRoad_Text_SacksStamped
	say Slice_Name_Tamsin, MireRoad_Text_TamsinThatsOurTithe
	applymovement LOCALID_MIRE_ROAD_WAGON_CLERK, Common_Movement_FacePlayer
	waitmovement 0
	say Slice_Name_Clerk, MireRoad_Text_ClerkFairAndSquare
	say Slice_Name_Clerk2, MireRoad_Text_Clerk2ToLowmere
	applymovement LOCALID_MIRE_ROAD_WAGON_CLERK, Common_Movement_ExclamationMark
	waitmovement 0
	say_g Slice_Name_Clerk, MireRoad_Text_ClerkBastard_M, MireRoad_Text_ClerkBastard_F
	closemessage
	trainerbattle_no_intro TRAINER_MIRE_ROAD_WAGON_CLERK, MireRoad_Text_WagonClerkDefeated
	say Slice_Name_Clerk, MireRoad_Text_ClerkGuildWillHear
	say Slice_Name_Clerk2, MireRoad_Text_Clerk2Run
	closemessage
	fadescreen FADE_TO_BLACK
	removeobject LOCALID_MIRE_ROAD_WAGON_CLERK
	removeobject LOCALID_MIRE_ROAD_WAGON_CLERK_2
	setflag FLAG_HIDE_MIRE_ROAD_WAGON_CLERKS
	fadescreen FADE_FROM_BLACK
	narrate MireRoad_Text_DroppedSlip
	narrate MireRoad_Text_ReceiptDetails
	setspeaker SP_NAME_NONE
	giveitem ITEM_BELL_RECEIPT
	setflag FLAG_FOUND_BELL_RECEIPT
	applymovement LOCALID_MIRE_ROAD_TAMSIN_WAGON, Common_Movement_FacePlayer
	waitmovement 0
	say Slice_Name_Tamsin, MireRoad_Text_TamsinBell
	say Slice_Name_Tamsin, MireRoad_Text_TamsinWhoIsBuying
	closemessage
	fadescreen FADE_TO_BLACK
	removeobject LOCALID_MIRE_ROAD_TAMSIN_WAGON
	setflag FLAG_HIDE_MIRE_ROAD_TAMSIN_WAGON
	setvar VAR_SLICE_STATE, 8
	fadescreen FADE_FROM_BLACK
	releaseall
	end

MireRoad_EventScript_WagonDone::
	releaseall
	end

@ The wagon itself (object or bg event).
MireRoad_EventScript_Wagon::
	goto_if_set FLAG_FOUND_BELL_RECEIPT, MireRoad_EventScript_WagonAfter
	sign MireRoad_Text_WagonBefore
	end

MireRoad_EventScript_WagonAfter::
	sign MireRoad_Text_WagonAfter
	end

@ ---------------------------------------------------------------------------
@ Route trainers
@ ---------------------------------------------------------------------------
MireRoad_EventScript_Clerk::
	trainerbattle_single TRAINER_MIRE_ROAD_CLERK, MireRoad_Text_ClerkIntro, MireRoad_Text_ClerkDefeat
	npc Slice_Name_Clerk, MireRoad_Text_ClerkPostBattle
	end

MireRoad_EventScript_Poacher::
	trainerbattle_single TRAINER_MIRE_ROAD_POACHER, MireRoad_Text_PoacherIntro, MireRoad_Text_PoacherDefeat
	npc SP_NAME_NONE, MireRoad_Text_PoacherPostBattle
	end

MireRoad_EventScript_Youngster::
	checkplayergender
	goto_if_eq VAR_RESULT, FEMALE, MireRoad_EventScript_YoungsterFemale
	trainerbattle_single TRAINER_MIRE_ROAD_YOUNGSTER, MireRoad_Text_YoungsterIntro_M, MireRoad_Text_YoungsterDefeat
	npc SP_NAME_NONE, MireRoad_Text_YoungsterPostBattle_M
	end

MireRoad_EventScript_YoungsterFemale::
	trainerbattle_single TRAINER_MIRE_ROAD_YOUNGSTER, MireRoad_Text_YoungsterIntro_F, MireRoad_Text_YoungsterDefeat
	npc SP_NAME_NONE, MireRoad_Text_YoungsterPostBattle_F
	end

@ The carter quit the guild over the scales. After the battle he gives the
@ first lead on the false weights.
MireRoad_EventScript_Carter::
	trainerbattle_single TRAINER_MIRE_ROAD_CARTER, MireRoad_Text_CarterIntro, MireRoad_Text_CarterDefeat
	lock
	faceplayer
	setspeaker Slice_Name_Carter
	goto_if_set FLAG_TALKED_TO_CARTER, MireRoad_EventScript_CarterAgain
	msgbox MireRoad_Text_CarterNineYears, MSGBOX_DEFAULT
	setspeaker Slice_Name_Carter
	msgbox MireRoad_Text_CarterWeights, MSGBOX_DEFAULT
	setspeaker Slice_Name_Carter
	msgbox MireRoad_Text_CarterSums, MSGBOX_DEFAULT
	setspeaker Slice_Name_Carter
	msgbox MireRoad_Text_CarterFined, MSGBOX_DEFAULT
	setflag FLAG_TALKED_TO_CARTER
	release
	end

MireRoad_EventScript_CarterAgain::
	msgbox MireRoad_Text_CarterAgain, MSGBOX_DEFAULT
	release
	end

MireRoad_EventScript_RouteSign::
	sign MireRoad_Text_RouteSign
	end

@ ---------------------------------------------------------------------------
@ Text
@ ---------------------------------------------------------------------------

MireRoad_Text_TamsinWhatWasInLetter::
	.string "Took you long enough. Well? What was in\n"
	.string "the letter?$"

MireRoad_Text_ShowTamsinBall::
	.string "You show TAMSIN the letter, and the\n"
	.string "king's gift.$"

MireRoad_Text_TamsinYourFather::
	.string "...Your father sent you a LEGEND? The\n"
	.string "king? The actual king?\p"
	.string "And they say he's lost his mind. I'd like\n"
	.string "to lose mine like that.$"

MireRoad_Text_TamsinBeforeDark::
	.string "Come on. If we reach HAYMARKET before\n"
	.string "dark, we might find somewhere to sleep\l"
	.string "that hasn't got frogs in it.\p"
	.string "I'll scout ahead. Try to keep up, Your\n"
	.string "Highness.$"

MireRoad_Text_WagonStuck::
	.string "A heavy wagon is stuck to its axles in\n"
	.string "the mud. Two clerks in GILDED SCALE\l"
	.string "yellow are arguing over which of them\l"
	.string "should push.$"

MireRoad_Text_TamsinLookAtSacks::
	.string "{PLAYER}. Look at the sacks. Look at the\n"
	.string "stamp on them.$"

MireRoad_Text_SacksStamped::
	.string "Every sack is stamped with the same\n"
	.string "word: LOWMERE.$"

MireRoad_Text_TamsinThatsOurTithe::
	.string "That's our tithe. That's the grain the\n"
	.string "guild took off the marsh farms in the\l"
	.string "spring.\p"
	.string "They told us it was going to the Crown\n"
	.string "granary.$"

MireRoad_Text_ClerkFairAndSquare::
	.string "Collected fair and square, miss. Now it's\n"
	.string "going to HAYMARKET, where it'll be\l"
	.string "weighed fair and square, and sold fair\l"
	.string "and square.$"

MireRoad_Text_Clerk2ToLowmere::
	.string "Sold to LOWMERE, probably! Ha!$"

MireRoad_Text_ClerkBastard_M::
	.string "Move along, marsh rats. This is guild\n"
	.string "business.\p"
	.string "...Hang on. I know that face. You're the\n"
	.string "bastard prince.\p"
	.string "HAYMARKET doesn't take IOUs, Your\n"
	.string "Highness, and neither do I!$"

MireRoad_Text_ClerkBastard_F::
	.string "Move along, marsh rats. This is guild\n"
	.string "business.\p"
	.string "...Hang on. I know that face. You're the\n"
	.string "bastard princess.\p"
	.string "HAYMARKET doesn't take IOUs, Your\n"
	.string "Highness, and neither do I!$"

MireRoad_Text_WagonClerkDefeated::
	.string "The guild will hear about this!$"

MireRoad_Text_ClerkGuildWillHear::
	.string "MASTER CRANE will hear about this!\n"
	.string "Every word!$"

MireRoad_Text_Clerk2Run::
	.string "Leave the wagon! It isn't worth it! Run!$"

MireRoad_Text_DroppedSlip::
	.string "In their hurry, one of the clerks has\n"
	.string "dropped a slip of paper in the mud.$"

MireRoad_Text_ReceiptDetails::
	.string "It's a receipt. LOWMERE TITHE, 40 SACKS.\n"
	.string "DELIVERED TO THE GILDED SCALE,\l"
	.string "HAYMARKET.\p"
	.string "At the bottom is a wax seal. A pale bell,\n"
	.string "with no clapper.$"

MireRoad_Text_TamsinBell::
	.string "A bell? The guild's mark is a pair of\n"
	.string "scales. Everybody knows that.\p"
	.string "Keep it. Anything people drop while\n"
	.string "they're running away is worth keeping.$"

MireRoad_Text_TamsinWhoIsBuying::
	.string "We can't drag a wagon out of the mud\n"
	.string "with two POKéMON and a bad temper.\p"
	.string "Let's go and find out who's buying it.$"

MireRoad_Text_WagonBefore::
	.string "The wagon is piled high with grain\n"
	.string "sacks. Every one is stamped LOWMERE.$"

MireRoad_Text_WagonAfter::
	.string "The abandoned wagon is sinking slowly\n"
	.string "into the mud. Every sack is stamped\l"
	.string "LOWMERE.$"

MireRoad_Text_ClerkIntro::
	.string "Toll inspection! The GILDED SCALE\n"
	.string "keeps this road safe for honest trade.\p"
	.string "Pay up, or... oh, you've got POKéMON.\n"
	.string "HAYMARKET doesn't take IOUs, friend,\l"
	.string "and neither do I!$"

MireRoad_Text_ClerkDefeat::
	.string "Fine! Pass! Free of charge, just this\n"
	.string "once!$"

MireRoad_Text_ClerkPostBattle::
	.string "The guild runs everything between here\n"
	.string "and the capital. You'll see.$"

MireRoad_Text_PoacherIntro::
	.string "Shh! You'll scare off the catch!\p"
	.string "...Too late. Now you owe me a battle!$"

MireRoad_Text_PoacherDefeat::
	.string "Lost the catch and the battle!$"

MireRoad_Text_PoacherPostBattle::
	.string "Nobody watches the marsh any more. No\n"
	.string "warden, no RANGER. Not since the old\l"
	.string "RANGER retired.\p"
	.string "Who's going to stop me? The frogs?$"

MireRoad_Text_YoungsterIntro_M::
	.string "Is it true there's a real prince on this\n"
	.string "road? Mum says royals have gold teeth!\p"
	.string "Are you them? Show me your gold teeth!\n"
	.string "No? Then battle me!$"

MireRoad_Text_YoungsterIntro_F::
	.string "Is it true there's a real princess on\n"
	.string "this road? Mum says royals have gold\l"
	.string "teeth!\p"
	.string "Are you them? Show me your gold teeth!\n"
	.string "No? Then battle me!$"

MireRoad_Text_YoungsterDefeat::
	.string "You're really strong! Maybe you are\n"
	.string "royal!$"

MireRoad_Text_YoungsterPostBattle_M::
	.string "You don't look like a prince. You look\n"
	.string "like you fell in the marsh.\p"
	.string "Is that what a real one looks like? Wow.$"

MireRoad_Text_YoungsterPostBattle_F::
	.string "You don't look like a princess. You look\n"
	.string "like you fell in the marsh.\p"
	.string "Is that what a real one looks like? Wow.$"

MireRoad_Text_CarterIntro::
	.string "Going to HAYMARKET? Then you'd better\n"
	.string "get used to losing money.\p"
	.string "Let me give you some practice!$"

MireRoad_Text_CarterDefeat::
	.string "Well, I've lost worse. I lost my job.$"

MireRoad_Text_CarterNineYears::
	.string "I hauled grain for the GILDED SCALE for\n"
	.string "nine years. Quit in the spring.$"

MireRoad_Text_CarterWeights::
	.string "They swapped the weights in the market.\n"
	.string "The old CROWN weights, the honest iron\l"
	.string "ones, went into a crate. New brass ones\l"
	.string "came out. Pretty. And light.$"

MireRoad_Text_CarterSums::
	.string "A sack that weighs a hundredweight on\n"
	.string "the CROWN's weights only weighs three\l"
	.string "quarters on the guild's.\p"
	.string "The guild buys by the light weights and\n"
	.string "sells by the honest ones. Do the sums.$"

MireRoad_Text_CarterFined::
	.string "I said so, out loud, in the market\n"
	.string "square. The next day I had a fine for\l"
	.string "“slandering the market”.\p"
	.string "Signed by PRINCE CORWIN himself.$"

MireRoad_Text_CarterAgain::
	.string "If you want proof, look in the\n"
	.string "HAYMARKET GRANARY. That's where they\l"
	.string "keep the scales.$"

MireRoad_Text_RouteSign::
	.string "MIRE ROAD\p"
	.string "North: HAYMARKET\n"
	.string "South: LOWMERE$"

