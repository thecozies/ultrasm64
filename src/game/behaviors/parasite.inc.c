// mushroom_1up.c.inc

// gParasitesGrabbed

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
    } else if (gParasitesGrabbed[parasiteGroup] == gParasitesGoals[parasiteGroup]) {
        o->activeFlags = ACTIVE_FLAG_DEACTIVATED;
    } else if (o->oOpacity != 0) {
        o->oOpacity -= 15;
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
    gParasitesGoals[parasiteGroup]++;
    o->oMoveAnglePitch = 0;
    o->oGravity = 0;
    o->oFriction = 0.0f;
    o->oBuoyancy = 0.0f;
    o->oOpacity = 255;
}
