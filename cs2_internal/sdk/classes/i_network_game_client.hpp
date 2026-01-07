#pragma once
#include "../../sdk/utils/virual.h"
#include "../../sdk/modules/c_modules.hpp"
#include <string_view>

class i_network_client 
{
public:
	int get_client_tick() 
	{
		return vmt::call_virtual<int>(this, 5);
	}

	int get_server_tick()
	{
		return vmt::call_virtual<int>(this, 6);
	}

	const char* get_map_name()
	{
		return *reinterpret_cast<const char**>(reinterpret_cast<std::uintptr_t>(this) + 0x240);
	}

	bool is_connected()
	{
		return std::string_view{ get_map_name() }.find("maps/<empty>.vpk") == std::string::npos;
	}

	void set_prediction(bool value)
	{
		*(bool*)(std::uintptr_t(this) + 0xF8) = value;
	}

	void set_delta_tick(int tick)
	{
		*(int*)(std::uintptr_t(this) + 0x25C) = tick;
	}

	OFFSET(m_predicted, bool, 0xF8);
	OFFSET(m_delta_tick, int, 0x25C);
};

class i_network_client_service
{
public:
	i_network_client* get_network_client()
	{
		return vmt::call_virtual<i_network_client*>(this, 23);
	}

	void force_full_update(const char* reason = "initial update") 
	{
		vmt::call_virtual<void>(this, 31, reason);
	}
};