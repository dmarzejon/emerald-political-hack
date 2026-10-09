@ Lowmere. Its scenes, NPCs and signs live in data/scripts/slice/lowmere.inc;
@ this file only holds the map-script header.
LittlerootTown_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, LittlerootTown_OnTransition
	map_script MAP_SCRIPT_ON_FRAME_TABLE, LittlerootTown_OnFrame
	.byte 0

LittlerootTown_OnTransition:
	setflag FLAG_VISITED_LITTLEROOT_TOWN
	end

@ Bram's greeting as the player steps off the cart, and the grain carts on
@ the first visit after the Haymarket pact.
LittlerootTown_OnFrame:
	map_script_2 VAR_SLICE_STATE, 1, Lowmere_EventScript_Arrival
	map_script_2 VAR_SLICE_STATE, 12, Lowmere_EventScript_GrainArrives
	.2byte 0
