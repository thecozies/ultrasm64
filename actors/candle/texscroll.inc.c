void scroll_candle_fire_flame_mesh_layer_5_vtx_0() {
	int i = 0;
	int count = 9;
	int width = 32 * 0x20;
	int height = 32 * 0x20;

	static int currentX = 0;
	int deltaX;
	static int timeX;
	float amplitudeX = 0.15000000596046448;
	float frequencyX = 0.20999999344348907;
	float offsetX = 0.0;
	Vtx *vertices = segmented_to_virtual(candle_fire_flame_mesh_layer_5_vtx_0);

	deltaX = (int)(amplitudeX * frequencyX * coss((frequencyX * timeX + offsetX) * (1024 * 16 - 1) / 6.28318530718) * 0x20);

	if (absi(currentX) > width) {
		deltaX -= (int)(absi(currentX) / width) * width * signum_positive(deltaX);
	}

	for (i = 0; i < count; i++) {
		vertices[i].n.tc[0] += deltaX;
	}
	currentX += deltaX;	timeX += 1;
}

void scroll_sts_mat_candle_fireflame_layer5() {
	Gfx *mat = segmented_to_virtual(mat_candle_fireflame_layer5);
	shift_t_down(mat, 13, PACK_TILESIZE(0, 3));
	shift_t_down(mat, 21, PACK_TILESIZE(0, 5));
};

void scroll_actor_geo_candle() {
	scroll_candle_fire_flame_mesh_layer_5_vtx_0();
	scroll_sts_mat_candle_fireflame_layer5();
}
