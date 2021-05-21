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

#include "make_const_nonconst.h"
#include "levels/castle_courtyard/header.h"

const LevelScript level_castle_courtyard_entry[] = {
	INIT_LEVEL(),
	LOAD_YAY0(0x07, _castle_courtyard_segment_7SegmentRomStart, _castle_courtyard_segment_7SegmentRomEnd), 
	LOAD_YAY0(0x0A, _water_skybox_yay0SegmentRomStart, _water_skybox_yay0SegmentRomEnd), 
	LOAD_YAY0_TEXTURE(0x09, _outside_yay0SegmentRomStart, _outside_yay0SegmentRomEnd), 
	LOAD_YAY0(0x05, _group9_yay0SegmentRomStart, _group9_yay0SegmentRomEnd), 
	LOAD_RAW(0x0C, _group9_geoSegmentRomStart, _group9_geoSegmentRomEnd), 
	LOAD_YAY0(0x08, _common0_yay0SegmentRomStart, _common0_yay0SegmentRomEnd), 
	LOAD_RAW(0x0F, _common0_geoSegmentRomStart, _common0_geoSegmentRomEnd), 
	LOAD_YAY0(0xb, _effect_yay0SegmentRomStart, _effect_yay0SegmentRomEnd), 
	ALLOC_LEVEL_POOL(),
	MARIO(MODEL_MARIO, 0x00000001, bhvMario), 
	JUMP_LINK(script_func_global_1), 
	JUMP_LINK(script_func_global_10), 

	AREA(1, castle_courtyard_area_1),
		WARP_NODE(0x0A, LEVEL_CASTLE_COURTYARD, 0x01, 0x0A, WARP_NO_CHECKPOINT),
		WARP_NODE(0xF0, LEVEL_CASTLE_COURTYARD, 0x01, 0x0A, WARP_NO_CHECKPOINT),
		WARP_NODE(0xF1, LEVEL_CASTLE_GROUNDS, 0x01, 0x0A, WARP_NO_CHECKPOINT),
		WARP_NODE(0x0B, LEVEL_CASTLE_GROUNDS, 0x01, 0x0A, WARP_NO_CHECKPOINT),
		OBJECT(MODEL_NONE, 67, 1, 510, 0, -180, 0, 0x00000000, bhvLightObj),
		OBJECT(MODEL_SINGLE_FLAME, 67, 1, 510, 0, 0, 0, 0x00000000, bhvSingleFlame),
		OBJECT(MODEL_SLIDING_DOOR, 149, -3, -2207, 0, -180, 0, 0x10100000, bhvSlidingDoor),
		OBJECT(MODEL_PARASITE, 149, 243, -2245, 0, 0, 0, 0x10100000, bhvParasite),
		OBJECT(MODEL_MONKAT_HEAD, 149, 243, -2245, 0, 0, 0, 0x10100000, bhvMonkatHead),
		OBJECT(MODEL_SMALL_PALM, -491, -3, -1622, 0, -180, 0, 0x00000000, bhvSmallPalm),
		OBJECT(MODEL_CANDLE, -101, 304, -2505, 0, 90, 0, 0x00000000, bhvCandle),
		OBJECT(MODEL_CANDLE, 400, 304, -2505, 0, -90, 0, 0x00000000, bhvCandle),
		OBJECT(MODEL_NONE, 248, -19, 385, 0, -53, 0, 0x000A0000, bhvInstantActiveWarp),
		MARIO_POS(0x01, -53, 248, -19, 385),
		OBJECT(MODEL_NONE, 145, -1, -4298, 0, -180, 0, 0x500B0000, bhvWarp),
		TERRAIN(castle_courtyard_area_1_collision),
		MACRO_OBJECTS(castle_courtyard_area_1_macro_objs),
		SET_BACKGROUND_MUSIC(0x00, SEQ_STREAMED_CAMPING),
		TERRAIN_TYPE(TERRAIN_STONE),
	END_AREA(),

	FREE_LEVEL_POOL(),
	MARIO_POS(0x01, -53, 248, -19, 385),
	CALL(0, lvl_init_or_update),
	CALL_LOOP(1, lvl_init_or_update),
	CLEAR_LEVEL(),
	SLEEP_BEFORE_EXIT(1),
	EXIT(),
};
