// Copyright (c) rAthena Dev Teams - Licensed under GNU GPL
// For more information, see LICENCE in the main folder

#include "warpportal.hpp"

#include "map/clif.hpp"
#include "map/pc.hpp"
#include "map/status.hpp"

SkillWarpPortal::SkillWarpPortal() : SkillImpl(AL_WARP) {
}

void SkillWarpPortal::castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const {
	map_session_data* sd = BL_CAST(BL_PC, src);
	status_change* sc = status_get_sc(src);

	if(sd != nullptr) {
		int64 extendedMemo = pc_readreg2(sd, "EXT_MEMO_SLOTS");
#if PACKETVER_MAIN_NUM < 20170502 || PACKETVER_RE_NUM < 20170419 || !defined(PACKETVER_ZERO)
		extendedMemo = 0; // Extended memo points are not supported before these versions, so we need to ignore the value read from the register.
#endif
		std::vector<std::string> maps(static_cast<size_t>(MAX_MEMOPOINTS + extendedMemo + 1));
		maps.push_back( sd->status.save_point.map );

		if( skill_lv >= 2 ){
			maps.push_back( sd->status.memo_point[0].map );

			if( skill_lv >= 3 ){
				maps.push_back( sd->status.memo_point[1].map );

				if( skill_lv >= 4 ){
					maps.push_back( sd->status.memo_point[2].map );

					if (skill_lv + extendedMemo >= 5) {
						maps.push_back(sd->status.memo_point[3].map);

						if (skill_lv + extendedMemo >= 6) {
							maps.push_back(sd->status.memo_point[4].map);

							if (skill_lv + extendedMemo >= 7) {
								maps.push_back(sd->status.memo_point[5].map);
							}
						}
					}
				}
			}
		}

		clif_skill_warppoint( *sd, getSkillId(), skill_lv, maps );
	}
	if( sc && sc->getSCE(SC_CURSEDCIRCLE_ATKER) ) //Should only remove after the skill has been casted.
		status_change_end(src,SC_CURSEDCIRCLE_ATKER);
	// not to consume item.
	flag |= SKILL_NOCONSUME_REQ;
}
