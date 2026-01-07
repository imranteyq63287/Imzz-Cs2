#include "walkbot.h"


void c_walkbot::get_positions()
{
    if (!interfaces::engine_client && !interfaces::engine_client->is_in_game() && !interfaces::engine_client->is_connected())
        return;
    if (!g_ctx.local || !g_ctx.local_controller)
        return;
    if (!g_ctx.local->is_alive())
        return;
    
    vec3_t local_pos = g_ctx.local->game_scene_node()->vec_abs_origin();
    auto entities = entity::get("CCSPlayerController");

    for (auto entity : entities)
    {
        auto controller = reinterpret_cast<c_cs_player_controller*>(entity);
        if (!controller || controller == g_ctx.local_controller)
            continue;

        auto pawn = reinterpret_cast<c_cs_player_pawn*>(interfaces::entity_system->get_base_entity(controller->pawn().get_entry_index()));
        if (!pawn || !pawn->is_alive() || !g_ctx.local || pawn == g_ctx.local || !pawn->is_enemy())
            continue;

        vec3_t other_pos = pawn->game_scene_node()->vec_abs_origin();
        float distance = (other_pos - local_pos).length();

        if (distance < closest_distance)
        {
            closest_distance = distance;
            closest_enemy = pawn;
            closest_controller = controller;
        }

        //std::cout << "position: x = " << other_pos.x << ", y = " << other_pos.y << ", z = " << other_pos.z << std::endl;
    }

    if (closest_enemy && closest_controller)
    {
        closest_pos = closest_enemy->game_scene_node()->vec_abs_origin();
        const char* player_name = closest_controller->player_name();

        //std::cout << "[" << player_name << "] Position: x = " << closest_pos.x << ", y = " << closest_pos.y << ", z = " << closest_pos.z << std::endl;

        //std::cout << "own position: x = " << local_pos.x << ", y = " << local_pos.y << ", z = " << local_pos.z << std::endl;

    }
    else
    {
        /*std::cout << "No enemy found." << std::endl;*/
    }
}
vec3_t c_walkbot::get_local_viewangles(c_user_cmd* user_cmd)
{
    if (!interfaces::engine_client)
        return {};

    if (!interfaces::engine_client->is_in_game() || !interfaces::engine_client->is_connected())
        return {};

    if (!g_ctx.local || !g_ctx.local_controller)
        return {};

    if (!g_ctx.local->is_alive())
        return {};

    vec3_t original_angles =
    {
        user_cmd->pb.base().viewangles().x(),
        user_cmd->pb.base().viewangles().y(),
        user_cmd->pb.base().viewangles().z()
    };

    std::cout << "own viewangles: x = " << original_angles.x << " y = " << original_angles.y << " z = " << original_angles.z << std::endl;

    return original_angles;
}
void c_walkbot::walk_to_target(c_user_cmd* user_cmd)
{
    if (!user_cmd || !interfaces::engine_client || !interfaces::engine_client->is_in_game() || !interfaces::engine_client->is_connected())
        return;

    if (!g_ctx.local || !g_ctx.local_controller || !g_ctx.local->is_alive() || !config.movement.walkbot)
        return;

    const vec3_t local_eye = g_ctx.local->get_eye_pos();
    vec3_t to_target = closest_pos - local_eye;
    float distance_to_target = to_target.length();

    static vec3_t last_position = local_eye;
    static int stuck_ticks = 0;
    static int stuck_cooldown = 0;
    static bool reversing = false;


    if (distance_to_target < 10.0f)
    {
        user_cmd->pb.mutable_base()->set_forwardmove(0.f);
        stuck_ticks = 0;
        stuck_cooldown = 0;
        reversing = false;
        return;
    }


    if ((local_eye - last_position).length() < 1.f)
        stuck_ticks++;
    else
        stuck_ticks = 0;

    last_position = local_eye;


    vec3_t ideal_angles{};
    math::vector_angles(to_target, ideal_angles);
    ideal_angles.x = 0.f;
    math::normalize_angles(ideal_angles);


    constexpr int ray_count = 72;
    constexpr float max_angle = 90.f;
    constexpr float ray_distance = 100.f;

    vec3_t best_direction = {};
    float best_dot = -1.f;
    bool path_found = false;

    for (int i = 0; i < ray_count; ++i)
    {
        float offset = -max_angle + (2.f * max_angle * i) / (ray_count - 1);
        vec3_t test_angle = ideal_angles;
        test_angle.y += offset;
        math::normalize_angles(test_angle);

        vec3_t dir = math::angle_vectors(test_angle);
        vec3_t end_pos = local_eye + dir * ray_distance;

        ray_t ray{};
        game_trace_t trace{};
        trace_filter_t filter{ 0x1C3003, g_ctx.local, nullptr, 4 };

        if (!interfaces::engine_trace->trace_shape(&ray, local_eye, end_pos, &filter, &trace) || !trace.m_hit_entity)
        {
            float dot = dir.normalized().dot(to_target.normalized());
            if (dot > best_dot)
            {
                best_dot = dot;
                best_direction = test_angle;
                path_found = true;
            }
        }
    }

    vec3_t move_angles = ideal_angles;


    if (path_found)
    {
        move_angles = best_direction;
        reversing = false;
        stuck_cooldown = 0;
    }
    else
    {
        stuck_cooldown++;

        if (stuck_cooldown > 25 && !reversing)
        {
            move_angles.y += 180.f;
            reversing = true;
        }
        else
        {
            move_angles.y += (stuck_ticks % 2 == 0 ? 25.f : -25.f);
        }

        math::normalize_angles(move_angles);
    }


    vec3_t view = get_local_viewangles(user_cmd);
    vec3_t delta = move_angles - view;
    math::normalize_angles(delta);
    view += delta * 0.12f;


    view.y += math::random_float(-0.25f, 0.25f);
    math::normalize_angles(view);
    interfaces::csgo_input->set_view_angles(view);


    if (path_found)
        user_cmd->pb.mutable_base()->set_forwardmove(1.0f);
    else if (stuck_cooldown > 35)
        user_cmd->pb.mutable_base()->set_forwardmove(-1.0f);
    else
        user_cmd->pb.mutable_base()->set_forwardmove(0.0f);
}
