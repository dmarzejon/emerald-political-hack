Haymarket_CountingHouse_MapScripts::
	.byte 0

@ ---- Map-thread placeholders. The dialogue thread replaces these. ----

Haymarket_CountingHouse_EventScript_Clerk::
	trainerbattle_single TRAINER_GRUNT_MUSEUM_1, Haymarket_CountingHouse_Text_ClerkIntro, Haymarket_CountingHouse_Text_ClerkDefeat
	msgbox Haymarket_CountingHouse_Text_ClerkPostBattle, MSGBOX_AUTOCLOSE
	end

Haymarket_CountingHouse_Text_ClerkIntro:
	.string "GUILD CLERK: (placeholder intro)$"

Haymarket_CountingHouse_Text_ClerkDefeat:
	.string "GUILD CLERK: (placeholder defeat)$"

Haymarket_CountingHouse_Text_ClerkPostBattle:
	.string "GUILD CLERK: (placeholder after battle)$"

Haymarket_CountingHouse_EventScript_Receptionist::
	msgbox Haymarket_CountingHouse_Text_Receptionist, MSGBOX_NPC
	end

Haymarket_CountingHouse_Text_Receptionist:
	.string "RECEPTIONIST: (placeholder)$"
