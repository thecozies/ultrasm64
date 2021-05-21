#ifndef o
#include "game/level_update.h"
#include "game/object_helpers.h"
#include "game/object_list_processor.h"
#include "game/rendering_graph_node.h"
#include "game/puppycam2.h"
#include "object_fields.h"

#define o gCurrentObject
#endif

#define BIG_ORB_RADIUS 850.0f

// #include "actors/group8.h"

void bhv_big_orb_interact(void) {
    f32 distFromCam = dist_between_object_and_camera(o);
    f32 lateralDistFromMario = lateral_dist_between_objects(o, gMarioObject);
    gOverrideLOD = distFromCam < BIG_ORB_RADIUS;

    if (gOverrideLOD) {
        o->oPosX = gPuppyCam.pos[0];
        o->oPosY = gPuppyCam.pos[1];
        o->oPosZ = gPuppyCam.pos[2];
    } else {
        o->oPosX = o->oHomeX;
        o->oPosY = lateralDistFromMario < BIG_ORB_RADIUS ? gMarioState->pos[1] : o->oHomeY;
        o->oPosZ = o->oHomeZ;
    }

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
    else if (lateralDistFromMario < BIG_ORB_RADIUS) o->oOpacity = 255;
    else {
        o->oOpacity = (int) MAX(0, MIN(255, get_relative_position_between_ranges(
            lateralDistFromMario,
            BIG_ORB_RADIUS,
            5000.0f,
            255,
            0
        )));
    }
}

void bhv_big_orb_loop(void) {
    // s32 isBeam = (o->oBehParams >> 16) > 0;

    // if (!isBeam) {
    switch (o->oAction) {
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
