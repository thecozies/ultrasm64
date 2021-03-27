void scroll_cloud_dome_cloud_dome_mesh_layer_5_vtx_0() {
	int i = 0;
	int count = 17;
	int width = 64 * 0x20;
	int height = 16 * 0x20;

	static int currentX = 0;
	int deltaX;
	Vtx *vertices = segmented_to_virtual(cloud_dome_cloud_dome_mesh_layer_5_vtx_0);

	deltaX = (int)(0.15000000596046448 * 0x20) % width;

	if (absi(currentX) > width) {
		deltaX -= (int)(absi(currentX) / width) * width * signum_positive(deltaX);
	}

	for (i = 0; i < count; i++) {
		vertices[i].n.tc[0] += deltaX;
	}
	currentX += deltaX;
}

void scroll_sts_mat_cloud_dome_cloud_material_layer5() {
	Gfx *mat = segmented_to_virtual(mat_cloud_dome_cloud_material_layer5);
	shift_t_down(mat, 22, PACK_TILESIZE(0, 1));
};

void scroll_castle_grounds_level_geo_cloud_dome() {
	scroll_cloud_dome_cloud_dome_mesh_layer_5_vtx_0();
	scroll_sts_mat_cloud_dome_cloud_material_layer5();
}
