@ Old Penrose's cottage on the Chain Road; see docs/dialogue/chapter3.md.
Route104_MrBrineysHouse_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, ChainRoad_PenroseCottage_OnTransition
	.byte 0

ChainRoad_PenroseCottage_OnTransition:
	call_if_lt VAR_CRAGHOLT_STATE, 3, ChainRoad_PenroseCottage_EventScript_HideWil
	end
