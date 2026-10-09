@ Shared helpers for the vertical slice (Lowmere, Mire Road, Haymarket, Gilt Pavilion).
@ Owned by the dialogue thread. Map layout, flags/vars and trainer teams are owned by
@ other threads; see docs/dialogue/events.md for the contract.

@ ---------------------------------------------------------------------------
@ Macros
@ ---------------------------------------------------------------------------

	@ Shows a message with the speaker's name in the name box.
	.macro say speaker:req, text:req
	setspeaker \speaker
	msgbox \text, MSGBOX_DEFAULT
	.endm

	@ Same as say, but picks the boy (_M) or girl (_F) form of the text.
	.macro say_g speaker:req, male:req, female:req
	setspeaker \speaker
	checkplayergender
	goto_if_eq VAR_RESULT, FEMALE, .Lsay_g_female_\@
	msgbox \male, MSGBOX_DEFAULT
	goto .Lsay_g_done_\@
.Lsay_g_female_\@:
	msgbox \female, MSGBOX_DEFAULT
.Lsay_g_done_\@:
	.endm

	@ Narration: no name box.
	.macro narrate text:req
	setspeaker SP_NAME_NONE
	msgbox \text, MSGBOX_DEFAULT
	.endm

	@ A whole talk-to-NPC interaction with a name box.
	.macro npc speaker:req, text:req
	setspeaker \speaker
	msgbox \text, MSGBOX_NPC
	.endm

	@ A sign or object the player examines.
	.macro sign text:req
	setspeaker SP_NAME_NONE
	msgbox \text, MSGBOX_SIGN
	.endm

	@ Emote above an object and wait for it.
	.macro emote obj:req, movement:req
	applymovement \obj, \movement
	waitmovement 0
	.endm

@ ---------------------------------------------------------------------------
@ Shared scripts
@ ---------------------------------------------------------------------------

Slice_EventScript_ShakeScreenSmall::
	setvar VAR_0x8004, 0
	setvar VAR_0x8005, 2
	setvar VAR_0x8006, 4
	setvar VAR_0x8007, 2
	special ShakeScreen
	delay 20
	return

@ ---------------------------------------------------------------------------
@ Speaker names (name box). Proper names in capitals, as in Emerald.
@ ---------------------------------------------------------------------------

@ The name box is at most 64 pixels wide in the small font (about 13 capitals),
@ so long titles are cut down to the name.
Slice_Name_Venn::
	.string "VENN$"
Slice_Name_Corwin::
	.string "CORWIN$"
Slice_Name_Isolde::
	.string "ISOLDE$"
Slice_Name_Aurelian::
	.string "AURELIAN$"
Slice_Name_Herald::
	.string "HERALD$"
Slice_Name_Bram::
	.string "BRAM$"
Slice_Name_Tamsin::
	.string "TAMSIN$"
Slice_Name_Hesk::
	.string "MOTHER HESK$"
Slice_Name_Toft::
	.string "REEVE TOFT$"
Slice_Name_Courier::
	.string "COURIER$"
Slice_Name_Villager::
	.string "VILLAGER$"
Slice_Name_Fisher::
	.string "FISHERMAN$"
Slice_Name_Mother::
	.string "MOTHER$"
Slice_Name_OldMan::
	.string "OLD MAN$"
Slice_Name_Child::
	.string "CHILD$"
Slice_Name_Stallkeeper::
	.string "STALLKEEPER$"
Slice_Name_Clerk::
	.string "GUILD CLERK$"
Slice_Name_Clerk2::
	.string "SECOND CLERK$"
Slice_Name_Carter::
	.string "CARTER$"
Slice_Name_Penn::
	.string "WIDOW PENN$"
Slice_Name_Crane::
	.string "SILAS CRANE$"
Slice_Name_Arden::
	.string "ARDEN$"
Slice_Name_Foreman::
	.string "FOREMAN$"
Slice_Name_Auctioneer::
	.string "AUCTIONEER$"
Slice_Name_Guard::
	.string "GUARD$"
Slice_Name_Crowd::
	.string "CROWD$"
Slice_Name_Townsfolk::
	.string "TOWNSPERSON$"
Slice_Name_Merchant::
	.string "MERCHANT$"
Slice_Name_Farmer::
	.string "FARMER$"
Slice_Name_Granny::
	.string "GRANNY$"
Slice_Name_Youngster::
	.string "BOY$"
Slice_Name_Girl::
	.string "GIRL$"
Slice_Name_Player::
	.string "{PLAYER}$"
