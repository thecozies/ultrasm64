#include <ultra64.h>
#include "sm64.h"
#include "behavior_data.h"
#include "model_ids.h"
#include "seq_ids.h"
#include "dialog_ids.h"
#include "segment_symbols.h"
#include "level_commands.h"

#include "game/level_update.h"

#include "levels/scripts.h"

#include "actors/common1.h"
#include "actors/group8.h"

#include "make_const_nonconst.h"
#include "levels/pss/header.h"

const LevelScript level_pss_entry[] = {
	INIT_LEVEL(),
	LOAD_YAY0(0x07, _pss_segment_7SegmentRomStart, _pss_segment_7SegmentRomEnd), 
	LOAD_YAY0_TEXTURE(0x09, _mountain_yay0SegmentRomStart, _mountain_yay0SegmentRomEnd), 
	LOAD_YAY0(0x05, _group8_yay0SegmentRomStart, _group8_yay0SegmentRomEnd), 
	LOAD_RAW(0x0C, _group8_geoSegmentRomStart, _group8_geoSegmentRomEnd), 
	LOAD_YAY0(0x08, _common0_yay0SegmentRomStart, _common0_yay0SegmentRomEnd), 
	LOAD_RAW(0x0F, _common0_geoSegmentRomStart, _common0_geoSegmentRomEnd), 
	LOAD_YAY0(0xb, _effect_yay0SegmentRomStart, _effect_yay0SegmentRomEnd), 
	ALLOC_LEVEL_POOL(),
	MARIO(MODEL_MARIO, 0x00000001, bhvMario), 
	JUMP_LINK(script_func_global_1), 
	JUMP_LINK(script_func_global_9), 
	LOAD_MODEL_FROM_GEO(MODEL_BEAM, beam_geo), 
	LOAD_MODEL_FROM_GEO(MODEL_SQUAT_PALM, squat_palm_geo), 

	AREA(0, pss_area_0),
		WARP_NODE(0x0A, LEVEL_PSS, 0x00, 0x0A, WARP_NO_CHECKPOINT),
		WARP_NODE(0xF0, LEVEL_PSS, 0x00, 0x0A, WARP_NO_CHECKPOINT),
		WARP_NODE(0xF1, LEVEL_PSS, 0x00, 0x0A, WARP_NO_CHECKPOINT),
		OBJECT(MODEL_SLIDING_DOOR, -5876, -5347, -95, 180, -90, 0, 0xFF080000, bhvSlidingDoorNoRoom),
		OBJECT(MODEL_SQUAT_PALM, -8699, -6037, -95, 0, -5, 0, 0x00000000, bhvSquatPalm),
		OBJECT(MODEL_SMALL_PALM, -7320, -6037, -1474, 0, -28, 0, 0x00000000, bhvSmallPalm),
		OBJECT(MODEL_SMALL_PALM, -7320, -6037, 1284, 0, -177, 0, 0x00000000, bhvSmallPalm),
		OBJECT(MODEL_NONE, -4876, -6037, -95, 0, 90, 0, 0xFF070000, bhvCheckpoint),
		OBJECT(MODEL_NONE, -5676, -6037, -95, 0, 90, 0, 0xFF060000, bhvCheckpoint),
		OBJECT(MODEL_NONE, -7318, 5026, -95, -180, 90, 0, 0xFF060000, bhvCheckpoint),
		OBJECT(MODEL_PARASITE, -7318, -5525, -95, -180, 90, 0, 0x00080000, bhvParasite),
		OBJECT(MODEL_PARASITE, -5756, -4499, -95, -180, 90, 0, 0x00080002, bhvParasite),
		OBJECT(MODEL_NONE, -7318, 5258, -95, -180, 90, 0, 0x000A0000, bhvAirborneWarp),
		OBJECT(MODEL_SLIDING_DOOR, 1128, -6037, -95, 0, 90, 0, 0xFF090000, bhvSlidingDoorNoRoom),
		OBJECT(MODEL_SQUAT_PALM, 1714, -4037, -2386, 0, -76, 0, 0x00000000, bhvSquatPalm),
		OBJECT(MODEL_SQUAT_PALM, 6286, -4037, -2386, 0, 169, 0, 0x00000000, bhvSquatPalm),
		OBJECT(MODEL_SQUAT_PALM, 1714, -4037, 2186, 0, 169, 0, 0x00000000, bhvSquatPalm),
		OBJECT(MODEL_SQUAT_PALM, 6286, -4037, 2186, 0, -65, 0, 0x00000000, bhvSquatPalm),
		OBJECT(MODEL_SQUAT_PALM, 766, -37, -100, 0, -65, 0, 0x00000000, bhvSquatPalm),
		OBJECT(MODEL_SQUAT_PALM, 4000, -37, 3134, 0, 114, 0, 0x00000000, bhvSquatPalm),
		OBJECT(MODEL_SQUAT_PALM, 4000, -37, -3334, 0, 23, 0, 0x00000000, bhvSquatPalm),
		OBJECT(MODEL_NONE, 1028, -6037, -95, 0, 90, 0, 0xFF080000, bhvCheckpoint),
		OBJECT(MODEL_NONE, 228, -6037, -95, 0, 90, 0, 0xFF070000, bhvCheckpoint),
		OBJECT(MODEL_BEAM, 4000, -6080, -100, 0, 0, 0, 0x00010000, bhvBigOrb),
		OBJECT(MODEL_BIG_ORB, 4000, -6080, -100, 0, 0, 0, 0x00000000, bhvBigOrb),
		OBJECT(MODEL_PARASITE, 4, -5928, -95, 0, -180, 0, 0x00090000, bhvParasiteNoRoom),
		OBJECT(MODEL_PARASITE, 4, -5928, -95, 0, -180, 0, 0x00090002, bhvParasiteNoRoom),
		TERRAIN(pss_area_0_collision),
		ROOMS(pss_area_0_collision_rooms),
		MACRO_OBJECTS(pss_area_0_macro_objs),
		SET_BACKGROUND_MUSIC(0x00, SEQ_STREAMED_TEMPLE_TWO),
		TERRAIN_TYPE(TERRAIN_STONE),
	END_AREA(),

	FREE_LEVEL_POOL(),
	MARIO_POS(1, 0, 0, 0, 0),
	CALL(0, lvl_init_or_update),
	CALL_LOOP(1, lvl_init_or_update),
	CLEAR_LEVEL(),
	SLEEP_BEFORE_EXIT(1),
	EXIT(),
};
