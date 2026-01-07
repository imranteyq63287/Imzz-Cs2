#include "hooks.h"
#include "../utils/virual.h"
#include "../interfaces/interfaces.h"
#include "directx.h"
#include "../../features/menu/menu.h"
#include "../utils/render/render.h"
#include "../entity/entity.h"
#include "../classes/c_scene_light.h"
#include "../../features/config_system.h"
#include "../../features/visuals/main/overlay.h"
#include "../../features/Misc/Movement/movement.h"
#include "../../features/Aimbot/Rage/antiaim.h"
#include "../utils/math.h"
#include "../../features/Aimbot/Rage/ragebot.h"
#include "../../features/skins/skins.h"
#include "../../sdk/classes/c_view_setup.h"
#include "../../features/Aimbot/Legit/legit.h"
#include "../../features/engine prediction/eprediction.hpp"
#include "../../features/Misc/other/other.h"
#include "../../features/Misc/walkbot/walkbot.h"
#include "../../Bind System/key.h"
#include "../../features/visuals/main/weather.hpp"
#include "../../features/visuals/main/custom_sky.h"

bool hooks::initialize()
{
	MH_Initialize();

	g_skins->dump();

	if (directx::m_present_address)
		present::m_present.hook(directx::m_present_address, present::hook);

	create_move::m_create_move.hook(vmt::get_v_method(interfaces::csgo_input, 5), create_move::hook);
	enable_cursor::m_enable_cursor.hook(vmt::get_v_method(interfaces::input_system, 76), enable_cursor::hook);
	validate_view_angles::m_validate_view_angles.hook(vmt::get_v_method(interfaces::csgo_input, 7), validate_view_angles::hook);
	mouse_input_enabled::m_mouse_input_enabled.hook(vmt::get_v_method(interfaces::csgo_input, 19), mouse_input_enabled::hook);

	//glow_obj::m_glow_obj.hook(reinterpret_cast<uintptr_t>(utils::ResolveRelativeAddress((uint8_t*)utils::find_pattern(g_modules.client, "E8 ? ? ? ? F3 0F 10 BE ? ? ? ? 48 8B CF"), 0x1, 0x5)), glow_obj::hook);

	//is_gloving::m_dis_gloving.hook(reinterpret_cast<uintptr_t>(utils::ResolveRelativeAddress((uint8_t*)utils::find_pattern(g_modules.client, "E8 ? ? ? ? 33 DB 84 C0 0F 84 ? ? ? ? 48 8B 4F"), 0x1, 0x5)), is_gloving::hook);

	frame_stage_notify::m_frame_stage_notify.hook(utils::find_pattern(g_modules.client, XOR_STR("48 89 5C 24 ? 57 48 81 EC ? ? ? ? 48 8B F9 8B DA")), frame_stage_notify::hook);

	override_view::m_override_view.hook(utils::find_pattern(g_modules.client, XOR_STR("48 89 5C 24 ? 48 89 6C 24 ? 48 89 74 24 ? 57 41 56 41 57 48 83 EC ? 48 8B FA E8")), override_view::hook);

	on_add_entity::m_on_add_entity.hook(utils::find_pattern(g_modules.client, XOR_STR("48 89 74 24 ? 57 48 83 EC ? 41 B9 ? ? ? ? 41 8B C0 41 23 C1 48 8B F2 41 83 F8 ? 48 8B F9 44 0F 45 C8 41 81 F9 ? ? ? ? 73 ? FF 81")), on_add_entity::hook);
	on_remove_entity::m_on_remove_entity.hook(utils::find_pattern(g_modules.client, XOR_STR("48 89 74 24 ? 57 48 83 EC ? 41 B9 ? ? ? ? 41 8B C0 41 23 C1 48 8B F2 41 83 F8 ? 48 8B F9 44 0F 45 C8 41 81 F9 ? ? ? ? 73 ? FF 89")), on_remove_entity::hook);

	on_level_init::m_on_level_init.hook(utils::find_pattern(g_modules.client, XOR_STR("40 55 56 41 56 48 8D 6C 24 ? 48 81 EC ? ? ? ? 48 8B 0D")), on_level_init::hook);
	on_level_shutdown::m_on_level_shutdown.hook(utils::find_pattern(g_modules.client, XOR_STR("48 83 EC ? 48 8B 0D ? ? ? ? 48 8D 15 ? ? ? ? 45 33 C9 45 33 C0 48 8B 01 FF 50 ? 48 85 C0 74 ? 48 8B 0D ? ? ? ? 48 8B D0 4C 8B 01 41 FF 50 ? 48 83 C4")), on_level_shutdown::hook);

	draw_skybox_array::m_draw_skybox_array.hook(utils::find_pattern(g_modules.scenesystem, XOR_STR("45 85 C9 0F 8E ? ? ? ? 4C 8B DC 55")), draw_skybox_array::hook);

	draw_light_scene::m_draw_light_scene.hook(utils::find_pattern(g_modules.scenesystem, XOR_STR("8B 02 89 01 F2 0F 10 42 ? F2 0F 11 41 ? 8B 42 ? 89 41 ? F2 0F 10 42")), draw_light_scene::hook);
	remove_intro::m_remove_intro.hook(utils::find_pattern(g_modules.client, XOR_STR("48 83 EC 28 45 0F B6")), remove_intro::hook);
	draw_viewmodel::m_draw_viewmodel.hook(utils::find_pattern(g_modules.client, XOR_STR("48 89 5C 24")), draw_viewmodel::hook);
	get_field_of_view::m_get_field_of_view.hook(utils::find_pattern(g_modules.client, XOR_STR("40 57 48 83 EC ? 48 8B F9 E8 ? ? ? ? 48 85 C0 74")), get_field_of_view::hook);

	update_aggregate_scene_object::m_update_aggregate_scene_object.hook(utils::find_pattern(g_modules.scenesystem, XOR_STR("48 89 ?? ?? ??")), update_aggregate_scene_object::hook);

	draw_aggregate_scene_object::m_draw_aggregate_scene_object.hook(utils::find_pattern(g_modules.scenesystem, XOR_STR("48 8B C4 4C 89 40 ? 48 89 50 ? 55 53 41 57")), draw_aggregate_scene_object::hook);

	smoke_voxel_draw::m_smoke_voxel_draw.hook(utils::find_pattern(g_modules.client, XOR_STR("48 89 5C 24 ? 48 89 6C 24 ? 48 89 74 24 ? 57 41 56 41 57 48 83 EC ? 48 8B 9C 24 ? ? ? ? 4D 8B F8")), smoke_voxel_draw::hook);

	removescope::m_removescope.hook(utils::find_pattern(g_modules.client, XOR_STR("4C 8B DC 53 56 57 48 83 EC")), removescope::hook);

	//auto_accept::m_auto_accept.hook(utils::find_pattern(g_modules.client, XOR_STR("40 56 57 41 57 48 83 EC ? 48 8B 3D ? ? ? ? 4D 85 C0")), auto_accept::hook);

	draw_sniper_crosshair::m_draw_sniper_crosshair.hook(utils::find_pattern(g_modules.client, XOR_STR("48 89 5C 24 08 57 48 83 EC 20 48 8B D9 E8 ?? ?? ?? ?? 48 85")), draw_sniper_crosshair::hook);
	remove_legs::m_remove_legs.hook(utils::find_pattern(g_modules.client, XOR_STR("40 55 53 56 41 56 41 57 48 8D AC 24 ? ? ? ? 48 81 EC ? ? ? ? F2 0F 10 42")), remove_legs::hook);

	return true;
}

void __fastcall hooks::auto_accept::hook(void* unk, const char* event_name, void* unk1, float unk2)
{
	static auto original = hooks::auto_accept::m_auto_accept.get_original<decltype(&hook)>();

	if (interfaces::engine_client->is_in_game())
		return original(unk, event_name, unk1, unk2);

	if (!config.extra.auto_accept)
		return original(unk, event_name, unk1, unk2);

	if (std::string(event_name).find("popup_accept_match_found") != std::string::npos) {
		using set_player_ready_t = bool(__fastcall*)(void*, const char*);
		static const auto fn = reinterpret_cast<set_player_ready_t>(utils::find_pattern(g_modules.client, XOR_STR("40 53 48 83 EC ? 48 8B DA 48 8D 15 ? ? ? ? 48 8B CB FF 15 ? ? ? ? 85 C0 75 ? 8D 50")));
		fn(nullptr, "");
	}

	return original(unk, event_name, unk1, unk2);
}

bool __fastcall hooks::draw_sniper_crosshair::hook(c_cs_weapon_base* pWeaponBaseGun)
{
	static auto original = hooks::draw_sniper_crosshair::m_draw_sniper_crosshair.get_original<decltype(&hook)>();

	if (!config.extra.sniper_cross)
		return original(pWeaponBaseGun);

	if (!g_ctx.local)
		return original(pWeaponBaseGun);

	return !g_ctx.local->scoped();
}


void __fastcall hooks::create_move::hook(c_csgo_input* rcx, int slot, bool active)
{
	static auto original = m_create_move.get_original<decltype(&hook)>();
	original(rcx, slot, active);

	c_user_cmd* user_cmd = rcx->get_user_cmd(g_ctx.local_controller);

	if (!g_ctx.local || !g_ctx.local_controller || !user_cmd)
		return;

	CBaseUserCmdPB* base_user_cmd = user_cmd->pb.mutable_base();
	if (!base_user_cmd)
		return;

	base_user_cmd->clear_subtick_moves();

	g_ctx.user_cmd = user_cmd;

	const vec3_t old_view_angles = { base_user_cmd->viewangles().x(),base_user_cmd->viewangles().y(),base_user_cmd->viewangles().z() };

	menu::on_create_move();

	g_other->handswitch();

	g_movement->bunnyhop(user_cmd);
	g_movement->duck_jump(user_cmd);

	g_antiaim->antiaim(user_cmd);

	float actual_yaw = user_cmd->pb.mutable_base()->viewangles().y();

	g_movement->auto_strafe(user_cmd, old_view_angles.y);
	g_movement->air_strafe(user_cmd);

	//g_prediction->run();

	g_movement->auto_pick(user_cmd);

	g_ragebot->ragebot(user_cmd);
	//g_ragebot->run(rcx, user_cmd);
	g_legitbot->legitbot(user_cmd);

	g_ragebot->double_tap(user_cmd, g_ctx.local->get_active_weapon());
	g_ragebot->check_doubletap_status(g_ctx.local->get_active_weapon());

	//g_prediction->end();

	g_antiaim->fix_movement(user_cmd, old_view_angles);
}

void hooks::smoke_voxel_draw::hook(__int64 a1, __int64* a2, int a3, int a4, __int64 a5, __int64 a6)
{
	static auto original = m_smoke_voxel_draw.get_original< decltype(&hook) >();

	if (config.extra.removals[3])
		return;

	original(a1, a2, a3, a4, a5, a6);
}

float hooks::get_field_of_view::hook(void* a1)
{
	static auto original = m_get_field_of_view.get_original<decltype(&hook)>();

	if (fov_hook_debug)
		std::printf("[FOV Hook] Called\n");

	if (!interfaces::engine_client->is_in_game() || !interfaces::engine_client->is_connected())
	{
		if (fov_hook_debug)
			std::printf("[FOV Hook] Not in game or not connected\n");

		return original(a1);
	}

	if (!g_ctx.local || !g_ctx.local_controller)
	{
		if (fov_hook_debug)
			std::printf("[FOV Hook] Local player or controller is null\n");

		return original(a1);
	}

	if (!config.extra.removals[1])
	{
		if (fov_hook_debug)
			std::printf("[FOV Hook] Zoom removal not enabled\n");

		return original(a1);
	}

	auto weapon = g_ctx.local->get_active_weapon();
	if (!weapon)
	{
		if (fov_hook_debug)
			std::printf("[FOV Hook] Weapon is null\n");

		return original(a1);
	}

	int zoom = weapon->zoom_level();

	if (fov_hook_debug)
		std::printf("[FOV Hook] Zoom level: %d\n", zoom);

	auto category = get_weapon_category_by_defindex(get_active_weapon_def_index());

	if (fov_hook_debug)
		std::printf("[FOV Hook] Weapon category: %d\n", static_cast<int>(category));

	if (category != weapon_category_t::scout && category != weapon_category_t::rifles)
	{
		if (fov_hook_debug)
			std::printf("[FOV Hook] Not applicable weapon category, using desiredfov\n");

		return config.extra.desiredfov;
	}

	if (zoom == 0)
	{
		if (fov_hook_debug)
			std::printf("[FOV Hook] Unzoomed, using desiredfov: %.1f\n", config.extra.desiredfov);

		return config.extra.desiredfov;
	}

	if (zoom == 1)
	{
		if (fov_hook_debug)
			std::printf("[FOV Hook] Zoom level 1, using fov: %.1f\n", config.extra.desired_fov_zoom_level_1);

		return config.extra.desired_fov_zoom_level_1;
	}

	if (zoom == 2)
	{
		if (fov_hook_debug)
			std::printf("[FOV Hook] Zoom level 2, using fov: %.1f\n", config.extra.desired_fov_zoom_level_2);

		return config.extra.desired_fov_zoom_level_2;
	}

	if (fov_hook_debug)
		std::printf("[FOV Hook] Fallback to original\n");

	return original(a1);
}

void hooks::remove_intro::hook(std::uintptr_t a1, std::uintptr_t a2, char* a3)
{
	static auto original = m_remove_intro.get_original< decltype(&hook) >();
	if (config.extra.removals[4])
		return;
	original(a1, a2, a3);
}

void hooks::remove_legs::hook(void* a1, void* a2, void* a3, void* a4, void* a5)
{
	static auto original = m_remove_legs.get_original< decltype(&hook) >();
	if (!config.extra.removals[0])
		original(a1, a2, a3, a4, a5);
}

void hooks::removescope::hook(void* a1, void* a2) {
	static auto original = m_removescope.get_original< decltype(&hook) >();

	if (!config.extra.removals[2])
		return original(a1, a2);

	else
		return;

	return original(a1, a2);
}

void hooks::draw_viewmodel::hook(float* a1, float* offsets, float* fov)
{
	static auto original = m_draw_viewmodel.get_original< decltype(&hook) >();
	if (config.extra.viewmodel)
	{
		updatepos();
		offsets[0] = viewX;
		offsets[1] = viewY;
		offsets[2] = viewZ;
		*fov = viewFov;
	}
	else
	{
		original(a1, offsets, fov);
	}
}

bool hooks::mouse_input_enabled::hook(void* ptr)
{
	static auto original = m_mouse_input_enabled.get_original< decltype(&hook) >();
	return menu::was_opened ? false : original(ptr);
}

void* hooks::enable_cursor::hook(void* rcx, bool active)
{
	static auto original = m_enable_cursor.get_original< decltype(&hook) >();

	m_enable_cursor_input = active;

	if (menu::was_opened)
		active = false;

	return original(rcx, active);
}

void hooks::enable_cursor::unhook()
{
	static auto original = m_enable_cursor.get_original< decltype(&hook) >();

	original(interfaces::input_system, m_enable_cursor_input);
}

HRESULT hooks::present::hook(IDXGISwapChain* swap_chain, unsigned int sync_interval, unsigned int flags)
{
	static auto original = m_present.get_original<decltype(&hook)>();

	directx::start_frame(swap_chain);

	directx::new_frame();
	{
		g_render->update_background_drawlist(ImGui::GetBackgroundDrawList());

		menu::draw();

		ESP::esp();

		//g_other->active_molotov(ImGui::GetBackgroundDrawList());
		//g_other->active_smoke(ImGui::GetBackgroundDrawList());
		g_other->active_grenade_timers(ImGui::GetBackgroundDrawList());

		if (config.world_modulation.sparkeffects)
		{
			draw_effects();
		}

		g_legitbot->draw_fov_circle();
		//g_other->spectator(g_ctx.local_controller, g_ctx.local);

		{
			std::lock_guard<std::mutex> lock(multipoint_mutex);

			for (auto pair : multipoint_debug_positions)
			{
				vec3_t screen;
				vec3_t pos_copy = pair.first;

				if (g_render->world_to_screen(pos_copy, screen) && std::isfinite(screen.x) && std::isfinite(screen.y))
				{
					//ImGui::GetBackgroundDrawList()->AddCircleFilled(
						//ImVec2(screen.x, screen.y), g_ragebot->radius_head, IM_COL32(255, 255, 255, 255));

					if (!pair.second.empty()) {
						//ImGui::GetBackgroundDrawList()->AddText(
							//ImVec2(screen.x, screen.y - 12.f), IM_COL32(255, 0, 255, 255), pair.second.c_str());
					}
				}
			}

			multipoint_debug_positions.clear();
		}
	}

	directx::end_frame();

	return original(swap_chain, sync_interval, flags);
}

void hooks::validate_view_angles::hook(c_csgo_input* input, void* a2)
{
	static auto original = m_validate_view_angles.get_original< decltype(&hook) >();

	vec3_t view_angles = input->get_view_angles();

	original(input, a2);

	input->set_view_angles(view_angles);
}

void hooks::frame_stage_notify::hook(void* source_to_client, int stage)
{
	static auto original = m_frame_stage_notify.get_original< decltype(&hook) >();

	g_ctx.local = entity::get_local_player();
	g_ctx.local_controller = interfaces::entity_system->get_base_entity<c_cs_player_controller>(interfaces::engine_client->get_local_player_index());

	g_ctx.active_weapon = g_ctx.local ? g_ctx.local->get_active_weapon() : nullptr;
	g_ctx.weapon_data = g_ctx.active_weapon ? g_ctx.active_weapon->get_weapon_data() : nullptr;

	if (config.extra.model_t || config.extra.model_ct || config.extra.custom_models)
	{
		g_skins->agents(stage);
	}

	original(source_to_client, stage);

	switch (stage)
	{
	case FRAME_RENDER_START:
		if (config.extra.knife_changer)
			g_skins->knifes(stage);

		weather::update();
		break;
	case FRAME_RENDER_END:
		//for (auto it = ESP::m_dropped_weapons.begin(); it != ESP::m_dropped_weapons.end();)
		//{
		//	auto idx = it->handle.get_entry_index() & 0x7FFF;
		//	auto player = interfaces::entity_system->get_base_entity(idx);

		//	if (!player) {
		//		//debug_text(RED_COLOR, "[ESP] Removing: nullptr entity at index %d\n", idx);
		//		it = ESP::m_dropped_weapons.erase(it);
		//		continue;
		//	}

		//	if (!player->find_class(fnv1a::hash_64("C_BasePlayerWeapon"))) {
		//		//debug_text(RED_COLOR, "[ESP] Removing: entity %d is not weapon\n", idx);
		//		it = ESP::m_dropped_weapons.erase(it);
		//		continue;
		//	}

		//	if (player->owner_entity().is_valid()) {
		//		//debug_text(BLUE_COLOR, "[ESP] Removing: entity %d picked up\n", idx);
		//		it = ESP::m_dropped_weapons.erase(it);
		//		continue;
		//	}

		//	it++;
		//}

		//for (int i = 0; i <= interfaces::entity_system->get_highest_entiy_index(); i++) {
		//	auto entity = interfaces::entity_system->get_base_entity(i);

		//	if (!entity)
		//		continue;

		//	if (!entity->find_class(fnv1a::hash_64("C_BasePlayerWeapon")))
		//		continue;

		//	if (!entity->game_scene_node())
		//		continue;

		//	if (entity->owner_entity().is_valid())
		//		continue;

		//	auto handle = entity->get_ref_ehandle();

		//	auto find = std::find_if(ESP::m_dropped_weapons.begin(), ESP::m_dropped_weapons.end(),
		//		[handle](const c_dropped_weapons& x) {
		//			return x.handle == handle;
		//		});

		//	c_cs_weapon_base* weapon_ = (c_cs_weapon_base*)(entity);

		//	if (find != ESP::m_dropped_weapons.end()) {
		//		auto& weapon = *find;
		//		weapon.handle = handle;
		//		weapon.abs_origin = entity->game_scene_node()->vec_abs_origin();
		//		weapon.mins = entity->collision()->vec_mins();
		//		weapon.maxs = entity->collision()->vec_maxs();
		//		weapon.weapon = weapon_;

		//		if (weapon_->attribute_manager() && weapon_->attribute_manager()->item() && weapon_->attribute_manager()->item()->get_static_data()) {
		//			weapon.weapon_name = interfaces::localize->find_key(weapon_->attribute_manager()->item()->get_static_data()->m_item_base_name);
		//			weapon.szweapon_name = weapon_->attribute_manager()->item()->get_static_data()->m_item_base_name;
		//			weapon.item_index = weapon_->attribute_manager()->item()->item_definition_index();
		//			//debug_text(GREEN_COLOR, "[ESP] Updated weapon: entity %d -> %s [%d]\n", i, weapon.weapon_name.c_str(), weapon.item_index);
		//		}
		//		else {
		//			//debug_text(BLUE_COLOR, "[ESP] Updated weapon: entity %d (missing static data)\n", i);
		//		}
		//	}
		//	else {
		//		auto& weapon = ESP::m_dropped_weapons.emplace_back();
		//		weapon.weapon = weapon_;
		//		weapon.handle = handle;
		//		weapon.abs_origin = entity->game_scene_node()->vec_abs_origin();
		//		weapon.mins = entity->collision()->vec_mins();
		//		weapon.maxs = entity->collision()->vec_maxs();

		//		if (weapon_->attribute_manager() && weapon_->attribute_manager()->item() && weapon_->attribute_manager()->item()->get_static_data()) {
		//			weapon.weapon_name = interfaces::localize->find_key(weapon_->attribute_manager()->item()->get_static_data()->m_item_base_name);
		//			weapon.szweapon_name = weapon_->attribute_manager()->item()->get_static_data()->m_item_base_name;

		//			weapon.item_index = weapon_->attribute_manager()->item()->item_definition_index();
		//			//debug_text(GREEN_COLOR, "[ESP] Added weapon: entity %d -> %s [%d]\n", i, weapon.weapon_name.c_str(), weapon.item_index);
		//		}
		//		else {
		//			//debug_text(BLUE_COLOR, "[ESP] Added weapon: entity %d (missing static data)\n", i);
		//		}
		//	}
		//}

		if (g_ctx.local && g_ctx.local->is_alive() && config.extra.removals[5])
			g_ctx.local->flash_duration() = 0.f;
		if (config.extra.removals[6])
		{
			g_other->hidehud();
		}
		else
		{
			g_other->unhidehud();
		}

		g_ragebot->store_records(stage);

		if (config.extra.knife_changer)
			g_skins->knifes(stage);


		/*static auto draw = interfaces::cvar->get_by_name(("sv_grenade_trajectory_prac_pipreview"));
		static auto trailtime = interfaces::cvar->get_by_name(("sv_grenade_trajectory_prac_trailtime"));
		if (config.extra.nade_traces) {
			trailtime->set_float(3.f);
		}
		else {
			trailtime->set_float(0.f);

		}
		if (config.extra.nade_camera)
		{
			draw->set_bool(true);
		}
		else
		{
			draw->set_bool(false);
		}*/

		break;
	case FRAME_NET_UPDATE_END:
		break;
	case FRAME_SIMULATE_END:
		break;
	default:
		break;
	}
}

void hooks::override_view::hook(void* source_to_client, c_view_setup* view_setup)
{
	static auto original = m_override_view.get_original<decltype(&hook)>();
	original(source_to_client, view_setup);

	c_cs_player_pawn* local_player = g_ctx.local;
	if (!local_player)
		return;

	bool in_third_person = false;

	if (config.extra.thirdperson || (g_key_handler->is_pressed(config.extra.thirdperson_keybind, config.extra.thirdperson_keybind_style)))
		in_third_person = true;
	if (in_third_person && local_player->is_alive())
	{
		vec3_t adjusted_cam_view_angle = interfaces::csgo_input->get_view_angles();
		adjusted_cam_view_angle.x = -adjusted_cam_view_angle.x;

		view_setup->m_origin = math::calculate_camera_pos(local_player->get_eye_pos(), -config.extra.thirdpersondist, adjusted_cam_view_angle);

		ray_t ray{};
		game_trace_t trace{};
		trace_filter_t filter{ 0x1C3003, local_player, nullptr, 4 };

		vec3_t direction = (local_player->get_eye_pos() - view_setup->m_origin).normalized();
		vec3_t temp = view_setup->m_origin + direction * -10.f;

		if (interfaces::engine_trace->trace_shape(&ray, local_player->get_eye_pos(), view_setup->m_origin, &filter, &trace))
		{
			if (trace.m_hit_entity != nullptr)
				view_setup->m_origin = trace.m_pos + (direction * 10.f);
		}

		vec3_t pos = math::calculate_angles(view_setup->m_origin, local_player->get_eye_pos()).normalize_angle();
		view_setup->m_angles = vec3_t{ pos.x, pos.y };
	}

	if (config.extra.aspect_ratio != 3.f) {
		view_setup->m_some_flags |= 2;
		view_setup->m_aspect_ratio = config.extra.aspect_ratio;
	}
	else
		view_setup->m_some_flags &= ~2;
}


void* hooks::on_add_entity::hook(void* a1, c_entity_instance* entity_instance, int handle)
{
	static auto original = m_on_add_entity.get_original< decltype(&hook) >();

	entity::add_entity(entity_instance, handle);

	return original(a1, entity_instance, handle);
}

void* hooks::on_remove_entity::hook(void* a1, c_entity_instance* entity_instance, int handle)
{
	static auto original = m_on_remove_entity.get_original< decltype(&hook) >();

	entity::remove_entity(entity_instance, handle);

	return original(a1, entity_instance, handle);
}

void* hooks::on_level_init::hook(void* a1, const char* map_name)
{
	static auto original = m_on_level_init.get_original< decltype(&hook) >();

	entity::level_init();
	if (!interfaces::c_game_particle_manager_system)
	{
		using fn_t = i_particle_system * (__fastcall*)();
		static fn_t fn = reinterpret_cast<fn_t>(g_opcodes->scan("client.dll", ("48 8B 05 ? ? ? ? C3 CC CC CC CC CC CC CC CC 40 53")));
		interfaces::c_game_particle_manager_system = fn();
	}

	interfaces::m_particle_manager = interfaces::find_interface<i_particle_manager*>(g_modules.particle, XOR_STR("ParticleSystemMgr003"));
	interfaces::global_vars = *reinterpret_cast<c_global_vars**>(utils::find_pattern_rel(g_modules.client, "48 8B 05 ? ? ? ? 4C 8D 84 24 ? ? ? ? F3 0F 10 35 ? ? ? ? 48 8D 94 24 ? ? ? ? 48 8D 8C 24 ? ? ? ? C7 84 24 ? ? ? ? ? ? ? ? C7 84 24 ? ? ? ? ? ? ? ? F3 0F 10 40 ? F3 0F 5C 83", 0x3));

	debug_text(WHITE_COLOR, "[PARTICLE INIT] ParticleManager: %p\n", interfaces::m_particle_manager);
	debug_text(WHITE_COLOR, "[PARTICLE INIT] GameParticleManagerSystem: %p\n", interfaces::c_game_particle_manager_system);
	debug_text(WHITE_COLOR, "[PARTICLE INIT] global_vars: %p\n", interfaces::global_vars);

	return original(a1, map_name);
}

void* hooks::on_level_shutdown::hook(void* a1, const char* map_name)
{
	static auto original = m_on_level_shutdown.get_original< decltype(&hook) >();

	entity::level_shutdown();

	interfaces::m_particle_manager = nullptr;
	interfaces::c_game_particle_manager_system = nullptr;
	interfaces::global_vars = nullptr;

	if (!g_ragebot->m_aim_targets.empty())
		g_ragebot->m_aim_targets.clear();

	if (!g_ragebot->m_lag_records.empty())
		g_ragebot->m_lag_records.clear();

	debug_text(WHITE_COLOR, "[PARTICLE SHIT DOWN] ParticleManager: %p\n", interfaces::m_particle_manager);
	debug_text(WHITE_COLOR, "[PARTICLE SHIT DOWN] GameParticleManagerSystem: %p\n", interfaces::c_game_particle_manager_system);
	debug_text(WHITE_COLOR, "[PARTICLE SHIT DOWN] global_vars: %p\n", interfaces::global_vars);

	return original(a1, map_name);
}

void hooks::draw_skybox_array::hook(void* a1, void* a2, void* a3, int a4, int a5, void* a6, void* a7)
{
	static auto original = m_draw_skybox_array.get_original< decltype(&hook) >();

	c_scene_skybox_object* v8 = *reinterpret_cast<c_scene_skybox_object**>(reinterpret_cast<uintptr_t>(a3) + 0x18);

	if (v8 && v8->sky_material && a4 > 0)
	{
		if (config.world_modulation.ambience)
			v8->sky_color = config.world_modulation.sky_clr;
	}

	original(a1, a2, a3, a4, a5, a6, a7);
}

void hooks::draw_light_scene::hook(__int64 a1, __int64 a2)
{
	static auto original = m_draw_light_scene.get_original< decltype(&hook) >();

	original(a1, a2);
	if (config.world_modulation.ambience)
	{
		c_color& clr = config.world_modulation.lighting_clr;

		reinterpret_cast<float*>(a1 + 4)[0] = clr.r * 3;
		reinterpret_cast<float*>(a1 + 4)[1] = clr.g * 3;
		reinterpret_cast<float*>(a1 + 4)[2] = clr.b * 3;
		reinterpret_cast<float*>(a1 + 4)[3] = clr.a * 3;
	}
}

void* hooks::update_aggregate_scene_object::hook(c_aggregate_scene_object* a1, void* a2)
{
	static auto original = m_update_aggregate_scene_object.get_original< decltype(&hook) >();

	c_color_rgb target_color{};

	if (!(config.world_modulation.ambience))
		target_color = c_color_rgb(255, 255, 255);
	else
		target_color = config.world_modulation.world_clr.to_rgb();

	for (int i = 0; i < a1->count; i++)
	{
		a1->array[i].color.r = target_color.r;
		a1->array[i].color.g = target_color.g;
		a1->array[i].color.b = target_color.b;
	}

	return original(a1, a2);
}

enum e_model_type : int
{
	MODEL_SUN,
	MODEL_CLOUDS,
	MODEL_PROPS,
};

int get_model_type(const std::string_view& name)
{
	if (name.find("sun") != std::string::npos)
		return MODEL_SUN;

	if (name.find("clouds") != std::string::npos || name.find("effects") != std::string::npos)
		return MODEL_CLOUDS;

	return MODEL_PROPS;
}

void hooks::draw_aggregate_scene_object::hook(void* a1, void* a2, c_base_scene_data* a3, int a4, int a5, void* a6, void* a7, void* a8)
{
	static auto original = m_draw_aggregate_scene_object.get_original< decltype(&hook) >();

	int type = get_model_type(a3->material->get_name());
	c_color_rgb color{};

	switch (type)
	{
	case MODEL_SUN:
		color = config.world_modulation.sun_clr.to_rgb();
		break;
	case MODEL_CLOUDS:
		color = config.world_modulation.clouds_clr.to_rgb();
		break;
	case MODEL_PROPS:
		color = config.world_modulation.props_clr.to_rgb();
		break;
	}

	if (config.world_modulation.ambience)
	{

		for (int i = 0; i < a4; ++i)
		{
			auto scene = &a3[i];
			if (!scene)
				continue;

			scene->r = color.r;
			scene->g = color.g;
			scene->b = color.b;
			scene->a = 255;
		}
	}

	original(a1, a2, a3, a4, a5, a6, a7, a8);
}

void hooks::destroy()
{
	hooks::enable_cursor::unhook();
	hooks::create_move::m_create_move.unhook();
	hooks::enable_cursor::m_enable_cursor.unhook();
	hooks::frame_stage_notify::m_frame_stage_notify.unhook();
	hooks::mouse_input_enabled::m_mouse_input_enabled.unhook();
	hooks::on_add_entity::m_on_add_entity.unhook();
	hooks::on_remove_entity::m_on_remove_entity.unhook();
	hooks::on_level_init::m_on_level_init.unhook();
	hooks::on_level_shutdown::m_on_level_shutdown.unhook();
	hooks::override_view::m_override_view.unhook();
	hooks::present::m_present.unhook();
	hooks::validate_view_angles::m_validate_view_angles.unhook();
	hooks::draw_skybox_array::m_draw_skybox_array.unhook();
	hooks::draw_light_scene::m_draw_light_scene.unhook();
	hooks::update_aggregate_scene_object::m_update_aggregate_scene_object.unhook();
	hooks::draw_aggregate_scene_object::m_draw_aggregate_scene_object.unhook();
	hooks::draw_viewmodel::m_draw_viewmodel.unhook();
	hooks::get_field_of_view::m_get_field_of_view.unhook();
	hooks::remove_intro::m_remove_intro.unhook();
	hooks::remove_legs::m_remove_legs.unhook();
	hooks::removescope::m_removescope.unhook();
}