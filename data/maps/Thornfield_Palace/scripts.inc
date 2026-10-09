Thornfield_Palace_MapScripts::
	.byte 0

@ Chapter 2 placeholders for Isolde's palace garden (map thread). The dialogue and
@ balance threads replace these with the Trial and the gardeners' battles; see
@ docs/maps.md, "Chapter 2 events".
ThornfieldPalace_EventScript_Isolde::
	msgbox ThornfieldPalace_Text_Isolde, MSGBOX_NPC
	end

ThornfieldPalace_EventScript_Gardener1::
ThornfieldPalace_EventScript_Gardener2::
ThornfieldPalace_EventScript_Gardener3::
	msgbox ThornfieldPalace_Text_Gardener, MSGBOX_NPC
	end

ThornfieldPalace_Text_Isolde:
	.string "ISOLDE: Oh, little one. You came all\n"
	.string "this way. I do wish you wouldn't.$"

ThornfieldPalace_Text_Gardener:
	.string "Mind the beds. The Princess counts\n"
	.string "every petal.$"
