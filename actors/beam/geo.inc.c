#include "src/game/envfx_snow.h"

const GeoLayout beam_geo[] = {
	GEO_CULLING_RADIUS(850),
	GEO_OPEN_NODE(),
		GEO_TRANSLATE_ROTATE(LAYER_OPAQUE, 0, 3, 0, 0, 90, 0),
		GEO_OPEN_NODE(),
			GEO_ANIMATED_PART(LAYER_TRANSPARENT, 0, 0, 0, NULL),
			GEO_OPEN_NODE(),
				GEO_ASM(21, geo_trippy_lookat),
				GEO_ASM(0, geo_update_layer_transparency),
				GEO_ANIMATED_PART(LAYER_TRANSPARENT, 0, 0, 0, beam_beam_mesh_layer_5),
			GEO_CLOSE_NODE(),
		GEO_CLOSE_NODE(),
		GEO_DISPLAY_LIST(LAYER_TRANSPARENT, beam_material_revert_render_settings),
	GEO_CLOSE_NODE(),
	GEO_END(),
};
