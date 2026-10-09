@ Chapter 3: the Chain Road (Route 104 south and north), the Penrose cottage
@ and Gallows Wood (Petalburg Woods), from Thornfield north to Cragholt.
@ Object, trigger and sign placement: docs/dialogue/chapter3.md#the-chain-road

@ ---------------------------------------------------------------------------
@ The coffle resting by the beach path: three bonded workers roped together
@ and their guard. They are there until the Cragholt pact frees them.
@ ---------------------------------------------------------------------------
ChainRoad_EventScript_HideCoffle::
	setflag FLAG_TEMP_1
	return

ChainRoad_EventScript_CoffleGuard::
	npc Slice_Name_Guard, ChainRoad_Text_CoffleGuard
	end

ChainRoad_EventScript_Harrow::
	lock
	faceplayer
	say Ch3_Name_Harrow, ChainRoad_Text_Harrow
	goto_if_ge VAR_THORNFIELD_STATE, 3, ChainRoad_EventScript_HarrowHeardPact
	release
	end

ChainRoad_EventScript_HarrowHeardPact::
	say Ch3_Name_Harrow, ChainRoad_Text_HarrowPact
	release
	end

ChainRoad_EventScript_Bonded1::
	npc Ch3_Name_Bonded, ChainRoad_Text_Bonded1
	end

ChainRoad_EventScript_Bonded2::
	npc Ch3_Name_Bonded, ChainRoad_Text_Bonded2
	end

ChainRoad_EventScript_Rope::
	sign ChainRoad_Text_Rope
	end

@ ---------------------------------------------------------------------------
@ The Penrose cottage. Old Penrose's son Wil was sold to the mines for debt.
@ ---------------------------------------------------------------------------
ChainRoad_PenroseCottage_EventScript_Penrose::
	lock
	faceplayer
	setspeaker Ch3_Name_Penrose
	goto_if_ge VAR_CRAGHOLT_STATE, 3, ChainRoad_PenroseCottage_EventScript_PenroseHome
	goto_if_set FLAG_RESCUED_WIL, ChainRoad_PenroseCottage_EventScript_PenroseAlive
	msgbox ChainRoad_Text_PenroseSon, MSGBOX_DEFAULT
	release
	end

ChainRoad_PenroseCottage_EventScript_PenroseAlive::
	msgbox ChainRoad_Text_PenroseAlive, MSGBOX_DEFAULT
	release
	end

ChainRoad_PenroseCottage_EventScript_PenroseHome::
	msgbox ChainRoad_Text_PenroseHome, MSGBOX_DEFAULT
	release
	end

@ Wil comes home after the pact.
ChainRoad_PenroseCottage_EventScript_HideWil::
	setflag FLAG_TEMP_1
	return

ChainRoad_PenroseCottage_EventScript_Wil::
	npc Ch3_Name_Wil, ChainRoad_Text_WilHome
	end

ChainRoad_PenroseCottage_EventScript_Nets::
	sign ChainRoad_Text_Nets
	end

@ ---------------------------------------------------------------------------
@ Trainers.
@ ---------------------------------------------------------------------------
ChainRoad_EventScript_Fisher::
	trainerbattle_single TRAINER_CHAIN_ROAD_FISHER, ChainRoad_Text_FisherIntro, ChainRoad_Text_FisherDefeat
	npc SP_NAME_NONE, ChainRoad_Text_FisherPostBattle
	end

ChainRoad_EventScript_Collector::
	trainerbattle_single TRAINER_CHAIN_ROAD_COLLECTOR, ChainRoad_Text_CollectorIntro, ChainRoad_Text_CollectorDefeat
	npc Ch3_Name_Collector, ChainRoad_Text_CollectorPostBattle
	end

GallowsWood_EventScript_BugCatcher::
	trainerbattle_single TRAINER_GALLOWS_WOOD_BUG_CATCHER, GallowsWood_Text_BugCatcherIntro, GallowsWood_Text_BugCatcherDefeat
	npc SP_NAME_NONE, GallowsWood_Text_BugCatcherPostBattle
	end

@ The debt warden blocks the path out of the wood.
GallowsWood_EventScript_DebtWarden::
	trainerbattle_single TRAINER_DEBT_WARDEN, GallowsWood_Text_WardenIntro, GallowsWood_Text_WardenDefeat
	npc Ch3_Name_Warden, GallowsWood_Text_WardenPostBattle
	end

@ ---------------------------------------------------------------------------
@ Signs.
@ ---------------------------------------------------------------------------
ChainRoad_EventScript_SignSouth::
	sign ChainRoad_Text_SignSouth
	end

ChainRoad_EventScript_SignNorth::
	sign ChainRoad_Text_SignNorth
	end

GallowsWood_EventScript_Sign::
	sign GallowsWood_Text_Sign
	end

GallowsWood_EventScript_Checkpoint::
	sign GallowsWood_Text_Checkpoint
	end

@ ---------------------------------------------------------------------------
@ Text
@ ---------------------------------------------------------------------------

ChainRoad_Text_CoffleGuard::
	.string "Bought and paid for, Your Highness.\n"
	.string "Every one of them.\p"
	.string "Debtors from THORNFIELD, convicts from\n"
	.string "HAYMARKET gaol. Prince BRANNOC buys\l"
	.string "them by the dozen. Don't talk to the\l"
	.string "stock.$"

ChainRoad_Text_Harrow::
	.string "HARROW. Ned HARROW. We had plot eleven,\n"
	.string "outside THORNFIELD.\p"
	.string "When they took the land, the debt was\n"
	.string "still there. So they sold the debt. Then\l"
	.string "they sold me with it.$"

ChainRoad_Text_HarrowPact::
	.string "They say the land's going back to the\n"
	.string "tenants. That's good. That's good for\l"
	.string "Mary and the girls.\p"
	.string "Doesn't do much for a man on a rope,\n"
	.string "mind.$"

ChainRoad_Text_Bonded1::
	.string "They tell us it's five years in the pits,\n"
	.string "then we're free.\p"
	.string "Nobody I know has done five years.$"

ChainRoad_Text_Bonded2::
	.string "Don't mind the rope. You stop feeling it\n"
	.string "after the first day.\p"
	.string "You stop feeling a lot of things.$"

ChainRoad_Text_Rope::
	.string "A rope ties the three of them together,\n"
	.string "wrist to wrist. The knots are old and\l"
	.string "very tight.$"

ChainRoad_Text_PenroseSon::
	.string "You're going north? To CRAGHOLT?\p"
	.string "My boy WIL is up there. I owed the\n"
	.string "GILDED SCALE for a boat, and the boat\l"
	.string "sank, and they took WIL for the debt.\p"
	.string "That was two winters ago. No letters.\n"
	.string "They say the pits are no place for\l"
	.string "letters.\p"
	.string "If you see him... tell him the nets are\n"
	.string "mended. He'll know what it means.$"

ChainRoad_Text_PenroseAlive::
	.string "WIL's alive? Under the mountain all this\n"
	.string "time, and alive?\p"
	.string "I'll mend the nets twice over. I'll mend\n"
	.string "every net on this beach.$"

ChainRoad_Text_PenroseHome::
	.string "He came down the road this morning, thin\n"
	.string "as a rail, and walked straight into the\l"
	.string "sea with his boots on.\p"
	.string "Said he wanted to feel something cold\n"
	.string "that wasn't rock. I let him.$"

ChainRoad_Text_WilHome::
	.string "I used to dream about this cottage,\n"
	.string "down in the dark. The smell of tar and\l"
	.string "salt.\p"
	.string "It's smaller than I remembered. It's\n"
	.string "perfect.$"

ChainRoad_Text_Nets::
	.string "Fishing nets, carefully mended. Every\n"
	.string "knot is new.$"

ChainRoad_Text_FisherIntro::
	.string "The fish don't care who's a prince.\n"
	.string "Neither do I!$"

ChainRoad_Text_FisherDefeat::
	.string "Well, the fish are safe. I'm not.$"

ChainRoad_Text_FisherPostBattle::
	.string "The coffles come up this road every\n"
	.string "week. I fish, and I don't look up. It's\l"
	.string "easier.$"

ChainRoad_Text_CollectorIntro::
	.string "GILDED SCALE collections! This road's\n"
	.string "paved with debts, and I'm here to pick\l"
	.string "them up.$"

ChainRoad_Text_CollectorDefeat::
	.string "That one's going on your account.$"

ChainRoad_Text_CollectorPostBattle::
	.string "A debtor in THORNFIELD is worth\n"
	.string "nothing. A debtor in CRAGHOLT is worth a\l"
	.string "ton of ore a month.\p"
	.string "Same man. Different town. That's\n"
	.string "business.$"

GallowsWood_Text_BugCatcherIntro::
	.string "Shh! There's something in the trees\n"
	.string "that only comes out when it's quiet!$"

GallowsWood_Text_BugCatcherDefeat::
	.string "You scared it off. Or I did.$"

GallowsWood_Text_BugCatcherPostBattle::
	.string "They call it GALLOWS WOOD because of\n"
	.string "the old oak by the path. Nobody's been\l"
	.string "hanged on it for a hundred years.\p"
	.string "The coffles still stop under it to rest,\n"
	.string "though. The guards think it's funny.$"

GallowsWood_Text_WardenIntro::
	.string "Halt! GILDED SCALE checkpoint. Nobody\n"
	.string "leaves this wood northbound without\l"
	.string "their debts checked.\p"
	.string "Royal? That just means bigger debts.$"

GallowsWood_Text_WardenDefeat::
	.string "Fine! Your account is... settled. For\n"
	.string "now.$"

GallowsWood_Text_WardenPostBattle::
	.string "We catch runaways here. Bonded men who\n"
	.string "slip the rope on the way to the pits.\p"
	.string "If they're lucky, we send them back. If\n"
	.string "they're not, we send them deeper.$"

ChainRoad_Text_SignSouth::
	.string "THE CHAIN ROAD\p"
	.string "North: GALLOWS WOOD, CRAGHOLT South:\n"
	.string "THORNFIELD$"

ChainRoad_Text_SignNorth::
	.string "THE CHAIN ROAD\p"
	.string "North: CRAGHOLT, City of Stone South:\n"
	.string "GALLOWS WOOD$"

GallowsWood_Text_Sign::
	.string "GALLOWS WOOD\p"
	.string "Keep to the path. Report runaways to\n"
	.string "the checkpoint.$"

GallowsWood_Text_Checkpoint::
	.string "GILDED SCALE DEBT CHECKPOINT\p"
	.string "All bonded persons must be roped. All\n"
	.string "debts must be declared.$"

