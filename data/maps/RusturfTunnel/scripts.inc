@ The collapsed mine east of Cragholt; see docs/dialogue/chapter3.md.
RusturfTunnel_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, CollapsedMine_OnTransition
	map_script MAP_SCRIPT_ON_LOAD, CollapsedMine_OnLoad
	.byte 0

CollapsedMine_OnTransition:
	call CollapsedMine_EventScript_SetObjects
	end

CollapsedMine_OnLoad:
	call_if_set FLAG_RESCUED_WIL, CollapsedMine_EventScript_ClearRubble
	end
