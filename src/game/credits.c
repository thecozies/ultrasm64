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
char sGameText[][10] = {
    "thecozies"
};

char sTitleTextures[] = "Textures";
char sTexturesText[][20] = {
    "moobs | thecozies"
};


char sTitleMusic[] = "Music";
char sMusicText[][20] = {
    "mommy | didgeridoo"
};

char sTitleCodingHelp[] = "Coding Help";
char sCodingHelpText[][15] = {
    "Wiseguy",
    "Fazana",
    "someone2639",
    "Axollyon",
    "Kaze",
    "Reonu",
    "CrashOveride",
    "Arthurtilly",
    "gamemasterplc"
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
    "moobs | thecozies"
};


