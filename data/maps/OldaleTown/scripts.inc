@ Haymarket. Its scenes, townsfolk and signs live in
@ data/scripts/slice/haymarket.inc; this file only holds the header.
OldaleTown_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, OldaleTown_OnTransition
	.byte 0

OldaleTown_OnTransition:
	setflag FLAG_VISITED_OLDALE_TOWN
	end
