@ Slice scenes 10-11: Corwin's Trial of Standing in the Gilt Pavilion, and the
@ Pact of Fair Measure.
@ Object placement: docs/dialogue/events.md#gilt-pavilion

#ifndef LOCALID_PAVILION_CORWIN
#define LOCALID_PAVILION_CORWIN 1
#endif

@ ---------------------------------------------------------------------------
@ The auctioneer at the door (the Gym guide's job).
@ ---------------------------------------------------------------------------
GiltPavilion_EventScript_Auctioneer::
	lock
	faceplayer
	setspeaker Slice_Name_Auctioneer
	goto_if_set FLAG_BADGE01_GET, GiltPavilion_EventScript_AuctioneerSold
	msgbox GiltPavilion_Text_AuctioneerWelcome, MSGBOX_DEFAULT
	setspeaker Slice_Name_Auctioneer
	msgbox GiltPavilion_Text_AuctioneerAdvice, MSGBOX_DEFAULT
	release
	end

GiltPavilion_EventScript_AuctioneerSold::
	checkplayergender
	goto_if_eq VAR_RESULT, FEMALE, GiltPavilion_EventScript_AuctioneerSoldFemale
	msgbox GiltPavilion_Text_AuctioneerSold_M, MSGBOX_DEFAULT
	release
	end

GiltPavilion_EventScript_AuctioneerSoldFemale::
	msgbox GiltPavilion_Text_AuctioneerSold_F, MSGBOX_DEFAULT
	release
	end

@ Lot calls for the floor puzzle. The map/features threads own the puzzle; these
@ are the lines the auctioneer calls. Each one buffers nothing, so any of them
@ can be used from a coord trigger with `narrate` or `say`.
GiltPavilion_EventScript_LotCallSeven::
	say Slice_Name_Auctioneer, GiltPavilion_Text_LotSeven
	closemessage
	return

GiltPavilion_EventScript_LotCallTwelve::
	say Slice_Name_Auctioneer, GiltPavilion_Text_LotTwelve
	closemessage
	return

GiltPavilion_EventScript_LotCallTwenty::
	say Slice_Name_Auctioneer, GiltPavilion_Text_LotTwenty
	closemessage
	return

GiltPavilion_EventScript_WrongLot::
	say Slice_Name_Auctioneer, GiltPavilion_Text_WrongLot
	closemessage
	return

@ ---------------------------------------------------------------------------
@ Courtiers: brokers and bidders.
@ ---------------------------------------------------------------------------
GiltPavilion_EventScript_Broker1::
	trainerbattle_single TRAINER_PAVILION_BROKER_1, GiltPavilion_Text_Broker1Intro, GiltPavilion_Text_Broker1Defeat
	npc SP_NAME_NONE, GiltPavilion_Text_Broker1PostBattle
	end

GiltPavilion_EventScript_Bidder::
	trainerbattle_single TRAINER_PAVILION_BIDDER, GiltPavilion_Text_BidderIntro, GiltPavilion_Text_BidderDefeat
	npc SP_NAME_NONE, GiltPavilion_Text_BidderPostBattle
	end

GiltPavilion_EventScript_Broker2::
	trainerbattle_single TRAINER_PAVILION_BROKER_2, GiltPavilion_Text_Broker2Intro, GiltPavilion_Text_Broker2Defeat
	npc SP_NAME_NONE, GiltPavilion_Text_Broker2PostBattle
	end

@ ---------------------------------------------------------------------------
@ Corwin. Trial 1. Win -> Haymarket Seal (badge 1), the pact, and Corwin's
@ private talk about the bell seal (state 11 -> 12).
@ ---------------------------------------------------------------------------
GiltPavilion_EventScript_Corwin::
	lock
	faceplayer
	goto_if_set FLAG_BADGE01_GET, GiltPavilion_EventScript_CorwinSulking
	say Slice_Name_Corwin, GiltPavilion_Text_CorwinMainEvent
	narrate GiltPavilion_Text_GalleriesPacked
	say_g Slice_Name_Corwin, GiltPavilion_Text_CorwinWritIsPaper_M, GiltPavilion_Text_CorwinWritIsPaper_F
	say Slice_Name_Corwin, GiltPavilion_Text_CorwinGiveThemAShow
	closemessage
	trainerbattle_no_intro TRAINER_CORWIN, GiltPavilion_Text_CorwinDefeat
	goto GiltPavilion_EventScript_CorwinDefeated
	end

GiltPavilion_EventScript_CorwinDefeated::
	narrate GiltPavilion_Text_Silence
	playse SE_APPLAUSE
	waitse
	say Slice_Name_Corwin, GiltPavilion_Text_CorwinTheyWereCheering
	say Slice_Name_Corwin, GiltPavilion_Text_CorwinNameYourTerms
	narrate GiltPavilion_Text_ArdenStepsDown
	say Slice_Name_Arden, GiltPavilion_Text_PactTerms
	narrate GiltPavilion_Text_CorwinSigns
	say Slice_Name_Corwin, GiltPavilion_Text_CorwinHereIsYourSeal
	setspeaker SP_NAME_NONE
	message GiltPavilion_Text_ReceivedSeal
	waitmessage
	call Common_EventScript_PlayGymBadgeFanfare
	setflag FLAG_BADGE01_GET
	setflag FLAG_PACT_HAYMARKET
	narrate GiltPavilion_Text_SealExplained
	closemessage
	fadescreen FADE_TO_BLACK
	delay 30
	fadescreen FADE_FROM_BLACK
	narrate GiltPavilion_Text_Later
	say Slice_Name_Corwin, GiltPavilion_Text_CorwinDontGloat
	narrate GiltPavilion_Text_ShowReceipt
	say Slice_Name_Corwin, GiltPavilion_Text_CorwinNotTheirSeal
	say Slice_Name_Corwin, GiltPavilion_Text_CorwinThatsOnMe
	say Slice_Name_Corwin, GiltPavilion_Text_CorwinJustInvestors
	say Slice_Name_Corwin, GiltPavilion_Text_CorwinGoHome
	say Slice_Name_Guard, GiltPavilion_Text_GuardCraneGone
	closemessage
	setflag FLAG_CORWIN_PRIVATE_TALK
	setvar VAR_SLICE_STATE, 12
	release
	end

GiltPavilion_EventScript_CorwinSulking::
	npc Slice_Name_Corwin, GiltPavilion_Text_CorwinSulking
	end

@ The galleries (bg events along the side walls).
GiltPavilion_EventScript_Gallery::
	goto_if_set FLAG_BADGE01_GET, GiltPavilion_EventScript_GalleryAfter
	sign GiltPavilion_Text_GalleryBefore
	end

GiltPavilion_EventScript_GalleryAfter::
	sign GiltPavilion_Text_GalleryAfter
	end

@ ---------------------------------------------------------------------------
@ Text
@ ---------------------------------------------------------------------------

GiltPavilion_Text_AuctioneerWelcome::
	.string "Welcome, welcome, to the GILT PAVILION!\n"
	.string "Bidding opens on the hour, every hour!\p"
	.string "A TRIAL, you say? Oh, how exciting.\n"
	.string "There's been a great deal of interest in\l"
	.string "this one. The galleries are already full.$"

GiltPavilion_Text_AuctioneerAdvice::
	.string "A word to the wise, free of charge,\n"
	.string "which is very rare in here.\p"
	.string "His Highness favours NORMAL POKéMON.\n"
	.string "Lots of muscle, lots of tricks. A\l"
	.string "FIGHTING POKéMON would fetch a very\l"
	.string "high price against them, if you follow\l"
	.string "me.\p"
	.string "And watch for the final lot. He always\n"
	.string "saves the best for last.$"

GiltPavilion_Text_AuctioneerSold_M::
	.string "Sold! To the prince from LOWMERE!\p"
	.string "I've called a thousand auctions, and I\n"
	.string "never saw that one coming.$"

GiltPavilion_Text_AuctioneerSold_F::
	.string "Sold! To the princess from LOWMERE!\p"
	.string "I've called a thousand auctions, and I\n"
	.string "never saw that one coming.$"

GiltPavilion_Text_LotSeven::
	.string "Lot seven! Do I hear lot seven?$"

GiltPavilion_Text_LotTwelve::
	.string "Lot twelve! Lot twelve, going once!$"

GiltPavilion_Text_LotTwenty::
	.string "Lot twenty! Final call for lot twenty!$"

GiltPavilion_Text_WrongLot::
	.string "Oh dear, wrong lot! That one already has\n"
	.string "a bidder!$"

GiltPavilion_Text_Broker1Intro::
	.string "Ah, a new client! Everything in the\n"
	.string "PAVILION has a price. Including your\l"
	.string "dignity.\p"
	.string "Shall we negotiate?$"

GiltPavilion_Text_Broker1Defeat::
	.string "I've been outbid!$"

GiltPavilion_Text_Broker1PostBattle::
	.string "His Highness doesn't care where the\n"
	.string "money comes from. Neither do we. That's\l"
	.string "what makes us such good friends.$"

GiltPavilion_Text_BidderIntro::
	.string "I've bid on every lot this season, and I\n"
	.string "haven't won one! I'm not losing to you\l"
	.string "as well!$"

GiltPavilion_Text_BidderDefeat::
	.string "Outbid again...$"

GiltPavilion_Text_BidderPostBattle::
	.string "The guild always wins the good lots.\n"
	.string "Always. Some of us are starting to\l"
	.string "notice.$"

GiltPavilion_Text_Broker2Intro::
	.string "You're the one with the writ? The guild\n"
	.string "sent word. I'm to make sure you arrive...\l"
	.string "tired.$"

GiltPavilion_Text_Broker2Defeat::
	.string "MASTER CRANE won't like this.$"

GiltPavilion_Text_Broker2PostBattle::
	.string "Go on. His Highness hates to be kept\n"
	.string "waiting. Nearly as much as he hates\l"
	.string "losing.$"

GiltPavilion_Text_CorwinMainEvent::
	.string "Well, well! Ladies and gentlemen, the\n"
	.string "main event!$"

GiltPavilion_Text_GalleriesPacked::
	.string "The galleries are packed. It looks as if\n"
	.string "half of HAYMARKET has come to watch.$"

GiltPavilion_Text_CorwinWritIsPaper_M::
	.string "I have to admit, little brother, I didn't\n"
	.string "think old ARDEN had it in him.\p"
	.string "Still. A writ is only paper.$"

GiltPavilion_Text_CorwinWritIsPaper_F::
	.string "I have to admit, little sister, I didn't\n"
	.string "think old ARDEN had it in him.\p"
	.string "Still. A writ is only paper.$"

GiltPavilion_Text_CorwinGiveThemAShow::
	.string "Fine! Let's give them a show!\p"
	.string "When I win, you go back to your puddle,\n"
	.string "and you thank me for the grain.\p"
	.string "Lot one! The bidding starts now!$"

GiltPavilion_Text_CorwinDefeat::
	.string "No. No, no, no. That was my final lot...$"

GiltPavilion_Text_Silence::
	.string "The PAVILION goes quiet. Then,\n"
	.string "somewhere in the gallery, someone\l"
	.string "starts to clap.\p"
	.string "Then everyone does.$"

GiltPavilion_Text_CorwinTheyWereCheering::
	.string "...They were cheering for you.$"

GiltPavilion_Text_CorwinNameYourTerms::
	.string "Fine. FINE. The law is the law. Name your\n"
	.string "terms.$"

GiltPavilion_Text_ArdenStepsDown::
	.string "MAGISTRATE ARDEN comes down from the\n"
	.string "gallery with the pact already written\l"
	.string "out.$"

GiltPavilion_Text_PactTerms::
	.string "The PACT OF FAIR MEASURE.\p"
	.string "One. The CROWN's weights are restored\n"
	.string "to HAYMARKET market, from today.\p"
	.string "Two. The GILDED SCALE's grain contract\n"
	.string "is void.\p"
	.string "Three. HAYMARKET sells grain to LOWMERE\n"
	.string "at a fair price, by honest weight.$"

GiltPavilion_Text_CorwinSigns::
	.string "CORWIN signs. He presses so hard that\n"
	.string "the pen tears the paper.$"

GiltPavilion_Text_CorwinHereIsYourSeal::
	.string "There. And here's your seal. Don't lose\n"
	.string "it. It's worth more than your whole\l"
	.string "town.$"

GiltPavilion_Text_ReceivedSeal::
	.string "{PLAYER} received the HAYMARKET SEAL\n"
	.string "from CORWIN!$"

GiltPavilion_Text_SealExplained::
	.string "A royal seal is proof of a TRIAL OF\n"
	.string "STANDING won, and of a pact that must\l"
	.string "be kept.$"

GiltPavilion_Text_Later::
	.string "Later. The galleries are empty. The\n"
	.string "bunting looks tired.$"

GiltPavilion_Text_CorwinDontGloat::
	.string "Don't gloat. I can see you not gloating,\n"
	.string "and it's worse.$"

GiltPavilion_Text_ShowReceipt::
	.string "You show CORWIN the receipt from the\n"
	.string "MIRE ROAD.$"

GiltPavilion_Text_CorwinNotTheirSeal::
	.string "That's the guild's... no. No, that isn't\n"
	.string "their seal. Theirs is a pair of scales.\p"
	.string "What's this? A bell?$"

GiltPavilion_Text_CorwinThatsOnMe::
	.string "Look. I signed off on the weights. I'm\n"
	.string "not going to pretend I didn't. CRANE\l"
	.string "paid for the fairs, and I never asked\l"
	.string "how.\p"
	.string "That's on me.$"

GiltPavilion_Text_CorwinJustInvestors::
	.string "But this... CRANE said they were just\n"
	.string "investors. Friends of the Chancellery.\l"
	.string "Quiet money from the capital.\p"
	.string "...Who puts a bell with no clapper on a\n"
	.string "receipt?$"

GiltPavilion_Text_CorwinGoHome::
	.string "Go home, {PLAYER}. Your grain will be on\n"
	.string "the road by morning.\p"
	.string "I'll see to it myself. ...Somebody should.$"

GiltPavilion_Text_GuardCraneGone::
	.string "Your Highness! Your Highness! MASTER\n"
	.string "CRANE's gone! His office at the\l"
	.string "COUNTING HOUSE is empty!$"

GiltPavilion_Text_CorwinSulking::
	.string "Go away. I'm sulking.\p"
	.string "...The grain went out this morning. Fair\n"
	.string "weight. I checked it myself.$"

GiltPavilion_Text_GalleryBefore::
	.string "The galleries are packed with\n"
	.string "townsfolk. Some of them are eating\l"
	.string "nuts.$"

GiltPavilion_Text_GalleryAfter::
	.string "The galleries are empty. Someone has\n"
	.string "left behind a bag of nuts.$"

