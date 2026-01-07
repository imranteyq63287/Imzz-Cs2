#ifndef EVENTS_H
#define EVENTS_H
static bool bullet_impacts = false;
static c_color bullet_impact_color = c_color(255, 0, 0, 255);
#include "../../../includes.h"

class c_event : public i_game_event_listener
{
public:
	void fire_game_event(c_game_event* event);
	void initilization();
	void bullet_impact(c_game_event* event);
};

inline const std::unique_ptr< c_event > g_events{ new c_event() };

#endif 
