#include "src/game/envfx_snow.h"

const GeoLayout castle_grounds_area_2_geo[] = {
	GEO_NODE_START(),
	GEO_OPEN_NODE(),
		GEO_ASM(0, geo_update_fog),
		GEO_DISPLAY_LIST(1, castle_grounds_dl_a_ROOMa_mesh_layer_1),
		GEO_TRANSLATE_NODE_WITH_DL(5, -4482, 836, 2665, castle_grounds_dl_afakewater_mesh_layer_5),
		GEO_TRANSLATE_NODE(LAYER_OPAQUE, -6149, -370, 2641),
		GEO_TRANSLATE_NODE_WITH_DL(5, 6692, 3716, 2666, castle_grounds_dl_aBlob_001_mesh_layer_5),
		GEO_TRANSLATE_NODE(LAYER_OPAQUE, 7227, 4704, 2688),
		GEO_TRANSLATE_NODE_WITH_DL(5, 3460, 1504, 2742, castle_grounds_dl_awaters_006_mesh_layer_5),
		GEO_TRANSLATE_NODE(LAYER_OPAQUE, -5524, 6195, 0),
	GEO_CLOSE_NODE(),
	GEO_RETURN(),
};
const GeoLayout castle_grounds_area_2[] = {
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
					GEO_ASM(0, geo_zbuffer_clear),
					GEO_BRANCH(1, castle_grounds_area_2_geo),
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
