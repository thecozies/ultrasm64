#include "types.h"
#include "game/mario.h"
#include "game/game_init.h"
#include "levels/pss/header.h"

#define PAL_ROT_SPEED 8

void scroll_marble(void) {
	int cycle = gGlobalTimer % PAL_ROT_SPEED;

	if (gGlobalTimer % PAL_ROT_SPEED == 0) {
		u16 *pal = segmented_to_virtual(marble_pal);
		u16 *pal_cpy = segmented_to_virtual(marble_pal_cpy);
		u16 firstVal = pal_cpy[0];
		for (int i = 1; i <= 15; i++) {
			pal_cpy[i - 1] = pal_cpy[i];
			pal[i - 1] = pal_cpy[i];
		}
		pal_cpy[15] = firstVal;
	} else {
		u16 *pal = segmented_to_virtual(marble_pal);
		u16 *pal_cpy = segmented_to_virtual(marble_pal_cpy);

		for (int i = 0; i <= 15; i++) {
			u8 firstIdx = i == 0 ? 15 : i - 1;

			s32 c1 = pal_cpy[firstIdx] >> 4;
			s32 c2 = pal_cpy[i] >> 4;
			s32 diff = (c2 - c1) * (((f32)(PAL_ROT_SPEED - cycle)) / PAL_ROT_SPEED);
			u16 col = ((c2 - diff) << 4) | 0xFF;

			pal[firstIdx] = col;
		}
	}

}