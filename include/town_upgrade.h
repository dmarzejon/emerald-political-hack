#ifndef GUARD_TOWN_UPGRADE_H
#define GUARD_TOWN_UPGRADE_H

#include "constants/town_upgrade.h"

const struct MapHeader *TownUpgrade_GetMapHeader(u16 mapGroup, u16 mapNum, const struct MapHeader *header);
u32 CountSignedPacts(void);
u32 GetLowmereTargetStage(void);
bool32 TownUpgrade_AdvanceLowmereStage(void);
void TownUpgrade_SignPact(u32 town, u32 terms);
u32 TownUpgrade_GetPactTerms(u32 town);

// Specials
void SignPact(void);
void GetPactTerms(void);
void GetSignedPactCount(void);
void UpdateLowmereStage(void);

#endif // GUARD_TOWN_UPGRADE_H
