#include "src/game/envfx_snow.h"

const GeoLayout cloud_dome_geo[] = {
	GEO_NODE_START(),
	GEO_OPEN_NODE(),
		GEO_DISPLAY_LIST(5, cloud_dome_cloud_dome_mesh_layer_5),
		GEO_DISPLAY_LIST(5, cloud_dome_material_revert_render_settings),
	GEO_CLOSE_NODE(),
	GEO_END(),
};
