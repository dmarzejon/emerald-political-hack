@ Chapter 2: Isolde's Trial of Standing in the Glasshouse, her palace, and the
@ Pact of the Open Field.
@ Object placement: docs/dialogue/chapter2.md#the-glasshouse

@ ---------------------------------------------------------------------------
@ The herald at the door (the Gym guide's job).
@ ---------------------------------------------------------------------------
Glasshouse_EventScript_Herald::
	lock
	faceplayer
	setspeaker Slice_Name_Herald
	goto_if_set FLAG_BADGE02_GET, Glasshouse_EventScript_HeraldAfter
	msgbox Glasshouse_Text_HeraldWelcome, MSGBOX_DEFAULT
	setspeaker Slice_Name_Herald
	msgbox Glasshouse_Text_HeraldAdvice, MSGBOX_DEFAULT
	release
	end

Glasshouse_EventScript_HeraldAfter::
	msgbox Glasshouse_Text_HeraldAfter, MSGBOX_DEFAULT
	release
	end

@ ---------------------------------------------------------------------------
@ The court: two gardeners and a courtier.
@ ---------------------------------------------------------------------------
Glasshouse_EventScript_Gardener1::
	trainerbattle_single TRAINER_PALACE_GARDENER_1, Glasshouse_Text_Gardener1Intro, Glasshouse_Text_Gardener1Defeat
	npc Ch2_Name_Gardener, Glasshouse_Text_Gardener1PostBattle
	end

Glasshouse_EventScript_Gardener2::
	trainerbattle_single TRAINER_PALACE_GARDENER_2, Glasshouse_Text_Gardener2Intro, Glasshouse_Text_Gardener2Defeat
	npc Ch2_Name_Gardener, Glasshouse_Text_Gardener2PostBattle
	end

Glasshouse_EventScript_Courtier::
	trainerbattle_single TRAINER_PALACE_COURTIER, Glasshouse_Text_CourtierIntro, Glasshouse_Text_CourtierDefeat
	npc Ch2_Name_Courtier, Glasshouse_Text_CourtierPostBattle
	end

@ ---------------------------------------------------------------------------
@ Isolde. Trial 2. Win -> Thornfield Seal (badge 2), the pact, and Isolde's
@ private talk about the bell and the sealed grove (VAR_THORNFIELD_STATE 2 -> 3).
@ ---------------------------------------------------------------------------
Glasshouse_EventScript_Isolde::
	lock
	faceplayer
	goto_if_set FLAG_BADGE02_GET, Glasshouse_EventScript_IsoldeAfter
	say_g Slice_Name_Isolde, Glasshouse_Text_IsoldeYouCame_M, Glasshouse_Text_IsoldeYouCame_F
	narrate Glasshouse_Text_FenwickReadsCase
	narrate Glasshouse_Text_MabrySpeaks
	say Slice_Name_Isolde, Glasshouse_Text_IsoldeSentimental
	say Slice_Name_Isolde, Glasshouse_Text_IsoldeLetTheTrialDecide
	closemessage
	trainerbattle_no_intro TRAINER_ISOLDE, Glasshouse_Text_IsoldeDefeat
	goto Glasshouse_EventScript_IsoldeDefeated
	end

Glasshouse_EventScript_IsoldeDefeated::
	narrate Glasshouse_Text_Silence
	say Slice_Name_Isolde, Glasshouse_Text_IsoldeWhereAreThey
	narrate Glasshouse_Text_YouTellHer
	say Slice_Name_Isolde, Glasshouse_Text_IsoldeInTheMorning
	narrate Glasshouse_Text_FenwickStepsForward
	say Ch2_Name_Fenwick, Glasshouse_Text_PactTerms
	say Ch2_Name_Fenwick, Glasshouse_Text_FenwickFourthTerm
	setspeaker SP_NAME_NONE
	message Glasshouse_Text_ChooseTerms
	waitmessage
	dynmultichoice 0, 0, TRUE, 3, 0, DYN_MULTICHOICE_CB_NONE, Glasshouse_Text_TermsRestitution, Glasshouse_Text_TermsReform, Glasshouse_Text_TermsClemency
	@ Options are in PACT_TERMS_* order. signpact sets FLAG_PACT_THORNFIELD and
	@ raises VAR_LOWMERE_STAGE when enough pacts are signed.
	signpact PACT_THORNFIELD, VAR_RESULT
	getpactterms PACT_THORNFIELD
	switch VAR_RESULT
	case PACT_TERMS_RESTITUTION, Glasshouse_EventScript_TermsRestitution
	case PACT_TERMS_REFORM, Glasshouse_EventScript_TermsReform
	goto Glasshouse_EventScript_TermsClemency
	end

Glasshouse_EventScript_TermsRestitution::
	say Ch2_Name_Fenwick, Glasshouse_Text_FenwickRestitution
	say Slice_Name_Isolde, Glasshouse_Text_IsoldeRestitution
	goto Glasshouse_EventScript_SignPact
	end

Glasshouse_EventScript_TermsReform::
	say Ch2_Name_Fenwick, Glasshouse_Text_FenwickReform
	say Slice_Name_Isolde, Glasshouse_Text_IsoldeReform
	goto Glasshouse_EventScript_SignPact
	end

Glasshouse_EventScript_TermsClemency::
	say Ch2_Name_Fenwick, Glasshouse_Text_FenwickClemency
	say Slice_Name_Isolde, Glasshouse_Text_IsoldeClemency
	goto Glasshouse_EventScript_SignPact
	end

Glasshouse_EventScript_SignPact::
	narrate Glasshouse_Text_IsoldeSigns
	say Slice_Name_Isolde, Glasshouse_Text_IsoldeHereIsYourSeal
	setspeaker SP_NAME_NONE
	message Glasshouse_Text_ReceivedSeal
	waitmessage
	call Common_EventScript_PlayGymBadgeFanfare
	setflag FLAG_BADGE02_GET
	closemessage
	fadescreen FADE_TO_BLACK
	delay 30
	fadescreen FADE_FROM_BLACK
	narrate Glasshouse_Text_Later
	say Slice_Name_Isolde, Glasshouse_Text_IsoldeStillHere
	narrate Glasshouse_Text_ShowBell
	say Slice_Name_Isolde, Glasshouse_Text_IsoldeGuildFoundHim
	say Slice_Name_Isolde, Glasshouse_Text_IsoldeCorwinsBell
	say Slice_Name_Isolde, Glasshouse_Text_IsoldeTheGrove
	say Slice_Name_Isolde, Glasshouse_Text_IsoldeWhyTellYou
	say Slice_Name_Guard, Glasshouse_Text_GuardVossGone
	say Slice_Name_Isolde, Glasshouse_Text_IsoldeGoToTheWillows
	closemessage
	setvar VAR_THORNFIELD_STATE, 3
	release
	end

Glasshouse_EventScript_IsoldeAfter::
	npc Slice_Name_Isolde, Glasshouse_Text_IsoldeAfter
	end

@ The glass walls (bg events).
Glasshouse_EventScript_GlassWall::
	goto_if_set FLAG_BADGE02_GET, Glasshouse_EventScript_GlassWallAfter
	sign Glasshouse_Text_GlassWallBefore
	end

Glasshouse_EventScript_GlassWallAfter::
	sign Glasshouse_Text_GlassWallAfter
	end

@ ---------------------------------------------------------------------------
@ Text
@ ---------------------------------------------------------------------------

Glasshouse_Text_HeraldWelcome::
	.string "Welcome to the GLASSHOUSE, Your\n"
	.string "Highness. Please don't touch the\l"
	.string "orchids. They bite.\p"
	.string "Her Highness has gathered the whole\n"
	.string "court for your TRIAL. She says you\l"
	.string "deserve an audience, even for this.$"

Glasshouse_Text_HeraldAdvice::
	.string "If I may. Her Highness keeps GRASS\n"
	.string "POKéMON. FIRE, FLYING, ICE, POISON and\l"
	.string "BUG all do very well against them.\p"
	.string "Those are not words I'm allowed to say\n"
	.string "in here. You didn't hear them from me.\p"
	.string "And beware the last bloom. It's always\n"
	.string "the loveliest, and the most dangerous.$"

Glasshouse_Text_HeraldAfter::
	.string "Her Highness went down to the fields\n"
	.string "this morning.\p"
	.string "The court doesn't know what to do with\n"
	.string "itself.$"

Glasshouse_Text_Gardener1Intro::
	.string "Mind the seedlings! Mind the... oh, it's a\n"
	.string "TRIAL. Then mind my POKéMON!$"

Glasshouse_Text_Gardener1Defeat::
	.string "Trampled. Just like the seedlings.$"

Glasshouse_Text_Gardener1PostBattle::
	.string "I grow orchids for Her Highness. Took me\n"
	.string "eleven years to learn how.\p"
	.string "The tenants grew beans. Took them\n"
	.string "about eleven minutes to lose them.$"

Glasshouse_Text_Gardener2Intro::
	.string "Every flower in this hall came from a\n"
	.string "field that used to feed somebody. Did\l"
	.string "you know that?\p"
	.string "I do now. Let's see if you can make me\n"
	.string "forget.$"

Glasshouse_Text_Gardener2Defeat::
	.string "No, I still remember. Go on.$"

Glasshouse_Text_Gardener2PostBattle::
	.string "OLD MABRY trained all of us. If he's\n"
	.string "turned against the palace, the rest of\l"
	.string "us aren't far behind.$"

Glasshouse_Text_CourtierIntro::
	.string "A poem in your honour! “A muddy heir\n"
	.string "came up the stair...”\p"
	.string "Oh, just battle me.$"

Glasshouse_Text_CourtierDefeat::
	.string "“...and lost to me with dignity.” No.\n"
	.string "That's not how it goes at all.$"

Glasshouse_Text_CourtierPostBattle::
	.string "We applaud Her Highness's poems every\n"
	.string "evening. It's the only work most of us\l"
	.string "do.$"

Glasshouse_Text_IsoldeYouCame_M::
	.string "Little brother. You came after all.\p"
	.string "I had the whole court gather. I didn't\n"
	.string "want you to lose in an empty room. That\l"
	.string "would be too sad.$"

Glasshouse_Text_IsoldeYouCame_F::
	.string "Little sister. You came after all.\p"
	.string "I had the whole court gather. I didn't\n"
	.string "want you to lose in an empty room. That\l"
	.string "would be too sad.$"

Glasshouse_Text_FenwickReadsCase::
	.string "MAGISTRATE FENWICK reads out the\n"
	.string "case. Forty-one plots. The interest on\l"
	.string "the rent. The steward's ledger, and the\l"
	.string "writ from the WILLOWS' door, signed\l"
	.string "ISOLDE.$"

Glasshouse_Text_MabrySpeaks::
	.string "Then OLD MABRY stands up in his good\n"
	.string "coat and tells the court about the\l"
	.string "WILLOWS' beans.\p"
	.string "The court goes very quiet.$"

Glasshouse_Text_IsoldeSentimental::
	.string "MABRY, you've always been sentimental.\n"
	.string "It's why you're good with roses.\p"
	.string "They were only papers. VOSS said they\n"
	.string "were only papers.$"

Glasshouse_Text_IsoldeLetTheTrialDecide::
	.string "...Very well. Let the TRIAL decide.\n"
	.string "That's what it's for.\p"
	.string "SHAYMIN, come. Let's show them\n"
	.string "something beautiful.$"

Glasshouse_Text_IsoldeDefeat::
	.string "Oh. Oh, I see.$"

Glasshouse_Text_Silence::
	.string "Nobody in the court applauds. Nobody\n"
	.string "knows if they're allowed to.$"

Glasshouse_Text_IsoldeWhereAreThey::
	.string "The WILLOWS. Where did they go? The\n"
	.string "family from plot nineteen?$"

Glasshouse_Text_YouTellHer::
	.string "You tell her about the camp at the edge\n"
	.string "of town, and the boy who asked to take\l"
	.string "his beans.$"

Glasshouse_Text_IsoldeInTheMorning::
	.string "I sign them in the morning, with my tea.\n"
	.string "A whole stack. VOSS brings them up on a\l"
	.string "silver tray.\p"
	.string "I never once went down to see. I\n"
	.string "suppose I made sure I didn't have to.$"

Glasshouse_Text_FenwickStepsForward::
	.string "MAGISTRATE FENWICK steps down from\n"
	.string "the gallery with the pact.$"

Glasshouse_Text_PactTerms::
	.string "By the old law, the Pact of the Open\n"
	.string "Field. One: every plot seized for debt\l"
	.string "returns to its tenant.\p"
	.string "Two: THORNFIELD sends seed and\n"
	.string "gardeners to LOWMERE, every season.\p"
	.string "Three: Her Highness answers to this\n"
	.string "pact before the Crown.$"

Glasshouse_Text_FenwickFourthTerm::
	.string "The fourth term is the victor's to set,\n"
	.string "Your Highness.$"

Glasshouse_Text_ChooseTerms::
	.string "What will you add to the pact?$"

Glasshouse_Text_TermsRestitution::
	.string "ISOLDE pays LOWMERE$"

Glasshouse_Text_TermsReform::
	.string "Cancel every tenant's debt$"

Glasshouse_Text_TermsClemency::
	.string "Let her give it as a gift$"

Glasshouse_Text_FenwickRestitution::
	.string "Four. Her Highness pays LOWMERE from\n"
	.string "her own purse: orchard stock, and a full\l"
	.string "year's seed, on top of the rest.$"

Glasshouse_Text_IsoldeRestitution::
	.string "My purse? I've never paid for anything\n"
	.string "myself. I'm not sure where it's kept.\p"
	.string "...I'll find it. That's fair.$"

Glasshouse_Text_FenwickReform::
	.string "Four. Every tenant's debt in\n"
	.string "THORNFIELD is cancelled, and the\l"
	.string "steward's ledgers are burned in the\l"
	.string "square.$"

Glasshouse_Text_IsoldeReform::
	.string "All of them? Every page with my name on\n"
	.string "it?\p"
	.string "...Yes. Burn them. I don't want to read\n"
	.string "them again.$"

Glasshouse_Text_FenwickClemency::
	.string "Four. The land is returned as Her\n"
	.string "Highness's own gift, announced in her\l"
	.string "name. THORNFIELD will not hear that it\l"
	.string "was ordered.$"

Glasshouse_Text_IsoldeClemency::
	.string "You'd let me keep my dignity? After all\n"
	.string "of this?\p"
	.string "That's very kind. I don't think I'd have\n"
	.string "done the same for you.$"

Glasshouse_Text_IsoldeSigns::
	.string "ISOLDE signs in green ink. Her hand is\n"
	.string "shaking, so the letters come out wrong.$"

Glasshouse_Text_IsoldeHereIsYourSeal::
	.string "Here. The THORNFIELD SEAL. Take care of\n"
	.string "it. Better care than I took of anything.$"

Glasshouse_Text_ReceivedSeal::
	.string "{PLAYER} received the THORNFIELD SEAL\n"
	.string "from ISOLDE!$"

Glasshouse_Text_Later::
	.string "Later. The court has gone. The evening\n"
	.string "sun turns the glass walls gold.$"

Glasshouse_Text_IsoldeStillHere::
	.string "You're still here. I thought you'd go and\n"
	.string "gloat with Corwin.$"

Glasshouse_Text_ShowBell::
	.string "You tell her about the small pale bell\n"
	.string "stamped inside the steward's ledger.$"

Glasshouse_Text_IsoldeGuildFoundHim::
	.string "A bell? VOSS's books came with him. The\n"
	.string "GILDED SCALE found him for me, two\l"
	.string "years ago.\p"
	.string "They lent me the gardeners, too, and\n"
	.string "the glass for this hall. They were so\l"
	.string "generous. I never asked why.$"

Glasshouse_Text_IsoldeCorwinsBell::
	.string "Corwin wrote to me about a bell, after\n"
	.string "you beat him. I thought he was being\l"
	.string "dramatic. He usually is.$"

Glasshouse_Text_IsoldeTheGrove::
	.string "There's a grove behind the palace,\n"
	.string "behind the thorn gate. Mother sealed it\l"
	.string "before I was born.\p"
	.string "The flowers in there never close, not\n"
	.string "even at night. Even SHAYMIN won't go\l"
	.string "near it.$"

Glasshouse_Text_IsoldeWhyTellYou::
	.string "I don't know why I'm telling you this. I\n"
	.string "think I just wanted to tell someone a\l"
	.string "true thing today.$"

Glasshouse_Text_GuardVossGone::
	.string "Your Highness! The steward's gone. He\n"
	.string "left his keys on the desk and ran.$"

Glasshouse_Text_IsoldeGoToTheWillows::
	.string "Let him go. I have somewhere to be.\p"
	.string "I'm going down to see the WILLOWS. I\n"
	.string "should have gone two years ago.$"

Glasshouse_Text_IsoldeAfter::
	.string "I went down to the fields. My shoes are\n"
	.string "ruined. MABRY laughed at me. I deserved\l"
	.string "it.\p"
	.string "I'm writing a poem about the WILLOWS.\n"
	.string "I'm not going to read it to anyone.$"

Glasshouse_Text_GlassWallBefore::
	.string "Panes of glass, floor to ceiling. Behind\n"
	.string "them, flowerbeds stretch down the hill\l"
	.string "in neat green steps.$"

Glasshouse_Text_GlassWallAfter::
	.string "Someone has opened a pane. By the\n"
	.string "throne, there's a clay pot with a single\l"
	.string "bean plant in it.$"

