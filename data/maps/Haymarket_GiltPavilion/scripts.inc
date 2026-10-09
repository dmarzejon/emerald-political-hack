Haymarket_GiltPavilion_MapScripts::
	.byte 0

@ ---- Map-thread placeholders. The dialogue thread replaces these. ----

Haymarket_GiltPavilion_EventScript_Corwin::
	msgbox Haymarket_GiltPavilion_Text_Corwin, MSGBOX_NPC
	end

Haymarket_GiltPavilion_Text_Corwin:
	.string "CORWIN (Trial 1): (placeholder)$"

Haymarket_GiltPavilion_EventScript_Auctioneer::
	msgbox Haymarket_GiltPavilion_Text_Auctioneer, MSGBOX_NPC
	end

Haymarket_GiltPavilion_Text_Auctioneer:
	.string "AUCTIONEER: (placeholder)$"

Haymarket_GiltPavilion_EventScript_Fenwick::
	trainerbattle_single TRAINER_MARC, Haymarket_GiltPavilion_Text_FenwickIntro, Haymarket_GiltPavilion_Text_FenwickDefeat
	msgbox Haymarket_GiltPavilion_Text_FenwickPostBattle, MSGBOX_AUTOCLOSE
	end

Haymarket_GiltPavilion_Text_FenwickIntro:
	.string "BROKER FENWICK: (placeholder intro)$"

Haymarket_GiltPavilion_Text_FenwickDefeat:
	.string "BROKER FENWICK: (placeholder defeat)$"

Haymarket_GiltPavilion_Text_FenwickPostBattle:
	.string "BROKER FENWICK: (placeholder after battle)$"

Haymarket_GiltPavilion_EventScript_Celeste::
	trainerbattle_single TRAINER_TOMMY, Haymarket_GiltPavilion_Text_CelesteIntro, Haymarket_GiltPavilion_Text_CelesteDefeat
	msgbox Haymarket_GiltPavilion_Text_CelestePostBattle, MSGBOX_AUTOCLOSE
	end

Haymarket_GiltPavilion_Text_CelesteIntro:
	.string "BIDDER CELESTE: (placeholder intro)$"

Haymarket_GiltPavilion_Text_CelesteDefeat:
	.string "BIDDER CELESTE: (placeholder defeat)$"

Haymarket_GiltPavilion_Text_CelestePostBattle:
	.string "BIDDER CELESTE: (placeholder after battle)$"

Haymarket_GiltPavilion_EventScript_Albrecht::
	trainerbattle_single TRAINER_JOSH, Haymarket_GiltPavilion_Text_AlbrechtIntro, Haymarket_GiltPavilion_Text_AlbrechtDefeat
	msgbox Haymarket_GiltPavilion_Text_AlbrechtPostBattle, MSGBOX_AUTOCLOSE
	end

Haymarket_GiltPavilion_Text_AlbrechtIntro:
	.string "BIDDER ALBRECHT: (placeholder intro)$"

Haymarket_GiltPavilion_Text_AlbrechtDefeat:
	.string "BIDDER ALBRECHT: (placeholder defeat)$"

Haymarket_GiltPavilion_Text_AlbrechtPostBattle:
	.string "BIDDER ALBRECHT: (placeholder after battle)$"

Haymarket_GiltPavilion_EventScript_SpectatorA::
	msgbox Haymarket_GiltPavilion_Text_SpectatorA, MSGBOX_NPC
	end

Haymarket_GiltPavilion_Text_SpectatorA:
	.string "SPECTATOR: (placeholder)$"

Haymarket_GiltPavilion_EventScript_SpectatorB::
	msgbox Haymarket_GiltPavilion_Text_SpectatorB, MSGBOX_NPC
	end

Haymarket_GiltPavilion_Text_SpectatorB:
	.string "SPECTATOR: (placeholder)$"

Haymarket_GiltPavilion_EventScript_SpectatorC::
	msgbox Haymarket_GiltPavilion_Text_SpectatorC, MSGBOX_NPC
	end

Haymarket_GiltPavilion_Text_SpectatorC:
	.string "SPECTATOR: (placeholder)$"

Haymarket_GiltPavilion_EventScript_Steward::
	msgbox Haymarket_GiltPavilion_Text_Steward, MSGBOX_NPC
	end

Haymarket_GiltPavilion_Text_Steward:
	.string "STEWARD: (placeholder)$"
