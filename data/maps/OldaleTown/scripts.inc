@ Haymarket. Its scenes, townsfolk and signs live in
@ data/scripts/slice/haymarket.inc; this file only holds the header.
OldaleTown_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, OldaleTown_OnTransition
	map_script MAP_SCRIPT_ON_FRAME_TABLE, OldaleTown_OnFrame
	.byte 0

@ Crane waits outside the Magistrate's Hall once the Trial is granted. The
@ step out of the hall's door is automatic and doesn't fire the trigger in
@ front of it, so the scene starts on frame.
OldaleTown_OnFrame:
	map_script_2 VAR_SLICE_STATE, 10, Haymarket_EventScript_Crane
	.2byte 0

OldaleTown_OnTransition:
	setflag FLAG_VISITED_OLDALE_TOWN
	end
