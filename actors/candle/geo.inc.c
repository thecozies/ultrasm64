#include "src/game/envfx_snow.h"

const GeoLayout candle_geo[] = {
	GEO_NODE_START(),
	GEO_OPEN_NODE(),
		GEO_TRANSLATE_ROTATE(LAYER_OPAQUE, 0, 0, 0, 0, 90, 0),
		GEO_OPEN_NODE(),
			GEO_ANIMATED_PART(LAYER_OPAQUE, 0, 0, 0, candle_candle_mesh_layer_1),
			GEO_OPEN_NODE(),
				GEO_BILLBOARD_WITH_PARAMS(LAYER_OPAQUE, -73, 56, 0),
				GEO_OPEN_NODE(),
					GEO_ANIMATED_PART(LAYER_TRANSPARENT, 0, 0, 0, candle_fire_flame_mesh_layer_5),
				GEO_CLOSE_NODE(),
			GEO_CLOSE_NODE(),
		GEO_CLOSE_NODE(),
		GEO_DISPLAY_LIST(LAYER_OPAQUE, candle_material_revert_render_settings),
		GEO_DISPLAY_LIST(LAYER_TRANSPARENT, candle_material_revert_render_settings),
	GEO_CLOSE_NODE(),
	GEO_END(),
};
