#include "events.h"
#include "../../ESP/other/tracers.h"
#include "../../engine prediction/eprediction.hpp"

void c_event::initilization() {
	interfaces::m_event_manager->add_listeners(this, ("player_hurt"), false);
	interfaces::m_event_manager->add_listeners(this, ("player_death"), false);
	interfaces::m_event_manager->add_listeners(this, ("add_bullet_hit_marker"), false);
	interfaces::m_event_manager->add_listeners(this, ("bullet_impact"), false);
	interfaces::m_event_manager->add_listeners(this, ("round_start"), false);
	interfaces::m_event_manager->add_listeners(this, ("weapon_fire"), false);
}

void c_event::bullet_impact(c_game_event* event)
{
	auto player = event->get_event_helper().get_player_controller();
	auto position = vec3_t(event->get_int2(("x"), 0), event->get_int2(("y"), 0), event->get_int2(("z"), 0));

	if (!player)
		return;

	if (bullet_impacts)
		g_bullet_tracers->process_tracers(g_prediction->get_local_data()->m_eye_pos, position, bullet_impact_color);
}

void c_event::fire_game_event(c_game_event* event) {
	if (!interfaces::engine_client->is_in_game() || !event)
		return;

	std::string name = event->get_name();

	if (name.find("bullet_impact") != std::string::npos)
		bullet_impact(event);
}