#include "src/game/envfx_snow.h"
#include "geo_commands.h"

const GeoLayout sky_dome_geo[] = {
	GEO_NODE_START(),
	GEO_OPEN_NODE(),
		GEO_DISPLAY_LIST(0, sky_dome_sky_dome_001_mesh_layer_0),
		GEO_DISPLAY_LIST(0, sky_dome_material_revert_render_settings),
	
					// GEO_ASM(0, geo_backdrop_move),
					// GEO_TRANSLATE_ROTATE_WITH_DL(0, 0, 0, 0, 0, 0, 0, 0, sky_dome_sky_dome_001_mesh_layer_0),
					// GEO_DISPLAY_LIST(0, sky_dome_material_revert_render_settings),
					// GEO_ASM(0, geo_zbuffer_clear),


					// GEO_ASM(0, geo_backdrop_move),
					// GEO_TRANSLATE_NODE_WITH_DL(0, 0, 0, 0, sky_dome_sky_dome_001_mesh_layer_0),
					// GEO_DISPLAY_LIST(0, sky_dome_material_revert_render_settings),
					// GEO_ASM(0, geo_zbuffer_clear),
	GEO_CLOSE_NODE(),
	GEO_END(),
};
