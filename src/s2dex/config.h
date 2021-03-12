#ifndef S2D_CONFIG_H
#define S2D_CONFIG_H

#define const

// include your font header here
#include "billy.h"

// get your font symbols defined here
#define s2d_font            billy_obj
#define s2d_font_dropshadow billy_obj_dropshadow
#define s2d_tex             billy_tex
#define s2d_kerning_table   billy_kerning_table
extern char s2d_kerning_table[];

// Debug: bypass custom matrix setup
#define s2d_mat billy_mtx

// Homebrew users: use this to rename your gdl head
#define gdl_head gDisplayListHead
extern Gfx *gdl_head;


// Other games/Homebrew users: change these values

// an allocator function of the format void *alloc(size_t bytes)
#define alloc alloc_display_list

// your init functions for the RDP/RSP
#define my_rdp_init my_rdp_init
#define my_rsp_init my_rsp_init

// The frame timer that is used to time s2d_type_print
#define s2d_timer gGlobalTimer
extern u32 s2d_timer;

// The equivalent vsprintf in your game (defaults to libultra _Printf)
#define vsprintf(dst, str, fmt, args) _Printf(proutSprintf, dst, fmt, args)
extern char *proutSprintf(char *dst, const char *src, size_t count);

// Set these to optimize s2d texture "cache"
#define TEX_WIDTH 32
#define TEX_HEIGHT 32
#define TEX_BITDEPTH 8

// tab glyph width and height
#define TAB_WIDTH_H TEX_WIDTH * 2
#define TAB_WIDTH_V TEX_WIDTH / 2

// Texture resolution (pixels on the texture per pixel on the framebuffer)
#define TEX_RES 1

#define _NUM_CACHE (4096 / (TEX_WIDTH * TEX_HEIGHT * (TEX_BITDEPTH / 8)))

#endif