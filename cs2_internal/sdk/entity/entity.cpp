#include "entity.h"
#include "../../context.h"

std::unordered_map<uint64_t, std::vector<c_entity_instance*>> entities;

void entity::initialize()
{
    int highest_idx = interfaces::entity_system->get_highest_entiy_index();

    for (int i = 0; i <= highest_idx; i++)
    {
        c_entity_instance* entity = interfaces::entity_system->get_base_entity<c_entity_instance>(i);

        if (!entity)
            continue;

        const char* class_name = entity->get_entity_class_name();

        if (!class_name)
            continue;

        entities[fnv1a::hash_64(class_name)].emplace_back(entity);
    }
}

void entity::level_init()
{
    entities.clear();
    initialize();
}

void entity::level_shutdown()
{
    entities.clear();
}

void entity::add_entity(c_entity_instance* entity, int index)
{
    const char* className = entity->get_entity_class_name();

    entities[fnv1a::hash_64(className)].emplace_back(entity);
}

void entity::remove_entity(c_entity_instance* entity, int index)
{
    const char* class_name = entity->get_entity_class_name();

    auto it = entities.find(fnv1a::hash_64(class_name));
    if (it == entities.end())
        return;

    std::vector<c_entity_instance*>& entities = it->second;

    for (auto iter = entities.begin(); iter != entities.end(); ++iter) {
        if ((*iter) == entity) {
            entities.erase(iter);

            break;
        }
    }
}

c_cs_player_pawn* entity::get_local_player()
{
    static auto fn = reinterpret_cast<c_cs_player_pawn * (__fastcall*)(int)>(utils::find_pattern_rel(g_modules.client, "E8 ? ? ? ? 48 8B F0 48 85 C0 74 ? 48 8D 15 ? ? ? ? B9", 0x1));
  
    return fn(0);
}

c_cs_player_controller* entity::get_local_player_controller()
{
    static auto local_controller = reinterpret_cast<c_cs_player_controller**>(utils::find_pattern_rel(g_modules.client, "E8 ? ? ? ? 48 85 C0 74 ? 8B CB E8", 0x1));
 
    return local_controller[0];
}

std::vector<c_entity_instance*> entity::get(const char* name)
{
    return entities[fnv1a::hash_64(name)];
}

int entity::get_entity_handle(c_cs_player_pawn* pawn)
{
    if (!pawn)
        return -1;

    using fn = unsigned int(__fastcall*)(void*);
    static auto function = reinterpret_cast<fn>(utils::find_pattern(g_modules.client, XOR_STR("48 85 C9 74 32 48 8B 41 10 48 85 C0 74 29 44")));

    return function(pawn);
}

std::vector<c_cs_player_pawn*> entity::get_players(int index)
{
    std::vector<c_cs_player_pawn*> enteties;

    c_cs_player_pawn* local_player = g_ctx.local;
    if (!local_player)
        return enteties;

    int local_team_num = local_player->team_num();

    std::vector<c_entity_instance*> entities = get("C_CSPlayerPawn");
   
    for (c_entity_instance* entity : entities)
    {
        c_cs_player_pawn* player_pawn = reinterpret_cast<c_cs_player_pawn*>(entity);

        int team_num = player_pawn->team_num();

        if (index == ENTITY_ALL || ((index == ENTITY_ENEMIES_ONLY && local_team_num != team_num) || (index == ENTITY_TEAMMATES_ONLY && local_team_num == team_num && local_player != player_pawn)))
            enteties.emplace_back(player_pawn);
    }

    return enteties;
}

std::vector<player_controller_pair> entity::get_players_with_controller(int index)
{
    std::vector<player_controller_pair> return_enteties;

    c_cs_player_pawn* local_player = g_ctx.local;
    if (!local_player)
        return return_enteties;

    int local_team_num = local_player->team_num();

    std::vector<c_entity_instance*> entities = get("CCSPlayerController");
    for (auto entity : entities)
    {
        c_cs_player_controller* controller = reinterpret_cast<c_cs_player_controller*>(entity);
        if (!controller)
            continue;

        c_cs_player_pawn* player_pawn = reinterpret_cast<c_cs_player_pawn*>(interfaces::entity_system->get_base_entity(controller->pawn().get_entry_index()));
        if (!player_pawn)
            continue;

        int team_num = player_pawn->team_num();

        if (index == ENTITY_ALL || ((index == ENTITY_ENEMIES_ONLY && local_team_num != team_num) || (index == ENTITY_TEAMMATES_ONLY && local_team_num == team_num && local_player != player_pawn)))
            return_enteties.emplace_back(std::make_pair(controller, player_pawn));
    }

    return return_enteties;
}