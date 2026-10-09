@ Shared helpers for the vertical slice (Lowmere, Mire Road, Haymarket, Gilt Pavilion).
@ Owned by the dialogue thread. Map layout, flags/vars and trainer teams are owned by
@ other threads; see docs/dialogue/events.md for the contract.

@ ---------------------------------------------------------------------------
@ Temporary constants.
@ Each name below is the one the slice scripts use. Until the features thread
@ (flags/vars/items), the balance thread (trainers) and the map thread (local IDs)
@ define the real ones, these fallbacks keep the build green. Every fallback is
@ inside #ifndef, so it drops out by itself as soon as the real name exists.
@ Delete this block once all of them are defined elsewhere.
@ ---------------------------------------------------------------------------

@ Story progress through the slice. See docs/dialogue/events.md for each step.
#ifndef VAR_SLICE_STATE
#define VAR_SLICE_STATE VAR_UNUSED_0x404E
#endif
#ifndef VAR_LOWMERE_STAGE
#define VAR_LOWMERE_STAGE VAR_UNUSED_0x4083
#endif
#ifndef VAR_KINGS_GIFT_DOG
#define VAR_KINGS_GIFT_DOG VAR_UNUSED_0x408B
#endif

#ifndef FLAG_RECEIVED_SILVER_WING
#define FLAG_RECEIVED_SILVER_WING FLAG_UNUSED_0x020
#endif
#ifndef FLAG_RECEIVED_KINGS_GIFT
#define FLAG_RECEIVED_KINGS_GIFT FLAG_UNUSED_0x021
#endif
#ifndef FLAG_FOUND_BELL_RECEIPT
#define FLAG_FOUND_BELL_RECEIPT FLAG_UNUSED_0x022
#endif
#ifndef FLAG_HAYMARKET_WEIGHTS_FOUND
#define FLAG_HAYMARKET_WEIGHTS_FOUND FLAG_UNUSED_0x023
#endif
#ifndef FLAG_HAYMARKET_WITNESS
#define FLAG_HAYMARKET_WITNESS FLAG_UNUSED_0x024
#endif
#ifndef FLAG_HAYMARKET_TRIAL_GRANTED
#define FLAG_HAYMARKET_TRIAL_GRANTED FLAG_UNUSED_0x025
#endif
#ifndef FLAG_PACT_HAYMARKET
#define FLAG_PACT_HAYMARKET FLAG_UNUSED_0x026
#endif
#ifndef FLAG_PENN_MON_RECOVERED
#define FLAG_PENN_MON_RECOVERED FLAG_UNUSED_0x046
#endif
#ifndef FLAG_RECEIVED_SEALED_LETTER
#define FLAG_RECEIVED_SEALED_LETTER FLAG_UNUSED_0x047
#endif
#ifndef FLAG_TALKED_TO_CARTER
#define FLAG_TALKED_TO_CARTER FLAG_UNUSED_0x048
#endif
#ifndef FLAG_MET_CRANE
#define FLAG_MET_CRANE FLAG_UNUSED_0x049
#endif
#ifndef FLAG_CORWIN_PRIVATE_TALK
#define FLAG_CORWIN_PRIVATE_TALK FLAG_UNUSED_0x04A
#endif
#ifndef FLAG_TAMSIN_MIRE_ROAD_TALK
#define FLAG_TAMSIN_MIRE_ROAD_TALK FLAG_UNUSED_0x04B
#endif

@ Object hide flags (set = hidden). The map thread puts these on the objects.
#ifndef FLAG_HIDE_LOWMERE_ARRIVAL_BRAM
#define FLAG_HIDE_LOWMERE_ARRIVAL_BRAM FLAG_UNUSED_0x04C
#endif
#ifndef FLAG_HIDE_OLD_LODGE_TOFT
#define FLAG_HIDE_OLD_LODGE_TOFT FLAG_UNUSED_0x04D
#endif
#ifndef FLAG_HIDE_OLD_LODGE_BRAM
#define FLAG_HIDE_OLD_LODGE_BRAM FLAG_UNUSED_0x04E
#endif
#ifndef FLAG_HIDE_OLD_LODGE_COURIER
#define FLAG_HIDE_OLD_LODGE_COURIER FLAG_UNUSED_0x030
#endif
#ifndef FLAG_HIDE_LOWMERE_TAMSIN_FORGE
#define FLAG_HIDE_LOWMERE_TAMSIN_FORGE FLAG_UNUSED_0x031
#endif
#ifndef FLAG_HIDE_LOWMERE_TAMSIN_ROAD
#define FLAG_HIDE_LOWMERE_TAMSIN_ROAD FLAG_UNUSED_0x032
#endif
#ifndef FLAG_HIDE_MIRE_ROAD_TAMSIN
#define FLAG_HIDE_MIRE_ROAD_TAMSIN FLAG_UNUSED_0x033
#endif
#ifndef FLAG_HIDE_MIRE_ROAD_WAGON_CLERKS
#define FLAG_HIDE_MIRE_ROAD_WAGON_CLERKS FLAG_UNUSED_0x034
#endif
#ifndef FLAG_HIDE_HAYMARKET_SEIZURE_CLERKS
#define FLAG_HIDE_HAYMARKET_SEIZURE_CLERKS FLAG_UNUSED_0x035
#endif
#ifndef FLAG_HIDE_HAYMARKET_CORWIN_SQUARE
#define FLAG_HIDE_HAYMARKET_CORWIN_SQUARE FLAG_UNUSED_0x036
#endif
#ifndef FLAG_HIDE_HAYMARKET_CRANE
#define FLAG_HIDE_HAYMARKET_CRANE FLAG_UNUSED_0x037
#endif
#ifndef FLAG_HIDE_COUNTING_HOUSE_PENN_BALL
#define FLAG_HIDE_COUNTING_HOUSE_PENN_BALL FLAG_UNUSED_0x038
#endif
#ifndef FLAG_HIDE_PAVILION_CORWIN_AFTER
#define FLAG_HIDE_PAVILION_CORWIN_AFTER FLAG_UNUSED_0x039
#endif
#ifndef FLAG_HIDE_LOWMERE_GRAIN_CARTS
#define FLAG_HIDE_LOWMERE_GRAIN_CARTS FLAG_UNUSED_0x03A
#endif
#ifndef FLAG_HIDE_MIRE_ROAD_TAMSIN_WAGON
#define FLAG_HIDE_MIRE_ROAD_TAMSIN_WAGON FLAG_UNUSED_0x03C
#endif
#ifndef FLAG_HIDE_HAYMARKET_TAMSIN
#define FLAG_HIDE_HAYMARKET_TAMSIN FLAG_UNUSED_0x03B
#endif

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
