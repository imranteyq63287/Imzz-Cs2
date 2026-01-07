#pragma once

#include "../utils/schema/schema.h"

class cs_match_stats
{
public:
    SCHEMA(i_enemy_5ks, int32_t, "CSMatchStats_t", "m_iEnemy5Ks");
    SCHEMA(i_enemy_4ks, int32_t, "CSMatchStats_t", "m_iEnemy4Ks");
    SCHEMA(i_enemy_3ks, int32_t, "CSMatchStats_t", "m_iEnemy3Ks");
    SCHEMA(i_enemy_knife_kills, int32_t, "CSMatchStats_t", "m_iEnemyKnifeKills");
    SCHEMA(i_enemy_taser_kills, int32_t, "CSMatchStats_t", "m_iEnemyTaserKills");
};