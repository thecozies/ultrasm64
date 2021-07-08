#include <PR/ultratypes.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "prevent_bss_reordering.h"
#include "area.h"
#include "sm64.h"
#include "gfx_dimensions.h"
#include "behavior_data.h"
#include "game_init.h"
#include "object_list_processor.h"
#include "engine/surface_load.h"
#include "ingame_menu.h"
#include "screen_transition.h"
#include "mario.h"
#include "mario_actions_cutscene.h"
#include "print.h"
#include "hud.h"
#include "audio/external.h"
#include "area.h"
#include "rendering_graph_node.h"
#include "level_update.h"
#include "engine/geo_layout.h"
#include "save_file.h"
#include "level_table.h"
#include "game_init.h"
#include "s2dex/init.h"
#include "s2dex/s2d_draw.h"
#include "s2dex/s2d_print.h"
#include "s2dex/s2d_buffer.h"
#include "tutorial.h"


struct SpawnInfo gPlayerSpawnInfos[1];
struct GraphNode *D_8033A160[0x100];
struct Area gAreaData[8];

struct WarpTransition gWarpTransition;

s16 gCurrCourseNum;
s16 gCurrActNum;
s16 gCurrAreaIndex;
s16 gSavedCourseNum;
s16 gPauseScreenMode;
s16 gSaveOptSelectIndex;

struct SpawnInfo *gMarioSpawnInfo = &gPlayerSpawnInfos[0];
struct GraphNode **gLoadedGraphNodes = D_8033A160;
struct Area *gAreas = gAreaData;
struct Area *gCurrentArea = NULL;
struct CreditsEntry *gCurrCreditsEntry = NULL;
Vp *D_8032CE74 = NULL;
Vp *D_8032CE78 = NULL;
s16 gWarpTransDelay = 0;
u32 gFBSetColor = 0;
u32 gWarpTransFBSetColor = 0;
u8 gWarpTransRed = 0;
u8 gWarpTransGreen = 0;
u8 gWarpTransBlue = 0;
s16 gCurrSaveFileNum = 1;
s16 gCurrLevelNum = LEVEL_MIN;

struct GlobalFog gGlobalFog = { 73, 63, 52, 0xFF, 832, 1000 };
struct GlobalFog sDefaultFog = { 73, 63, 52, 0xFF, 832, 1000 };
struct GlobalFog sWaterFog = { 0x33, 0x6C, 0x71, 0xDF, 800, 1000 };
struct GlobalFog sTemple1Room4Fog = { 73, 63, 52, 0xFF, 812, 1100 };
struct GlobalFog sTrippyFog = { 73, 63, 52, 0xFF, 750, 1100 };
struct GlobalFog sTemple2Fog = { 94, 88, 105, 0xDF, 830, 1100 };
struct GlobalFog sTutorialFog = { 22, 22, 22, 0xFF, 350, 1000 };
struct GlobalFog sEndFog = { 74, 68, 85, 0xDF, 900, 1100 };
struct GlobalFog sEndTunnelFog = { 24, 20, 28, 0xFF, 850, 1000 };
struct GlobalFog sEndFinalFog = { 114, 108, 125, 0xDF, 900, 1100 };
struct GlobalFog sCampFog = { 0x17, 0x17, 0x17, 0xDF, 800, 1000 };

s16 gGoalFadeState = 0;
s32 sGoalFadeTimer = 0;
s8 gIntroTextShowing = FALSE;
s8 sShowFPS = FALSE;
s32 gIntroTextTimer = 0;
// s32 gIntroTextPos = 0;
s32 gCameraWaterLevel = FLOOR_LOWER_LIMIT;
u8 sFadeAlpha = 0;
f32 sFPS = 0.0f;
s8 gGameIsLagging = FALSE;
u32 gFramesWithoutLag = 30;

char sGameTitle[] = "Lucy's\nLevitation";
char sPressStartToPlay[] = "Press Start";
char sNewGame[] = "New Game";
char sContinueGame[] = "Continue";
char sContinueGameDisabled[] = COLOR_GREY "Continue";
char sEnabled[] = "Enabled";
char sDisabled[] = COLOR_GREY "Disabled";
char s16By9[] = "Widescreen:";
char sChallengeMode[] = "Challenge Mode:";
char sSpeedrunMode[] = "Speedrun Mode:";
char sOptions[] = "Options";
char sOptionsReturn[] = "Return";

u8 gChallengeMode = FALSE;
u8 gSpeedrunMode = FALSE;

s32 sTimesRenderedText = 0;
s8 sNumOptions = 1;
s8 gSelectedOption = 0;
s8 sWaitingForStickReturn = FALSE;
s8 gHasCheckpoint = FALSE;
s8 gHasChallengeCheckpoint = FALSE;
s8 sOptionsMenuOpen = FALSE;

s8 sCurFog = DEFAULT_FOG;
u8 sTimeInRoom[4] = {50, 50, 50, 50};
u8 sTimeLeftRoom[4] = {0, 0, 0, 0};

u8 gCurTipType = 0;
s8 sTriggeredRingReminder = FALSE;

enum GOAL_FADE_STATES {
    NO_GOAL,
    STARTING_SHOW_GOAL,
    FULL_SHOW_GOAL,
    FADING_SHOW_GOAL
};

#define GOAL_FADE_IN_LEN 30
#define GOAL_SHOW_LEN 60
#define GOAL_FADE_OUT_LEN 30

/*
 * The following two tables are used in get_mario_spawn_type() to determine spawn type
 * from warp behavior.
 * When looping through sWarpBhvSpawnTable, if the behavior function in the table matches
 * the spawn behavior executed, the index of that behavior is used with sSpawnTypeFromWarpBhv
*/

const BehaviorScript *sWarpBhvSpawnTable[] = {
    bhvDoorWarp,                bhvStar,                   bhvExitPodiumWarp,          bhvWarp,
    bhvWarpPipe,                bhvFadingWarp,             bhvInstantActiveWarp,       bhvAirborneWarp,
    bhvHardAirKnockBackWarp,    bhvSpinAirborneCircleWarp, bhvDeathWarp,               bhvSpinAirborneWarp,
    bhvFlyingWarp,              bhvSwimmingWarp,           bhvPaintingStarCollectWarp, bhvPaintingDeathWarp,
    bhvAirborneStarCollectWarp, bhvAirborneDeathWarp,      bhvLaunchStarCollectWarp,   bhvLaunchDeathWarp,
};

u8 sSpawnTypeFromWarpBhv[] = {
    MARIO_SPAWN_DOOR_WARP,             MARIO_SPAWN_UNKNOWN_02,           MARIO_SPAWN_UNKNOWN_03,            MARIO_SPAWN_UNKNOWN_03,
    MARIO_SPAWN_UNKNOWN_03,            MARIO_SPAWN_TELEPORT,             MARIO_SPAWN_INSTANT_ACTIVE,        MARIO_SPAWN_AIRBORNE,
    MARIO_SPAWN_HARD_AIR_KNOCKBACK,    MARIO_SPAWN_SPIN_AIRBORNE_CIRCLE, MARIO_SPAWN_DEATH,                 MARIO_SPAWN_SPIN_AIRBORNE,
    MARIO_SPAWN_FLYING,                MARIO_SPAWN_SWIMMING,             MARIO_SPAWN_PAINTING_STAR_COLLECT, MARIO_SPAWN_PAINTING_DEATH,
    MARIO_SPAWN_AIRBORNE_STAR_COLLECT, MARIO_SPAWN_AIRBORNE_DEATH,       MARIO_SPAWN_LAUNCH_STAR_COLLECT,   MARIO_SPAWN_LAUNCH_DEATH,
};

Vp D_8032CF00 = { {
    { 640, 480, 511, 0 },
    { 640, 480, 511, 0 },
} };


// char myString[] = "Where am I\n"
//                 "                    ";
// char myString[] = "This is a " SCALE "2" "test string!\n"
//                 "\tThis is " COLOR "255 0 0 0" "Colorful text!";

#ifdef VERSION_EU
const char *gNoControllerMsg[] = {
    "NO CONTROLLER",
    "MANETTE DEBRANCHEE",
    "CONTROLLER FEHLT",
};
#endif


#define FRAMETIME_COUNT 30
#define GAME_IS_LAGGING_WAIT_FOR_CALC 15.0f
#define GAME_IS_LAGGING_THRESHOLD 27.0f

OSTime frameTimes[FRAMETIME_COUNT];
u8 curFrameTimeIndex = 0;

#include "PR/os_convert.h"

// Call once per frame
void calculate_and_update_fps(void)
{
    OSTime newTime = osGetTime();
    OSTime oldTime = frameTimes[curFrameTimeIndex];
    frameTimes[curFrameTimeIndex] = newTime;

    curFrameTimeIndex++;
    if (curFrameTimeIndex >= FRAMETIME_COUNT)
        curFrameTimeIndex = 0;

    sFPS = ((f32)FRAMETIME_COUNT * 1000000.0f) / (s32)OS_CYCLES_TO_USEC(newTime - oldTime);
    gGameIsLagging = gIsConsole ? sFPS > GAME_IS_LAGGING_WAIT_FOR_CALC && sFPS < GAME_IS_LAGGING_THRESHOLD : FALSE;
    if (gGameIsLagging) gFramesWithoutLag = 0;
    else gFramesWithoutLag = MIN(1000, gFramesWithoutLag + 1);
}

void print_fps(s32 x, s32 y)
{
    char text[10];
    sprintf(text, "%2.2f", sFPS);
    if (gGameIsLagging) s2d_print_deferred(x, y, ALIGN_LEFT, 255, 0.5f, text);
    else s2d_print_deferred(x, y, ALIGN_LEFT, 100, 0.5f, text);
}

void override_viewport_and_clip(Vp *a, Vp *b, u8 c, u8 d, u8 e) {
    u16 sp6 = ((c >> 3) << 11) | ((d >> 3) << 6) | ((e >> 3) << 1) | 1;

    gFBSetColor = (sp6 << 16) | sp6;
    D_8032CE74 = a;
    D_8032CE78 = b;
}

void set_warp_transition_rgb(u8 red, u8 green, u8 blue) {
    u16 warpTransitionRGBA16 = ((red >> 3) << 11) | ((green >> 3) << 6) | ((blue >> 3) << 1) | 1;

    gWarpTransFBSetColor = (warpTransitionRGBA16 << 16) | warpTransitionRGBA16;
    gWarpTransRed = red;
    gWarpTransGreen = green;
    gWarpTransBlue = blue;
}

void print_intro_text(void) {
#ifdef VERSION_EU
    s32 language = eu_get_language();
#endif
    if ((gGlobalTimer & 0x1F) < 20) {
        if (gControllerBits == 0) {
#ifdef VERSION_EU
            print_text_centered(SCREEN_WIDTH / 2, 20, gNoControllerMsg[language]);
#else
            print_text_centered(SCREEN_WIDTH / 2, 20, "NO CONTROLLER");
#endif
        } else {
#ifdef VERSION_EU
            print_text(20, 20, "START");
#else
            print_text_centered(60, 38, "PRESS");
            print_text_centered(60, 20, "START");
#endif
        }
    }
}

u32 get_mario_spawn_type(struct Object *o) {
    s32 i;
    const BehaviorScript *behavior = virtual_to_segmented(0x13, o->behavior);

    for (i = 0; i < 20; i++) {
        if (sWarpBhvSpawnTable[i] == behavior) {
            return sSpawnTypeFromWarpBhv[i];
        }
    }
    return 0;
}

struct ObjectWarpNode *area_get_warp_node(u8 id) {
    struct ObjectWarpNode *node = NULL;

    for (node = gCurrentArea->warpNodes; node != NULL; node = node->next) {
        if (node->node.id == id) {
            break;
        }
    }
    return node;
}

struct ObjectWarpNode *area_get_warp_node_from_params(struct Object *o) {
    u8 sp1F = (o->oBehParams & 0x00FF0000) >> 16;

    return area_get_warp_node(sp1F);
}

void load_obj_warp_nodes(void) {
    struct ObjectWarpNode *sp24;
    struct Object *sp20 = (struct Object *) gObjParentGraphNode.children;

    do {
        struct Object *sp1C = sp20;

        if (sp1C->activeFlags != ACTIVE_FLAG_DEACTIVATED && get_mario_spawn_type(sp1C) != 0) {
            sp24 = area_get_warp_node_from_params(sp1C);
            if (sp24 != NULL) {
                sp24->object = sp1C;
            }
        }
    } while ((sp20 = (struct Object *) sp20->header.gfx.node.next)
             != (struct Object *) gObjParentGraphNode.children);
}

void clear_areas(void) {
    s32 i;

    gCurrentArea = NULL;
    gWarpTransition.isActive = FALSE;
    gWarpTransition.pauseRendering = FALSE;
    gMarioSpawnInfo->areaIndex = -1;

    for (i = 0; i < 8; i++) {
        gAreaData[i].index = i;
        gAreaData[i].flags = 0;
        gAreaData[i].terrainType = 0;
        gAreaData[i].unk04 = NULL;
        gAreaData[i].terrainData = NULL;
        gAreaData[i].surfaceRooms = NULL;
        gAreaData[i].macroObjects = NULL;
        gAreaData[i].warpNodes = NULL;
        gAreaData[i].paintingWarpNodes = NULL;
        gAreaData[i].instantWarps = NULL;
        gAreaData[i].objectSpawnInfos = NULL;
        gAreaData[i].camera = NULL;
        gAreaData[i].unused28 = NULL;
        gAreaData[i].whirlpools[0] = NULL;
        gAreaData[i].whirlpools[1] = NULL;
        gAreaData[i].dialog[0] = 255;
        gAreaData[i].dialog[1] = 255;
        gAreaData[i].musicParam = 0;
        gAreaData[i].musicParam2 = 0;
    }
}

void clear_area_graph_nodes(void) {
    s32 i;

    if (gCurrentArea != NULL) {
        geo_call_global_function_nodes(&gCurrentArea->unk04->node, GEO_CONTEXT_AREA_UNLOAD);
        gCurrentArea = NULL;
        gWarpTransition.isActive = FALSE;
    }

    for (i = 0; i < 8; i++) {
        if (gAreaData[i].unk04 != NULL) {
            geo_call_global_function_nodes(&gAreaData[i].unk04->node, GEO_CONTEXT_AREA_INIT);
            gAreaData[i].unk04 = NULL;
        }
    }
}

void load_area(s32 index) {
    if (gCurrentArea == NULL && gAreaData[index].unk04 != NULL) {
        gCurrentArea = &gAreaData[index];
        gCurrAreaIndex = gCurrentArea->index;

        if (gCurrentArea->terrainData != NULL) {
            load_area_terrain(index, gCurrentArea->terrainData, gCurrentArea->surfaceRooms,
                              gCurrentArea->macroObjects);
        }

        if (gCurrentArea->objectSpawnInfos != NULL) {
            spawn_objects_from_info(0, gCurrentArea->objectSpawnInfos);
        }

        load_obj_warp_nodes();
        geo_call_global_function_nodes(&gCurrentArea->unk04->node, GEO_CONTEXT_AREA_LOAD);
    }
}

void unload_area(void) {
    if (gCurrentArea != NULL) {
        unload_objects_from_area(0, gCurrentArea->index);
        geo_call_global_function_nodes(&gCurrentArea->unk04->node, GEO_CONTEXT_AREA_UNLOAD);

        gCurrentArea->flags = 0;
        gCurrentArea = NULL;
        gWarpTransition.isActive = FALSE;
    }
}

void load_mario_area(void) {
    stop_sounds_in_continuous_banks();
    load_area(gMarioSpawnInfo->areaIndex);

    if (gCurrentArea->index == gMarioSpawnInfo->areaIndex) {
        gCurrentArea->flags |= 0x01;
        spawn_objects_from_info(0, gMarioSpawnInfo);
    }
}

void unload_mario_area(void) {
    if (gCurrentArea != NULL && (gCurrentArea->flags & 0x01)) {
        unload_objects_from_area(0, gMarioSpawnInfo->activeAreaIndex);

        gCurrentArea->flags &= ~0x01;
        if (gCurrentArea->flags == 0) {
            unload_area();
        }
    }
}

void change_area(s32 index) {
    s32 areaFlags = gCurrentArea->flags;

    if (gCurrAreaIndex != index) {
        unload_area();
        load_area(index);

        gCurrentArea->flags = areaFlags;
        gMarioObject->oActiveParticleFlags = 0;
    }

    if (areaFlags & 0x01) {
        gMarioObject->header.gfx.areaIndex = index, gMarioSpawnInfo->areaIndex = index;
    }
}

void area_update_objects(void) {
    gAreaUpdateCounter++;
    update_objects(0);
}

/*
 * Sets up the information needed to play a warp transition, including the
 * transition type, time in frames, and the RGB color that will fill the screen.
 */
void play_transition(s16 transType, s16 time, u8 red, u8 green, u8 blue) {
    gWarpTransition.isActive = TRUE;
    gWarpTransition.type = transType;
    gWarpTransition.time = time;
    gWarpTransition.pauseRendering = FALSE;
    reset_transition_fade_timer(0);

    // The lowest bit of transType determines if the transition is fading in or out.
    if (transType & 1) {
        set_warp_transition_rgb(red, green, blue);
    } else {
        red = gWarpTransRed, green = gWarpTransGreen, blue = gWarpTransBlue;
    }

    if (transType < 8) { // if transition is RGB
        gWarpTransition.data.red = red;
        gWarpTransition.data.green = green;
        gWarpTransition.data.blue = blue;
    } else { // if transition is textured
        gWarpTransition.data.red = red;
        gWarpTransition.data.green = green;
        gWarpTransition.data.blue = blue;

        // Both the start and end textured transition are always located in the middle of the screen.
        // If you really wanted to, you could place the start at one corner and the end at
        // the opposite corner. This will make the transition image look like it is moving
        // across the screen.
        gWarpTransition.data.startTexX = SCREEN_WIDTH / 2;
        gWarpTransition.data.startTexY = SCREEN_HEIGHT / 2;
        gWarpTransition.data.endTexX = SCREEN_WIDTH / 2;
        gWarpTransition.data.endTexY = SCREEN_HEIGHT / 2;

        gWarpTransition.data.texTimer = 0;

        if (transType & 1) // Is the image fading in?
        {
            gWarpTransition.data.startTexRadius = GFX_DIMENSIONS_FULL_RADIUS;
            if (transType >= 0x0F) {
                gWarpTransition.data.endTexRadius = 16;
            } else {
                gWarpTransition.data.endTexRadius = 0;
            }
        } else // The image is fading out. (Reverses start & end circles)
        {
            if (transType >= 0x0E) {
                gWarpTransition.data.startTexRadius = 16;
            } else {
                gWarpTransition.data.startTexRadius = 0;
            }
            gWarpTransition.data.endTexRadius = GFX_DIMENSIONS_FULL_RADIUS;
        }
    }
}

/*
 * Sets up the information needed to play a warp transition, including the
 * transition type, time in frames, and the RGB color that will fill the screen.
 * The transition will play only after a number of frames specified by 'delay'
 */
void play_transition_after_delay(s16 transType, s16 time, u8 red, u8 green, u8 blue, s16 delay) {
    gWarpTransDelay = delay; // Number of frames to delay playing the transition.
    play_transition(transType, time, red, green, blue);
}

void set_next_goal_state(s32 state, s32 nextAlpha) {
    gGoalFadeState = state;
    sGoalFadeTimer = 0;
    sFadeAlpha = nextAlpha;
    if (state == NO_GOAL) {
        gIntroTextShowing = FALSE;
    }
}

void set_collected_para(s32 group) {
    set_next_goal_state(STARTING_SHOW_GOAL, 0);
    gParasitesGrabbed[group]++;
    gMarioState->lastParaGroup = group;
}

void reset_collected_para(void) {
    set_next_goal_state(STARTING_SHOW_GOAL, 0);
    gParasitesGrabbed[gMarioState->lastParaGroup] = 0;
}

s32 update_text_fade(s32 attack, s32 sustain, s32 release) {
    if (gGoalFadeState == NO_GOAL) return FALSE;
    
    if (gGoalFadeState == STARTING_SHOW_GOAL) {
        if (sGoalFadeTimer >= attack) set_next_goal_state(FULL_SHOW_GOAL, 255);
        sFadeAlpha = MIN(sFadeAlpha + 9, 255);
    } else if (gGoalFadeState == FULL_SHOW_GOAL && sGoalFadeTimer >= sustain) {
        set_next_goal_state(FADING_SHOW_GOAL, 255);
        sFadeAlpha = 255;
    } else if (gGoalFadeState == FADING_SHOW_GOAL) { // FADING_SHOW_GOAL
        if (sGoalFadeTimer > release) set_next_goal_state(NO_GOAL, 0);
        sFadeAlpha = MAX(sFadeAlpha - (255 / release), 0);
    }

    sGoalFadeTimer++;
    return TRUE;
}

void render_goals(void) {
    if (
        gGameStarted &&
        gCurrLevelNum == LEVEL_CASTLE_GROUNDS &&
        gCurCutscene != CUTSCENE_INTRO_TEMPLE &&
        !render_tutorial(FALSE) &&
        update_text_fade(GOAL_FADE_IN_LEN, GOAL_SHOW_LEN, GOAL_FADE_OUT_LEN) &&
        gMarioState->lastParaGroup != -1
    ) {
        s2d_init();
        char s1[30];
        s2d_alpha = sFadeAlpha;

        sprintf(
            s1,
            "orbs: %d\ngoal: %d",
            gParasitesGrabbed[gMarioState->lastParaGroup],
            gParasitesGoals[gMarioState->lastParaGroup]
        );
        s2d_print_alloc(40, 20, ALIGN_LEFT, s1);

        s2d_stop();
    }
}

#define INTRO_FADE_IN_LEN 45
#define INTRO_FADE_OUT_LEN 35
#define INTRO_FADE_HOLD_LEN (TEMPLE_INTRO_WALKING_DOWN_HALLWAY - TEMPLE_INTRO_SHOW_TITLE_SCREEN - INTRO_FADE_OUT_LEN - INTRO_FADE_IN_LEN - 5)

void render_intro_text(void) {
    if (
        gCurCutscene == CUTSCENE_INTRO_TEMPLE &&
        gCurCutsceneTimer >= TEMPLE_INTRO_SHOW_TITLE_SCREEN &&
        gCurCutsceneTimer < TEMPLE_INTRO_WALKING_DOWN_HALLWAY &&
        update_text_fade(INTRO_FADE_IN_LEN, INTRO_FADE_HOLD_LEN, INTRO_FADE_OUT_LEN)
    ) {
        // char s1[] = "Lucy's\nLevitation";
        s2d_init();
        s2d_alpha = sFadeAlpha;

        s2d_print_alloc((SCREEN_WIDTH / 2) + 10, (SCREEN_HEIGHT / 4), ALIGN_CENTER, sGameTitle);
        // gIntroTextPos++;
        s2d_stop();
        sTimesRenderedText++;
    }
}

enum DEFAULT_START_OPTIONS {
    OPT_CONTINUE,
    OPT_NEW_GAME,
    OPT_OPTIONS
};

enum DEFAULT_OPTIONS_MENU_OPTIONS {
    OPT_WIDE,
    OPT_RETURN
};

enum SPECIAL_OPTIONS_MENU_OPTIONS {
    OPT_SPECIAL_WIDE,
    OPT_SPECIAL_CHALLENGE,
    OPT_SPECIAL_SPEEDRUN,
    OPT_SPECIAL_RETURN
};

void set_or_clear_special_modes(void) {
    if (gChallengeMode) save_file_set_flags(SAVE_FLAG_CHALLENGE_MODE);
    else save_file_clear_flags(SAVE_FLAG_CHALLENGE_MODE);

    if (gSpeedrunMode) save_file_set_flags(SAVE_FLAG_SPEEDRUN_MODE);
    else save_file_clear_flags(SAVE_FLAG_SPEEDRUN_MODE);
}

// called from mario.c due to needing to warp mario before his code is executed
void handle_waiting_to_start(void) {
    if (gWaitingToStart) {
        s8 canContinue = !gSpeedrunMode && ((gHasCheckpoint && !gChallengeMode) || (gHasChallengeCheckpoint && gChallengeMode));
        s8 canAccessOpt0 = (sOptionsMenuOpen || canContinue);
        s8 minOpt = canAccessOpt0 ? 0 : 1;
        gStartWaitTimer++;

        if (!canAccessOpt0 && gSelectedOption == 0) gSelectedOption = 1;
        if (gSelectedOption > sNumOptions - 1) gSelectedOption = sNumOptions - 1;

        if (sOptionsMenuOpen && gPlayer1Controller->buttonPressed & B_BUTTON) {
            sOptionsMenuOpen = FALSE;
            gSelectedOption = canContinue ? 0 : 1;
            return;
        }

        if (gStartWaitTimer > 60 && gPlayer1Controller->buttonPressed & (START_BUTTON | A_BUTTON)) {
            if (sOptionsMenuOpen && sNumOptions == 2) {
                // opt menu
                switch (gSelectedOption)
                {
                    case OPT_WIDE:
                        gWidescreen = !gWidescreen;
                        break;
                    case OPT_RETURN:
                    default:
                        sOptionsMenuOpen = FALSE;
                        gSelectedOption = canContinue ? 0 : 1;
                        break;
                }
            } else if (sOptionsMenuOpen) {
                // special opt menu
                switch (gSelectedOption)
                {
                    case OPT_SPECIAL_WIDE:
                        gWidescreen = !gWidescreen;
                        break;
                    case OPT_SPECIAL_CHALLENGE:
                        gChallengeMode = !gChallengeMode;
                        gTutorialDone = gSpeedrunMode || gChallengeMode;
                        break;
                    case OPT_SPECIAL_SPEEDRUN:
                        gSpeedrunMode = !gSpeedrunMode;
                        gTutorialDone = gSpeedrunMode || gChallengeMode;
                        break;
                    case OPT_SPECIAL_RETURN:
                    default:
                        sOptionsMenuOpen = FALSE;
                        gSelectedOption = canContinue ? 0 : 1;
                        break;
                }
            } else {
                // main menu
                switch (gSelectedOption)
                {
                    case OPT_CONTINUE:
                        if (gChallengeMode) gCurrSaveFileNum = CHALLENGE_MODE_FILE_NUM;
                        else gCurrSaveFileNum = NORMAL_MODE_FILE_NUM;

                        gWaitingToStart = FALSE;
                        gStartWaitTimer = 0;
                        gTutorialDone = TRUE;
                        level_trigger_warp(gMarioState, WARP_OP_CONTINUE);
                        set_or_clear_special_modes();
                        break;
                    case OPT_NEW_GAME:
                        if (gChallengeMode) gCurrSaveFileNum = CHALLENGE_MODE_FILE_NUM;
                        else gCurrSaveFileNum = NORMAL_MODE_FILE_NUM;

                        gWaitingToStart = FALSE;
                        gStartWaitTimer = 0;
                        if (gChallengeMode || gSpeedrunMode) {
                            gTutorialDone = TRUE;
                            // CTODO: delay warp
                            set_current_cutscene(0);
                            initiate_warp(LEVEL_CASTLE_GROUNDS, 1, 0x0A, 0);
                            set_or_clear_special_modes();
                        }
                        break;
                    case OPT_OPTIONS:
                    default:
                        sOptionsMenuOpen = TRUE;
                        gSelectedOption = 0;
                        break;
                }
            }
            return;
        }

        if (!sWaitingForStickReturn) {
            if (gPlayer1Controller->rawStickY > 60 || gPlayer1Controller->buttonPressed & U_JPAD) {
                if (gSelectedOption - 1 < minOpt) gSelectedOption = sNumOptions - 1;
                else gSelectedOption = MAX(minOpt, gSelectedOption - 1);

                sWaitingForStickReturn = TRUE;
            }

            if (gPlayer1Controller->rawStickY < -60 || gPlayer1Controller->buttonPressed & D_JPAD) {
                if (gSelectedOption + 1 > sNumOptions - 1) gSelectedOption = minOpt;
                else gSelectedOption = MIN(sNumOptions - 1, gSelectedOption + 1);

                sWaitingForStickReturn = TRUE;
            }
        } else if (ABS(gPlayer1Controller->rawStickY) < 4) {
            sWaitingForStickReturn = FALSE;
        }
    }
}

#define OPT_STATUS_LEFT 90
#define OPT_STATUS_LEFT_EXTRA 120

void render_start_options(void) {
    sNumOptions = save_file_get_flags() & SAVE_FLAG_BEAT_GAME ? 4 : 2;
#ifdef CDEBUG
    if (sNumOptions == 2) {
        save_file_set_flags(SAVE_FLAG_BEAT_GAME);
        sNumOptions = 4;
    }
#endif

    if (sNumOptions == 2) {
        s2d_print_deferred(
            20,
            (int) (SCREEN_HEIGHT - 70),
            ALIGN_LEFT,
            MIN(200, sFadeAlpha * (gSelectedOption == 0 ? 1.0f : 0.4f)),
            0.5f,
            s16By9
        );
        s2d_print_deferred(
            OPT_STATUS_LEFT,
            (int) (SCREEN_HEIGHT - 70),
            ALIGN_LEFT,
            MIN(200, sFadeAlpha * (gSelectedOption == 0 ? 1.0f : 0.4f)),
            0.5f,
            gWidescreen ? sEnabled : sDisabled
        );

        s2d_print_deferred(
            20,
            (int) (SCREEN_HEIGHT - 50),
            ALIGN_LEFT,
            MIN(200, sFadeAlpha * (gSelectedOption == 1 ? 1.0f : 0.4f)),
            0.5f,
            sOptionsReturn
        );
    } else {
        s2d_print_deferred(
            20,
            (int) (SCREEN_HEIGHT - 110),
            ALIGN_LEFT,
            MIN(200, sFadeAlpha * (gSelectedOption == OPT_SPECIAL_WIDE ? 1.0f : 0.4f)),
            0.5f,
            s16By9
        );
        s2d_print_deferred(
            OPT_STATUS_LEFT,
            (int) (SCREEN_HEIGHT - 110),
            ALIGN_LEFT,
            MIN(200, sFadeAlpha * (gSelectedOption == OPT_SPECIAL_WIDE ? 1.0f : 0.4f)),
            0.5f,
            gWidescreen ? sEnabled : sDisabled
        );

        s2d_print_deferred(
            20,
            (int) (SCREEN_HEIGHT - 90),
            ALIGN_LEFT,
            MIN(200, sFadeAlpha * (gSelectedOption == OPT_SPECIAL_CHALLENGE ? 1.0f : 0.4f)),
            0.5f,
            sChallengeMode
        );
        s2d_print_deferred(
            OPT_STATUS_LEFT_EXTRA,
            (int) (SCREEN_HEIGHT - 90),
            ALIGN_LEFT,
            MIN(200, sFadeAlpha * (gSelectedOption == OPT_SPECIAL_CHALLENGE ? 1.0f : 0.4f)),
            0.5f,
            gChallengeMode ? sEnabled : sDisabled
        );

        s2d_print_deferred(
            20,
            (int) (SCREEN_HEIGHT - 70),
            ALIGN_LEFT,
            MIN(200, sFadeAlpha * (gSelectedOption == OPT_SPECIAL_SPEEDRUN ? 1.0f : 0.4f)),
            0.5f,
            sSpeedrunMode
        );
        s2d_print_deferred(
            OPT_STATUS_LEFT_EXTRA,
            (int) (SCREEN_HEIGHT - 70),
            ALIGN_LEFT,
            MIN(200, sFadeAlpha * (gSelectedOption == OPT_SPECIAL_SPEEDRUN ? 1.0f : 0.4f)),
            0.5f,
            gSpeedrunMode ? sEnabled : sDisabled
        );

        s2d_print_deferred(
            20,
            (int) (SCREEN_HEIGHT - 50),
            ALIGN_LEFT,
            MIN(200, sFadeAlpha * (gSelectedOption == OPT_SPECIAL_RETURN ? 1.0f : 0.4f)),
            0.5f,
            sOptionsReturn
        );
    }
    
}

void title_screen_waiting_to_start(void) {
    u32 saveFlags = save_file_get_file_num_flags(NORMAL_MODE_FILE_NUM);
    u32 challengeSaveFlags = save_file_get_file_num_flags(CHALLENGE_MODE_FILE_NUM);
    gHasCheckpoint = saveFlags & SAVE_FLAG_HAS_CHECKPOINT;
    gHasChallengeCheckpoint = challengeSaveFlags & SAVE_FLAG_HAS_CHECKPOINT;

    if (gGoalFadeState == FULL_SHOW_GOAL && !gWaitingToStart) set_next_goal_state(FADING_SHOW_GOAL, 150);
    else if (gGoalFadeState == FULL_SHOW_GOAL) sGoalFadeTimer = 0;
    else if (gGoalFadeState == NO_GOAL && gWaitingToStart) set_next_goal_state(STARTING_SHOW_GOAL, 0);

    if (update_text_fade(50, 0xFFFFFFF, 35)) {
        if (sOptionsMenuOpen) {
            render_start_options();
        } else {
            sNumOptions = 3;
            s2d_print_deferred(
                20,
                (int) (SCREEN_HEIGHT - 90),
                ALIGN_LEFT,
                MIN(200, sFadeAlpha * (gSelectedOption == 0 ? 1.0f : (gHasCheckpoint ? 0.4f : 0.2f))),
                0.5f,
                gHasCheckpoint ? sContinueGame : sContinueGameDisabled
            );

            s2d_print_deferred(
                20,
                (int) (SCREEN_HEIGHT - 70),
                ALIGN_LEFT,
                MIN(200, sFadeAlpha * (gSelectedOption == 1 ? 1.0f : 0.4f)),
                0.5f,
                sNewGame
            );

            s2d_print_deferred(
                20,
                (int) (SCREEN_HEIGHT - 50),
                ALIGN_LEFT,
                MIN(200, sFadeAlpha * (gSelectedOption == 2 ? 1.0f : 0.4f)),
                0.5f,
                sOptions
            );
        }
    }
}

// deferred
// void render_intro_text(void) {
//     if (
//         gIntroTextShowing &&
//         !gGameStarted &&
//         update_text_fade(GOAL_FADE_IN_LEN + 15, GOAL_SHOW_LEN, GOAL_FADE_OUT_LEN + 30)
//     ) {
//         s2d_print_deferred(40, (int) (SCREEN_HEIGHT - 80), ALIGN_LEFT, sFadeAlpha, sGameTitle);
//         sTimesRenderedText++;
//     }
// }

void set_current_fog_state(s32 fogState) {
    // Make sure tunnel fog doesn't overwrite the END_FINAL_BOG
    if (fogState == END_TUNNEL_FOG && sCurFog == END_FINAL_FOG) return;
    sCurFog = fogState;
}

void update_fog(void) {
    struct GlobalFog* targetFog;
    if (gCurrLevelNum == LEVEL_CASTLE_COURTYARD && sCurFog != CAMP_FOG) set_current_fog_state(CAMP_FOG);

    gCheckingSurfaceCollisionsForCamera = TRUE;
    gCameraWaterLevel = find_water_level(gLakituState.pos[0], gLakituState.pos[2]);
    gCheckingSurfaceCollisionsForCamera = FALSE;

    if (gCameraWaterLevel > gLakituState.pos[1]) targetFog = &sWaterFog;
    else {
        switch (sCurFog) {
            case WATER_FOG:
                targetFog = &sWaterFog;
                break;
            case TEMPLE1_ROOM4_FOG:
                targetFog = &sTemple1Room4Fog;
                break;
            case TRIPPY_FOG:
                targetFog = &sTrippyFog;
                break;
            case TEMPLE2_FOG:
                targetFog = &sTemple2Fog;
                break;
            case TUTORIAL_FOG:
                targetFog = &sTutorialFog;
                break;
            case END_FOG:
                targetFog = &sEndFog;
                break;
            case END_TUNNEL_FOG:
                targetFog = &sEndTunnelFog;
                break;
            case END_FINAL_FOG:
                targetFog = &sEndFinalFog;
                break;
            case CAMP_FOG:
                targetFog = &sCampFog;
                break;
            default:
                targetFog = &sDefaultFog;
        }
    }

    gGlobalFog.r = (u8) MIN_MAX(approach_s16_asymptotic(gGlobalFog.r, targetFog->r, 5), 0, 255);
    gGlobalFog.g = (u8) MIN_MAX(approach_s16_asymptotic(gGlobalFog.g, targetFog->g, 5), 0, 255);
    gGlobalFog.b = (u8) MIN_MAX(approach_s16_asymptotic(gGlobalFog.b, targetFog->b, 5), 0, 255);
    gGlobalFog.a = (u8) MIN_MAX(approach_s16_asymptotic(gGlobalFog.a, targetFog->a, 5), 0, 255);
    gGlobalFog.low = MIN_MAX(approach_s16_asymptotic(gGlobalFog.low, targetFog->low, 8), 0, 4000);
    gGlobalFog.high = MIN_MAX(approach_s16_asymptotic(gGlobalFog.high, targetFog->high, 8), 0, 4000);
}

void print_speed_run_timer(void) {
    char timerText[40]; // why the hell do i need to make this huge?
    u16 timerMins = gSpeedRunTimer / (30 * 60);
    u16 timerSecs = (gSpeedRunTimer - (timerMins * 1800)) / 30;
    u16 timerFracSecs = ((gSpeedRunTimer - (timerMins * 1800) - (timerSecs * 30)) & 0xFFFF) / 3;
    sprintf(timerText, "%d:%02d.%d     ", timerMins, timerSecs, timerFracSecs);

    s2d_print_deferred(
        20,
        (int) (SCREEN_HEIGHT - 20),
        ALIGN_LEFT,
        180,
        0.5f,
        timerText
    );
}

void render_s2dex(void) {
    if (s2d_charBuffer_index || (gSpeedrunMode && gCurrLevelNum == LEVEL_CASTLE_GROUNDS)) {
        s2d_init();
        if (gSpeedrunMode && gCurrLevelNum == LEVEL_CASTLE_GROUNDS) print_speed_run_timer();
        if (s2d_charBuffer_index) s2d_handle_deferred();
        s2d_stop();
    }
}

void set_tip_type(u8 tipType) {
    if (tipType == TIP_TYPE_RING_REMINDER) {
        if (sTriggeredRingReminder) return;
        sTriggeredRingReminder = TRUE;
    }
    gCurTipType = tipType;
}

enum {
    TEMPLE2_ROOM1,
    TEMPLE2_ROOM2,
    TEMPLE2_ROOM3,
    TEMPLE2_ROOM4,
    TEMPLE_2_TRACKS
};

#define TEMPLE2_ROOM2_X -1993.0f

u8 handle_channel_volume(u8 channel, s8 isInRoom) {
    if (isInRoom) {
        sTimeInRoom[channel] = MIN(127, sTimeInRoom[channel] + 1);
        sTimeLeftRoom[channel] = sTimeInRoom[channel];
        return sTimeInRoom[channel];
    } else if (sTimeLeftRoom[channel] != 0) {
        sTimeLeftRoom[channel] = MAX(0, sTimeInRoom[channel] - 1);
        sTimeInRoom[channel] = sTimeLeftRoom[channel];
        return sTimeInRoom[channel];
    }

    return 0;
}

void handle_temple_2_channels(void) {
    if (gCurrLevelNum != LEVEL_CASTLE_GROUNDS || gCurrAreaIndex == 1 || gCurrAreaIndex == 3) return;

    for (u8 i = 0; i < TEMPLE_2_TRACKS; i++)
    {
        u8 targetVol = 0;
        switch (i) {
            case TEMPLE2_ROOM1:
                targetVol = handle_channel_volume(i, gCurrAreaIndex == 2 && gMarioState->pos[0] < TEMPLE2_ROOM2_X);
                break;
            case TEMPLE2_ROOM2:
                targetVol = handle_channel_volume(i, gCurrAreaIndex == 2 && gMarioState->pos[0] >= TEMPLE2_ROOM2_X);
                break;
            case TEMPLE2_ROOM3:
                targetVol = handle_channel_volume(i, gCurrAreaIndex == 4);
                break;
            case TEMPLE2_ROOM4:
                targetVol = handle_channel_volume(i, gCurrAreaIndex == 5);
        }
        fade_channel_volume_scale(SEQ_PLAYER_LEVEL, i * 2, targetVol, (u16)30);
        fade_channel_volume_scale(SEQ_PLAYER_LEVEL, (i * 2) + 1, targetVol, (u16)30);
    }
}

void render_game(void) {
    if (gCurrentArea != NULL && !gWarpTransition.pauseRendering) {
        handle_temple_2_channels();
        if (!gWaitingToStart && !(gContinuing && gCurCutscene == CUTSCENE_INTRO)) gCurCutsceneTimer++;
        if (gGameStarted) update_fog();
        if (
            (gWaitingToStart && gStartWaitTimer > 55) ||
            (!gWaitingToStart && gCurCutscene == CUTSCENE_INTRO && gCurCutsceneTimer < 100)
        ) title_screen_waiting_to_start();

        geo_process_root(gCurrentArea->unk04, D_8032CE74, D_8032CE78, gFBSetColor);

        gSPViewport(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(&D_8032CF00));

        gDPSetScissor(gDisplayListHead++, G_SC_NON_INTERLACE, 0, BORDER_HEIGHT, SCREEN_WIDTH,
                      SCREEN_HEIGHT - BORDER_HEIGHT);
        render_hud();

        gDPSetScissor(gDisplayListHead++, G_SC_NON_INTERLACE, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
        // print_text_fmt_int(10, 10, "%d", sTimesRenderedText);
        render_text_labels();
        do_cutscene_handler();
        print_displaying_credits_entry();

        gDPSetScissor(gDisplayListHead++, G_SC_NON_INTERLACE, 0, BORDER_HEIGHT, SCREEN_WIDTH,
                      SCREEN_HEIGHT - BORDER_HEIGHT);
        gPauseScreenMode = render_menus_and_dialogs();

        if (gPauseScreenMode != 0) {
            gSaveOptSelectIndex = gPauseScreenMode;
        }

        if (D_8032CE78 != NULL) {
            make_viewport_clip_rect(D_8032CE78);
        } else
            gDPSetScissor(gDisplayListHead++, G_SC_NON_INTERLACE, 0, BORDER_HEIGHT, SCREEN_WIDTH,
                          SCREEN_HEIGHT - BORDER_HEIGHT);

        if (gWarpTransition.isActive) {
            if (gWarpTransDelay == 0) {
                gWarpTransition.isActive = !render_screen_transition(0, gWarpTransition.type, gWarpTransition.time,
                                                                     &gWarpTransition.data);
                if (!gWarpTransition.isActive) {
                    if (gWarpTransition.type & 1) {
                        gWarpTransition.pauseRendering = TRUE;
                    } else {
                        set_warp_transition_rgb(0, 0, 0);
                    }
                }
            } else {
                gWarpTransDelay--;
            }
        }

        // if (gControllers[0].buttonDown & D_JPAD) {
        //     render_s2dex();
        // }
        render_goals();
        render_intro_text();
        if (gCurTipType) {
            s8 isDone = render_tip(gCurTipType);
            if (isDone) set_tip_type(0);
        }
        calculate_and_update_fps();
#ifdef CDEBUG
        if (gPlayer1Controller->buttonPressed & L_TRIG) sShowFPS = !sShowFPS;
        if (sShowFPS && gIsConsole) {
            print_fps(20, 40);
        }
#endif

        render_s2dex();
    } else {
        render_text_labels();
        if (D_8032CE78 != NULL) {
            clear_viewport(D_8032CE78, gWarpTransFBSetColor);
            // render_intro_text();
        } else {
            clear_frame_buffer(gWarpTransFBSetColor);
        }
    }

    D_8032CE74 = NULL;
    D_8032CE78 = NULL;
}
