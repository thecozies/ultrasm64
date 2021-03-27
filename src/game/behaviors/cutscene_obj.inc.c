#ifndef o

#include "game/mario.h"
#include "game/level_update.h"
#include "game/area.h"
#include "game/object_list_processor.h"
#include "object_fields.h"
#include "game/puppycam2.h"

#define o gCurrentObject
#endif

void bhv_cutscene_obj_loop(void) {
    o->oLateralDistToMario = ABS(lateral_dist_between_objects(o, gMarioObject));
    o->oPosY = gMarioState->pos[1];
    o->oYDistToHome = o->oPosY - o->oHomeY;

    if (o->oLateralDistToMario < 3000.0f && o->oYDistToHome > 0.0f) {
        if (gCurCutscene != CUTSCENE_TOWERCLIMB) {
            set_current_cutscene(CUTSCENE_TOWERCLIMB);
            gPuppyCam.yawTarget = 0;
        }
        gPuppyCam.targetObj2 = o;
        gPuppyCam.flags &= ~PUPPYCAM_BEHAVIOUR_COLLISION;
        gPuppyCam.flags |= PUPPYCAM_BEHAVIOUR_INPUT_8DIR;

    } else if (gCurCutscene == CUTSCENE_TOWERCLIMB) {
        set_current_cutscene(NO_CUTSCENE);
        gPuppyCam.yawTarget = gPuppyCam.yaw;
        gPuppyCam.targetObj2 = NULL;
        gPuppyCam.flags |= PUPPYCAM_BEHAVIOUR_COLLISION;
        gPuppyCam.flags &= ~PUPPYCAM_BEHAVIOUR_INPUT_8DIR;
    }
}
