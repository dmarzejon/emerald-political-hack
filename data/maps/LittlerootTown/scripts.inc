@ Lowmere. Its scenes, NPCs and signs live in data/scripts/slice/lowmere.inc;
@ this file only holds the map-script header.
LittlerootTown_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, LittlerootTown_OnTransition
	map_script MAP_SCRIPT_ON_FRAME_TABLE, LittlerootTown_OnFrame
	.byte 0

@ Marta's stall only opens at Stage 1 (her object's flag is FLAG_TEMP_1).
LittlerootTown_OnTransition:
	setflag FLAG_VISITED_LITTLEROOT_TOWN
	call_if_eq VAR_LOWMERE_STAGE, LOWMERE_STAGE_0, LittlerootTown_EventScript_HideStallkeeper
	end

LittlerootTown_EventScript_HideStallkeeper::
	setflag FLAG_TEMP_1
	return

@ Bram's greeting as the player steps off the cart, and the carts heard on
@ the first sight of the Stage 1 town after the Haymarket pact.
LittlerootTown_OnFrame:
	map_script_2 VAR_SLICE_STATE, 1, Lowmere_EventScript_Arrival
	map_script_2 VAR_TEMP_1, 0, Lowmere_EventScript_CheckReturn
	.2byte 0
