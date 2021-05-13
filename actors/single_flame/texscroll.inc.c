#include "game/level_update.h"

void scroll_single_flame_fire_flame_mesh_layer_5_vtx_0() {
	int i = 0;
	int count = 9;
	int width = 32 * 0x20;
	int height = 32 * 0x20;
	static int ogHeights[9] = { 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	static int ogWidths[9] = { 0, 0, 0, 0, 0, 0, 0, 0, 0 };

	static int currentX = 0;
	int deltaX;
	Vtx *vertices = segmented_to_virtual(single_flame_fire_flame_mesh_layer_5_vtx_0);

	deltaX = (int)(0.3100000023841858 * 0x20) % width;

	if (absi(currentX) > width) {
		deltaX -= (int)(absi(currentX) / width) * width * signum_positive(deltaX);
	}

	for (i = 0; i < count; i++) {
		vertices[i].n.tc[0] += deltaX;
		if (!ogHeights[i]) ogHeights[i] = vertices[i].n.ob[1];
		if (!ogWidths[i]) ogWidths[i] = vertices[i].n.ob[0];
		vertices[i].n.ob[1] = (f32)ogHeights[i] * (0.5f + ((gFireAlpha) / 80.0f));
		vertices[i].n.ob[0] = (f32)ogWidths[i] * (0.5f + ((gFireAlpha) / 80.0f));
	}
	currentX += deltaX;
}

void scroll_sts_mat_single_flame_fireflame_layer5() {
	Gfx *mat = segmented_to_virtual(mat_single_flame_fireflame_layer5);
	shift_t_down(mat, 13, PACK_TILESIZE(0, 3));
	shift_t_down(mat, 21, PACK_TILESIZE(0, 5));
};

void scroll_actor_geo_single_flame() {
	scroll_single_flame_fire_flame_mesh_layer_5_vtx_0();
	scroll_sts_mat_single_flame_fireflame_layer5();
}
