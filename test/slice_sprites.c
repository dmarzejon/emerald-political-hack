#include "global.h"
#include "event_object_movement.h"
#include "sprite.h"
#include "constants/event_objects.h"
#include "test/test.h"

TEST("Tamsin's overworld sprite has its own palette, not the player's")
{
    const struct ObjectEventGraphicsInfo *tamsin = GetObjectEventGraphicsInfo(OBJ_EVENT_GFX_TAMSIN);
    const struct ObjectEventGraphicsInfo *may = GetObjectEventGraphicsInfo(OBJ_EVENT_GFX_RIVAL_MAY_NORMAL);
    u8 tamsinSlot, maySlot;

    EXPECT_EQ(tamsin->images, may->images);
    EXPECT_EQ(tamsin->paletteTag, OBJ_EVENT_PAL_TAG_TAMSIN);

    FreeAllSpritePalettes();
    tamsinSlot = LoadObjectEventPalette(OBJ_EVENT_PAL_TAG_TAMSIN);
    maySlot = LoadObjectEventPalette(OBJ_EVENT_PAL_TAG_MAY);
    EXPECT_NE(tamsinSlot, 0xFF);
    EXPECT_NE(maySlot, 0xFF);
    EXPECT_NE(tamsinSlot, maySlot);
    FreeAllSpritePalettes();
}
