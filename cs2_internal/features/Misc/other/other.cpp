#include "other.h"
#include "../../config_system.h"
#include "../../../sdk/utils/render/render.h"
#include "../../menu/gui_base/ui.h"
#include "../../Aimbot/Legit/legit.h"
#include "../../Aimbot/Rage/ragebot.h"
#include "../../Aimbot/Rage/Wall Penetrate/auto_wall.h"

void c_other::spectator(c_cs_player_controller* player_controller, c_cs_player_pawn* player_pawn)
{
	if (!interfaces::engine_client->is_in_game() || !interfaces::engine_client->is_connected())
		return;

	std::vector<c_specs> specs_temp{};
	std::vector<std::string> debug_logs{};

	auto ObserverPawn = player_controller->get_observer_pawn();
	if (!ObserverPawn)
	{
		debug_logs.push_back("[ObserverPawn] is nullptr");
	}
	else
	{
		debug_logs.push_back("[ObserverPawn] valid");

		auto obs_service = ObserverPawn->observer_service();
		if (!obs_service)
		{
			debug_logs.push_back("[ObserverPawn] observer_service is null");
		}
		else
		{
			debug_logs.push_back("[ObserverPawn] observer_service valid");

			int target_index = obs_service->observer_target().get_entry_index();
			debug_logs.push_back("[ObserverPawn] target index: " + std::to_string(target_index));

			auto* PPawn = reinterpret_cast<c_cs_player_pawn*>(interfaces::entity_system->get_base_entity(target_index));
			if (PPawn == g_ctx.local)
			{
				auto controller_entity = interfaces::entity_system->get_base_entity(player_pawn->get_controller_handle().get_entry_index());
				if (!controller_entity)
				{
					debug_logs.push_back("[Spectator] controller_handle entity is nullptr");
				}
				else
				{
					auto* pPlayer2 = reinterpret_cast<c_cs_player_controller*>(controller_entity);
					if (!pPlayer2)
					{
						debug_logs.push_back("[Spectator] cast to c_cs_player_controller failed");
					}
					else if (!player_pawn->is_alive())
					{
						std::string name = pPlayer2->player_name();
						debug_logs.push_back("[Spectator] spectator found: " + name);
						specs_temp.push_back({ name });
					}
				}
			}
		}
	}

	// Отрисовка окна
	ImGui::PushStyleColor(ImGuiCol_TitleBgActive, ImVec4(0.10f, 0.10f, 0.10f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.0f, 0.0f, 0.0f, 1.0f));

	ImGuiStyle& style = ImGui::GetStyle();
	style.WindowTitleAlign = ImVec2(0, 0.5f);
	ImGui::SetNextWindowBgAlpha(0.45f);

	ImGui::Begin("spectators", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);
	ImGui::SetWindowSize({ 220, 140 });

	for (const auto& spec : specs_temp)
		ImGui::Text(spec.name.c_str());

	if (!debug_logs.empty())
	{
		ImGui::Separator();
		ImGui::TextColored(ImVec4(1, 0.4f, 0.4f, 1), "Debug:");
		for (const auto& msg : debug_logs)
			ImGui::TextUnformatted(msg.c_str());
	}

	ImGui::End();
	ImGui::PopStyleColor(2);
}

void c_other::RunSpectator()
{
	
}
void c_other::handswitch()
{
	if (!interfaces::engine_client->is_in_game() || !interfaces::engine_client->is_connected())
		return;

	if (!g_ctx.local || !g_ctx.local_controller)
		return;

	if (!g_ctx.local->is_alive())
		return;

	if (!config.extra.handswitch)
		return;

	int is_left_handed = g_ctx.local->left_handed();

	weapon_category_t category = get_weapon_category_by_defindex(get_active_weapon_def_index());

	if (category == weapon_category_t::knife)
	{
		if (handswitched == 0)
		{
			if (is_left_handed == 0)
				interfaces::engine_client->client_cmd_unrestricted("switchhandsleft");
			else
				interfaces::engine_client->client_cmd_unrestricted("switchhandsright");

			handswitched = 1;
		}
	}
	else
	{
		if (handswitched == 1)
		{
			interfaces::engine_client->client_cmd_unrestricted("switchhands");
			handswitched = 0;
		}
	}
}
void c_other::hidehud()
{
	interfaces::engine_client->client_cmd_unrestricted("cl_draw_only_deathnotices 1");
}
void c_other::unhidehud()
{
	interfaces::engine_client->client_cmd_unrestricted("cl_draw_only_deathnotices 0");
}

void c_other::downloadsound(std::string& path)
{
	std::vector<std::pair<std::string, std::string>> sounds = {
	{ "Cod", "https://raw.githubusercontent.com/tickcount/hitsounds/master/cod.vsnd_c" },
	{ "Click", "https://raw.githubusercontent.com/tickcount/hitsounds/master/click.vsnd_c" },
	{ "Bubble", "https://raw.githubusercontent.com/tickcount/hitsounds/master/bubble.vsnd_c" },
	{ "Bell", "https://raw.githubusercontent.com/tickcount/hitsounds/master/bell.vsnd_c" },
	{ "Switch", "https://raw.githubusercontent.com/tickcount/hitsounds/master/arena_switch.vsnd_c" },
	{ "Apple Pay", "https://raw.githubusercontent.com/tickcount/hitsounds/master/applepay.vsnd_c" },
	{ "Agpa", "https://raw.githubusercontent.com/tickcount/hitsounds/master/agpa2.vsnd_c" },
	{ "Bonk", "https://raw.githubusercontent.com/tickcount/hitsounds/master/bonk.vsnd_c" },
	{ "Fatality", "https://raw.githubusercontent.com/tickcount/hitsounds/master/fatality.vsnd_c" },
	{ "Hentai1", "https://raw.githubusercontent.com/tickcount/hitsounds/master/hentai1.vsnd_c" },
	{ "Hentai2", "https://raw.githubusercontent.com/tickcount/hitsounds/master/hentai2.vsnd_c" },
	{ "Hentai3", "https://raw.githubusercontent.com/tickcount/hitsounds/master/hentai3.vsnd_c" },
	{ "MC Hit", "https://raw.githubusercontent.com/tickcount/hitsounds/master/minecraft_hit.vsnd_c" },
	{ "MC XP Gain", "https://raw.githubusercontent.com/tickcount/hitsounds/master/minecraft_xp_gain.vsnd_c" },
	{ "Rust Headshot", "https://raw.githubusercontent.com/tickcount/hitsounds/master/rust_headshot.vsnd_c" },
	{ "Zelda", "https://raw.githubusercontent.com/tickcount/hitsounds/master/zelda.vsnd_c" }
	};

	for (const auto& [name, url] : sounds) {
		std::string filename = get_filename_from_url(url);
		std::string full_path = path + filename;

		if (std::filesystem::exists(full_path)) {
			std::cout << "[=] Already exists: " << name << " -> " << filename << std::endl;
			continue; 
		}

		if (download_file(url, full_path)) {
			std::cout << "[+] Dump: " << name << " -> " << filename << std::endl;
		}
		else {
			std::cout << "[-] Error for dump: " << name << std::endl;
		}
	}

}
inline int index[][2] = {
		{6,5},
		{27,26},
		{23,24},
		{26,25},
		{23,22},
		{25,1},
		{22,1},
		{1,2},
		{2,3},
		{3,4},
		{4,5},
		{5,13},
		{13,14},
		{14,15},
		{5,8},
		{8,9},
		{9,10}
};
void c_other::penitrations(ImDrawList* draw)
{
	if (!interfaces::engine_client->is_in_game() || !interfaces::engine_client->is_connected())
		return;

	if (!config.extra.pencross)
		return;

	if (!g_ctx.local || !g_ctx.local->get_active_weapon())
		return;

	if (!g_ragebot->m_best_target || !g_ragebot->m_best_target->m_pawn)
		return;

	auto pawn_ent = g_ragebot->m_best_target->m_pawn;
	if (pawn_ent == g_ctx.local)
		return;

	auto weapon_data = g_ctx.local->get_active_weapon()->get_weapon_data();
	if (!weapon_data || (uintptr_t)weapon_data <= 0x111)
		return;

	auto local_ent_base = interfaces::entity_system->get_base_entity(g_ctx.local->get_ref_ehandle().get_entry_index());
	if (!local_ent_base)
		return;

	if (!pawn_ent->game_scene_node())
		return;

	auto skeleton = pawn_ent->game_scene_node()->get_skeleton_instance();
	if (!skeleton)
		return;

	// Например, стреляем в голову
	vec3_t target_pos = skeleton->m_model_state().bones[6].position; // 6 — голова (можно заменить индекс)

	// Трассировка
	float dmg = g_auto_wall->penetration_damage(
		g_ctx.local->get_eye_pos(),
		target_pos,
		local_ent_base,
		g_ctx.local,
		pawn_ent,
		pawn_ent,
		pawn_ent->item_services()->has_heavy_armor(),
		pawn_ent->team_num(),
		weapon_data
	);

	// Центр экрана
	ImVec2 screen_center = ImVec2(
		g_render->m_screen_size.x * 0.5f,
		g_render->m_screen_size.y * 0.5f
	);

	// Цвет в зависимости от результата
	ImU32 color = (dmg >= config.ragebot.min_damage) ? IM_COL32(0, 255, 0, 255) : IM_COL32(255, 0, 0, 255);

	draw->AddCircleFilled(screen_center, 6.0f, color, 32);
}
static bool is_entity_visible(c_cs_player_pawn* local_pawn, c_base_entity* target_entity)
{
	if (!local_pawn || !target_entity) {
		debug_text(WHITE_COLOR, "\n[Visibility] Invalid local or target entity.");
		return false;
	}

	vec3_t local_eyes = local_pawn->get_eye_pos();
	auto game_scene_node = target_entity->game_scene_node();
	if (!game_scene_node) {
		debug_text(WHITE_COLOR, "\n[Visibility] Target entity has no game_scene_node.");
		return false;
	}

	vec3_t target_pos = game_scene_node->vec_abs_origin();

	vec3_t points[] = {
		target_pos + vec3_t(0, 0, 16.f), // top
		target_pos                      // center
	};

	game_trace_t trace;
	trace_filter_t filter(0x1C3003, local_pawn, nullptr, 4);
	ray_t ray;

	for (const auto& point : points)
	{
		interfaces::engine_trace->trace_shape(&ray, local_eyes, point, &filter, &trace);

		if (trace.m_hit_entity == target_entity && trace.m_fraction >= 0.97f) {
			debug_text(WHITE_COLOR, "\n[Visibility] Visible point found.");
			return true;
		}
	}

	debug_text(WHITE_COLOR, "\n[Visibility] All trace checks failed. Target not visible.");
	return false;
}
void c_other::active_molotov(ImDrawList* draw)
{
	if (!interfaces::engine_client->is_in_game() || !interfaces::engine_client->is_connected())
		return;
	if (config.world.granades)
	{
		for (int i{}; i <= interfaces::entity_system->get_highest_entiy_index(); i++) {
			auto entity = interfaces::entity_system->get_base_entity(i);

			if (!entity)
				continue;

			if (!entity->find_class(fnv1a::hash_64("C_Inferno")))
				continue;

			c_inferno* molotov = reinterpret_cast<c_inferno*>(entity);

			if (!molotov)
				continue;

			if (molotov->fire_count() != 0)
			{
				const float max_time = 22.f;
				vec3_t out3d;
				if (!g_render->world_to_screen(molotov->fire_positions(), out3d))
					return;

				vec2_t out(out3d.x, out3d.y);

				ImVec2 pos = { out.x, out.y };

				if (!g_ctx.local)
					return;
				float distance = (g_ctx.local->get_eye_pos() - molotov->fire_positions()).length();

				const float base_width = 25.0f;
				const float base_height = 2.0f;
				const float base_padding = 4.0f;

				float distance_scale = 1.0f;
				if (distance > 100.f) {
					distance_scale = (100.f / distance > 0.5f) ? (100.f / distance) : 0.5f;
				}

				float width = base_width * distance_scale;
				float height = base_height * distance_scale;
				float padding = base_padding * distance_scale;

				if (distance <= 1000)
				{
					g_render->granade_draw("T_MOLOTOV", pos, gui.accent_color.to_im_color(), gui.bg.to_im_color(), 0, 0, false, 25, 255);
				}


			}
		}
	}
}
void c_other::active_smoke(ImDrawList* draw)
{
	if (!interfaces::engine_client->is_in_game() || !interfaces::engine_client->is_connected())
		return;
	if (config.world.granades)
	{
		for (int i{}; i <= interfaces::entity_system->get_highest_entiy_index(); i++) {
			auto entity = interfaces::entity_system->get_base_entity(i);

			if (!entity)
				continue;

			if (!entity->find_class(fnv1a::hash_64("C_SmokeGrenadeProjectile")))
				continue;

			c_smoke_grenade_projectile* smoke = reinterpret_cast<c_smoke_grenade_projectile*>(entity);

			if (!smoke)
				continue;

			if (smoke->m_did_smoke_effect())
			{
				const float max_time = 22.f;
				vec3_t out3d;
				if (!g_render->world_to_screen(smoke->get_smoke_pos(), out3d))
					return;

				vec2_t out(out3d.x, out3d.y);

				float spawn_time = INTERVAL_PER_TICK * smoke->get_smoke_tick();
				float current_time = (spawn_time + max_time) - interfaces::global_vars->current_time;
				ImVec2 pos = { out.x, out.y };

				if (!g_ctx.local)
					return;
				float distance = (g_ctx.local->get_eye_pos() - smoke->get_smoke_pos()).length();

				const float base_width = 25.0f;
				const float base_height = 2.0f;
				const float base_padding = 4.0f;

				float distance_scale = 1.0f;
				if (distance > 100.f) {
					distance_scale = (100.f / distance > 0.5f) ? (100.f / distance) : 0.5f;
				}

				float width = base_width * distance_scale;
				float height = base_height * distance_scale;
				float padding = base_padding * distance_scale;

				float filled_width = (current_time / max_time) * width;
				if (distance <= 700)
				{
				g_render->granade_draw("SMOKE", pos, gui.accent_color.to_im_color(), gui.bg.to_im_color(), current_time, max_time, true, 25, 255);
				}


			}
		}
	}
}
void c_other::active_grenade_timers(ImDrawList* draw) {
	if (!interfaces::engine_client->is_in_game() || !interfaces::engine_client->is_connected())
		return;

	if (!config.world.granades || grenade_effects.empty())
		return;

	float current_time = interfaces::global_vars->current_time;

	for (int i = 0; i < grenade_effects.size(); ++i) {
		auto& effect = grenade_effects[i];

		float duration = (effect.type == "smoke") ? SMOKE_DURATION : MOLOTOV_DURATION;
		const char* label = (effect.type == "smoke") ? "SMOKE" : "T_MOLOTOV";

		float time_elapsed = current_time - effect.spawn_time;
		float time_remaining = duration - time_elapsed;

		if (time_remaining <= 0.f) {
			grenade_effects.erase(grenade_effects.begin() + i);
			--i;
			continue;
		}

		vec3_t screen3d;
		if (!g_render->world_to_screen(effect.position, screen3d))
			continue;

		vec2_t screen2d(screen3d.x, screen3d.y);
		ImVec2 pos(screen2d.x, screen2d.y);

		if (!g_ctx.local)
			continue;

		float distance = (g_ctx.local->get_eye_pos() - effect.position).length();
		if (distance > 1000.f)
			continue;

		g_render->granade_draw(
			label,
			pos,
			gui.accent_color.to_im_color(),
			gui.bg.to_im_color(),
			time_remaining,
			duration,
			true, // показать прогресс бар только у смока
			25, 255
		);
	}
}

void c_other::debug_inferno_log()
{
	for (int i = 0; i <= interfaces::entity_system->get_highest_entiy_index(); ++i)
	{
		auto entity = interfaces::entity_system->get_base_entity(i);
		if (!entity)
			continue;

		if (!entity->find_class(fnv1a::hash_64("C_Inferno")))
			continue;

		auto inferno = reinterpret_cast<c_inferno*>(entity);

		const int count = inferno->fire_count();
		const float half_width = 1.f;
		const float height = 1.f;
		const vec3_t min = inferno->bounds_min();
		const vec3_t max = inferno->bounds_max();

		// Если все поля нулевые — пропускаем
		if (count <= 0 && half_width == 0.f && height == 0.f &&
			min.is_zero_const() && max.is_zero_const())
			continue;

		std::cout << "===========================\n";
		std::cout << "[C_Inferno Entity ID: " << i << "]\n";

		if (count > 0)
			std::cout << "fire_count: " << count << "\n";

		if (half_width > 0.f)
			std::cout << "max_fire_half_width: " << half_width << "\n";

		if (height > 0.f)
			std::cout << "max_fire_height: " << height << "\n";

		if (!min.is_zero_const())
			std::cout << "bounds_min: " << min.x << ", " << min.y << ", " << min.z << "\n";

		if (!max.is_zero_const())
			std::cout << "bounds_max: " << max.x << ", " << max.y << ", " << max.z << "\n";

		if (count > 0)
		{
			const int safe_count = std::clamp(count, 0, 64);
			for (int j = 0; j < safe_count; ++j)
			{
				const vec3_t& pos = inferno->fire_positions();
				const bool burning = inferno->fire_is_burning();

				if (!pos.is_zero_const())
				{
					std::cout << "  🔥 [" << j << "] pos: "
						<< pos.x << ", " << pos.y << ", " << pos.z
						<< " | is_burning: " << (burning ? "true" : "false") << "\n";
				}
			}
		}

		std::cout << "\n";
	}
}
