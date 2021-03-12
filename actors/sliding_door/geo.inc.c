#include "src/game/envfx_snow.h"

const GeoLayout sliding_door_geo[] = {
	GEO_NODE_START(),
	GEO_OPEN_NODE(),
		GEO_TRANSLATE_ROTATE(LAYER_OPAQUE, 0, 0, 0, 0, 90, 0),
		GEO_OPEN_NODE(),
			GEO_ANIMATED_PART(LAYER_OPAQUE, 0, 0, 0, sliding_door_sliding_door_mesh_layer_1),
		GEO_CLOSE_NODE(),
		GEO_DISPLAY_LIST(LAYER_OPAQUE, sliding_door_material_revert_render_settings),
	GEO_CLOSE_NODE(),
	GEO_END(),
};
