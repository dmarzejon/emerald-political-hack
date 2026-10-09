InsideOfTruck_MapScripts::
	map_script MAP_SCRIPT_ON_LOAD, InsideOfTruck_OnLoad
	map_script MAP_SCRIPT_ON_RESUME, InsideOfTruck_OnResume
	map_script MAP_SCRIPT_ON_FRAME_TABLE, InsideOfTruck_OnFrame
	.byte 0

@ The truck is the cart that brings the player to Lowmere. On a new game the
@ screen stays black (FieldCB_SlicePrologue in src/overworld.c), the court
@ prologue plays, and it warps straight to Lowmere.
InsideOfTruck_OnFrame:
	map_script_2 VAR_SLICE_STATE, 0, Prologue_EventScript_Apportionment
	.2byte 0

InsideOfTruck_OnLoad:
	setmetatile 4, 1, METATILE_InsideOfTruck_ExitLight_Top, FALSE
	setmetatile 4, 2, METATILE_InsideOfTruck_ExitLight_Mid, FALSE
	setmetatile 4, 3, METATILE_InsideOfTruck_ExitLight_Bottom, FALSE
	end

InsideOfTruck_OnResume:
	setstepcallback STEP_CB_TRUCK
	end

@ Arriving in Lowmere. The prologue calls this and then warps the prince or
@ the princess to Lowmere (9,10), beside Bram; both live in the Old Lodge.
@ The vanilla moving-day cast in the two houses stays hidden for good.
InsideOfTruck_EventScript_LeaveCart::
	setrespawn HEAL_LOCATION_LITTLEROOT_TOWN_BRENDANS_HOUSE_2F
	setvar VAR_LITTLEROOT_INTRO_STATE, 7
	setflag FLAG_HIDE_LITTLEROOT_TOWN_BRENDANS_HOUSE_MOM
	setflag FLAG_HIDE_LITTLEROOT_TOWN_MAYS_HOUSE_MOM
	setflag FLAG_HIDE_LITTLEROOT_TOWN_BRENDANS_HOUSE_RIVAL_MOM
	setflag FLAG_HIDE_LITTLEROOT_TOWN_MAYS_HOUSE_RIVAL_MOM
	setflag FLAG_HIDE_LITTLEROOT_TOWN_BRENDANS_HOUSE_RIVAL_SIBLING
	setflag FLAG_HIDE_LITTLEROOT_TOWN_MAYS_HOUSE_RIVAL_SIBLING
	setflag FLAG_HIDE_LITTLEROOT_TOWN_BRENDANS_HOUSE_2F_POKE_BALL
	setflag FLAG_HIDE_LITTLEROOT_TOWN_MAYS_HOUSE_2F_POKE_BALL
	setflag FLAG_HIDE_LITTLEROOT_TOWN_PLAYERS_HOUSE_VIGOROTH_1
	setflag FLAG_HIDE_LITTLEROOT_TOWN_PLAYERS_HOUSE_VIGOROTH_2
	return

@ Only reached if the player is ever left standing in the cart (the exit
@ triggers in map.json): the same arrival, by the cart's door.
InsideOfTruck_EventScript_SetIntroFlags::
	lockall
	call InsideOfTruck_EventScript_LeaveCart
	setdynamicwarp MAP_LITTLEROOT_TOWN, 9, 10
	releaseall
	end

InsideOfTruck_EventScript_MovingBox::
	msgbox InsideOfTruck_Text_BoxPrintedWithMonLogo, MSGBOX_SIGN
	end

InsideOfTruck_Text_BoxPrintedWithMonLogo:
	.string "A crate of your things from the capital.\p"
	.string "It's stamped: PROPERTY OF THE CROWN.\n"
	.string "On loan.$"

