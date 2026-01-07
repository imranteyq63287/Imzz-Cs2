#pragma once

#include "../../includes.h"
#include "../classes/c_cs_player_controller.h"
#include "../classes/c_cs_player_pawn.h"

#define ENTITY_ALL 1
#define ENTITY_ENEMIES_ONLY 2
#define ENTITY_TEAMMATES_ONLY 3

using player_controller_pair = std::pair<c_cs_player_controller*, c_cs_player_pawn*>;

namespace entity 
{
    void initialize();

    void level_init();
    void level_shutdown();
    void add_entity(c_entity_instance*, int);
    void remove_entity(c_entity_instance*, int);

    c_cs_player_pawn* get_local_player();
    c_cs_player_controller* get_local_player_controller();

    std::vector<c_entity_instance*> get(const char*);

    int get_entity_handle(c_cs_player_pawn* pawn);

    std::vector<c_cs_player_pawn*> get_players(int);
    std::vector<player_controller_pair> get_players_with_controller(int);
}