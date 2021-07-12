#ifndef o
#include "game/level_update.h"
#include "game/object_helpers.h"
#include "game/object_list_processor.h"
#include "game/rendering_graph_node.h"
#include "game/puppycam2.h"
#include "game/mario.h"
#include "object_fields.h"
#include "sm64.h"
#include "engine/math_util.h"

#define o gCurrentObject
#endif

#define BIG_ORB_RADIUS 850.0f

// #include "actors/group8.h"

void set_orb_locked_to_cam(void) {
    // CTODO: Set flag and use GEO_ASM to move pos
    // f32 x = 50.0f * sins(gPuppyCam.yaw);
    // f32 z = 50.0f * coss(gPuppyCam.yaw);

    // o->oPosX = gPuppyCam.pos[0] + x;
    // o->oPosY = gPuppyCam.pos[1];
    // o->oPosZ = gPuppyCam.pos[2] + z;
    o->oPosX = gPuppyCam.pos[0];
    o->oPosY = gPuppyCam.pos[1];
    o->oPosZ = gPuppyCam.pos[2];
    o->header.gfx.scale[0] = -1.0f;
}

void set_orb_to_home(void) {
    o->header.gfx.scale[0] = 1.0f;
    o->oPosX = o->oHomeX;
    o->oPosY = o->oHomeY;
    o->oPosZ = o->oHomeZ;
}

void bhv_big_orb_interact(void) {
    f32 distFromCam = dist_between_object_and_camera(o);
    f32 lateralDistFromMario = lateral_dist_between_object_home_and_object(o, gMarioObject);
    gOverrideLOD = distFromCam < BIG_ORB_RADIUS;

    if (gOverrideLOD) set_orb_locked_to_cam();
    else set_orb_to_home();

    // if (lateralDistFromMario <= 1500.0f) {
    //     o->header.gfx.scale[1] = approach_f32_asymptotic(
    //         o->header.gfx.scale[1],
    //         20.0f,
    //         0.02f
    //     );
    // } else {
    //     o->header.gfx.scale[1] = approach_f32_asymptotic(
    //         o->header.gfx.scale[1],
    //         1.0f,
    //         0.02f
    //     );
    // }

    if (lateralDistFromMario >= 5000) o->oOpacity = 0;
    else if (lateralDistFromMario < BIG_ORB_RADIUS) {
        o->oOpacity = 200;
        set_current_cutscene(CUTSCENE_LUCYS_LEVITATION);
        vec3f_copy(gFinalOrbPos, &o->oPosX);
    }
    else {
        o->oOpacity = (int) MAX(0, MIN(255, get_relative_position_between_ranges(
            lateralDistFromMario,
            BIG_ORB_RADIUS,
            5000.0f,
            200,
            0
        )));
    }
}

void bhv_big_orb_reveal(void) {
    s8 insideOrb = gCurCutsceneTimer >= ORB_REVEAL_INSIDE_ORB + 1;
    u8 increaseAmt = insideOrb ? 140 : 180;

    f32 maxMag = ABS((sinf(((f32)gCurCutsceneTimer) * 0.1f) * 25.0f) + 5.0f);

    o->oOpacity = (sinf(((f32)gCurCutsceneTimer) * 0.25f) * maxMag) + increaseAmt;

    if (insideOrb) set_orb_locked_to_cam();
    else set_orb_to_home();
}

void bhv_big_orb_levitation(void) {
    s32 isBeam = (o->oBehParams >> 16) > 0;
    s32 lockOrbToCam;
    f32 distFromCam;
    s32 creditsInOrb = CUTSCENE_RANGE(LUCYS_LEVITATION_CREDITS_IN_ORB, LUCYS_LEVITATION_CAM_EXIT_ORB);
    vec3f_copy(&o->oPosX, gFinalOrbPos);

    distFromCam = dist_between_object_and_camera(o);
    lockOrbToCam =
        (distFromCam < BIG_ORB_RADIUS + 100) ||
        (creditsInOrb);

    u8 increaseAmt = lockOrbToCam ? 140 : 180;
    f32 magRange = 25.0f;
    if (creditsInOrb) {
        increaseAmt = 220;
        magRange = 12.0f;
    }
    f32 maxMag = ABS((sinf(((f32)gCurCutsceneTimer) * 0.1f) * magRange) + 5.0f);
    o->oOpacity = MIN((sinf(((f32)gCurCutsceneTimer) * 0.25f) * maxMag) + increaseAmt, 255);

    if (lockOrbToCam) set_orb_locked_to_cam();
    gOverrideLOD = lockOrbToCam || (gCurCutsceneTimer >= LUCYS_LEVITATION_HIDE_LEVEL);

    if (creditsInOrb) o->oFaceAngleYaw -= DEGREES(2);
}

void bhv_big_orb_loop(void) {
    // s32 isBeam = (o->oBehParams >> 16) > 0;

    // if (!isBeam) {
    switch (gCurCutscene) {
        case CUTSCENE_LUCYS_LEVITATION:
            bhv_big_orb_levitation();
            break;
        case CUTSCENE_ORB_REVEAL:
            bhv_big_orb_reveal();
            break;
        default:
            bhv_big_orb_interact();
    }
    // } else {
    //     f32 lateralDistFromMario = lateral_dist_between_objects(o, gMarioObject);
    //     o->oOpacity = 155;
    //     o->oPosY = lateralDistFromMario < BIG_ORB_RADIUS ? gMarioState->pos[1] : o->oHomeY;
    // }
}

void bhv_big_orb_init(void) {
    // s32 p = o->oBehParams >> 16;
    // if (p > 0) {
    //     o->header.gfx.scale[0] = 4.0;
    //     o->header.gfx.scale[1] = 4.0;
    //     o->header.gfx.scale[2] = 4.0;
    // }
}
