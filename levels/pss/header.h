#ifndef PSS_HEADER_H
#define PSS_HEADER_H

#include "types.h"

// geo
extern const GeoLayout pss_geo_000100[];

// leveldata
extern const Gfx pss_seg7_dl_0700A7C0[];
extern const Gfx pss_seg7_dl_0700AFA8[];
extern const Gfx pss_seg7_dl_0700B3F0[];
extern const Gfx pss_seg7_dl_0700D338[];
extern const Gfx pss_seg7_dl_0700DAD8[];
extern const Gfx pss_seg7_dl_0700E2B0[];
extern const Gfx pss_seg7_dl_0700E3E8[];
extern const Collision pss_seg7_collision[];
extern const MacroObject pss_seg7_macro_objs[];
#include "levels/pss/marble/marble.h"

// script
extern const LevelScript level_pss_entry[];

#include "levels/pss/header.inc.h"

#include "levels/pss/sky_dome2/geo_header.h"

#include "levels/pss/squat_palm/geo_header.h"

#include "levels/pss/squat_palm/collision_header.h"

#endif
