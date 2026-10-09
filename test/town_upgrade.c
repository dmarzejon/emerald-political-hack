#include "global.h"
#include "event_data.h"
#include "overworld.h"
#include "string_util.h"
#include "town_upgrade.h"
#include "constants/layouts.h"
#include "constants/maps.h"
#include "test/overworld_script.h"
#include "test/test.h"

TEST("Lowmere starts at stage 0 with no pacts")
{
    EXPECT_EQ(CountSignedPacts(), 0);
    EXPECT_EQ(VarGet(VAR_LOWMERE_STAGE), LOWMERE_STAGE_0);
    RUN_OVERWORLD_SCRIPT( special UpdateLowmereStage; );
    EXPECT_EQ(gSpecialVar_Result, FALSE);
    EXPECT_EQ(VarGet(VAR_LOWMERE_STAGE), LOWMERE_STAGE_0);
}

TEST("Signing the Haymarket pact sets its flag and raises Lowmere to stage 1")
{
    RUN_OVERWORLD_SCRIPT( signpact PACT_HAYMARKET, PACT_TERMS_REFORM; );
    EXPECT(FlagGet(FLAG_PACT_HAYMARKET));
    EXPECT_EQ(gSpecialVar_Result, LOWMERE_STAGE_1);
    EXPECT_EQ(VarGet(VAR_LOWMERE_STAGE), LOWMERE_STAGE_1);
}

TEST("Pact terms are stored per town and read back")
{
    RUN_OVERWORLD_SCRIPT(
        signpact PACT_HAYMARKET, PACT_TERMS_CLEMENCY;
        signpact PACT_DUSKMOOR, PACT_TERMS_REFORM;
        signpact PACT_HIGHCREST, PACT_TERMS_RESTITUTION;
    );
    EXPECT_EQ(TownUpgrade_GetPactTerms(PACT_HAYMARKET), PACT_TERMS_CLEMENCY);
    EXPECT_EQ(TownUpgrade_GetPactTerms(PACT_DUSKMOOR), PACT_TERMS_REFORM);
    EXPECT_EQ(TownUpgrade_GetPactTerms(PACT_HIGHCREST), PACT_TERMS_RESTITUTION);
    EXPECT_EQ(TownUpgrade_GetPactTerms(PACT_THORNFIELD), PACT_TERMS_NONE);
    RUN_OVERWORLD_SCRIPT( getpactterms PACT_HAYMARKET; );
    EXPECT_EQ(gSpecialVar_Result, PACT_TERMS_CLEMENCY);
    RUN_OVERWORLD_SCRIPT( special GetSignedPactCount; );
    EXPECT_EQ(gSpecialVar_Result, 3);
}

TEST("Pact terms can come from a var such as a menu result")
{
    RUN_OVERWORLD_SCRIPT(
        setvar VAR_RESULT, PACT_TERMS_REFORM;
        signpact PACT_CRAGHOLT, VAR_RESULT;
    );
    EXPECT_EQ(TownUpgrade_GetPactTerms(PACT_CRAGHOLT), PACT_TERMS_REFORM);
}

TEST("Lowmere stages follow the pact count, and stage 4 needs the siege")
{
    u32 i;
    static const u8 expected[PACT_TOWN_COUNT] = {1, 1, 2, 2, 2, 3, 3, 3, 3};

    for (i = 0; i < PACT_TOWN_COUNT; i++)
    {
        TownUpgrade_SignPact(i, PACT_TERMS_RESTITUTION);
        EXPECT_EQ(VarGet(VAR_LOWMERE_STAGE), expected[i]);
    }
    FlagSet(FLAG_LOWMERE_SIEGE_SURVIVED);
    RUN_OVERWORLD_SCRIPT( special UpdateLowmereStage; );
    EXPECT_EQ(gSpecialVar_Result, TRUE);
    EXPECT_EQ(VarGet(VAR_LOWMERE_STAGE), LOWMERE_STAGE_4);
}

TEST("Updating Lowmere's stage never lowers a stage set by script")
{
    VarSet(VAR_LOWMERE_STAGE, LOWMERE_STAGE_2);
    TownUpgrade_SignPact(PACT_HAYMARKET, PACT_TERMS_REFORM);
    EXPECT_EQ(VarGet(VAR_LOWMERE_STAGE), LOWMERE_STAGE_2);
}

TEST("Invalid pact towns and terms are ignored")
{
    TownUpgrade_SignPact(PACT_TOWN_COUNT, PACT_TERMS_REFORM);
    TownUpgrade_SignPact(PACT_HAYMARKET, PACT_TERMS_COUNT);
    EXPECT_EQ(CountSignedPacts(), 0);
}

TEST("The new Lowmere stage macro jumps until the stage is marked seen")
{
    VarSet(VAR_LOWMERE_STAGE, LOWMERE_STAGE_1);
    RUN_OVERWORLD_SCRIPT(
        setvar VAR_TEMP_0, 0;
        goto_if_lowmere_stage_new IsNew;
        end;
    IsNew:
        setvar VAR_TEMP_0, 1;
        lowmere_mark_stage_seen;
        end;
    );
    EXPECT_EQ(VarGet(VAR_TEMP_0), 1);
    EXPECT_EQ(VarGet(VAR_LOWMERE_STAGE_SEEN), LOWMERE_STAGE_1);
    RUN_OVERWORLD_SCRIPT(
        setvar VAR_TEMP_0, 0;
        goto_if_lowmere_stage_new StillNew;
        end;
    StillNew:
        setvar VAR_TEMP_0, 1;
        end;
    );
    EXPECT_EQ(VarGet(VAR_TEMP_0), 0);
}

TEST("Lowmere's map keeps its own layout for stages without one")
{
    const struct MapHeader *header;
    u32 stage;

    for (stage = 0; stage < LOWMERE_STAGE_COUNT; stage++)
    {
        VarSet(VAR_LOWMERE_STAGE, stage);
        header = Overworld_GetMapHeaderByGroupAndId(MAP_GROUP(MAP_LITTLEROOT_TOWN), MAP_NUM(MAP_LITTLEROOT_TOWN));
#ifndef MAP_LOWMERE
        EXPECT(header->mapLayoutId != 0);
        EXPECT(header->mapLayout == GetMapLayout(header->mapLayoutId));
#endif
    }
    header = Overworld_GetMapHeaderByGroupAndId(MAP_GROUP(MAP_OLDALE_TOWN), MAP_NUM(MAP_OLDALE_TOWN));
    EXPECT_EQ(header->mapLayoutId, LAYOUT_OLDALE_TOWN);
}

TEST("Title placeholders follow the player's gender")
{
    static const u8 text[] = _("{TITLE_CAP} {TITLE}, {SIBLING}, {CHILD}");
    u8 buffer[64];

    gSaveBlock2Ptr->playerGender = MALE;
    StringExpandPlaceholders(buffer, text);
    EXPECT_EQ(StringCompare(buffer, COMPOUND_STRING("Prince prince, brother, son")), 0);

    gSaveBlock2Ptr->playerGender = FEMALE;
    StringExpandPlaceholders(buffer, text);
    EXPECT_EQ(StringCompare(buffer, COMPOUND_STRING("Princess princess, sister, daughter")), 0);
}
