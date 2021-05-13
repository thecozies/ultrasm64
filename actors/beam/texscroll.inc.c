void scroll_beam_beam_mesh_layer_5_vtx_0() {
	int i = 0;
	int count = 89;
	int width = 64 * 0x20;
	int height = 64 * 0x20;

	static int currentX = 0;
	int deltaX;
	static int timeX;
	float amplitudeX = 0.05000000074505806;
	float frequencyX = 0.10000000149011612;
	float offsetX = 0.0;
	Vtx *vertices = segmented_to_virtual(beam_beam_mesh_layer_5_vtx_0);

	deltaX = (int)(amplitudeX * frequencyX * coss((frequencyX * timeX + offsetX) * (1024 * 16 - 1) / 6.28318530718) * 0x20);

	if (absi(currentX) > width) {
		deltaX -= (int)(absi(currentX) / width) * width * signum_positive(deltaX);
	}

	for (i = 0; i < count; i++) {
		vertices[i].n.tc[0] += deltaX;
	}
	currentX += deltaX;	timeX += 1;
}

void scroll_sts_mat_beam_inner_beam_layer5() {
	Gfx *mat = segmented_to_virtual(mat_beam_inner_beam_layer5);
	shift_s(mat, 14, PACK_TILESIZE(0, 1));
	shift_t(mat, 14, PACK_TILESIZE(0, 1));
	shift_t_down(mat, 22, PACK_TILESIZE(0, 1));
};

void scroll_actor_geo_beam() {
	scroll_beam_beam_mesh_layer_5_vtx_0();
	scroll_sts_mat_beam_inner_beam_layer5();
}
