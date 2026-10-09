@ Chapter 2: the Orchard Road (Route 102), from Haymarket west to Thornfield.
@ Object, trigger and sign placement: docs/dialogue/chapter2.md#orchard-road

@ ---------------------------------------------------------------------------
@ The road warden blocks the road until the slice is over (state 13). The
@ map header hides him from then on with FLAG_TEMP_1.
@ ---------------------------------------------------------------------------
OrchardRoad_EventScript_HideWarden::
	setflag FLAG_TEMP_1
	return

OrchardRoad_EventScript_Warden::
	lock
	faceplayer
	setspeaker Ch2_Name_Warden
	goto_if_set FLAG_BADGE01_GET, OrchardRoad_EventScript_WardenSeal
	msgbox OrchardRoad_Text_WardenClosed, MSGBOX_DEFAULT
	release
	end

OrchardRoad_EventScript_WardenSeal::
	msgbox OrchardRoad_Text_WardenSeal, MSGBOX_DEFAULT
	release
	end

@ ---------------------------------------------------------------------------
@ The tenant with the handcart: the first sign of what Thornfield does.
@ ---------------------------------------------------------------------------
OrchardRoad_EventScript_Tenant::
	lock
	faceplayer
	setspeaker Ch2_Name_Tenant
	goto_if_ge VAR_THORNFIELD_STATE, 3, OrchardRoad_EventScript_TenantGoingBack
	msgbox OrchardRoad_Text_Tenant, MSGBOX_DEFAULT
	release
	end

OrchardRoad_EventScript_TenantGoingBack::
	msgbox OrchardRoad_Text_TenantGoingBack, MSGBOX_DEFAULT
	release
	end

OrchardRoad_EventScript_TenantCart::
	sign OrchardRoad_Text_TenantCart
	end

@ ---------------------------------------------------------------------------
@ Trainers.
@ ---------------------------------------------------------------------------
OrchardRoad_EventScript_Picker::
	trainerbattle_single TRAINER_ORCHARD_ROAD_PICKER, OrchardRoad_Text_PickerIntro, OrchardRoad_Text_PickerDefeat
	npc Ch2_Name_Picker, OrchardRoad_Text_PickerPostBattle
	end

OrchardRoad_EventScript_Bailiff::
	trainerbattle_single TRAINER_ORCHARD_ROAD_BAILIFF, OrchardRoad_Text_BailiffIntro, OrchardRoad_Text_BailiffDefeat
	npc Ch2_Name_Bailiff, OrchardRoad_Text_BailiffPostBattle
	end

OrchardRoad_EventScript_Poet::
	trainerbattle_single TRAINER_ORCHARD_ROAD_POET, OrchardRoad_Text_PoetIntro, OrchardRoad_Text_PoetDefeat
	npc Ch2_Name_Poet, OrchardRoad_Text_PoetPostBattle
	end

@ ---------------------------------------------------------------------------
@ Signs.
@ ---------------------------------------------------------------------------
OrchardRoad_EventScript_SignEast::
	sign OrchardRoad_Text_SignEast
	end

OrchardRoad_EventScript_SignWest::
	sign OrchardRoad_Text_SignWest
	end

OrchardRoad_EventScript_Orchard::
	sign OrchardRoad_Text_Orchard
	end

@ ---------------------------------------------------------------------------
@ Text
@ ---------------------------------------------------------------------------

OrchardRoad_Text_WardenClosed::
	.string "Halt! This is Her Highness PRINCESS\n"
	.string "ISOLDE's road, and it's closed.\p"
	.string "It's planting season in the terraces. No\n"
	.string "carts and no strangers until it's done.\p"
	.string "If you've business in THORNFIELD, come\n"
	.string "back when you've got something better\l"
	.string "than business.$"

OrchardRoad_Text_WardenSeal::
	.string "That's a seal from a TRIAL OF STANDING.\n"
	.string "A real one.\p"
	.string "I don't know what the rule is for that.\n"
	.string "Nobody's ever had one.\p"
	.string "Give me a day to ask, Your Highness.\n"
	.string "Then the road's yours.$"

OrchardRoad_Text_Tenant::
	.string "Going to THORNFIELD? Don't buy\n"
	.string "anything, and don't sign anything.\p"
	.string "We farmed plot six for nineteen years.\n"
	.string "Then the new steward came, and\l"
	.string "suddenly we owed four years' rent.\p"
	.string "We'd paid three of them. He had a ledger\n"
	.string "that said we hadn't. Who are you going\l"
	.string "to believe, a ledger or a farmer?$"

OrchardRoad_Text_TenantGoingBack::
	.string "They say the land's going back to the\n"
	.string "tenants! Is it true?\p"
	.string "Then I'm turning this cart round. Plot\n"
	.string "six. Nineteen years. I'd know it with my\l"
	.string "eyes shut.$"

OrchardRoad_Text_TenantCart::
	.string "A handcart piled with a whole house's\n"
	.string "worth of things. A cooking pot. A chair.\l"
	.string "A child's wooden POKéMON, painted\l"
	.string "green.$"

OrchardRoad_Text_PickerIntro::
	.string "Hands off the apples! These belong to\n"
	.string "Her Highness. Every single one.$"

OrchardRoad_Text_PickerDefeat::
	.string "Ow! Fine. Take an apple. Take two. She'll\n"
	.string "never count them.$"

OrchardRoad_Text_PickerPostBattle::
	.string "I get a penny a basket. The apples go to\n"
	.string "the palace, and the palace sends them\l"
	.string "on to the GILDED SCALE.\p"
	.string "I've never eaten one. Not officially.$"

OrchardRoad_Text_BailiffIntro::
	.string "GILDED SCALE, debt collection. You look\n"
	.string "like somebody who owes something.$"

OrchardRoad_Text_BailiffDefeat::
	.string "That's going in the ledger. With\n"
	.string "interest.$"

OrchardRoad_Text_BailiffPostBattle::
	.string "Rent's one thing. Interest on the rent\n"
	.string "is where the money is. Interest on the\l"
	.string "interest is where the land is.\p"
	.string "STEWARD VOSS taught me that. He's a\n"
	.string "genius.$"

OrchardRoad_Text_PoetIntro::
	.string "Hush! I'm composing. “O LOWMERE, land\n"
	.string "where nothing grows...”\p"
	.string "Wait. You're from LOWMERE?$"

OrchardRoad_Text_PoetDefeat::
	.string "Something grows in LOWMERE after all.\n"
	.string "Spite, mostly.$"

OrchardRoad_Text_PoetPostBattle::
	.string "I'm on my way to Her Highness's salon.\n"
	.string "She reads her poems on the terraces\l"
	.string "every evening, and we all applaud.\p"
	.string "She writes beautifully about the land.\n"
	.string "She just doesn't go out on it.$"

OrchardRoad_Text_SignEast::
	.string "ORCHARD ROAD\p"
	.string "West: THORNFIELD, the Garden of the\n"
	.string "Realm East: HAYMARKET$"

OrchardRoad_Text_SignWest::
	.string "THORNFIELD\p"
	.string "Garden of the Realm. By order of Her\n"
	.string "Royal Highness PRINCESS ISOLDE, keep\l"
	.string "off the terraces.$"

OrchardRoad_Text_Orchard::
	.string "Rows of apple trees, every branch heavy\n"
	.string "with fruit. Each tree has a little tag:\l"
	.string "PROPERTY OF THE CROWN.$"

