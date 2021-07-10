#include "config.h"

#include <ultra64.h>
#include "mtx.h"

#include "s2d_draw.h"
#include "game/rendering_graph_node.h"

#define s(sprite) ((uObjSprite *)seg2virt(&sprite))
#define t(texture) ((uObjTxtr *)seg2virt(&texture))

void f3d_rdp_init(void) {
    create_dl_ortho_matrix();
    // create_dl_scale_matrix(MENU_MTX_NOPUSH, 1.0f, 1.0f, 1.0f);
    gDPPipeSync(gdl_head++);
    gDPSetTextureFilter(gdl_head++, G_TF_POINT);
    gDPSetCycleType(gdl_head++, G_CYC_1CYCLE);
    gSPClearGeometryMode(gDisplayListHead++, G_LIGHTING);
    gSPClearGeometryMode(gDisplayListHead++, G_ZBUFFER);
    // gDPSetRenderMode(gdl_head++, G_RM_XLU_SPRITE, G_RM_XLU_SPRITE2);
    // gDPSetTextureLUT(gdl_head++, G_TT_NONE);
    // gDPSetTexturePersp(gdl_head++, G_TP_NONE);
    gDPSetCombineLERP(gdl_head++,
        0, 0, 0, ENVIRONMENT,
        TEXEL0, 0, ENVIRONMENT, 0,
        0, 0, 0, ENVIRONMENT,
        TEXEL0, 0, ENVIRONMENT, 0
    );
    // gDPSetRenderMode(gDisplayListHead++, G_RM_XLU_SURF, G_RM_XLU_SURF2);
    gDPSetRenderMode(gDisplayListHead++, G_RM_CLD_SURF, G_RM_CLD_SURF2);
    gDPPipeSync(gdl_head++);
}

void setup_f3d_texture(int idx) {
    gDPPipeSync(gdl_head++);


    gDPSetEnvColor(gdl_head++, s2d_red, s2d_green, s2d_blue, s2d_alpha);

    gDPLoadTextureBlock(
        gdl_head++,
        t(s2d_tex[idx])->block.image,
        s(s2d_font)->s.imageFmt,
        G_IM_SIZ_4b,
        s(s2d_font)->s.imageW >> 5,
        s(s2d_font)->s.imageH >> 5,
        s(s2d_font)->s.imagePal,
        G_TX_WRAP,
        G_TX_WRAP,
        G_TX_NOMASK,
        G_TX_NOMASK,
        G_TX_NOLOD,
        G_TX_NOLOD);
    gDPLoadSync(gdl_head++);
}

#define CLAMP_0(x) ((x < 0) ? 0 : x)

void texrect(int x, int y, float scale) {
    #define qs510(n) ((s16)((n)*0x0400))
    gSPScisTextureRectangle (
        gdl_head++,
        x << 2,
        y << 2,
        (u32)(x + ((f32) ((s(s2d_font)->s.imageW >> 5) - 1) * scale)) << 2,
        (u32)(y + ((f32) ((s(s2d_font)->s.imageH >> 5) - 1) * scale)) << 2,
        G_TX_RENDERTILE,
        0,
        0,
        qs510(1.0f / scale), qs510(1.0f / scale));
}

void draw_f3d_dropshadow(char c, int x, int y, uObjMtx *ds) {
    setup_f3d_texture(c);

    if (s2d_red != 0
        && s2d_green != 0
        && s2d_blue != 0
    ) {
        gDPPipeSync(gdl_head++);
        gDPSetEnvColor(gdl_head++,
                   CLAMP_0(s2d_red - 100),
                   CLAMP_0(s2d_green - 100),
                   CLAMP_0(s2d_blue - 100),
                   s2d_alpha);
        texrect(x, y, myScale);
        gDPPipeSync(gdl_head++);
        gDPSetEnvColor(gdl_head++, s2d_red, s2d_green, s2d_blue, s2d_alpha);
    }
}

// void draw_f3d_glyph(char c, int x, int y, uObjMtx *mt) {
//     setup_f3d_texture(c);

//     texrect(x, y, 1.0f);
//     gDPPipeSync(gdl_head++);
// }

void set_up_texture(int idx) {
	gDPPipeSync(gDisplayListHead++);
    gDPSetEnvColor(gdl_head++, s2d_red, s2d_green, s2d_blue, s2d_alpha);
	gSPTexture(gDisplayListHead++, 65535, 65535, 0, 0, 1);
	gDPTileSync(gDisplayListHead++);

	gDPSetTextureImage(gDisplayListHead++, G_IM_FMT_I, G_IM_SIZ_8b, 16, s2d_lut[idx]);

	gDPSetTile(gDisplayListHead++, G_IM_FMT_I, G_IM_SIZ_8b, 2, 0, 7, 0, G_TX_WRAP | G_TX_NOMIRROR, 5, 0, G_TX_WRAP | G_TX_NOMIRROR, 5, 0);
	gDPLoadSync(gDisplayListHead++);
	gDPLoadTile(gDisplayListHead++, 7, 0, 0, 62, 124);
	gDPPipeSync(gDisplayListHead++);
	gDPSetTile(gDisplayListHead++, G_IM_FMT_I, G_IM_SIZ_4b, 2, 0, 0, 0, G_TX_WRAP | G_TX_NOMIRROR, 5, 0, G_TX_WRAP | G_TX_NOMIRROR, 5, 0);
	gDPSetTileSize(gDisplayListHead++, 0, 0, 0, 124, 124);
	gSPEndDisplayList(gDisplayListHead);
}

#define ORTHO_TRI_CENTER_OFFSET 8.0f

void draw_f3d_glyph(char c, int x, int y, uObjMtx *mt) {
    Mtx *matrix = (Mtx *) alloc_display_list(sizeof(Mtx));
    Mtx *scaleMtx = (Mtx *) alloc_display_list(sizeof(Mtx));

    if (matrix == NULL || scaleMtx == NULL) {
        return;
    }

	guTranslate(
		matrix,
		x + (ORTHO_TRI_CENTER_OFFSET * gS2DScale),
		SCREEN_HEIGHT - y - (ORTHO_TRI_CENTER_OFFSET * gS2DScale),
		0.0f
	);

    if (gWidescreen) guScale(scaleMtx, gS2DScale * 0.75f, gS2DScale, 1.0f);
    else guScale(scaleMtx, gS2DScale, gS2DScale, 1.0f);

    gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(matrix), G_MTX_MODELVIEW | G_MTX_MUL | G_MTX_PUSH);
    gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(scaleMtx), G_MTX_MODELVIEW | G_MTX_MUL | G_MTX_NOPUSH);
    gDPSetScissor(gDisplayListHead++, G_SC_NON_INTERLACE, BORDER_HEIGHT, BORDER_HEIGHT, SCREEN_WIDTH - BORDER_HEIGHT, SCREEN_HEIGHT - BORDER_HEIGHT);
    set_up_texture(c);
    gSPDisplayList(gDisplayListHead++, s2d_dl);
    gSPPopMatrix(gDisplayListHead++, G_MTX_MODELVIEW);
    gDPPipeSync(gdl_head++);
}

#undef s
#undef t