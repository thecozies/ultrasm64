
void bhv_parasite_interact(void) {
    if (obj_check_if_collided_with_object(o, gMarioObject)) {
        set_collected_para((o->oBehParams >> 16) & 0xFF);

        play_sound(SOUND_MENU_STAR_SOUND, gGlobalSoundSource);
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
    if (gParasitesGoalsSet[parasiteGroup] == FALSE) gParasitesGoals[parasiteGroup]++;

    o->oMoveAnglePitch = 0;
    o->oGravity = 0;
    o->oFriction = 0.0f;
    o->oBuoyancy = 0.0f;
    o->oOpacity = 255;
}
