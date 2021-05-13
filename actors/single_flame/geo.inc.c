#include "src/game/envfx_snow.h"

const GeoLayout single_flame_geo[] = {
	GEO_NODE_START(),
	GEO_OPEN_NODE(),
		GEO_SCALE(LAYER_TRANSPARENT, 65536),
		GEO_OPEN_NODE(),
			GEO_BILLBOARD_WITH_PARAMS(LAYER_TRANSPARENT, 0, 0, 0),
			GEO_OPEN_NODE(),
				GEO_ASM(0x65, geo_set_fire_env),
				GEO_ANIMATED_PART(LAYER_TRANSPARENT, 0, 0, 0, single_flame_fire_flame_mesh_layer_5),
			GEO_CLOSE_NODE(),
		GEO_CLOSE_NODE(),
		GEO_DISPLAY_LIST(LAYER_TRANSPARENT, single_flame_material_revert_render_settings),
	GEO_CLOSE_NODE(),
	GEO_END(),
};
