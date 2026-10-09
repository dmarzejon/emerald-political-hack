@ Chapter 3: Brannoc's Trial of Standing in the Pithead Hall, his palace (the
@ Rustboro Gym slot), and the Pact of the Open Shaft.
@ Object placement: docs/dialogue/chapter3.md#the-pithead-hall

@ ---------------------------------------------------------------------------
@ The herald at the door (the Gym guide's job).
@ ---------------------------------------------------------------------------
PitheadHall_EventScript_Herald::
	lock
	faceplayer
	setspeaker Slice_Name_Herald
	goto_if_set FLAG_BADGE03_GET, PitheadHall_EventScript_HeraldAfter
	msgbox PitheadHall_Text_HeraldWelcome, MSGBOX_DEFAULT
	setspeaker Slice_Name_Herald
	msgbox PitheadHall_Text_HeraldAdvice, MSGBOX_DEFAULT
	release
	end

PitheadHall_EventScript_HeraldAfter::
	msgbox PitheadHall_Text_HeraldAfter, MSGBOX_DEFAULT
	release
	end

@ ---------------------------------------------------------------------------
@ The court: the foreman and two free miners.
@ ---------------------------------------------------------------------------
PitheadHall_EventScript_Foreman::
	trainerbattle_single TRAINER_PITHEAD_FOREMAN, PitheadHall_Text_ForemanIntro, PitheadHall_Text_ForemanDefeat
	npc Slice_Name_Foreman, PitheadHall_Text_ForemanPostBattle
	end

PitheadHall_EventScript_Miner1::
	trainerbattle_single TRAINER_PITHEAD_MINER_1, PitheadHall_Text_Miner1Intro, PitheadHall_Text_Miner1Defeat
	npc Ch3_Name_Miner, PitheadHall_Text_Miner1PostBattle
	end

PitheadHall_EventScript_Miner2::
	trainerbattle_single TRAINER_PITHEAD_MINER_2, PitheadHall_Text_Miner2Intro, PitheadHall_Text_Miner2Defeat
	npc Ch3_Name_Miner, PitheadHall_Text_Miner2PostBattle
	end

@ ---------------------------------------------------------------------------
@ The canteen cook before the Trial: a full heal (balance assumes one).
@ ---------------------------------------------------------------------------
PitheadHall_EventScript_Cook::
	lock
	faceplayer
	say Ch3_Name_Cook, PitheadHall_Text_CookOffer
	closemessage
	call Common_EventScript_OutOfCenterPartyHeal
	npc Ch3_Name_Cook, PitheadHall_Text_CookDone
	end

@ ---------------------------------------------------------------------------
@ Brannoc. Trial 3. Win -> the Cragholt Pick (badge 3), the pact, and his
@ private talk about the quota, the collapse and Regirock
@ (VAR_CRAGHOLT_STATE 2 -> 3).
@ ---------------------------------------------------------------------------
PitheadHall_EventScript_Brannoc::
	lock
	faceplayer
	goto_if_set FLAG_BADGE03_GET, PitheadHall_EventScript_BrannocAfter
	say_g Ch3_Name_Brannoc, PitheadHall_Text_BrannocYouCame_M, PitheadHall_Text_BrannocYouCame_F
	narrate PitheadHall_Text_HollenReadsCase
	narrate PitheadHall_Text_WilSpeaks
	say Ch3_Name_Brannoc, PitheadHall_Text_BrannocTheyKnewTheRisk
	say Ch3_Name_Brannoc, PitheadHall_Text_BrannocLetTheTrialDecide
	closemessage
	trainerbattle_no_intro TRAINER_BRANNOC, PitheadHall_Text_BrannocDefeat
	goto PitheadHall_EventScript_BrannocDefeated
	end

PitheadHall_EventScript_BrannocDefeated::
	narrate PitheadHall_Text_Silence
	say Ch3_Name_Brannoc, PitheadHall_Text_BrannocYouLifted
	narrate PitheadHall_Text_HollenStepsForward
	say Ch3_Name_Hollen, PitheadHall_Text_PactTerms
	say Ch3_Name_Hollen, PitheadHall_Text_HollenFourthTerm
	setspeaker SP_NAME_NONE
	message PitheadHall_Text_ChooseTerms
	waitmessage
	dynmultichoice 0, 0, TRUE, 3, 0, DYN_MULTICHOICE_CB_NONE, PitheadHall_Text_TermsRestitution, PitheadHall_Text_TermsReform, PitheadHall_Text_TermsClemency
	@ Options are in PACT_TERMS_* order. signpact sets FLAG_PACT_CRAGHOLT and
	@ raises VAR_LOWMERE_STAGE when enough pacts are signed.
	signpact PACT_CRAGHOLT, VAR_RESULT
	getpactterms PACT_CRAGHOLT
	switch VAR_RESULT
	case PACT_TERMS_RESTITUTION, PitheadHall_EventScript_TermsRestitution
	case PACT_TERMS_REFORM, PitheadHall_EventScript_TermsReform
	goto PitheadHall_EventScript_TermsClemency
	end

PitheadHall_EventScript_TermsRestitution::
	say Ch3_Name_Hollen, PitheadHall_Text_HollenRestitution
	say Ch3_Name_Brannoc, PitheadHall_Text_BrannocRestitution
	goto PitheadHall_EventScript_SignPact
	end

PitheadHall_EventScript_TermsReform::
	say Ch3_Name_Hollen, PitheadHall_Text_HollenReform
	say Ch3_Name_Brannoc, PitheadHall_Text_BrannocReform
	goto PitheadHall_EventScript_SignPact
	end

PitheadHall_EventScript_TermsClemency::
	say Ch3_Name_Hollen, PitheadHall_Text_HollenClemency
	say Ch3_Name_Brannoc, PitheadHall_Text_BrannocClemency
	goto PitheadHall_EventScript_SignPact
	end

PitheadHall_EventScript_SignPact::
	narrate PitheadHall_Text_BrannocSigns
	say Ch3_Name_Brannoc, PitheadHall_Text_BrannocHereIsYourPick
	setspeaker SP_NAME_NONE
	message PitheadHall_Text_ReceivedPick
	waitmessage
	call Common_EventScript_PlayGymBadgeFanfare
	setflag FLAG_BADGE03_GET
	closemessage
	fadescreen FADE_TO_BLACK
	delay 30
	fadescreen FADE_FROM_BLACK
	narrate PitheadHall_Text_Later
	say Ch3_Name_Brannoc, PitheadHall_Text_BrannocNotSorry
	say Ch3_Name_Brannoc, PitheadHall_Text_BrannocIToldThem
	goto_if_unset FLAG_FOUND_BELL_CRATE, PitheadHall_EventScript_NoCrate
	narrate PitheadHall_Text_ShowCrate
	say Ch3_Name_Brannoc, PitheadHall_Text_BrannocTheGuild
	goto PitheadHall_EventScript_Regirock
	end

PitheadHall_EventScript_NoCrate::
	say Ch3_Name_Brannoc, PitheadHall_Text_BrannocSuspects
	goto PitheadHall_EventScript_Regirock
	end

PitheadHall_EventScript_Regirock::
	say Ch3_Name_Brannoc, PitheadHall_Text_BrannocRegirock
	closemessage
	showmonpic SPECIES_REGIROCK, 10, 3
	playmoncry SPECIES_REGIROCK, CRY_MODE_NORMAL
	waitmoncry
	hidemonpic
	say Ch3_Name_Brannoc, PitheadHall_Text_BrannocDugItOut
	say Ch3_Name_Brannoc, PitheadHall_Text_BrannocFirstCrew
	closemessage
	setvar VAR_CRAGHOLT_STATE, 3
	release
	end

PitheadHall_EventScript_BrannocAfter::
	say_g Ch3_Name_Brannoc, PitheadHall_Text_BrannocAfter_M, PitheadHall_Text_BrannocAfter_F
	release
	end

@ The ore seams in the hall walls (bg events).
PitheadHall_EventScript_Seam::
	goto_if_set FLAG_BADGE03_GET, PitheadHall_EventScript_SeamAfter
	sign PitheadHall_Text_SeamBefore
	end

PitheadHall_EventScript_SeamAfter::
	sign PitheadHall_Text_SeamAfter
	end

@ ---------------------------------------------------------------------------
@ Text
@ ---------------------------------------------------------------------------

PitheadHall_Text_HeraldWelcome::
	.string "Welcome to the PITHEAD HALL, seat of\n"
	.string "Prince BRANNOC.\p"
	.string "There are no carpets here. He says\n"
	.string "carpets are for people who don't work.$"

PitheadHall_Text_HeraldAdvice::
	.string "The prince fights with ROCK POKéMON.\n"
	.string "They shrug off normal blows like rain.\p"
	.string "WATER wears stone down. GRASS splits it\n"
	.string "from below. The court will test you on\l"
	.string "the way in.$"

PitheadHall_Text_HeraldAfter::
	.string "I've worked this door eight years. I\n"
	.string "never saw the prince lose before.\p"
	.string "He laughed. I never saw that either.$"

PitheadHall_Text_ForemanIntro::
	.string "I run the crews. I know what goes on\n"
	.string "down there. Do you?$"

PitheadHall_Text_ForemanDefeat::
	.string "Huh. You hit like a falling roof.$"

PitheadHall_Text_ForemanPostBattle::
	.string "I told him the props were too thin.\n"
	.string "Twice. He said the quota doesn't wait\l"
	.string "for timber.$"

PitheadHall_Text_Miner1Intro::
	.string "I'm a free miner. Paid, not bought! I've\n"
	.string "got no quarrel with you.\p"
	.string "But I've got a quarrel with losing.$"

PitheadHall_Text_Miner1Defeat::
	.string "Fair enough. You earned that.$"

PitheadHall_Text_Miner1PostBattle::
	.string "The bought men do the deep work. We do\n"
	.string "the safe work. That's how it's been.\p"
	.string "I didn't ask. I just took the safe work.$"

PitheadHall_Text_Miner2Intro::
	.string "My DRILBUR's dug more ore than most men\n"
	.string "in this town!$"

PitheadHall_Text_Miner2Defeat::
	.string "Back to the rock face, then.$"

PitheadHall_Text_Miner2PostBattle::
	.string "The prince goes down himself, you know.\n"
	.string "Every week. He's not soft.\p"
	.string "He just never goes down the east\n"
	.string "tunnel.$"

PitheadHall_Text_CookOffer::
	.string "Nobody fights the prince on an empty\n"
	.string "stomach. Sit. Eat.$"

PitheadHall_Text_CookDone::
	.string "There. Miner's stew. It'll put iron back\n"
	.string "in your POKéMON.$"

PitheadHall_Text_BrannocYouCame_M::
	.string "So, little brother. HOLLEN signed his\n"
	.string "name. I didn't think the old man had it\l"
	.string "in him.$"

PitheadHall_Text_BrannocYouCame_F::
	.string "So, little sister. HOLLEN signed his\n"
	.string "name. I didn't think the old man had it\l"
	.string "in him.$"

PitheadHall_Text_HollenReadsCase::
	.string "Magistrate HOLLEN reads out the case:\n"
	.string "the bonding roll, sixty-one names\l"
	.string "struck through, and the quota ledger.$"

PitheadHall_Text_WilSpeaks::
	.string "Then WIL PENROSE stands up in front of\n"
	.string "the prince and describes the east\l"
	.string "tunnel. He doesn't stop until he's\l"
	.string "finished.$"

PitheadHall_Text_BrannocTheyKnewTheRisk::
	.string "They were debtors and convicts. They'd\n"
	.string "have rotted in gaol. I gave them work.\p"
	.string "Every man in this town goes down that\n"
	.string "shaft. I go down that shaft.$"

PitheadHall_Text_BrannocLetTheTrialDecide::
	.string "Words don't move stone. Show me what\n"
	.string "you can lift.$"

PitheadHall_Text_BrannocDefeat::
	.string "...Hah. HAH! Well. That's lifted.$"

PitheadHall_Text_Silence::
	.string "The hall is silent. Somewhere outside,\n"
	.string "the lift bell rings once and stops.$"

PitheadHall_Text_BrannocYouLifted::
	.string "I told you to prove you could lift\n"
	.string "something. You lifted the whole\l"
	.string "mountain off me.\p"
	.string "Say your terms. I'll hear them standing.$"

PitheadHall_Text_HollenStepsForward::
	.string "Magistrate HOLLEN steps forward with\n"
	.string "the pact.$"

PitheadHall_Text_PactTerms::
	.string "By the law of the Trial: every bonded\n"
	.string "worker in CRAGHOLT is freed, and paid\l"
	.string "for every day they worked.\p"
	.string "CRAGHOLT buys no more labour, from\n"
	.string "anyone. And CRAGHOLT sends stone and\l"
	.string "masons to LOWMERE.$"

PitheadHall_Text_HollenFourthTerm::
	.string "The law allows the victor one term of\n"
	.string "their own. What will you add?$"

PitheadHall_Text_ChooseTerms::
	.string "Choose the fourth term.$"

PitheadHall_Text_TermsRestitution::
	.string "Pay the families$"

PitheadHall_Text_TermsReform::
	.string "Refuse the quotas$"

PitheadHall_Text_TermsClemency::
	.string "Call it an accident$"

PitheadHall_Text_HollenRestitution::
	.string "Prince BRANNOC pays the family of every\n"
	.string "man struck from the roll, from his own\l"
	.string "bonuses. And sends LOWMERE double\l"
	.string "stone.$"

PitheadHall_Text_BrannocRestitution::
	.string "Every coin. I'll carry it to their doors\n"
	.string "myself, if I have to walk to THORNFIELD.$"

PitheadHall_Text_HollenReform::
	.string "CRAGHOLT refuses all further quotas\n"
	.string "from the CHANCELLERY. And the miners\l"
	.string "elect their own safety warden, who\l"
	.string "answers to them, not the prince.$"

PitheadHall_Text_BrannocReform::
	.string "The CHANCELLERY won't like it. Good.\p"
	.string "Let them come and dig it themselves.$"

PitheadHall_Text_HollenClemency::
	.string "The deaths are entered in the public\n"
	.string "record as an accident. The prince's\l"
	.string "name is kept out of it.$"

PitheadHall_Text_BrannocClemency::
	.string "...You'd do that. For me.\p"
	.string "I won't forget it. I'm not sure that's a\n"
	.string "kindness.$"

PitheadHall_Text_BrannocSigns::
	.string "BRANNOC signs the pact in one heavy\n"
	.string "stroke, hard enough to tear the paper.$"

PitheadHall_Text_BrannocHereIsYourPick::
	.string "Here. The CRAGHOLT PICK. Every miner\n"
	.string "who survives his first year gets one.\p"
	.string "You've earned yours.$"

PitheadHall_Text_ReceivedPick::
	.string "{PLAYER} received the CRAGHOLT PICK\n"
	.string "from BRANNOC.$"

PitheadHall_Text_Later::
	.string "Later, when the hall has emptied,\n"
	.string "BRANNOC is still standing by the window,\l"
	.string "looking at the lift.$"

PitheadHall_Text_BrannocNotSorry::
	.string "I'm not going to say I'm sorry. Sorry's a\n"
	.string "word. Words don't move stone.\p"
	.string "I'll show them. Every day. That's the\n"
	.string "best I've got.$"

PitheadHall_Text_BrannocIToldThem::
	.string "You want to know the worst of it? I'd\n"
	.string "had enough. A month ago I told the\l"
	.string "CHANCELLERY: no more quota rises. The\l"
	.string "pit can't take it.\p"
	.string "Two days later the east tunnel came\n"
	.string "down. And I blamed the men.$"

PitheadHall_Text_ShowCrate::
	.string "You tell him about the crate at the\n"
	.string "collapse face, with the pale bell\l"
	.string "stencilled on its side.$"

PitheadHall_Text_BrannocTheGuild::
	.string "A pale bell. The blasting powder came\n"
	.string "through the GILDED SCALE. The guild\l"
	.string "supplies every pit in the kingdom.\p"
	.string "So that's how they say no to a prince.\n"
	.string "They bury his men and let him do the\l"
	.string "burying.$"

PitheadHall_Text_BrannocSuspects::
	.string "That collapse was wrong. I've dug forty\n"
	.string "tunnels. That one came down from the\l"
	.string "front, not the back.\p"
	.string "Somebody wanted me to remember who I\n"
	.string "answer to. Find out who, and I'll bring\l"
	.string "the pick.$"

PitheadHall_Text_BrannocRegirock::
	.string "You want to know where my REGIROCK\n"
	.string "came from? Everyone asks.$"

PitheadHall_Text_BrannocDugItOut::
	.string "I dug it out of the mountain myself,\n"
	.string "with these hands. Nine days, in the\l"
	.string "deepest seam we've ever cut.\p"
	.string "That's the truth. The only thing I've\n"
	.string "ever been proud of that I don't have to\l"
	.string "take back.$"

PitheadHall_Text_BrannocFirstCrew::
	.string "Go on. Your stone's waiting. I'm going\n"
	.string "down with the first paid crew.\p"
	.string "Somebody has to go first, and it's going\n"
	.string "to be me.$"

PitheadHall_Text_BrannocAfter_M::
	.string "Still here? Go home, little brother. Your\n"
	.string "walls won't build themselves.$"

PitheadHall_Text_BrannocAfter_F::
	.string "Still here? Go home, little sister. Your\n"
	.string "walls won't build themselves.$"

PitheadHall_Text_SeamBefore::
	.string "A seam of raw iron runs through the\n"
	.string "wall. The ore is stained dark with the\l"
	.string "dust of the pit.$"

PitheadHall_Text_SeamAfter::
	.string "A seam of raw iron runs through the\n"
	.string "wall. Someone has scratched names into\l"
	.string "the rock beside it, one after another.$"

