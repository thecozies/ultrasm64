#include "src/game/envfx_snow.h"

#ifndef GEO_CULLING_RADIUS
#include "ultra64.h"
#include "sm64.h"
#include "types.h"
#include "geo_commands.h"
#include "game/object_helpers.h"
#include "actors/parasite/geo_header.h"
#define const
#endif

#define ORB_LOD_MIN  2000
#define ORB_LOD_MID  3500
#define ORB_LOD_MAX 10000

const GeoLayout parasite_geo[] = {
	GEO_CULLING_RADIUS(72),
	GEO_OPEN_NODE(),
		GEO_SHADOW(0, 135, 65),
		GEO_OPEN_NODE(),
			GEO_SCALE(LAYER_FORCE, 98304),
			GEO_OPEN_NODE(),
				GEO_NODE_START(),
				GEO_OPEN_NODE(),
					GEO_TRANSLATE_ROTATE(LAYER_OPAQUE, 0, -25, 0, 0, 90, 0),
					GEO_OPEN_NODE(),
						GEO_ANIMATED_PART(LAYER_TRANSPARENT, 0, 0, 0, NULL),
						GEO_OPEN_NODE(),
							GEO_ANIMATED_PART(LAYER_TRANSPARENT, 0, 0, 0, NULL),
							GEO_OPEN_NODE(),
								GEO_ASM(0, geo_update_layer_transparency),
								GEO_ASM(5, geo_trippy_lookat),
								GEO_RENDER_RANGE(0, ORB_LOD_MIN),
								GEO_OPEN_NODE(),
									GEO_ANIMATED_PART(LAYER_TRANSPARENT, 0, 0, 0, parasite_2egg_mesh_layer_5),
								GEO_CLOSE_NODE(),
								GEO_BILLBOARD_WITH_PARAMS(LAYER_TRANSPARENT, 0, 0, 0),
								GEO_OPEN_NODE(),
									GEO_RENDER_RANGE(0, ORB_LOD_MIN),
									GEO_OPEN_NODE(),
										GEO_ANIMATED_PART(LAYER_TRANSPARENT, 0, 0, 0, parasite_haze_mesh_layer_5),
									GEO_CLOSE_NODE(),
									GEO_RENDER_RANGE(ORB_LOD_MIN, ORB_LOD_MID),
									GEO_OPEN_NODE(),
										GEO_ANIMATED_PART(LAYER_TRANSPARENT, 0, 0, 0, parasite_orbLOD_mesh_layer_5),
										GEO_ANIMATED_PART(LAYER_TRANSPARENT, 0, 0, 0, parasite_hazeLOD_mesh_layer_5),
									GEO_CLOSE_NODE(),
									GEO_RENDER_RANGE(ORB_LOD_MID, ORB_LOD_MAX),
									GEO_OPEN_NODE(),
										GEO_ANIMATED_PART(LAYER_ALPHA, 0, 0, 0, parasite_zorbBB_mesh_layer_5),
									GEO_CLOSE_NODE(),
								GEO_CLOSE_NODE(),
							GEO_CLOSE_NODE(),
						GEO_CLOSE_NODE(),
					GEO_CLOSE_NODE(),
				GEO_CLOSE_NODE(),
			GEO_CLOSE_NODE(),
		GEO_CLOSE_NODE(),
		GEO_DISPLAY_LIST(LAYER_TRANSPARENT, parasite_material_revert_render_settings),
	GEO_CLOSE_NODE(),
	GEO_END(),
};
