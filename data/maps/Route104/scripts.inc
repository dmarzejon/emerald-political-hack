@ The Chain Road. Its people are in the dialogue thread's chapter 3 scripts
@ (docs/dialogue/chapter3.md); this file only holds the map header.
Route104_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, ChainRoad_OnTransition
	.byte 0

ChainRoad_OnTransition:
	call_if_ge VAR_CRAGHOLT_STATE, 3, ChainRoad_EventScript_HideCoffle
	end

Route104_EventScript_FlowerShopSign::
	msgbox Route104_Text_PrettyPetalFlowShop, MSGBOX_SIGN
	end

Route104_Text_PrettyPetalFlowShop:
	.string "PRETTY PETAL FLOWER SHOP$"
