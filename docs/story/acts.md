# Act structure

Four acts, nine sibling Trials, one capital. Each town follows the same loop:

1. **Arrive.** See what is wrong with the town and how the sibling rules it.
2. **Build the case.** Investigate (talk, search, small puzzle, a cult or guild fight).
3. **Petition the magistrate.** The case earns the right to a Trial of Standing.
4. **Trial.** Fought in the throne hall of the sibling's palace, before their court.
   Winning earns the sibling's **seal**, which is the badge and the proof of the pact.
5. **Pact.** The town is reformed and something flows back to Lowmere. Lowmere
   upgrades at fixed points (see [vertical-slice.md](vertical-slice.md#lowmere-upgrade-stages)).

## Prologue: The Apportionment

Opens on a black screen: "Who are you?" The player chooses boy or girl and their name.
Then the court in Crownspire: Chancellor Venn reads out the Apportionment of the tenth
child: "To [Name] Marsh, called Valcourt, the town of Lowmere." Laughter from the
siblings. Corwin: "Does it have a town, or just the mere?" The king's chair is empty.
Cut to the player arriving in Lowmere by cart. This is the game's opening cutscene and
replaces the truck ride.

Soon after arrival, a royal courier brings the king's letter and gift: a choice of
**Entei, Raikou or Suicune** (see [vertical-slice.md](vertical-slice.md)).

## Act 1: The Least Heir (Trials 1-3)

The player learns how power works. Small towns, siblings who profit from their people, a merchant guild that
keeps turning up.

- **Haymarket, Corwin, Normal.** Rigged grain scales. See
  [vertical-slice.md](vertical-slice.md).
- **Thornfield, Isolde, Grass.** Tenant farmers bound in debt and evicted so Isolde can
  turn their land into gardens. The player
  notices a sealed grove behind her palace that no one is allowed to enter.
- **Cragholt, Brannoc, Rock.** Bonded convicts and debtors worked to death to beat ore
  quotas. The pale bell is found in a recent mine collapse, which was no accident: the
  Choir's warning to Brannoc when he balked at a new quota.
- **Act end:** Lowmere reaches Stage 2. Tamsin becomes the player's envoy. A letter
  sealed with a pale bell arrives: "The tenth child should stay small."

## Act 2: The Pacts (Trials 4-6)

The campaign gets noticed. The player is now a contender.

- **Saltmere, Marisol, Water.** Marisol's own smuggling ring: night shipments of
  Acolytes and sleeping draught, and families sold passage into servitude.
  First time the player hears "the Pale Choir".
- **Voltaine, Teodor, Electric.** Towns that can't pay, Lowmere included, are left dark.
  The grid also feeds something in Crownspire. Teodor shrugs: the Chancellery pays.
- **Midpoint: the Midsummer Court.** All ten heirs are summoned to Crownspire. The king
  appears in public for the first time in years, sees the player, says Elena's name,
  and collapses. For an instant everyone in the hall sees the shadow of Darkrai behind
  the throne. Venn declares a regency.
- **Lumenhall, Seraphine, Psychic.** The case: students and servants with holes in
  their memories. After the Trial, Seraphine tells the player the truth about the
  king, the Sleeper and their mother. She explains the Silver Wing, and that the grove
  behind Thornfield holds **Xerneas**, the one power that can undo the Sleeper's hold.
- **Seraphine taken.** That night **Yveltal** descends on Lumenhall and carries her off.
  The Choir has stopped hiding.
- **The grove.** The player returns to Thornfield. Isolde, bound by her pact, opens the
  sealed grove. **Xerneas** wakes for the player.
- **Act end:** Lowmere reaches Stage 3. The regency issues a decree revoking Lowmere's
  charter.

## Act 3: The Succession War (Trials 7-8)

The court stops pretending.

- **Siege of Lowmere.** Garrick enforces the regency's decree and marches on Lowmere,
  with Yveltal overhead. The town's pacts pay off: Corwin smuggles grain, Isolde's
  gardeners, Brannoc's miners and Marisol's ships each hold a part of the defence (a
  sequence of battles in town). At the darkest point the Silver Wing calls **Lugia** out
  of the sea. Lugia and the player drive Yveltal off.
- **Emberfort, Garrick, Fire.** The case is the buried bread-protest massacre. The player
  takes the fight to Garrick's fortress palace and wins the Trial in his throne hall. Garrick withdraws.
- **Duskmoor, Vespera, Dark.** Vespera hands the player to the Choir, then frees them in
  the same night: the double-agent reveal. She gives the player the Choir's plan for the
  Bell Tower. Her Trial follows: she insists.
- **Act end:** Lowmere reaches Stage 4. The ten towns' pacts now form an alliance in all
  but name.

## Act 4: The Crown and the Choir (Trial 9, Crownspire)

- **Highcrest, Aurelian, Dragon.** The case is the bribed magistrates. The final sibling
  Trial, fought in Highcrest's palace in front of the whole court. Aurelian loses and confesses that he let the king be kept
  asleep.
- **Storming Crownspire.** Marisol blockades the harbour, Teodor sabotages the engine
  from inside, Brannoc's miners open the undercroft. Seraphine is freed. The player
  climbs the Bell Tower through the Four Voices (the Privy Council, Elite Four slot).
- **The Hierophant.** Venn at the top of the tower. Final human battle; his team
  includes the two dogs the player didn't choose, taken from the royal kennels. As he loses, he
  rings the great bell anyway. The sky tears and **Giratina** comes through.
- **Rayquaza.** Drawn by the tear in the sky, **Rayquaza** descends on the tower and
  answers the player. Giratina battle.
- **The Sleeper.** With the bell silenced, the player goes down into the undercroft and
  faces **Darkrai**.
- **The king wakes.** Xerneas restores him.

## Ending

The waking king offers the player the crown in front of all ten heirs. **The player
chooses.**

- **Take the crown.** The player is crowned. The siblings kneel one by one, in Trial
  order; some gladly, some not. Final scene: the player's first court, with Lowmere's
  people in the hall where nobles used to stand. Bittersweet: the player has won the
  game the court was playing.
- **Refuse it and found the Charter.** The player proposes the **Ten-Town Charter**:
  each town governed by a council of its people, the heirs serving as stewards rather
  than rulers, and the crown as a symbol. The siblings sign one by one, in Trial order.
  Final scene: Lowmere, fully rebuilt, holding a festival. Corwin is running the stalls.

The choice is a yes/no at the end of the final cutscene. Both endings roll the same
credits; the epilogue scene differs. Features should record it in one flag (suggested
`FLAG_ENDING_TOOK_CROWN`) so post-game dialogue can refer to it.

## Post-game ideas

- Lowmere opens its own palace hall for Trials, led by Tamsin.
- Rematches as "Council sessions" with the siblings.
- Catching the Choir's legendaries.
- Hunting the remaining Acolytes and the last Warden.
