
#define SLIDING_DOOR_CUTSCENE_END_HEIGHT 550.0f
#define SLIDING_DOOR_GOAL_HEIGHT 700.0f
#define SLIDING_DOOR_VELY_1 25.0f
#define SLIDING_DOOR_VELY_2 16.0f
#define SLIDING_DOOR_TIME_START  20
#define SLIDING_DOOR_JOLT_END    26
#define SLIDING_DOOR_KICKBACK_END 27
#define SLIDING_DOOR_SLIDE_START 32

void sliding_door_act_3(void) {
    return;
}

void sliding_door_act_2(void) {
    o->oForwardVel = 0.0f;
    o->oVelY = 0.0f;
    o->oAction = 3;
}

void sliding_door_act_1(void) {
    if (o->oTimer == 0) {
        // cutscene_object(CUTSCENE_POV_TO_OBJECT_FAST, o);
        gPuppyCam.targetObj2 = o;
        gPrevPuppyZoomDist = gPuppyCam.zoomTarget;
        gPuppyCam.zoomTarget = o->oDistanceToMario + 1000;
        enable_time_stop_including_mario();
    }

    o->oVelY = 0.0f;

    if (o->oTimer > SLIDING_DOOR_SLIDE_START)
    {
        o->oPosY += SLIDING_DOOR_VELY_2;
        // play sliding sound
    }
    if (o->oTimer > SLIDING_DOOR_KICKBACK_END)
    {
        // nothing
    }
    if (o->oTimer > SLIDING_DOOR_JOLT_END)
    {
        o->oPosY -= 10.0f;
    }
    else if (o->oTimer > SLIDING_DOOR_TIME_START)
    {
        o->oPosY += SLIDING_DOOR_VELY_1;
        spawn_mist_particles_variable(0, 0, 45.0f);
        // gPuppyCam.targetObj = o;
        // gPuppyCam.targetObj2 = NULL;
        // play jolt sound
    }

    if (ABS(o->oHomeY - o->oPosY) >= SLIDING_DOOR_GOAL_HEIGHT)
    {
        o->oAction = 2;
    }
    else if (ABS(o->oHomeY - o->oPosY) >= SLIDING_DOOR_CUTSCENE_END_HEIGHT)
    {
        gPuppyCam.targetObj2 = NULL;
        gObjCutsceneDone = TRUE;
        gPuppyCam.zoomTarget = gPrevPuppyZoomDist;
        disable_time_stop_including_mario();
    }
}

void sliding_door_act_0(void) {
    s32 parasiteGroup = (o->oBehParams >> 16) & 0xFF;
    o->oVelY = 0.0f;
    if (
        gParasitesGrabbed[parasiteGroup] > 0 &&
        gParasitesGoals[parasiteGroup] == gParasitesGrabbed[parasiteGroup]
    ) {
        o->oAction = 1;
        gMarioState->lastParaGroup = -1;
    }
}

void (*sSlidingDoorActions[])(void) = { sliding_door_act_0, sliding_door_act_1,
                                          sliding_door_act_2, sliding_door_act_3 };

void bhv_sliding_door_loop(void) {
    cur_obj_call_action_function(sSlidingDoorActions);
}
