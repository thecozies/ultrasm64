
void bhv_parasite_interact(void) {
    if (gCurrLevelNum == LEVEL_PSS) {
        if (o->oRoom != -1) {
            if (gMarioCurrentRoom == o->oRoom) o->oOpacity = MIN(255, o->oOpacity + 10);
            else o->oOpacity = 0;
        } else {
            if (o->oDistanceToMario >= 5000) o->oOpacity = 0;
            else if (o->oDistanceToMario < 300) o->oOpacity = 255;
            else {
                o->oOpacity = (int) MAX(0, MIN(255, get_relative_position_between_ranges(
                    o->oDistanceToMario,
                    300.0f,
                    5000.0f,
                    255,
                    0
                )));
            }
        }
    } else {
        o->oOpacity = 255;
    }

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
    if (parasiteGroup >= 1) o->oOpacity = 0;
    else o->oOpacity = 255;
}
