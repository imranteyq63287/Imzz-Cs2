#include "overlay.h"
#include "../../Aimbot/Rage/ragebot.h"
#include "../../../auth/auth.h"
namespace ESP {
    std::vector<c_dropped_weapons> m_dropped_weapons;
}
void ESP::radarhack()
{
    if (radarhack_debug)
        std::cout << "[RadarHack] Starting radarhack()\n";

    if (!interfaces::engine_client->is_connected() || !interfaces::engine_client->is_in_game())
    {
        if (radarhack_debug)
            std::cout << "[RadarHack] Not connected or not in game. Exiting.\n";
        return;
    }

    if (!g_ctx.local_controller)
    {
        if (radarhack_debug)
            std::cout << "[RadarHack] Local controller is null. Exiting.\n";
        return;
    }

    auto player_entities = entity::get("CCSPlayerController");
    if (radarhack_debug)
        std::cout << "[RadarHack] Found " << player_entities.size() << " player controllers.\n";

    const bool want_spotted = config.esp.radarhack;

    for (auto player_entity : player_entities)
    {
        auto player_controller = reinterpret_cast<c_cs_player_controller*>(player_entity);
        if (!player_controller || player_controller == g_ctx.local_controller)
            continue;

        const auto pawn_handle = player_controller->pawn();
        auto player_pawn = reinterpret_cast<c_cs_player_pawn*>(
            interfaces::entity_system->get_base_entity(pawn_handle.get_entry_index()));

        if (!player_pawn || player_pawn == g_ctx.local || !player_pawn->is_alive())
            continue;

        if (!player_pawn->is_enemy())
            continue;

        int& spotted = player_pawn->entity_spotted_state();

        if (want_spotted && !spotted)
        {
            spotted = true;
            if (radarhack_debug)
                std::cout << "[RadarHack] Set spotted to 1 for enemy pawn.\n";
        }
        else if (!want_spotted && spotted)
        {
            spotted = false;
            if (radarhack_debug)
                std::cout << "[RadarHack] Set spotted to 0 for enemy pawn.\n";
        }
    }

    if (radarhack_debug)
        std::cout << "[RadarHack] Finished radarhack()\n";
}
void ESP::draw_names(bbox_t box, vec3_t left_top, vec3_t bottom_right, c_cs_player_pawn* player_pawn, c_cs_player_controller* player_controller)
{
    if (config.esp.onlyvisible)
    {
        if (!is_player_visible_esp(g_ctx.local, player_pawn))
        {
            return;
        }
    }
    if (config.esp.show_name)
    {
        std::string player_names = player_controller->player_name();
        if (player_names.empty())
        {
            if (esp_debug)
                std::cout << "ESP: player name is empty, skipping name draw." << std::endl;
            return;
        }
        else if (esp_debug)
        {
            std::cout << "ESP: player name: " << player_names << std::endl;
        }

        vec3_t name_pos = { box.x + box.width * 0.5f - ImGui::CalcTextSize(player_names.c_str()).x * 0.5f, box.y - 15.f };
        if (name_pos.is_zero())
        {
            if (esp_debug)
                std::cout << "ESP: name_pos is zero, skipping name draw." << std::endl;
            return;
        }

        float name_text_outline_offset = 1.0f;

        if (config.esp.name_outline)
        {
            if (esp_debug)
                std::cout << "ESP: drawing name outline." << std::endl;

            for (int x_off = -1; x_off <= 1; ++x_off)
            {
                for (int y_off = -1; y_off <= 1; ++y_off)
                {
                    if (x_off == 0 && y_off == 0)
                        continue;

                    g_render->text(name_pos + vec3_t(x_off * name_text_outline_offset, y_off * name_text_outline_offset), player_names.c_str(), config.esp.name_color_outline, g_render->fonts.verdana, g_render->fonts.verdana->FontSize);
                }
            }
        }
        g_render->text(name_pos, player_names.c_str(), config.esp.name_color, g_render->fonts.verdana, g_render->fonts.verdana->FontSize);
    }
}
void ESP::draw_healthbar(bbox_t box, vec3_t left_top, vec3_t bottom_right, c_cs_player_pawn* player_pawn, c_cs_player_controller* player_controller)
{
    if (config.esp.onlyvisible)
    {
        if (!is_player_visible_esp(g_ctx.local, player_pawn))
        {
            return;
        }
    }
    if (config.esp.show_health)
    {
        if (esp_debug)
            std::cout << "ESP: show_health enabled." << std::endl;

        int health = player_pawn->health();
        if (esp_debug)
            std::cout << "ESP: player health = " << health << std::endl;

        if (health <= 0)
        {
            if (esp_debug)
                std::cout << "ESP: health is 0 or less, skipping." << std::endl;
            return;
        }

        float current_health = std::clamp(health, 0, 100);

        uintptr_t entity_id = reinterpret_cast<uintptr_t>(player_pawn);
        if (!entity_id)
        {
            if (esp_debug)
                std::cout << "ESP: entity_id is nullptr, skipping." << std::endl;
            return;
        }

        if (animated_health_map.find(entity_id) == animated_health_map.end())
        {
            if (esp_debug)
                std::cout << "ESP: adding new entry to animated_health_map with value " << current_health << std::endl;
            animated_health_map[entity_id] = current_health;
        }

        float& animated_health = animated_health_map[entity_id];
        animated_health = std::lerp(animated_health, current_health, 0.1f);
        if (esp_debug)
            std::cout << "ESP: animated_health = " << animated_health << std::endl;

        vec3_t box_min = { box.x, box.y };
        vec3_t box_max = { box.x + box.width, box.y + box.height };

        if (box_min.is_zero() || box_max.is_zero())
        {
            if (esp_debug)
                std::cout << "ESP: box_min or box_max is zero, skipping health bar." << std::endl;
            return;
        }

        float offset = 2.5f;
        float bar_height = box_max.y - box_min.y;
        float filled_height = bar_height * (animated_health / 100.0f);
        float bar_x = box_min.x - config.esp.hp_bar_width - offset;

        if (esp_debug)
        {
            std::cout << "ESP: offset = " << offset << std::endl;
            std::cout << "ESP: bar_height = " << bar_height << std::endl;
            std::cout << "ESP: filled_height = " << filled_height << std::endl;
            std::cout << "ESP: bar_x = " << bar_x << std::endl;
        }

        ImVec2 bar_min = { bar_x, box_min.y };
        ImVec2 bar_max = { bar_x + config.esp.hp_bar_width, box_max.y };

        if (esp_debug)
        {
            std::cout << "ESP: bar_min = (" << bar_min.x << ", " << bar_min.y << ")" << std::endl;
            std::cout << "ESP: bar_max = (" << bar_max.x << ", " << bar_max.y << ")" << std::endl;
        }

        if (config.esp.health_outline)
        {
            if (esp_debug)
                std::cout << "ESP: drawing health outline." << std::endl;
            g_render->rect_filled({ bar_min.x - 1, bar_min.y - 1 }, { bar_max.x + 1, bar_max.y + 1 }, config.esp.health_color_outline);
        }

        g_render->rect_filled({ bar_min.x, bar_max.y - filled_height }, { bar_max.x, bar_max.y }, config.esp.health_color);
        if (esp_debug)
            std::cout << "ESP: drawing health bar filled area." << std::endl;

        if (current_health < 100 && config.esp.hp_bar_text)
        {
            vec3_t text_pos = { bar_min.x - 15.f, bar_max.y - filled_height };
            std::string health_str = std::to_string((int)current_health);

            if (esp_debug)
                std::cout << "ESP: drawing health value: " << health_str << " at (" << text_pos.x << ", " << text_pos.y << ")" << std::endl;

            if (config.esp.hp_bar_text_outline)
            {
                if (esp_debug)
                    std::cout << "ESP: drawing health text outline." << std::endl;

                float hp_bar_text_outline_offset = 1.0f;
                for (int x_off = -1; x_off <= 1; ++x_off)
                {
                    for (int y_off = -1; y_off <= 1; ++y_off)
                    {
                        if (x_off == 0 && y_off == 0)
                            continue;

                        g_render->text(text_pos + vec3_t(x_off * hp_bar_text_outline_offset, y_off * hp_bar_text_outline_offset), health_str, config.esp.health_text_color_outline, g_render->fonts.verdana, g_render->fonts.verdana->FontSize);
                    }
                }
            }

            g_render->text({ bar_min.x - 15, bar_max.y - filled_height }, health_str, config.esp.hp_bar_text_color, g_render->fonts.verdana, g_render->fonts.verdana->FontSize);
        }
    }
}
void ESP::draw_ammobar(bbox_t box, vec3_t left_top, vec3_t bottom_right, c_cs_player_pawn* player_pawn, c_cs_player_controller* player_controller)
{
    if (config.esp.onlyvisible)
    {
        if (!is_player_visible_esp(g_ctx.local, player_pawn))
        {
            return;
        }
    }
    if (config.esp.show_ammo)
    {
        if (esp_debug)
            std::cout << "ESP: show_ammo enabled." << std::endl;

        auto weapon = player_pawn->get_active_weapon();
        if (!weapon)
        {
            if (esp_debug)
                std::cout << "ESP: weapon is nullptr, skipping." << std::endl;
            return;
        }

        int ammo = weapon->i_clip1();
        if (esp_debug)
            std::cout << "ESP: weapon->i_clip1() = " << ammo << std::endl;

        if (ammo <= 0)
        {
            if (esp_debug)
                std::cout << "ESP: no ammo in clip, skipping." << std::endl;
            return;
        }

        auto wdata = weapon->get_weapon_data();
        if (!wdata)
        {
            if (esp_debug)
                std::cout << "ESP: weapon data is nullptr, skipping." << std::endl;
            return;
        }

        int max_ammo = wdata->max_clip1();
        if (esp_debug)
            std::cout << "ESP: max_clip1 = " << max_ammo << std::endl;

        if (max_ammo <= 0)
        {
            if (esp_debug)
                std::cout << "ESP: max_ammo is zero or negative, skipping." << std::endl;
            return;
        }

        float current_ratio = std::clamp((float)ammo / max_ammo, 0.f, 1.f);
        if (current_ratio <= 0.f)
        {
            if (esp_debug)
                std::cout << "ESP: current_ratio is zero, skipping." << std::endl;
            return;
        }

        uintptr_t weapon_id = reinterpret_cast<uintptr_t>(weapon);
        if (!weapon_id)
        {
            if (esp_debug)
                std::cout << "ESP: weapon_id is null, skipping." << std::endl;
            return;
        }

        if (animated_ammo_map.find(weapon_id) == animated_ammo_map.end())
        {
            if (esp_debug)
                std::cout << "ESP: initializing ammo animation: " << current_ratio << std::endl;
            animated_ammo_map[weapon_id] = current_ratio;
        }

        float& animated_ratio = animated_ammo_map[weapon_id];
        animated_ratio = std::lerp(animated_ratio, current_ratio, 0.1f);
        if (esp_debug)
            std::cout << "ESP: animated_ratio = " << animated_ratio << std::endl;

        float offset = 3.f;
        float filled = box.width * animated_ratio;

        if (esp_debug)
        {
            std::cout << "ESP: offset = " << offset << std::endl;
            std::cout << "ESP: filled = " << filled << std::endl;
        }

        vec3_t min = { box.x, box.y + box.height + offset };
        vec3_t filled_max = { box.x + filled, min.y + config.esp.ammo_bar_width };
        vec3_t full_max = { box.x + box.width, min.y + config.esp.ammo_bar_width };

        if (min.is_zero() || filled_max.is_zero() || full_max.is_zero())
        {
            if (esp_debug)
                std::cout << "ESP: ammo bar positions are zero, skipping." << std::endl;
            return;
        }

        if (config.esp.ammo_outline)
        {
            if (esp_debug)
                std::cout << "ESP: drawing ammo outline." << std::endl;

            g_render->rect_filled({ min.x - 1, min.y - 1 }, { full_max.x + 1, full_max.y + 1 }, config.esp.ammo_color_outline);
        }

        g_render->rect_filled(min, filled_max, config.esp.ammo_color);
        if (esp_debug)
            std::cout << "ESP: drawing ammo bar filled region." << std::endl;

        if (ammo < max_ammo && config.esp.ammo_bar_text)
        {
            vec3_t pos = { filled_max.x + 2, min.y };
            std::string ammo_str = std::to_string(ammo);

            if (esp_debug)
                std::cout << "ESP: drawing ammo count: " << ammo_str << " at (" << pos.x << ", " << pos.y << ")" << std::endl;

            if (config.esp.ammo_bar_text_outline)
            {
                if (esp_debug)
                    std::cout << "ESP: drawing ammo text outline." << std::endl;

                float ammo_bar_text_outline_offset = 1.0f;
                for (int x_off = -1; x_off <= 1; ++x_off)
                {
                    for (int y_off = -1; y_off <= 1; ++y_off)
                    {
                        if (x_off == 0 && y_off == 0)
                            continue;

                        g_render->text(pos + vec3_t(x_off * ammo_bar_text_outline_offset, y_off * ammo_bar_text_outline_offset), ammo_str, config.esp.ammo_text_color_outline, g_render->fonts.verdana, g_render->fonts.verdana->FontSize);
                    }
                }
            }

            g_render->text(pos, ammo_str, config.esp.ammo_bar_text_color, g_render->fonts.verdana, g_render->fonts.verdana->FontSize);
        }
    }
}
void ESP::draw_weapons(bbox_t box, vec3_t left_top, vec3_t bottom_right, c_cs_player_pawn* player_pawn, c_cs_player_controller* player_controller)
{
    if (config.esp.onlyvisible)
    {
        if (!is_player_visible_esp(g_ctx.local, player_pawn))
        {
            return;
        }
    }
    if (config.esp.show_weapon)
    {
        auto weapon = player_pawn->get_active_weapon();
        if (!weapon)
        {
            if (esp_debug)
                std::cout << "ESP: weapon is null, skipping weapon display." << std::endl;
            return;
        }
        auto weapon_data = weapon->get_weapon_data();
        if (!weapon_data)
        {
            if (esp_debug)
                std::cout << "ESP: weapon_data is null, skipping weapon display." << std::endl;
            return;
        }
        const char* name = weapon_data->name();
        if (!name || *name == '\0')
        {
            if (esp_debug)
                std::cout << "ESP: weapon name is empty, skipping weapon display." << std::endl;
            return;
        }

        if (config.esp.show_ammo)
        {
            float ammo_bar_height = config.esp.ammo_bar_width;  
            current_y += ammo_bar_height + 4.f;                 
        }

        if (weapon && config.esp.weapon_name)
        {
            const char* name_len = strstr(name, "weapon_");
            if (!name_len || *name_len == '\0')
            {
                if (esp_debug)
                    std::cout << "ESP: weapon name does not contain 'weapon_', skipping." << std::endl;
                return;
            }

            std::string weapon_name = name_len ? name_len + 7 : name;

            float name_height = ImGui::CalcTextSize(weapon_name.c_str()).y;

            vec3_t weapon_pos = { box.x + box.width * 0.5f - ImGui::CalcTextSize(weapon_name.c_str()).x * 0.5f, current_y };
            if (weapon_pos.is_zero())
            {
                if (esp_debug)
                    std::cout << "ESP: weapon_pos is zero vector, skipping weapon name draw." << std::endl;
                return;
            }

            current_y += name_height + 1.f;

            if (config.esp.weapon_name_outline)
            {
                if (esp_debug)
                    std::cout << "ESP: drawing weapon name outline." << std::endl;

                for (int x_off = -1; x_off <= 1; ++x_off)
                {
                    for (int y_off = -1; y_off <= 1; ++y_off)
                    {
                        if (x_off == 0 && y_off == 0)
                            continue;

                        g_render->text(weapon_pos + vec3_t(x_off * 1.0f, y_off * 1.0f), weapon_name.c_str(), config.esp.weapon_color_outline, g_render->fonts.verdana, g_render->fonts.verdana->FontSize);
                    }
                }
                g_render->text(weapon_pos, weapon_name.c_str(), config.esp.weapon_color, g_render->fonts.verdana, g_render->fonts.verdana->FontSize);
            }

            if (weapon_name.empty())
            {
                if (esp_debug)
                    std::cout << "ESP: weapon_name string is empty after parsing, skipping." << std::endl;
                return;
            }
            else if (esp_debug)
            {
                std::cout << "ESP: weapon_name parsed: " << weapon_name << std::endl;
            }
        }
        if (weapon && config.esp.weapon_icon)
        {
            std::string weapon_icon = GunIcon(name);
            if (weapon_icon.empty())
            {
                if (esp_debug)
                    std::cout << "ESP: weapon icon name empty, skipping." << std::endl;
                return;
            }

            ImGui::PushFont(gun_icon10);
            ImVec2 text_size = ImGui::CalcTextSize(weapon_icon.c_str());
            ImGui::PopFont();

            if (!text_size.x && !text_size.y)
            {
                if (esp_debug)
                    std::cout << "ESP: weapon icon text size zero, skipping." << std::endl;
                return;
            }

            float icon_offset_y = box.y + box.height + 2.f;

            vec3_t weapon_icon_pos = { box.x + box.width * 0.5f - text_size.x * 0.5f, current_y };
            if (weapon_icon_pos.is_zero())
            {
                if (esp_debug)
                    std::cout << "ESP: weapon_icon_pos is zero, skipping." << std::endl;
                return;
            }

            if (config.esp.weapon_name_outline)
            {
                if (esp_debug)
                    std::cout << "ESP: drawing weapon icon outline." << std::endl;

                for (int x_off = -1; x_off <= 1; ++x_off)
                {
                    for (int y_off = -1; y_off <= 1; ++y_off)
                    {
                        if (x_off == 0 && y_off == 0)
                            continue;

                        g_render->text(weapon_icon_pos + vec3_t(x_off, y_off), weapon_icon.c_str(), config.esp.weapon_color_outline, gun_icon10, gun_icon10->FontSize);
                    }
                }
            }
            g_render->text(weapon_icon_pos, weapon_icon.c_str(), config.esp.weapon_color, gun_icon10, 10);
        }
    }
}
void ESP::draw_boxes(bbox_t box, vec3_t left_top, vec3_t bottom_right, c_cs_player_pawn* player_pawn, c_cs_player_controller* player_controller)
{
    if (config.esp.onlyvisible)
    {
        if (!is_player_visible_esp(g_ctx.local, player_pawn))
        {
            return;
        }
    }
    if (config.esp.show_box)
    {
        if (esp_debug)
            std::cout << "ESP: show_box enabled, boxtype=" << config.esp.boxtype << std::endl;

        if (config.esp.boxtype == 0)
        {
            if (config.esp.box_outline)
            {
                if (esp_debug)
                    std::cout << "ESP: drawing box outline for boxtype 0." << std::endl;

                g_render->rect(left_top - vec3_t(1, 1), bottom_right + vec3_t(1, 1), config.esp.box_outline_color);
                g_render->rect(left_top + vec3_t(1, 1), bottom_right - vec3_t(1, 1), config.esp.box_outline_color);
            }
            g_render->rect(left_top, bottom_right, config.esp.box_color);
        }
        else if (config.esp.boxtype == 1)
        {
            if (esp_debug)
                std::cout << "ESP: drawing corner box (boxtype 1)." << std::endl;

            vec3_t box_min = vec3_t(box.x, box.y);
            vec3_t box_max = vec3_t(box.x + box.width, box.y + box.height);

            float line_length_x = (box_max.x - box_min.x) / 4.0f;
            float line_length_y = (box_max.y - box_min.y) / 4.0f;

            if (config.esp.box_outline)
            {
                if (esp_debug)
                    std::cout << "ESP: drawing box outline for corner box." << std::endl;

                g_render->line(box_min - vec3_t(1, 1), box_min + vec3_t(line_length_x - 1, -1), config.esp.box_outline_color);
                g_render->line(box_min - vec3_t(1, 1), box_min + vec3_t(-1, line_length_y - 1), config.esp.box_outline_color);

                g_render->line(vec3_t(box_max.x + 1, box_min.y - 1), vec3_t(box_max.x - line_length_x + 1, box_min.y - 1), config.esp.box_outline_color);
                g_render->line(vec3_t(box_max.x + 1, box_min.y - 1), vec3_t(box_max.x + 1, box_min.y + line_length_y - 1), config.esp.box_outline_color);

                g_render->line(vec3_t(box_min.x - 1, box_max.y + 1), vec3_t(box_min.x - 1, box_max.y - line_length_y + 1), config.esp.box_outline_color);
                g_render->line(vec3_t(box_min.x - 1, box_max.y + 1), vec3_t(box_min.x + line_length_x - 1, box_max.y + 1), config.esp.box_outline_color);

                g_render->line(box_max + vec3_t(1, 1), box_max - vec3_t(line_length_x - 1, -1), config.esp.box_outline_color);
                g_render->line(box_max + vec3_t(1, 1), box_max - vec3_t(-1, line_length_y - 1), config.esp.box_outline_color);
            }

            g_render->line(box_min, box_min + vec3_t(line_length_x, 0), config.esp.box_color);
            g_render->line(box_min, box_min + vec3_t(0, line_length_y), config.esp.box_color);

            g_render->line(vec3_t(box_max.x, box_min.y), vec3_t(box_max.x - line_length_x, box_min.y), config.esp.box_color);
            g_render->line(vec3_t(box_max.x, box_min.y), vec3_t(box_max.x, box_min.y + line_length_y), config.esp.box_color);

            g_render->line(vec3_t(box_min.x, box_max.y), vec3_t(box_min.x, box_max.y - line_length_y), config.esp.box_color);
            g_render->line(vec3_t(box_min.x, box_max.y), vec3_t(box_min.x + line_length_x, box_max.y), config.esp.box_color);

            g_render->line(box_max, box_max - vec3_t(line_length_x, 0), config.esp.box_color);
            g_render->line(box_max, box_max - vec3_t(0, line_length_y), config.esp.box_color);
        }
    }
}
void ESP::draw_flags(bbox_t box, vec3_t left_top, vec3_t bottom_right, c_cs_player_pawn* player_pawn, c_cs_player_controller* player_controller)
{
    if (config.esp.onlyvisible)
    {
        if (!is_player_visible_esp(g_ctx.local, player_pawn))
        {
            return;
        }
    }
    if (config.esp.show_flags)
    {
        if (esp_debug)
            std::cout << "[ESP] show_flags enabled" << std::endl;
        std::string flag_hit;
        std::string flag_sync;
        std::string flag_scoped;
        std::string flag_c4;
        std::string flag_armor;
        std::string flag_kits;
        std::string flag_flashed;
        std::string flag_zeus;
        std::string flag_defuser;
        std::string flag_immune;
        if (g_ragebot->m_best_target)
        {
        if(player_pawn == g_ragebot->m_best_target->m_pawn)
        {
            flag_hit = "HIT";
            if (esp_debug) std::cout << "  + HIT\n";
		}
        }

        if (player_pawn->scoped())
        {
            flag_scoped = "SCOPED";
            if (esp_debug) std::cout << "  + SCOPED\n";
        }
        //std::future<std::unordered_map<std::string, std::string>> future = g_authorization->load_known_players();
        //std::unordered_map<std::string, std::string> known_players = future.get();

        //std::string player_nickname = player_controller->player_name();
        //for (const auto& [steamid, name] : known_players) {
            //if (_stricmp(name.c_str(), player_nickname.c_str()) == 0) {
                //flag_sync = "GS";
            //}
        //}

        if (player_pawn->has_c4())
        {
            flag_c4 = "C4";
            if (esp_debug) std::cout << "  + C4\n";
        }

        if (player_pawn->armor_value() > 0)
        {
            if (player_pawn->item_services() && player_pawn->item_services()->has_helmet())
                flag_armor = "HK"; 
            else
                flag_armor = "K";   

            if (esp_debug) std::cout << "  + " << flag_armor << '\n';
        }

        if (player_pawn->item_services() && player_pawn->item_services()->has_defuser())
        {
            flag_kits = "KITS";
            if (esp_debug) std::cout << "  + KITS\n";
        }

        if (player_pawn->flash_duration() > 0.f)
        {
            flag_flashed = "FLASHED";
            if (esp_debug) std::cout << "  + FLASHED\n";
        }

        if (player_pawn->weapon_services() && player_pawn->weapon_services()->weapon_owns_this_type("weapon_taser"))
        {
            flag_zeus = "ZEUS";
            if (esp_debug) std::cout << "  + ZEUS\n";
        }

        if (player_pawn->is_defusing())
        {
            flag_defuser = "DEFUSER";
            if (esp_debug) std::cout << "  + DEFUSER\n";
        }

        if (player_pawn->gungame_immunity() > 0)
        {
            flag_immune = "IMMUNE";
            if (esp_debug) std::cout << "  + IMMUNE\n";
        }

        int start_x = box.x + box.width + 5;
        int start_y = box.y;
        const int text_height = 8; 

        auto draw_flag = [&](const std::string& flag) {
            if (!flag.empty())
            {
                vec3_t pos{ (float)start_x, (float)start_y, 0.f };

                if (config.esp.flags_outline)
                {
                    float outline_offset = 1.0f;
                    for (int x_off = -1; x_off <= 1; ++x_off)
                    {
                        for (int y_off = -1; y_off <= 1; ++y_off)
                        {
                            if (x_off == 0 && y_off == 0)
                                continue;

                            g_render->text(pos + vec3_t{ x_off * outline_offset, y_off * outline_offset, 0.f },
                                flag,
                                config.esp.flags_text_color_outline,
                                g_render->fonts.onetap_pixel,
                                g_render->fonts.onetap_pixel->FontSize);
                        }
                    }
                }

                g_render->text(pos, flag, config.esp.flags_color, g_render->fonts.onetap_pixel, g_render->fonts.onetap_pixel->FontSize);

                start_y += text_height;
            }
            };

        draw_flag(flag_hit);
		//draw_flag(flag_sync);
        draw_flag(flag_scoped);
        draw_flag(flag_c4);
        draw_flag(flag_armor);
        draw_flag(flag_kits);
        draw_flag(flag_flashed);
        draw_flag(flag_zeus);
        draw_flag(flag_defuser);
        draw_flag(flag_immune);
    }
}
void ESP::draw_skeleton(bbox_t box, vec3_t left_top, vec3_t bottom_right, c_cs_player_pawn* player_pawn, c_cs_player_controller* player_controller)
{
    if (config.esp.onlyvisible)
    {
        if (!is_player_visible_esp(g_ctx.local, player_pawn))
        {
            return;
        }
    }
    if (config.esp.show_skeleton)
    {
        if (esp_debug)
            std::cout << "ESP: show_skeleton enabled, drawing bones." << std::endl;

        for (const auto& bone_connection : bone_connections)
        {
            int bone_start = bone_connection.first;
            if (!bone_start)
            {
                if (esp_debug)
                    std::cout << "ESP: bone_start is 0, skipping." << std::endl;
                continue;
            }
            int bone_end = bone_connection.second;
            if (!bone_end)
            {
                if (esp_debug)
                    std::cout << "ESP: bone_end is 0, skipping." << std::endl;
                continue;
            }

            vec3_t pos_start = player_pawn->get_bone_position(bone_start);
            if (pos_start.is_zero())
            {
                if (esp_debug)
                    std::cout << "ESP: pos_start is zero vector, skipping bone line." << std::endl;
                continue;
            }
            vec3_t pos_end = player_pawn->get_bone_position(bone_end);
            if (pos_end.is_zero())
            {
                if (esp_debug)
                    std::cout << "ESP: pos_end is zero vector, skipping bone line." << std::endl;
                continue;
            }

            vec3_t screen_start, screen_end;

            if (g_render->world_to_screen(pos_start, screen_start) && g_render->world_to_screen(pos_end, screen_end))
            {
                if (esp_debug)
                    std::cout << "ESP: drawing skeleton bone line from (" <<
                    screen_start.x << "," << screen_start.y << ") to (" <<
                    screen_end.x << "," << screen_end.y << ")." << std::endl;

                if (config.esp.skeleton_outline)
                {
                    g_render->line({ screen_start.x - 1.0f, screen_start.y - 1.0f }, { screen_end.x - 1.0f, screen_end.y - 1.0f }, config.esp.skeleton_outline_color);
                    g_render->line({ screen_start.x + 1.0f, screen_start.y - 1.0f }, { screen_end.x + 1.0f, screen_end.y - 1.0f }, config.esp.skeleton_outline_color);
                    g_render->line({ screen_start.x - 1.0f, screen_start.y + 1.0f }, { screen_end.x - 1.0f, screen_end.y + 1.0f }, config.esp.skeleton_outline_color);
                    g_render->line({ screen_start.x + 1.0f, screen_start.y + 1.0f }, { screen_end.x + 1.0f, screen_end.y + 1.0f }, config.esp.skeleton_outline_color);
                }
                g_render->line(screen_start, screen_end, config.esp.skeleton_color);
            }
            else
            {
                if (esp_debug)
                    std::cout << "ESP: world_to_screen failed for bone line, skipping." << std::endl;
                continue;
            }
        }
    }
}
void ESP::draw_sounds(bbox_t box, vec3_t left_top, vec3_t bottom_right, c_cs_player_pawn* player_pawn, c_cs_player_controller* player_controller)
{
    if (config.esp.show_sound)
    {
        auto game_scene_node_pawn = player_pawn->game_scene_node();
        if (!game_scene_node_pawn)
            return;
        float sound = player_pawn->emit_sound_time();

        static float static_sound = sound;

        if (static_sound != sound) {
            sound_info.push_back(sound_info_t{ game_scene_node_pawn->vec_abs_origin(), true });
            static_sound = sound;
        }

        for (int i = 0; i < sound_info.size(); i++)
        {
            g_render->radial_gradient_3d(sound_info[i].player_origin, 20.f, config.esp.sound_color, 1.f);
            if (i >= 3)
            {
                sound_info.erase(sound_info.begin(), sound_info.begin() + 1);
            }
        }
    }
}
void ESP::draw_snaplines(bbox_t box, vec3_t left_top, vec3_t bottom_right, c_cs_player_pawn* player_pawn, c_cs_player_controller* player_controller)
{
    if (config.esp.onlyvisible)
    {
        if (!is_player_visible_esp(g_ctx.local, player_pawn))
        {
            return;
        }
    }
    if (config.esp.show_snapline)
    {
        if (config.esp.show_snapline)
        {
            if (esp_debug)
                std::cout << "ESP: snapline enabled." << std::endl;

            auto local_node = g_ctx.local ? g_ctx.local->game_scene_node() : nullptr;
            if (!local_node)
            {
                if (esp_debug)
                    std::cout << "ESP: Snapline - local_node is nullptr." << std::endl;
                return;
            }

            vec3_t local_pos = local_node->vec_origin();
            if (local_pos.is_zero())
            {
                if (esp_debug)
                    std::cout << "ESP: Snapline - local_pos is zero." << std::endl;
                return;
            }

            auto pawn_node = player_pawn->game_scene_node();
            if (!pawn_node)
            {
                if (esp_debug)
                    std::cout << "ESP: Snapline - pawn_node is nullptr." << std::endl;
                return;
            }

            vec3_t player_pos = pawn_node->vec_origin();
            if (player_pos.is_zero())
            {
                if (esp_debug)
                    std::cout << "ESP: Snapline - player_pos is zero." << std::endl;
                return;
            }

            vec3_t screen_pos;
            if (!g_render->world_to_screen(player_pos, screen_pos))
            {
                if (esp_debug)
                    std::cout << "ESP: Snapline - failed to convert player_pos to screen." << std::endl;
                return;
            }

            vec3_t from, to;
            const float outline = 1.0f;

            if (config.esp.snaplinetype == 0)
            {
                from = { g_render->m_screen_size.x * 0.5f, g_render->m_screen_size.y };
                to = { box.x + box.width * 0.5f, box.y + box.height };

                if (esp_debug)
                    std::cout << "ESP: Snapline bottom-to-bottom from screen_bottom to box_bottom." << std::endl;
            }
            else if (config.esp.snaplinetype == 1)
            {
                from = { g_render->m_screen_size.x * 0.5f, 0.f };
                to = { box.x + box.width * 0.5f, box.y };

                if (esp_debug)
                    std::cout << "ESP: Snapline top-to-top from screen_top to box_top." << std::endl;
            }
            else if (config.esp.snaplinetype == 2)
            {
                from = { g_render->m_screen_size.x * 0.5f, g_render->m_screen_size.y * 0.5f };
                to = { box.x + box.width * 0.5f, box.y };

                if (esp_debug)
                    std::cout << "ESP: Snapline center-to-top from screen_center to box_top." << std::endl;
            }
            else
            {
                if (esp_debug)
                    std::cout << "ESP: Snapline - invalid snaplinetype: " << config.esp.snaplinetype << std::endl;
                return;
            }

            if (config.esp.snapline_outline)
            {
                for (int x_off = -1; x_off <= 1; ++x_off)
                {
                    for (int y_off = -1; y_off <= 1; ++y_off)
                    {
                        if (x_off == 0 && y_off == 0)
                            continue;

                        vec3_t offset = { x_off * outline, y_off * outline };
                        g_render->line(from, to + offset, config.esp.snapline_outline_color);
                    }
                }


                if (esp_debug)
                    std::cout << "ESP: Snapline - drew outlines." << std::endl;
            }

            g_render->line(from, to, config.esp.snapline_color);
            if (esp_debug)
                std::cout << "ESP: Snapline - drew main line." << std::endl;
        }
    }
}
void ESP::draw_weapon_in_world(bbox_t box, vec3_t left_top, vec3_t bottom_right, c_cs_player_pawn* player_pawn, c_cs_player_controller* player_controller)
{
    if (config.world.weapon && g_ctx.local_controller)
    {

        for (auto& it : ESP::m_dropped_weapons) {
            if (it.is_invalid || !it.weapon)
                continue;

            auto node = it.weapon->game_scene_node();
            if (!node)
                continue;

            vec3_t world_pos = node->vec_abs_origin();
            if (world_pos.is_zero())
            {
                continue;
            }
            vec3_t screen_pos;

            if (!g_render->world_to_screen(world_pos, screen_pos))
                continue;

            bbox_t box = calculate_bbox_weapon(it.weapon);
            if (box.x < 0 || box.y < 0 || box.width <= 0 || box.height <= 0) {
                continue;
            }

            c_game_scene_node* local_node = g_ctx.local->game_scene_node();
            if (!local_node)
                continue;
            vec3_t local_pos = local_node->vec_abs_origin();
            if (local_pos.is_zero())
                continue;
            float distance = local_pos.dist(world_pos);
            float alpha = 255.f;
            if (distance > 500.f) {
                float fade = std::clamp((distance - 500.f) / 400.f, 0.f, 1.f);
                alpha *= (1.f - fade);
            }

            c_color color(255, 255, 255, static_cast<int>(alpha));
            vec3_t top_left = { box.x, box.y };
            if (top_left.is_zero())
                continue;
            vec3_t bottom_right = { box.x + box.width, box.y + box.height };
            if (bottom_right.is_zero())
                continue;
            if (config.world.weaponbox) {
                if (!config.world.weapon_outline_box)
                {
                    g_render->rect(top_left, bottom_right, config.world.weapon_box_color);
                }
                if (config.world.weapon_outline_box)
                {
                    g_render->rect(top_left - vec3_t(1, 1), bottom_right + vec3_t(1, 1), config.world.weapon_box_outline_color);
                    g_render->rect(top_left + vec3_t(1, 1), bottom_right - vec3_t(1, 1), config.world.weapon_box_outline_color);
                    g_render->rect(top_left, bottom_right, config.world.weapon_box_color);
                }
            }

            if (config.world.weaponname) {
                std::string name = it.weapon_name;
                if (name.empty())
                    continue;
                float text_width = ImGui::CalcTextSize(name.c_str()).x;
                if (!text_width)
                    continue;
                vec3_t name_pos = { top_left.x + (bottom_right.x - top_left.x) * 0.5f - text_width * 0.5f,top_left.y - 15.f };
                if (name_pos.is_zero())
                    continue;

                if (config.world.weapon_outline_name) {
                    for (int x = -1; x <= 1; x++) {
                        for (int y = -1; y <= 1; y++) {
                            if (x == 0 && y == 0) continue;
                            g_render->text(name_pos + vec3_t(x, y), name.c_str(), config.world.weapon_name_outline_color,
                                g_render->fonts.verdana, g_render->fonts.verdana->FontSize);
                        }
                    }
                }

                g_render->text(name_pos, name.c_str(), config.world.weapon_name_color,
                    g_render->fonts.verdana, g_render->fonts.verdana->FontSize);
            }

            if (config.world.weaponicon) {
                std::string icon = GunIcon(it.szweapon_name.c_str());
                if (icon.empty())
                    continue;
                float icon_width = ImGui::CalcTextSize(icon.c_str(), nullptr, true, FLT_MAX).x;
                if (!icon_width)
                    continue;
                vec3_t icon_pos = { top_left.x + (bottom_right.x - top_left.x) * 0.5f - icon_width * 0.5f,top_left.y + 18.f };
                if (icon_pos.is_zero())
                    continue;

                g_render->text(icon_pos, icon.c_str(), config.world.weapon_icon_color,
                    gun_icon10, gun_icon10->FontSize);
            }
        }
    }
}
void ESP::draw_c4(bbox_t box, vec3_t left_top, vec3_t bottom_right, c_cs_player_pawn* player_pawn, c_cs_player_controller* player_controller)
{
    auto planted_c4_entities = entity::get("C_PlantedC4");
    if (planted_c4_entities.empty())
    {
        if (esp_debug)
            std::cout << "ESP: no C_PlantedC4 entities found." << std::endl;
    }
    else if (esp_debug)
    {
        std::cout << "ESP: found " << planted_c4_entities.size() << " C_PlantedC4 entities." << std::endl;
    }
    if (config.world.bomb)
    {
        for (auto entity : planted_c4_entities) {
            if (!entity)
                continue;

            auto c4 = reinterpret_cast<c_planted_c4*>(entity);
            if (!c4)
                continue;

            auto gamenode = c4->game_scene_node();
            if (!gamenode)
                continue;

            vec3_t world_pos = gamenode->vec_abs_origin();
            if (world_pos.is_zero())
                continue;
            vec3_t screen_pos;

            if (!g_render->world_to_screen(world_pos, screen_pos))
                continue;

            is_planted = c4->is_c4_activated();
            if (!is_planted)
                continue;

            bomb_site = c4->get_bomb_site();
            if (bomb_site < 0 || bomb_site > 1)
                continue;
            time_to_explode = (c4->get_c4_blow_time() - interfaces::global_vars->current_time) + 1.f;
            if (time_to_explode < 0.f)
                continue;

            std::string explode_timer_str = std::format("{:.1f}", time_to_explode);
            if (explode_timer_str.empty())
                continue;

            c_color color = { 177, 0, 0 };
            if (c4->if_bomb_defused())
                color = { 0, 177, 0 };

            constexpr int box_width = 30.0f;
            constexpr int box_height = 20.0f;

            vec3_t top_left = { screen_pos.x - box_width / 2, screen_pos.y - box_height / 2 };
            if (top_left.is_zero())
                continue;
            vec3_t bottom_right = { screen_pos.x + box_width / 2, screen_pos.y + box_height / 2 };
            if (bottom_right.is_zero())
                continue;
            if (config.world.bombbox)
            {
                if (!config.world.bomb_outline_box)
                    g_render->rect(top_left, bottom_right, config.world.bomb_box_color);
                if (config.world.bomb_outline_box)
                {
                    g_render->rect(top_left - vec3_t(1, 1), bottom_right + vec3_t(1, 1), config.world.bomb_box_outline_color);
                    g_render->rect(top_left + vec3_t(1, 1), bottom_right - vec3_t(1, 1), config.world.bomb_box_outline_color);
                    g_render->rect(top_left, bottom_right, config.esp.box_color);
                }
            }
            if (config.world.bombname)
            {
                float name_text_outline_offset = 1.0f;

                std::string bomb_name = "Planted C4";
                if (bomb_name.empty())
                    continue;
                float text_width = ImGui::CalcTextSize(bomb_name.c_str()).x;
                if (text_width <= 0.f)
                    continue;

                vec3_t name_pos = { top_left.x + (bottom_right.x - top_left.x) * 0.5f - text_width * 0.5f,top_left.y - 15.f };

                if (name_pos.is_zero())
                    continue;

                if (config.world.bomb_outline_name)
                {
                    for (int x_off = -1; x_off <= 1; ++x_off)
                    {
                        for (int y_off = -1; y_off <= 1; ++y_off)
                        {
                            if (x_off == 0 && y_off == 0)
                                continue;

                            g_render->text(name_pos + vec3_t(x_off * name_text_outline_offset, y_off * name_text_outline_offset), bomb_name.c_str(), config.world.bomb_name_outline_color, g_render->fonts.verdana, g_render->fonts.verdana->FontSize);
                        }
                    }
                }

                g_render->text(name_pos, bomb_name.c_str(), config.world.bomb_name_color, g_render->fonts.verdana, g_render->fonts.verdana->FontSize);
            }
            if (config.world.bombtimertext)
            {
                float timer_text_outline_offset = 1.0f;

                float text_width = ImGui::CalcTextSize(explode_timer_str.c_str()).x;
                if (text_width <= 0.f)
                    continue;

                vec3_t name_pos = { top_left.x + (bottom_right.x - top_left.x) * 0.5f - text_width * 0.5f,top_left.y + 15.f };

                if (name_pos.is_zero())
                    continue;

                if (config.world.bomb_outline_timertext)
                {
                    for (int x_off = -1; x_off <= 1; ++x_off)
                    {
                        for (int y_off = -1; y_off <= 1; ++y_off)
                        {
                            if (x_off == 0 && y_off == 0)
                                continue;

                            g_render->text(name_pos + vec3_t(x_off * timer_text_outline_offset, y_off * timer_text_outline_offset), explode_timer_str.c_str(), config.world.bomb_timer_text_outline_color, g_render->fonts.verdana, g_render->fonts.verdana->FontSize);
                        }
                    }
                }

                g_render->text(name_pos, explode_timer_str.c_str(), config.world.bomb_timer_text_color, g_render->fonts.verdana, g_render->fonts.verdana->FontSize);
            }
        }
    }
}
void ESP::draw_chicken(bbox_t box, vec3_t left_top, vec3_t bottom_right, c_cs_player_pawn* player_pawn, c_cs_player_controller* player_controller)
{
    auto chicken_entities = entity::get("C_Chicken");
    if (chicken_entities.empty())
    {
        if (esp_debug)
            std::cout << "ESP: no C_Chicken entities found." << std::endl;
    }
    else if (esp_debug)
    {
        std::cout << "ESP: found " << chicken_entities.size() << " C_Chicken entities." << std::endl;
    }
    if (config.esp.chicken)
    {
        for (auto chicken : chicken_entities)
        {
            if (!chicken)
                continue;

            auto base_entity = reinterpret_cast<c_base_entity*>(chicken);
            if (!base_entity)
                continue;

            auto node = base_entity->game_scene_node();
            if (!node)
                continue;

            vec3_t origin = node->vec_abs_origin();
            if (origin.is_zero())
                continue;
            vec3_t top = origin + vec3_t(0.f, 0.f, 20.f);
            if (top.is_zero())
                continue;
            vec3_t screen_pos, screen_top;

            if (!g_render->world_to_screen(origin, screen_pos) || !g_render->world_to_screen(top, screen_top))
                continue;

            float height = std::fabsf(screen_top.y - screen_pos.y);
            if (height <= 0.f)
                continue;
            float width = height * 1.6f;
            if (width <= 0.f)
                continue;

            vec3_t top_left = { screen_pos.x - width / 2.f, screen_top.y };
            if (top_left.is_zero())
                continue;
            vec3_t bottom_right = { screen_pos.x + width / 2.f, screen_pos.y };
            if (bottom_right.is_zero())
                continue;

            if (config.esp.chickenbox)
            {
                g_render->rect(top_left, bottom_right, config.esp.chicken_box_color);
            }

            if (config.esp.chickenname)
            {
                std::string text = "Chicken";
                if (text.empty())
                    continue;
                float text_width = ImGui::CalcTextSize(text.c_str()).x;
                if (text_width <= 0.f)
                    continue;
                vec3_t text_pos = { screen_pos.x - text_width / 2.f, bottom_right.y + 1.f };
                if (text_pos.is_zero())
                    continue;

                g_render->text(text_pos, text.c_str(), config.esp.chicken_name_color,
                    g_render->fonts.verdana, g_render->fonts.verdana->FontSize);
            }
        }
    }
}
void ESP::draw_hostage(bbox_t box, vec3_t left_top, vec3_t bottom_right, c_cs_player_pawn* player_pawn, c_cs_player_controller* player_controller)
{
    auto hostage_entities = entity::get("C_Hostage");
    if (hostage_entities.empty())
    {
        if (esp_debug)
            std::cout << "ESP: no C_Hostage entities found." << std::endl;
    }
    else if (esp_debug)
    {
        std::cout << "ESP: found " << hostage_entities.size() << " C_Hostage entities." << std::endl;
    }
    if (config.esp.hostage)
    {
        for (auto hostage : hostage_entities)
        {
            if (!hostage)
                continue;

            auto base_entity = reinterpret_cast<c_base_entity*>(hostage);
            if (!base_entity)
                continue;

            auto node = base_entity->game_scene_node();
            if (!node)
                continue;
            c_game_scene_node* local_node = g_ctx.local->game_scene_node();
            if (!local_node)
                continue;
            vec3_t local_pos = local_node->vec_abs_origin();
            if (local_pos.is_zero())
                continue;
            vec3_t origin = node->vec_abs_origin();
            if (origin.is_zero())
                continue;
            vec3_t top = origin + vec3_t(0.f, 0.f, 36.f);
            if (top.is_zero())
                continue;
            vec3_t screen_pos, screen_top;
            if (origin == local_pos)
                continue;
            if (!g_render->world_to_screen(origin, screen_pos) || !g_render->world_to_screen(top, screen_top))
                continue;

            float height = std::fabsf(screen_top.y - screen_pos.y);
            if (height <= 0.f)
                continue;
            float width = height * 0.6f;
            if (width <= 0.f)
                continue;

            vec3_t top_left = { screen_pos.x - width / 2.f, screen_top.y };
            if (top_left.is_zero())
                continue;
            vec3_t bottom_right = { screen_pos.x + width / 2.f, screen_pos.y };
            if (bottom_right.is_zero())
                continue;

            if (config.esp.hostagebox)
            {
                g_render->rect(top_left, bottom_right, config.esp.hostage_box_color);
            }

            if (config.esp.hostagename)
            {
                std::string text = "Hostage";
                if (text.empty())
                    continue;
                float text_width = ImGui::CalcTextSize(text.c_str()).x;
                if (text_width <= 0.f)
                    continue;
                vec3_t text_pos = { screen_pos.x - text_width / 2.f, bottom_right.y + 1.f };
                if (text_pos.is_zero())
                    continue;

                g_render->text(text_pos, text.c_str(), config.esp.hostage_name_color,
                    g_render->fonts.verdana, g_render->fonts.verdana->FontSize);
            }
        }
    }
}
void ESP::esp()
{
    bool logging = false;

    if (!interfaces::engine_client->is_connected() || !interfaces::engine_client->is_in_game())
    {
        if (logging)
            std::cout << "ESP: not connected or not in game." << std::endl;
        return;
    }
    if (!g_ctx.local_controller)
    {
        if (logging)
            std::cout << "ESP: local controller is null." << std::endl;
        return;
    }

    auto player_entities = entity::get("CCSPlayerController");
    if (player_entities.empty())
    {
        if (logging)
            std::cout << "ESP: no CCSPlayerController entities found." << std::endl;
        return;
    }
    else if (logging)
    {
        std::cout << "ESP: found " << player_entities.size() << " CCSPlayerController entities." << std::endl;
    }

    for (auto player_entity : player_entities)
    {
        auto player_controller = reinterpret_cast<c_cs_player_controller*>(player_entity);
        if (!player_controller)
        {
            if (logging)
                std::cout << "ESP: player_controller is null, skipping." << std::endl;
            continue;
        }

        auto player_controller_pawn = player_controller->pawn();
        if (logging)
            std::cout << "ESP: player_controller_pawn obtained: " << player_controller_pawn.get_entry_index() << std::endl;

        auto player_pawn = reinterpret_cast<c_cs_player_pawn*>(interfaces::entity_system->get_base_entity(player_controller_pawn.get_entry_index()));
        if (!player_pawn)
        {
            if (logging)
                std::cout << "ESP: player_pawn is null, skipping." << std::endl;
            continue;
        }

        if (player_controller == g_ctx.local_controller)
        {
            if (logging)
                std::cout << "ESP: player_controller is local controller, skipping." << std::endl;
            continue;
        }

        if (!g_ctx.local && player_pawn == g_ctx.local)
        {
            if (logging)
                std::cout << "ESP: player_pawn is local player, skipping." << std::endl;
            continue;
        }

        if (!player_pawn->is_alive())
        {
            if (logging)
                std::cout << "ESP: player_pawn not alive, skipping." << std::endl;
            continue;
        }

        if (!player_pawn->is_enemy())
        {
            if (logging)
                std::cout << "ESP: player_pawn is not enemy, skipping." << std::endl;
            continue;
        }

        bbox_t box = ESP::calculate_bbox(player_pawn);
        if (!box.m_found)
        {
            if (logging)
                std::cout << "ESP: bbox not found for player_pawn, skipping." << std::endl;
            continue;
        }
        else if (logging)
        {
            std::cout << "ESP: bbox found: x=" << box.x << " y=" << box.y
                << " width=" << box.width << " height=" << box.height << std::endl;
        }

        vec3_t top_left = { box.x, box.y };
        if (top_left.is_zero())
        {
            if (logging)
                std::cout << "ESP: top_left is zero vector, skipping." << std::endl;
            continue;
        }

        vec3_t bottom_right = { box.x + box.width, box.y + box.height };
        if (bottom_right.is_zero())
        {
            if (logging)
                std::cout << "ESP: bottom_right is zero vector, skipping." << std::endl;
            continue;
        }

        if (config.esp.enable)
        {
            if (logging)
                std::cout << "ESP: ESP enabled, proceeding to draw." << std::endl;
            
            current_y = box.y + box.height;

            draw_boxes(box, top_left, bottom_right, player_pawn, player_controller);
            draw_names(box, top_left, bottom_right, player_pawn, player_controller);
            draw_healthbar(box, top_left, bottom_right, player_pawn, player_controller);
            draw_ammobar(box, top_left, bottom_right, player_pawn, player_controller);
            draw_weapons(box, top_left, bottom_right, player_pawn, player_controller);
            draw_flags(box, top_left, bottom_right, player_pawn, player_controller);
            draw_skeleton(box, top_left, bottom_right, player_pawn, player_controller);
            draw_snaplines(box, top_left, bottom_right, player_pawn, player_controller);
            draw_sounds(box, top_left, bottom_right, player_pawn, player_controller);
            draw_c4(box, top_left, bottom_right, player_pawn, player_controller);
            draw_weapon_in_world(box, top_left, bottom_right, player_pawn, player_controller);
            draw_chicken(box, top_left, bottom_right, player_pawn, player_controller);
            draw_hostage(box, top_left, bottom_right, player_pawn, player_controller);
            
            
        }
    }
}

