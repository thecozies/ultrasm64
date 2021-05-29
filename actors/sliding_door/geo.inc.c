#include "src/game/envfx_snow.h"

const GeoLayout sliding_door_geo[] = {
	GEO_NODE_START(),
	GEO_OPEN_NODE(),
		GEO_DISPLAY_LIST(1, sliding_door_sliding_door_mesh_mesh_layer_1),
		GEO_DISPLAY_LIST(1, sliding_door_material_revert_render_settings),
	GEO_CLOSE_NODE(),
	GEO_END(),
};
