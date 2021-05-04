
void bhv_small_palm_loop(void) {
    s32 action = 0;
    s16 plantAngle = o->oAngleToMario - o->oFaceAngleYaw;
    if (o->oDistanceToMario < 200.0f) {
        if (plantAngle < -0x4000) action = 2;
        else if (plantAngle > 0x4000) action = 3;
        else action = 1;
    }

    if (action != 0) {
        cur_obj_init_animation(action);
    } else if (cur_obj_check_if_near_animation_end()) {
        cur_obj_init_animation(action);
    }
}
