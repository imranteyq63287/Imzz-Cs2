#pragma once

#include "event.hpp"

#include <cassert>
#include <playsoundapi.h>
#include "../../config_system.h"
#include "../../menu/gui_base/notification.hpp"
#include "../../menu/gui_base/hashes.hpp"
#include "../../visuals/main/overlay.h"
std::vector<std::string> trash_talk_phrases = {
	"Play with Minorydev or keep sucking with your default Nixware",
	"Minory because winning fair is boring",
	"Your aim is like a typical NL user try Minory maybe youll hit something",
	"Using a public config We are closing scrims with Minory",
	"Minory isnt for everyone just for those tired of being cannon fodder",
	"You are using RU paste Im using Minory See the difference",
	"Do you have Minory Because I do",
	"Minory turns nobodies into legends But not you",
	"All you can do is report Minory wont get banned it dominates",
	"You move like a typical Nixware user without a clue",
	"Daddy is back from work time to humiliate",
	"Why are you on the team if you are minus morale and minus skill",
	"You are just a warm up for me zero challenge",
	"You would still lose even with wallhack",
	"How much are they paying you to throw games",
	"You sound like a cheater but got zero frags",
	"Dont stress Ill tell your mom you gave it your best",
	"You are the guy who dies first and types ping last",
	"Even my knife is more useful than you",
	"I have seen better gameplay from a bot In warmup With the monitor off",
	"Seriously you are wasting internet on this"
};
#include <vector>
#include "../../../sdk/utils/memory.h"
#include "../../../auth/auth.h"


std::vector<std::string> trash_talk_phrasess = {
	"1"
};
#define CHAT_COLOR_WHITE "\x01"
#define CHAT_COLOR_DARK_RED "\x02"
#define CHAT_COLOR_PURPLE "\x03"
#define CHAT_COLOR_DARK_GREEN "\x04"
#define CHAT_COLOR_LIGHT_GREEN "\x05"
#define CHAT_COLOR_GREEN "\x06"
#define CHAT_COLOR_LIGHT_RED "\x07"
#define CHAT_COLOR_GRAY "\x08"
#define CHAT_COLOR_YELLOW "\x09"
#define CHAT_COLOR_DARK_YELLOW "\x10"
#define CHAT_COLOR_LIGHT_BLUE "\x0A"
#define CHAT_COLOR_BLUE "\x0B"
#define CHAT_COLOR_DARK_BLUE "\x0C"
#define CHAT_COLOR_DARK_PURPLE "\x0D"
#define CHAT_COLOR_PINK "\x0E"
#define CHAT_COLOR_RED "\x0F"
const char* m_groups[] = {
   ("body"),
   ("head"),
   ("chest"),
   ("body"),
   ("left arm"),
   ("right arm"),
   ("left leg"),
   ("right leg"),
   ("neck"),
   ("unknown"),
   ("gear")
};

c_cs_player_controller* cs_game_event_helper::GetPlayerController()
{
	if (!Event)
		return nullptr;

	int controller_id{};

	cs_buffer buffer;
	buffer.name = ("userid");

	Event->GetControllerId(controller_id, &buffer);

	if (controller_id == -1)
		return nullptr;

	return (c_cs_player_controller*)interfaces::entity_system->get_base_entity(controller_id + 1);
}

c_cs_player_controller* cs_game_event_helper::GetAttackerController()
{
	if (!Event)
		return nullptr;

	int controller_id{};

	cs_buffer buffer;
	buffer.name = ("attacker");

	Event->GetControllerId(controller_id, &buffer);

	if (controller_id == -1)
		return nullptr;

	return (c_cs_player_controller*)interfaces::entity_system->get_base_entity(controller_id + 1);
}

int cs_game_event_helper::GetDamage()
{
	if (!Event)
		return -1;

	return Event->get_int2(("dmg_health"), false);
}

int cs_game_event::get_int2(const char* Name, bool Unk)
{
	using GetEventInt_t = int(__fastcall*)(void*, const char*, bool);
	static GetEventInt_t GetEventInt = reinterpret_cast<GetEventInt_t>(g_opcodes->scan("client.dll", ("48 89 5C 24 08 48 89 74 24 10 48 89 7C 24 18 41 56 48 83 EC 30 48 8B 01 41 8B F0 4C 8B F1 41 B0 01 48 8D 4C 24 20 48 8B DA 48 8B 78")));

	return GetEventInt(this, Name, Unk);
}

float cs_game_event::get_float2(const char* Name, bool Unk)
{
	using GetEventFloat_t = float(__fastcall*)(void*, const char*, bool);
	static GetEventFloat_t GetEventFloat = reinterpret_cast<GetEventFloat_t>(g_opcodes->scan("client.dll", ("48 89 5C 24 08 48 89 74 24 10 57 48 83 EC 40 48 8B 01 48 8B F1 0F 29 74 24 30 48 8D 4C 24 20 0F 28 F2 48 8B DA 48 8B 78")));

	return GetEventFloat(this, Name, Unk);
}


bool c_events::initialize()
{
	interfaces::m_game_event_manager->add(this, "player_hurt", false);
	if (!interfaces::m_game_event_manager->find(this, ("player_hurt")))
		return true;

	interfaces::m_game_event_manager->add(this, ("player_death"), false);
	if (!interfaces::m_game_event_manager->find(this, ("player_death")))
		return true;

	interfaces::m_game_event_manager->add(this, ("bullet_impact"), false);
	if (!interfaces::m_game_event_manager->find(this, ("bullet_impact")))
		return true;

	interfaces::m_game_event_manager->add(this, ("round_start"), false);
	if (!interfaces::m_game_event_manager->find(this, ("round_start")))
		return true;

	interfaces::m_game_event_manager->add(this, ("round_freeze_end"), false);
	if (!interfaces::m_game_event_manager->find(this, ("round_freeze_end")))
		return true;

	interfaces::m_game_event_manager->add(this, ("round_announce_warmup"), false);
	if (!interfaces::m_game_event_manager->find(this, ("round_announce_warmup")))
		return true;

	interfaces::m_game_event_manager->add(this, ("round_officially_ended"), false);
	if (!interfaces::m_game_event_manager->find(this, ("round_officially_ended")))
		return true;

	interfaces::m_game_event_manager->add(this, ("round_end"), false);
	if (!interfaces::m_game_event_manager->find(this, ("round_end")))
		return true;
	interfaces::m_game_event_manager->add(this, "smokegrenade_detonate", false);
	interfaces::m_game_event_manager->add(this, "smokegrenade_expired", false);
	interfaces::m_game_event_manager->add(this, "inferno_startburn", false);
	interfaces::m_game_event_manager->add(this, "inferno_expire", false);
	interfaces::m_game_event_manager->add(this, "inferno_extinguish", false);

	return false;
}
namespace grenade_timers {
	void handle_event(cs_game_event* event) {
		if (!config.world.granades)
			return;

		std::string name = event->get_name();

		if (name == "round_start") {
			grenade_effects.clear();
			return;
		}

		vec3_t position(event->get_int2("x", false), event->get_int2("y", false), event->get_int2("z", false));
		int entity_id = event->get_int2("entityid", false);

		if (name == "smokegrenade_detonate") {
			grenade_effects.push_back({ "smoke", entity_id, position, interfaces::global_vars->current_time });
		}
		else if (name == "inferno_startburn") {
			grenade_effects.push_back({ "molotov", entity_id, position, interfaces::global_vars->current_time });
		}
		else if (name == "smokegrenade_expired" || name == "inferno_expire" || name == "inferno_extinguish") {
			grenade_effects.erase(
				std::remove_if(grenade_effects.begin(), grenade_effects.end(), [entity_id](const auto& g) {
					return g.entity_id == entity_id;
					}), grenade_effects.end()
						);
		}
	}
}

void c_events::fire_game_event(cs_game_event* event)
{
	std::string name = event->get_name();

	if (name.find(("bullet_impact")) != std::string::npos)
		//on_bullet_tracer(event);

		if (name.find(("player_death")) != std::string::npos)
			player_death(event);

	if (name.find(("player_hurt")) != std::string::npos)
		on_player_hurt(event);
	grenade_timers::handle_event(event);

	if (name.find(("round_start")) != std::string::npos)
	{
		g_authorization->future = g_authorization->load_known_players();
	}

}

void c_events::player_death(cs_game_event* event)
{
	if (!interfaces::engine_client->is_connected() || !interfaces::engine_client->is_in_game())
		return;

	auto player = event->get_event_helper().GetPlayerController();
	auto attacker = event->get_event_helper().GetAttackerController();

	if (!player || !attacker)
		return;
}

std::atomic<int> animated_dmg = 0;
std::atomic<bool> anim_running = false;

void smooth_change_async(int from, int to, int delay_ms = 10)
{
	if (anim_running)
		return;

	anim_running = true;

	std::thread([from, to, delay_ms]() {
		int step = (to > from) ? 1 : -1;
		int current = from;

		while (current != to)
		{
			current += step;
			animated_dmg = current;
			std::this_thread::sleep_for(std::chrono::milliseconds(delay_ms));
		}
		animated_dmg = to;
		anim_running = false;
		}).detach();
}


void c_events::on_player_hurt(cs_game_event* event)
{
	if (!interfaces::engine_client->is_connected() || !interfaces::engine_client->is_in_game())
		return;
	if (!g_ctx.local) return; // fix by �����

	if (!g_ctx.local->is_alive())
		return;

	cs_game_event_helper EventHelper = event->get_event_helper();

	auto pPlayer = EventHelper.GetPlayerController();
	auto attacker = EventHelper.GetAttackerController();

	if (!pPlayer || !attacker)
		return;

	auto pawn = g_ctx.local;
	if (!pawn)
		return;

	auto localController = g_ctx.local_controller;
	if (!localController)
		return;

	auto damage = EventHelper.GetDamage();
	if (damage < 1)
		return;

	auto hitbox = event->get_int2(("hitgroup"), false);
	auto dmg = event->get_int2(("dmg_health"), false);
	auto health = event->get_int2(("health"), false);
	auto name = reinterpret_cast<c_cs_player_controller*>(pPlayer)->player_name();
	constexpr size_t group_count = sizeof(m_groups) / sizeof(m_groups[0]);

	const char* safe_name = name ? name : "unknown";
	const char* safe_hitgroup = (hitbox >= 0 && hitbox < group_count && m_groups[hitbox]) ? m_groups[hitbox] : "unknown";

	std::string message = tfm::format(
		CHAT_COLOR_WHITE "[ " CHAT_COLOR_PURPLE "%s" CHAT_COLOR_WHITE " ] Hit"
		CHAT_COLOR_BLUE " %s "
		CHAT_COLOR_WHITE "in the"
		CHAT_COLOR_PINK " %s "
		CHAT_COLOR_WHITE "for"
		CHAT_COLOR_LIGHT_GREEN " %d "
		CHAT_COLOR_WHITE "damage"
		CHAT_COLOR_RED " ( %d remaining )",
		g_ctx.cheat_name.c_str(), safe_name, safe_hitgroup, dmg, health
	);

	if (attacker == localController && pPlayer != localController)
	{


		if (config.gui.hitlog)
		{
			if (config.gui.misslog)
			{
				g_notification->AddAnimatedNotification(reinterpret_cast<const char*>(ICON_FA_CROSSHAIRS), name, m_groups[hitbox], dmg, health);
			}
			else
			{
				if (config.gui.hitlog_position == 0)
				{
					g_notification->AddNotification(reinterpret_cast<const char*>(ICON_FA_CROSSHAIRS), tfm::format(("Hit %s in the %s for %d damage (%d health remaining)"), name, m_groups[hitbox], dmg, health).c_str());
				}
				else if (config.gui.hitlog_position == 1)
				{
					g_notification->AddNotificationTopLeft(reinterpret_cast<const char*>(ICON_FA_CROSSHAIRS), tfm::format(("Hit %s in the %s for %d damage (%d health remaining)"), name, m_groups[hitbox], dmg, health).c_str());
				}
			}
			if (config.gui.chatlogs)
			{
				g_memory->sent_message_to_local_chat(message.c_str());
			}
		}
		if (config.extra.trashtalk)
		{
			if (health == 0)
			{
				static std::mt19937 rng(std::random_device{}());
				std::uniform_int_distribution<size_t> dist(0, trash_talk_phrases.size() - 1);
				std::string phrase = "say " + trash_talk_phrases[dist(rng)];
				interfaces::engine_client->client_cmd_unrestricted(phrase.c_str());
				//g_notification->AddNotification(reinterpret_cast<const char*>(ICON_FA_COMMENT), phrase.c_str());
			}
		}
		c_cs_player_pawn* player_pawn = reinterpret_cast<c_cs_player_pawn*>(
			interfaces::entity_system->get_base_entity(pPlayer->player_pawn_handle().get_entry_index())
			);
		auto scene_node = player_pawn->game_scene_node();
		if (!scene_node)
			return;
		if (health == 0)
		{

			vec3_t victim_pos = scene_node->vec_origin();
			std::vector<vec3_t> bolt_points;
			float start_z = victim_pos.z + 500.0f;

			for (int i = 0; i <= bolt_segments; ++i) {
				float t = static_cast<float>(i) / bolt_segments;
				float x = victim_pos.x + (rand() % 100 - 50) * (1 - t);
				float y = victim_pos.y + (rand() % 100 - 50) * (1 - t);
				float z = victim_pos.z + (start_z - victim_pos.z) * (1 - t);
				bolt_points.push_back(vec3_t(x, y, z));
			}

			std::vector<Spark> sparks;
			for (int i = 0; i < max_sparks; ++i) {
				float angle = static_cast<float>(rand()) / RAND_MAX * 2.0f * 3.14159265358979323846f;
				float speed = spark_min_speed + static_cast<float>(rand()) / RAND_MAX * (spark_max_speed - spark_min_speed);
				vec2_t velocity(std::cos(angle) * speed, std::sin(angle) * speed);
				float size = spark_min_size + static_cast<float>(rand()) / RAND_MAX * (spark_max_size - spark_min_size);
				sparks.push_back({ victim_pos, velocity, size, 1.f });
			}

			float current_time = interfaces::global_vars->current_time;

			active_effect.push_back({ victim_pos, current_time, bolt_points, sparks });
		}
		vec3_t pos = scene_node->vec_abs_origin();
		pos.z += 50.f;

		active_effects.push_back({
			.position = pos,
			.start_time = interfaces::global_vars->current_time
			});
		static const std::vector<std::string> hitsound_paths =
		{
		"", // 0 = None
		"\\sounds\\ui\\coin_pickup_01.vsnd_c",                       // 1 - Money
		"\\sounds\\ambient\\atmosphere\\balloon_pop_01.vsnd_c",     // 2 - Balloon
		"\\sounds\\cod.vsnd_c",                                     // 3 - Cod
		"\\sounds\\click.vsnd_c",                                   // 5 - Click
		"\\sounds\\bubble.vsnd_c",                                  // 6 - Bubble
		"\\sounds\\bell.vsnd_c",                                    // 7 - Bell
		"\\sounds\\arena_switch.vsnd_c",                            // 8 - Switch
		"\\sounds\\applepay.vsnd_c",                                // 9 - Apple Pay
		"\\sounds\\agpa2.vsnd_c",                                   // 13 - Agpa
		"\\sounds\\bonk.vsnd_c",                                    // 16 - Bonk
		"\\sounds\\fatality.vsnd_c",                                // 20 - Fatality
		"\\sounds\\hentai1.vsnd_c",                                 // 22 - Hentai1
		"\\sounds\\hentai2.vsnd_c",                                 // 23 - Hentai2
		"\\sounds\\hentai3.vsnd_c",                                 // 24 - Hentai3
		"\\sounds\\minecraft_hit.vsnd_c",                           // 31 - MC Hit
		"\\sounds\\minecraft_xp_gain.vsnd_c",                       // 32 - MC XP Gain
		"\\sounds\\rust_headshot.vsnd_c",                           // 40 - Rust Headshot
		"\\sounds\\zelda.vsnd_c",                                   // 48 - Zelda

		};
		if (config.gui.hsound != 0 && health != 0)
		{

			if (config.gui.hsound != 0 && config.gui.hsound < hitsound_paths.size()) {
				float volume_float = static_cast<float>(config.gui.hsoundvol) / 100.0f;
				std::string volume_cmd = "snd_toolvolume " + std::to_string(volume_float);
				interfaces::engine_client->client_cmd_unrestricted(volume_cmd.c_str());

				std::string play_cmd = "play " + hitsound_paths[config.gui.hsound];
				interfaces::engine_client->client_cmd_unrestricted(play_cmd.c_str());
			}

		}
		if (config.gui.ksound != 0 && health == 0)
		{
			if (config.gui.ksound != 0 && config.gui.ksound < hitsound_paths.size()) {
				float volume_float = static_cast<float>(config.gui.ksoundvol) / 100.0f;
				std::string volume_cmd = "snd_toolvolume " + std::to_string(volume_float);
				interfaces::engine_client->client_cmd_unrestricted(volume_cmd.c_str());

				std::string play_cmd = "play " + hitsound_paths[config.gui.ksound];
				interfaces::engine_client->client_cmd_unrestricted(play_cmd.c_str());
			}

		}
	}
}

//void c_events::on_bullet_tracer(cs_game_event* event)
//{
//	if (!interfaces::engine_client->is_connected() || !interfaces::engine_client->is_in_game())
//		return;
//
//	if (!g_ctx.local->is_alive())
//		return;
//
//	cs_utl_string_token token_x(("x"));
//	cs_utl_string_token token_y(("y"));
//	cs_utl_string_token token_z(("z"));
//
//	auto player = event->get_event_helper().GetPlayerController();
//	auto position = vec3_t(event->get_float_new2(token_x), event->get_float_new2(token_y), event->get_float_new2(token_z));
//
//	c_cs_player_pawn* local_pawn = g_ctx.local;
//
//	if (!local_pawn)
//		return;
//
//	cs_game_event_helper event_helper = event->get_event_helper();
//
//	auto pPlayer = event_helper.GetPlayerController();
//
//	if (event->get_event_helper().GetPlayerController() == g_ctx.local_controller)
//	{
//		//g_bullet_tracer->add_bullet_trace(local_pawn->get_eye_pos(), position, (config.world_modulation.lighting_clr * 255));
//
//		//if (!g_configs->world.m_hit_effect)
//			//return;
//
//		//g_bullet_tracer->add_hit_effect(position, (255, 255, 255));
//	}
//}