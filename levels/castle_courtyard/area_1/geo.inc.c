#include "src/game/envfx_snow.h"

const GeoLayout castle_courtyard_area_1_geo[] = {
	GEO_NODE_START(),
	GEO_OPEN_NODE(),
		GEO_ASM(1, geo_update_fog),
		GEO_DISPLAY_LIST(1, castle_courtyard_dl_da_mountain_mesh_layer_1),
		GEO_DISPLAY_LIST(LAYER_OPAQUE, castle_courtyard_dl_da_mountain_mesh),
		GEO_ASM(6, geo_set_fire_env),
		GEO_DISPLAY_LIST(6, castle_courtyard_dl_fireglow_mesh_layer_6),
		GEO_ASM(4, geo_update_fog),
		GEO_DISPLAY_LIST(4, castle_courtyard_dl_grassnstuff_002_mesh_layer_4),
		GEO_TRANSLATE_NODE(LAYER_OPAQUE, 220, 692, -1758),
		GEO_OPEN_NODE(),
			GEO_ASM(5, geo_update_fog),
			GEO_DISPLAY_LIST(5, castle_courtyard_dl_overhanggg_mesh_layer_5),
		GEO_CLOSE_NODE(),
		GEO_TRANSLATE_NODE(LAYER_OPAQUE, 303, 33, 312),
		GEO_OPEN_NODE(),
			GEO_ASM(1, geo_set_fire_env),
			GEO_DISPLAY_LIST(1, castle_courtyard_dl_rock_mesh_layer_1),
		GEO_CLOSE_NODE(),
		GEO_TRANSLATE_NODE_WITH_DL(1, -401, 85, 299, castle_courtyard_dl_tent_mesh_layer_1),
		GEO_ASM(1, geo_set_fire_env),
		GEO_DISPLAY_LIST(1, castle_courtyard_dl_themlogs_mesh_layer_1),
		GEO_TRANSLATE_NODE(LAYER_OPAQUE, -419, -33, 10),
		GEO_OPEN_NODE(),
			GEO_ASM(1, geo_set_fire_env),
			GEO_DISPLAY_LIST(1, castle_courtyard_dl_treeee_mesh_layer_1),
			GEO_ASM(1, geo_set_fire_env),
			GEO_DISPLAY_LIST(1, castle_courtyard_dl_treeee_001_mesh_layer_1),
			GEO_ASM(7, geo_set_fire_env),
			GEO_DISPLAY_LIST(7, castle_courtyard_dl_treeee_002_mesh_layer_7),
			GEO_TRANSLATE_NODE(LAYER_TRANSPARENT, 1032, 0, 1312),
			GEO_OPEN_NODE(),
				GEO_ASM(1, geo_set_fire_env),
				GEO_DISPLAY_LIST(1, castle_courtyard_dl_treeee_003_mesh_layer_1),
			GEO_CLOSE_NODE(),
			GEO_TRANSLATE_NODE(LAYER_TRANSPARENT, 1032, 0, 1312),
			GEO_OPEN_NODE(),
				GEO_ASM(1, geo_set_fire_env),
				GEO_DISPLAY_LIST(1, castle_courtyard_dl_treeee_004_mesh_layer_1),
			GEO_CLOSE_NODE(),
			GEO_TRANSLATE_NODE(LAYER_TRANSPARENT, 1032, 0, 1312),
			GEO_OPEN_NODE(),
				GEO_ASM(7, geo_set_fire_env),
				GEO_DISPLAY_LIST(7, castle_courtyard_dl_treeee_005_mesh_layer_7),
			GEO_CLOSE_NODE(),
		GEO_CLOSE_NODE(),
	GEO_CLOSE_NODE(),
	GEO_RETURN(),
};
const GeoLayout castle_courtyard_area_1[] = {
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
					GEO_BRANCH(1, castle_courtyard_area_1_geo),
					GEO_RENDER_OBJ(),
					GEO_ASM(ENVFX_SNOW_WATER, geo_envfx_main),
				GEO_CLOSE_NODE(),
			GEO_CLOSE_NODE(),
		GEO_CLOSE_NODE(),
		GEO_DISPLAY_LIST(LAYER_OPAQUE, castle_courtyard_dl_material_revert_render_settings),
		GEO_DISPLAY_LIST(1, castle_courtyard_dl_material_revert_render_settings),
		GEO_DISPLAY_LIST(5, castle_courtyard_dl_material_revert_render_settings),
		GEO_DISPLAY_LIST(7, castle_courtyard_dl_material_revert_render_settings),
		GEO_DISPLAY_LIST(6, castle_courtyard_dl_material_revert_render_settings),
		GEO_DISPLAY_LIST(4, castle_courtyard_dl_material_revert_render_settings),
	GEO_CLOSE_NODE(),
	GEO_END(),
};
