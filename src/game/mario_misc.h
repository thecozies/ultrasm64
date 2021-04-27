#ifndef MARIO_MISC_H
#define MARIO_MISC_H

#include <PR/ultratypes.h>

#include "macros.h"
#include "types.h"

extern struct GraphNodeObject gMirrorMario;
extern struct MarioBodyState gBodyStates[2];

void bhv_toad_message_loop(void);
void bhv_toad_message_init(void);
void bhv_unlock_door_star_init(void);
void bhv_unlock_door_star_loop(void);
Gfx *geo_mirror_mario_set_alpha(s32 callContext, struct GraphNode *node, UNUSED Mat4 *c);
Gfx *geo_switch_mario_stand_run(s32 callContext, struct GraphNode *node, UNUSED Mat4 *mtx);
Gfx *geo_switch_mario_eyes(s32 callContext, struct GraphNode *node, UNUSED Mat4 *c);
Gfx *geo_mario_tilt_torso(s32 callContext, struct GraphNode *node, UNUSED Mat4 *c);
Gfx *geo_mario_head_rotation(s32 callContext, struct GraphNode *node, UNUSED Mat4 *c);
Gfx *geo_switch_mario_hand(s32 callContext, struct GraphNode *node, UNUSED Mat4 *c);
Gfx *geo_mario_hand_foot_scaler(s32 callContext, struct GraphNode *node, UNUSED Mat4 *c);
Gfx *geo_switch_mario_cap_effect(s32 callContext, struct GraphNode *node, UNUSED Mat4 *c);
Gfx *geo_switch_mario_cap_on_off(s32 callContext, struct GraphNode *node, UNUSED Mat4 *c);
Gfx *geo_mario_rotate_wing_cap_wings(s32 callContext, struct GraphNode *node, UNUSED Mat4 *c);
Gfx *geo_switch_mario_hand_grab_pos(s32 callContext, struct GraphNode *b, Mat4 *mtx);
Gfx *geo_render_mirror_mario(s32 callContext, struct GraphNode *node, UNUSED Mat4 *c);
Gfx *geo_mirror_mario_backface_culling(s32 callContext, struct GraphNode *node, UNUSED Mat4 *c);
Gfx *geo_lucy_eyes(s32 callContext, struct GraphNode *node, UNUSED void *context);
Gfx *geo_lucy_mouth(s32 callContext, struct GraphNode *node, UNUSED void *context);

#endif // MARIO_MISC_H
