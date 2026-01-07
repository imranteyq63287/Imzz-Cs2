#include "ragebot.h"
#include "../../engine prediction/eprediction.hpp"
#include "../../menu/gui_base/notification.hpp"
#include "../../menu/gui_base/hashes.hpp"
#include "Wall Penetrate/auto_wall.h"
#include "../../../Bind System/key.h"
#include "../../Misc/Movement/movement.h"
#include "../../../sdk/entity/entity.h"
#include "../../../sdk/utils/render/render.h"
#include <future>

constexpr float M_PI = 3.14159265358979323846f;
constexpr float M_PI_F = static_cast<float>(M_PI);
constexpr float RAD2DEG = 180.f / M_PI_F;
constexpr float DEG2RAD = M_PI_F / 180.f;
using namespace std;
#define MASK_SHOT 0x46004003 // Пример маски (включает стены, объекты и т.п.)

class my_trace_filter : public trace_filter_t
{
public:
    c_cs_player_pawn* skip;

    my_trace_filter(c_cs_player_pawn* pSkip) : skip(pSkip) {}

    bool should_hit_entity(void* entity, int contents_mask)
    {
        return entity != skip;
    }
};

bool weapon_is_at_max_accuracy(c_cs_weapon_base_v_data* weapon_data, float inaccuracy) {
    auto local_data = g_prediction->get_local_data();

    if (!local_data)
        return false;

    constexpr auto round_accuracy = [](float accuracy) { return floorf(accuracy * 170.f) / 170.f; };
    constexpr auto round_duck_accuracy = [](float accuracy) { return floorf(accuracy * 300.f) / 300.f; };

    float speed = g_ctx.local->abs_velocity().length();

    bool is_scoped = ((weapon_data->weapon_type() == WEAPONTYPE_SNIPER_RIFLE)
        && !(g_ctx.user_cmd->buttons.button_state & IN_ZOOM)
        && !g_ctx.local->scoped());

    bool is_ducking = ((g_ctx.local->flags() & FL_DUCKING) || (g_ctx.user_cmd->buttons.button_state & IN_DUCK));

    float rounded_accuracy = round_accuracy(inaccuracy);
    float rounded_duck_accuracy = round_duck_accuracy(inaccuracy);

    if (is_ducking && is_scoped && (g_ctx.local->flags() & FL_ONGROUND) && (rounded_duck_accuracy < local_data->m_inaccuracy))
        return true;

    if (speed <= 0 && is_scoped && (g_ctx.local->flags() & FL_ONGROUND) && (rounded_accuracy < local_data->m_inaccuracy))
        return true;

    return false;
}
vec3_t calculate_spread_angles(vec3_t angle, int random_seed, float weapon_inaccuarcy, float weapon_spread)
{
    float r1, r2, r3, r4, s1, c1, s2, c2;

    interfaces::random_seed(random_seed + 1);

    r1 = interfaces::random_float(0.f, 1.f);
    r2 = interfaces::random_float(0.f, _pi2);
    r3 = interfaces::random_float(0.f, 1.f);
    r4 = interfaces::random_float(0.f, _pi2);

    c1 = std::cos(r2);
    c2 = std::cos(r4);
    s1 = std::sin(r2);
    s2 = std::sin(r4);

    vec3_t spread = {
        (c1 * (r1 * weapon_inaccuarcy)) + (c2 * (r3 * weapon_spread)),
        (s1 * (r1 * weapon_inaccuarcy)) + (s2 * (r3 * weapon_spread))
    };

    vec3_t vec_forward, vec_right, vec_up;
    math::angle_vectors(angle, vec_forward, vec_right, vec_up);

    return (vec_forward + (vec_right * spread.x) + (vec_up * spread.y)).normalized();
}
int calculate_hit_chance(c_cs_player_pawn* pawn, vec3_t angles, c_base_player_weapon* active_weapon, c_cs_weapon_base_v_data* weapon_data, bool no_spread) {
    if (!g_ctx.local->is_alive())
        return 0;

    if (!pawn)
        return 0;

    auto local_data = g_prediction->get_local_data();

    if (!local_data)
        return 0;

    if (no_spread)
        return 100;

    vec3_t shoot_pos = local_data->m_eye_pos;

    if (shoot_pos.dist(angles) > weapon_data->range())
        return 0;

    active_weapon->update_accuracy_penality();

    if (weapon_is_at_max_accuracy(weapon_data, active_weapon->get_inaccuracy()))
        return 100;

    const float spread = local_data->m_spread;
    const float inaccuracy = local_data->m_inaccuracy;

    int hits = 0;

    for (int seed = 0; seed < 570; seed++) {
        vec3_t spread_angle = calculate_spread_angles(angles, seed, inaccuracy, spread);

        vec3_t result = spread_angle * weapon_data->range() + shoot_pos;

        ray_t ray{};
        game_trace_t trace{};
        trace_filter_t filter{};
        interfaces::engine_trace->init_trace(filter, g_ctx.local, MASK_SHOT, 0x3, 0x7);

        interfaces::engine_trace->trace_shape(&ray, shoot_pos, result, &filter, &trace);
        interfaces::engine_trace->clip_ray_entity(&ray, shoot_pos, result, pawn, &filter, &trace);

        if (trace.m_hit_entity && trace.m_hit_entity->is_player_pawn() && trace.m_hit_entity->get_ref_ehandle().get_entry_index() == pawn->get_ref_ehandle().get_entry_index())
            hits++;
    }

    return static_cast<int>((static_cast<float>(hits / 570.f)) * 100.f);
}

bool can_shoot(c_cs_player_pawn* pawn, c_base_player_weapon* active_weapon) {
    return (active_weapon->i_clip1() > 0) && (active_weapon->n_next_primary_attack_tick() <= g_ctx.local_controller->tick_base() + 2);
}

void process_attack(c_user_cmd* user_cmd) {
        user_cmd->buttons.button_state |= IN_ATTACK;
}
float get_max_player_speed()
{
    if (!g_ctx.local->get_active_weapon() || !g_ctx.local->get_active_weapon()->get_weapon_data())
        return 260.f;

    return  g_ctx.local->scoped() ? g_ctx.local->get_active_weapon()->get_weapon_data()->firing_max_speed()[1] : g_ctx.local->get_active_weapon()->get_weapon_data()->firing_max_speed()[0];
}
void stop_movement(c_user_cmd* user_cmd, c_cs_player_pawn* local_player)
{
    if (!local_player)
        return;

    vec3_t velocity = local_player->velocity();
    float speed = velocity.length_2d();

    if (speed < 1.f)
    {
        user_cmd->pb.mutable_base()->set_forwardmove(0.f);
        user_cmd->pb.mutable_base()->set_leftmove(0.f);
        return;
    }

    vec3_t velocity_angle;
    math::vector_angles(velocity * -1.f, velocity_angle);

    velocity_angle.y = interfaces::csgo_input->get_view_angles().y - velocity_angle.y;

    vec3_t forward;
    math::angle_vectors(velocity_angle, forward);

    float stop_power = std::min<float>(speed * 2.0f, get_max_player_speed());

    user_cmd->pb.mutable_base()->set_forwardmove(forward.x * stop_power);
    user_cmd->pb.mutable_base()->set_leftmove(forward.y * -stop_power);
}

void limit_speed(c_user_cmd* user_cmd, c_cs_player_pawn* local_player, c_base_player_weapon* active_weapon, float max_speed)
{
    if (!local_player || !active_weapon)
        return;

    c_player_movement_service* movement_services = local_player->movement_services();
    if (!movement_services)
        return;

    vec3_t velocity = local_player->abs_velocity();
    float speed_2d = velocity.length_2d();

    if (speed_2d <= max_speed)
    {
        user_cmd->pb.mutable_base()->set_forwardmove(0.f);
        user_cmd->pb.mutable_base()->set_leftmove(0.f);
        return;
    }

    vec3_t stop_dir = -velocity.normalize_in_place();

    vec3_t view_angles = interfaces::csgo_input->get_view_angles();
    view_angles.x = 0.f;

    float sy, cy;
    math::sin_cos(DEG2RAD * view_angles.y, sy, cy);

    vec3_t local_stop_dir;
    local_stop_dir.x = stop_dir.x * cy + stop_dir.y * sy;
    local_stop_dir.y = -stop_dir.x * sy + stop_dir.y * cy;

    user_cmd->pb.mutable_base()->set_forwardmove(local_stop_dir.x);
    user_cmd->pb.mutable_base()->set_leftmove(local_stop_dir.y);

    if (speed_2d > 50.f)
    {
        const float scale = max_speed / speed_2d;
        user_cmd->pb.mutable_base()->set_forwardmove(user_cmd->pb.mutable_base()->forwardmove() * scale);
        user_cmd->pb.mutable_base()->set_leftmove(user_cmd->pb.mutable_base()->leftmove() * scale);
    }
}
void auto_stop(c_user_cmd* user_cmd, c_cs_player_pawn* local_player, c_base_player_weapon* active_weapon, bool no_spread)
{
    if (!config.ragebot.auto_stop)
        return;

    if (no_spread)
        return;

    if (local_player->velocity().length_2d() <= 0.f)
        return;

    float max_accurate_speed = get_max_player_speed() * 0.25f;
    float wish_speed = active_weapon->get_max_speed() * 0.25f;
    if (local_player->velocity().length_2d() <= max_accurate_speed)
        return limit_speed(user_cmd, local_player, active_weapon, wish_speed);

    stop_movement(user_cmd, local_player);
}
inline void auto_stop_air(c_user_cmd* user_cmd, c_cs_player_pawn* local_player, c_base_player_weapon* active_weapon, bool no_spread)
{
    if (!local_player || !active_weapon || no_spread)
        return;
    if (!config.ragebot.auto_stop)
        return;
    const vec3_t velocity = local_player->velocity();
    const float speed_2d = velocity.length_2d();

    // Если скорость почти нулевая — сбрасываем движение
    if (speed_2d < 1.0f)
    {
        user_cmd->pb.mutable_base()->set_forwardmove(0.f);
        user_cmd->pb.mutable_base()->set_leftmove(0.f);
        return;
    }

    // Получение firing max speed (всё в одной функции)
    float max_speed = 260.f;
    const auto weapon_data = active_weapon->get_weapon_data();
    if (weapon_data)
    {
        const bool scoped = g_ctx.local && g_ctx.local->scoped();
        max_speed = scoped ? weapon_data->firing_max_speed()[1] : weapon_data->firing_max_speed()[0];
    }

    const float accurate_stop_speed = max_speed * 0.25f;
    const float weapon_limit_speed = active_weapon->get_max_speed() * 0.25f;

    vec3_t direction = velocity;
    direction.z = 0.f;
    direction.normalized();

    direction *= -1.f; // Инверсия для торможения

    // Получаем view angles
    vec3_t view_angles = interfaces::csgo_input->get_view_angles();
    view_angles.x = 0.f;

    float sy, cy;
    math::sin_cos(DEG2RAD * view_angles.y, sy, cy);

    // Перевод в локальные координаты
    vec3_t local_dir;
    local_dir.x = direction.x * cy + direction.y * sy;
    local_dir.y = -direction.x * sy + direction.y * cy;

    // Если скорость уже низкая — лимитируем до weapon_limit_speed
    if (speed_2d <= accurate_stop_speed)
    {
        float scale = std::clamp(weapon_limit_speed / speed_2d, 0.f, 1.f);
        user_cmd->pb.mutable_base()->set_forwardmove(local_dir.x * scale);
        user_cmd->pb.mutable_base()->set_leftmove(local_dir.y * scale);
        return;
    }

    // Иначе — полноценная остановка
    float stop_force = std::min(speed_2d * 2.f, max_speed);
    user_cmd->pb.mutable_base()->set_forwardmove(local_dir.x * stop_force);
    user_cmd->pb.mutable_base()->set_leftmove(local_dir.y * stop_force);
}
void c_ragebot::check_doubletap_status(c_base_player_weapon* weapon) {
    if (!weapon || !g_ctx.local)
        return;
    if (!config.ragebot.enable)
		return;
    if (!config.ragebot.doubletap)
        return;

    if (weapon->i_clip1() <= 1) {
        std::cout << "[DT] Недостаточно патронов для зарядки (1 или меньше).\n";
        config.gui.doubletapready = false;
        config.gui.doubletapbroken = true;
        icondtbroken = reinterpret_cast<const char*>(ICON_FA_BATTERY_EMPTY);
        return;
    }
    constexpr float tick_interval = 1.0f / 64.0f;

    int current_tick = g_ctx.local_controller->tick_base();

    double offset_tick = 0.0;
    modf(weapon->fl_wat_tick_offset(), &offset_tick);
    int shoot_tick = static_cast<int>(weapon->n_next_primary_attack_tick() + offset_tick);

    int ticks_until_next_shot = shoot_tick - current_tick;
    if (ticks_until_next_shot < 0)
        ticks_until_next_shot = 0;

    float time_until_next_shot = ticks_until_next_shot * tick_interval;

    float estimated_fire_delay = 1.0f / weapon->fl_next_primary_attack_tick_ratio();
    float time_needed_for_dt = 2 * estimated_fire_delay * tick_interval;

    if (time_until_next_shot <= 0.0f) {
        std::cout << ("[DT] Ready!\n");
        config.gui.doubletapready = true;
        config.gui.doubletapbroken = false;

    }
    else {
        std::cout << "[DT] Charging... "
            << std::fixed << std::setprecision(2)
            << time_until_next_shot << " sec | Need: "
            << time_needed_for_dt << " sec\n";
        config.gui.doubletapready = false;
        config.gui.doubletapbroken = true;
        icondtbroken = reinterpret_cast<const char*>(ICON_FA_BATTERY_HALF);

    }
}

void c_ragebot::double_tap(c_user_cmd* user_cmd, c_base_player_weapon* weapon) {
    if (!weapon || (!config.ragebot.doubletap && !config.ragebot.enable))
        return;
    if (!config.ragebot.enable)
        return;
    if (!config.ragebot.doubletap)
        return;
    static int shot_count = 0;
    static int old_shot_count = 0;

    double offset_tick = 0.0;
    auto temp = weapon->fl_next_primary_attack_tick_ratio() + modf(weapon->fl_wat_tick_offset(), &offset_tick);
    auto next_attack = weapon->n_next_primary_attack_tick();
    int shoot_tick = static_cast<int>(next_attack + offset_tick);

    if (temp >= 0.0) {
        if (temp >= 1.0) ++shoot_tick;
    }
    else {
        --shoot_tick;
    }

    bool use_tick_offset = (shot_count % 2 == 1);

    if (user_cmd->pb.has_attack1_start_history_index() > -1)
        shot_count++;

    for (int i = 0; i < user_cmd->pb.input_history_size(); ++i) {
        auto entry = user_cmd->pb.mutable_input_history(i);
        if (!entry) continue;

        entry->set_player_tick_count(use_tick_offset ? shoot_tick - 1 : 0);
        entry->set_render_tick_fraction(0.f);
    }

    user_cmd->pb.set_attack1_start_history_index(-1);

    old_shot_count = shot_count;
}



void c_ragebot::ragebot(c_user_cmd* user_cmd)
{
    if (!interfaces::engine_client->is_in_game() || !interfaces::engine_client->is_connected())
        return;

    if (!g_ctx.local || !g_ctx.local->is_alive())
        return;
    config.movement.m_autopeek.did_shoot = false;

    if (!config.ragebot.enable)
        return;
    static int target_hit_chance;
    static int target_damage = 0;
        if (target_damage != config.ragebot.min_damage) {
            target_damage = config.ragebot.min_damage;
            std::cout << "Target damage set to: " << target_damage << "\n";
            if (target_damage == 0)
            {
                std::cout << "Target damage set to: " << "LETHAL" << "\n";

            }
        }
    if (g_key_handler->is_pressed(config.ragebot.min_damage_keybind, config.ragebot.min_damage_keybind_style)) {
        target_damage = config.ragebot.min_damage_override;
    }
    vec3_t original_angles = {
        interfaces::csgo_input->get_view_angles().x,
        interfaces::csgo_input->get_view_angles().y,
		interfaces::csgo_input->get_view_angles().z
    };
    vec3_t local_eye = g_ctx.local->get_eye_pos();
    vec3_t best_angle;
    float closest_dist = FLT_MAX;
    c_cs_player_pawn* best_target = nullptr;
    vec3_t head;

    for (int i = 1; i <= interfaces::entity_system->get_highest_entiy_index(); ++i)
    {
        auto player = (c_cs_player_pawn*)interfaces::entity_system->get_base_entity(i);
        if (!player || !player->is_alive() || player == g_ctx.local)
            continue;

        if (!player->is_enemy())
            continue;

            head = player->get_bone_position(6);


        vec3_t aim_angle = math::calculate_angles(local_eye, head);
        float dist = (head - local_eye).length();
        float fov = math::get_fov(original_angles, local_eye, head);
        if (config.ragebot.group_type == 0)
        {
            if (dist < closest_dist)
            {
                closest_dist = dist;
                best_angle = aim_angle;
                best_target = player;
            }
        }
        if (config.ragebot.group_type == 1)
        {
            if (fov < closest_dist && fov <= config.ragebot.maximum_fov)
            {
                closest_dist = fov;
                best_angle = aim_angle;
                best_target = player;
            }
        }
    }

    if (best_target && closest_dist != FLT_MAX)
    {
        c_base_player_weapon* weapon = g_ctx.local->get_active_weapon();
        if (!weapon)
            return;

        auto weapon_data = weapon->get_weapon_data();
        if (!weapon_data)
            return;

        bool no_spread = /*interfaces::cvar->get_by_name("weapon_accuracy_nospread")->get_bool()*/ false;
        penetration_data_t pen_data{};
        if (!g_auto_wall->fire_bullet(g_ctx.local->get_eye_pos(), head, best_target, weapon_data, pen_data))
        {
            debug_text(WHITE_COLOR, "bullet trace failed, no penetration data found\n");
            return; 
        }
        if (target_damage == 0)
        {
                        target_damage = best_target->health(); 
        }
        if (pen_data.m_damage < target_damage)
        {
			debug_text(WHITE_COLOR, "damage too low: %d < %d\n", pen_data.m_damage, target_damage);
            return; 
        }

        target_hit_chance = config.ragebot.hit_chance;

        if (g_key_handler->is_pressed(config.ragebot.hit_chance_keybind, config.ragebot.hit_chance_keybind_style))
        {
            target_hit_chance = config.ragebot.hit_chance_override;
        }
        int hit_chance = calculate_hit_chance(best_target, best_angle, weapon, weapon_data, no_spread);
        int required_chance = (weapon_data->weapon_type() == WEAPONTYPE_TASER) ? 70 : target_hit_chance;
        std::cout << "GLOBAL Target damage: " << target_damage << "\n";
		std::cout << "GLOBAL Target chance: " << required_chance << "%\n";

        if (!can_shoot(g_ctx.local, weapon))
            return;
        auto_stop(user_cmd, g_ctx.local, weapon, no_spread);
        if (hit_chance >= required_chance)
        {
            math::clamp_angles(best_angle);
            std::cout << "Aiming with sufficient hitchance: " << hit_chance << "%\n";

            if (config.ragebot.silent_aim)
            {
                for (int i = 0; i < user_cmd->pb.input_history_size(); i++) {
                    auto tick = user_cmd->pb.mutable_input_history(i);
                    if (tick) {
                        tick->mutable_view_angles()->set_x(best_angle.x);
                        tick->mutable_view_angles()->set_y(best_angle.y);
                        tick->mutable_view_angles()->set_z(best_angle.z);
                    }
                }
            }
            else
            {
                interfaces::csgo_input->set_view_angles(best_angle);
            }

            if (config.ragebot.automatic_fire)
                user_cmd->buttons.button_state |= IN_ATTACK;
            config.movement.m_autopeek.did_shoot = true;
        }
        else
        {
            std::cout << "Hitchance too low: " << hit_chance << "%\n";
        }
    }
}


inline void store_hitboxes() {
    if (config.ragebot.m_hitboxes[0])
        g_ragebot->m_hitboxes.emplace_back(hitbox_head);

    if (config.ragebot.m_hitboxes[1]) {
        g_ragebot->m_hitboxes.emplace_back(hitbox_chest);
        g_ragebot->m_hitboxes.emplace_back(hitbox_lower_chest);
        g_ragebot->m_hitboxes.emplace_back(hitbox_upper_chest);
    }

    if (config.ragebot.m_hitboxes[2]) {
        g_ragebot->m_hitboxes.emplace_back(hitbox_pelvis);
        g_ragebot->m_hitboxes.emplace_back(hitbox_stomach);
    }

    if (config.ragebot.m_hitboxes[3]) {
        g_ragebot->m_hitboxes.emplace_back(hitbox_right_hand);
        g_ragebot->m_hitboxes.emplace_back(hitbox_left_hand);
        g_ragebot->m_hitboxes.emplace_back(hitbox_right_upper_arm);
        g_ragebot->m_hitboxes.emplace_back(hitbox_right_forearm);
        g_ragebot->m_hitboxes.emplace_back(hitbox_left_upper_arm);
        g_ragebot->m_hitboxes.emplace_back(hitbox_left_forearm);
    }

    if (config.ragebot.m_hitboxes[4]) {
        g_ragebot->m_hitboxes.emplace_back(hitbox_right_thigh);
        g_ragebot->m_hitboxes.emplace_back(hitbox_left_thigh);

        g_ragebot->m_hitboxes.emplace_back(hitbox_right_calf);
        g_ragebot->m_hitboxes.emplace_back(hitbox_left_calf);
    }

    if (config.ragebot.m_hitboxes[5]) {
        g_ragebot->m_hitboxes.emplace_back(hitbox_right_foot);
        g_ragebot->m_hitboxes.emplace_back(hitbox_left_foot);
    }
}

lag_record_t* c_ragebot::select_record(int handle) {
    if (!g_ctx.local->is_alive())
        return nullptr;

    auto& records = m_lag_records[handle];

    if (records.empty())
        return nullptr;

    lag_record_t* best_record = nullptr;
    lag_record_t* last_record = &records.front();

    if (records.size() == 1u)
        return last_record;

    for (auto i = records.begin(); i != records.end(); i = std::next(i)) {
        auto record = &*i;

        if (!record->m_pawn || !record->is_valid())
            continue;

        if (!record || !record->is_valid())
            continue;

        if (!best_record) {
            best_record = record;

            continue;
        }

        if (record->m_throwing) {
            best_record = record;

            continue;
        }

        if ((record->m_pawn->flags() & FL_ONGROUND) != (record->m_pawn->flags() & FL_ONGROUND)) {
            if ((record->m_pawn->flags() & FL_ONGROUND))
                best_record = record;
        }
    }

    return best_record;
}
void c_ragebot::find_targets() {
    if (!g_ctx.local->is_alive()) {
        debug_text(WHITE_COLOR, "[ragebot] Local player not alive, skipping target search\n");
        return;
    }

    m_aim_targets.clear();  // обязательно очищаем старые цели

    auto entities = entity::get("CCSPlayerController");
    debug_text(WHITE_COLOR, "[ragebot] Searching targets... Found %d player controllers\n", entities.size());

    int added = 0;
    int skipped_self = 0;
    int skipped_null = 0;
    int skipped_norecord = 0;

    for (auto entity : entities) {
        if (!entity) {
            skipped_null++;
            continue;
        }

        auto player_controller = reinterpret_cast<c_cs_player_controller*>(entity);
        if (!player_controller) {
            skipped_null++;
            continue;
        }

        if (player_controller == g_ctx.local_controller) {
            skipped_self++;
            continue;
        }

        int handle = player_controller->get_ref_ehandle().to_int();
        lag_record_t* best_record = this->select_record(handle);

        if (!best_record) {
            skipped_norecord++;
            continue;
        }

        m_aim_targets[handle] = std::make_shared<aim_target_t>(best_record);
        added++;

        debug_text(WHITE_COLOR, "[ragebot] Added target: handle = %d\n", handle);
    }

    debug_text(WHITE_COLOR, "[ragebot] Target search complete: %d added, %d skipped (null: %d, self: %d, no_record: %d)\n",
        added, skipped_null + skipped_self + skipped_norecord, skipped_null, skipped_self, skipped_norecord);
}


void c_ragebot::store_records(int stage) {
    if (!config.ragebot.enable)
        return;
    if (!interfaces::engine_client->is_in_game()) {
        m_lag_records.clear();
        return;
    }

    if (stage != FRAME_RENDER_END || !g_ctx.local)
        return;
    static auto sv_maxunlag = /*interfaces::cvar->get_by_name(("sv_maxunlag"))->get_float()*/ 0.2;
    const int max_ticks = TIME_TO_TICKS(sv_maxunlag);

    const auto& entity_list = entity::get("CCSPlayerController");
    for (int i = 0; i < entity_list.size(); ++i) {

        auto entity = entity_list[i];

        auto player_controller = reinterpret_cast<c_cs_player_controller*>(entity);
        if (!player_controller || player_controller == g_ctx.local_controller)
            continue;

        auto handle = player_controller->get_ref_ehandle().to_int();

        if (!player_controller->pawn_is_alive()) {
            auto player_iterator = m_lag_records.find(handle);
            if (player_iterator != m_lag_records.end())
                m_lag_records.erase(player_iterator);

            continue;
        }

        auto player_pawn = reinterpret_cast<c_cs_player_pawn*>(interfaces::entity_system->get_base_entity(player_controller->pawn().get_entry_index()));
        if (!player_pawn) {
            auto player_iterator = m_lag_records.find(handle);
            if (player_iterator != m_lag_records.end())
                m_lag_records.erase(player_iterator);

            continue;
        }

        if (player_pawn->team_num() == g_ctx.local->team_num())
            continue;

        if (player_pawn == g_ctx.local)
            continue;

        if (m_lag_records.find(handle) == m_lag_records.end()) {
            m_lag_records.insert_or_assign(handle, std::deque<lag_record_t>{});

            continue;
        }

        auto& records = m_lag_records[handle];

        if (records.size() != max_ticks) {
            records.clear();
            records.resize(max_ticks);
        }

        auto& record = records.emplace_front(player_pawn);
        if (records.size() > max_ticks - 1)
            records.erase(records.end() - 1);
    }
}


struct aim_result_t {
    aim_point_t point;
    float damage;
};
inline std::vector<vec3_t> calculate_sphere_points(float radius, int num_points)
{
    std::vector<vec3_t> points;
    points.reserve(num_points);

    const float phi = 3.141592654f * (3.0f - std::sqrt(5.0f));

    for (int i = 0; i < num_points; ++i)
    {
        float y = 1 - (i / float(num_points - 1)) * 2;
        float radius_at_y = std::sqrt(1 - y * y);

        float theta = phi * i;

        float x = std::cos(theta) * radius_at_y;
        float z = std::sin(theta) * radius_at_y;

        vec3_t vec_point{};
        points.push_back({ x * radius, y * radius, z * radius });
    }

    return points;
}
inline std::vector<vec3_t> calculate_points(int num_points, float radius, float height, matrix3x4_t matrix, hitbox_data_t hitbox)
{
    std::vector<vec3_t> points;
    points.reserve(num_points);

    for (int i = 0; i < num_points; ++i)
    {
        float theta = 2.f * 3.141592654f * i / num_points;
        float y = radius * std::cos(theta);
        float z = radius * std::sin(theta);

        vec3_t in_point = { hitbox.m_mins.x + hitbox.m_radius * height, y, z };

        vec3_t vec_point{};
        vector_transform(in_point, matrix, vec_point);
        points.push_back(vec_point);
    }

    return points;
}
bool c_ragebot::multi_points(lag_record_t* record, int hitbox, std::vector<aim_point_t>& points) {
    if (!record)
        return false;

    c_cs_player_pawn* pawn = record->m_pawn;
    if (!pawn || !pawn->is_alive() || !pawn->is_player_pawn())
        return false;

    hitbox_data_t hitbox_data = get_hitbox_data(pawn, hitbox);
    if (hitbox_data.m_invalid_data)
        return false;

    auto local_data = g_prediction->get_local_data();
    if (!local_data)
        return false;

    vec3_t center = (hitbox_data.m_mins + hitbox_data.m_maxs) * 0.5f;
    matrix3x4_t matrix = math::transform_to_matrix(record->m_bone_data[hitbox_data.m_num_bone]);
    vec3_t point = transform_point(matrix, center);

    int hitbox_from_menu = get_hitbox_from_menu(hitbox);
    if (hitbox_from_menu == -1)
        return false;

    points.emplace_back(point, hitbox);

    {
        std::lock_guard<std::mutex> lock(multipoint_mutex);
        multipoint_debug_positions.emplace_back(point, "");
    }

    if (!config.ragebot.m_multi_points[hitbox_from_menu])
        return true;

    float scale = config.ragebot.multipointscale;
    float radius = hitbox_data.m_radius * (scale / 100.f);
    if (radius <= 0.f)
        return false;

    // optional: отрисовать радиус только на голове
    if (hitbox == hitbox_head) {
        std::lock_guard<std::mutex> lock(multipoint_mutex);
        multipoint_debug_positions.emplace_back(point, "Radius: " + std::to_string(static_cast<int>(radius)));
        radius_head = radius;

    }

    // HEAD / NECK
    if (hitbox == hitbox_head || hitbox == hitbox_neck) {
        auto sphere_points = calculate_sphere_points(radius, 15);
        if (sphere_points.empty()) return true;

        for (const auto& offset : sphere_points) {
            auto point_pos = transform_point(matrix, {
                hitbox_data.m_maxs.x + offset.x,
                hitbox_data.m_maxs.y + offset.y,
                hitbox_data.m_maxs.z + offset.z
                });

            points.emplace_back(point_pos, hitbox);
            std::lock_guard<std::mutex> lock(multipoint_mutex);
            multipoint_debug_positions.emplace_back(point_pos, "");
        }

        return true;
    }

    // LIMBS
    if (hitbox > hitbox_upper_chest) {
        auto limb_points = calculate_sphere_points(radius, 3);
        if (limb_points.empty()) return true;

        for (const auto& offset : limb_points) {
            auto point_pos = transform_point(matrix, {
                hitbox_data.m_maxs.x + offset.x,
                hitbox_data.m_maxs.y + offset.y,
                hitbox_data.m_maxs.z + offset.z
                });

            points.emplace_back(point_pos, hitbox);
            std::lock_guard<std::mutex> lock(multipoint_mutex);
            multipoint_debug_positions.emplace_back(point_pos, "");
        }

        return true;
    }

    // TORSO
    auto capsule_points = calculate_points(4, radius, 0.5f, matrix, hitbox_data);
    if (capsule_points.empty()) return true;

    for (const auto& p : capsule_points) {
        points.emplace_back(p, hitbox);
        std::lock_guard<std::mutex> lock(multipoint_mutex);
        multipoint_debug_positions.emplace_back(p, "");
    }

    return true;
}



/*bool c_ragebot::multi_points(lag_record_t* record, int hitbox, std::vector<aim_point_t>& points) {
    if (!record)
        return false;

    c_cs_player_pawn* pawn = record->m_pawn;

    if (!pawn || !pawn->is_alive() || !pawn->is_player_pawn())
        return false;

    hitbox_data_t hitbox_data = get_hitbox_data(pawn, hitbox);

    if (hitbox_data.m_invalid_data)
        return false;

    auto local_data = g_prediction->get_local_data();

    if (!local_data)
        return false;

    vec3_t center = (hitbox_data.m_mins + hitbox_data.m_maxs) * 0.5f;
    matrix3x4_t matrix = math::transform_to_matrix(record->m_bone_data[hitbox_data.m_num_bone]);
    vec3_t point = transform_point(matrix, center);

    int hitbox_from_menu = get_hitbox_from_menu(hitbox);
    if (hitbox_from_menu == -1)
        return false;

    points.emplace_back(point, hitbox);

    // Рисуем круг на экране с помощью ImGui
    {
        vec3_t screen;
        if (g_render->world_to_screen(point, screen))
        {
            // Используем ImGui для отрисовки круга
            ImGui::GetBackgroundDrawList()->AddCircleFilled(
                ImVec2(screen.x, screen.y), 4.f, IM_COL32(255, 255, 255, 255));
        }
    }

    if (!config.ragebot.m_multi_points[hitbox_from_menu])
        return true;

    float scale = config.ragebot.multipointscale;
    float radius = hitbox_data.m_radius * (scale / 100.f);

    if (radius <= 0.f)
        return false;

    if (hitbox == hitbox_upper_chest)
        return false;

    if (hitbox > hitbox_upper_chest) {
        auto limb_points = calculate_sphere_points(radius, 3);
        if (limb_points.empty())
            return false;

        for (const auto& point_offset : limb_points) {
            auto point_pos = transform_point(matrix, {
                hitbox_data.m_maxs.x + point_offset.x,
                hitbox_data.m_maxs.y + point_offset.y,
                hitbox_data.m_maxs.z + point_offset.z
                });

            points.emplace_back(point_pos, hitbox);

            vec3_t screen;
            if (g_render->world_to_screen(point_pos, screen))
            {
                // Используем ImGui для отрисовки круга
                ImGui::GetBackgroundDrawList()->AddCircleFilled(
                    ImVec2(screen.x, screen.y), 4.f, IM_COL32(255, 255, 255, 255));
            }
        }

        return true;
    }

    if (hitbox == hitbox_head || hitbox == hitbox_neck) {
        auto sphere_points = calculate_sphere_points(radius, 15);
        if (sphere_points.empty())
            return false;

        for (const auto& point_offset : sphere_points) {
            auto point_pos = transform_point(matrix, {
                hitbox_data.m_maxs.x + point_offset.x,
                hitbox_data.m_maxs.y + point_offset.y,
                hitbox_data.m_maxs.z + point_offset.z
                });

            points.emplace_back(point_pos, hitbox);

            vec3_t screen;
            if (g_render->world_to_screen(point_pos, screen))
            {
                // Используем ImGui для отрисовки круга
                ImGui::GetBackgroundDrawList()->AddCircleFilled(
                    ImVec2(screen.x, screen.y), 4.f, IM_COL32(255, 255, 255, 255));
            }
        }

        return true;
    }

    auto capsule_points = calculate_points(4, radius, 0.5f, matrix, hitbox_data);
    if (capsule_points.empty())
        return false;

    for (const auto& p : capsule_points) {
        points.emplace_back(p, hitbox);
    }

    return true;
}*/
aim_point_t c_ragebot::select_points(lag_record_t* record, float& damage) {
    std::vector<std::future<std::optional<aim_result_t>>> futures;
    std::atomic<bool> lethal_found{ false };
    std::atomic<float> best_damage{ 0.0f };
    std::mutex lethal_mutex;
    aim_result_t best_result{ {vec3_t(0.f, 0.f, 0.f), -1}, 0.f };

    if (!g_ctx.local || !g_ctx.local->is_alive())
        return best_result.point;
    if (!record->m_pawn || !record->m_pawn->is_alive())
        return best_result.point;

    c_base_player_weapon* weapon = g_ctx.local->get_active_weapon();
    c_cs_weapon_base_v_data* weapon_data = weapon->get_weapon_data();

    if (!weapon_data)
        return best_result.point;

    auto local_ent_base = interfaces::entity_system->get_base_entity(g_ctx.local->get_ref_ehandle().get_entry_index());
    c_cs_player_pawn* pawn_ent = interfaces::entity_system->get_base_entity<c_cs_player_pawn>(record->m_pawn->get_ref_ehandle().get_entry_index());

    auto local_data = g_prediction->get_local_data();
    if (!local_data)
        return best_result.point;

    vec3_t shoot_pos = local_data->m_eye_pos;

    for (auto& hitbox : m_hitboxes) {
        futures.push_back(std::async(std::launch::async, [&]() -> std::optional<aim_result_t> {
            if (lethal_found.load(std::memory_order_relaxed))
                return std::nullopt;
            std::vector<aim_point_t> points;
            //if (!multipoints(record->m_pawn, record->m_bone_data/*->m_game_scene_node()->get_skeleton_instance()->m_model_state().bones*/, hitbox, points, 0)) // 0 prediction == false
            //	return std::nullopt;
            if (!multi_points(record, hitbox, points))
                return std::nullopt;

            aim_result_t local_best{ {vec3_t(0.f, 0.f, 0.f), -1}, 0.f };
            for (const auto& point : points) {
                if (lethal_found.load(std::memory_order_relaxed))
                    return std::nullopt;

                float point_damage = g_auto_wall->penetration_damage(shoot_pos, point.m_point, local_ent_base, g_ctx.local, record->m_pawn, pawn_ent, record->m_pawn->item_services()->has_heavy_armor(), record->m_pawn->team_num(), weapon_data);
                if (point_damage == 0)
                    continue;

                if (point_damage > best_damage.load(std::memory_order_relaxed)) {
                    local_best = { point, point_damage };
                    best_damage.store(point_damage, std::memory_order_relaxed);
                }

                if (point_damage >= record->m_pawn->health()) {
                    std::lock_guard<std::mutex> lock(lethal_mutex);
                    if (!lethal_found.load(std::memory_order_relaxed)) {
                        lethal_found.store(true, std::memory_order_release);
                        return local_best;
                    }
                }
            }
            return local_best.damage > 0 ? std::optional<aim_result_t>(local_best) : std::nullopt;
            }));
    }

    for (auto& future : futures) {
        if (auto result = future.get()) {
            if (result->damage > best_result.damage) {
                best_result = *result;
            }
        }
    }

    damage = best_result.damage;
    return best_result.point;
}
std::shared_ptr<aim_target_t> c_ragebot::get_nearest_target() {
    if (!g_ctx.local->is_alive()) {
        debug_text(WHITE_COLOR, "[ragebot] Local player not alive\n");
        return nullptr;
    }

    auto local_data = g_prediction->get_local_data();
    if (!local_data) {
        debug_text(WHITE_COLOR, "[ragebot] No local prediction data\n");
        return nullptr;
    }

    vec3_t shoot_pos = local_data->m_eye_pos;
    vec2_t screen_center(ImGui::GetIO().DisplaySize.x / 2.f, ImGui::GetIO().DisplaySize.y / 2.f);

    std::shared_ptr<aim_target_t> best_target = nullptr;
    float best_screen_distance = FLT_MAX;
    float best_distance = FLT_MAX;
    int best_damage = 0;

    for (auto it = m_aim_targets.begin(); it != m_aim_targets.end(); it = std::next(it)) {
        auto target = &it->second;
        if (!target || !*target) continue;

        auto t = target->get();

        if (!t->m_lag_record || !t->m_lag_record->m_pawn || !t->m_pawn)
            continue;

        if (!t->m_pawn->is_alive() || !t->m_lag_record->m_pawn->is_alive())
            continue;

        auto node = t->m_lag_record->m_pawn->game_scene_node();
        if (!node) continue;

        vec3_t target_pos{};
        try {
            target_pos = node->vec_origin();
        }
        catch (...) {
            continue;
        }

        vec3_t screen3d;
        if (!g_render->world_to_screen(target_pos, screen3d))
            continue;

        float distance = target_pos.dist(shoot_pos);
        float screen_distance = vec2_t(screen3d.x, screen3d.y).dist_to(screen_center);

        // Подсчёт потенциального урона
        float damage_float = 0.0f;
        aim_point_t dmg_point = select_points(t->m_lag_record, damage_float);
        int potential_damage = static_cast<int>(damage_float + 0.5f); // округление


        if (config.ragebot.force_shoot_style == 10) {
            // Классическая логика: по screen_distance и distance
            if (screen_distance < best_screen_distance || distance < best_distance) {
                best_screen_distance = screen_distance;
                best_distance = distance;
                best_target = *target;
            }
        }
        else {
            // По максимальному урону
            if (potential_damage > best_damage) {
                best_damage = potential_damage;
                best_target = *target;
            }
        }
    }

    if (!best_target)
    {
        debug_text(WHITE_COLOR, "[ragebot] No valid target found\n");
    }
    else
        debug_text(WHITE_COLOR, "[ragebot] Target selected\n");

    return best_target;
}

void c_ragebot::select_target() {
    if (!g_ctx.local->is_alive()) {
        debug_text(WHITE_COLOR, "[ragebot] Local player not alive (select_target)\n");
        return;
    }

    if (m_aim_targets.empty()) {
        debug_text(WHITE_COLOR, "[ragebot] No aim targets\n");
        return;
    }

    std::shared_ptr<aim_target_t> best_target = this->get_nearest_target();
    if (!best_target) {
        debug_text(WHITE_COLOR, "[ragebot] get_nearest_target returned nullptr\n");
        return;
    }

    if (!best_target->m_lag_record) {
        debug_text(WHITE_COLOR, "[ragebot] Best target has no lag record\n");
        return;
    }

    if (!best_target->m_pawn || !best_target->m_pawn->is_alive()) {
        debug_text(WHITE_COLOR, "[ragebot] Best target pawn is dead or null\n");
        return;
    }

    if (best_target->m_pawn->gungame_immunity()) {
        debug_text(WHITE_COLOR, "[ragebot] Best target has gungame immunity\n");
        return;
    }

    bool is_taser = false;
    auto weapon = g_ctx.local->get_active_weapon();
    if (weapon && weapon->get_weapon_data())
        is_taser = weapon->get_weapon_data()->weapon_type() == WEAPONTYPE_TASER;

    int min_damage = config.ragebot.min_damage;
    if (is_taser)
        min_damage = best_target->m_lag_record->m_pawn->health();
    if (g_key_handler->is_pressed(config.ragebot.min_damage_keybind, config.ragebot.min_damage_keybind_style)) {
        min_damage = config.ragebot.min_damage_override;
    }
    if (min_damage == 0)
    {
        min_damage = best_target->m_lag_record->m_pawn->health();
    }
    debug_text(WHITE_COLOR, "[ragebot] Applying lag record...\n");
    best_target->m_lag_record->apply(best_target->m_pawn);

    aim_point_t best_point = this->select_points(best_target->m_lag_record, best_damage);
    best_target->m_lag_record->reset(best_target->m_pawn);

    debug_text(WHITE_COLOR, "[ragebot] Point selected: hitbox %d, damage %d (min %d)\n",
        best_point.m_hitbox, best_damage, min_damage);

    if (best_point.m_hitbox == -1 || best_damage < min_damage) {
        debug_text(WHITE_COLOR, "[ragebot] Rejected best point — no valid hitbox or low damage\n");
        return;
    }

    best_target->m_best_point = std::make_shared<aim_point_t>(best_point);
    m_best_target = std::move(best_target);

    debug_text(WHITE_COLOR, "[ragebot] Best target selected and stored\n");
}
inline vec3_t get_removed_aim_punch_angles(c_cs_player_pawn* local_player) {
    using fnUpdateAnimPunch = void(__fastcall*)(void*, vec3_t*, float, bool);
    static fnUpdateAnimPunch fn = reinterpret_cast<fnUpdateAnimPunch>(utils::find_pattern_rel(g_modules.client, XOR_STR("E8 ? ? ? ? 48 8D 4E ? 48 8D 54 24 ? E8 ? ? ? ? F2 0F 10 4C 24"), 0x1));

    vec3_t aim_punch = {};
    fn(local_player, &aim_punch, 0.0f, true);
    return aim_punch;
}
void process_backtrack(lag_record_t* record, c_user_cmd* user_cmd) {
    if (!record)
        return;

    const int tick_shoot = TIME_TO_TICKS(record->m_simulation_time) + 2;

    for (int i = 0; i < user_cmd->pb.input_history_size(); i++) {

        if (user_cmd->pb.attack1_start_history_index() == -1)
            continue;

        auto input_history = user_cmd->pb.mutable_input_history(i);
        if (!input_history)
            continue;

        if (input_history->has_cl_interp()) {
            auto interp_info = input_history->mutable_cl_interp();
            interp_info->set_frac(0.f);
        }


        if (input_history->has_sv_interp0()) {
            auto interp_info = input_history->mutable_sv_interp0();
            interp_info->set_src_tick(tick_shoot);
            interp_info->set_dst_tick(tick_shoot);
            interp_info->set_frac(0.f);
        }

        if (input_history->has_sv_interp1()) {
            auto interp_info = input_history->mutable_sv_interp1();
            interp_info->set_src_tick(tick_shoot);
            interp_info->set_dst_tick(tick_shoot);
            interp_info->set_frac(0.f);
        }

        input_history->set_render_tick_count(tick_shoot);

        user_cmd->pb.mutable_base()->set_client_tick(tick_shoot);

        input_history->set_player_tick_count(g_ctx.local_controller->tick_base());
        input_history->set_player_tick_fraction(g_prediction->get_local_data()->m_player_tick_fraction);
    }
}
void attack_with_backtrack(c_user_cmd* user_cmd, vec3_t angle) {
    for (int i = 0; i < user_cmd->pb.input_history_size(); i++) {
        auto container = user_cmd->pb.mutable_input_history(i);
        if (container) {
            container->set_player_tick_count(g_prediction->get_local_data()->m_shoot_tick);
        }
    }

    user_cmd->pb.set_attack3_start_history_index(0);
    user_cmd->pb.set_attack1_start_history_index(0);
    process_backtrack(g_ragebot->m_best_target->m_lag_record, user_cmd);

        user_cmd->buttons.button_state |= IN_ATTACK;
}
void revolver_cock(c_user_cmd* cmd) {
    if (!g_ctx.local->get_active_weapon())
        return;

    auto weapon = g_ctx.local->get_active_weapon();
    if (!weapon)
        return;

    auto attribute = weapon->attribute_manager();
    if (!attribute)
        return;

    auto item = attribute->item();
    if (!item)
        return;

    if (item->item_definition_index() != 64)
        return;

    if (bool valid_revolver = !(cmd->buttons.button_state & IN_RELOAD) && !(cmd->buttons.button_state & IN_ATTACK) && weapon->i_clip1(); !valid_revolver)
        return;

    const auto server_time = g_prediction->get_local_data()->m_tick_base;

    if (weapon->n_postpone_fire_ready_ticks() > server_time) {
        if (server_time > g_ragebot->next_cock_time && g_ctx.local->get_active_weapon()->n_next_primary_attack_tick() <= server_time) {
            cmd->buttons.button_state |= IN_ATTACK;
        }
    }
    else {
        //g_ragebot->next_cock_time = server_time + 0.2365f;
        g_ragebot->next_cock_time = server_time + 0.3;

    }
}

void c_ragebot::run(c_csgo_input* pInput, c_user_cmd* pCmd)
{
    if (!config.ragebot.enable)
        return;
    //debug_text(WHITE_COLOR, "[ragebot] Enabled\n");

    if (!interfaces::engine_client->is_in_game() || !interfaces::engine_client->is_connected())
        return;
    //debug_text(WHITE_COLOR, "[ragebot] Player is not in game or not connected\n");

    c_user_cmd* user_cmd = pCmd;
    //debug_text(WHITE_COLOR, "[ragebot] user_cmd is valid\n");

    c_cs_player_pawn* local_player = g_ctx.local;
    if (!local_player || !local_player->is_alive())
        return;
    //debug_text(WHITE_COLOR, "[ragebot] Local player is alive\n");

    config.movement.m_autopeek.did_shoot = false;

    if (!m_hitboxes.empty()) {
        debug_text(WHITE_COLOR, "[ragebot] Clearing previous hitboxes\n");
        m_hitboxes.clear();
    }

    vec3_t qatovec{};

    c_base_player_weapon* active_weapon = local_player->get_active_weapon();
    if (!active_weapon) {
        debug_text(WHITE_COLOR, "[ragebot] Active weapon not found\n");
        return;
    }

    c_cs_weapon_base_v_data* weapon_data = active_weapon->get_weapon_data();
    if (!weapon_data) {
        debug_text(WHITE_COLOR, "[ragebot] Weapon data missing\n");
        return;
    }

    bool is_taser = weapon_data->weapon_type() == WEAPONTYPE_TASER;
    bool b_no_spread = /*interfaces::cvar->get_by_name("weapon_accuracy_nospread")->get_bool()*/ false;

    try {
        if (m_best_target)
            m_best_target->reset();

        if (weapon_data->weapon_type() == WEAPONTYPE_KNIFE || weapon_data->weapon_type() == WEAPONTYPE_GRENADE) {
            //debug_text(WHITE_COLOR, "[ragebot] Skipping knife or grenade\n");
            return;
        }

        //debug_text(WHITE_COLOR, "[ragebot] Storing hitboxes\n");
        store_hitboxes();

        if (m_hitboxes.empty()) {
            //debug_text(WHITE_COLOR, "[ragebot] No hitboxes found\n");
            return;
        }

        //debug_text(WHITE_COLOR, "[ragebot] Finding targets...\n");
        find_targets();

        //debug_text(WHITE_COLOR, "[ragebot] Selecting best target\n");
        select_target();

        if (!m_best_target || !m_best_target->m_best_point) {
            debug_text(WHITE_COLOR, "[ragebot] No best target found\n");
            return;
        }

        auto local_data = g_prediction->get_local_data();
        if (!local_data) {
            debug_text(WHITE_COLOR, "[ragebot] Local prediction data missing\n");
            return;
        }
        if (!can_shoot(g_ctx.local, active_weapon)) {
            debug_text(WHITE_COLOR, "[ragebot] Cannot shoot at this moment\n");
            return;
        }
        revolver_cock(user_cmd);

        if ((weapon_data->weapon_type() == WEAPONTYPE_SNIPER_RIFLE)
            && !(user_cmd->buttons.button_state & IN_ZOOM)
            && !local_player->scoped()
            && !b_no_spread) {
            debug_text(WHITE_COLOR, "[ragebot] Forcing zoom for sniper\n");
            user_cmd->buttons.button_state |= IN_ATTACK2;
        }

        if ((uintptr_t)m_best_target->m_best_point.get() == 0xdddddddddddddddd) {
            debug_text(WHITE_COLOR, "[ragebot] Invalid best point address\n");
            return;
        }

        //debug_text(WHITE_COLOR, "[ragebot] Applying lag record\n");
        m_best_target->m_lag_record->apply(m_best_target->m_pawn);
        //debug_text(WHITE_COLOR, "[ragebot] Appled lag record\n");

        vec3_t angle = math::calculate_angles(local_data->m_eye_pos, m_best_target->m_best_point->m_point);
        vec3_t aim_punch = get_removed_aim_punch_angles(local_player);

        qatovec = { angle.x, angle.y, angle.z };
        //debug_text(WHITE_COLOR, "[ragebot] Calculated angles: %.2f %.2f %.2f\n", qatovec.x, qatovec.y, qatovec.z);
        if (g_ctx.local->flags() & FL_ONGROUND) {
            auto_stop(user_cmd, g_ctx.local, active_weapon, b_no_spread);
        }
        else if (!(g_ctx.local->flags() & FL_ONGROUND)) {
            auto_stop_air(user_cmd, g_ctx.local, active_weapon, b_no_spread);
        }

        qatovec.x -= aim_punch.x;
        qatovec.y -= aim_punch.y;
        qatovec.z -= aim_punch.z;

    }
    catch (...) {
        //std::cout << "[ERROR] ragebot exception" << std::endl;
        //debug_text(RED_COLOR, "[ragebot] Exception caught in main logic\n");
        return;
    }
    static int target_hc = config.ragebot.hit_chance;
    //debug_text(WHITE_COLOR, "[ragebot] Calculating hit chance...\n");
    int hit_chance = calculate_hit_chance(m_best_target->m_pawn, qatovec, active_weapon, weapon_data, b_no_spread);
    //debug_text(WHITE_COLOR, "[ragebot] Hit chance = %d\n", hit_chance);
    if (g_key_handler->is_pressed(config.ragebot.hit_chance_keybind, config.ragebot.hit_chance_keybind_style)) {
        target_hc = config.ragebot.hit_chance_override;
    }
	debug_text(WHITE_COLOR, "[ragebot] Target hit chance: %d (need: %d)\n", hit_chance, target_hc);
    if (hit_chance >= (is_taser ? 70 : target_hc)) {
        //debug_text(WHITE_COLOR, "[ragebot] Hit chance met, shooting...\n");

        if (config.ragebot.silent_aim) {
            //debug_text(WHITE_COLOR, "[ragebot] Applying silent aim\n");
            for (int i = 0; i < user_cmd->pb.input_history_size(); i++) {
                auto tick = user_cmd->pb.mutable_input_history(i);
                if (tick) {
                    tick->mutable_view_angles()->set_x(qatovec.x);
                    tick->mutable_view_angles()->set_y(qatovec.y);
                    tick->mutable_view_angles()->set_z(qatovec.z);
                }
            }
        }
        else {
            //debug_text(WHITE_COLOR, "[ragebot] Setting view angles normally\n");
            interfaces::csgo_input->set_view_angles(qatovec);
        }
        if (config.ragebot.automatic_fire)
        {
            //process_attack(user_cmd);
			attack_with_backtrack(user_cmd, qatovec);
        }
        //debug_text(WHITE_COLOR, "[ragebot] Attack processed\n");
    }
    else {
        //debug_text(WHITE_COLOR, "[ragebot] Hit chance too low, aborting attack\n");
    }

    config.movement.m_autopeek.did_shoot = true;

    if (m_best_target && m_best_target->m_lag_record)
        m_best_target->m_lag_record->reset(m_best_target->m_pawn);
    //debug_text(WHITE_COLOR, "[ragebot] Lag record reset\n");
}
