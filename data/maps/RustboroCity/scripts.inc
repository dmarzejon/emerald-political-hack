@ Cragholt. Its people and scenes are in the dialogue thread's chapter 3 scripts
@ (docs/dialogue/chapter3.md); this file only holds the map header.
RustboroCity_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, Cragholt_OnTransition
	.byte 0

Cragholt_OnTransition:
	setflag FLAG_VISITED_RUSTBORO_CITY
	call Cragholt_EventScript_SetObjects
	end
