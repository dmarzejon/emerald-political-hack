# Game systems (political hack)

Engine support for the political-thriller hack: names the map, dialogue, balance and story
work can rely on. The features thread owns `src/` and `include/` and keeps this page current.
Story context: `docs/story/vertical-slice.md` (the story bible).

Need a new flag or var? Ask the features thread rather than taking an `UNUSED` one, so two
threads never claim the same ID. Free story flags are `0x46`-`0x4F` in
`include/constants/flags.h`; free vars are `0x40FD`-`0x40FF` in `include/constants/vars.h`.

## Flags

Defined in `include/constants/flags.h`. Scripts use them with `setflag`, `checkflag`,
`goto_if_set` and so on.

| Flag | Set when |
| --- | --- |
| `FLAG_RECEIVED_SILVER_WING` | Mother Hesk gives the Silver Wing |
| `FLAG_RECEIVED_KINGS_GIFT` | The player picks Entei, Raikou or Suicune |
| `FLAG_FOUND_BELL_RECEIPT` | Mire Road wagon event |
| `FLAG_HAYMARKET_WEIGHTS_FOUND` | Granary evidence found |
| `FLAG_HAYMARKET_WITNESS` | Widow Penn agrees to testify |
| `FLAG_HAYMARKET_TRIAL_GRANTED` | Magistrate Arden grants the Trial; the Gilt Pavilion opens |
| `FLAG_PACT_HAYMARKET` | Pact signed after Trial 1 (Corwin). Set by `signpact`, not by hand |
| `FLAG_PACT_THORNFIELD` | Pact after Trial 2 (Isolde) |
| `FLAG_PACT_CRAGHOLT` | Pact after Trial 3 (Brannoc) |
| `FLAG_PACT_SALTMERE` | Pact after Trial 4 (Marisol) |
| `FLAG_PACT_VOLTAINE` | Pact after Trial 5 (Teodor) |
| `FLAG_PACT_LUMENHALL` | Pact after Trial 6 (Seraphine) |
| `FLAG_PACT_EMBERFORT` | Pact after Trial 7 (Garrick) |
| `FLAG_PACT_DUSKMOOR` | Pact after Trial 8 (Vespera) |
| `FLAG_PACT_HIGHCREST` | Pact after Trial 9 (Aurelian) |
| `FLAG_LOWMERE_SIEGE_SURVIVED` | Lowmere survives the siege (needed for stage 4) |

The dialogue thread's slice scripts also use these, documented in `docs/dialogue/events.md`:
`FLAG_PENN_MON_RECOVERED`, `FLAG_RECEIVED_SEALED_LETTER`, `FLAG_TALKED_TO_CARTER`,
`FLAG_MET_CRANE`, `FLAG_CORWIN_PRIVATE_TALK`, `FLAG_TAMSIN_MIRE_ROAD_TALK`, and the NPC hide flags
`FLAG_HIDE_LOWMERE_ARRIVAL_BRAM`, `FLAG_HIDE_OLD_LODGE_TOFT`, `FLAG_HIDE_OLD_LODGE_BRAM`,
`FLAG_HIDE_OLD_LODGE_COURIER`, `FLAG_HIDE_LOWMERE_TAMSIN_FORGE`, `FLAG_HIDE_LOWMERE_TAMSIN_ROAD`,
`FLAG_HIDE_MIRE_ROAD_TAMSIN`, `FLAG_HIDE_MIRE_ROAD_WAGON_CLERKS`,
`FLAG_HIDE_HAYMARKET_SEIZURE_CLERKS`, `FLAG_HIDE_HAYMARKET_CORWIN_SQUARE`,
`FLAG_HIDE_HAYMARKET_CRANE`, `FLAG_HIDE_COUNTING_HOUSE_PENN_BALL`,
`FLAG_HIDE_PAVILION_CORWIN_AFTER`, `FLAG_HIDE_LOWMERE_GRAIN_CARTS`,
`FLAG_HIDE_MIRE_ROAD_TAMSIN_WAGON`, `FLAG_HIDE_HAYMARKET_TAMSIN` (`0x30`-`0x45`).
A new game starts with these hidden (set): Old Lodge Bram and courier, Lowmere road Tamsin,
both Mire Road Tamsins, Haymarket Tamsin, Haymarket Crane and the Lowmere grain carts
(`SetSliceStartFlags` in `src/new_game.c`). The other hide flags start clear.

The badge for each Trial is still the vanilla `FLAG_BADGE0x_GET`; the pact flag is separate,
so a scene can happen between the win and the signing.

## Vars

Defined in `include/constants/vars.h`. They are saved with the game.

| Var | Values |
| --- | --- |
| `VAR_LOWMERE_STAGE` | Lowmere's upgrade stage, `LOWMERE_STAGE_0` to `LOWMERE_STAGE_4`. Raised automatically by `signpact` |
| `VAR_LOWMERE_STAGE_SEEN` | The last stage the player was shown an upgrade scene for |
| `VAR_SLICE_STATE` | Main story step through the slice; the dialogue thread lists the values in its docs |
| `VAR_KINGS_GIFT_DOG` | `KINGS_GIFT_ENTEI` (0), `KINGS_GIFT_RAIKOU` (1), `KINGS_GIFT_SUICUNE` (2) |
| `VAR_PACT_TERMS_1`, `VAR_PACT_TERMS_2` | Internal storage for pact terms. Read them with `getpactterms` |

Constants for these are in `include/constants/town_upgrade.h`, which scripts can use directly.

## Lowmere's upgrade stages

Lowmere is the Littleroot Town map (`MAP_LITTLEROOT_TOWN`; if the map is renamed to
`MAP_LOWMERE`, the engine follows it). Each time the map loads, the engine picks the layout
for the current `VAR_LOWMERE_STAGE`:

| Stage | Reached when | Layout used |
| --- | --- | --- |
| 0 | Start | `LAYOUT_LOWMERE_STAGE0` |
| 1 | 1 pact signed (Haymarket) | `LAYOUT_LOWMERE_STAGE1` |
| 2 | 3 pacts | `LAYOUT_LOWMERE_STAGE2` |
| 3 | 6 pacts | `LAYOUT_LOWMERE_STAGE3` |
| 4 | 8 pacts and `FLAG_LOWMERE_SIEGE_SURVIVED` | `LAYOUT_LOWMERE_STAGE4` |

**For the map thread:** add each stage as its own layout in `data/layouts/layouts.json` with
exactly the ids above. No code change is needed; the engine finds them by name. A stage
whose layout doesn't exist yet uses the map's own layout from `map.json`. All stage layouts
must be the **same width and height** as the map's own layout, because warps, NPCs, signs
and the connection to the Mire Road are shared by every stage. NPCs and objects that only
appear at some stages are hidden with flags or map scripts in the usual way.

The swap happens whenever the map is loaded: on a warp, on walking in from the Mire Road,
and when the game is continued from a save. Neighbouring maps also see the right stage
across the border. If the stage changes while the player is standing in Lowmere, the town
updates the next time the map loads. A cutscene that wants to show the change on the spot
can `warpsilent` to Lowmere at the player's current position after the stage changes.

### Upgrade scene

To play Reeve Toft's announcement the first time the player sees a new stage, a Lowmere map
script can do:

```
LowmereTown_OnFrame:
	map_script_2 VAR_TEMP_0, 0, LowmereTown_CheckUpgrade
	.2byte 0

LowmereTown_CheckUpgrade:
	setvar VAR_TEMP_0, 1
	goto_if_lowmere_stage_new LowmereTown_UpgradeScene
	end

LowmereTown_UpgradeScene:
	lockall
	@ ... the scene for the new VAR_LOWMERE_STAGE ...
	lowmere_mark_stage_seen
	releaseall
	end
```

### Setting the stage by hand

`signpact` keeps the stage in step with the pacts, and it never lowers the stage. A story
beat can also `setvar VAR_LOWMERE_STAGE, LOWMERE_STAGE_2` directly. After setting
`FLAG_LOWMERE_SIEGE_SURVIVED`, call `special UpdateLowmereStage` to apply stage 4
(`VAR_RESULT` is `TRUE` if the stage went up).

## Pacts after a Trial of Standing

After the player beats a sibling in their palace, the dialogue script lets the player dictate
the pact, then signs it. Every pact sends resources to Lowmere (that is what drives the
stages). The player's stance on top of that is recorded as the pact's **terms**:

| Terms | Meaning (story and dialogue decide the wording for each town) |
| --- | --- |
| `PACT_TERMS_RESTITUTION` | The sibling pays: extra goods for Lowmere |
| `PACT_TERMS_REFORM` | The sibling's town is fixed: its people benefit |
| `PACT_TERMS_CLEMENCY` | The sibling keeps face: goodwill, a possible ally later |

Towns, in Trial order: `PACT_HAYMARKET`, `PACT_THORNFIELD`, `PACT_CRAGHOLT`, `PACT_SALTMERE`,
`PACT_VOLTAINE`, `PACT_LUMENHALL`, `PACT_EMBERFORT`, `PACT_DUSKMOOR`, `PACT_HIGHCREST`.

Script commands (from `asm/macros/event.inc`):

| Command | What it does |
| --- | --- |
| `signpact TOWN, TERMS` | Sets the town's `FLAG_PACT_*`, records the terms and raises `VAR_LOWMERE_STAGE`. `TERMS` may be a var such as `VAR_RESULT`. Afterwards `VAR_RESULT` is Lowmere's stage |
| `getpactterms TOWN` | `VAR_RESULT` = that town's `PACT_TERMS_*`, or `PACT_TERMS_NONE` if no pact yet |
| `special GetSignedPactCount` | `VAR_RESULT` = number of pacts signed |
| `special UpdateLowmereStage` | Re-checks the stage (see above) |
| `goto_if_lowmere_stage_new LABEL` | Jumps if Lowmere has a stage the player hasn't been shown |
| `lowmere_mark_stage_seen` | Marks the current stage as shown |

Example after Corwin's Trial, using the built-in menu (`MULTI_PACT_TERMS` lists
Restitution, Reform, Clemency, in `PACT_TERMS_*` order):

```
	msgbox GiltPavilion_Text_DictateTerms, MSGBOX_DEFAULT
	multichoice 17, 6, MULTI_PACT_TERMS, TRUE
	signpact PACT_HAYMARKET, VAR_RESULT
```

For town-specific wording, use `dynmultichoice` with your own three options in the same
order, then `signpact` with `VAR_RESULT` the same way. Later scenes branch on the choice with
`getpactterms PACT_HAYMARKET` and `goto_if_eq VAR_RESULT, PACT_TERMS_REFORM, ...`.

## Prince or princess in text

The player's boy/girl choice at the start of the game decides their title. These codes work
in any field text (`msgbox`, signs, trainer intro and defeat lines):

| Code | Boy | Girl |
| --- | --- | --- |
| `{TITLE}` | prince | princess |
| `{TITLE_CAP}` | Prince | Princess |
| `{SIBLING}` | brother | sister |
| `{CHILD}` | son | daughter |

Example: `"Look, everyone, the Marsh {TITLE_CAP}!"`. "Your Highness" is the same for both
and needs no code. `{PLAYER}` is still the player's name. For anything else that differs by
gender, branch with `checkplayergender`.

## Opening: "Who are you?"

A new game opens on a black screen instead of Professor Birch: narration only, then the
choice of prince or princess (this sets the gender used by `{TITLE}`), then the player's
name (default **Rowan** for both). The game then starts as usual, so the court
prologue is the first map script that runs. Birch, Lotad and the Poké Ball are never shown,
and the gender menu reads Prince / Princess. The text belongs to the dialogue thread: every
`gText_Birch_*` label in `data/text/birch_speech.inc` is shown in the vanilla order (Welcome,
Pokemon, MainSpeech, AndYouAre, BoyOrGirl, WhatsYourName, SoItsPlayer, YourePlayer, AreYouReady).

## Map names

The map popups and region map show **LOWMERE** (Littleroot Town's slot), **MIRE ROAD**
(Route 101) and **HAYMARKET** (Oldale Town). Map constants keep their vanilla names.

## Key items

| Item | Given when | Icon (placeholder) |
| --- | --- | --- |
| `ITEM_SILVER_WING` | Mother Hesk gives the keepsake (`FLAG_RECEIVED_SILVER_WING`) | Pretty Feather |
| `ITEM_BELL_RECEIPT` | Mire Road wagon event (`FLAG_FOUND_BELL_RECEIPT`) | Bike Voucher |
| `ITEM_SEALED_LETTER` | Found on Crane's desk after the Haymarket pact | Letter |

All three are key items with no use from the bag. Give them with `giveitem` as usual.
