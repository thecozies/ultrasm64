// warp.c.inc

void bhv_checkpoint_loop(void) {
    if (o->oDistanceToMario < 500.0f) {
        gMarioState->checkpointPos[0] = o->oPosX;
        gMarioState->checkpointPos[1] = o->oPosY;
        gMarioState->checkpointPos[2] = o->oPosZ;
    }
}
