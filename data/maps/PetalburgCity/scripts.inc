@ Thornfield. Its scenes and people are in the dialogue thread's chapter 2 scripts
@ (docs/dialogue/chapter2.md); this file only holds the map header.
PetalburgCity_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, Thornfield_OnTransition
	map_script MAP_SCRIPT_ON_LOAD, Thornfield_OnLoad
	.byte 0

Thornfield_OnTransition:
	setflag FLAG_VISITED_PETALBURG_CITY
	call Thornfield_EventScript_SetObjects
	end

@ While the Willows are evicted (states 1 and 2), their door is boarded up.
Thornfield_OnLoad:
	call_if_eq VAR_THORNFIELD_STATE, 1, Thornfield_EventScript_BoardUpDoor
	call_if_eq VAR_THORNFIELD_STATE, 2, Thornfield_EventScript_BoardUpDoor
	end
