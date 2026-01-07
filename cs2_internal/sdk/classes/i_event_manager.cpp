#include "i_event_manager.h"
#include "../../includes.h"

c_player_controller* c_game_event_helper::get_player_controller()
{
    if (!event)
        return nullptr;

    int controller_id{};

    c_buffer buffer;
    buffer.name = "userid";

    event->get_controller_id(controller_id, &buffer);

    if (controller_id == -1)
        return nullptr;

    return interfaces::entity_system->get_base_entity<c_player_controller>(controller_id + 1);
}

c_player_controller* c_game_event_helper::get_attacker_controller()
{
    if (!event)
        return nullptr;

    int controller_id{};

    c_buffer buffer;
    buffer.name = "attacker";

    event->get_controller_id(controller_id, &buffer);

    if (controller_id == -1)
        return nullptr;

    return interfaces::entity_system->get_base_entity<c_player_controller>(controller_id + 1);
}

int c_game_event_helper::get_damage()
{
    if (!event)
        return -1;

    return event->get_int2("dmg_health", false);
}

int c_game_event_helper::get_health()
{
    if (!event)
        return -1;

    c_buffer buffer;
    buffer.name = "health";

    return event->get_int(&buffer);
}

int c_game_event::get_int2(const char* name, bool unk)
{
    using fn_t = int(__thiscall*)(void*, const char*, bool);

    return reinterpret_cast<fn_t>((*reinterpret_cast<void***>(this))[4])(this, name, unk);
}