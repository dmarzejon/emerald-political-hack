@ Thornfield: the Steward's Office. Scripts are in the dialogue thread's chapter 2 files.
PetalburgCity_WallysHouse_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, Thornfield_StewardsOffice_OnTransition
	.byte 0

Thornfield_StewardsOffice_OnTransition:
	call_if_ge VAR_THORNFIELD_STATE, 3, Thornfield_StewardsOffice_EventScript_HideVoss
	end
