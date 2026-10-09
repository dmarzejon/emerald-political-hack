Haymarket_GiltPavilion_MapScripts::
	.byte 0

@ The auction floor. Three rows of numbered gold lots cross the hall. On the row
@ below each one the auctioneer calls a lot (VAR_TEMP_1-3 stop the call repeating),
@ and only the called lot lets you on (FLAG_TEMP_1-3 remember it until you leave).
@ A wrong lot sends you back a step. After Corwin's Trial the floor is quiet.
@ The auctioneer's lines are in data/scripts/slice/gilt_pavilion.inc.
Haymarket_GiltPavilion_EventScript_CallSeven::
	setvar VAR_TEMP_1, 1
	goto_if_set FLAG_BADGE01_GET, Haymarket_GiltPavilion_EventScript_FloorQuiet
	goto_if_set FLAG_TEMP_1, Haymarket_GiltPavilion_EventScript_FloorQuiet
	lockall
	call GiltPavilion_EventScript_LotCallSeven
	releaseall
	end

Haymarket_GiltPavilion_EventScript_CallTwelve::
	setvar VAR_TEMP_2, 1
	goto_if_set FLAG_BADGE01_GET, Haymarket_GiltPavilion_EventScript_FloorQuiet
	goto_if_set FLAG_TEMP_2, Haymarket_GiltPavilion_EventScript_FloorQuiet
	lockall
	call GiltPavilion_EventScript_LotCallTwelve
	releaseall
	end

Haymarket_GiltPavilion_EventScript_CallTwenty::
	setvar VAR_TEMP_3, 1
	goto_if_set FLAG_BADGE01_GET, Haymarket_GiltPavilion_EventScript_FloorQuiet
	goto_if_set FLAG_TEMP_3, Haymarket_GiltPavilion_EventScript_FloorQuiet
	lockall
	call GiltPavilion_EventScript_LotCallTwenty
	releaseall
	end

Haymarket_GiltPavilion_EventScript_LotACalled::
	goto_if_set FLAG_TEMP_1, Haymarket_GiltPavilion_EventScript_FloorQuiet
	setflag FLAG_TEMP_1
	goto Haymarket_GiltPavilion_EventScript_LotWon

Haymarket_GiltPavilion_EventScript_LotBCalled::
	goto_if_set FLAG_TEMP_2, Haymarket_GiltPavilion_EventScript_FloorQuiet
	setflag FLAG_TEMP_2
	goto Haymarket_GiltPavilion_EventScript_LotWon

Haymarket_GiltPavilion_EventScript_LotCCalled::
	goto_if_set FLAG_TEMP_3, Haymarket_GiltPavilion_EventScript_FloorQuiet
	setflag FLAG_TEMP_3
	goto Haymarket_GiltPavilion_EventScript_LotWon

Haymarket_GiltPavilion_EventScript_LotAWrong::
	goto_if_set FLAG_TEMP_1, Haymarket_GiltPavilion_EventScript_FloorQuiet
	goto Haymarket_GiltPavilion_EventScript_LotLost

Haymarket_GiltPavilion_EventScript_LotBWrong::
	goto_if_set FLAG_TEMP_2, Haymarket_GiltPavilion_EventScript_FloorQuiet
	goto Haymarket_GiltPavilion_EventScript_LotLost

Haymarket_GiltPavilion_EventScript_LotCWrong::
	goto_if_set FLAG_TEMP_3, Haymarket_GiltPavilion_EventScript_FloorQuiet
	goto Haymarket_GiltPavilion_EventScript_LotLost

Haymarket_GiltPavilion_EventScript_LotWon::
	goto_if_set FLAG_BADGE01_GET, Haymarket_GiltPavilion_EventScript_FloorQuiet
	lockall
	playse SE_SUCCESS
	waitse
	releaseall
	end

Haymarket_GiltPavilion_EventScript_LotLost::
	goto_if_set FLAG_BADGE01_GET, Haymarket_GiltPavilion_EventScript_FloorQuiet
	lockall
	playse SE_FAILURE
	waitse
	call GiltPavilion_EventScript_WrongLot
	applymovement LOCALID_PLAYER, Haymarket_GiltPavilion_Movement_StepBack
	waitmovement 0
	releaseall
	end

Haymarket_GiltPavilion_EventScript_FloorQuiet::
	end

Haymarket_GiltPavilion_Movement_StepBack:
	lock_facing_direction
	walk_down
	unlock_facing_direction
	step_end
