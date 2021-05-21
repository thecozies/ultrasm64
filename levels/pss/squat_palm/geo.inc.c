#include "src/game/envfx_snow.h"

const GeoLayout squat_palm_geo[] = {
	GEO_NODE_START(),
	GEO_OPEN_NODE(),
		GEO_ASM(0, geo_update_fog),
		GEO_DISPLAY_LIST(1, squat_palm_squat_palm_mesh_layer_1),
		GEO_DISPLAY_LIST(1, squat_palm_material_revert_render_settings),
	GEO_CLOSE_NODE(),
	GEO_END(),
};
