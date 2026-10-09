# Vertical slice: Lowmere, Mire Road, Haymarket

The first playable chunk: from the opening cutscene to Lowmere's first upgrade. It should
show every pillar of the game once: a hometown that changes, a route, a town with a
corrupt sibling, a case to build, a Trial in a royal palace, a pact, and a glimpse of the
cult.

Target length: about 60-90 minutes for a first-time player.

Owner notes: the map thread owns layouts, dialogue owns final text, balance owns teams and
levels, features owns flags and vars. Names in `code` below are suggestions for features
to adopt or rename; please record the final names in `docs/systems.md`.

## Scene list

### 1. Prologue: the Apportionment (cutscene)
- Black screen: "Who are you?" The player chooses boy or girl and their name (replaces
  Birch's intro). This comes first so the court scene can use the right name and title.
- Crownspire throne room. Chancellor Venn reads the Apportionment, using the player's
  name. Corwin's joke about
  the mere. Isolde looks embarrassed for the player. Aurelian smiles politely. The
  king's chair is empty.
- Venn, quietly, to the player as the court leaves: "Lowmere has good air, they say.
  Rest well there, Your Highness." (First Choir phrasing, unnoticed.)
- The throne room is a scripted scene, not an explorable map. It can be a series of text
  boxes over a still image if a full map is too much for the slice.

### 2. Arrival in Lowmere (Stage 0)
- The player returns by cart with Bram Ashdown, six years after the Crown took them to
  court. Townsfolk remember them as a child; some are glad, some resent the title. Lowmere: six houses, three boarded up,
  a broken well, an empty market square, a collapsed jetty, marsh all around.
- Bram walks the player to **the Old Lodge**, the shabby house that is now the "royal
  residence". Replaces the player's house.
- Reeve Toft greets the player with a ledger: the town has eleven days of grain left.
  Haymarket's prices have tripled this year. Sets the goal: **get Lowmere fed**.
- NPCs: Tamsin at the forge, Mother Hesk in Elena's old house, three or four villagers
  with complaints (rotten jetty, no healer, no shop, bad dreams).

### 3. Choosing a partner
- Bram takes the player to the **Ranger's Shed** (replaces Birch's Lab). Three Pokémon
  he has been caring for. Player picks one. Bram: "Your mother would have made you
  pick the one that was afraid of you. I'm not her. Pick the one you like."
- Mother Hesk gives the **Silver Wing**: "Your mother's. She said you'd want it one day."
  It's a key item with a flavour description only for now; in Act 3 it calls Lugia.
  `FLAG_RECEIVED_SILVER_WING`.

### 4. Tamsin: first rival battle
- Tamsin has the Pokémon that is strong against the player's (as in Emerald). She
  challenges the player at the edge of town to see "if a prince (princess) is worth
  following up the road". First battle. This comes before the king's gift so the rival
  fight stays starter against starter.

### 5. The king's gift
- Back at the Old Lodge, a royal courier is waiting, nervous and muddy. He carries a
  letter sealed with the king's personal signet, not the Chancellery's, and three
  Poké Balls from the royal kennels.
- The letter is short and shaky, in Aldric's own hand: he is sorry, he is not well, he
  could not stop the Apportionment, and he wants his child to have something of his.
  "Choose the one that suits you. Your mother always said I chose badly for myself."
- The player chooses **Entei, Raikou or Suicune**. `VAR_KINGS_GIFT_DOG` (0 Entei, 1
  Raikou, 2 Suicune) and `FLAG_RECEIVED_KINGS_GIFT`.
- The courier takes the other two Poké Balls back to the royal kennels. (Payoff: in
  Act 4, Chancellor Venn's team includes both of them.)
- How the gift got out: the kennel master was Elena's friend and slipped the three
  Balls to the courier on the king's word. Venn found out and stopped the courier at
  the city gate, then let him go: "The king's gift is the king's affair." The courier
  repeats this, shaken. Venn's choice is deliberate: a bastard with a legendary will
  stir up the heirs, which suits the Choir's plan to set them against each other.
- Bram reads the letter over the player's shoulder and goes quiet. "He wrote this
  himself. He hasn't written anything himself in years." First hint that the king is
  not simply mad.
- Balance note: the dog must not trivialise the first Trial. Suggested approach: it
  arrives at a low level, close to the starter's (balance decides).
- Tamsin then joins the player on the walk to the Mire Road.

### 6. The Mire Road (route)
- Suggested slot: Route 101, extended into a longer marsh path with boardwalks, tall
  grass and a few shallow pools.
- **Wild Pokémon:** marsh and roadside species, early-game level. Balance owns the list.
  Tone: muddy, buggy, water's edge.
- **Trainers (3-4):** a Gilded Scale clerk ("Haymarket doesn't take IOUs"), a poacher, a
  youngster who wants to see a real prince, and a grain carter who quit over the scales.
- **Event, mid-route:** a Gilded Scale wagon stuck in the mud, guarded by two
  "clerks". The wagon is full of **Lowmere's own tithe grain**, collected by the guild
  from the marsh farms and hauled to Haymarket, where it is sold back to Lowmere at triple
  price over rigged scales. One clerk battles the player. The other drops a
  receipt stamped with a **pale bell** seal. `FLAG_FOUND_BELL_RECEIPT`. This receipt is
  later evidence in the case.
- The carter NPC tells the player that the scales in Haymarket are rigged, and that
  the old weights were swapped this spring. First lead.

### 7. Haymarket: arrival
- A bustling market town with a permanent fairground, stalls, a granary, the Gilded
  Scale's counting house, a Pokémon Center and Mart, and Corwin's palace, the **Gilt
  Pavilion**, which replaces the Gym. It is built like an auction house.
- Corwin is in the square throwing a fair. He spots the player and makes a show of it:
  "Look, everyone, the Marsh Prince (Princess)! Has Lowmere sent us its finest frog?" The crowd
  laughs. He refuses a Trial: "On what grounds? That you're poor?"
- Goal: **build a case** for the magistrate.

### 8. Building the case
On first entering the square, the player watches two guild clerks take Widow Penn's
Pokémon from her stall as "payment" while the crowd looks away (on-screen scene; see
the tone rules in [README.md](README.md#tone)).

Three pieces of evidence. Any order. Each is short.

1. **The receipt** from the Mire Road (already held).
2. **The false weights.** In the granary, a small push-the-sacks puzzle reaches the
   scale room. Two sets of weights: the honest Crown weights in a locked crate, the
   light ones in use. Guarded by a Gilded Scale foreman (battle).
   `FLAG_HAYMARKET_WEIGHTS_FOUND`.
3. **A witness.** A stallholder, Widow Penn, has been fined twice for complaining. She
   will only testify if the player recovers her Pokémon, seized as "payment" and kept in
   the counting house's back room. A short sneak-in with one or two clerk battles.
   `FLAG_HAYMARKET_WITNESS`.

Townsfolk dialogue should make it clear that most people like Corwin and blame the
guild, not him. The case should show that Corwin knows and takes his cut: greedy
rather than cruel, but corrupt.

### 9. The magistrate
- Magistrate Lyle Arden, an honest old man worn down by the guild. When shown all three
  pieces he authorises a **Trial of Standing**: "The law is old, Your Highness, but it
  is still the law." `FLAG_HAYMARKET_TRIAL_GRANTED`; opens the palace doors.
- Silas Crane, the guildmaster, is waiting outside. Smiling, he offers the player a
  "royal discount" on grain if they withdraw. Refusing is the only option. Crane:
  "Then rest well tonight, little prince (princess)."

### 10. The Trial: Corwin's palace, the Gilt Pavilion
- Theme: a gaudy palace built around an auction floor. The trainers on the way in are
  Corwin's courtiers: **brokers** and **bidders**. The Trial is fought on the auction
  floor in front of the throne, with Haymarket's townsfolk packed into the galleries.
- Puzzle idea for the map thread: the floor is a grid of lot numbers. An auctioneer calls
  numbers; stepping on the called lots opens the gates, wrong lots make a bidder
  challenge you. Simpler fallback: a straight run of 3 trainers with gate switches.
- **Corwin, Trial 1, Normal type.** Fights with showmanship: his Pokémon are
  "lots" he presents to the crowd. Balance owns the team. Difficulty should be the
  first real wall, as Roxanne is in Emerald. His ace is his bestowed legendary,
  **Type: Null**, unveiled from under a sheet as the "final lot". Balance should keep
  its level in line with the rest of his team.
- **Before the battle:** "Fine! Let's give them a show. When I win, you go back to your
  puddle and you thank me for the grain."
- **After the battle:** the crowd goes quiet. Corwin: "...They were cheering for you."
  He signs the pact, angrily, and the player gets the **Haymarket Seal** (badge 1).
  `FLAG_BADGE01_GET` as usual.

### 11. The Pact of Fair Measure
- Terms: honest Crown weights restored, the Gilded Scale's grain contract cancelled,
  and Haymarket sells to Lowmere at fair price.
- Corwin, privately, after the crowd leaves: he genuinely didn't know about the bell
  seal. He's rattled by it. "Crane said they were just investors." First crack in him.
- Crane is gone. His office is cleared out. On the desk, a letter with a pale bell seal,
  unopened, addressed to no one. The player can't open it yet (key item for later).
  `FLAG_PACT_HAYMARKET`.

### 12. Return to Lowmere (Stage 1)
- Grain carts arrive. Reeve Toft is giddy. The town upgrades to Stage 1 when the player
  re-enters (see below). Villagers comment on each change.
- Tamsin: "One down. Eight to go, Your Highness." She's being sarcastic. Mostly.
- Mother Hesk: the dreams in the village have gotten worse since the player came.
  (Cult hook.)
- **End of slice.** Bram points the player toward the road to Thornfield.

## Lowmere upgrade stages

The map thread builds Lowmere in at least these stages. Each stage is triggered by a pact
count, read from one var (suggested `VAR_LOWMERE_STAGE`, 0-4). The slice needs Stages 0
and 1 working; 2-4 can be sketched.

| Stage | Trigger | What visibly changes | Gameplay |
| --- | --- | --- | --- |
| 0 | Start | Broken well, boarded houses, empty square, collapsed jetty, mud paths | No Mart, no Center. Bram heals your party. |
| 1 | Pact with Haymarket (Trial 1) | Well repaired, grain store open, two market stalls in the square, one house reopened, fences mended | A small Mart (stall) opens. Villagers return. |
| 2 | Pacts with Thornfield and Cragholt (Trial 3) | Gardens and orchard plots, stone paths, jetty rebuilt with boats, Pokémon Center | Center opens. Berry plots. Boat to a new area. |
| 3 | Pacts through Lumenhall (Trial 6) | Town walls and gate, new houses, a schoolhouse, lanterns, the Old Lodge restored as a manor hall | Move tutor, more shops. The town looks like a town. |
| 4 | Siege survived, pacts through Duskmoor (Trial 8) | Banners of all allied towns, festival square, statue of Elena, a palace hall for Trials (closed until post-game) | Seat of the Ten-Town alliance. |

Each stage should look clearly different from the one before when the player walks in.
The first upgrade is the most important moment in the slice: it is the payoff for the
whole premise.

## Flags and vars the slice needs

Suggestions for the features thread.

| Name | Set when |
| --- | --- |
| `VAR_LOWMERE_STAGE` | 0 at start, 1 after the Haymarket pact |
| `FLAG_RECEIVED_SILVER_WING` | Mother Hesk gives the keepsake |
| `FLAG_RECEIVED_KINGS_GIFT` | The player picks the king's gift |
| `VAR_KINGS_GIFT_DOG` | 0 Entei, 1 Raikou, 2 Suicune |
| `FLAG_FOUND_BELL_RECEIPT` | Mire Road wagon event |
| `FLAG_HAYMARKET_WEIGHTS_FOUND` | Granary evidence |
| `FLAG_HAYMARKET_WITNESS` | Widow Penn agrees to testify |
| `FLAG_HAYMARKET_TRIAL_GRANTED` | Magistrate authorises the Trial; palace doors open |
| `FLAG_PACT_HAYMARKET` | Pact signed after Trial 1 (diplomacy flag) |

New key items: **Silver Wing** (exists in the expansion as an item; make it a key item), **Bell Receipt**, **Sealed
Letter**.

## Cast in the slice

| Character | Where | Role |
| --- | --- | --- |
| Player | All | Protagonist |
| Bram Ashdown | Lowmere | Mentor, gives the starter, heals party at Stage 0 |
| Tamsin Reed | Lowmere, Mire Road | Rival, first battle |
| Mother Hesk | Lowmere | Gives the Silver Wing, dream hook |
| Royal courier | Lowmere | Delivers the king's letter and gift |
| Reeve Abel Toft | Lowmere | Tracks the town; announces upgrades |
| Chancellor Venn | Prologue | Hidden cult leader |
| Corwin | Haymarket | Trial 1 (palace leader), Normal |
| Silas Crane | Haymarket | Gilded Scale guildmaster, Warden |
| Magistrate Lyle Arden | Haymarket | Grants the Trial |
| Widow Penn | Haymarket | Witness |
| Gilded Scale clerks/foreman | Mire Road, Haymarket | Grunt-style trainers |
| Grain carter | Mire Road | First lead |
