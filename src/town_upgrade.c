#include "global.h"
#include "event_data.h"
#include "overworld.h"
#include "town_upgrade.h"
#include "constants/layouts.h"
#include "constants/maps.h"

// Town upgrades and post-Trial pacts. See docs/systems.md.
//
// Each upgradeable town lists one layout per stage. When the town's map is
// loaded (by warp, by walking in, or as a connection seen from a neighbouring
// map), the layout for the current stage replaces the map's own layout. A
// stage with no layout of its own keeps the map's layout from map.json.
//
// The map thread adds layouts named LAYOUT_LOWMERE_STAGE<n> to
// data/layouts/layouts.json; each one is picked up here automatically.
// All stage layouts of a town must have the same width and height as the
// map's own layout, because warps, events and connections are shared.

#ifdef MAP_LOWMERE
#define MAP_LOWMERE_TOWN MAP_LOWMERE
#else
#define MAP_LOWMERE_TOWN MAP_LITTLEROOT_TOWN
#endif

#ifndef LAYOUT_LOWMERE_STAGE0
#define LAYOUT_LOWMERE_STAGE0 0
#endif
#ifndef LAYOUT_LOWMERE_STAGE1
#define LAYOUT_LOWMERE_STAGE1 0
#endif
#ifndef LAYOUT_LOWMERE_STAGE2
#define LAYOUT_LOWMERE_STAGE2 0
#endif
#ifndef LAYOUT_LOWMERE_STAGE3
#define LAYOUT_LOWMERE_STAGE3 0
#endif
#ifndef LAYOUT_LOWMERE_STAGE4
#define LAYOUT_LOWMERE_STAGE4 0
#endif

struct UpgradeableTown
{
    u16 map;
    u16 stageVar;
    u16 layouts[LOWMERE_STAGE_COUNT]; // 0 = use the map's own layout
};

static const struct UpgradeableTown sUpgradeableTowns[] =
{
    {
        .map = MAP_LOWMERE_TOWN,
        .stageVar = VAR_LOWMERE_STAGE,
        .layouts =
        {
            LAYOUT_LOWMERE_STAGE0,
            LAYOUT_LOWMERE_STAGE1,
            LAYOUT_LOWMERE_STAGE2,
            LAYOUT_LOWMERE_STAGE3,
            LAYOUT_LOWMERE_STAGE4,
        },
    },
};

static const u16 sPactFlags[PACT_TOWN_COUNT] =
{
    [PACT_HAYMARKET]  = FLAG_PACT_HAYMARKET,
    [PACT_THORNFIELD] = FLAG_PACT_THORNFIELD,
    [PACT_CRAGHOLT]   = FLAG_PACT_CRAGHOLT,
    [PACT_SALTMERE]   = FLAG_PACT_SALTMERE,
    [PACT_VOLTAINE]   = FLAG_PACT_VOLTAINE,
    [PACT_LUMENHALL]  = FLAG_PACT_LUMENHALL,
    [PACT_EMBERFORT]  = FLAG_PACT_EMBERFORT,
    [PACT_DUSKMOOR]   = FLAG_PACT_DUSKMOOR,
    [PACT_HIGHCREST]  = FLAG_PACT_HIGHCREST,
};

// Pact terms take 2 bits per town: towns 0-7 in VAR_PACT_TERMS_1, town 8 in VAR_PACT_TERMS_2.
#define PACT_TERMS_BITS     2
#define PACT_TERMS_MASK     ((1 << PACT_TERMS_BITS) - 1)
#define PACT_TOWNS_PER_VAR  (16 / PACT_TERMS_BITS)

STATIC_ASSERT(PACT_TERMS_COUNT <= (1 << PACT_TERMS_BITS), PactTermsFitInTwoBits)
STATIC_ASSERT(PACT_TOWN_COUNT <= PACT_TOWNS_PER_VAR * 2, PactTermsFitInTwoVars)

// One RAM copy of each town's map header, so the stage layout can be swapped in
// without touching the ROM header.
static EWRAM_DATA struct MapHeader sUpgradedHeaders[ARRAY_COUNT(sUpgradeableTowns)] = {0};

const struct MapHeader *TownUpgrade_GetMapHeader(u16 mapGroup, u16 mapNum, const struct MapHeader *header)
{
    u32 i;
    u16 map = (mapGroup << 8) | mapNum;

    for (i = 0; i < ARRAY_COUNT(sUpgradeableTowns); i++)
    {
        const struct UpgradeableTown *town = &sUpgradeableTowns[i];
        u32 stage;
        u16 layoutId;

        if (town->map != map)
            continue;

        stage = VarGet(town->stageVar);
        if (stage >= LOWMERE_STAGE_COUNT)
            stage = LOWMERE_STAGE_COUNT - 1;
        layoutId = town->layouts[stage];
        if (layoutId == 0 || layoutId == header->mapLayoutId)
            return header;

        sUpgradedHeaders[i] = *header;
        sUpgradedHeaders[i].mapLayoutId = layoutId;
        sUpgradedHeaders[i].mapLayout = GetMapLayout(layoutId);
        return &sUpgradedHeaders[i];
    }
    return header;
}

u32 CountSignedPacts(void)
{
    u32 i, count = 0;

    for (i = 0; i < PACT_TOWN_COUNT; i++)
    {
        if (FlagGet(sPactFlags[i]))
            count++;
    }
    return count;
}

u32 GetLowmereTargetStage(void)
{
    u32 pacts = CountSignedPacts();

    if (pacts >= LOWMERE_STAGE_4_PACTS && FlagGet(FLAG_LOWMERE_SIEGE_SURVIVED))
        return LOWMERE_STAGE_4;
    if (pacts >= LOWMERE_STAGE_3_PACTS)
        return LOWMERE_STAGE_3;
    if (pacts >= LOWMERE_STAGE_2_PACTS)
        return LOWMERE_STAGE_2;
    if (pacts >= LOWMERE_STAGE_1_PACTS)
        return LOWMERE_STAGE_1;
    return LOWMERE_STAGE_0;
}

// Raises VAR_LOWMERE_STAGE to match the pacts signed. Never lowers it, so a
// script may also set the stage directly for a story beat.
bool32 TownUpgrade_AdvanceLowmereStage(void)
{
    u32 target = GetLowmereTargetStage();

    if (target <= VarGet(VAR_LOWMERE_STAGE))
        return FALSE;
    VarSet(VAR_LOWMERE_STAGE, target);
    return TRUE;
}

static u16 GetPactTermsVar(u32 town)
{
    return town < PACT_TOWNS_PER_VAR ? VAR_PACT_TERMS_1 : VAR_PACT_TERMS_2;
}

static u32 GetPactTermsShift(u32 town)
{
    return (town % PACT_TOWNS_PER_VAR) * PACT_TERMS_BITS;
}

void TownUpgrade_SignPact(u32 town, u32 terms)
{
    u16 var, value;
    u32 shift;

    if (town >= PACT_TOWN_COUNT || terms >= PACT_TERMS_COUNT)
        return;

    var = GetPactTermsVar(town);
    shift = GetPactTermsShift(town);
    value = VarGet(var) & ~(PACT_TERMS_MASK << shift);
    VarSet(var, value | (terms << shift));
    FlagSet(sPactFlags[town]);
    TownUpgrade_AdvanceLowmereStage();
}

u32 TownUpgrade_GetPactTerms(u32 town)
{
    if (town >= PACT_TOWN_COUNT || !FlagGet(sPactFlags[town]))
        return PACT_TERMS_NONE;
    return (VarGet(GetPactTermsVar(town)) >> GetPactTermsShift(town)) & PACT_TERMS_MASK;
}

// VAR_0x8004 = PACT_* town, VAR_0x8005 = PACT_TERMS_*.
// Sets the town's pact flag, records the terms and raises Lowmere's stage.
// VAR_RESULT = Lowmere's stage afterwards.
void SignPact(void)
{
    TownUpgrade_SignPact(gSpecialVar_0x8004, gSpecialVar_0x8005);
    gSpecialVar_Result = VarGet(VAR_LOWMERE_STAGE);
}

// VAR_0x8004 = PACT_* town. VAR_RESULT = PACT_TERMS_*, or PACT_TERMS_NONE if not signed.
void GetPactTerms(void)
{
    gSpecialVar_Result = TownUpgrade_GetPactTerms(gSpecialVar_0x8004);
}

// VAR_RESULT = number of pacts signed.
void GetSignedPactCount(void)
{
    gSpecialVar_Result = CountSignedPacts();
}

// Raises VAR_LOWMERE_STAGE to match the pacts signed (and the siege flag).
// VAR_RESULT = TRUE if the stage went up.
void UpdateLowmereStage(void)
{
    gSpecialVar_Result = TownUpgrade_AdvanceLowmereStage();
}
