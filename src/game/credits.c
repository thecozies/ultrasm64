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
#include "tutorial.h"

#define CREDITS_FADE_IN_LEN 30
#define CREDITS_SHOW_LEN 200
#define CREDITS_FADE_OUT_LEN 20

#define AIR_THANKS 3
#define WATER_THANKS 4
#define CAMERA_THANKS 6
#define EXTRA_THANKS 1
s32 sNumThanks[4] = { AIR_THANKS, WATER_THANKS, CAMERA_THANKS, EXTRA_THANKS };

char sLucys[] = "Lucy's";
char sLevitation[] = "Levitation";

char sTitleGame[] = "Modeling | Music | Programming";
char sGameText[] = "C | The Cozies";

char sTitleTextures[] = "Textures";
char sTexturesText[] = "L | The Cozies";


char sTitleMusic[] = "Music";
char sMusicText[][22] = {
    "The Cozies",
    "mommi | On didgeridoo"
};

char sTitleCodingHelp[] = "Coding Help";
char sCodingHelpText[][16] = {
    "Wiseguy",
    "Fazana",
    "someone2639",
    "axollyon",
    "Kaze",
    "Reonu",
    "anonymous_moose",
    "CrashOveride",
    "devwizard",
    "InTheBeef",
    "gamemasterplc",
    "Arthurtilly",
};

char sTitleTools[] = "Tools | Engines";
char sToolsText[][34] = {
    "Fazana | Puppycam",
    "someone2639 | S2D Text Engine",
    "kurethedead | Fast64",
    "CrashOveride | ultrasm64",
    "Arthurtilly | sm64-audio-manager",
    "Buu342 | UNFLoader"
};

char sTitleTesters[] = "Testing";
char sTestersText[][20] = {
    "rocky | fordescort",
    "Reonu",
    "The Sour OG"
};

char sTitleSpecial[] = "Special thanks to...";
char sMessageSimple[] = "Simpleflips - for expanding the SM64 hacking community.";
char sMessageSimple2[] = "From players to modders, we couldn't have gone this far without you.";

char sMessageJams[] = "Axollyon and Wiseguy";
char sMessageJams2[] = "For organizing and creating Mario Jams.";


enum CREDIT_STATES {
    CREDITS_GAME_TITLE,
    CREDITS_GAME,
    CREDITS_TEXTURES,
    CREDITS_MUSIC,
    CREDITS_CODING,
    CREDITS_TOOLS,
    CREDITS_TESTING,
    CREDITS_SPECIAL,
    CREDITS_END,
};

enum FADE_STATES {
    FADING_IN,
    HOLD_FADE,
    FADING_OUT,
    FADE_DONE,
};

struct CreditState
{
    u8 section;
    u16 timer;
    u8 fadeState;
    u8 fadeAlpha;
};

struct CreditState sCreditTitleState = { 0, 0, FADING_IN, 0 };
struct CreditState sCreditMessageState = { 0, 0, FADING_IN, 0 };

void update_credit_state(struct CreditState *creditState, u8 newState) {
    creditState->fadeState = newState;
    creditState->timer = 0;
    switch(creditState->fadeState) {
        case HOLD_FADE:
        case FADING_OUT:
            creditState->fadeAlpha = 255;
            break;
        case FADING_IN:
        case FADE_DONE:
        default:
            creditState->fadeAlpha = 0;
            break;
    }
}

// returns TRUE if done fading out
s32 handle_fade(f32 attack, s32 sustain, f32 release, struct CreditState *creditState) {
    int iAttack = attack;
    int iRelease = release;

    switch (creditState->fadeState) {
        case FADING_IN:
            if (creditState->timer >= iAttack) update_credit_state(creditState, HOLD_FADE);
            else creditState->fadeAlpha = (u8)MIN((s16)(creditState->fadeAlpha + (255.0f / attack)), 255);
            break;
        case HOLD_FADE:
            // titles need to decide when to fade out manually
            if (sustain && creditState->timer >= sustain) update_credit_state(creditState, FADING_OUT);
            break;
        case FADING_OUT:
            if (creditState->timer >= iRelease) update_credit_state(creditState, FADE_DONE);
            else creditState->fadeAlpha = (u8)MAX(((s16)creditState->fadeAlpha - (255.0f / release)), 0);
            break;
        case FADE_DONE:
        default:
            creditState->fadeAlpha = 0;
            return TRUE;
    }

    creditState->timer++;

    return FALSE;
}

#define C_ATTACK 30.0f
#define C_SUSTAIN 150
#define C_SUSTAIN_LONG 200
#define C_SUSTAIN_SHORT 100
#define C_SUSTAIN_VERY_SHORT 60
#define C_RELEASE 20.0f

#define CENTER_W ((SCREEN_WIDTH / 2))
#define TITLE_OFFSET 8
#define TITLE_H 50
#define TITLE_MID_H ((SCREEN_HEIGHT / 2) - 30)
#define TITLE_MID_L ((SCREEN_HEIGHT / 2) + 30)

#define MESSAGE_OFFSET 4
#define MESSAGE_H 110
#define MESSAGE_H_2 130

#define TITLE_SIZE 1.0f
#define MESSAGE_SIZE 0.5f

#define print_title(s)        (s2d_print_deferred((CENTER_W + TITLE_OFFSET), TITLE_H,     ALIGN_CENTER, sCreditTitleState.fadeAlpha, TITLE_SIZE, s))
#define print_title_low(s)    (s2d_print_deferred((CENTER_W + TITLE_OFFSET), MESSAGE_H,   ALIGN_CENTER, sCreditTitleState.fadeAlpha, TITLE_SIZE, s))
#define print_title_midH(s)   (s2d_print_deferred((CENTER_W + TITLE_OFFSET), TITLE_MID_H, ALIGN_CENTER, sCreditTitleState.fadeAlpha, TITLE_SIZE, s))
#define print_title_midL(s)   (s2d_print_deferred((CENTER_W + TITLE_OFFSET), TITLE_MID_L, ALIGN_CENTER, sCreditTitleState.fadeAlpha, TITLE_SIZE, s))

#define print_message(s)      (s2d_print_deferred((CENTER_W + MESSAGE_OFFSET), MESSAGE_H,    ALIGN_CENTER, sCreditMessageState.fadeAlpha, MESSAGE_SIZE, s))
#define print_message_low(s)  (s2d_print_deferred((CENTER_W + MESSAGE_OFFSET), MESSAGE_H_2,  ALIGN_CENTER, sCreditMessageState.fadeAlpha, MESSAGE_SIZE, s))
#define print_message_hold(s) (s2d_print_deferred((CENTER_W + MESSAGE_OFFSET), MESSAGE_H,    ALIGN_CENTER, 255,                           MESSAGE_SIZE, s))

void reset_credit_state(struct CreditState *creditState) {
    creditState->fadeState = FADING_IN;
    creditState->fadeAlpha = 0;
    creditState->timer = 0;
}

void next_title(void) {
    sCreditTitleState.section++;
    sCreditMessageState.section = 0;

    reset_credit_state(&sCreditTitleState);
    reset_credit_state(&sCreditMessageState);

    if (sCreditTitleState.section == CREDITS_END) {
        gShowingCredits = FALSE;
        sCreditTitleState.section = 0;
    }
}

void print_titles(void) {
    if (sCreditTitleState.section == CREDITS_END) return;

    s32 done = handle_fade(C_ATTACK, sCreditTitleState.section == CREDITS_GAME_TITLE ? C_SUSTAIN_LONG : 0, C_RELEASE, &sCreditTitleState);
    if (done) next_title();

    switch(sCreditTitleState.section) {
        case CREDITS_GAME_TITLE:
            print_title_midH(sLucys);
            print_title_midL(sLevitation);
            break;
        case CREDITS_GAME:
            print_title(sTitleGame);
            break;
        case CREDITS_TEXTURES:
            print_title(sTitleTextures);
            break;
        case CREDITS_MUSIC:
            print_title(sTitleMusic);
            break;
        case CREDITS_CODING:
            print_title(sTitleCodingHelp);
            break;
        case CREDITS_TOOLS:
            print_title(sTitleTools);
            break;
        case CREDITS_TESTING:
            print_title(sTitleTesters);
            break;
        case CREDITS_SPECIAL:
            print_title(sTitleSpecial);
            break;
        default:
            return;
    }
}

void print_messages(void) {
    if (sCreditTitleState.section == CREDITS_END) return;
    static s32 holdForFollowup = FALSE;

    s32 sustain = C_SUSTAIN;
    s32 lastMessage = 0;
    s32 done;

    switch(sCreditTitleState.section) {
        case CREDITS_GAME_TITLE:
            return;
        case CREDITS_GAME:
            print_message(sGameText);
            break;
        case CREDITS_TEXTURES:
            print_message(sTexturesText);
            break;
        case CREDITS_MUSIC:
            print_message(sMusicText[sCreditMessageState.section]);
            lastMessage = 1;
            sustain = C_SUSTAIN_SHORT;
            break;
        case CREDITS_CODING:
            print_message(sCodingHelpText[sCreditMessageState.section]);
            // lastMessage = LAST_INDEX(sCodingHelpText);
            lastMessage = 11;
            sustain = C_SUSTAIN_VERY_SHORT;
            break;
        case CREDITS_TOOLS:
            print_message(sToolsText[sCreditMessageState.section]);
            // lastMessage = LAST_INDEX(sToolsText);
            lastMessage = 5;
            sustain = C_SUSTAIN_SHORT;
            break;
        case CREDITS_TESTING:
            print_message(sTestersText[sCreditMessageState.section]);
            // lastMessage = LAST_INDEX(sTestersText);
            lastMessage = 2;
            sustain = C_SUSTAIN_SHORT;
            break;
        case CREDITS_SPECIAL:
            if (sCreditMessageState.section == 0) {
                if (holdForFollowup) {
                    if (sCreditMessageState.fadeState >= FADING_OUT) print_message(sMessageSimple);
                    else print_message_hold(sMessageSimple);

                    print_message_low(sMessageSimple2);
                    if (handle_fade(C_ATTACK, C_SUSTAIN_LONG, C_RELEASE, &sCreditMessageState)) {
                        sCreditMessageState.section++;
                        holdForFollowup = FALSE;
                        reset_credit_state(&sCreditMessageState);
                    }
                } else {
                    print_message(sMessageSimple);
                    handle_fade(C_ATTACK, C_SUSTAIN, C_RELEASE, &sCreditMessageState);
                    if (sCreditMessageState.fadeState == HOLD_FADE) {
                        holdForFollowup = TRUE;
                        reset_credit_state(&sCreditMessageState);
                    }
                }
                return;
            } else {
                if (holdForFollowup) {
                    if (sCreditMessageState.fadeState >= FADING_OUT) print_message(sMessageJams);
                    else print_message_hold(sMessageJams);

                    print_message_low(sMessageJams2);
                    sustain = C_SUSTAIN_LONG;
                    lastMessage = 1;
                } else {
                    print_message(sMessageJams);
                    handle_fade(C_ATTACK, C_SUSTAIN, C_RELEASE, &sCreditMessageState);
                    if (sCreditMessageState.fadeState == HOLD_FADE) {
                        holdForFollowup = TRUE;
                        reset_credit_state(&sCreditMessageState);
                    }
                    return;
                }
            }
            break;
        default:
            return;
    }

    done = handle_fade(C_ATTACK, sustain, C_RELEASE, &sCreditMessageState);

    if (lastMessage == sCreditMessageState.section && sCreditMessageState.fadeState == FADING_OUT && sCreditTitleState.fadeState != FADING_OUT)
        update_credit_state(&sCreditTitleState, FADING_OUT);
    else if (done) {
        sCreditMessageState.section++;
        update_credit_state(&sCreditMessageState, FADING_IN);
    }
}

void render_final_credits(void) {
    print_messages();
    print_titles();
}
