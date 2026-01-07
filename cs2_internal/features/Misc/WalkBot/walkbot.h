#pragma once
#include "../../config_system.h"
#include "../../../includes.h"
#include "../../../sdk/entity/entity.h"
#include "../../../sdk/classes/c_view_setup.h"
class c_walkbot {
public:
	void get_positions();
	vec3_t get_local_viewangles(c_user_cmd* user_cmd);
	void walk_to_target(c_user_cmd* user_cmd);
};
inline float closest_distance = FLT_MAX;
inline vec3_t closest_pos;
inline c_cs_player_pawn* closest_enemy = nullptr;
inline c_cs_player_controller* closest_controller = nullptr;
inline const auto g_walkbot = std::make_unique<c_walkbot>();