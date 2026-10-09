Haymarket_CountingHouse_BackRoom_MapScripts::
	.byte 0

@ ---- Map-thread placeholders. The dialogue thread replaces these. ----

Haymarket_CountingHouse_BackRoom_EventScript_Clerk::
	trainerbattle_single TRAINER_GRUNT_MUSEUM_2, Haymarket_CountingHouse_BackRoom_Text_ClerkIntro, Haymarket_CountingHouse_BackRoom_Text_ClerkDefeat
	msgbox Haymarket_CountingHouse_BackRoom_Text_ClerkPostBattle, MSGBOX_AUTOCLOSE
	end

Haymarket_CountingHouse_BackRoom_Text_ClerkIntro:
	.string "GUILD CLERK: (placeholder intro)$"

Haymarket_CountingHouse_BackRoom_Text_ClerkDefeat:
	.string "GUILD CLERK: (placeholder defeat)$"

Haymarket_CountingHouse_BackRoom_Text_ClerkPostBattle:
	.string "GUILD CLERK: (placeholder after battle)$"

Haymarket_CountingHouse_BackRoom_EventScript_PennsPokemon::
	msgbox Haymarket_CountingHouse_BackRoom_Text_PennsPokemon, MSGBOX_NPC
	end

Haymarket_CountingHouse_BackRoom_Text_PennsPokemon:
	.string "WIDOW PENN'S POKEMON: (placeholder)$"

Haymarket_CountingHouse_BackRoom_EventScript_CranesDesk::
	msgbox Haymarket_CountingHouse_BackRoom_Text_CranesDesk, MSGBOX_SIGN
	end

Haymarket_CountingHouse_BackRoom_Text_CranesDesk:
	.string "CRANE'S DESK (sealed letter, placeholder)$"
