#pragma once

#include "sdk/interfaces/interfaces.h"
#include "sdk/classes/c_cs_player_pawn.h"
#include "sdk/classes/c_cs_player_controller.h"

struct c_global_context
{
	c_base_player_weapon* active_weapon = nullptr;
	c_cs_weapon_base_v_data* weapon_data = nullptr;
	c_cs_player_pawn* local = nullptr;
	c_cs_player_controller* local_controller = nullptr;

	c_user_cmd* user_cmd = nullptr;

	std::string cheat_name = "imzz client";
	std::string cheat_name1 = "imzz";
	std::string cheat_name2 = "client";
};

inline c_global_context g_ctx{};