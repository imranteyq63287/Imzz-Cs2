#include "eprediction.hpp"
#include "../../sdk/modules/c_modules.hpp"

void c_eng_pred::run() {
	if (!interfaces::engine_client->is_in_game() || !interfaces::engine_client->is_connected() || !g_ctx.local->is_alive())
		return;

	c_user_cmd* user_cmd = g_ctx.user_cmd;
	if (!user_cmd)
		return;

	i_network_client* network_client = interfaces::m_network_client_service->get_network_client();
	if (!network_client)
		return;

	if (!interfaces::global_vars)
		return;

	/* store all predicted info there */
	m_pred_data.m_absolute_frame_start_time_std_dev = interfaces::global_vars->absolute_frame_start_time_std_dev;
	m_pred_data.m_absolute_frame_time = interfaces::global_vars->absolute_frame_time;
	m_pred_data.m_current_time = interfaces::global_vars->current_time;
	m_pred_data.m_tick_base = g_ctx.local_controller->tick_base();

	if (auto active_weapon = g_ctx.local->get_active_weapon()) {
		active_weapon->update_accuracy_penality();

		m_pred_data.m_spread = active_weapon->get_spread();
		m_pred_data.m_inaccuracy = active_weapon->get_inaccuracy();
	}

	this->predict();
}

void c_eng_pred::predict() {
	c_networked_client_info* client_info = interfaces::engine_client->get_networked_client_info();
	if (!client_info)
		return;

	if (!client_info->m_local_data)
		return;

	m_pred_data.m_eye_pos = client_info->m_local_data->m_eye_pos;
	m_pred_data.m_player_tick = client_info->m_player_tick_count;
	m_pred_data.m_player_tick_fraction = client_info->m_player_tick_fraction;
	m_pred_data.m_render_tick = client_info->m_render_tick;
	m_pred_data.m_render_tick_fraction = client_info->m_render_tick_fraction;

	if (m_pred_data.m_player_tick_fraction > (1.f - 0.0099999998f))
		m_pred_data.m_tick_base++;

	g_ctx.local->movement_services()->set_prediction_command(g_ctx.user_cmd);

	/* premium fixes */
	g_ctx.local_controller->physics_run_think();

	m_initialized = true;
}
void c_eng_pred::end() {
	if (!interfaces::engine_client->is_in_game() || !interfaces::engine_client->is_connected() || !g_ctx.local->is_alive())
		return;

	if (!m_initialized)
		return;

	i_network_client* network_client = interfaces::m_network_client_service->get_network_client();
	if (!network_client)
		return;

	c_networked_client_info* client_info = interfaces::engine_client->get_networked_client_info();
	if (!client_info)
		return;

	if (!client_info->m_local_data)
		return;

	static auto interpolate_shoot_position = reinterpret_cast<void* (__fastcall*)(void*, vec3_t*, int, float)>(g_opcodes->scan_absolute("client.dll", "E8 ? ? ? ? 41 8B 86 ? ? ? ? C1 E8 ? A8 ? 0F 85", 0x1));

	g_ctx.local->movement_services()->reset_prediction_command();

	/* restore all predicted info there */
	interfaces::global_vars->absolute_frame_start_time_std_dev = m_pred_data.m_absolute_frame_start_time_std_dev;
	interfaces::global_vars->absolute_frame_time = m_pred_data.m_absolute_frame_time;
	interfaces::global_vars->current_time = m_pred_data.m_current_time;

	m_pred_data.m_shoot_tick = g_ctx.local_controller->tick_base() - 1;
	interpolate_shoot_position(g_ctx.local->weapon_services(), &m_pred_data.m_eye_pos, m_pred_data.m_shoot_tick, 0.0099999998f);

	g_ctx.local_controller->tick_base() = m_pred_data.m_tick_base;
}
/*void endold() {
	//std::cout << "[end] Function start\n";

	if (!interfaces::engine_client) {
		std::cout << "[end] engine_client is nullptr\n";
		return;
	}

	if (!interfaces::engine_client->is_in_game()) {
		std::cout << "[end] Not in game\n";
		return;
	}

	if (!interfaces::engine_client->is_connected()) {
		std::cout << "[end] Not connected\n";
		return;
	}

	if (!g_ctx.local) {
		std::cout << "[end] g_ctx.local is nullptr\n";
		return;
	}

	if (!g_ctx.local->is_alive()) {
		std::cout << "[end] g_ctx.local is dead\n";
		return;
	}

	if (!m_initialized) {
		std::cout << "[end] Not initialized\n";
		return;
	}

	if (!interfaces::m_network_client_service) {
		std::cout << "[end] network_client_service is nullptr\n";
		return;
	}

	i_network_client* network_client = interfaces::m_network_client_service->get_network_client();
	if (!network_client) {
		std::cout << "[end] network_client is nullptr\n";
		return;
	}

	if (!interfaces::engine_client) {
		std::cout << "[end] engine_client is nullptr (duplicate check)\n";
		return;
	}

	c_networked_client_info* client_info = interfaces::engine_client->get_networked_client_info();
	if (!client_info) {
		std::cout << "[end] client_info is nullptr\n";
		return;
	}

	if (!client_info->m_local_data) {
		std::cout << "[end] client_info->m_local_data is nullptr\n";
		return;
	}

	if (!g_opcodes) {
		std::cout << "[end] g_opcodes is nullptr\n";
		return;
	}

	if (!gc_modules) {
		std::cout << "[end] gc_modules is nullptr\n";
		return;
	}

	auto interpolate_shoot_position = reinterpret_cast<void* (__fastcall*)(void*, vec3_t*, int, float)>(
		g_opcodes->scan_absolute("client.dll",
			"E8 ? ? ? ? 41 8B 86 ? ? ? ? C1 E8 ? A8 ? 0F 85", 0x1)
		);

	if (!interpolate_shoot_position) {
		std::cout << "[end] interpolate_shoot_position is nullptr\n";

		return;
	}
		//std::cout << gc_modules->m_modules.client_dll.get_name();

	if (!g_ctx.local->movement_services()) {
		std::cout << "[end] movement_services is nullptr\n";
		return;
	}

	g_ctx.local->movement_services()->reset_prediction_command();

	//std::cout << "[end] Restoring predicted values\n";

	interfaces::global_vars->absolute_frame_start_time_std_dev = m_pred_data.m_absolute_frame_start_time_std_dev;
	interfaces::global_vars->absolute_frame_time = m_pred_data.m_absolute_frame_time;
	interfaces::global_vars->current_time = m_pred_data.m_current_time;

	if (!g_ctx.local_controller) {
		std::cout << "[end] local_controller is nullptr\n";
		return;
	}

	m_pred_data.m_shoot_tick = g_ctx.local_controller->tick_base() - 1;

	if (!g_ctx.local->weapon_services()) {
		std::cout << "[end] weapon_services is nullptr\n";
		return;
	}

	//std::cout << "[end] Calling interpolate_shoot_position\n";
	interpolate_shoot_position(g_ctx.local->weapon_services(), &m_pred_data.m_eye_pos, m_pred_data.m_shoot_tick, 0.0099999998f);

	//std::cout << "[end] Restoring tick_base\n";
	g_ctx.local_controller->tick_base() = m_pred_data.m_tick_base;

	//std::cout << "[end] Function end\n";
} */
