#ifndef o

#include "game/mario.h"
#include "game/level_update.h"
#include "game/area.h"
#include "game/object_list_processor.h"
#include "object_fields.h"
#include "game/puppycam2.h"

#define o gCurrentObject
#endif

void bhv_cutscene_obj_towerclimb(void) {
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
        set_current_cutscene(CUTSCENE_NONE);
        gPuppyCam.yawTarget = gPuppyCam.yaw;
        gPuppyCam.targetObj2 = NULL;
        gPuppyCam.flags |= PUPPYCAM_BEHAVIOUR_COLLISION;
        gPuppyCam.flags &= ~PUPPYCAM_BEHAVIOUR_INPUT_8DIR;
    }
}

void bhv_cutscene_obj_force_yaw(void) {
    o->oLateralDistToMario = ABS(lateral_dist_between_objects(o, gMarioObject));

    if (o->oLateralDistToMario < 1700.0f && ABS(gMarioState->pos[1] - o->oPosY) < 1000.0f) {
        if (gCurCutscene != CUTSCENE_FORCE_YAW) {
            set_current_cutscene(CUTSCENE_FORCE_YAW);
        }

        gPuppyCam.yawTarget = o->oFaceAngleYaw + 0x8000;
        gPuppyCam.flags &= ~PUPPYCAM_BEHAVIOUR_YAW_ROTATION;

    } else if (gCurCutscene == CUTSCENE_FORCE_YAW) {
        set_current_cutscene(CUTSCENE_NONE);
        gPuppyCam.flags |= PUPPYCAM_BEHAVIOUR_YAW_ROTATION;
    }
}

void bhv_cutscene_ring_reminder(void) {
    o->oLateralDistToMario = ABS(lateral_dist_between_objects(o, gMarioObject));

    if (gCurCutscene != CUTSCENE_RING_REMINDER && o->oLateralDistToMario < 1000.0f) set_current_cutscene(CUTSCENE_RING_REMINDER);
}

void bhv_cutscene_obj_loop(void) {
    s32 cutscene = (o->oBehParams >> 16) & 0xFF;
    // if (current_mario_room_check(o->oRoom)) {
    switch (cutscene) {
        case CUTSCENE_TOWERCLIMB:
            bhv_cutscene_obj_towerclimb();
            break;
        case CUTSCENE_FORCE_YAW:
            bhv_cutscene_obj_force_yaw();
            break;
        case CUTSCENE_RING_REMINDER:
            bhv_cutscene_ring_reminder();
            break;
    }
    // }
}