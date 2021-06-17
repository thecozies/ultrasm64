#include <PR/ultratypes.h>
#include <PR/gbi.h>
#include "sm64.h"
#include "types.h"
#include "level_update.h"
#include "puppycam2.h"
#include "audio/external.h"
#include "audio/data.h"
#include "game_init.h"
#include "engine/math_util.h"
#include "print.h"
#include "engine/surface_collision.h"
#include "engine/surface_load.h"
#include "include/text_strings.h"
#include "segment2.h"
#include "ingame_menu.h"
#include "memory.h"
#include "object_list_processor.h"
#include "object_helpers.h"
#include "behavior_data.h"
#include "save_file.h"
#include "mario.h"
#include "puppyframes.h"
#include "rendering_graph_node.h"

#define OFFSET 30.0f
#define STEPS 4
#define DECELERATION 0.75f
#define DEADZONE 20
#define SCRIPT_MEMORY_POOL 0x1000

#define SNAP_HOLD_LENGTH 0x100

struct gPuppyStruct gPuppyCam;
struct sPuppyVolume *sPuppyVolumeStack[MAX_PUPPYCAM_VOLUMES];
s16 sFloorHeight = 0;
u8 gPCOptionOpen = 0;
s8 gPCOptionSelected = 0;
s32 gPCOptionTimer = 0;
u8 gPCOptionIndex = 0;
u8 gPCOptionScroll = 0;
u16 gPuppyVolumeCount = 0;
struct MemoryPool *gPuppyMemoryPool;
s32 gPuppyError = 0;
s16 gPrevPuppyTargetYaw = 0;
s16 gPrevPuppyZoomDist = 0;
s16 sZoomPitchTargets[3] = {14300, 12650, 11010};
s8 sLWasHeld = FALSE;
s8 sWaitForLUnheld = FALSE;
s32 sMovingFramesSinceSnap = SNAP_HOLD_LENGTH + 1;
static Vp sCutsceneVp = { { { 640, 480, 511, 0 }, { 640, 480, 511, 0 } } };
f32 sCameraWallOffset = OFFSET;
f32 sRayCastOffset = OFFSET;
u8 sSavedZoomSet = 4;

#if defined(VERSION_EU)
static u8 gPCOptionStringsFR[][64] = {{NC_ANALOGUE_FR}, {NC_CAMX_FR}, {NC_CAMY_FR}, {NC_INVERTX_FR}, {NC_INVERTY_FR}, {NC_CAMC_FR}, {NC_CAMP_FR}, {NC_CAMD_FR}};
static u8 gPCOptionStringsDE[][64] = {{NC_ANALOGUE_DE}, {NC_CAMX_DE}, {NC_CAMY_DE}, {NC_INVERTX_DE}, {NC_INVERTY_DE}, {NC_CAMC_DE}, {NC_CAMP_DE}, {NC_CAMD_DE}};
static u8 gPCFlagStringsFR[][64] = {{OPTION_DISABLED_FR}, {OPTION_ENABLED_FR}};
static u8 gPCFlagStringsDE[][64] = {{OPTION_DISABLED_DE}, {OPTION_ENABLED_DE}};
static u8 gPCToggleStringsFR[][64] = {{NC_ANALOGUE_EN}, {NC_ANALOGUE_EN}, {NC_ANALOGUE_EN}, {NC_ANALOGUE_EN}, {NC_ANALOGUE_EN}};
static u8 gPCToggleStringsDE[][64] = {{NC_ANALOGUE_EN}, {NC_ANALOGUE_EN}, {NC_ANALOGUE_EN}, {NC_ANALOGUE_EN}, {NC_ANALOGUE_EN}};
//static u8 gPCToggleStringsFR[][64] = {{NC_BUTTON_FR}, {NC_BUTTON2_FR}, {NC_OPTION_FR}, {NC_HIGHLIGHT_L_FR}, {NC_HIGHLIGHT_R_FR}};
//static u8 gPCToggleStringsDE[][64] = {{NC_BUTTON_DE}, {NC_BUTTON2_DE}, {NC_OPTION_DE}, {NC_HIGHLIGHT_L_DE}, {NC_HIGHLIGHT_R_DE}};
#endif
static u8 gPCOptionStringsEN[][64] = {{NC_ANALOGUE_EN}, {NC_CAMX_EN}, {NC_CAMY_EN}, {NC_INVERTX_EN}, {NC_INVERTY_EN}, {NC_CAMC_EN},  };
static u8 gPCFlagStringsEN[][64] = {{OPTION_DISABLED_EN}, {OPTION_ENABLED_EN}};
static u8 gPCToggleStringsEN[][64] = {{NC_BUTTON_EN}, {NC_BUTTON2_EN}, {NC_OPTION_EN}, {NC_HIGHLIGHT_L}, {NC_HIGHLIGHT_R}};


#define OPT 32 //Just a temp thing

static u8 (*gPCOptionStringsPtr)[OPT][64] = &gPCOptionStringsEN;
static u8 (*gPCFlagStringsPtr)[OPT][64] = &gPCFlagStringsEN;
static u8 (*gPCToggleStringsPtr)[OPT][64] = &gPCToggleStringsEN;


static const struct gPCOptionStruct
{
    u8 gPCOptionName; //This is the position in the newcam_options text array. It doesn't have to directly correlate with its position in the struct
    s16 *gPCOptionVar; //This is the value that the option is going to directly affect.
    u8 gPCOptionStart; //This is where the text array will start. Set it to 255 to have it be ignored.
    s32 gPCOptionMin; //The minimum value of the option.
    s32 gPCOptionMax; //The maximum value of the option.
};

static const struct gPCOptionStruct gPCOptions[]=
{ //If the min and max are 0 and 1, then the value text is used, otherwise it's ignored.
    {/*Option Name*/ 0, /*Option Variable*/ &gPuppyCam.options.analogue,       /*Option Value Text Start*/ 0, /*Option Minimum*/ FALSE, /*Option Maximum*/ TRUE},
    {/*Option Name*/ 1, /*Option Variable*/ &gPuppyCam.options.sensitivityX,   /*Option Value Text Start*/ 255, /*Option Minimum*/ 10, /*Option Maximum*/ 500},
    {/*Option Name*/ 2, /*Option Variable*/ &gPuppyCam.options.sensitivityY,   /*Option Value Text Start*/ 255, /*Option Minimum*/ 10, /*Option Maximum*/ 500},
    {/*Option Name*/ 3, /*Option Variable*/ &gPuppyCam.options.invertX,        /*Option Value Text Start*/ 0, /*Option Minimum*/ FALSE, /*Option Maximum*/ TRUE},
    {/*Option Name*/ 4, /*Option Variable*/ &gPuppyCam.options.invertY,        /*Option Value Text Start*/ 0, /*Option Minimum*/ FALSE, /*Option Maximum*/ TRUE},
    {/*Option Name*/ 5, /*Option Variable*/ &gPuppyCam.options.turnAggression, /*Option Value Text Start*/ 255, /*Option Minimum*/ 0, /*Option Maximum*/ 100},
};

u8 gPCOptionCap = sizeof(gPCOptions) / sizeof(struct gPCOptionStruct); //How many options there are in newcam_uptions.

//Some macros for the sake of basic human sanity.
#define CLAMP(x, low, high)  (((x) > (high)) ? (high) : (((x) < (low)) ? (low) : (x)))
#define ABS(x) ((x) > 0.f ? (x) : -(x))

s16 LENSIN(s16 length, s16 direction)
{
    return (length * sins(direction));
}
s16 LENCOS(s16 length, s16 direction)
{
    return (length * coss(direction));
}

static void puppycam_analogue_stick(void)
{
    #ifdef TARGET_N64
    if (!gPuppyCam.options.analogue)
        return;

    //I make the X axis negative, so that the movement reflects the Cbuttons.
    gPuppyCam.stick2[0] = -gPlayer2Controller->rawStickX;
    gPuppyCam.stick2[1] = gPlayer2Controller->rawStickY;

    if (ABS(gPuppyCam.stick2[0]) < DEADZONE)
    {
        gPuppyCam.stick2[0] = 0;
        gPuppyCam.stickN[0] = 0;
    }
    if (ABS(gPuppyCam.stick2[1]) < DEADZONE)
    {
        gPuppyCam.stick2[1] = 0;
        gPuppyCam.stickN[1] = 0;
    }
    #endif
}

void puppycam_default_config(void)
{
    gPuppyCam.options.invertX = 1;
    gPuppyCam.options.invertY = 1;
    gPuppyCam.options.sensitivityX = 100;
    gPuppyCam.options.sensitivityY = 100;
    gPuppyCam.options.turnAggression = 50;
    gPuppyCam.options.analogue = 0;
}

//Initial setup. Ran at the beginning of the game and never again.
void puppycam_boot(void)
{
    gPuppyCam.zoomPoints[0] = 500;
    gPuppyCam.zoomPoints[1] = 1000;
    gPuppyCam.zoomPoints[2] = 1500;
    gPuppyCam.povHeight = 125;
    gPuppyCam.stick2[0] = 0;
    gPuppyCam.stick2[1] = 0;
    gPuppyCam.stickN[0] = 0;
    gPuppyCam.stickN[1] = 0;
    gPuppyMemoryPool = mem_pool_init(MAX_PUPPYCAM_VOLUMES * sizeof(struct sPuppyVolume), MEMORY_POOL_LEFT);
    gPuppyVolumeCount = 0;
    gPuppyCam.enabled = 1;
    puppycam_default_config();

    // puppycam_get_save();
}

#if defined(VERSION_EU)
static void newcam_set_language(void)
{
    switch (eu_get_language())
    {
    case 0:
        gPCOptionStringsPtr = &gPCOptionStringsEN;
        gPCFlagStringsPtr = &gPCFlagStringsEN;
        gPCToggleStringsPtr = &gPCToggleStringsEN;
        break;
    case 1:
        gPCOptionStringsPtr = &gPCOptionStringsFR;
        gPCFlagStringsPtr = &gPCFlagStringsFR;
        gPCToggleStringsPtr = &gPCToggleStringsFR;
        break;
    case 2:
        gPCOptionStringsPtr = &gPCOptionStringsDE;
        gPCFlagStringsPtr = &gPCFlagStringsDE;
        gPCToggleStringsPtr = &gPCToggleStringsDE;
        break;
    }
}
#endif

///MENU

#define BLANK 0, 0, 0, ENVIRONMENT, 0, 0, 0, ENVIRONMENT

static void puppycam_display_box(s16 x1, s16 y1, s16 x2, s16 y2, u8 r, u8 g, u8 b, u8 a)
{
    gDPSetCombineMode(gDisplayListHead++, BLANK, BLANK);
    gDPSetCycleType(gDisplayListHead++, G_CYC_1CYCLE);
    if (a !=255)
    {
        gDPSetRenderMode(gDisplayListHead++, G_RM_XLU_SURF, G_RM_XLU_SURF2);
    }
    else
    {
        gDPSetRenderMode(gDisplayListHead++, G_RM_OPA_SURF, G_RM_OPA_SURF);
    }
    gDPSetEnvColor(gDisplayListHead++, r, g, b, a);
    gDPFillRectangle(gDisplayListHead++, x1, y1, x2, y2);
    gDPPipeSync(gDisplayListHead++);
    gDPSetEnvColor(gDisplayListHead++,255,255,255,255);
    gDPSetCycleType(gDisplayListHead++, G_CYC_1CYCLE);
    gSPDisplayList(gDisplayListHead++,dl_hud_img_end);
}

//I actually took the time to redo this, properly. Lmao. Please don't bully me over this anymore :(
void puppycam_change_setting(s8 toggle)
{
    if (gPlayer1Controller->buttonDown & A_BUTTON)
        toggle*= 5;
    if (gPlayer1Controller->buttonDown & B_BUTTON)
        toggle*= 10;

    if (gPCOptions[gPCOptionSelected].gPCOptionMin == FALSE && gPCOptions[gPCOptionSelected].gPCOptionMax == TRUE)
    {
        *gPCOptions[gPCOptionSelected].gPCOptionVar ^= 1;
    }
    else
        *gPCOptions[gPCOptionSelected].gPCOptionVar += toggle;
    //Forgive me father, for I have sinned. I guess if you wanted a selling point for a 21:9 monitor though, "I can view this line in puppycam's code without scrolling!" can be added to it.
    *gPCOptions[gPCOptionSelected].gPCOptionVar = CLAMP(*gPCOptions[gPCOptionSelected].gPCOptionVar, gPCOptions[gPCOptionSelected].gPCOptionMin, gPCOptions[gPCOptionSelected].gPCOptionMax);
}

void puppycam_print_text(s16 x, s16 y, u8 str[], u8 col)
{
    u8 textX;
    textX = get_str_x_pos_from_center(x,str,10.0f);
    gDPSetEnvColor(gDisplayListHead++, 0, 0, 0, 255);
    print_generic_string(textX+1,y-1,str);
    if (col != 0)
    {
        gDPSetEnvColor(gDisplayListHead++, 255, 255, 255, 255);
    }
    else
    {
        gDPSetEnvColor(gDisplayListHead++, 255, 32, 32, 255);
    }
    print_generic_string(textX,y,str);
}

//Options menu
void puppycam_display_options()
{
    u8 i = 0;
    u8 newstring[32];
    s16 scroll;
    s16 scrollpos;
    s16 var = gPCOptions;
    s16 vr;
    u16 maxvar;
    u16 minvar;
    f32 newcam_sinpos;

    // puppycam_display_box(47,83,281,84,0x0,0x0,0x0, 0xFF);
    // puppycam_display_box(47,218,281,219,0x0,0x0,0x0, 0xFF);
    // puppycam_display_box(47,83,48,219,0x0,0x0,0x0, 0xFF);
    // puppycam_display_box(280,83,281,219,0x0,0x0,0x0, 0xFF);
    // puppycam_display_box(271,83,272,219,0x0,0x0,0x0, 0xFF);

    puppycam_display_box(48, 84, 272, 218, 0x21, 0x21, 0x21, 0x80);
    // gSPDisplayList(gDisplayListHead++, dl_rgba16_text_begin);
    // gDPSetEnvColor(gDisplayListHead++, 255, 255, 255, 255);
    // print_hud_lut_string(HUD_LUT_GLOBAL, 64, 40, (*gPCToggleStringsPtr)[2]);
    // gSPDisplayList(gDisplayListHead++, dl_rgba16_text_end);

    if (gPCOptionCap>4)
    {
        puppycam_display_box(272,84,280,218,0x80,0x80,0x80, 0xFF);
        scrollpos = (62)*((f32)gPCOptionScroll/(gPCOptionCap-4));
        puppycam_display_box(272,84+scrollpos,280,156+scrollpos,0xFF,0xFF,0xFF, 0xFF);
    }

    gSPDisplayList(gDisplayListHead++, dl_ia_text_begin);
    gDPSetScissor(gDisplayListHead++, G_SC_NON_INTERLACE, 0, 80, SCREEN_WIDTH, SCREEN_HEIGHT);
    for (i = 0; i < gPCOptionCap; i++)
    {
        scroll = 140-(32*i)+(gPCOptionScroll*32);
        if (scroll <= 140 && scroll > 32)
        {
            puppycam_print_text(160,scroll,(*gPCOptionStringsPtr)[gPCOptions[i].gPCOptionName],gPCOptionSelected-i);
            if (gPCOptions[i].gPCOptionStart != 255)
            {
                var = *gPCOptions[i].gPCOptionVar+gPCOptions[i].gPCOptionStart;
                if (var < sizeof(gPCFlagStringsEN)) //Failsafe for if it somehow indexes an out of bounds array.
                    puppycam_print_text(160,scroll-12,(*gPCFlagStringsPtr)[var],gPCOptionSelected-i);
            }
            else
            {
                int_to_str(*gPCOptions[i].gPCOptionVar,newstring);
                puppycam_print_text(160,scroll-12,newstring,gPCOptionSelected-i);
                puppycam_display_box(96,111+(32*i)-(gPCOptionScroll*32),224,117+(32*i)-(gPCOptionScroll*32),0x80,0x80,0x80, 0xFF);
                maxvar = gPCOptions[i].gPCOptionMax - gPCOptions[i].gPCOptionMin;
                minvar = *gPCOptions[i].gPCOptionVar - gPCOptions[i].gPCOptionMin;
                puppycam_display_box(96,111+(32*i)-(gPCOptionScroll*32),96+(((f32)minvar/maxvar)*128),117+(32*i)-(gPCOptionScroll*32),0xFF,0xFF,0xFF, 0xFF);
                puppycam_display_box(94+(((f32)minvar/maxvar)*128),109+(32*i)-(gPCOptionScroll*32),98+(((f32)minvar/maxvar)*128),119+(32*i)-(gPCOptionScroll*32),0xFF,0x0,0x0, 0xFF);
                gSPDisplayList(gDisplayListHead++, dl_ia_text_begin);
            }
        }
    }
    newcam_sinpos = sins(gGlobalTimer*5000)*4;
    gDPSetScissor(gDisplayListHead++, G_SC_NON_INTERLACE, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    gDPSetEnvColor(gDisplayListHead++, 255, 255, 255, 255);
    print_generic_string(80-newcam_sinpos, 132-(32*(gPCOptionSelected-gPCOptionScroll)),  (*gPCToggleStringsPtr)[3]);
    print_generic_string(232+newcam_sinpos, 132-(32*(gPCOptionSelected-gPCOptionScroll)),  (*gPCToggleStringsPtr)[4]);
    gSPDisplayList(gDisplayListHead++, dl_ia_text_end);
}

//This has been separated for interesting reasons. Don't question it.
void puppycam_render_option_text(void)
{
    gSPDisplayList(gDisplayListHead++, dl_ia_text_begin);
    puppycam_print_text(278,212,(*gPCToggleStringsPtr)[gPCOptionOpen],1);
    gSPDisplayList(gDisplayListHead++, dl_ia_text_end);
}

extern struct SaveBuffer gSaveBuffer;

void puppycam_check_pause_buttons()
{
    if (gPlayer1Controller->buttonPressed & R_TRIG)
    {
        // play_sound(SOUND_MENU_CHANGE_SELECT, gGlobalSoundSource);
        if (gPCOptionOpen == 0)
        {
            gPCOptionOpen = 1;
            #if defined(VERSION_EU)
            newcam_set_language();
            #endif
        }

        else
        {
            gPCOptionOpen = 0;
            // puppycam_set_save();
        }
    }

    if (gPCOptionOpen)
    {
        if (ABS(gPlayer1Controller->rawStickY) > 60 || gPlayer1Controller->buttonDown & U_JPAD || gPlayer1Controller->buttonDown & D_JPAD)
        {
            gPCOptionTimer -= 1;
            if (gPCOptionTimer <= 0)
            {
                switch (gPCOptionIndex)
                {
                    case 0: gPCOptionIndex++; gPCOptionTimer += 10; break;
                    default: gPCOptionTimer += 5; break;
                }
                // play_sound(SOUND_MENU_CHANGE_SELECT, gGlobalSoundSource);
                if (gPlayer1Controller->rawStickY >= 60 || gPlayer1Controller->buttonDown & U_JPAD)
                {
                    gPCOptionSelected--;
                    if (gPCOptionSelected < 0)
                        gPCOptionSelected = gPCOptionCap-1;
                }
                else
                if (gPlayer1Controller->rawStickY <= -60 || gPlayer1Controller->buttonDown & D_JPAD)
                {
                    gPCOptionSelected++;
                    if (gPCOptionSelected >= gPCOptionCap)
                        gPCOptionSelected = 0;
                }
            }
        }
        else
        if (ABS(gPlayer1Controller->rawStickX) > 60 || gPlayer1Controller->buttonDown & L_JPAD || gPlayer1Controller->buttonDown & R_JPAD)
        {
            gPCOptionTimer -= 1;
            if (gPCOptionTimer <= 0)
            {
                switch (gPCOptionIndex)
                {
                    case 0: gPCOptionIndex++; gPCOptionTimer += 10; break;
                    default: gPCOptionTimer += 5; break;
                }
                // play_sound(SOUND_MENU_CHANGE_SELECT, gGlobalSoundSource);
                if (gPlayer1Controller->rawStickX >= 60 || gPlayer1Controller->buttonDown & R_JPAD)
                    puppycam_change_setting(1);
                else
                if (gPlayer1Controller->rawStickX <= -60 || gPlayer1Controller->buttonDown & L_JPAD)
                    puppycam_change_setting(-1);
            }
        }
        else
        {
            gPCOptionTimer = 0;
            gPCOptionIndex = 0;
        }

        while (gPCOptionScroll - gPCOptionSelected < -3 && gPCOptionSelected > gPCOptionScroll)
            gPCOptionScroll +=1;
        while (gPCOptionScroll + gPCOptionSelected > 0 && gPCOptionSelected < gPCOptionScroll)
            gPCOptionScroll -=1;
    }
}

///CORE

//Set up values. Runs on level load.
void puppycam_init(void)
{

    if (gMarioState->marioObj)
        gPuppyCam.targetObj = gMarioState->marioObj;
    gPuppyCam.targetObj2 = NULL;

    gPuppyCam.intendedFlags = PUPPYCAM_BEHAVIOUR_DEFAULT;

    if (gCurrLevelNum == 27 || (gCurrLevelNum == 36 && gCurrAreaIndex == 2) || (gCurrLevelNum == 5 && gCurrAreaIndex == 2))
        gPuppyCam.intendedFlags |= PUPPYCAM_BEHAVIOUR_SLIDE_CORRECTION;
    gPuppyCam.flags = gPuppyCam.intendedFlags;
    gPuppyCam.zoom = gPuppyCam.zoomPoints[1];
    gPuppyCam.zoomSet = 1;
    gPuppyCam.zoomTarget = gPuppyCam.zoom;
    gPuppyCam.yaw = gMarioState->faceAngle[1]+0x8000;
    gPuppyCam.yawTarget = gPuppyCam.yaw;
    gPuppyCam.pitch = 0x3000;
    gPuppyCam.pitchTarget = gPuppyCam.pitch;
    gPuppyCam.yawAcceleration = 0;
    gPuppyCam.pitchAcceleration = 0;
    gPuppyCam.shakeFrames = 0;
    gPuppyCam.shake[0] = 0;
    gPuppyCam.shake[1] = 0;
    gPuppyCam.shake[2] = 0;
    gPuppyCam.pos[0] = 0;
    gPuppyCam.pos[1] = 0;
    gPuppyCam.pos[2] = 0;
    gPuppyCam.focus[0] = 0;
    gPuppyCam.focus[1] = 0;
    gPuppyCam.focus[2] = 0;
    gPuppyCam.pan[0] = 0;
    gPuppyCam.pan[1] = 0;
    // gPuppyCam.pan[1] = gMarioState->pos[1];
    gPuppyCam.pan[2] = 0;
    gPuppyCam.posHeight[0] = 0;
    gPuppyCam.posHeight[1] = 0;
    gPuppyCam.targetFloorHeight = gPuppyCam.pan[1];
    gPuppyCam.lastTargetFloorHeight = gMarioState->pos[1];
    gPuppyCam.opacity = 255;
    gPuppyCam.swimPitch = 0;

}

//Handles C Button inputs for modes that have held inputs, rather than presses.
static void puppycam_input_hold(void)
{
    f32 ivX = ((gPuppyCam.options.invertX*2)-1)*(gPuppyCam.options.sensitivityX/100.f);
    s8 stickMag = 0;

    gPuppyCam.framesSinceC[0]++;
    gPuppyCam.framesSinceC[1]++;

    //Analogue Camera stuff. If it fails to find an input, then it just sets stickmag to 100, which after calculations means the value goes unchanged.
    if (gPuppyCam.options.analogue)
        stickMag = gPuppyCam.stick2[0]*1.25f;
    else
        stickMag = 100;

    //In theory this shouldn't be necessary, but it's nice to cover all bases.
    if (!(gPuppyCam.flags & PUPPYCAM_BEHAVIOUR_YAW_ROTATION))
        return;

    if (!gPuppyCam.options.analogue && gPlayer1Controller->buttonPressed & L_CBUTTONS)
    {
        gPuppyCam.yawTarget -= 0x2000 * ((gPuppyCam.options.invertX*2)-1);
        gPuppyCam.framesSinceC[0] = 0;
        play_sound(SOUND_MENU_CLICK_CHANGE_VIEW, gGlobalSoundSource);

        if (gPuppyCam.yawTarget % 0x2000 != 0) sMovingFramesSinceSnap = SNAP_HOLD_LENGTH + 1;
        else if (sMovingFramesSinceSnap <= SNAP_HOLD_LENGTH) sMovingFramesSinceSnap = 0;
    }
    else if (!gPuppyCam.options.analogue && gPlayer1Controller->buttonPressed & R_CBUTTONS)
    {
        gPuppyCam.yawTarget += 0x2000 * ((gPuppyCam.options.invertX*2)-1);
        gPuppyCam.framesSinceC[1] = 0;
        play_sound(SOUND_MENU_CLICK_CHANGE_VIEW, gGlobalSoundSource);

        if (gPuppyCam.yawTarget % 0x2000 != 0) sMovingFramesSinceSnap = SNAP_HOLD_LENGTH + 1;
        else if (sMovingFramesSinceSnap <= SNAP_HOLD_LENGTH) sMovingFramesSinceSnap = 0;
    }

    if (
        (
            !gPuppyCam.options.analogue
            // && (gPlayer1Controller->buttonDown & L_JPAD || gPlayer1Controller->buttonDown & L_CBUTTONS)
            && (gPlayer1Controller->buttonDown & L_JPAD)
            && gPuppyCam.framesSinceC[0] > 8
        )
        || gPuppyCam.stick2[0] != 0
    )
    {
        // if (gPlayer1Controller->buttonDown & L_CBUTTONS) gPuppyCam.yawAcceleration -= 2*(gPuppyCam.options.sensitivityX/100.f);
        // else gPuppyCam.yawAcceleration -= 40*(gPuppyCam.options.sensitivityX/100.f);
        gPuppyCam.yawAcceleration -= 40*(gPuppyCam.options.sensitivityX/100.f);
        // gPuppyCam.framesSinceC[0] = 0;
    }
    else if (
        (
            !gPuppyCam.options.analogue
            // && (gPlayer1Controller->buttonDown & R_JPAD || gPlayer1Controller->buttonDown & R_CBUTTONS)
            && (gPlayer1Controller->buttonDown & R_JPAD)
            && gPuppyCam.framesSinceC[1] > 8
        )
        || gPuppyCam.stick2[0] != 0
    )
    {
        // if (gPlayer1Controller->buttonDown & R_CBUTTONS) gPuppyCam.yawAcceleration += 2*(gPuppyCam.options.sensitivityX/100.f);
        // else gPuppyCam.yawAcceleration += 40*(gPuppyCam.options.sensitivityX/100.f);
        gPuppyCam.yawAcceleration += 40*(gPuppyCam.options.sensitivityX/100.f);
        // gPuppyCam.framesSinceC[1] = 0;
    }
    else
        gPuppyCam.yawAcceleration = approach_f32_asymptotic(gPuppyCam.yawAcceleration, 0, DECELERATION);

    gPuppyCam.yawAcceleration = CLAMP(gPuppyCam.yawAcceleration, -100, 100);

    gPuppyCam.yawTarget += (12*gPuppyCam.yawAcceleration*ivX)*(stickMag*0.01f);
}

//Handles C Button inputs for modes that have pressed inputs, rather than held.
static void puppycam_input_press(void)
{
    f32 ivX = ((gPuppyCam.options.invertX*2)-1)*(gPuppyCam.options.sensitivityX/100.f);

    //Just in case it happens to be nonzero.
    gPuppyCam.yawAcceleration = 0;

    //In theory this shouldn't be necessary, but it's nice to cover all bases.
    if (!(gPuppyCam.flags & PUPPYCAM_BEHAVIOUR_YAW_ROTATION))
        return;

    if ((gPlayer1Controller->buttonPressed & L_CBUTTONS && !gPuppyCam.options.analogue) || (gPuppyCam.stickN[0] == 0 && gPuppyCam.stick2[0] < -DEADZONE))
    {
        gPuppyCam.stickN[0] = 1;
        if (gPuppyCam.flags & PUPPYCAM_BEHAVIOUR_INPUT_8DIR)
            gPuppyCam.yawTarget -= 0x2000;
            // gPuppyCam.yawTarget -= 0x2000*ivX;
        else
            gPuppyCam.yawTarget -= 0x4000;
            // gPuppyCam.yawTarget -= 0x4000*ivX;
        play_sound(SOUND_MENU_CAMERA_ZOOM_IN,gGlobalSoundSource);
    }

    if ((gPlayer1Controller->buttonPressed & R_CBUTTONS && !gPuppyCam.options.analogue) || (gPuppyCam.stickN[0] == 0 && gPuppyCam.stick2[0] > DEADZONE))
    {
        gPuppyCam.stickN[0] = 1;
        if (gPuppyCam.flags & PUPPYCAM_BEHAVIOUR_INPUT_8DIR)
            gPuppyCam.yawTarget += 0x2000;
            // gPuppyCam.yawTarget += 0x2000*ivX;
        else
            gPuppyCam.yawTarget += 0x4000;
            // gPuppyCam.yawTarget += 0x4000*ivX;
        play_sound(SOUND_MENU_CAMERA_ZOOM_IN,gGlobalSoundSource);
    }
}

static void puppycam_view_panning(void)
{
    f32 panFloor, panMulti;
    s16 height = gPuppyCam.targetObj->oPosY;
    u8 panEx = (gPuppyCam.zoomTarget >= 1000) * 250; //Removes the basic panning when idling if the zoom level is at the closest.

    panMulti = CLAMP(gPuppyCam.zoom/(f32)gPuppyCam.zoomPoints[2], 0.f, 1.f);

    if (gPuppyCam.flags & PUPPYCAM_BEHAVIOUR_PANSHIFT && gMarioState->action != ACT_HOLDING_BOWSER && gMarioState->action != ACT_SLEEPING && gMarioState->action != ACT_START_SLEEPING)
    {
        gPuppyCam.pan[0] = approach_f32_asymptotic(gPuppyCam.pan[0], LENSIN(panEx+(500*(gMarioState->forwardVel/32.f)), gMarioState->faceAngle[1])*panMulti, 0.02f);
        gPuppyCam.pan[2] = approach_f32_asymptotic(gPuppyCam.pan[2], LENCOS(panEx+(500*(gMarioState->forwardVel/32.f)), gMarioState->faceAngle[1])*panMulti, 0.02f);
        if (gMarioState->vel[1] == 0.0f)
        {
            panFloor = CLAMP(
                find_floor_height(
                    (s16)(gPuppyCam.targetObj->oPosX + gPuppyCam.pan[0]),
                    (s16)(gPuppyCam.targetObj->oPosY + 200),
                    (s16)(gPuppyCam.targetObj->oPosZ + gPuppyCam.pan[2])
                ),
                gPuppyCam.targetObj->oPosY - 50,
                gPuppyCam.targetObj->oPosY + 50
            );
            gPuppyCam.pan[1] = approach_f32_asymptotic(gPuppyCam.pan[1], panFloor-height, 0.25f);
        }
        else
            gPuppyCam.pan[1] = approach_f32_asymptotic(gPuppyCam.pan[1], 0.0f, 0.5f);
    }
    else
    {
        gPuppyCam.pan[0] = 0;
        gPuppyCam.pan[1] = 0;
        gPuppyCam.pan[2] = 0;
    }
}

//Sets the
static void puppycam_view_height_offset(void)
{
    s16 floorTemp;
    s16 tempDist = sqrtf((gPuppyCam.pos[0] - gPuppyCam.focus[0]) * (gPuppyCam.pos[0] - gPuppyCam.focus[0]) + (gPuppyCam.pos[1] - gPuppyCam.focus[1]) *
                         (gPuppyCam.pos[1] - gPuppyCam.focus[1]) + (gPuppyCam.pos[2] - gPuppyCam.focus[2]) * (gPuppyCam.pos[2] - gPuppyCam.focus[2]));

    floorTemp = find_floor_height(gPuppyCam.targetObj->oPosX, gPuppyCam.targetObj->oPosY+200, gPuppyCam.targetObj->oPosZ);
    if (floorTemp > gPuppyCam.targetObj->oPosY - 200 && !(gMarioState->action & ACT_FLAG_SWIMMING))
    {
        gPuppyCam.posHeight[0] = approach_f32_asymptotic(
            gPuppyCam.posHeight[0],
            floorTemp-gPuppyCam.targetFloorHeight,
            0.05f
        );
        //if (gPuppyCam.posHeight[0]-gPuppyCam.shake[1] - gPuppyCam.floorY[1] < floorTemp)
        //    gPuppyCam.posHeight[0] = floorTemp-gPuppyCam.shake[1]+gPuppyCam.povHeight - gPuppyCam.floorY[1];
    }
    else
    {
            gPuppyCam.posHeight[0] = approach_f32_asymptotic(gPuppyCam.posHeight[0], 0, 0.05f);
    }


    floorTemp = find_floor_height(gPuppyCam.targetObj->oPosX + LENSIN(tempDist,gPuppyCam.yaw), gPuppyCam.targetObj->oPosY+200, gPuppyCam.targetObj->oPosZ + LENCOS(tempDist,gPuppyCam.yaw));
    if (floorTemp > gPuppyCam.targetObj->oPosY - 200 && !(gMarioState->action & ACT_FLAG_SWIMMING))
    {
        gPuppyCam.posHeight[1] = approach_f32_asymptotic(gPuppyCam.posHeight[1], floorTemp-gPuppyCam.targetFloorHeight, 0.05f);
        //if (gPuppyCam.posHeight[1]-gPuppyCam.shake[1] - gPuppyCam.floorY[0] < floorTemp)
        //    gPuppyCam.posHeight[1] = floorTemp-gPuppyCam.shake[1]+gPuppyCam.povHeight - gPuppyCam.floorY[0];
    }
    else
    {
            gPuppyCam.posHeight[1] = approach_f32_asymptotic(gPuppyCam.posHeight[1], 0, 0.05f);
    }
}

/// Get length of vector 'a'
f32 vec3f_length(Vec3f a)
{
	return sqrtf(a[0] * a[0] + a[1] * a[1] + a[2] * a[2]);
}

/// Get dot product of vectors 'a' and 'b'
f32 vec3f_dot(Vec3f a, Vec3f b)
{
	return a[0] * b[0] + a[1] * b[1] + a[2] * b[2];
}

//Raycasting
s32 ray_surface_intersect(Vec3f orig, Vec3f dir, f32 dir_length, struct Surface *surface, Vec3f hit_pos, f32 *length)
{
    Vec3f v0, v1, v2, e1, e2, h, s, q;
    f32 a, f, u, v;
    Vec3f add_dir;
    Vec3f norm;

    //Ignore certain surface types.
    if (surface->type == SURFACE_INTANGIBLE || surface->flags & SURFACE_FLAG_NO_CAM_COLLISION)
        return FALSE;

    // Get surface normal and some other stuff
    norm[0] = 0.0f;
    norm[1] = surface->normal.y;
    norm[2] = 0.0f;
    vec3f_mul(norm, sRayCastOffset);
    // vec3f_mul(norm, 0);

    vec3s_to_vec3f(v0, surface->vertex1);
    vec3s_to_vec3f(v1, surface->vertex2);
    vec3s_to_vec3f(v2, surface->vertex3);

    vec3f_add(v0, norm);
    vec3f_add(v1, norm);
    vec3f_add(v2, norm);

    vec3f_diff(e1, v1, v0);
    vec3f_diff(e2, v2, v0);

    vec3f_cross(h, dir, e2);

    // Check if we're perpendicular from the surface
    a = vec3f_dot(e1, h);
    if (a > -0.00001f && a < 0.00001f)
        return FALSE;

    // Check if we're making contact with the surface
    f = 1.0f / a;

    vec3f_diff(s, orig, v0);
    u = f * vec3f_dot(s, h);
    if (u < 0.0f || u > 1.0f)
        return FALSE;

    vec3f_cross(q, s, e1);
    v = f * vec3f_dot(dir, q);
    if (v < 0.0f || u + v > 1.0f)
        return FALSE;

    // Get the length between our origin and the surface contact point
    *length = f * vec3f_dot(e2, q);
    if (*length <= 0.00001 || *length > dir_length)
        return FALSE;

    // Successful contact
    vec3f_copy(add_dir, dir);
    vec3f_mul(add_dir, *length);
    vec3f_sum(hit_pos, orig, add_dir);

    // norm[0] = 0.0f;
    // norm[1] = surface->normal.y;
    // norm[2] = 0.0f;
    // norm[0] = surface->normal.x;
    // norm[1] = surface->normal.y;
    // norm[2] = surface->normal.z;
    // vec3f_mul(norm, sRayCastOffset);
    // vec3f_add(hit_pos, norm);

    return TRUE;
}

void find_surface_on_ray_list(struct SurfaceNode *list, Vec3f orig, Vec3f dir, f32 dir_length, struct Surface **hit_surface, Vec3f hit_pos, f32 *max_length)
{
    s32 hit;
    f32 length;
    Vec3f chk_hit_pos;
    f32 top, bottom;

    // Get upper and lower bounds of ray
    if (dir[1] >= 0.0f)
    {
        top = orig[1] + dir[1] * dir_length;
        bottom = orig[1];
    }
    else
    {
        top = orig[1];
        bottom = orig[1] + dir[1] * dir_length;
    }

    // Iterate through every surface of the list
    for (; list != NULL; list = list->next)
    {
        // Reject surface if out of vertical bounds
        if (list->surface->lowerY > top || list->surface->upperY < bottom)
            continue;

        // Check intersection between the ray and this surface
        if ((hit = ray_surface_intersect(orig, dir, dir_length, list->surface, chk_hit_pos, &length)) != 0)
        {
            if (length <= *max_length)
            {
                *hit_surface = list->surface;
                vec3f_copy(hit_pos, chk_hit_pos);
                *max_length = length;
            }
        }
    }
}


void find_surface_on_ray_cell(s16 cellX, s16 cellZ, Vec3f orig, Vec3f normalized_dir, f32 dir_length, struct Surface **hit_surface, Vec3f hit_pos, f32 *max_length)
{
	// Skip if OOB
	if (cellX >= 0 && cellX <= (NUM_CELLS - 1) && cellZ >= 0 && cellZ <= (NUM_CELLS - 1))
	{
		// Iterate through each surface in this partition
		if (normalized_dir[1] > -0.99999f)
		{
			find_surface_on_ray_list(gStaticSurfacePartition[cellZ][cellX][SPATIAL_PARTITION_CEILS].next, orig, normalized_dir, dir_length, hit_surface, hit_pos, max_length);
			find_surface_on_ray_list(gDynamicSurfacePartition[cellZ][cellX][SPATIAL_PARTITION_CEILS].next, orig, normalized_dir, dir_length, hit_surface, hit_pos, max_length);
		}
		if (normalized_dir[1] < 0.99999f)
		{
			find_surface_on_ray_list(gStaticSurfacePartition[cellZ][cellX][SPATIAL_PARTITION_FLOORS].next, orig, normalized_dir, dir_length, hit_surface, hit_pos, max_length);
			find_surface_on_ray_list(gDynamicSurfacePartition[cellZ][cellX][SPATIAL_PARTITION_FLOORS].next, orig, normalized_dir, dir_length, hit_surface, hit_pos, max_length);
		}
		find_surface_on_ray_list(gStaticSurfacePartition[cellZ][cellX][SPATIAL_PARTITION_WALLS].next, orig, normalized_dir, dir_length, hit_surface, hit_pos, max_length);
		find_surface_on_ray_list(gDynamicSurfacePartition[cellZ][cellX][SPATIAL_PARTITION_WALLS].next, orig, normalized_dir, dir_length, hit_surface, hit_pos, max_length);
	}
}

void find_surface_on_ray(Vec3f orig, Vec3f dir, struct Surface **hit_surface, Vec3f hit_pos)
{
    f32 max_length;
    s16 cellZ, cellX, cellPrevX, cellPrevZ;
    f32 fCellZ, fCellX;
    f32 dir_length;
    Vec3f normalized_dir;
    f32 step, dx, dz;
    u32 i;

    // Set that no surface has been hit
    *hit_surface = NULL;
    vec3f_sum(hit_pos, orig, dir);

    // Get normalized direction
    dir_length = vec3f_length(dir);
    max_length = dir_length;
    vec3f_copy(normalized_dir, dir);
    vec3f_normalize(normalized_dir);

    // Get our cell coordinate
    fCellX = (orig[0] + LEVEL_BOUNDARY_MAX) / CELL_SIZE;
    fCellZ = (orig[2] + LEVEL_BOUNDARY_MAX) / CELL_SIZE;
    cellX = (s16)fCellX;
    cellZ = (s16)fCellZ;

    // Don't do DDA if straight down
    if (normalized_dir[1] >= 0.99999f || normalized_dir[1] <= -0.99999f)
    {
		find_surface_on_ray_cell(cellX, cellZ, orig, normalized_dir, dir_length, hit_surface, hit_pos, &max_length);
		return;
	}

    // Get cells we cross using DDA
    if (ABS(dir[0]) >= ABS(dir[2]))
        step = STEPS*ABS(dir[0]) / CELL_SIZE;
    else
        step = STEPS*ABS(dir[2]) / CELL_SIZE;

    dx = dir[0] / step / CELL_SIZE;
    dz = dir[2] / step / CELL_SIZE;

    for (i = 0; i < step && *hit_surface == NULL; i++)
    {
		find_surface_on_ray_cell(cellX, cellZ, orig, normalized_dir, dir_length, hit_surface, hit_pos, &max_length);

        // Move cell coordinate
        fCellX += dx;
        fCellZ += dz;
        cellPrevX = cellX;
        cellPrevZ = cellZ;
        cellX = (s16)fCellX;
        cellZ = (s16)fCellZ;

        if ((cellPrevX != cellX) && (cellPrevZ != cellZ))
        {
            find_surface_on_ray_cell(cellX, cellPrevZ, orig, normalized_dir, dir_length, hit_surface, hit_pos, &max_length);
            find_surface_on_ray_cell(cellPrevX, cellZ, orig, normalized_dir, dir_length, hit_surface, hit_pos, &max_length);
        }
    }
}

//Checks the bounding box of a puppycam volume. If it's inside, then set the pointer to the current index.
static u8 puppycam_check_volume_bounds(struct sPuppyVolume *volume, u16 index)
{
    Vec3f rel;

    if (sPuppyVolumeStack[index]->room != gMarioCurrentRoom && sPuppyVolumeStack[index]->room != -1)
        return;

    //debug_box_pos_rot(sPuppyVolumeStack[index]->pos, sPuppyVolumeStack[index]->radius, sPuppyVolumeStack[index]->rot);

    //Fetch the relative position. to the triggeree.
    rel[0] = sPuppyVolumeStack[index]->pos[0] - gPuppyCam.targetObj->oPosX;
    rel[1] = sPuppyVolumeStack[index]->pos[1] - gPuppyCam.targetObj->oPosY;
    rel[2] = sPuppyVolumeStack[index]->pos[2] - gPuppyCam.targetObj->oPosZ;
    //Use the dark, forbidden arts of trig to rotate the volume.
    rel[0] = rel[2] * sins(sPuppyVolumeStack[index]->rot) + rel[0] * coss(sPuppyVolumeStack[index]->rot);
    rel[2] = rel[2] * coss(sPuppyVolumeStack[index]->rot) - rel[0] * sins(sPuppyVolumeStack[index]->rot);

    //Now compare values.
    if (-sPuppyVolumeStack[index]->radius[0] < rel[0] && rel[0] < sPuppyVolumeStack[index]->radius[0] &&
        -sPuppyVolumeStack[index]->radius[1] < rel[1] && rel[1] < sPuppyVolumeStack[index]->radius[1] &&
        -sPuppyVolumeStack[index]->radius[2] < rel[2] && rel[2] < sPuppyVolumeStack[index]->radius[2])
        {
            *volume = *sPuppyVolumeStack[index];
            return TRUE;
        }

    return FALSE;
}

void puppycam_shake(s16 x, s16 y, s16 z)
{

}


void snap_to_45_degrees(void) {
    if (gPuppyCam.yawTarget % 0x2000) {
        s16 d1 = ABS(gPuppyCam.yawTarget) % 0x2000;
        s16 d2 = 0x2000 - d1;
        if (gPuppyCam.yawTarget > 0) {
            if (d1 < d2) gPuppyCam.yawTarget -= d1;
            else gPuppyCam.yawTarget += d2;
        } else {
            if (d1 < d2) gPuppyCam.yawTarget += d1;
            else gPuppyCam.yawTarget -= d2;
        }
    }
}
///This is the highest level of the basic steps that go into the code. Anything above is called from these following functions.

s16 approach_yaw(s16 curYaw, s16 target, f32 speed) {
    return (s16) (target - approach_f32_asymptotic(
        (s16) (target - curYaw),
        0,
        speed
    ));
}

s32 handle_wall_bonk_cam(void) {
    if (
        gCurCutscene != CUTSCENE_TOWERCLIMB &&
        gMarioState->action == ACT_WALL_KICK_AIR &&
        gMarioState->actionTimer <= 5 &&
        abs_angle_diff(gMarioState->wallAngle, gMarioState->faceAngle[1]) < 0x1000
    ) {
        s16 left = gMarioState->wallAngle - 0x4000;
        s16 right = gMarioState->wallAngle + 0x4000;
        s32 leftDiff = abs_angle_diff(left, gPuppyCam.yaw);
        s32 rightDiff = abs_angle_diff(right, gPuppyCam.yaw);
        if (leftDiff < 0x2000) {
            gPuppyCam.yawTarget = left;
            return TRUE;
        }
        if (rightDiff < 0x2000) {
            gPuppyCam.yawTarget = right;
            return TRUE;
        }
    } else if (
        gCurCutscene == CUTSCENE_TOWERCLIMB &&
        gMarioState->action == ACT_WALL_KICK_AIR &&
        abs_angle_diff(gMarioState->wallAngle, gMarioState->faceAngle[1]) < 0x1000
    ) {
        s16 left = gMarioState->wallAngle - 0x4000;
        s16 right = gMarioState->wallAngle + 0x4000;
        s32 leftDiff = abs_angle_diff(left, gPuppyCam.yaw);
        s32 rightDiff = abs_angle_diff(right, gPuppyCam.yaw);
        if (leftDiff < 0x3000) {
            gPuppyCam.yaw = approach_yaw(gPuppyCam.yaw, left, 0.2f);
            return TRUE;
        }
        if (rightDiff < 0x3000) {
            gPuppyCam.yaw = approach_yaw(gPuppyCam.yaw, right, 0.2f);
            return TRUE;
        }
    }
    return FALSE;
}

//The centrepiece behind the input side of PuppyCam. The C buttons branch off.
static void puppycam_input_core(void)
{
    f32 ivY = ((gPuppyCam.options.invertY*2)-1)*(gPuppyCam.options.sensitivityY/100.f);
    s8 stickMag = 0;

    puppycam_analogue_stick();

    if (gPuppyCam.options.analogue)
        stickMag = gPuppyCam.stick2[1]*1.25f;
    else
        stickMag = 100;

    //Decide which input for left and right C buttons to use based on behaviour type.
    if (gPuppyCam.flags & PUPPYCAM_BEHAVIOUR_INPUT_NORMAL)
        puppycam_input_hold();

// NOTE: THIS BREAKS TRADITIONAL 8 DIR CAM
    // if (gPuppyCam.flags & PUPPYCAM_BEHAVIOUR_INPUT_8DIR || gPuppyCam.flags & PUPPYCAM_BEHAVIOUR_INPUT_4DIR)
    //     puppycam_input_press();


    //Handles R button zooming.
    // if (gPlayer1Controller->buttonPressed & R_TRIG && gPuppyCam.flags & PUPPYCAM_BEHAVIOUR_ZOOM_CHANGE)
    // {
    //     gPuppyCam.zoomSet++;

    //     if (gPuppyCam.zoomSet >= 3)
    //         gPuppyCam.zoomSet = 0;

    //     gPuppyCam.zoomTarget = gPuppyCam.zoomPoints[gPuppyCam.zoomSet];
    //     play_sound(SOUND_MENU_CLICK_CHANGE_VIEW,gGlobalSoundSource);
    // }

    if (!gPuppyCam.options.analogue)
    {
        //Handles vertical inputs.
        if (gPlayer1Controller->buttonPressed & U_CBUTTONS) {
            gPuppyCam.zoomSet = gPuppyCam.zoomSet == 0 ? 0 : gPuppyCam.zoomSet - 1;
            gPuppyCam.zoomTarget = gPuppyCam.zoomPoints[gPuppyCam.zoomSet];
            gPuppyCam.pitchTarget = sZoomPitchTargets[gPuppyCam.zoomSet];
            play_sound(SOUND_MENU_CLICK_CHANGE_VIEW,gGlobalSoundSource);
        }
        else if (gPlayer1Controller->buttonPressed & D_CBUTTONS) {
            gPuppyCam.zoomSet = gPuppyCam.zoomSet == 2 ? 2 : gPuppyCam.zoomSet + 1;
            gPuppyCam.zoomTarget = gPuppyCam.zoomPoints[gPuppyCam.zoomSet];
            gPuppyCam.pitchTarget = sZoomPitchTargets[gPuppyCam.zoomSet];
            play_sound(SOUND_MENU_CLICK_CHANGE_VIEW,gGlobalSoundSource);
        } else if (gPlayer1Controller->buttonPressed & R_TRIG && !(gPlayer1Controller->buttonDown & L_TRIG)) {
            gPuppyCam.zoomSet = gPuppyCam.zoomSet == 0 ? 2 : 0;
            gPuppyCam.zoomTarget = gPuppyCam.zoomPoints[gPuppyCam.zoomSet];
            gPuppyCam.pitchTarget = sZoomPitchTargets[gPuppyCam.zoomSet];
            if (gPuppyCam.zoomSet == 0) gPuppyCam.yawTarget = gMarioState->faceAngle[1]+0x8000;
            play_sound(SOUND_MENU_CLICK_CHANGE_VIEW,gGlobalSoundSource);
        }
    }
    

    //Handles L button centering.
    if (
        // Flag checks
        !(gPuppyCam.flags & PUPPYCAM_BEHAVIOUR_INPUT_8DIR) &&
        !(gPuppyCam.flags & PUPPYCAM_BEHAVIOUR_INPUT_4DIR) &&
        gPuppyCam.flags & PUPPYCAM_BEHAVIOUR_YAW_ROTATION &&
        // Action checks
        !sWaitForLUnheld && // Make sure the L+R combo hasn't been inputted
        sLWasHeld && // Make sure the L button was held in the previous frame
        !(gPlayer1Controller->buttonDown & L_TRIG) && // Make sure the L button isn't held now
        !(gPlayer1Controller->buttonDown & U_JPAD) // Make sure DPad up isn't held? whats the point
    ) {
        gPuppyCam.yawTarget = gMarioState->faceAngle[1]+0x8000;
        gPuppyCam.pitchTarget = sZoomPitchTargets[gPuppyCam.zoomSet];
        play_sound(SOUND_MENU_CLICK_CHANGE_VIEW,gGlobalSoundSource);
    }

    if (gPuppyCam.flags & PUPPYCAM_BEHAVIOUR_PITCH_ROTATION)
    {
        //Handles vertical inputs.
        if (gPlayer1Controller->buttonDown & U_JPAD || gPuppyCam.stick2[1] != 0)
            gPuppyCam.pitchAcceleration -= 15*(gPuppyCam.options.sensitivityY/100.f);
        else
        if (gPlayer1Controller->buttonDown & D_JPAD || gPuppyCam.stick2[1] != 0)
            gPuppyCam.pitchAcceleration += 15*(gPuppyCam.options.sensitivityY/100.f);
        else
            gPuppyCam.pitchAcceleration = approach_f32_asymptotic(gPuppyCam.pitchAcceleration, 0, DECELERATION);

        gPuppyCam.pitchAcceleration = CLAMP(gPuppyCam.pitchAcceleration, -100, 100);
        gPuppyCam.pitchTarget += (8*gPuppyCam.pitchAcceleration*ivY)*(stickMag*0.01f);
    }

    if ((gPlayer1Controller->buttonDown & L_TRIG && gPlayer1Controller->buttonPressed & R_TRIG))
    {
        sWaitForLUnheld = TRUE;
        // if (gPuppyCam.yawTarget % 0x2000) {
        //     s16 d1 = ABS(gPuppyCam.yawTarget) % 0x2000;
        //     s16 d2 = 0x2000 - d1;
        //     if (gPuppyCam.yawTarget > 0) {
        //         if (d1 < d2) gPuppyCam.yawTarget -= d1;
        //         else gPuppyCam.yawTarget += d2;
        //     } else {
        //         if (d1 < d2) gPuppyCam.yawTarget += d1;
        //         else gPuppyCam.yawTarget -= d2;
        //     }
        // }
        snap_to_45_degrees();
        sMovingFramesSinceSnap = 0;
    } else if (sMovingFramesSinceSnap < SNAP_HOLD_LENGTH && gMarioState->forwardVel != 0) {
        sMovingFramesSinceSnap++;
    }
    //If the camera's in these modes, snap the yaw to prevent desync.
    // if (gPuppyCam.flags & PUPPYCAM_BEHAVIOUR_INPUT_8DIR && gCurCutscene != CUTSCENE_TOWERCLIMB)
    if (gPuppyCam.flags & PUPPYCAM_BEHAVIOUR_INPUT_8DIR)
    {
        // if (gPuppyCam.yawTarget % 0x2000)
        //     gPuppyCam.yawTarget += 0x2000 - gPuppyCam.yawTarget % 0x2000;
        snap_to_45_degrees();
    }
    if (gPuppyCam.flags & PUPPYCAM_BEHAVIOUR_INPUT_4DIR)
        gPuppyCam.yawTarget %= 0x4000;

    if (gPuppyCam.targetObj2 != NULL) {
        // CTODO: find a safe place to see both in view
        if (gCurCutscene != CUTSCENE_TOWERCLIMB) gPuppyCam.yawTarget = obj_angle_to_object(gPuppyCam.targetObj2, gPuppyCam.targetObj);
    }

    if (gPlayer1Controller->buttonDown & L_TRIG) sLWasHeld = TRUE;
    else {
        sLWasHeld = FALSE;
        sWaitForLUnheld = FALSE;
    }

    if (gCurCutscene != CUTSCENE_TOWERCLIMB) handle_wall_bonk_cam();
}

f32 vec3f_dist(Vec3f fromVec, Vec3f toVec) {
    return sqrtf((fromVec[0] - toVec[0]) * (fromVec[0] - toVec[0]) + (fromVec[1] - toVec[1]) * (fromVec[1] - toVec[1]) + (fromVec[2] - toVec[2]) * (fromVec[2] - toVec[2]));
}

/**
 * Pitch the camera down when the camera is facing down a slope
 */
// s16 get_floor_pitch(void) {
//     struct Surface *floor = gMarioState->floor;
//     if (floor && gMarioState->action & (ACT_FLAG_MOVING | ACT_FLAG_STATIONARY)) {
//         s16 slopeAngle = atan2s(floor->normal.z, floor->normal.x);
//         if (ABS(slopeAngle - gPuppyCam.yawTarget) > 0x8000) {
//             f32 steepness = sqrtf(floor->normal.x * floor->normal.x + floor->normal.z * floor->normal.z);
//             return DEGREES(steepness);
//         }
//     }
//     return 0;
// }

static s16 get_pitch_offset(void)
{
    struct Surface *surf;
    Vec3f ceildir;
    Vec3f campos;
    Vec3f hitpos;
    Vec3f target;

    target[0] = gPuppyCam.targetObj->oPosX;
    target[1] = gPuppyCam.targetObj->oPosY + gPuppyCam.povHeight;
    target[2] = gPuppyCam.targetObj->oPosZ;

    ceildir[0] = 0;
    ceildir[1] = 1000.0f;
    ceildir[2] = 0;

    campos[0] = gPuppyCam.pos[0];
    campos[1] = gPuppyCam.pos[1];
    campos[2] = gPuppyCam.pos[2];

    sRayCastOffset = sCameraWallOffset;
    find_surface_on_ray(target, ceildir, &surf, &hitpos);

    if (surf && surf->normal.y < -0.5)
    {
        f32 opposite, adjacent;
        f32 dx = target[0] - campos[0];
        f32 dz = target[2] - campos[2];

        opposite = vec3f_dist(target, hitpos);
        adjacent = sqrtf(dx * dx + dz * dz);

        return MAX(gPuppyCam.pitchTarget, atan2s(opposite, adjacent));
    }
    return gPuppyCam.pitchTarget;
}

//Calculates the base position the camera should be, before any modification.
static void puppycam_projection(void)
{
    Vec3s targetPos, targetPos2, targetPos3;
    u8 panD = (gPuppyCam.flags & PUPPYCAM_BEHAVIOUR_PANSHIFT)/8192;
    u32 isSwimming = gMarioState->action & ACT_FLAG_SWIMMING;
    // s16 floorSteepness = get_floor_pitch();
    gPuppyCam.pitchTarget = get_pitch_offset();
    // if (floorSteepness) {
    //     gPuppyCam.pitchTarget = floorSteepness;
    // }

    if (
        gPuppyCam.options.turnAggression > 0 &&
        gPuppyCam.flags & PUPPYCAM_BEHAVIOUR_TURN_HELPER &&
        !(gPuppyCam.flags & PUPPYCAM_BEHAVIOUR_INPUT_8DIR) &&
        (gMarioState->vel[1] == 0.0f || (isSwimming && ABS(gMarioState->vel[1]) < 5.0f)) &&
        !(gMarioState->action & ACT_FLAG_BUTT_OR_STOMACH_SLIDE) &&
        !(gPuppyCam.flags & PUPPYCAM_BEHAVIOUR_INPUT_4DIR)
    )
    {
        if (ABS(gPlayer1Controller->rawStickX) > 20 && sMovingFramesSinceSnap > 60)
            gPuppyCam.yawTarget  = gMarioState->faceAngle[1]+0x8000 - approach_s32((s16)(gMarioState->faceAngle[1]+0x8000 - gPuppyCam.yawTarget), 0,
            (gPuppyCam.options.turnAggression*10)*(gMarioState->forwardVel/32) * ABS(gPlayer1Controller->rawStickX/80.0f),
            (gPuppyCam.options.turnAggression*10)*(gMarioState->forwardVel/32) * ABS(gPlayer1Controller->rawStickX/80.0f));
    }

    if (gCurCutscene != CUTSCENE_TOWERCLIMB) {
        gPuppyCam.yaw = gPuppyCam.yawTarget - approach_f32_asymptotic((s16)(gPuppyCam.yawTarget - gPuppyCam.yaw), 0, 0.2f);
    }
    gPuppyCam.pitch = gPuppyCam.pitchTarget - approach_f32_asymptotic((s16)(gPuppyCam.pitchTarget - gPuppyCam.pitch), 0, 0.1f);
    // if (gCurCutscene == CUTSCENE_TOWERCLIMB) gPuppyCam.yawTarget = gPuppyCam.yawTarget + (gPuppyCam.targetObj2->oAngleToMario + 0x8000);

    gPuppyCam.pitch = CLAMP(gPuppyCam.pitch,0x1000,0x7000);
    gPuppyCam.pitchTarget = CLAMP(gPuppyCam.pitchTarget,0x1000,0x7000);
    if (!isSwimming && (gMarioState->forwardVel || gMarioState->vel[1])) {
        gPuppyCam.pitchTarget = (s16)approach_f32_asymptotic(gPuppyCam.pitchTarget, sZoomPitchTargets[gPuppyCam.zoomSet], 0.05f);
    }

    if (gPuppyCam.targetObj == gMarioState->marioObj)
    {
        gPuppyCam.targetFloorHeight = CLAMP(find_floor_height(gPuppyCam.targetObj->oPosX, gPuppyCam.targetObj->oPosY, gPuppyCam.targetObj->oPosZ), gPuppyCam.targetObj->oPosY-350, gPuppyCam.targetObj->oPosY+300);

        if (gMarioState->vel[1] <= 0.0f)
            gPuppyCam.lastTargetFloorHeight = CLAMP(approach_f32_asymptotic(gPuppyCam.lastTargetFloorHeight,gPuppyCam.targetFloorHeight, 0.1f), gPuppyCam.targetObj->oPosY-350, gPuppyCam.targetObj->oPosY+300);

        if (gMarioState->action == ACT_SLEEPING || gMarioState->action == ACT_START_SLEEPING)
            gPuppyCam.zoom = approach_f32_asymptotic(gPuppyCam.zoom, gPuppyCam.zoomPoints[0], 0.01f);
        else if (gCurCutscene == CUTSCENE_SLIDE) {
            Vec3f nextMarioPos;
            s16 dirPitch, dirYaw;
            nextMarioPos[0] = gMarioState->pos[0] + (gMarioState->vel[0] * 8);
            nextMarioPos[1] = gMarioState->pos[1] + (gMarioState->vel[1] * 8);
            nextMarioPos[2] = gMarioState->pos[2] + (gMarioState->vel[2] * 8);
            dirPitch = calculate_pitch(gMarioState->pos, nextMarioPos);
            gPuppyCam.zoom = approach_f32_asymptotic(gPuppyCam.zoom, 350.0f, 0.02f);


            if (gMarioState->forwardVel && ABS(gMarioState->forwardVel) > ABS(gMarioState->vel[1])) {
                gPuppyCam.pitchTarget = (s16) approach_f32_asymptotic(gPuppyCam.pitchTarget, 7000.0f, 0.065f);
            } else {
                gPuppyCam.pitchTarget = (s16) approach_f32_asymptotic(gPuppyCam.pitchTarget, dirPitch, 0.065f);
            }
            if (gMarioState->forwardVel >= 0.0f) {
                gPuppyCam.yawTarget = gMarioState->faceAngle[1] > 0
                    ? 0 - 0x4000
                    : 0x4000;
            } else {
                gPuppyCam.yawTarget = gMarioState->faceAngle[1] > 0
                    ? 0x4000
                    : 0 - 0x4000;
            }

            if (gCurCutsceneTimer < 15) gLakituState.roll = approach_f32_asymptotic(gLakituState.roll, 0x8000, 0.01f);
            else gLakituState.roll += 0x148;
        } else {
            if (gLakituState.roll != 0) {
                if (ABS(gLakituState.roll) <= 500) gLakituState.roll = approach_s16_symmetric(gLakituState.roll, 0x0, 0x10);
                else if (gLakituState.roll > 0) gLakituState.roll = approach_f32_asymptotic(gLakituState.roll, 0x10000, 0.05);
                else gLakituState.roll = approach_f32_asymptotic(gLakituState.roll, 0x0, 0.05f);
            }
            gPuppyCam.zoom = approach_f32_asymptotic(gPuppyCam.zoom, gPuppyCam.zoomTarget, 0.1f);
        }

        if (!(gMarioState->action & ACT_FLAG_SWIMMING))
        {
            gPuppyCam.floorY[0] = CLAMP(gPuppyCam.targetObj->oPosY - gPuppyCam.lastTargetFloorHeight, 0, 300);
            gPuppyCam.floorY[1] = CLAMP(gPuppyCam.targetObj->oPosY - gPuppyCam.lastTargetFloorHeight, 0, 350);
            gPuppyCam.swimPitch = approach_f32_asymptotic(gPuppyCam.swimPitch, 0, 0.2f);
        }
        else
        {
            gPuppyCam.floorY[0] = 0;
            gPuppyCam.floorY[1] = 0;
            gPuppyCam.targetFloorHeight = gPuppyCam.targetObj->oPosY;
            gPuppyCam.lastTargetFloorHeight = gPuppyCam.targetObj->oPosY;
            if (gCurCutscene != CUTSCENE_TOWERCLIMB) {
                gPuppyCam.swimPitch = approach_f32_asymptotic(
                    gPuppyCam.swimPitch,
                    gMarioState->faceAngle[0]/10,
                    0.05f
                );
                // gPuppyCam.yawTarget  = gMarioState->faceAngle[1] + 0x8000 - approach_s32(
                //     (s16)(gMarioState->faceAngle[1]+0x8000 - gPuppyCam.yawTarget),
                //     0,
                //     1000 * (gMarioState->forwardVel / 32), 1000 * (gMarioState->forwardVel / 32)
                // );
            }
        }
    }
    else
    {
        gPuppyCam.floorY[0] = 0;
        gPuppyCam.floorY[1] = 0;
        gPuppyCam.swimPitch = 0;
    }

    if (gPuppyCam.targetObj)
    {
        vec3s_set(targetPos, gPuppyCam.targetObj->oPosX, gPuppyCam.targetObj->oPosY, gPuppyCam.targetObj->oPosZ);
        vec3s_copy(targetPos3, targetPos);
        if (gPuppyCam.targetObj2 && gCurCutscene != CUTSCENE_TOWERCLIMB)
        {
            vec3s_set(targetPos2, gPuppyCam.targetObj2->oPosX, gPuppyCam.targetObj2->oPosY, gPuppyCam.targetObj2->oPosZ);
            targetPos3[0] = (s16)approach_f32_asymptotic(targetPos[0], targetPos2[0], 0.5f);
            targetPos3[1] = (s16)approach_f32_asymptotic(targetPos[1], targetPos2[1], 0.5f);
            targetPos3[2] = (s16)approach_f32_asymptotic(targetPos[2], targetPos2[2], 0.5f);

        }

        puppycam_view_panning();

        if (gPuppyCam.flags & PUPPYCAM_BEHAVIOUR_HEIGHT_HELPER)
        {
             puppycam_view_height_offset();
        }
        else
        {
            gPuppyCam.posHeight[0] = 0;
            gPuppyCam.posHeight[1] = 0;
        }

        gPuppyCam.focus[0] = targetPos3[0] + gPuppyCam.shake[0] + (
            gPuppyCam.pan[0] * gPuppyCam.collisionDistance / gPuppyCam.zoomPoints[2]
        ) * panD;
        gPuppyCam.focus[1] = targetPos3[1] + gPuppyCam.shake[1] + (
            gPuppyCam.pan[1] * gPuppyCam.collisionDistance / gPuppyCam.zoomPoints[2]
        ) + gPuppyCam.povHeight - gPuppyCam.floorY[0] + gPuppyCam.posHeight[0] + (gPuppyCam.swimPitch / 10);
        gPuppyCam.focus[2] = targetPos3[2] + gPuppyCam.shake[2] + (
            gPuppyCam.pan[2] * gPuppyCam.collisionDistance / gPuppyCam.zoomPoints[2]
        ) * panD;

        gPuppyCam.pos[0] = gPuppyCam.targetObj->oPosX + LENSIN(LENSIN(gPuppyCam.zoom, gPuppyCam.pitch), gPuppyCam.yaw) + gPuppyCam.shake[0];
        gPuppyCam.pos[1] = gPuppyCam.targetObj->oPosY + LENCOS(gPuppyCam.zoom, gPuppyCam.pitch + (gPuppyCam.swimPitch * 5)) + gPuppyCam.shake[1] - gPuppyCam.floorY[1] + gPuppyCam.posHeight[1];
        gPuppyCam.pos[2] = gPuppyCam.targetObj->oPosZ + LENCOS(LENSIN(gPuppyCam.zoom, gPuppyCam.pitch), gPuppyCam.yaw) + gPuppyCam.shake[2];
    }


    //Adds support for wing mario tower
    if (gMarioState->floor)
    if (gMarioState->floor->type == SURFACE_LOOK_UP_WARP) {
        if (save_file_get_total_star_count(gCurrSaveFileNum - 1, 0, 0x18) >= 10) {
            if (gPuppyCam.pitch < -8000 && gMarioState->forwardVel == 0) {
                level_trigger_warp(gMarioState, 1);
            }
        }
    }

}

#define DOOR_CUTSCENE_LEN 108
#define DOOR_LERP_START 30
#define DOOR_LERP_END 75

void door_cutscene(void) {
    sCutsceneVp.vp.vscale[0] = 640;
    sCutsceneVp.vp.vscale[1] = 360;
    sCutsceneVp.vp.vtrans[0] = 640;
    sCutsceneVp.vp.vtrans[1] = 480;
    override_viewport_and_clip(NULL, &sCutsceneVp, 0, 0, 0);

    if (gCurCutsceneTimer >= DOOR_LERP_START) {
        struct Surface *surf;
        Vec3f camdir;
        Vec3f campos;
        Vec3f hitpos;
        Vec3f target;
        f32 lerp_pow = CLAMP(
            get_relative_position_between_ranges(gCurCutsceneTimer, DOOR_LERP_START, DOOR_LERP_END, 0.01f, 1.0f),
            0.0f,
            1.0f
        );
        s32 faceUp = gPuppyCam.targetObj2->oFaceAnglePitch == 0 && gPuppyCam.targetObj2->oFaceAngleRoll == 0;
        // f32 relativePercent = get_relative_position_between_ranges()

        target[0] = gPuppyCam.targetObj2->oHomeX;
        // NO NO NO NO should get actual middle of door you idiot
        target[1] = faceUp
            ? gPuppyCam.targetObj2->oHomeY + 300.0f // add height to get middle of door
            : gPuppyCam.targetObj2->oHomeY - 300.0f; // add height to get door of middle
        target[2] = gPuppyCam.targetObj2->oHomeZ;

        // instead this should be position of where we want the camera
        vec3f_set_dist_and_angle(
            target,
            campos,
            1800.0f,
            gPuppyCam.targetObj2->oFaceAnglePitch,
            gPuppyCam.targetObj2->oFaceAngleYaw + 0x8000
        );
        camdir[0] = campos[0] - target[0];
        camdir[1] = campos[1] - target[1];
        camdir[2] = campos[2] - target[2];

        find_surface_on_ray(target, camdir, &surf, &hitpos);

        // what do wit dis
        // gPuppyCam.collisionDistance = sqrtf((target[0] - hitpos[0]) * (target[0] - hitpos[0]) + (target[1] - hitpos[1]) * (target[1] - hitpos[1]) + (target[2] - hitpos[2]) * (target[2] - hitpos[2]));

        if (surf)
        {
            gPuppyCam.pos[0] = (s16) approach_f32_asymptotic(gPuppyCam.pos[0], hitpos[0], lerp_pow);
            gPuppyCam.pos[1] = (s16) approach_f32_asymptotic(gPuppyCam.pos[1], hitpos[1], lerp_pow);
            gPuppyCam.pos[2] = (s16) approach_f32_asymptotic(gPuppyCam.pos[2], hitpos[2], lerp_pow);
        } else {
            gPuppyCam.pos[0] = (s16) approach_f32_asymptotic(gPuppyCam.pos[0], campos[0], lerp_pow);
            gPuppyCam.pos[1] = (s16) approach_f32_asymptotic(gPuppyCam.pos[1], campos[1], lerp_pow);
            gPuppyCam.pos[2] = (s16) approach_f32_asymptotic(gPuppyCam.pos[2], campos[2], lerp_pow);
        }
        gPuppyCam.focus[0] = (s16) approach_f32_asymptotic(gPuppyCam.focus[0], target[0], lerp_pow);
        gPuppyCam.focus[1] = (s16) approach_f32_asymptotic(gPuppyCam.focus[1], target[1], lerp_pow);
        gPuppyCam.focus[2] = (s16) approach_f32_asymptotic(gPuppyCam.focus[2], target[2], lerp_pow);
    }
}

void slide_cutscene(void) {
    // uhh
    if (sSavedZoomSet == 4) sSavedZoomSet = gPuppyCam.zoomSet;

    gCloseClip = TRUE;
    sCameraWallOffset = approach_f32_asymptotic(sCameraWallOffset, 100.0f, 0.15f);
}

void towerclimb_cutscene(void) {
    // gPuppyCam.yaw = (s16) approach_f32_asymptotic(
    //     gPuppyCam.yaw,
    //     (s16) obj_angle_to_object(gPuppyCam.targetObj2, gPuppyCam.targetObj) + gPuppyCam.yawTarget,
    //     0.2f
    // );
    // gPuppyCam.yaw = gPuppyCam.yawTarget - approach_f32_asymptotic((s16)(gPuppyCam.yawTarget - gPuppyCam.yaw), 0, 0.2f);
    s16 newTarget = (s16) obj_angle_to_object(gPuppyCam.targetObj2, gPuppyCam.targetObj) + gPuppyCam.yawTarget;
    if (!handle_wall_bonk_cam()) {
        gPuppyCam.yaw = newTarget - approach_f32_asymptotic(
            (s16) (newTarget - gPuppyCam.yaw),
            0,
            0.1f
        );
    }
}

#define TEMPLE_X_START_OFFSET 2500.0f
#define TEMPLE_Y_START_OFFSET 1000.0f

void temple_intro_cutscene(void) {
    Vec3f pos;
    Vec3f focus;

    sCutsceneVp.vp.vscale[0] = 640;
    sCutsceneVp.vp.vscale[1] = 360;
    sCutsceneVp.vp.vtrans[0] = 640;
    sCutsceneVp.vp.vtrans[1] = 480;
    override_viewport_and_clip(NULL, &sCutsceneVp, 0, 0, 0);

#ifdef CDEBUG
    if (
        gCurCutsceneTimer < TEMPLE_INTRO_FINAL_FRAME - 10 &&
        gPlayer1Controller->buttonDown & START_BUTTON &&
        gPlayer1Controller->buttonPressed & A_BUTTON
    ) {
        gCurCutsceneTimer = TEMPLE_INTRO_FINAL_FRAME - 5;
        gMarioState->pos[0] = -1558.0f;
    }
#endif

    if (gCurCutsceneTimer == 0) {
        set_mario_action(gMarioState, ACT_TEMPLE_1_INTRO, 0);
        gMarioState->pos[0] -= 5000.0f;
        gMarioState->pos[1] += 100.0f;
        set_fov_function(CAM_FOV_SET_45);
    }
    else if (gCurCutsceneTimer >= TEMPLE_INTRO_FINAL_FRAME) {
        set_current_cutscene(NO_CUTSCENE);
        set_mario_action(gMarioState, ACT_IDLE, 2);

        set_fov_function(CAM_FOV_DEFAULT);
        gMarioState->pos[0] += 200.0f;
        gMarioState->marioObj->header.gfx.pos[0] = gMarioState->pos[0];
        gMarioState->faceAngle[1] = DEGREES(90);
        gMarioState->marioObj->header.gfx.angle[1] = gMarioState->faceAngle[1];
        gPuppyCam.yaw = gMarioState->faceAngle[1] + 0x8000;
        gPuppyCam.yawTarget = gPuppyCam.yaw;
        gPuppyCam.pos[0] = -2161;
        gPuppyCam.pos[1] = 138;
        gPuppyCam.pos[2] = -8;
        gPuppyCam.focus[0] = -1164;
        gPuppyCam.focus[1] = -124;
        gPuppyCam.focus[2] = -8;
        return;
    }
    else if (gCurCutsceneTimer == TEMPLE_INTRO_DOOR_SLAM_SHOT_START - 1) {
        gMarioState->pos[0] = -1558.0f;
    }

    vec3s_copy(gPuppyCam.pos, temple_intro[gCurCutsceneTimer][0]);
    vec3s_copy(gPuppyCam.focus, temple_intro[gCurCutsceneTimer][1]);
}

void camping_intro_cutscene(void) {
    Vec3f pos;
    Vec3f focus;

    sCutsceneVp.vp.vscale[0] = 640;
    sCutsceneVp.vp.vscale[1] = 360;
    sCutsceneVp.vp.vtrans[0] = 640;
    sCutsceneVp.vp.vtrans[1] = 480;
    override_viewport_and_clip(NULL, &sCutsceneVp, 0, 0, 0);
    gCloseClip = TRUE;

#ifdef CDEBUG
    if (
        gCurCutsceneTimer < CUTSCENE_INTRO_LUCY_WALKS_OVER - 10 &&
        gPlayer1Controller->buttonDown & START_BUTTON &&
        gPlayer1Controller->buttonPressed & A_BUTTON
    ) {
        gCurCutsceneTimer = CUTSCENE_INTRO_LUCY_WALKS_OVER - 10;
        gMarioState->pos[1] += 100.0f;
        gMarioState->actionState = 3;
    }
#endif

    if (gCurCutsceneTimer == CUTSCENE_INTRO_END - 1) {
        gPuppyCam.yaw = gMarioState->faceAngle[1] + 0x8000;
        gPuppyCam.yawTarget = gPuppyCam.yaw;
        vec3s_copy(gPuppyCam.pos, camping_intro[gCurCutsceneTimer][0]);
        vec3s_copy(gPuppyCam.focus, camping_intro[gCurCutsceneTimer][1]);
    } else if (gCurCutsceneTimer >= CUTSCENE_INTRO_END) {
        gPuppyCam.yaw = gMarioState->faceAngle[1] + 0x8000;
        gPuppyCam.yawTarget = gPuppyCam.yaw;
        gCloseClip = FALSE;
    } else {
        vec3s_copy(gPuppyCam.pos, camping_intro[gCurCutsceneTimer][0]);
        vec3s_copy(gPuppyCam.focus, camping_intro[gCurCutsceneTimer][1]);
    }

}

void orb_reveal_cutscene(void) {
    sCutsceneVp.vp.vscale[0] = 640;
    sCutsceneVp.vp.vscale[1] = 360;
    sCutsceneVp.vp.vtrans[0] = 640;
    sCutsceneVp.vp.vtrans[1] = 480;
    override_viewport_and_clip(NULL, &sCutsceneVp, 0, 0, 0);
    gCloseClip = 2;

// #ifdef CDEBUG
//     if (
//         gCurCutsceneTimer < CUTSCENE_INTRO_LUCY_WALKS_OVER - 10 &&
//         gPlayer1Controller->buttonDown & START_BUTTON &&
//         gPlayer1Controller->buttonPressed & A_BUTTON
//     ) {
//         gCurCutsceneTimer = CUTSCENE_INTRO_LUCY_WALKS_OVER - 10;
//         gMarioState->pos[1] += 100.0f;
//         gMarioState->actionState = 3;
//     }
// #endif

    if (gCurCutsceneTimer == ORB_REVEAL_BEHIND_LUCY) {
        gCloseClip = 0;
        set_fov_function(CAM_FOV_SET_CUSTOM);
        gCustomFOV = FOV_24MM;
    } else if (gCurCutsceneTimer >= ORB_REVEAL_INSIDE_ORB) {
        gCustomFOV = get_relative_position_between_ranges(
            gCurCutsceneTimer,
            ORB_REVEAL_INSIDE_ORB,
            ORB_REVEAL_FINAL_FRAME,
            FOV_100MM,
            FOV_120MM
        );
    } else if (gCurCutsceneTimer > ORB_REVEAL_BEHIND_LUCY) {
        gCloseClip = 0;
        set_fov_function(CAM_FOV_SET_CUSTOM);
        gCustomFOV = get_relative_position_between_ranges(
            gCurCutsceneTimer,
            ORB_REVEAL_BEHIND_LUCY,
            ORB_REVEAL_INSIDE_ORB,
            FOV_35MM,
            FOV_50MM
            // FOV_24MM,
            // FOV_35MM
        );
    }

    if (gCurCutsceneTimer == ORB_REVEAL_FINAL_FRAME - 1) {
        gPuppyCam.yaw = gMarioState->faceAngle[1] + 0x8000;
        gPuppyCam.yawTarget = gPuppyCam.yaw;
        vec3s_copy(gPuppyCam.pos, final_orb_reveal[gCurCutsceneTimer][0]);
        vec3s_copy(gPuppyCam.focus, final_orb_reveal[gCurCutsceneTimer][1]);
    } else if (gCurCutsceneTimer >= ORB_REVEAL_FINAL_FRAME) {
        gPuppyCam.yaw = gMarioState->faceAngle[1] + 0x8000;
        gPuppyCam.yawTarget = gPuppyCam.yaw;
        gCloseClip = FALSE;
        set_current_cutscene(NO_CUTSCENE);
        set_fov_45();
        set_fov_function(CAM_FOV_DEFAULT);
    } else {
        vec3s_copy(gPuppyCam.pos, final_orb_reveal[gCurCutsceneTimer][0]);
        vec3s_copy(gPuppyCam.focus, final_orb_reveal[gCurCutsceneTimer][1]);
        gPuppyCam.yaw = calculate_yaws(gPuppyCam.pos, gPuppyCam.focus);
    }
}

void lucys_levitation_cutscene(void) {
    sCutsceneVp.vp.vscale[0] = 640;
    sCutsceneVp.vp.vscale[1] = 360;
    sCutsceneVp.vp.vtrans[0] = 640;
    sCutsceneVp.vp.vtrans[1] = 480;
    override_viewport_and_clip(NULL, &sCutsceneVp, 0, 0, 0);
    set_fov_function(CAM_FOV_SET_CUSTOM);
    gCustomFOV = FOV_150MM;
    gCloseClip = 2;
    // gCloseClip = 2;


    // if (gCurCutsceneTimer == ORB_REVEAL_FINAL_FRAME - 1) {
    //     gPuppyCam.yaw = gMarioState->faceAngle[1] + 0x8000;
    //     gPuppyCam.yawTarget = gPuppyCam.yaw;
    //     vec3s_copy(gPuppyCam.pos, final_orb_reveal[gCurCutsceneTimer][0]);
    //     vec3s_copy(gPuppyCam.focus, final_orb_reveal[gCurCutsceneTimer][1]);
    // } else if (gCurCutsceneTimer >= ORB_REVEAL_FINAL_FRAME) {
    //     gPuppyCam.yaw = gMarioState->faceAngle[1] + 0x8000;
    //     gPuppyCam.yawTarget = gPuppyCam.yaw;
    //     gCloseClip = FALSE;
    //     set_current_cutscene(NO_CUTSCENE);
    //     set_fov_45();
    //     set_fov_function(CAM_FOV_DEFAULT);
    // } else {
        // vec3s_copy(gPuppyCam.pos, final_orb_reveal[gCurCutsceneTimer][0]);
        // vec3s_copy(gPuppyCam.focus, final_orb_reveal[gCurCutsceneTimer][1]);
        // gPuppyCam.yaw = calculate_yaws(gPuppyCam.pos, gPuppyCam.focus);
    // }
    // vec3s_copy(gPuppyCam.focus, gMarioState->pos);
    vec3s_copy(gPuppyCam.pos, final_orb_reveal[109][0]);
    // vec3s_copy(gPuppyCam.focus, final_orb_reveal[109][1]);
    gPuppyCam.focus[0] = (s16)gFinalOrbPos[0];
    gPuppyCam.focus[1] = (s16)gFinalOrbPos[1];
    gPuppyCam.focus[2] = (s16)gFinalOrbPos[2];
    gPuppyCam.yaw = calculate_yaws(gPuppyCam.pos, gPuppyCam.focus);
    gPuppyCam.yawTarget = gPuppyCam.yaw;
    gPuppyCam.flags &= ~PUPPYCAM_BEHAVIOUR_COLLISION;
}

void puppycam_handle_cutscene(void) {
    switch (gCurCutscene) {
        case CUTSCENE_DOOR_OPEN:
            door_cutscene();
            break;
        case CUTSCENE_SLIDE:
            slide_cutscene();
            break;
        case CUTSCENE_TOWERCLIMB:
            towerclimb_cutscene();
            break;
        case CUTSCENE_INTRO_TEMPLE:
            temple_intro_cutscene();
            break;
        case CUTSCENE_INTRO:
            camping_intro_cutscene();
            break;
        case CUTSCENE_ORB_REVEAL:
            orb_reveal_cutscene();
            break;
        case CUTSCENE_LUCYS_LEVITATION:
            lucys_levitation_cutscene();
            break;
        default:
            if (sSavedZoomSet != 4) {
                gPuppyCam.zoomSet = sSavedZoomSet;
                sSavedZoomSet = 4;
            }
    }
} 

//Calls any scripts to affect the camera, if applicable.
static void puppycam_script(void)
{
    u16 i = 0;
    struct sPuppyVolume volume;
    void (*func)();

    if (gCurCutscene) puppycam_handle_cutscene();
    else sCameraWallOffset = approach_f32_asymptotic(sCameraWallOffset, OFFSET, 0.2f);

    //Sets this before going through any possible modifications.
    gPuppyCam.flags = gPuppyCam.intendedFlags;

    if (gPuppyVolumeCount == 0 || !gPuppyCam.targetObj)
        return;

    for (i = 0; i < gPuppyVolumeCount; i++)
    {
        if (puppycam_check_volume_bounds(&volume, i))
        {
            //First applies pos and focus, for the most basic of volumes.
            if (volume.angles != NULL)
            {
                if (volume.angles->pos[0] != PUPPY_NULL)
                    gPuppyCam.pos[0] = volume.angles->pos[0];
                if (volume.angles->pos[1] != PUPPY_NULL)
                    gPuppyCam.pos[1] = volume.angles->pos[1];
                if (volume.angles->pos[2] != PUPPY_NULL)
                    gPuppyCam.pos[2] = volume.angles->pos[2];

                if (volume.angles->focus[0] != PUPPY_NULL)
                    gPuppyCam.focus[0] = volume.angles->focus[0];
                if (volume.angles->focus[1] != PUPPY_NULL)
                    gPuppyCam.focus[1] = volume.angles->focus[1];
                if (volume.angles->focus[2] != PUPPY_NULL)
                    gPuppyCam.focus[2] = volume.angles->focus[2];

                if (volume.angles->yaw != PUPPY_NULL)
                {
                    gPuppyCam.yawTarget = volume.angles->yaw;
                    gPuppyCam.yaw = volume.angles->yaw;

                    gPuppyCam.flags &= ~PUPPYCAM_BEHAVIOUR_YAW_ROTATION;
                }

                if (volume.angles->pitch != PUPPY_NULL)
                {
                    gPuppyCam.pitchTarget = volume.angles->pitch;
                    gPuppyCam.pitch = volume.angles->pitch;

                    gPuppyCam.flags &= ~PUPPYCAM_BEHAVIOUR_PITCH_ROTATION;
                }

                if (volume.angles->zoom != PUPPY_NULL)
                {
                    gPuppyCam.zoomTarget = volume.angles->zoom;
                    gPuppyCam.zoom = gPuppyCam.zoomTarget;

                    gPuppyCam.flags &= ~PUPPYCAM_BEHAVIOUR_ZOOM_CHANGE;
                }
            }

            //Adds and removes behaviour flags, as set.
            if (volume.flagsRemove != NULL)
                gPuppyCam.flags &= ~volume.flagsRemove;
            if (volume.flagsAdd != NULL)
                gPuppyCam.flags |= volume.flagsAdd;
            if (volume.flagPersistance == PUPPYCAM_BEHAVIOUR_PERMANENT)
            {
                //Adds and removes behaviour flags, as set.
                if (volume.flagsRemove != NULL)
                    gPuppyCam.intendedFlags &= ~volume.flagsRemove;
                if (volume.flagsAdd != NULL)
                    gPuppyCam.intendedFlags |= volume.flagsAdd;
            }

            //Last and probably least, check if there's a function attached, and call it, if so.
            if (volume.func != NULL)
            {
                func = volume.func;
                (func)();
            }
        }
    }
}

//Handles collision detection using ray casting.
// static s32 puppycam_collision_test(s16 offsetX, s16 offsetY, s16 offsetZ, s16 tOffsetY, Vec3s curPos)
// {
//     struct Surface *surf;
//     Vec3f camdir;
//     Vec3f hitpos;
//     Vec3f target;
//     Vec3s pPos;
//     s16 colDist;

//     vec3s_set(pPos, offsetX, offsetY, offsetZ);
//     vec3s_add(pPos, curPos);

//     target[0] = gPuppyCam.targetObj->oPosX;
//     target[1] = gPuppyCam.targetObj->oPosY + tOffsetY;
//     target[2] = gPuppyCam.targetObj->oPosZ;

//     camdir[0] = pPos[0] - target[0];
//     camdir[1] = pPos[1] - target[1];
//     camdir[2] = pPos[2] - target[2];

//     find_surface_on_ray(target, camdir, &surf, &hitpos);
//     colDist = vec3f_dist(target, hitpos);

//     if (!surf)
//     {
//         gPuppyCam.pos[0] = pPos[0];
//         gPuppyCam.pos[1] = pPos[1];
//         gPuppyCam.pos[2] = pPos[2];
//         return 0;
//     }
//     return colDist;
// }

static s16 get_target_y_offset(void) {
    struct Surface *surf;
    Vec3f posDir;
    Vec3f hitpos;
    Vec3f fromPos;
    Vec3f toPos;
    s32 t1, t2;

    fromPos[0] = gPuppyCam.targetObj->oPosX;
    fromPos[1] = gPuppyCam.targetObj->oPosY;
    fromPos[2] = gPuppyCam.targetObj->oPosZ;

    toPos[0] = gPuppyCam.targetObj->oPosX;
    toPos[1] = gPuppyCam.targetObj->oPosY + gPuppyCam.povHeight;
    toPos[2] = gPuppyCam.targetObj->oPosZ;

    posDir[0] = fromPos[0] - toPos[0];
    posDir[1] = fromPos[1] - toPos[1];
    posDir[2] = fromPos[2] - toPos[2];

    find_surface_on_ray(fromPos, posDir, &surf, &hitpos);
 
    if (surf) return toPos[1] - hitpos[1];
    return gPuppyCam.povHeight;
}

//Handles collision detection using ray casting.
static void puppycam_collision(void)
{
    struct Surface *surf;
    struct Surface *surfWall;
    Vec3f camdir;
    Vec3f hitpos, hitpos2;
    Vec3f target;
    s32 t1, t2;

    target[0] = gPuppyCam.targetObj->oPosX;
    target[1] = gPuppyCam.targetObj->oPosY + gPuppyCam.povHeight;
    target[2] = gPuppyCam.targetObj->oPosZ;

    if (
        (gMarioState->action & ACT_GROUP_MASK) == ACT_GROUP_SUBMERGED &&
        gMarioState->pos[1] > gMarioState->waterLevel - 150 &&
        gPuppyCam.pos[1] < gCameraWaterLevel + 80 &&
        ABS(gCameraWaterLevel - gMarioState->waterLevel) < 150
    ) {
        // gPuppyCam.pos[1] = approach_f32_asymptotic(target[1], (f32) (gCameraWaterLevel + 400.0f), 0.1f);
        gPuppyCam.pos[1] = approach_f32_asymptotic(gPuppyCam.pos[1], (f32) (gCameraWaterLevel + 400.0f), 0.1f);
        gPuppyCam.pitchTarget = (s16)approach_f32_asymptotic(gPuppyCam.pitchTarget, sZoomPitchTargets[gPuppyCam.zoomSet], 0.2f);
        // gPuppyCam.pitch = (s16)approach_f32_asymptotic(gPuppyCam.pitchTarget, sZoomPitchTargets[gPuppyCam.zoomSet], 0.2f);
    }

    camdir[0] = gPuppyCam.pos[0] - target[0];
    camdir[1] = gPuppyCam.pos[1] - target[1];
    camdir[2] = gPuppyCam.pos[2] - target[2];

    sRayCastOffset = sCameraWallOffset;
    find_surface_on_ray(target, camdir, &surf, &hitpos);
    gPuppyCam.collisionDistance = vec3f_dist(target, hitpos);

    vec3f_copy(hitpos2, hitpos);
    surfWall = resolve_and_return_wall_collisions(hitpos2, 0.0f, sRayCastOffset);
    if (surfWall != NULL) {
        surf = surfWall;
        vec3f_copy(hitpos, hitpos2);
    }

    if (surf)
    {   
        // Vec3f target2;
        // target2[0] = hitpos[0];
        // target2[1] = hitpos[1];
        // target2[2] = hitpos[2];

        // REGULAR
        gPuppyCam.pos[0] = hitpos[0];
        gPuppyCam.pos[1] = hitpos[1];
        gPuppyCam.pos[2] = hitpos[2];

        // triple double check
        // if (gPuppyCam.collisionDistance < 100)
        // {
        //     Vec3f norm;
        //     Vec3f hitpos2;
        //     struct Surface *surf2;
        //     sRayCastOffset = 0.0f;
        //     find_surface_on_ray(target, camdir, &surf2, &hitpos2);

        //     gPuppyCam.collisionDistance = vec3f_dist(target, hitpos2);
        //     if (surf2)
        //     {
        //         norm[0] = surf->normal.x;
        //         norm[1] = surf->normal.y;
        //         norm[2] = surf->normal.z;
        //         vec3f_mul(norm, sCameraWallOffset);
        //         vec3f_add(hitpos2, norm);
        //         gPuppyCam.pos[0] = hitpos2[0];
        //         gPuppyCam.pos[1] = hitpos2[1];
        //         gPuppyCam.pos[2] = hitpos2[2];
        //     }
        // }
        // else
        // {
        //     gPuppyCam.pos[0] = hitpos[0];
        //     gPuppyCam.pos[1] = hitpos[1];
        //     gPuppyCam.pos[2] = hitpos[2];
        // }
    }

    gPuppyCam.opacity = 255;

    if (gCurCutscene != CUTSCENE_SLIDE && gPuppyCam.collisionDistance <= 250 && (gPuppyCam.collisionDistance-150)*2.55 < 255)
    {
        if ((gPuppyCam.collisionDistance-150)*2.55 > 0)
            gPuppyCam.opacity = (gPuppyCam.collisionDistance-150)*2.55;
        else
            gPuppyCam.opacity = 0;
    }
    else
        gPuppyCam.opacity = 255;
}

//Applies the PuppyCam values to the actual game's camera, giving the final product.
static void puppycam_apply(void)
{
    gLakituState.pos[0] = gPuppyCam.pos[0];
    gLakituState.pos[1] = gPuppyCam.pos[1];
    gLakituState.pos[2] = gPuppyCam.pos[2];

    gLakituState.focus[0] = gPuppyCam.focus[0];
    gLakituState.focus[1] = gPuppyCam.focus[1];
    gLakituState.focus[2] = gPuppyCam.focus[2];

    gCamera->pos[0] = gPuppyCam.pos[0];
    gCamera->pos[1] = gPuppyCam.pos[1];
    gCamera->pos[2] = gPuppyCam.pos[2];

    gCamera->focus[0] = gPuppyCam.focus[0];
    gCamera->focus[1] = gPuppyCam.focus[1];
    gCamera->focus[2] = gPuppyCam.focus[2];

    gCamera->yaw = gPuppyCam.yaw;
}

//The basic loop sequence, which is called outside.
void puppycam_loop(void)
{
    puppycam_input_core();
    puppycam_projection();
    if (gPuppyCam.flags & PUPPYCAM_BEHAVIOUR_COLLISION)
        puppycam_collision();
    else
        gPuppyCam.opacity = 255;
    puppycam_script();

    puppycam_apply();
}

