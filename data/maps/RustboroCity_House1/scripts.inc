@ Magistrate Hollen's hall in Cragholt; see docs/dialogue/chapter3.md.
RustboroCity_House1_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, RustboroCity_House1_OnTransition
	.byte 0

RustboroCity_House1_OnTransition:
	call Cragholt_MagistrateHall_EventScript_SetObjects
	end
