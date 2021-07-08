#ifndef o
#include "game/level_update.h"
#include "game/object_helpers.h"
#include "game/object_list_processor.h"
#include "game/rendering_graph_node.h"
#include "game/puppycam2.h"
#include "game/mario.h"
#include "object_fields.h"

#define o gCurrentObject
#endif

#include "s2dex/s2d_buffer.h"

#define FIRST_ORB_END_Z -1026.0f

#define FADING_ORB_START_DIST 3450.0f
#define FADING_ORB_END_DIST 400.0f

void bhv_parasite_interact(void) {
    if (gCurrLevelNum == LEVEL_PSS) {
        if (o->oRoom != -1) {
            if (gMarioCurrentRoom == o->oRoom) o->oOpacity = MIN(255, o->oOpacity + 10);
            else o->oOpacity = 0;
        } else {
            f32 camDist = dist_between_object_and_camera(o);
            if (camDist >= FADING_ORB_START_DIST) o->oOpacity = 0;
            else if (camDist < FADING_ORB_END_DIST) o->oOpacity = 255;
            else {
                o->oOpacity = (int) MAX(0, MIN(255, get_relative_position_between_ranges(
                    camDist,
                    FADING_ORB_END_DIST,
                    FADING_ORB_START_DIST,
                    255.0f,
                    0.0f
                )));
            }
            
            if (o->oOpacity == 0) cur_obj_hide();
            else cur_obj_unhide();
        }
    }
    else if (gCurrLevelNum == LEVEL_CASTLE_COURTYARD) {
        struct Object *monkat_head;
        monkat_head = cur_obj_nearest_object_with_behavior(bhvMonkatHead);
        if (monkat_head != NULL) {
            if (gCurCutsceneTimer < CUTSCENE_INTRO_ORB_APPEAR) {
                o->header.gfx.scale[0] = 0.020f;
                o->header.gfx.scale[1] = 0.020f;
                o->header.gfx.scale[2] = 0.020f;
                o->oPosX = monkat_head->oPosX;
                o->oPosY = monkat_head->oPosY + 35.0f;
                o->oPosZ = monkat_head->oPosZ;
                o->oOpacity = 0;
            } else if (gCurCutsceneTimer == CUTSCENE_INTRO_ORB_APPEAR) {
                o->oOpacity = MIN(255, o->oOpacity + 10);
                o->header.gfx.scale[0] = 0.020f;
                o->header.gfx.scale[1] = 0.020f;
                o->header.gfx.scale[2] = 0.020f;
            } else if (gCurCutsceneTimer >= CUTSCENE_INTRO_ORB_APPEAR) {
                o->oOpacity = MIN(255, o->oOpacity + 10);
                o->oPosZ = MIN(FIRST_ORB_END_Z, o->oPosZ + 7.0f);
                if (gCurCutsceneTimer >= CUTSCENE_INTRO_ORB_APPEAR + 10) {
                    o->oPosY = MAX(o->oHomeY, o->oPosY - 0.3f);
                }
                o->header.gfx.scale[0] = MIN(1.0f, o->header.gfx.scale[0] + 0.02f);
                o->header.gfx.scale[1] = MIN(1.0f, o->header.gfx.scale[1] + 0.02f);
                o->header.gfx.scale[2] = MIN(1.0f, o->header.gfx.scale[2] + 0.02f);
                // o->oPosZ = approach_f32_asymptotic(o->oPosZ, FIRST_ORB_END_Z, 0.03f);
            }
        }
    } else {
        o->oOpacity = 255;
    }


    if (obj_check_if_collided_with_object(o, gMarioObject)) {
        set_collected_para((o->oBehParams >> 16) & 0xFF);

        play_sound(SOUND_ORB_COLLECT, gGlobalSoundSource);
        cur_obj_change_action(1);
    }
}

void bhv_parasite_grabbed(void) {
    s32 parasiteGroup = (o->oBehParams >> 16) & 0xFF;
    if (gParasitesGrabbed[parasiteGroup] == 0) {
        cur_obj_change_action(0);
        o->oOpacity = 255;
        cur_obj_unhide();
        o->header.gfx.scale[0] = 1.0f;
        o->header.gfx.scale[1] = 1.0f;
        o->header.gfx.scale[2] = 1.0f;
        o->oPosX = o->oHomeX;
        o->oPosY = o->oHomeY;
        o->oPosZ = o->oHomeZ;
    } else if (gParasitesGrabbed[parasiteGroup] == gParasitesGoals[parasiteGroup]) {
        o->activeFlags = ACTIVE_FLAG_DEACTIVATED;
    } else if (o->oOpacity != 0) {
        o->oOpacity = MAX(0, o->oOpacity - 10);
        o->header.gfx.scale[0] += 0.08f;
        o->header.gfx.scale[1] += 0.08f;
        o->header.gfx.scale[2] += 0.08f;
        o->oPosX = approach_f32_asymptotic(o->header.gfx.pos[0], gMarioState->pos[0], 0.3f);
        o->oPosY = approach_f32_asymptotic(o->header.gfx.pos[1], gMarioState->pos[1], 0.3f);
        o->oPosZ = approach_f32_asymptotic(o->header.gfx.pos[2], gMarioState->pos[2], 0.3f);
    }

    if (o->oOpacity <= 0) {
        cur_obj_hide();
    }
}

void bhv_parasite_loop(void) {
    switch (o->oAction) {
        case 1:
            bhv_parasite_grabbed();
            break;
        default:
            bhv_parasite_interact();
    }
}

void bhv_parasite_init(void) {
    s32 parasiteGroup = (o->oBehParams >> 16) & 0xFF;
    s32 challengeOrb = o->oBehParams & 0xFF;
    if ((gChallengeMode && !challengeOrb) || (!gChallengeMode && challengeOrb)) {
        obj_mark_for_deletion(o);
        o->oOpacity = 0;
        return;
    }
    if (gParasitesGoalsSet[parasiteGroup] == FALSE) gParasitesGoals[parasiteGroup]++;

    o->oMoveAnglePitch = 0;
    o->oGravity = 0;
    o->oFriction = 0.0f;
    o->oBuoyancy = 0.0f;
    if (parasiteGroup >= 1) o->oOpacity = 0;
    else o->oOpacity = 255;
}
