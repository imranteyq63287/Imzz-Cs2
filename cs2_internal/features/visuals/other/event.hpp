#pragma once

#include <array>
#include "../../../includes.h"
#include "../../../sdk/utils/render/render.h"
#include "../../../sdk/classes/c_event.h"
#include "../../../features/config_system.h"

struct HeartEffect {
	vec3_t position;

	float start_time;
};

struct Colors {
	std::uint8_t r = 0U, g = 0U, b = 0U, a = 0U;
};

inline std::vector<HeartEffect> active_effects;
inline constexpr float heart_duration = 3.f;
inline Colors custom_color = Colors(255, 0, 0, 255);

struct Spark {
	vec3_t pos;
	vec2_t velocity;

	float size;
	float alpha;
};

struct Effect {
	vec3_t pos;

	float start_time;

	std::vector<vec3_t> bolt_points;
	std::vector<Spark> sparks;
};

struct grenade_effect_t {
	std::string type;

	int entity_id;

	vec3_t position;

	float spawn_time;
};

inline std::vector<grenade_effect_t> grenade_effects;

inline float SMOKE_DURATION = 22.06f;
inline float MOLOTOV_DURATION = 7.03f;
inline std::vector<Effect> active_effect;

inline void imgui_rect(float x, float y, float w, float h, ImColor color) {
	ImDrawList* draw = ImGui::GetBackgroundDrawList();
	draw->AddRectFilled(ImVec2(x, y), ImVec2(x + w, y + h), color);
}

inline void draw_heart_imgui(float x, float y, ImColor color) {
	ImColor outline = ImColor(0, 0, 0, static_cast<int>(color.Value.w * 255.0f));

	imgui_rect(x + 2, y + 14, 2, 2, outline);
	imgui_rect(x, y + 12, 2, 2, outline);
	imgui_rect(x - 2, y + 10, 2, 2, outline);
	imgui_rect(x - 4, y + 4, 2, 6, outline);
	imgui_rect(x - 2, y + 2, 2, 2, outline);
	imgui_rect(x, y, 2, 2, outline);
	imgui_rect(x + 2, y, 2, 2, outline);
	imgui_rect(x + 4, y + 2, 2, 2, outline);
	imgui_rect(x + 6, y, 2, 2, outline);
	imgui_rect(x + 8, y, 2, 2, outline);
	imgui_rect(x + 10, y + 2, 2, 2, outline);
	imgui_rect(x + 12, y + 4, 2, 6, outline);
	imgui_rect(x + 10, y + 10, 2, 2, outline);
	imgui_rect(x + 8, y + 12, 2, 2, outline);
	imgui_rect(x + 6, y + 14, 2, 2, outline);
	imgui_rect(x + 4, y + 16, 2, 2, outline);

	imgui_rect(x - 2, y + 4, 2, 6, color);
	imgui_rect(x, y + 2, 4, 2, color);
	imgui_rect(x, y + 6, 4, 6, color);
	imgui_rect(x + 2, y + 4, 2, 2, color);
	imgui_rect(x + 2, y + 12, 2, 2, color);
	imgui_rect(x + 4, y + 4, 2, 12, color);
	imgui_rect(x + 6, y + 2, 4, 10, color);
	imgui_rect(x + 6, y + 12, 2, 2, color);
	imgui_rect(x + 10, y + 4, 2, 6, color);
	imgui_rect(x, y + 4, 2, 2, ImColor(254, 199, 199, static_cast<int>(color.Value.w * 255.0f)));
}

constexpr int max_sparks = 20;
constexpr float bolt_duration = 0.5f;
constexpr float spark_duration = 0.7f;
constexpr int bolt_segments = 10;
constexpr float spark_min_size = 2.f, spark_max_size = 5.f;
constexpr float spark_min_speed = 50.f, spark_max_speed = 150.f;

inline void draw_effects() {
	float current_time = interfaces::global_vars->current_time;
	float frame_time = interfaces::global_vars->frame_time;

	std::vector<Effect> new_effects;

	for (auto& effect : active_effect) {
		float elapsed = current_time - effect.start_time;

		if (elapsed > bolt_duration && elapsed > spark_duration)
			continue;

		// Bolt drawing
		if (elapsed < bolt_duration) {
			for (size_t i = 0; i < effect.bolt_points.size() - 1; ++i) {
				vec3_t start, end;
				if (g_render->world_to_screen(effect.bolt_points[i], start) &&
					g_render->world_to_screen(effect.bolt_points[i + 1], end)) {
					g_render->line(start, end, config.world_modulation.clouds_clr);
				}
			}
		}

		// Spark drawing
		if (elapsed < spark_duration) {
			float fade = 1.0f - (elapsed / spark_duration);

			for (auto& spark : effect.sparks) {
				spark.pos.x += spark.velocity.x * frame_time;
				spark.pos.y += spark.velocity.y * frame_time;
				spark.pos.z += (rand() % 41 - 20) * frame_time;
				spark.alpha = std::max(0.0f, fade);

				vec3_t screen;
				if (g_render->world_to_screen(spark.pos, screen)) {
					ImDrawList* draw = ImGui::GetBackgroundDrawList();
					if (draw) {
						ImColor color(204, 230, 255, static_cast<int>(spark.alpha * 255));
						draw->AddCircleFilled(ImVec2(screen.x, screen.y), spark.size, color, 16);
					}
				}
			}
		}

		new_effects.push_back(effect);
	}

	active_effect = std::move(new_effects);
}

class c_events : public cs_i_game_event_listener
{
public:
	void on_bullet_tracer(cs_game_event* event);
	void fire_game_event(cs_game_event* event);
	void on_player_hurt(cs_game_event* event);
	void player_death(cs_game_event* event);

	bool initialize();
};

class c_glow_property {
public:
	c_cs_player_pawn* GetParentEntity() {
		return *(c_cs_player_pawn**)(this + 0x18);
	}
};

inline c_events* g_events = new c_events();