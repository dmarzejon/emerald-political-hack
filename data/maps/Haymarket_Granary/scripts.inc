Haymarket_Granary_MapScripts::
	.byte 0

@ ---- Map-thread placeholders. The dialogue thread replaces these. ----

Haymarket_Granary_EventScript_Foreman::
	trainerbattle_single TRAINER_GRUNT_RUSTURF_TUNNEL, Haymarket_Granary_Text_ForemanIntro, Haymarket_Granary_Text_ForemanDefeat
	msgbox Haymarket_Granary_Text_ForemanPostBattle, MSGBOX_AUTOCLOSE
	end

Haymarket_Granary_Text_ForemanIntro:
	.string "GUILD FOREMAN: (placeholder intro)$"

Haymarket_Granary_Text_ForemanDefeat:
	.string "GUILD FOREMAN: (placeholder defeat)$"

Haymarket_Granary_Text_ForemanPostBattle:
	.string "GUILD FOREMAN: (placeholder after battle)$"

Haymarket_Granary_EventScript_WeightsCrate::
	msgbox Haymarket_Granary_Text_WeightsCrate, MSGBOX_SIGN
	end

Haymarket_Granary_Text_WeightsCrate:
	.string "CROWN WEIGHTS CRATE (placeholder)$"

Haymarket_Granary_EventScript_Scale::
	msgbox Haymarket_Granary_Text_Scale, MSGBOX_SIGN
	end

Haymarket_Granary_Text_Scale:
	.string "THE SCALE (placeholder)$"
