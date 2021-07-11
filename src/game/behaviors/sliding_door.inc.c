#ifndef o
#include "game/level_update.h"
#include "game/object_helpers.h"
#include "game/object_list_processor.h"
#include "game/rendering_graph_node.h"
#include "game/puppycam2.h"
#include "game/area.h"
#include "game/mario.h"
#include "object_fields.h"
#include "sounds.h"
#include "game/spawn_sound.h"
#include "external.h"

#define o gCurrentObject
#endif

#define SLIDING_DOOR_CUTSCENE_END_HEIGHT 550.0f
#define SLIDING_DOOR_GOAL_HEIGHT 820.0f
#define SLIDING_DOOR_VELY_1 25.0f
#define SLIDING_DOOR_VELY_2 ((gChallengeMode || gSpeedrunMode) ? 20.0f : 16.0f)
#define SLIDING_DOOR_TIME_START  20
#define SLIDING_DOOR_JOLT_END    26
#define SLIDING_DOOR_KICKBACK_END 27
#define SLIDING_DOOR_SLIDE_START 32

#define FINAL_SLIDING_DOOR_GROUP 9

s32 is_sliding_door_face_up(void) {
    return o->oFaceAnglePitch == 0 && o->oFaceAngleRoll == 0;
}

void sliding_door_act_4(void) {
    s32 faceUp = is_sliding_door_face_up();
    f32 hiddenHeight = faceUp ? o->oHomeY + SLIDING_DOOR_GOAL_HEIGHT : o->oHomeY - SLIDING_DOOR_GOAL_HEIGHT;
    if (o->oFloorRoom != gMarioCurrentRoom)
    {
        o->oPosY = approach_f32_symmetric(o->oPosY, o->oHomeY, SLIDING_DOOR_VELY_1);
    }
    else if (o->oPosY != hiddenHeight)
    {
        o->oPosY = approach_f32_symmetric(o->oPosY, hiddenHeight, SLIDING_DOOR_VELY_1);
    }
    return;
}

void sliding_door_act_3(void) {
    if ((o->oBehParams >> 24) & 0xFF && gCurCutscene == 7) {
        if (gCurCutsceneTimer >= 550) {
            if (o->oPosY > o->oHomeY) {
                o->oVelY = MAX(o->oVelY - 0.6f, -40.0f);
                o->oPosY = MAX(o->oPosY + o->oVelY, o->oHomeY);
                if (o->oPosY == o->oHomeY) {
                    spawn_mist_particles_variable(0, 0, 45.0f);
                }
            }
        }
    }
    return;
}

void sliding_door_act_2(void) {
    s32 otherRoomClosesDoor = ((o->oBehParams >> 24) & 0xFF == 0xFF);
    o->oForwardVel = 0.0f;
    o->oVelY = 0.0f;
    o->oAction = (otherRoomClosesDoor) ? 4 : 3;
}

void sliding_door_act_1(void) {
    f32 direction = is_sliding_door_face_up() ? 1.0f : -1.0f;
    s32 parasiteGroup = (o->oBehParams >> 16) & 0xFF;

    if (o->oTimer == 0) {
        gParasitesGoalsSet[parasiteGroup] = TRUE;
        if (!gSpeedrunMode && !gChallengeMode) {
            gPuppyCam.targetObj2 = o;
            gPrevPuppyZoomDist = gPuppyCam.zoomTarget;
            gPrevPuppyTargetYaw = gPuppyCam.yawTarget;
            gPuppyCam.zoomTarget = o->oDistanceToMario + 1000;
            enable_time_stop_including_mario();
            set_current_cutscene(CUTSCENE_DOOR_OPEN);
        } else {
            set_tip_type(2);
        }
    }

    o->oVelY = 0.0f;
    if (parasiteGroup == FINAL_SLIDING_DOOR_GROUP && o->oTimer == SLIDING_DOOR_SLIDE_START + 30) set_current_fog_state(END_FINAL_FOG);
    if (o->oTimer > SLIDING_DOOR_SLIDE_START)
    {
        o->oPosY += (direction * SLIDING_DOOR_VELY_2);
        // play sliding sound
    }
    if (o->oTimer > SLIDING_DOOR_KICKBACK_END)
    {
        // nothing
    }
    if (o->oTimer > SLIDING_DOOR_JOLT_END)
    {
        o->oPosY -= (direction * 10.0f);
    }
    else if (o->oTimer > SLIDING_DOOR_TIME_START)
    {
        o->oPosY += (direction * SLIDING_DOOR_VELY_1);
        spawn_mist_particles_variable(0, 0, 45.0f);
        if (o->oTimer == SLIDING_DOOR_TIME_START + 1) play_sound(SOUND_DOOR_OPENING, gGlobalSoundSource);
    }

    if (ABS(o->oHomeY - o->oPosY) >= SLIDING_DOOR_GOAL_HEIGHT)
    {
        o->oAction = 2;
    }
    else if (ABS(o->oHomeY - o->oPosY) >= SLIDING_DOOR_CUTSCENE_END_HEIGHT)
    {
        if (gPuppyCam.targetObj2 != NULL) {
            gPuppyCam.targetObj2 = NULL;
            gObjCutsceneDone = TRUE;
            gPuppyCam.zoomTarget = gPrevPuppyZoomDist;
            gPuppyCam.yawTarget = gPrevPuppyTargetYaw;
            disable_time_stop_including_mario();
            if (parasiteGroup == FINAL_SLIDING_DOOR_GROUP)
            {
                set_current_cutscene(CUTSCENE_ORB_REVEAL);
            }
            else
            {
                set_current_cutscene(CUTSCENE_NONE);
            }
        }
    }
}

void sliding_door_act_0(void) {
    s32 parasiteGroup = (o->oBehParams >> 16) & 0xFF;

    // Have to check if timer > 0 because this act might get processed before any parasites
    if (gParasitesGoalsSet[parasiteGroup] == FALSE && o->oTimer++ > 0) gParasitesGoalsSet[parasiteGroup] = TRUE;

    o->oVelY = 0.0f;
    if (
        gParasitesGrabbed[parasiteGroup] > 0 &&
        gParasitesGoals[parasiteGroup] == gParasitesGrabbed[parasiteGroup]
    ) {
        o->oAction = 1;
        gMarioState->lastParaGroup = -1;
    }
}

void (*sSlidingDoorActions[])(void) = {
    sliding_door_act_0,
    sliding_door_act_1,
    sliding_door_act_2,
    sliding_door_act_3,
    sliding_door_act_4
};

void bhv_sliding_door_loop(void) {
    cur_obj_call_action_function(sSlidingDoorActions);
}

void bhv_sliding_door_init(void) {
    s32 parasiteGroup = (o->oBehParams >> 16) & 0xFF;
    s32 cutsceneIndex = (o->oBehParams >> 24) & 0xFF;

    if (cutsceneIndex && cutsceneIndex < 0xFF && cutsceneIndex != 0x10) {
        o->oPosY += SLIDING_DOOR_GOAL_HEIGHT;
        o->oAction = 3;
        return;
    }
    cur_obj_update_floor();

    if (
        !cutsceneIndex &&
        gParasitesGoalsSet[parasiteGroup] &&
        gParasitesGrabbed[parasiteGroup] > 0 &&
        gParasitesGoals[parasiteGroup] == gParasitesGrabbed[parasiteGroup]
    ) {
        if (is_sliding_door_face_up()) o->oPosY += SLIDING_DOOR_GOAL_HEIGHT;
        else o->oPosY -= SLIDING_DOOR_GOAL_HEIGHT;
        o->oAction = 3;
    }
    o->oTimer = 0;
    return;
}
