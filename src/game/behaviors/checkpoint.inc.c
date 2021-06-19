#ifndef o
#include "game/level_update.h"
#include "game/object_helpers.h"
#include "game/object_list_processor.h"
#include "game/rendering_graph_node.h"
#include "game/puppycam2.h"
#include "game/area.h"
#include "game/mario.h"
#include "object_fields.h"

#define o gCurrentObject
#endif

void bhv_checkpoint_loop(void) {
    s32 isFogPoint = (o->oBehParams >> 24) & 0xFF;
    if (isFogPoint && o->oDistanceToMario < 500.0f) {
        s32 fogSettings = (o->oBehParams >> 16) & 0xFF;
        set_current_fog_state(fogSettings);
    } else if (!isFogPoint) {
        f32 distMult = (o->oBehParams >> 16) & 0xFF;
        if (!distMult) distMult = 1.0f;
        if (o->oDistanceToMario < 500.0f * distMult) {
            set_object_checkpoint();
        }
    }
}
