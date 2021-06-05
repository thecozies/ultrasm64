// water_ring.c.inc

f32 water_ring_calc_mario_dist(void) {
    f32 marioDistX = o->oPosX - gMarioObject->header.gfx.pos[0];
    f32 marioDistY = o->oPosY - (gMarioObject->header.gfx.pos[1] + 64.0f);
    f32 marioDistZ = o->oPosZ - gMarioObject->header.gfx.pos[2];
    f32 marioDistInFront = marioDistX * o->oWaterRingNormalX + marioDistY * o->oWaterRingNormalY
                           + marioDistZ * o->oWaterRingNormalZ;

    return marioDistInFront;
}

void water_ring_init(void) {
    cur_obj_init_animation(0);
    o->oWaterRingScalePhaseX = (s32)(random_float() * 4096.0f) + 0x1000;
    o->oWaterRingScalePhaseY = (s32)(random_float() * 4096.0f) + 0x1000;
    o->oWaterRingScalePhaseZ = (s32)(random_float() * 4096.0f) + 0x1000;

    //! This normal calculation assumes a facing yaw of 0, which is not the case
    //  for the manta ray rings. It also errs by multiplying the normal X by -1.
    //  This cause the ring's orientation for the purposes of collision to be
    //  different than the graphical orientation, which means that Mario won't
    //  necessarily collect a ring even if he appears to swim through it.
    if (!o->oFaceAnglePitch) {
        o->oWaterRingNormalX = coss(o->oFaceAngleYaw * -1) * sins(o->oFaceAngleRoll);
        o->oWaterRingNormalY = coss(o->oFaceAngleYaw * -1) * coss(o->oFaceAngleRoll);
        o->oWaterRingNormalZ = sins(o->oFaceAngleYaw * -1);
    } else {
        o->oWaterRingNormalX = coss(o->oFaceAnglePitch * -1) * sins(o->oFaceAngleRoll);
        o->oWaterRingNormalY = coss(o->oFaceAnglePitch * -1) * coss(o->oFaceAngleRoll);
        o->oWaterRingNormalZ = sins(o->oFaceAnglePitch * -1);
        o->oFaceAngleYaw = o->oFaceAnglePitch;
        o->oFaceAnglePitch = 0;
    }

    o->oWaterRingMarioDistInFront = water_ring_calc_mario_dist();

    // Adding this code will alter the ring's graphical orientation to align with the faulty
    // collision orientation:
    // o->oFaceAngleRoll *= -1;
}

void bhv_jet_stream_water_ring_init(void) {
    water_ring_init();
    o->oOpacity = 200;
    cur_obj_init_animation(0);
}

// sp28 = arg0
// sp2c = ringManager

void water_ring_interaction(void) {
    struct Object *ringSpawner;
    o->oInteractStatus = INT_STATUS_INTERACTED;

    ringSpawner = o->parentObj;
    if (ringSpawner) {
        ringSpawner->oWaterRingSpawnerRingsCollected++;
        play_sound(SOUND_MENU_STAR_SOUND, gGlobalSoundSource);
    }

    o->oIntangibleTimer = 20;
    o->oAction = WATER_RING_ACT_COLLECTED;

    gMarioState->canAirJump = TRUE;
}

void water_ring_check_collection(f32 avgScale, struct Object *ringManager) {
    f32 marioDistInFront = water_ring_calc_mario_dist();
    struct Object *ringSpawner;

    if (!is_point_close_to_object(o, gMarioObject->header.gfx.pos[0],
                                  gMarioObject->header.gfx.pos[1] + 64.0f,
                                  gMarioObject->header.gfx.pos[2], avgScale * 150.0)) {
        o->oWaterRingMarioDistInFront = marioDistInFront;
        return;
    }

    if (o->oWaterRingMarioDistInFront * marioDistInFront < 0 || ABS(marioDistInFront) <= 40) {
        water_ring_interaction();
    }

    o->oWaterRingMarioDistInFront = marioDistInFront;
}

void water_ring_set_scale(f32 avgScale) {
    o->header.gfx.scale[0] = sins(o->oWaterRingScalePhaseX) * 0.1 + avgScale;
    o->header.gfx.scale[1] = sins(o->oWaterRingScalePhaseY) * 0.2 + avgScale;
    o->header.gfx.scale[2] = sins(o->oWaterRingScalePhaseZ) * 0.1 + avgScale;
    o->oWaterRingScalePhaseX += 0x1000;
    o->oWaterRingScalePhaseY += 0x1000;
    o->oWaterRingScalePhaseZ += 0x1000;
}

void water_ring_act_collected(void) {
    f32 avgScale = (f32) o->oTimer * 0.2 + o->oWaterRingAvgScale;

    if (o->oTimer >= 21)
        o->activeFlags = ACTIVE_FLAG_DEACTIVATED;

    o->oOpacity -= 10;
    if (o->oOpacity < 0)
        o->oOpacity = 0;

    water_ring_set_scale(avgScale);
}

#define WATER_RING_VISIBILITY_MAX_FADE 5000.0f
#define WATER_RING_VISIBILITY_MIN_FADE 4250.0f

#define WATER_RING_VISIBILITY_MAX_FADE_TOWER 2000.0f
#define WATER_RING_VISIBILITY_MIN_FADE_TOWER 1750.0f

void water_ring_act_not_collected(void) {
    // f32 avgScale = (f32) o->oTimer / 225.0 * 3.0 + 0.5;
    f32 avgScale = (f32) min(30.0, o->oTimer) / 50.0 * 3.0 + 0.5;
    f32 dist, distPow;
    f32 minFade = gCurrAreaIndex != 5 ? WATER_RING_VISIBILITY_MIN_FADE : WATER_RING_VISIBILITY_MIN_FADE_TOWER;
    f32 maxFade = gCurrAreaIndex != 5 ? WATER_RING_VISIBILITY_MAX_FADE : WATER_RING_VISIBILITY_MAX_FADE_TOWER;

    //! In this case ringSpawner and ringManager are the same object,
    //  because the Jet Stream Ring Spawner is its own parent object.
    struct Object *ringSpawner = o->parentObj;
    struct Object *ringManager = ringSpawner->parentObj;

    water_ring_check_collection(avgScale, ringManager);
    water_ring_set_scale(avgScale);

    // set_object_visibility(o, 5000);
    dist = dist_between_objects(o, gMarioObject);
    if (dist >= maxFade) {
        o->header.gfx.node.flags |= GRAPH_RENDER_INVISIBLE;
    } else {
        o->header.gfx.node.flags &= ~GRAPH_RENDER_INVISIBLE;
        distPow = get_relative_position_between_ranges(dist, minFade, maxFade, 1.0f, 0.0f);
        o->oOpacity = sins(o->oTimer * 0x0800) * 20.0f + 180.0f;
        if (dist >= minFade) o->oOpacity *= distPow; 
    }


    o->oWaterRingAvgScale = avgScale;
}

void bhv_jet_stream_water_ring_loop(void) {
    switch (o->oAction) {
        case WATER_RING_ACT_NOT_COLLECTED:
            water_ring_act_not_collected();
            break;

        case WATER_RING_ACT_COLLECTED:
            water_ring_act_collected();
            break;
    }
}


void Goldwater_ring_act_collected(void) {
    f32 avgScale = (f32) o->oTimer * 0.2 + o->oWaterRingAvgScale;

    if (o->oTimer >= 20)
        o->activeFlags = 0;

    o->oOpacity -= 10;
    if (o->oOpacity < 0)
        o->oOpacity = 0;

    water_ring_set_scale(avgScale);
}

void bhv_gold_water_ring_loop(void) {
    switch (o->oAction) {
        case WATER_RING_ACT_NOT_COLLECTED:
            water_ring_act_not_collected();
            break;

        case WATER_RING_ACT_COLLECTED:
            water_ring_act_collected();
            break;
    }
}

void spawn_manta_ray_ring_manager(void) {
    struct Object *ringManager = spawn_object(o, MODEL_NONE, bhvMantaRayRingManager);
    o->parentObj = ringManager;
}

void water_ring_spawner_act_active(void) {
    //! The Jet Stream Ring Spawner is its own parent object. The code may have been copied
    //  from the Manta Ray, which spawns rings but also has a Ring Manager object as its
    //  parent. The Jet Stream Ring Spawner functions as both a spawner and a Ring Manager.
    struct Object *waterRing;

    if (o->oTimer == 50 && o->oAction != JS_RING_SPAWNER_ACT_INACTIVE) {
        waterRing = spawn_object(o, MODEL_WATER_RING, bhvJetStreamWaterRing);
    }
}

void water_ring_spawner_act_inactive(void) {
    if (o->oTimer == 1000)
        o->oTimer = 0;
    o->oWaterRingSpawnerRingsCollected = 0;
    o->oAction = JS_RING_SPAWNER_ACT_ACTIVE;
}

void bhv_jet_stream_ring_spawner_loop(void) {
    switch (o->oAction) {
        case JS_RING_SPAWNER_ACT_ACTIVE:
            water_ring_spawner_act_active();

            if (o->oWaterRingSpawnerRingsCollected == 1) {
                o->oTimer = 0;
                o->oAction = JS_RING_SPAWNER_ACT_INACTIVE;
            }
            break;

        case JS_RING_SPAWNER_ACT_INACTIVE:
            water_ring_spawner_act_inactive();
            break;
    }
}

void bhv_manta_ray_water_ring_init(void) {
    water_ring_init();
    o->oOpacity = 150;
}

void manta_water_ring_act_not_collected(void) {
    f32 avgScale = (f32) o->oTimer / 50.0f * 1.3 + 0.1;
    struct Object *ringSpawner = o->parentObj;
    struct Object *ringManager = ringSpawner->parentObj;

    if (avgScale > 1.3)
        avgScale = 1.3;

    if (o->oTimer >= 151) {
        o->oOpacity -= 2;
        if (o->oOpacity < 3)
            o->activeFlags = ACTIVE_FLAG_DEACTIVATED;
    }

    water_ring_check_collection(avgScale, ringManager);
    water_ring_set_scale(avgScale);
    set_object_visibility(o, 5000);

    if (ringSpawner->oWaterRingSpawnerRingsCollected == 4
        && o->oWaterRingIndex == ringManager->oWaterRingMgrLastRingCollected + 1)
        o->oOpacity = sins(o->oTimer * 0x1000) * 200.0f + 50.0f;

    o->oWaterRingAvgScale = avgScale;
}

void bhv_manta_ray_water_ring_loop(void) {
    switch (o->oAction) {
        case WATER_RING_ACT_NOT_COLLECTED:
            manta_water_ring_act_not_collected();
            break;

        case WATER_RING_ACT_COLLECTED:
            water_ring_act_collected();
            break;
    }
}
