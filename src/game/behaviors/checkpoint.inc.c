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
    if (o->oDistanceToMario < 500.0f) {
        s32 fogPoint = (o->oBehParams >> 24) & 0xFF;
        s32 fogSettings = (o->oBehParams >> 16) & 0xFF;

        if (fogPoint) set_current_fog_state(fogSettings);
        else {
            gMarioState->checkpointPos[0] = o->oPosX;
            gMarioState->checkpointPos[1] = o->oPosY;
            gMarioState->checkpointPos[2] = o->oPosZ;
            gMarioState->checkpointAngle[0] = o->oFaceAnglePitch;
            gMarioState->checkpointAngle[1] = o->oFaceAngleYaw;
            gMarioState->checkpointAngle[2] = o->oFaceAngleRoll;
        }
    }
}
