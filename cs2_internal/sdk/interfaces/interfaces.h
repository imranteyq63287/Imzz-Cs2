#pragma once

#include <Windows.h>
#include <string>
#include <memory>
#include <vector>

#include "vtables/c_mem_alloc.h"
#include "vtables/c_convar.h"
#include "vtables/c_localize.h"
#include "vtables/c_global_vars.h"
#include "vtables/c_schema_system.h"
#include "vtables/c_engine_client.h"
#include "vtables/c_client.h"
#include "vtables/c_entity_system.h"
#include "vtables/c_csgo_input.h"
#include "vtables/c_engine_trace.h"

#include "../modules/modules.h"
#include "../classes/i_network_game_client.hpp"
#include "../classes/i_res_manager.h"
#include "../classes/c_event.h"
#include "../classes/i_particle_system.h"

#define INTERVAL_PER_TICK 0.015625f
#define TICK_INTERVAL (INTERVAL_PER_TICK)
#define TIME_TO_TICKS(TIME) (static_cast<int>(0.5f + static_cast<float>(TIME) / TICK_INTERVAL))
#define TICKS_TO_TIME(TICKS) (TICK_INTERVAL * static_cast<float>(TICKS))

namespace interfaces
{
	template <typename T = void*>
	T find_interface(HMODULE module_handle, const char* name)
	{
		using fn = T(*)(const char*, int*);
		const auto func = reinterpret_cast<fn>(GetProcAddress(module_handle, ("CreateInterface")));
	
		if (!func)
			return nullptr;

		return func(name, nullptr);
	}

	inline c_client* client;
	inline void* input_system;
	inline c_mem_alloc* mem_alloc;
	inline c_cvar* cvar;
	inline c_localize* localize;
	inline c_global_vars* global_vars;
	inline c_schema_system* schema_system;
	inline c_engine_client* engine_client;
	inline c_entity_system* entity_system;
	inline c_csgo_input* csgo_input;
	inline c_engine_trace* engine_trace;
	inline i_network_client_service* m_network_client_service;
	inline i_res_manager* m_res_manager;
	inline cs_i_game_event_manager* m_game_event_manager;
	inline static i_particle_manager* m_particle_manager;
	inline static i_particle_system* c_game_particle_manager_system;
	inline i_legacy_game_ui* m_legacy_game_ui;
	inline float(__cdecl* random_float)(float min, float max) = nullptr;
	inline int(__cdecl* random_seed)(int seed) = nullptr;

	void initialize();
};