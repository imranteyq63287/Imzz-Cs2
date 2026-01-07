#include "c_csgo_input.h"
#include "../../utils/utils.h"
#include "../../modules/modules.h"

vec3_t c_csgo_input::get_view_angles()
{
    using get_view_angles_t = void* (__fastcall*)(c_csgo_input*, int);
    static get_view_angles_t get_view_angles = reinterpret_cast<get_view_angles_t>(utils::find_pattern(g_modules.client, "4C 8B C1 85 D2 74 ? 48 8D 05"));

    return *reinterpret_cast<vec3_t*>(get_view_angles(this, 0));
}

void c_csgo_input::set_view_angles(vec3_t& view_angles)
{
    using set_view_angles_t = void(__fastcall*)(c_csgo_input*, int, vec3_t&);
    static set_view_angles_t set_view_angles = reinterpret_cast<set_view_angles_t>(utils::find_pattern(g_modules.client, "85 D2 75 ? 48 63 81"));

    set_view_angles(this, 0, view_angles);
}

c_user_cmd* c_csgo_input::get_user_cmd(void* local_controller)
{
    if (!local_controller)
        return nullptr;

    int player_idx = 0;

    using get_entity_index_t = void* (__fastcall*)(void*, int*);
    static get_entity_index_t get_entity_index = reinterpret_cast<get_entity_index_t>(utils::find_pattern_rel(g_modules.client, "E8 ? ? ? ? 8B 8D ? ? ? ? 8D 51", 0x1));

    get_entity_index(local_controller, &player_idx);

    int entity_index = player_idx - 1;

    if (player_idx == -1)
        entity_index = -1;

    static auto construct_input_data = reinterpret_cast<void* (__fastcall*)(void*, int)>(utils::find_pattern_rel(g_modules.client, "E8 ? ? ? ? 48 8B CF 4C 8B F8", 0x1));
    static auto sig_poo = *reinterpret_cast<void**>(utils::find_pattern_rel(g_modules.client, "48 8B 0D ? ? ? ? E8 ? ? ? ? 48 8B CF 4C 8B F8", 0x3));

    void* array_inputs = construct_input_data(sig_poo, entity_index);

    int sequence_number = *reinterpret_cast<int*>(reinterpret_cast<uintptr_t>(array_inputs) + 0x59A8);

    static auto automake_user_cmd = reinterpret_cast<void*>(utils::find_pattern_rel(g_modules.client, "E8 ? ? ? ? 48 8B 0D ? ? ? ? 45 33 E4 48 89 45", 0x1));
    static auto poo = reinterpret_cast<c_user_cmd * (__fastcall*)(void*, int)>(automake_user_cmd);

    return poo(local_controller, sequence_number);
}