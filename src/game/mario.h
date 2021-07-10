#ifndef MARIO_H
#define MARIO_H

#include <PR/ultratypes.h>

#include "macros.h"
#include "types.h"


#define CUTSCENE_NONE             0
#define CUTSCENE_INTRO            1
#define CUTSCENE_SLIDE            2
#define CUTSCENE_END              3
#define CUTSCENE_DOOR_OPEN        4
#define CUTSCENE_TOWERCLIMB       5
#define CUTSCENE_FORCE_YAW        6
#define CUTSCENE_INTRO_TEMPLE     7
#define CUTSCENE_ORB_REVEAL       8
#define CUTSCENE_LUCYS_LEVITATION 9
#define CUTSCENE_RING_REMINDER    10


enum CUTSCENE_INTRO_ACTS {
    CUTSCENE_INTRO_DOOR_CLOSE_UP = 321,
    CUTSCENE_INTRO_DOOR_HEAD_APPEARS = 380,
    CUTSCENE_INTRO_ORB_APPEAR = 430,
    CUTSCENE_INTRO_DOOR_HEAD_SIDE_VIEW = 441,
    CUTSCENE_INTRO_DOOR_FOLLOW_ORB = 550,
    CUTSCENE_INTRO_LUCY_LOOKS_OVER = 710,
    CUTSCENE_INTRO_LUCY_WALKS_OVER = 740,
    CUTSCENE_INTRO_END = 860
};

enum TEMPLE_INTRO_ACTS {
    TEMPLE_INTRO_SHOW_TITLE_SCREEN = 214,
    TEMPLE_INTRO_WALKING_DOWN_HALLWAY = 316,
    TEMPLE_INTRO_WALKING_OUT_OF_HALLWAY = 421,
    TEMPLE_INTRO_DOOR_SLAM_SHOT_START = 547,
    TEMPLE_INTRO_DOOR_SLAM_LUCY_NOTICE = 575,
    TEMPLE_INTRO_DOOR_SLAM_FROM_HALLWAY = 584,
    TEMPLE_INTRO_DOOR_SHUT_REVERSE_SHOT = 637,
    TEMPLE_INTRO_LUCY_CLOSE_UP = 697,
    TEMPLE_INTRO_FINAL_FRAME = 749
};

enum ORB_REVEAL_ACTS {
    ORB_REVEAL_BEHIND_LUCY = 261,
    ORB_REVEAL_INSIDE_ORB  = 308,
    ORB_REVEAL_FINAL_FRAME = 359
};

enum LUCYS_LEVITATION_ACTS {
    LUCYS_LEVITATION_START_ACCEPTING = 120,
    LUCYS_LEVITATION_ACCEPTED = 134,
    LUCYS_LEVITATION_NIRVANA  = 190,
    LUCYS_LEVITATION_START_CREDITS  = 1605,
    LUCYS_LEVITATION_FINAL_FRAME = 4920
};

enum LUCY_EYE_STATES {
    LUCY_EYE_OPEN,
    LUCY_EYE_HALF,
    LUCY_EYE_SHUT,
    LUCY_EYE_WIDE,
    LUCY_EYE_SAD
};

enum LUCY_MOUTH_STATES {
    LUCY_MOUTH_CLOSED,
    LUCY_MOUTH_SMILE,
    LUCY_MOUTH_HAPPY_OPEN,
    LUCY_MOUTH_OPEN
};

#define LUCY_IDLE_ANIM                  0
#define LUCY_TO_IDLE_ANIM               1
#define LUCY_SITTING_ANIM               2
#define LUCY_SITTING_LOOKING_OVER_ANIM  3
#define LUCY_HAPPY_DANCE_ANIM           4
#define LUCY_LEVITATING_START           5
#define LUCY_LEVITATING                 6

s32 is_anim_at_end(struct MarioState *m);
s32 is_anim_past_end(struct MarioState *m);
s16 set_mario_animation(struct MarioState *m, s32 targetAnimID);
s16 set_custom_mario_animation(struct MarioState *m, s32 targetAnimID);
s16 set_custom_mario_animation_accel(struct MarioState *m, s32 targetAnimID, s32 accel);
s16 set_mario_anim_with_accel(struct MarioState *m, s32 targetAnimID, s32 accel);
void set_anim_to_frame(struct MarioState *m, s16 animFrame);
s32 is_anim_past_frame(struct MarioState *m, s16 animFrame);
s16 find_mario_anim_flags_and_translation(struct Object *o, s32 yaw, Vec3s translation);
void update_mario_pos_for_anim(struct MarioState *m);
s16 return_mario_anim_y_translation(struct MarioState *m);
void play_sound_if_no_flag(struct MarioState *m, u32 soundBits, u32 flags);
void play_mario_jump_sound(struct MarioState *m);
void adjust_sound_for_speed(struct MarioState *m);
void play_sound_and_spawn_particles(struct MarioState *m, u32 soundBits, u32 waveParticleType);
void play_mario_action_sound(struct MarioState *m, u32 soundBits, u32 waveParticleType);
void play_mario_landing_sound(struct MarioState *m, u32 soundBits);
void play_mario_landing_sound_once(struct MarioState *m, u32 soundBits);
void play_mario_heavy_landing_sound(struct MarioState *m, u32 soundBits);
void play_mario_heavy_landing_sound_once(struct MarioState *m, u32 soundBits);
void play_mario_sound(struct MarioState *m, s32 primarySoundBits, s32 scondarySoundBits);
void mario_set_forward_vel(struct MarioState *m, f32 speed);
s32 mario_get_floor_class(struct MarioState *m);
u32 mario_get_terrain_sound_addend(struct MarioState *m);
struct Surface *resolve_and_return_wall_collisions(Vec3f pos, f32 offset, f32 radius);
f32 vec3f_find_ceil(Vec3f pos, f32 height, struct Surface **ceil);
s32 mario_facing_downhill(struct MarioState *m, s32 turnYaw);
u32 mario_floor_is_slippery(struct MarioState *m);
s32 mario_floor_is_slope(struct MarioState *m);
s32 mario_floor_is_steep(struct MarioState *m);
f32 find_floor_height_relative_polar(struct MarioState *m, s16 angleFromMario, f32 distFromMario);
s16 find_floor_slope(struct MarioState *m, s16 yawOffset);
void update_mario_sound_and_camera(struct MarioState *m);
void set_steep_jump_action(struct MarioState *m);
u32 set_mario_action(struct MarioState *, u32 action, u32 actionArg);
s32 set_jump_from_landing(struct MarioState *m);
s32 set_jumping_action(struct MarioState *m, u32 action, u32 actionArg);
s32 drop_and_set_mario_action(struct MarioState *m, u32 action, u32 actionArg);
s32 hurt_and_set_mario_action(struct MarioState *m, u32 action, u32 actionArg, s16 hurtCounter);
s32 check_common_action_exits(struct MarioState *m);
s32 check_common_hold_action_exits(struct MarioState *m);
s32 transition_submerged_to_walking(struct MarioState *m);
s32 transition_submerged_to_airborne(struct MarioState *m);
s32 set_water_plunge_action(struct MarioState *m);
s32 execute_mario_action(UNUSED struct Object *o);
void warp_mario_to_checkpoint(void);
void init_mario(void);
void init_mario_from_save_file(void);
void set_current_cutscene(s32 cutscene);
void set_delayed_mario_warp(s32 warpOp);
void slow_warp_mario_to_checkpoint(void);
void set_object_checkpoint(void);
extern s8 gCheckingWaterForMario;
extern s8 gGameStarted;
extern s8 gCurCutscene;
extern u32 gCurCutsceneTimer;
extern s8 gWaitingToStart;
extern u32 gStartWaitTimer;
extern Vec3f gFinalOrbPos;
extern u32 gSpeedRunTimer;

#endif // MARIO_H
