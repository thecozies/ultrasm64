#ifndef o
#include "game/level_update.h"
#include "game/object_helpers.h"
#include "game/object_list_processor.h"
#include "object_fields.h"

#define o gCurrentObject
#endif

void fire_light_obj_loop(void) {
    if (o->oDistanceToMario < 1000.0f) {
        if (gMarioState->lightObj != NULL && gMarioState->lightObj->oDistanceToMario < o->oDistanceToMario) {
            return;
        }
        gMarioState->lightObj = o;
    }
}

void bhv_light_obj_loop(void) {
    s32 lightType = (o->oBehParams >> 16) & 0xFF;
    switch (lightType) {
        case 0:
        default:
            fire_light_obj_loop();
    }
}
