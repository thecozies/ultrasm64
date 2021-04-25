void scroll_sts_mat_castle_courtyard_dl_wood_layer1() {
	Gfx *mat = segmented_to_virtual(mat_castle_courtyard_dl_wood_layer1);
	shift_s(mat, 21, PACK_TILESIZE(0, 1));
};

void scroll_castle_courtyard() {
	scroll_sts_mat_castle_courtyard_dl_wood_layer1();
}
