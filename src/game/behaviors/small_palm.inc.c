
void bhv_small_palm_loop(void) {
    s32 action = 0;
    s16 plantAngle = o->oAngleToMario - o->oFaceAngleYaw;
    if (o->oDistanceToMario < 200.0f) {
        if (plantAngle < -0x4000) action = 2;
        else if (plantAngle > 0x4000) action = 3;
        else action = 1;
        if (o->oSoundStateID == action) {
            o->header.gfx.animInfo.animFrame = approach_f32_asymptotic(
                o->header.gfx.animInfo.animFrame,
                MAX(
                    MIN(
                        get_relative_position_between_ranges(
                            o->oDistanceToMario,
                            200.0f,
                            50.0f,
                            59,
                            30
                        ),
                        59
                    ),
                    30
                ),
                0.1f
            );
        } else if (o->oSoundStateID != 0 && o->header.gfx.animInfo.animFrame <= 52) {
            o->header.gfx.animInfo.animFrame = MIN(
                o->header.gfx.animInfo.animFrame + 4,
                58
            );
        }
    }

    // if (o->oDistanceToMario < 500.0f) {
    //     play_secondary_music(35, 0, 255, 1000);
    // } else {
    //     func_80321080(50);
    // }

    if (action != 0) {
        if (o->oSoundStateID == 0 || cur_obj_check_if_near_animation_end()) {
            cur_obj_init_animation_with_sound(action);
        }
    } else if (cur_obj_check_if_near_animation_end()) {
        cur_obj_init_animation_with_sound(action);
    }
}
