void scroll_pss_dl_dehwater_001_mesh_layer_5_vtx_0() {
	int i = 0;
	int count = 7;
	int width = 64 * 0x20;
	int height = 64 * 0x20;

	static int currentY = 0;
	int deltaY;
	Vtx *vertices = segmented_to_virtual(pss_dl_dehwater_001_mesh_layer_5_vtx_0);

	deltaY = (int)(-0.15000000596046448 * 0x20) % height;

	if (absi(currentY) > height) {
		deltaY -= (int)(absi(currentY) / height) * height * signum_positive(deltaY);
	}

	for (i = 0; i < count; i++) {
		vertices[i].n.tc[1] += deltaY;
	}
	currentY += deltaY;
}

void scroll_sts_mat_pss_dl_waterg_layer5() {
	Gfx *mat = segmented_to_virtual(mat_pss_dl_waterg_layer5);
	shift_s_down(mat, 13, PACK_TILESIZE(0, 1));
	shift_t(mat, 13, PACK_TILESIZE(0, 1));
	shift_s(mat, 21, PACK_TILESIZE(0, 1));
	shift_t(mat, 21, PACK_TILESIZE(0, 1));
};

void scroll_pss_dl_dehwater_mesh_layer_5_vtx_0() {
	int i = 0;
	int count = 73;
	int width = 64 * 0x20;
	int height = 64 * 0x20;

	static int currentY = 0;
	int deltaY;
	Vtx *vertices = segmented_to_virtual(pss_dl_dehwater_mesh_layer_5_vtx_0);

	deltaY = (int)(-0.15000000596046448 * 0x20) % height;

	if (absi(currentY) > height) {
		deltaY -= (int)(absi(currentY) / height) * height * signum_positive(deltaY);
	}

	for (i = 0; i < count; i++) {
		vertices[i].n.tc[1] += deltaY;
	}
	currentY += deltaY;
}

void scroll_pss() {
	scroll_pss_dl_dehwater_001_mesh_layer_5_vtx_0();
	scroll_pss_dl_dehwater_mesh_layer_5_vtx_0();
	scroll_sts_mat_pss_dl_waterg_layer5();
}
