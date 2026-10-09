Haymarket_Granary_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, Haymarket_Granary_OnTransition
	.byte 0

@ The sacks in front of the scale room are Strength boulders.
Haymarket_Granary_OnTransition:
	setflag FLAG_SYS_USE_STRENGTH
	end

Haymarket_Granary_EventScript_WeightsCrate::
	goto Haymarket_Granary_EventScript_CrownCrate
	end
