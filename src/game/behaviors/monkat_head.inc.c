
#ifndef o
#include "game/mario.h"
#include "game/level_update.h"
#include "game/object_helpers.h"
#include "game/object_list_processor.h"
#include "object_fields.h"

#define o gCurrentObject
#endif

#define MONKAT_HEAD_END_POS -1800.0f

void bhv_monkat_head_phase_in(void) {
    if (gCurCutsceneTimer > CUTSCENE_INTRO_DOOR_HEAD_APPEARS) {
        cur_obj_init_animation(0);
        o->oOpacity = MIN(240, o->oOpacity + 4);
        o->oPosZ = MIN(MONKAT_HEAD_END_POS, o->oPosZ + 2.8f);
        if (gCurCutsceneTimer > CUTSCENE_INTRO_DOOR_FOLLOW_ORB + 40) {
            o->oTimer = 0;
            o->oOpacity = 240;
            o->oAction = 1;
        }
    }
}
void bhv_monkat_head_phase_out(void) {
    o->oOpacity = MAX(0, o->oOpacity - 5);
    o->oPosZ = MAX(o->oPosZ - 5.0f, o->oHomeZ);
    if (gCurCutsceneTimer >= CUTSCENE_INTRO_LUCY_WALKS_OVER) {
        obj_mark_for_deletion(o);
    }
}

void bhv_monkat_head_loop(void) {
    switch (o->oAction) {
        case 1:
            bhv_monkat_head_phase_out();
            break;
        default:
            bhv_monkat_head_phase_in();
    }
}

void bhv_monkat_head_init(void) {
    o->oOpacity = 0;
    o->oAction = 0;
}
