#include "types.h"
#include "include/segment_symbols.h"
#include "memory.h"
#include "engine/math_util.h"
#include "src/engine/behavior_script.h"
#include "tile_scroll.h"
#include "texscroll.h"

#ifdef TARGET_N64
#define SCROLL_CONDITION(condition) condition
#else
#define SCROLL_CONDITION(condition) 1
#endif


#include "src/game/texscroll/castle_grounds_texscroll.inc.c"
#include "src/game/texscroll/group0_texscroll.inc.c"
#include "src/game/texscroll/pss_texscroll.inc.c"
#include "src/game/texscroll/castle_courtyard_texscroll.inc.c"
#include "src/game/texscroll/group9_texscroll.inc.c"
#include "src/game/texscroll/group8_texscroll.inc.c"
void scroll_textures() {
	if(SCROLL_CONDITION(sSegmentROMTable[0x7] == (uintptr_t)_castle_grounds_segment_7SegmentRomStart)) {
		scroll_textures_castle_grounds();
	}

	if(SCROLL_CONDITION(sSegmentROMTable[0x4] == (uintptr_t)_group0_yay0SegmentRomStart)) {
		scroll_textures_group0();
	}

	if(SCROLL_CONDITION(sSegmentROMTable[0x7] == (uintptr_t)_pss_segment_7SegmentRomStart)) {
		scroll_textures_pss();
	}

	if(SCROLL_CONDITION(sSegmentROMTable[0x7] == (uintptr_t)_castle_courtyard_segment_7SegmentRomStart)) {
		scroll_textures_castle_courtyard();
	}

	if(SCROLL_CONDITION(sSegmentROMTable[0x5] == (uintptr_t)_group9_yay0SegmentRomStart)) {
		scroll_textures_group9();
	}

	if(SCROLL_CONDITION(sSegmentROMTable[0x5] == (uintptr_t)_group8_yay0SegmentRomStart)) {
		scroll_textures_group8();
	}

}
