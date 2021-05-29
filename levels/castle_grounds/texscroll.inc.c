void scroll_castle_grounds_dl_awaters_mesh_layer_5_vtx_0() {
	int i = 0;
	int count = 15;
	int width = 64 * 0x20;
	int height = 64 * 0x20;

	static int currentY = 0;
	int deltaY;
	Vtx *vertices = segmented_to_virtual(castle_grounds_dl_awaters_mesh_layer_5_vtx_0);

	deltaY = (int)(-0.15000000596046448 * 0x20) % height;

	if (absi(currentY) > height) {
		deltaY -= (int)(absi(currentY) / height) * height * signum_positive(deltaY);
	}

	for (i = 0; i < count; i++) {
		vertices[i].n.tc[1] += deltaY;
	}
	currentY += deltaY;
}

void scroll_sts_mat_castle_grounds_dl_water_layer5() {
	Gfx *mat = segmented_to_virtual(mat_castle_grounds_dl_water_layer5);
	shift_s_down(mat, 13, PACK_TILESIZE(0, 1));
	shift_s(mat, 21, PACK_TILESIZE(0, 1));
	shift_t(mat, 21, PACK_TILESIZE(0, 1));
};

void scroll_castle_grounds_dl_awaters_001_mesh_layer_5_vtx_0() {
	int i = 0;
	int count = 30;
	int width = 64 * 0x20;
	int height = 64 * 0x20;

	static int currentY = 0;
	int deltaY;
	Vtx *vertices = segmented_to_virtual(castle_grounds_dl_awaters_001_mesh_layer_5_vtx_0);

	deltaY = (int)(-0.15000000596046448 * 0x20) % height;

	if (absi(currentY) > height) {
		deltaY -= (int)(absi(currentY) / height) * height * signum_positive(deltaY);
	}

	for (i = 0; i < count; i++) {
		vertices[i].n.tc[1] += deltaY;
	}
	currentY += deltaY;
}

void scroll_castle_grounds_dl_awaters_002_mesh_layer_5_vtx_0() {
	int i = 0;
	int count = 4;
	int width = 64 * 0x20;
	int height = 64 * 0x20;

	static int currentY = 0;
	int deltaY;
	Vtx *vertices = segmented_to_virtual(castle_grounds_dl_awaters_002_mesh_layer_5_vtx_0);

	deltaY = (int)(-0.15000000596046448 * 0x20) % height;

	if (absi(currentY) > height) {
		deltaY -= (int)(absi(currentY) / height) * height * signum_positive(deltaY);
	}

	for (i = 0; i < count; i++) {
		vertices[i].n.tc[1] += deltaY;
	}
	currentY += deltaY;
}

void scroll_castle_grounds_dl_awaters_green_mesh_layer_5_vtx_0() {
	int i = 0;
	int count = 12;
	int width = 64 * 0x20;
	int height = 64 * 0x20;

	static int currentY = 0;
	int deltaY;
	Vtx *vertices = segmented_to_virtual(castle_grounds_dl_awaters_green_mesh_layer_5_vtx_0);

	deltaY = (int)(-0.15000000596046448 * 0x20) % height;

	if (absi(currentY) > height) {
		deltaY -= (int)(absi(currentY) / height) * height * signum_positive(deltaY);
	}

	for (i = 0; i < count; i++) {
		vertices[i].n.tc[1] += deltaY;
	}
	currentY += deltaY;
}

void scroll_castle_grounds_dl_awaters_004_mesh_layer_5_vtx_0() {
	int i = 0;
	int count = 4;
	int width = 64 * 0x20;
	int height = 64 * 0x20;

	static int currentY = 0;
	int deltaY;
	Vtx *vertices = segmented_to_virtual(castle_grounds_dl_awaters_004_mesh_layer_5_vtx_0);

	deltaY = (int)(-0.15000000596046448 * 0x20) % height;

	if (absi(currentY) > height) {
		deltaY -= (int)(absi(currentY) / height) * height * signum_positive(deltaY);
	}

	for (i = 0; i < count; i++) {
		vertices[i].n.tc[1] += deltaY;
	}
	currentY += deltaY;
}

void scroll_sts_mat_castle_grounds_dl_trippy_warp_brick_layer1() {
	Gfx *mat = segmented_to_virtual(mat_castle_grounds_dl_trippy_warp_brick_layer1);
	shift_s(mat, 14, PACK_TILESIZE(0, 1));
	shift_t_down(mat, 14, PACK_TILESIZE(0, 1));
};

void scroll_castle_grounds_dl_awaters_003_mesh_layer_5_vtx_0() {
	int i = 0;
	int count = 4;
	int width = 64 * 0x20;
	int height = 64 * 0x20;

	static int currentY = 0;
	int deltaY;
	Vtx *vertices = segmented_to_virtual(castle_grounds_dl_awaters_003_mesh_layer_5_vtx_0);

	deltaY = (int)(-0.15000000596046448 * 0x20) % height;

	if (absi(currentY) > height) {
		deltaY -= (int)(absi(currentY) / height) * height * signum_positive(deltaY);
	}

	for (i = 0; i < count; i++) {
		vertices[i].n.tc[1] += deltaY;
	}
	currentY += deltaY;
}

void scroll_castle_grounds_dl_afakewater_mesh_layer_5_vtx_0() {
	int i = 0;
	int count = 15;
	int width = 64 * 0x20;
	int height = 64 * 0x20;

	static int currentY = 0;
	int deltaY;
	Vtx *vertices = segmented_to_virtual(castle_grounds_dl_afakewater_mesh_layer_5_vtx_0);

	deltaY = (int)(-0.15000000596046448 * 0x20) % height;

	if (absi(currentY) > height) {
		deltaY -= (int)(absi(currentY) / height) * height * signum_positive(deltaY);
	}

	for (i = 0; i < count; i++) {
		vertices[i].n.tc[1] += deltaY;
	}
	currentY += deltaY;
}

void scroll_castle_grounds_dl_aBlob_001_mesh_layer_5_vtx_0() {
	int i = 0;
	int count = 100;
	int width = 64 * 0x20;
	int height = 64 * 0x20;

	static int currentY = 0;
	int deltaY;
	Vtx *vertices = segmented_to_virtual(castle_grounds_dl_aBlob_001_mesh_layer_5_vtx_0);

	deltaY = (int)(-0.15000000596046448 * 0x20) % height;

	if (absi(currentY) > height) {
		deltaY -= (int)(absi(currentY) / height) * height * signum_positive(deltaY);
	}

	for (i = 0; i < count; i++) {
		vertices[i].n.tc[1] += deltaY;
	}
	currentY += deltaY;
}

void scroll_castle_grounds_dl_awaters_006_mesh_layer_5_vtx_0() {
	int i = 0;
	int count = 30;
	int width = 64 * 0x20;
	int height = 64 * 0x20;

	static int currentY = 0;
	int deltaY;
	Vtx *vertices = segmented_to_virtual(castle_grounds_dl_awaters_006_mesh_layer_5_vtx_0);

	deltaY = (int)(-0.15000000596046448 * 0x20) % height;

	if (absi(currentY) > height) {
		deltaY -= (int)(absi(currentY) / height) * height * signum_positive(deltaY);
	}

	for (i = 0; i < count; i++) {
		vertices[i].n.tc[1] += deltaY;
	}
	currentY += deltaY;
}

void scroll_castle_grounds_dl_aBlob_mesh_layer_5_vtx_0() {
	int i = 0;
	int count = 138;
	int width = 64 * 0x20;
	int height = 64 * 0x20;

	static int currentY = 0;
	int deltaY;
	Vtx *vertices = segmented_to_virtual(castle_grounds_dl_aBlob_mesh_layer_5_vtx_0);

	deltaY = (int)(-0.15000000596046448 * 0x20) % height;

	if (absi(currentY) > height) {
		deltaY -= (int)(absi(currentY) / height) * height * signum_positive(deltaY);
	}

	for (i = 0; i < count; i++) {
		vertices[i].n.tc[1] += deltaY;
	}
	currentY += deltaY;
}

void scroll_castle_grounds_dl_apurpwaters_007_mesh_layer_5_vtx_0() {
	int i = 0;
	int count = 12;
	int width = 64 * 0x20;
	int height = 64 * 0x20;

	static int currentY = 0;
	int deltaY;
	Vtx *vertices = segmented_to_virtual(castle_grounds_dl_apurpwaters_007_mesh_layer_5_vtx_0);

	deltaY = (int)(-0.15000000596046448 * 0x20) % height;

	if (absi(currentY) > height) {
		deltaY -= (int)(absi(currentY) / height) * height * signum_positive(deltaY);
	}

	for (i = 0; i < count; i++) {
		vertices[i].n.tc[1] += deltaY;
	}
	currentY += deltaY;
}

void scroll_castle_grounds_dl_apurpwaters_001_mesh_layer_5_vtx_0() {
	int i = 0;
	int count = 298;
	int width = 64 * 0x20;
	int height = 64 * 0x20;

	static int currentY = 0;
	int deltaY;
	Vtx *vertices = segmented_to_virtual(castle_grounds_dl_apurpwaters_001_mesh_layer_5_vtx_0);

	deltaY = (int)(-0.15000000596046448 * 0x20) % height;

	if (absi(currentY) > height) {
		deltaY -= (int)(absi(currentY) / height) * height * signum_positive(deltaY);
	}

	for (i = 0; i < count; i++) {
		vertices[i].n.tc[1] += deltaY;
	}
	currentY += deltaY;
}

void scroll_castle_grounds() {
	scroll_sts_mat_castle_grounds_dl_water_layer5();
	scroll_sts_mat_castle_grounds_dl_trippy_warp_brick_layer1();
	scroll_castle_grounds_dl_awaters_mesh_layer_5_vtx_0();
	scroll_castle_grounds_dl_awaters_001_mesh_layer_5_vtx_0();
	scroll_castle_grounds_dl_awaters_002_mesh_layer_5_vtx_0();
	scroll_castle_grounds_dl_awaters_green_mesh_layer_5_vtx_0();
	scroll_castle_grounds_dl_awaters_004_mesh_layer_5_vtx_0();
	scroll_castle_grounds_dl_awaters_003_mesh_layer_5_vtx_0();
	scroll_castle_grounds_dl_afakewater_mesh_layer_5_vtx_0();
	scroll_castle_grounds_dl_aBlob_001_mesh_layer_5_vtx_0();
	scroll_castle_grounds_dl_awaters_006_mesh_layer_5_vtx_0();
	scroll_castle_grounds_dl_aBlob_mesh_layer_5_vtx_0();
	scroll_castle_grounds_dl_apurpwaters_007_mesh_layer_5_vtx_0();
	scroll_castle_grounds_dl_apurpwaters_001_mesh_layer_5_vtx_0();
}
