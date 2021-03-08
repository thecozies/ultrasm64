void scroll_castle_grounds_dl_Plane_mesh_layer_5_vtx_0() {
	int i = 0;
	int count = 4;
	int width = 32 * 0x20;
	int height = 32 * 0x20;

	static int currentY = 0;
	int deltaY;
	Vtx *vertices = segmented_to_virtual(castle_grounds_dl_Plane_mesh_layer_5_vtx_0);

	deltaY = (int)(-0.15000000596046448 * 0x20) % height;

	if (absi(currentY) > height) {
		deltaY -= (int)(absi(currentY) / height) * height * signum_positive(deltaY);
	}

	for (i = 0; i < count; i++) {
		vertices[i].n.tc[1] += deltaY;
	}
	currentY += deltaY;
}

void scroll_sts_mat_castle_grounds_dl_water_layer5_area1() {
	Gfx *mat = segmented_to_virtual(mat_castle_grounds_dl_water_layer5_area1);
	shift_s_down(mat, 16, PACK_TILESIZE(0, 1));
	shift_s(mat, 24, PACK_TILESIZE(0, 1));
	shift_t(mat, 24, PACK_TILESIZE(0, 1));
};

void scroll_castle_grounds() {
	scroll_castle_grounds_dl_Plane_mesh_layer_5_vtx_0();
	scroll_sts_mat_castle_grounds_dl_water_layer5_area1();
}
