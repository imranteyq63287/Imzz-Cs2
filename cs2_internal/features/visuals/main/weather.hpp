#pragma once
#include "../../../includes.h"
#include "../../../sdk/classes/i_particle_system.h"
enum class weather_type
{
    RAIN,
    SNOW,
    ASH
};
    static i_particle_system* g_particle_system = nullptr;
    static i_particle_manager* g_particle_manager = nullptr;
namespace weather
{
    void init(class i_particle_system* system, class i_particle_manager* manager);
    void update();
    void remove();
    void set_type(weather_type type);
    bool is_active();
}
