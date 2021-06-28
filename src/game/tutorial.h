#ifndef TUTORIAL_H
#define TUTORIAL_H

extern s32 render_tutorial(s32 onPause);
extern void next_tip(void);
extern void prev_tip(void);
extern void reset_tutorial(void);
extern void render_pause_hint_text(void);
extern void render_ring_tip(void);
extern s32 gTutorialDone;

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

#endif
