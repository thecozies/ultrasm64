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
#include "puppycam2.h"

// - **Air:**
//     - Groundpound jump. Press A when your ground pound hits the ground.
//     - Air dive. Any direction you hold your controller stick, hold Z and press B while in the air to dive that direction. This is a very important mechanic for crossing large gaps in Lucy's Levitation.
//     - Air jump. A certain object will allow you to jump again in the air with A.
// - **Water:**
//     - Move any lateral direction using only the controller stick.
//     - Hold A to ascend, and B to decend
//     - Water groundpound. Like a normal groundpund, but in water.
//     - Water jump. After reaching the surface, press A to jump.
//     - Similar movement as Super Mario Sunshine.
// - **Camera**
//     - C left/right rotates the camera 45 degress.
//     - C up/down to move between three different zoom positions.
//     - D-pad acts as an analog camera for fine adjustments or looking around.
//     - Press R to toggle between close and far, and center focus.
//     - Press and release L to center the camera behind Lucy.
//     - Hold L and press R to snap the camera to the closest 45 degree angle.
//     - Camera will automatically snap perpendicular when walljumping if close enough to the perpendicular angle


#define COLOR_WHITE     COLOR "255 255 255 255"
#define COLOR_GREY      COLOR "195 195 195 255"
#define COLOR_BLUE      COLOR "106 223 255 255" // #6ADFFF
#define COLOR_BLUE_2    COLOR "89 136 255 255" // #5988FF
#define COLOR_YELLOW    COLOR "214 216 86 255"  // #D6D856
#define COLOR_GREEN     COLOR "93 228 100 255"  // #5DE464
#define COLOR_RED       COLOR "216 90 58 255"  // #D85A3A

#define TITLE_AIR       COLOR_BLUE   "In Air"   COLOR_WHITE
#define TITLE_WATER     COLOR_BLUE   "In Water" COLOR_WHITE
#define TITLE_CAMERA    COLOR_BLUE   "Camera"   COLOR_WHITE
#define TITLE_EXTRA     COLOR_YELLOW "Note"     COLOR_WHITE
// #define TITLE_AIR       SCALE "2" COLOR_BLUE "Air:\n"       COLOR_WHITE SCALE "1"
// #define TITLE_WATER     SCALE "2" COLOR_BLUE "Water:\n"     COLOR_WHITE SCALE "1"
// #define TITLE_CAMERA    SCALE "2" COLOR_BLUE "CAMERA:\n"    COLOR_WHITE SCALE "1"

#define TITLE_A_BUTTON          COLOR_BLUE_2 "A"            COLOR_WHITE
#define TITLE_B_BUTTON          COLOR_GREEN  "B"            COLOR_WHITE
#define TITLE_C_BUTTON_LR       COLOR_YELLOW "C left/right" COLOR_WHITE
#define TITLE_C_BUTTON_UD       COLOR_YELLOW "C up/down"    COLOR_WHITE
#define TITLE_Z_BUTTON          COLOR_GREY   "Z"            COLOR_WHITE
#define TITLE_L_BUTTON          COLOR_GREY   "L"            COLOR_WHITE
#define TITLE_R_BUTTON          COLOR_GREY   "R"            COLOR_WHITE
#define TITLE_DPAD              COLOR_GREY   "D-Pad"        COLOR_WHITE
#define TITLE_STICK             COLOR_GREY   "controller stick" COLOR_WHITE
#define TITLE_PAUSE             COLOR_RED    "start"        COLOR_WHITE

#define TUTORIAL_FADE_IN_LEN 30
#define TUTORIAL_SHOW_LEN 200
#define TUTORIAL_FADE_OUT_LEN 20

#define AIR_TIPS 3
#define WATER_TIPS 4
#define CAMERA_TIPS 6
#define EXTRA_TIPS 1
s32 sNumTips[4] = { AIR_TIPS, WATER_TIPS, CAMERA_TIPS, EXTRA_TIPS };

char sTitleAir[] = TITLE_AIR;
char sTitleWater[] = TITLE_WATER;
char sTitleCamera[] = TITLE_CAMERA;
char sTitleExtra[] = TITLE_EXTRA;

char sAirText[AIR_TIPS][198] = {
    "Groundpound jump.\n"
    "Press " TITLE_A_BUTTON " when your ground pound\n"
    "hits the ground.",

    "Air dive. Any direction you hold your\n"
    "controller stick, hold " TITLE_Z_BUTTON " and press " TITLE_B_BUTTON "\n"
    "while in the air to dive that direction.",

    "Air jump. A certain object will allow you to\n"
    "jump again in the air with " TITLE_A_BUTTON "."
};

char sRingJumpText2[] =
    "Passing through a ring will\n"
    "give you an extra jump.";

char sWaterText[WATER_TIPS][198] = {
    "Move any lateral direction using only the \n"
    TITLE_STICK ".",

    "Hold " TITLE_A_BUTTON " to ascend,\n"
    "and " TITLE_B_BUTTON " to decend.",

    "Water groundpound. Press " TITLE_Z_BUTTON "\n"
    "like a normal groundpound, but in water.",

    "Water jump. After reaching the surface,\n"
    "press " TITLE_A_BUTTON " to jump."
};

char sCameraText[CAMERA_TIPS][198] = {
    "" TITLE_C_BUTTON_LR " rotates the camera 45 degress.",

    "" TITLE_C_BUTTON_UD " to move between three different\n"
    "zoom positions.",

    "" TITLE_DPAD " acts as an analog camera.",

    "Press " TITLE_R_BUTTON " to toggle between close and far,\n"
    "and center focus.",

    "Press and release " TITLE_L_BUTTON " to center the camera.",

    "Hold " TITLE_L_BUTTON " and press " TITLE_R_BUTTON " to snap\n"
    "the camera to the closest 45 degree angle."
};

char sExtraText[EXTRA_TIPS][198] = {
    "Press " TITLE_PAUSE " to see these tips again." 
};

char sSkipText[] =  TITLE_A_BUTTON ": Next tip";
char sSkipTextPause[] =  TITLE_PAUSE ": End tutorial";
char sPauseText[] =
    COLOR_GREY "Up: " COLOR_WHITE "Prev tip\n"
    COLOR_GREY "Down: " COLOR_WHITE "Next tip";
char sPuppyOptionText1[] = COLOR_GREY "R: " COLOR_WHITE "Options";
char sPuppyOptionText2[] = COLOR_GREY "R: " COLOR_WHITE "Return";
char sPuppyOptionTitle[] = COLOR_BLUE "Puppycam Options";
char sAspect169[] = COLOR_GREY "L:" COLOR_WHITE " 16:9";
char sAspect43[] = COLOR_GREY "L:" COLOR_WHITE " 4:3";
u8 sSkipTimer = 0;

enum TUTORIAL_FADE_STATES {
    NO_TUTORIAL,
    STARTING_SHOW_TUTORIAL,
    FULL_SHOW_TUTORIAL,
    FADING_SHOW_TUTORIAL
};

enum TUTORIALS {
    TUTORIAL_AIR,
    TUTORIAL_WATER,
    TUTORIAL_CAMERA,
    TUTORIAL_EXTRA
};

s16 sTutorialTitleFadeState = STARTING_SHOW_TUTORIAL;
s16 sTutorialFadeState = STARTING_SHOW_TUTORIAL;
u8 sCurrentTutorial = TUTORIAL_AIR;
s32 sTutorialTitleFadeTimer = 0;
s32 sTutorialFadeTimer = 0;
s32 gTutorialDone = FALSE;
u8 sCurTip = 0;
s32 sTutorialSkipQueued = FALSE;
u8 sTutorialFadeAlpha = 0;
s32 sSkipToLast = FALSE;
// s32 sAskedForTutorial = FALSE;
// s32 sFadeAskTutorial = FALSE;

void reset_tutorial(void) {
    sTutorialTitleFadeState = STARTING_SHOW_TUTORIAL;
    sTutorialFadeState = STARTING_SHOW_TUTORIAL;
    sCurrentTutorial = TUTORIAL_AIR;
    sTutorialTitleFadeTimer = 0;
    sTutorialFadeTimer = 0;
    sCurTip = 0;
    sTutorialSkipQueued = FALSE;
    sTutorialFadeAlpha = 0;
    sSkipTimer = 0;
}

void set_next_tutorial_state(s32 state, s32 nextAlpha, s16 *pState, s32 *timer, s32 incTip) {
    *pState = state;
    *timer = 0;
    sTutorialFadeAlpha = nextAlpha;
    if (state == NO_TUTORIAL) {
        if (!incTip) sCurTip++;
        *pState = STARTING_SHOW_TUTORIAL;
    }
}

void next_tip(void) {
    if (++sCurTip >= sNumTips[sCurrentTutorial]) {
        sCurrentTutorial++;
        sCurTip = 0;
        if (sCurrentTutorial > TUTORIAL_CAMERA) sCurrentTutorial = TUTORIAL_AIR;
    }
}

void prev_tip(void) {
    if (sCurTip == 0) {
        if (sCurrentTutorial == TUTORIAL_AIR) sCurrentTutorial = TUTORIAL_CAMERA;
        else sCurrentTutorial--;
        sCurTip = sNumTips[sCurrentTutorial] - 1;
    }
    else sCurTip--;
}

void intro_advance_tip(void) {
    if (!sTutorialSkipQueued) {
        sTutorialSkipQueued = TRUE;
        if (sTutorialTitleFadeState != FADING_SHOW_TUTORIAL) {
            sTutorialTitleFadeState = FADING_SHOW_TUTORIAL;
            sTutorialTitleFadeTimer = 5;
        }
        if (sTutorialFadeState != FADING_SHOW_TUTORIAL) {
            sTutorialFadeState = FADING_SHOW_TUTORIAL;
            sTutorialFadeTimer = 5;
        }
    }
}

s32 update_tutorial_fade(s32 attack, s32 sustain, s32 release, s16 *state, s32 *timer, s32 incTip) {
    if (*state == NO_TUTORIAL) return FALSE;
    
    if (*state == STARTING_SHOW_TUTORIAL)
    {
        if (*timer >= attack) set_next_tutorial_state(FULL_SHOW_TUTORIAL, 255, state, timer, incTip);
        sTutorialFadeAlpha = MIN(sTutorialFadeAlpha + (255 / attack), 255);
    }
    else if (*state == FULL_SHOW_TUTORIAL && *timer >= sustain)
    {
        set_next_tutorial_state(FADING_SHOW_TUTORIAL, 255, state, timer, incTip);
    }
    else if (*state == FADING_SHOW_TUTORIAL)
    {
        if (*timer > release) {
            set_next_tutorial_state(NO_TUTORIAL, 0, state, timer, incTip);
            sTutorialSkipQueued = FALSE;
        }
        sTutorialFadeAlpha = MAX(sTutorialFadeAlpha - (255 / release), 0);
    }

    *timer = *timer + 1;
    return TRUE;
}

void render_tutorial_title(void) {
    gS2DScale = 1.0f;
    drop_shadow = TRUE;
    s2d_alpha = sTutorialFadeAlpha;
    switch(sCurrentTutorial) {
        case TUTORIAL_AIR:
            s2d_print_alloc(24, 20, ALIGN_LEFT, sTitleAir);
            break;
        case TUTORIAL_WATER:
            s2d_print_alloc(24, 20, ALIGN_LEFT, sTitleWater);
            break;
        case TUTORIAL_CAMERA:
            s2d_print_alloc(24, 20, ALIGN_LEFT, sTitleCamera);
            break;
        case TUTORIAL_EXTRA:
            s2d_print_alloc(24, 20, ALIGN_LEFT, sTitleExtra);
            break;
    }
}

void render_tutorial_text(void) {
    gS2DScale = 0.5f;
    drop_shadow = FALSE;
    s2d_alpha = sTutorialFadeAlpha;
    switch(sCurrentTutorial) {
        case TUTORIAL_AIR:
            s2d_print_alloc(20, 50, ALIGN_LEFT, sAirText[sCurTip]);
            break;
        case TUTORIAL_WATER:
            s2d_print_alloc(20, 50, ALIGN_LEFT, sWaterText[sCurTip]);
            break;
        case TUTORIAL_CAMERA:
            s2d_print_alloc(20, 50, ALIGN_LEFT, sCameraText[sCurTip]);
            break;
        case TUTORIAL_EXTRA:
            s2d_print_alloc(20, 50, ALIGN_LEFT, sExtraText[sCurTip]);
            break;
    }
}

void render_skip_text(void) {
    gS2DScale = 0.5f;
    drop_shadow = FALSE;


    if (
        sSkipToLast ||
        (
            sCurrentTutorial == TUTORIAL_CAMERA &&
            sCurTip >= CAMERA_TIPS - 1 &&
            sTutorialFadeState == FADING_SHOW_TUTORIAL
        )
    ) sSkipTimer = sTutorialFadeAlpha;
    else sSkipTimer = MIN(255, sSkipTimer + 3);

    s2d_alpha = sSkipTimer;

    s2d_print_alloc(
        GFX_DIMENSIONS_FROM_RIGHT_EDGE(35),
        SCREEN_HEIGHT - 35,
        ALIGN_RIGHT,
        sSkipText
    );

    gS2DScale = 0.5f;
    s2d_alpha = (int)((f32)s2d_alpha * 0.6f);
    s2d_print_alloc(
        GFX_DIMENSIONS_FROM_RIGHT_EDGE(10),
        SCREEN_HEIGHT - 20,
        ALIGN_RIGHT,
        sSkipTextPause
    );
}

void render_pause_hint_text(void) {
    gS2DScale = 0.5f;
    drop_shadow = FALSE;

    sSkipTimer = MIN(255, sSkipTimer + 10);

    s2d_alpha = sSkipTimer;

    if (!gPCOptionOpen) {
        s2d_print_alloc(
            GFX_DIMENSIONS_FROM_RIGHT_EDGE(10),
            SCREEN_HEIGHT - 36,
            ALIGN_RIGHT,
            sPauseText
        );
    }

    gS2DScale = 0.5f;
    s2d_print_alloc(
        GFX_DIMENSIONS_FROM_RIGHT_EDGE(70),
        10,
        ALIGN_LEFT,
        gPCOptionOpen ? sPuppyOptionText2 : sPuppyOptionText1
    );

    gS2DScale = 0.5f;
    s2d_print_alloc(
        GFX_DIMENSIONS_FROM_RIGHT_EDGE(70),
        24,
        ALIGN_LEFT,
        gWidescreen ? sAspect169 : sAspect43
    );
    if (gPlayer1Controller->buttonPressed & L_TRIG) gWidescreen = !gWidescreen;

    if (gPCOptionOpen) {
        gS2DScale = 1.0f;
        s2d_print_alloc(
            (SCREEN_WIDTH / 2) + 8,
            55,
            ALIGN_CENTER,
            sPuppyOptionTitle
        );
    }
}

// CTODO: Add pause to skip for intro tutorial
s32 render_tutorial(s32 onPause) {
    static u8 askTutorialTimer = 0;
    if (!onPause) {
        if (gTutorialDone) return FALSE;

        if (sCurrentTutorial != TUTORIAL_EXTRA) {
            if (askTutorialTimer > 20 && gPlayer1Controller->buttonPressed & START_BUTTON) {
                sSkipToLast = TRUE;
                intro_advance_tip();
            }

            if (askTutorialTimer > 20 && gPlayer1Controller->buttonPressed & A_BUTTON) intro_advance_tip();
            askTutorialTimer++;
        } else if (sTutorialFadeState == FULL_SHOW_TUTORIAL && gPlayer1Controller->buttonPressed & START_BUTTON) {
            set_next_tutorial_state(FADING_SHOW_TUTORIAL, 255, &sTutorialFadeState, &sTutorialFadeTimer, FALSE);
            set_next_tutorial_state(FADING_SHOW_TUTORIAL, 255, &sTutorialTitleFadeState, &sTutorialTitleFadeTimer, FALSE);
        }

        if (sSkipToLast && sTutorialFadeTimer == 1) {
            sCurTip = CAMERA_TIPS + 1;
            sCurrentTutorial = TUTORIAL_CAMERA;
            sSkipToLast = FALSE;
        }

        if (sCurTip >= sNumTips[sCurrentTutorial]) {
            sCurrentTutorial++;
            sCurTip = 0;
        }


        if (sCurrentTutorial > TUTORIAL_EXTRA) {
            gTutorialDone = TRUE;
            set_current_fog_state(0);
            gMarioState->paralyzed = FALSE;
            return FALSE;
        } else if (sCurrentTutorial == TUTORIAL_EXTRA) {
            gMarioState->paralyzed = FALSE;
        } else {
            gMarioState->paralyzed = TRUE;
        }
        set_current_fog_state(5);
    }

    s2d_init();

    if (update_tutorial_fade(
        TUTORIAL_FADE_IN_LEN,
        onPause ? 0x8000 : TUTORIAL_SHOW_LEN,
        TUTORIAL_FADE_OUT_LEN,
        &sTutorialFadeState,
        &sTutorialFadeTimer,
        FALSE
    )) render_tutorial_text();

    if (update_tutorial_fade(
        TUTORIAL_FADE_IN_LEN,
        onPause ? 0x8000 : TUTORIAL_SHOW_LEN,
        TUTORIAL_FADE_OUT_LEN,
        &sTutorialTitleFadeState,
        &sTutorialTitleFadeTimer,
        TRUE
    )) render_tutorial_title();

    if (!onPause) {
        if (sCurrentTutorial != TUTORIAL_EXTRA) render_skip_text();
    }
    else render_pause_hint_text();

    s2d_stop();

    return TRUE;
}

void render_ring_tip(void) {
    if (gCurCutsceneTimer == 0) {
        reset_tutorial();
    }

    if (update_tutorial_fade(
        TUTORIAL_FADE_IN_LEN,
        TUTORIAL_SHOW_LEN,
        TUTORIAL_FADE_OUT_LEN,
        &sTutorialFadeState,
        &sTutorialFadeTimer,
        FALSE
    ) && sTutorialFadeAlpha >= 5) {
        s2d_init();
        gS2DScale = 0.5f;
        drop_shadow = FALSE;
        s2d_alpha = sTutorialFadeAlpha;
        s2d_print_alloc(20, 50, ALIGN_LEFT, sRingJumpText2);
        s2d_stop();
    } else {
        set_current_cutscene(NO_CUTSCENE);
        reset_tutorial();
    }
}