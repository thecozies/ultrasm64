void scroll_pss_dl_a_avisualMiddle_mesh_layer_1_vtx_0() {
	int i = 0;
	int count = 24;
	int width = 64 * 0x20;
	int height = 64 * 0x20;

	static int currentX = 0;
	int deltaX;
	static int currentY = 0;
	int deltaY;
	Vtx *vertices = segmented_to_virtual(pss_dl_a_avisualMiddle_mesh_layer_1_vtx_0);

	deltaX = (int)(0.05000000074505806 * 0x20) % width;
	deltaY = (int)(0.05000000074505806 * 0x20) % height;

	if (absi(currentX) > width) {
		deltaX -= (int)(absi(currentX) / width) * width * signum_positive(deltaX);
	}
	if (absi(currentY) > height) {
		deltaY -= (int)(absi(currentY) / height) * height * signum_positive(deltaY);
	}

	for (i = 0; i < count; i++) {
		vertices[i].n.tc[0] += deltaX;
		vertices[i].n.tc[1] += deltaY;
	}
	currentX += deltaX;	currentY += deltaY;
}

void scroll_pss_dl_a_main_mesh_layer_1_vtx_1() {
	int i = 0;
	int count = 64;
	int width = 64 * 0x20;
	int height = 64 * 0x20;

	static int currentX = 0;
	int deltaX;
	static int currentY = 0;
	int deltaY;
	Vtx *vertices = segmented_to_virtual(pss_dl_a_main_mesh_layer_1_vtx_1);

	deltaX = (int)(0.05000000074505806 * 0x20) % width;
	deltaY = (int)(0.05000000074505806 * 0x20) % height;

	if (absi(currentX) > width) {
		deltaX -= (int)(absi(currentX) / width) * width * signum_positive(deltaX);
	}
	if (absi(currentY) > height) {
		deltaY -= (int)(absi(currentY) / height) * height * signum_positive(deltaY);
	}

	for (i = 0; i < count; i++) {
		vertices[i].n.tc[0] += deltaX;
		vertices[i].n.tc[1] += deltaY;
	}
	currentX += deltaX;	currentY += deltaY;
}

void scroll_pss_dl_dehwater_001_mesh_layer_5_vtx_0() {
	int i = 0;
	int count = 12;
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

void scroll_pss_dl_a__dehwater_mesh_layer_5_vtx_0() {
	int i = 0;
	int count = 20;
	int width = 64 * 0x20;
	int height = 64 * 0x20;

	static int currentY = 0;
	int deltaY;
	Vtx *vertices = segmented_to_virtual(pss_dl_a__dehwater_mesh_layer_5_vtx_0);

	deltaY = (int)(-0.15000000596046448 * 0x20) % height;

	if (absi(currentY) > height) {
		deltaY -= (int)(absi(currentY) / height) * height * signum_positive(deltaY);
	}

	for (i = 0; i < count; i++) {
		vertices[i].n.tc[1] += deltaY;
	}
	currentY += deltaY;
}

void scroll_pss_dl_a__dehwater_mesh_layer_1_vtx_0() {
	int i = 0;
	int count = 46;
	int width = 64 * 0x20;
	int height = 64 * 0x20;

	static int currentY = 0;
	int deltaY;
	Vtx *vertices = segmented_to_virtual(pss_dl_a__dehwater_mesh_layer_1_vtx_0);

	deltaY = (int)(-0.15000000596046448 * 0x20) % height;

	if (absi(currentY) > height) {
		deltaY -= (int)(absi(currentY) / height) * height * signum_positive(deltaY);
	}

	for (i = 0; i < count; i++) {
		vertices[i].n.tc[1] += deltaY;
	}
	currentY += deltaY;
}

void scroll_pss_dl_a__dehwater_mesh_layer_1_vtx_1() {
	int i = 0;
	int count = 6;
	int width = 64 * 0x20;
	int height = 16 * 0x20;

	static int currentY = 0;
	int deltaY;
	Vtx *vertices = segmented_to_virtual(pss_dl_a__dehwater_mesh_layer_1_vtx_1);

	deltaY = (int)(-0.15000000596046448 * 0x20) % height;

	if (absi(currentY) > height) {
		deltaY -= (int)(absi(currentY) / height) * height * signum_positive(deltaY);
	}

	for (i = 0; i < count; i++) {
		vertices[i].n.tc[1] += deltaY;
	}
	currentY += deltaY;
}

void scroll_sts_mat_pss_dl_watero_layer1() {
	Gfx *mat = segmented_to_virtual(mat_pss_dl_watero_layer1);
	shift_s_down(mat, 13, PACK_TILESIZE(0, 1));
	shift_t(mat, 13, PACK_TILESIZE(0, 1));
	shift_s(mat, 21, PACK_TILESIZE(0, 1));
	shift_t(mat, 21, PACK_TILESIZE(0, 1));
};

void scroll_sts_mat_pss_dl_mist_layer5() {
	Gfx *mat = segmented_to_virtual(mat_pss_dl_mist_layer5);
	shift_s_down(mat, 13, PACK_TILESIZE(0, 1));
	shift_t(mat, 13, PACK_TILESIZE(0, 1));
	shift_s(mat, 21, PACK_TILESIZE(0, 1));
	shift_t(mat, 21, PACK_TILESIZE(0, 1));
};

void scroll_pss_dl_a_main2_mesh_layer_1_vtx_1() {
	int i = 0;
	int count = 691;
	int width = 64 * 0x20;
	int height = 64 * 0x20;

	static int currentX = 0;
	int deltaX;
	static int currentY = 0;
	int deltaY;
	Vtx *vertices = segmented_to_virtual(pss_dl_a_main2_mesh_layer_1_vtx_1);

	deltaX = (int)(0.05000000074505806 * 0x20) % width;
	deltaY = (int)(0.05000000074505806 * 0x20) % height;

	if (absi(currentX) > width) {
		deltaX -= (int)(absi(currentX) / width) * width * signum_positive(deltaX);
	}
	if (absi(currentY) > height) {
		deltaY -= (int)(absi(currentY) / height) * height * signum_positive(deltaY);
	}

	for (i = 0; i < count; i++) {
		vertices[i].n.tc[0] += deltaX;
		vertices[i].n.tc[1] += deltaY;
	}
	currentX += deltaX;	currentY += deltaY;
}

void scroll_pss() {
	scroll_pss_dl_dehwater_001_mesh_layer_5_vtx_0();
	scroll_sts_mat_pss_dl_waterg_layer5();
	scroll_pss_dl_a__dehwater_mesh_layer_5_vtx_0();
	scroll_pss_dl_a__dehwater_mesh_layer_1_vtx_0();
	scroll_pss_dl_a__dehwater_mesh_layer_1_vtx_1();
	scroll_sts_mat_pss_dl_watero_layer1();
	scroll_sts_mat_pss_dl_mist_layer5();
	if (
		(gCurCutscene == CUTSCENE_LUCYS_LEVITATION)
		&&
		(gCurCutsceneTimer >= LUCYS_LEVITATION_WARPING)
	) {
		scroll_pss_dl_a_avisualMiddle_mesh_layer_1_vtx_0();
		scroll_pss_dl_a_main_mesh_layer_1_vtx_1();
		scroll_pss_dl_a_main2_mesh_layer_1_vtx_1();
	}
}
