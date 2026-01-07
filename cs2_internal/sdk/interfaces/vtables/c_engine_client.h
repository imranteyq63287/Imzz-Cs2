#pragma once

#include "../../utils/virual.h"
#include <cstdint>
#include "../../utils/vector.h"

enum EClientFrameStage : int
{
	FRAME_UNDEFINED = -1,
	FRAME_START,
	FRAME_NET_UPDATE_START,
	FRAME_NET_UPDATE_POSTDATAUPDATE_START,
	FRAME_NET_UPDATE_POSTDATAUPDATE_END,
	FRAME_NET_FULL_FRAME_UPDATE_ON_REMOVE,
	FRAME_RENDER_START,
	FRAME_RENDER_END,
	FRAME_NET_UPDATE_END,
	FRAME_NET_CREATION,
	FRAME_SIMULATE_END
};

class c_local_data
{
public:
	char pad_0000[8]; //0x0000
	float N0000006F; //0x0008
	vec3_t m_eye_pos; //0x000C
}; //Size: 0x0018

class c_networked_client_info
{
public:
	int32_t unk; //0x0000
	int32_t m_render_tick; //0x0004
	float m_render_tick_fraction; //0x0008
	int32_t m_player_tick_count; //0x000C
	float m_player_tick_fraction; //0x0010
	char pad_0014[4]; //0x0014
	c_local_data* m_local_data; //0x0018
	char pad_0020[32]; //0x0020
}; //Size: 0x0040

class c_engine_client
{
public:
	bool is_in_game()
	{
		return vmt::call_virtual<bool>(this, 37);
	}

	bool is_connected()
	{
		return vmt::call_virtual<bool>(this, 38);
	}

	void client_cmd_unrestricted(const char* cmd)
	{
		return vmt::call_virtual<void>(this, 48, 0, cmd, 0x7FFEF001);
	}

	int get_local_player_index()
	{
		int idx = -1;

		vmt::call_virtual<void>(this, 52, &idx, 0);
		return idx + 1;
	}

	const char* get_level_name()
	{
		return vmt::call_virtual<const char*>(this, 61);
	}

	const char* get_level_name_short()
	{
		return vmt::call_virtual<const char*>(this, 62);
	}

	c_networked_client_info* get_networked_client_info()
	{
		c_networked_client_info client_info;

		vmt::call_virtual<void*>(this, 178, &client_info);
		return &client_info;
	}

	void ExecuteClientCmd(const char* cmd)
	{
		using ExecuteClientCmdFn = void(__thiscall*)(void*, const char*);
		static ExecuteClientCmdFn fn = reinterpret_cast<ExecuteClientCmdFn>(GetModuleHandleA(("engine2.dll")) + 0x7FFEF001); // not fixed mby
	
		fn(this, cmd);
	}

	void send_cmd(const char* szCmd)
	{
		vmt::call_virtual<void>(this, 48, 0, szCmd, 0x7FFEF001);
	}
};

class i_legacy_game_ui
{
public:
	void show_message_box(const char* title, const char* message, bool show_ok = true, bool show_cancel = false, const char* ok_command = nullptr, const char* cancel_command = nullptr, const char* closed_command = nullptr, const char* legend = nullptr, const char* unknown = nullptr) 
	{
		vmt::call_virtual<void>(this, 28, title, message, show_ok, show_cancel, ok_command, cancel_command, closed_command, legend, unknown);
	}
};