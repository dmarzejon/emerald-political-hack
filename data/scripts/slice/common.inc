@ Shared helpers for the vertical slice (Lowmere, Mire Road, Haymarket, Gilt Pavilion).
@ Owned by the dialogue thread. Map layout, flags/vars and trainer teams are owned by
@ other threads; see docs/dialogue/events.md for the contract.

@ ---------------------------------------------------------------------------
@ Temporary constants.
@ The features thread now defines every flag, var and item the slice uses
@ (include/constants/flags.h, vars.h, items.h). What is left here are the
@ trainer names and KINGS_GIFT_LEVEL, which the balance thread defines in
@ include/constants/slice_balance.h. Each fallback is inside #ifndef, so it
@ drops out by itself once that header is on main.
@ Delete this block once all of them are defined elsewhere.
@ ---------------------------------------------------------------------------

@ Trainers. The balance thread owns the teams; the fallbacks borrow vanilla
@ early-game trainers so the battles run until the real ones exist.
#ifndef TRAINER_TAMSIN_LOWMERE_0
#define TRAINER_TAMSIN_LOWMERE_0 TRAINER_MAY_ROUTE_103_TREECKO
#endif
#ifndef TRAINER_TAMSIN_LOWMERE_1
#define TRAINER_TAMSIN_LOWMERE_1 TRAINER_MAY_ROUTE_103_TORCHIC
#endif
#ifndef TRAINER_TAMSIN_LOWMERE_2
#define TRAINER_TAMSIN_LOWMERE_2 TRAINER_MAY_ROUTE_103_MUDKIP
#endif
#ifndef TRAINER_MIRE_ROAD_CLERK
#define TRAINER_MIRE_ROAD_CLERK TRAINER_CALVIN_1
#endif
#ifndef TRAINER_MIRE_ROAD_POACHER
#define TRAINER_MIRE_ROAD_POACHER TRAINER_RICK
#endif
#ifndef TRAINER_MIRE_ROAD_YOUNGSTER
#define TRAINER_MIRE_ROAD_YOUNGSTER TRAINER_ALLEN
#endif
#ifndef TRAINER_MIRE_ROAD_CARTER
#define TRAINER_MIRE_ROAD_CARTER TRAINER_TIANA
#endif
#ifndef TRAINER_MIRE_ROAD_WAGON_CLERK
#define TRAINER_MIRE_ROAD_WAGON_CLERK TRAINER_GRUNT_PETALBURG_WOODS
#endif
#ifndef TRAINER_HAYMARKET_FOREMAN
#define TRAINER_HAYMARKET_FOREMAN TRAINER_JOSH
#endif
#ifndef TRAINER_HAYMARKET_CLERK_1
#define TRAINER_HAYMARKET_CLERK_1 TRAINER_GRUNT_RUSTURF_TUNNEL
#endif
#ifndef TRAINER_HAYMARKET_CLERK_2
#define TRAINER_HAYMARKET_CLERK_2 TRAINER_GRUNT_MUSEUM_1
#endif
#ifndef TRAINER_PAVILION_BROKER_1
#define TRAINER_PAVILION_BROKER_1 TRAINER_TOMMY
#endif
#ifndef TRAINER_PAVILION_BIDDER
#define TRAINER_PAVILION_BIDDER TRAINER_MARC
#endif
#ifndef TRAINER_PAVILION_BROKER_2
#define TRAINER_PAVILION_BROKER_2 TRAINER_JOSH
#endif
#ifndef TRAINER_CORWIN
#define TRAINER_CORWIN TRAINER_ROXANNE_1
#endif

@ Levels the balance thread owns.
#ifndef KINGS_GIFT_LEVEL
#define KINGS_GIFT_LEVEL 5
#endif

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

Slice_Name_Venn::
	.string "CHANCELLOR VENN$"
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
	.string "MAGISTRATE ARDEN$"
Slice_Name_Foreman::
	.string "FOREMAN$"
Slice_Name_Auctioneer::
	.string "AUCTIONEER$"
Slice_Name_Guard::
	.string "PAVILION GUARD$"
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
