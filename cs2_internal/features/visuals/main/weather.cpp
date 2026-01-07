#include "weather.hpp"


static unsigned int g_effect_index = 0;
static weather_type g_type = weather_type::RAIN;
static bool g_initialized = false;
static bool g_effect_spawned = false;

const char* get_vpcf_name(weather_type type)
{
    switch (type)
    {
    case weather_type::RAIN: return "particles/rain_fx/rain.vpcf";
    case weather_type::SNOW: return "particles/rain_fx/snow.vpcf";
    case weather_type::ASH:  return "particles/rain_fx/ash.vpcf";
    default: return "particles/rain_fx/rain.vpcf";
    }
}

void weather::init(i_particle_system* system, i_particle_manager* manager)
{
    g_particle_system = system;
    g_particle_manager = manager;
    g_initialized = (g_particle_system && g_particle_manager);
    //g_initialized = true;

    debug_text(WHITE_COLOR, "[weather] particle_system: %p\n", system);
    debug_text(WHITE_COLOR, "[weather] particle_manager: %p\n", manager);

    if (g_initialized)
    {
        debug_text(GREEN_COLOR, "[weather] Initialized weather system\n");
    }
    else
        debug_text(RED_COLOR, "[weather] Failed to initialize weather system\n");
}

void weather::set_type(weather_type type)
{
    g_type = type;
    g_effect_index = 0;
    g_effect_spawned = false;
}

bool weather::is_active()
{
    return g_effect_spawned;
}

void weather::remove()
{
    g_effect_index = 0;
    g_effect_spawned = false;
}

void weather::update()
{
    if (!g_initialized || !interfaces::engine_client->is_in_game() || !g_ctx.local)
        return;
	debug_text(WHITE_COLOR, "[weather] Update called\n");
    if (g_effect_spawned)
        return;
	debug_text(WHITE_COLOR, "[weather] Spawning effect for weather type: %d\n", static_cast<int>(g_type));
    const char* vpcf_name = get_vpcf_name(g_type);

    particle_effect fx = {};
    fx.name = vpcf_name;

    g_particle_system->create_effect_index(&g_effect_index, &fx);
	debug_text(WHITE_COLOR, "[weather] Created effect index: %d for effect: %s\n", g_effect_index, vpcf_name);
    if (!g_effect_index)
    {
        debug_text(RED_COLOR, "[weather] Failed to create effect index for: %s\n", vpcf_name);
        return;
    }
	debug_text(WHITE_COLOR, "[weather] Effect index created: %d\n", g_effect_index);
    particle_color red = { 1.0f, 0.0f, 0.0f };
    g_particle_system->create_effect(g_effect_index, 0, &red, 0);

    debug_text(GREEN_COLOR, "[weather] Spawned effect: %s (index = %d)\n", vpcf_name, g_effect_index);

    g_effect_spawned = true;
}