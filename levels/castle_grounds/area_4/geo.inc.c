#include "src/game/envfx_snow.h"

const GeoLayout castle_grounds_area_4_geo[] = {
	GEO_NODE_START(),
	GEO_OPEN_NODE(),
		GEO_ASM(0, geo_update_fog),
		GEO_DISPLAY_LIST(1, castle_grounds_dl_a_abigolBlocks_mesh_layer_1),
		GEO_ASM(0, geo_update_fog),
		GEO_DISPLAY_LIST(1, castle_grounds_dl_a_abigolBlocks_002_mesh_layer_1),
		GEO_TRANSLATE_NODE_WITH_DL(1, 5000, -2800, 500, castle_grounds_dl_a_abigolBlocks_003_mesh_layer_1),
		GEO_TRANSLATE_NODE_WITH_DL(5, 2300, -2400, 7100, castle_grounds_dl_a_Blob_mesh_layer_5),
		GEO_TRANSLATE_NODE_WITH_DL(5, 6700, -4500, 2700, castle_grounds_dl_a_Blob_001_mesh_layer_5),
		GEO_DISPLAY_LIST(5, castle_grounds_dl_a_purpwaters_007_mesh_layer_5),
		GEO_TRANSLATE_NODE_WITH_DL(1, 4500, -1900, 3200, castle_grounds_dl_a_ROOM3_mesh_layer_1),
		GEO_OPEN_NODE(),
			GEO_DISPLAY_LIST(4, castle_grounds_dl_a_ROOM3_mesh_layer_4),
		GEO_CLOSE_NODE(),
		GEO_ASM(0, geo_update_fog),
		GEO_DISPLAY_LIST(1, castle_grounds_dl_a_ROOM3_002_mesh_layer_1),
		GEO_TRANSLATE_NODE(LAYER_OPAQUE, 7000, -2800, 2700),
		GEO_OPEN_NODE(),
			GEO_RENDER_RANGE(-2000, 2000),
			GEO_OPEN_NODE(),
				GEO_DISPLAY_LIST(1, castle_grounds_dl_a_ROOM3_003_mesh_layer_1),
			GEO_CLOSE_NODE(),
		GEO_CLOSE_NODE(),
		GEO_TRANSLATE_NODE(LAYER_OPAQUE, -2400, -1600, 2700),
		GEO_OPEN_NODE(),
			GEO_RENDER_RANGE(-3000, 7000),
			GEO_OPEN_NODE(),
				GEO_DISPLAY_LIST(1, castle_grounds_dl_haallway_mesh_layer_1),
			GEO_CLOSE_NODE(),
		GEO_CLOSE_NODE(),
		GEO_TRANSLATE_NODE(LAYER_OPAQUE, -2400, -1600, 2700),
		GEO_OPEN_NODE(),
			GEO_RENDER_RANGE(-3600, 7000),
			GEO_OPEN_NODE(),
				GEO_DISPLAY_LIST(1, castle_grounds_dl_hallway_mesh_layer_1),
			GEO_CLOSE_NODE(),
		GEO_CLOSE_NODE(),
		GEO_TRANSLATE_NODE(LAYER_OPAQUE, 3407, -7489, 0),
	GEO_CLOSE_NODE(),
	GEO_RETURN(),
};
const GeoLayout castle_grounds_area_4[] = {
	GEO_NODE_SCREEN_AREA(10, SCREEN_WIDTH/2, SCREEN_HEIGHT/2, SCREEN_WIDTH/2, SCREEN_HEIGHT/2),
	GEO_OPEN_NODE(),
		GEO_ZBUFFER(0),
		GEO_OPEN_NODE(),
			GEO_NODE_ORTHO(100.0000),
			GEO_OPEN_NODE(),
				GEO_BACKGROUND_COLOR(0x0001),
			GEO_CLOSE_NODE(),
		GEO_CLOSE_NODE(),
		GEO_ZBUFFER(1),
		GEO_OPEN_NODE(),
			GEO_CAMERA_FRUSTUM_WITH_FUNC(45.0000, 100, 30000, geo_camera_fov),
			GEO_OPEN_NODE(),
				GEO_CAMERA(CAMERA_MODE_8_DIRECTIONS, 0, 0, 0, 0, -100, 0, geo_camera_main),
				GEO_OPEN_NODE(),
					GEO_ASM(0, geo_backdrop_move),
					GEO_TRANSLATE_NODE_WITH_DL(0, 0, 0, 0, sky_dome_sky_dome_001_mesh_layer_0),
					GEO_DISPLAY_LIST(0, sky_dome_material_revert_render_settings),
					GEO_BRANCH(1, castle_grounds_area_4_geo),
					GEO_RENDER_OBJ(),
					GEO_ASM(ENVFX_SNOW_WATER, geo_envfx_main),
				GEO_CLOSE_NODE(),
			GEO_CLOSE_NODE(),
		GEO_CLOSE_NODE(),
		GEO_DISPLAY_LIST(5, castle_grounds_dl_material_revert_render_settings),
		GEO_DISPLAY_LIST(4, castle_grounds_dl_material_revert_render_settings),
		GEO_DISPLAY_LIST(1, castle_grounds_dl_material_revert_render_settings),
	GEO_CLOSE_NODE(),
	GEO_END(),
};
