#include "src/game/envfx_snow.h"

const GeoLayout castle_grounds_area_5_geo[] = {
	GEO_NODE_START(),
	GEO_OPEN_NODE(),
		GEO_TRANSLATE_NODE(LAYER_OPAQUE, -200, -700, 300),
		GEO_OPEN_NODE(),
			GEO_ASM(0, geo_update_fog),
			GEO_DISPLAY_LIST(1, castle_grounds_dl_a_ROOM3_001_mesh_layer_1),
		GEO_CLOSE_NODE(),
		GEO_TRANSLATE_NODE(LAYER_OPAQUE, -100, 3000, 300),
		GEO_OPEN_NODE(),
			GEO_RENDER_RANGE(-2000, 3000),
			GEO_OPEN_NODE(),
				GEO_DISPLAY_LIST(1, castle_grounds_dl_a_ROOM3_004_mesh_layer_1),
			GEO_CLOSE_NODE(),
		GEO_CLOSE_NODE(),
		GEO_TRANSLATE_NODE(LAYER_OPAQUE, -313, -4159, -1087),
		GEO_OPEN_NODE(),
			GEO_RENDER_RANGE(-1500, 6500),
			GEO_OPEN_NODE(),
				GEO_DISPLAY_LIST(5, castle_grounds_dl_apurpwaters_001_mesh_layer_5),
			GEO_CLOSE_NODE(),
		GEO_CLOSE_NODE(),
		GEO_TRANSLATE_NODE(LAYER_OPAQUE, -38, 211, 2817),
		GEO_OPEN_NODE(),
			GEO_RENDER_RANGE(-5000, 5000),
			GEO_OPEN_NODE(),
				GEO_DISPLAY_LIST(5, castle_grounds_dl_apurpwaters_002_mesh_layer_5),
			GEO_CLOSE_NODE(),
		GEO_CLOSE_NODE(),
		GEO_TRANSLATE_NODE(LAYER_OPAQUE, 656, 298, 567),
		GEO_OPEN_NODE(),
			GEO_RENDER_RANGE(-1000, 3000),
			GEO_OPEN_NODE(),
				GEO_DISPLAY_LIST(5, castle_grounds_dl_apurpwaters_003_mesh_layer_5),
			GEO_CLOSE_NODE(),
		GEO_CLOSE_NODE(),
		GEO_TRANSLATE_NODE(LAYER_OPAQUE, -126, -2126, 1377),
		GEO_OPEN_NODE(),
			GEO_RENDER_RANGE(-1000, 4000),
			GEO_OPEN_NODE(),
				GEO_DISPLAY_LIST(5, castle_grounds_dl_apurpwaters_004_mesh_layer_5),
			GEO_CLOSE_NODE(),
		GEO_CLOSE_NODE(),
		GEO_TRANSLATE_NODE(LAYER_OPAQUE, 443, -1709, -983),
		GEO_OPEN_NODE(),
			GEO_RENDER_RANGE(-3000, 4000),
			GEO_OPEN_NODE(),
				GEO_DISPLAY_LIST(5, castle_grounds_dl_apurpwaters_005_mesh_layer_5),
			GEO_CLOSE_NODE(),
		GEO_CLOSE_NODE(),
		GEO_TRANSLATE_NODE_WITH_DL(1, -67, -5073, 3586, castle_grounds_dl_hallway_001_mesh_layer_1),
	GEO_CLOSE_NODE(),
	GEO_RETURN(),
};
const GeoLayout castle_grounds_area_5[] = {
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
					GEO_TRANSLATE_ROTATE_WITH_DL(0, 0, 0, 0, 0, 0, 0, sky_dome_sky_dome_001_mesh_layer_0),
					GEO_DISPLAY_LIST(0, sky_dome_material_revert_render_settings),
					GEO_BRANCH(1, castle_grounds_area_5_geo),
					GEO_RENDER_OBJ(),
					GEO_ASM(ENVFX_SNOW_WATER, geo_envfx_main),
				GEO_CLOSE_NODE(),
			GEO_CLOSE_NODE(),
		GEO_CLOSE_NODE(),
		GEO_DISPLAY_LIST(1, castle_grounds_dl_material_revert_render_settings),
		GEO_DISPLAY_LIST(5, castle_grounds_dl_material_revert_render_settings),
	GEO_CLOSE_NODE(),
	GEO_END(),
};
