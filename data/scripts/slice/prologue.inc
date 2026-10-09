@ Slice scene 1: the Apportionment.
@ Plays over a black screen at the start of a new game, before the cart (the old
@ moving truck) arrives in Lowmere. If the map thread builds a throne-room map,
@ this script can move there unchanged; only the fades would change.

Prologue_EventScript_Apportionment::
	lockall
	fadescreen FADE_TO_BLACK
	playbgm MUS_HALL_OF_FAME_ROOM, FALSE
	narrate Prologue_Text_Crownspire
	say Slice_Name_Herald, Prologue_Text_HeraldRise
	say Slice_Name_Venn, Prologue_Text_VennBeSeated
	say Slice_Name_Venn, Prologue_Text_VennCustom
	say Slice_Name_Venn, Prologue_Text_VennStepForward
	narrate Prologue_Text_Murmur
	say Slice_Name_Venn, Prologue_Text_VennLowmere
	say_g Slice_Name_Corwin, Prologue_Text_CorwinJoke_M, Prologue_Text_CorwinJoke_F
	narrate Prologue_Text_Laughter
	narrate Prologue_Text_IsoldeLooksAway
	say Slice_Name_Isolde, Prologue_Text_Isolde
	say Slice_Name_Aurelian, Prologue_Text_Aurelian
	say Slice_Name_Venn, Prologue_Text_VennReliquary
	narrate Prologue_Text_HeirsLeave
	@ A real fade-out to the cart's silence. fadeoutbgm only pauses the song,
	@ and a paused song blocks every later warp (it never counts as stopped).
	fadedefaultbgm
	narrate Prologue_Text_VennStops
	say Slice_Name_Venn, Prologue_Text_VennRestWell
	narrate Prologue_Text_VennGone
	closemessage
	delay 60
	narrate Prologue_Text_ThreeDaysLater
	say Slice_Name_Bram, Prologue_Text_BramHoldOn
	closemessage
	setvar VAR_SLICE_STATE, 1
	fadescreen FADE_FROM_BLACK
	releaseall
	end

Prologue_Text_Crownspire::
	.string "CROWNSPIRE. The throne room.\p"
	.string "Ten tall chairs stand below the throne.\n"
	.string "Nine of them are filled.\p"
	.string "The throne itself is empty. It has been\n"
	.string "empty for a long time.$"

Prologue_Text_HeraldRise::
	.string "The court will rise for the Chancellor\n"
	.string "of the realm, MAELIS VENN, who speaks in\l"
	.string "the king's name.$"

Prologue_Text_VennBeSeated::
	.string "Thank you. Please, be seated. This will\n"
	.string "not take long.\p"
	.string "The Apportionment is an old custom, and\n"
	.string "old customs are best kept brief.$"

Prologue_Text_VennCustom::
	.string "By the king's seal, each child of the\n"
	.string "HOUSE OF VALCOURT is given a town to\l"
	.string "govern.\p"
	.string "As the town prospers, so the heir is\n"
	.string "judged.\p"
	.string "Nine of you received your towns long\n"
	.string "ago. Today the tenth comes of age.$"

Prologue_Text_VennStepForward::
	.string "{PLAYER}, child of KING ALDRIC and of\n"
	.string "ELENA MARSH. Step forward.$"

Prologue_Text_Murmur::
	.string "A murmur runs along the row of chairs.\n"
	.string "Somebody laughs behind a glove.$"

Prologue_Text_VennLowmere::
	.string "To {PLAYER}, the Crown apportions the\n"
	.string "town of... LOWMERE.$"

Prologue_Text_CorwinJoke_M::
	.string "LOWMERE! The marsh! Oh, that's perfect.\p"
	.string "All hail our little brother, Lord of the\n"
	.string "Mere! Mind the frogs don't stage a coup,\l"
	.string "Your Highness.$"

Prologue_Text_CorwinJoke_F::
	.string "LOWMERE! The marsh! Oh, that's perfect.\p"
	.string "All hail our little sister, Lord of the\n"
	.string "Mere! Mind the frogs don't stage a coup,\l"
	.string "Your Highness.$"

Prologue_Text_Laughter::
	.string "The hall fills with laughter. Even a few\n"
	.string "of the guards join in.$"

Prologue_Text_IsoldeLooksAway::
	.string "PRINCESS ISOLDE looks down at her\n"
	.string "hands, as if she is embarrassed for you.$"

Prologue_Text_Isolde::
	.string "LOWMERE has... a great deal of sky. I'm\n"
	.string "sure it will be very peaceful there.$"

Prologue_Text_Aurelian::
	.string "Congratulations, {PLAYER}. Every town\n"
	.string "deserves a ruler who will do their best\l"
	.string "by it.\p"
	.string "Even that one.$"

Prologue_Text_VennReliquary::
	.string "With their town, each heir also receives\n"
	.string "a legendary POKéMON from the Crown's\l"
	.string "reliquary.\p"
	.string "The Chancellery has ruled that this\n"
	.string "custom applies to heirs of the blood.\l"
	.string "The full blood.\p"
	.string "The reliquary is closed. The court is\n"
	.string "dismissed.$"

Prologue_Text_HeirsLeave::
	.string "Chairs scrape on stone. Your brothers\n"
	.string "and sisters file out without looking\l"
	.string "back.$"

Prologue_Text_VennStops::
	.string "As the hall empties, the CHANCELLOR\n"
	.string "pauses at your side.$"

Prologue_Text_VennRestWell::
	.string "LOWMERE has good air, they say.\p"
	.string "Rest well there, Your Highness.$"

Prologue_Text_VennGone::
	.string "He smiles, and then he is gone.$"

Prologue_Text_ThreeDaysLater::
	.string "Three days later.\p"
	.string "The cart jolts along the marsh road.\n"
	.string "Somewhere up ahead is LOWMERE.$"

Prologue_Text_BramHoldOn::
	.string "Hold on to something. The road gets\n"
	.string "worse before it gets better.\p"
	.string "Like most things.$"

