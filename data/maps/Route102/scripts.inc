@ The Orchard Road. Its scenes and people are in the dialogue thread's chapter 2 scripts
@ (docs/dialogue/chapter2.md); this file only holds the map header.
Route102_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, OrchardRoad_OnTransition
	.byte 0

OrchardRoad_OnTransition:
	call_if_ge VAR_SLICE_STATE, 13, OrchardRoad_EventScript_HideWarden
	end
