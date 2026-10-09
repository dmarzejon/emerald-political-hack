@ The Ranger's Shed in Lowmere. Bram's scenes live in
@ data/scripts/slice/lowmere.inc; this file holds the header and the
@ furniture. The sign labels keep their vanilla names so map.json still
@ points at them.
LittlerootTown_ProfessorBirchsLab_MapScripts::
	.byte 0

LittlerootTown_ProfessorBirchsLab_EventScript_Machine::
	msgbox LittlerootTown_ProfessorBirchsLab_Text_Cages, MSGBOX_SIGN
	end

LittlerootTown_ProfessorBirchsLab_EventScript_PC::
	msgbox LittlerootTown_ProfessorBirchsLab_Text_PC, MSGBOX_SIGN
	end

LittlerootTown_ProfessorBirchsLab_EventScript_Bookshelf::
	msgbox LittlerootTown_ProfessorBirchsLab_Text_Bookshelf, MSGBOX_SIGN
	end

LittlerootTown_ProfessorBirchsLab_EventScript_Book::
	msgbox LittlerootTown_ProfessorBirchsLab_Text_Logbook, MSGBOX_SIGN
	end

LittlerootTown_ProfessorBirchsLab_Text_Cages:
	.string "Old trap cages, empty and freshly\n"
	.string "oiled. Somebody still cares for them.$"

LittlerootTown_ProfessorBirchsLab_Text_PC:
	.string "A crown-issue PC. The seal on the\n"
	.string "case is cracked, but it still hums.$"

LittlerootTown_ProfessorBirchsLab_Text_Bookshelf:
	.string "Field guides to the marsh. The pages\n"
	.string "have swollen in the damp.$"

LittlerootTown_ProfessorBirchsLab_Text_Logbook:
	.string "A ranger's logbook. The last entry\n"
	.string "is three years old.$"

@ Vanilla scripts that other code still calls (prof_birch.inc and
@ field_control_avatar.c), kept as they were.

EventScript_RegisterProfBirch::
	msgbox MatchCall_Text_BirchRegisterCall, MSGBOX_DEFAULT
	closemessage
	delay 30
	playfanfare MUS_REGISTER_MATCH_CALL
	msgbox MatchCall_Text_RegisteredBirch, MSGBOX_DEFAULT
	waitfanfare
	closemessage
	delay 30
	setflag FLAG_ENABLE_PROF_BIRCH_MATCH_CALL
	setvar VAR_REGISTER_BIRCH_STATE, 2
	release
	end

LittlerootTown_ProfessorBirchsLab_EventScript_ScottAboardSSTidalCall::
	lockall
	pokenavcall LittlerootTown_ProfessorBirchsLab_Text_ScottAboardSSTidalCall
	waitmessage
	clearflag FLAG_SCOTT_CALL_BATTLE_FRONTIER
	releaseall
	end

LittlerootTown_ProfessorBirchsLab_Text_ScottAboardSSTidalCall:
	.string "… … … … … …\n"
	.string "… … … … … Beep!\p"
	.string "SCOTT: Hi, hi, {PLAYER}!\n"
	.string "It's me, SCOTT.\p"
	.string "I'm aboard the S.S. TIDAL now.\n"
	.string "It feels great to be on the sea!\p"
	.string "By the way…\p"
	.string "There's this place that I'd like to\n"
	.string "invite you to for a visit.\p"
	.string "If you're interested, board a ferry\n"
	.string "at either SLATEPORT or LILYCOVE.\p"
	.string "I'll fill you in on the details when\n"
	.string "we meet. I'll be waiting!\p"
	.string "… … … … … …\n"
	.string "… … … … … Click!$"
