#pragma once

#include "cs_match_stats.h"
#include "../utils/schema/schema.h"
#include "../utils/typedefs/c_utl_vector.h"
#include "c_cs_player_controller.h"

class c_cs_player_controller_action_tracking_services
{
public:
    SCHEMA(per_round_stats, c_utl_vector<c_cs_per_round_stats>, "CCSPlayerController_ActionTrackingServices", "m_perRoundStats");
    SCHEMA(match_stats, cs_match_stats, "CCSPlayerController_ActionTrackingServices", "m_matchStats");
    SCHEMA(i_num_round_kills, int32_t, "CCSPlayerController_ActionTrackingServices", "m_iNumRoundKills");
    SCHEMA(i_num_round_kills_headshots, int32_t, "CCSPlayerController_ActionTrackingServices", "m_iNumRoundKillsHeadshots");
    SCHEMA(un_total_round_damage_dealt, uint32_t, "CCSPlayerController_ActionTrackingServices", "m_unTotalRoundDamageDealt");
};