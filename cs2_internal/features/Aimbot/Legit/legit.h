#pragma once
#include "../../config_system.h"
#include "../../../includes.h"
#include "../../../sdk/utils/math.h"
#include "../../engine prediction/eprediction.hpp"
constexpr float M_PI = 3.14159265358979323846f;
constexpr float M_PI_F = static_cast<float>(M_PI);
constexpr float RAD2DEG = 180.f / M_PI_F;
constexpr float DEG2RAD = M_PI_F / 180.f;
static struct legit_settings {
    float fov;
    float smooth;
    float delay;
    float humanize_float;
    float rcs_smooth;
    float rcs_fov;
    int rcs_after_shots;
    int triggerbot_delay_before_shoot;
    int triggerbot_delay_betwenn_shots;
    int triggerbot_hitchance;

    // Добавленные поля
    bool enable_rcs;
    int min_damage;
    int min_damage_keybind;
    int min_damage_keybind_style;
    int min_damage_override;
};
class c_skeleton_instace_legit
{
public:
    std::byte pad_003[0x1CC]; //0x0000
public:
    int m_bone_count; //0x01CC
private:
    std::byte pad_002[0x18]; //0x01D0
public:
    int m_mask; //0x01E8
private:
    std::byte pad_001[0x4]; //0x01EC
public:
    matrix2x4_t* m_bone_cache; //0x01F0
    SCHEMA(m_model_state, c_model_state, ("CSkeletonInstance"), ("m_modelState"));

};
struct hitbox_data_t_legit
{
    hitbox_data_t_legit() = default;

    int m_num_hitbox;
    int m_num_bone;
    const char* m_name_bone;
    int m_num_hitgroup;
    c_hitbox* m_hitbox_data;
    float m_radius;
    matrix3x4_t m_matrix;
    vec3_t m_mins;
    vec3_t m_maxs;
    bool m_invalid_data{ true };
};
class c_legitbot {
public:
	void legitbot(c_user_cmd* user_cmd);
	void triggerbot(c_user_cmd* user_cmd, c_cs_player_pawn* best_target, legit_settings sets, int hit_chance, vec3_t local_eye_pos, vec3_t best_head, c_base_player_weapon* weapon);
	void legit(c_user_cmd* user_cmd);
	void draw_fov_circle();
	c_cs_player_pawn* player;
	vec3_t smooth_angle;
    std::vector<int> m_hitboxes_legit;
    hitbox_data_t_legit get_hitbox_data_legit(c_cs_player_pawn* player_pawn, const int hitbox_id) {
        if (!player_pawn)
            return hitbox_data_t_legit();

        auto game_scene = player_pawn->game_scene_node();
        if (!game_scene)
            return hitbox_data_t_legit();

        auto skeleton = game_scene->get_skeleton_instance_legit();
        if (!skeleton)
            return hitbox_data_t_legit();

        auto& model_state = skeleton->m_model_state();
        auto& model = model_state.modelHandle;
        if (!model)
            return hitbox_data_t_legit();

        auto hitbox_data = model->get_hitbox(hitbox_id);
        if (!hitbox_data)
            return hitbox_data_t_legit();

        const auto bone_index = player_pawn->get_bone_index(hitbox_data->m_bone_name);
        const auto bones = model_state.bones;

        hitbox_data_t_legit hitbox;
        hitbox.m_num_hitbox = hitbox_id;
        hitbox.m_num_bone = bone_index;
        hitbox.m_hitbox_data = hitbox_data;
        hitbox.m_radius = hitbox_data->m_shape_radius;
        hitbox.m_num_hitgroup = hitbox_data->m_hitgroup;
        hitbox.m_matrix = math::transform_to_matrix(bones[bone_index]);
        hitbox.m_mins = hitbox_data->m_vec_min;
        hitbox.m_maxs = hitbox_data->m_vec_maxs;
        hitbox.m_name_bone = hitbox_data->m_name;
        hitbox.m_invalid_data = false;

        return hitbox;
    }
};

enum e_hitboxes_legit : std::uint32_t
{
    /* head */
    legit_hitbox_head,
    legit_hitbox_neck,
    /* body */
    legit_hitbox_pelvis,
    legit_hitbox_stomach,
    legit_hitbox_chest,
    legit_hitbox_lower_chest,
    legit_hitbox_upper_chest,
    /* legs */
    legit_hitbox_right_thigh,
    legit_hitbox_left_thigh,
    legit_hitbox_right_calf,
    legit_hitbox_left_calf,
    legit_hitbox_right_foot,
    legit_hitbox_left_foot,
    /* arms */
    legit_hitbox_right_hand,
    legit_hitbox_left_hand,
    legit_hitbox_right_upper_arm,
    legit_hitbox_right_forearm,
    legit_hitbox_left_upper_arm,
    legit_hitbox_left_forearm,
    legit_hitbox_max
};
struct aim_point_t_legit {
    aim_point_t_legit(vec3_t point, int hitbox, bool center = false) {
        this->m_point = point;
        this->m_hitbox = hitbox;
        this->m_center = center;
    }

    vec3_t m_point{};
    int m_hitbox{};
    bool m_center{};
};
inline int get_hitbox_from_menu_legit(int hitbox) {
    switch (hitbox) {
    case legit_hitbox_head:
        return 0;
    case legit_hitbox_chest:
    case legit_hitbox_lower_chest:
    case legit_hitbox_upper_chest:
        return 1;
    case legit_hitbox_pelvis:
    case legit_hitbox_stomach:
        return 2;
    case legit_hitbox_right_hand:
    case legit_hitbox_left_hand:
    case legit_hitbox_right_upper_arm:
    case legit_hitbox_right_forearm:
    case legit_hitbox_left_upper_arm:
    case legit_hitbox_left_forearm:
        return 3;
    case legit_hitbox_right_thigh:
    case legit_hitbox_left_thigh:
    case legit_hitbox_right_calf:
    case legit_hitbox_left_calf:
        return 4;
    case legit_hitbox_right_foot:
    case legit_hitbox_left_foot:
        return 5;
    }

    return -1;
}

static ImVec2 to_imvec2(const vec3_t& v) {
    return ImVec2(v.x, v.y);
}

static vec3_t angle_to_direction(const vec3_t& angles) {
    float pitch = deg2rad(angles.x);
    float yaw = deg2rad(angles.y);

    float cp = cosf(pitch), sp = sinf(pitch);
    float cy = cosf(yaw), sy = sinf(yaw);

    return vec3_t(cp * cy, cp * sy, -sp);
}
static vec3_t get_removed_aim_punch_angle(c_cs_player_pawn* local_player) {
    using fnUpdateAnimPunch = void(__fastcall*)(void*, vec3_t*, float, bool);
    static fnUpdateAnimPunch fn = reinterpret_cast<fnUpdateAnimPunch>(utils::find_pattern_rel(g_modules.client, XOR_STR("E8 ? ? ? ? 48 8D 54 24 ? 48 8B CF E8 ? ? ? ? F2 0F 10 4C 24"), 0x1));

    vec3_t aim_punch = {};
    fn(local_player, &aim_punch, 0.0f, true);
    return aim_punch;
}
enum contents_t {
    CONTENTS_EMPTY = 0,
    CONTENTS_SOLID = 0x1,
    CONTENTS_WINDOW = 0x2,
    CONTENTS_AUX = 0x4,
    CONTENTS_GRATE = 0x8,
    CONTENTS_SLIME = 0x10,
    CONTENTS_WATER = 0x20,
    CONTENTS_BLOCKLOS = 0x40,
    CONTENTS_OPAQUE = 0x80,
    CONTENTS_TESTFOGVOLUME = 0x100,
    CONTENTS_UNUSED = 0x200,
    CONTENTS_BLOCKLIGHT = 0x400,
    CONTENTS_TEAM1 = 0x800,
    CONTENTS_TEAM2 = 0x1000,
    CONTENTS_IGNORE_NODRAW_OPAQUE = 0x2000,
    CONTENTS_MOVEABLE = 0x4000,
    CONTENTS_AREAPORTAL = 0x8000,
    CONTENTS_PLAYERCLIP = 0x10000,
    CONTENTS_MONSTERCLIP = 0x20000,
    CONTENTS_CURRENT_0 = 0x40000,
    CONTENTS_CURRENT_90 = 0x80000,
    CONTENTS_CURRENT_180 = 0x100000,
    CONTENTS_CURRENT_270 = 0x200000,
    CONTENTS_CURRENT_UP = 0x400000,
    CONTENTS_CURRENT_DOWN = 0x800000,
    CONTENTS_ORIGIN = 0x1000000,
    CONTENTS_MONSTER = 0x2000000,
    CONTENTS_DEBRIS = 0x4000000,
    CONTENTS_DETAIL = 0x8000000,
    CONTENTS_TRANSLUCENT = 0x10000000,
    CONTENTS_LADDER = 0x20000000,
    CONTENTS_HITBOX = 0x40000000,
};
enum trace_masks_t {
    MASK_ALL = 0xFFFFFFFF,
    MASK_SOLID = CONTENTS_SOLID | CONTENTS_MOVEABLE | CONTENTS_WINDOW | CONTENTS_MONSTER | CONTENTS_GRATE,
    MASK_PLAYERSOLID = CONTENTS_SOLID | CONTENTS_MOVEABLE | CONTENTS_PLAYERCLIP | CONTENTS_WINDOW | CONTENTS_MONSTER | CONTENTS_GRATE,
    MASK_NPCSOLID = CONTENTS_SOLID | CONTENTS_MOVEABLE | CONTENTS_MONSTERCLIP | CONTENTS_WINDOW | CONTENTS_MONSTER | CONTENTS_GRATE,
    MASK_NPCFLUID = CONTENTS_SOLID | CONTENTS_MOVEABLE | CONTENTS_MONSTERCLIP | CONTENTS_WINDOW | CONTENTS_MONSTER | CONTENTS_GRATE,
    MASK_WATER = CONTENTS_WATER | CONTENTS_MOVEABLE | CONTENTS_SLIME,
    MASK_OPAQUE = CONTENTS_SOLID | CONTENTS_MOVEABLE | CONTENTS_OPAQUE,
    MASK_OPAQUE_AND_NPCS = MASK_OPAQUE | CONTENTS_MONSTER,
    MASK_BLOCKLOS = CONTENTS_SOLID | CONTENTS_MOVEABLE | CONTENTS_BLOCKLOS,
    MASK_BLOCKLOS_AND_NPCS = MASK_BLOCKLOS | CONTENTS_MONSTER,
    MASK_VISIBLE = MASK_OPAQUE | CONTENTS_IGNORE_NODRAW_OPAQUE,
    MASK_VISIBLE_AND_NPCS = MASK_OPAQUE_AND_NPCS | CONTENTS_IGNORE_NODRAW_OPAQUE,
    MASK_SHOT = CONTENTS_SOLID | CONTENTS_MOVEABLE | CONTENTS_MONSTER | CONTENTS_WINDOW | CONTENTS_DEBRIS | CONTENTS_GRATE | CONTENTS_HITBOX,
    MASK_SHOT_BRUSHONLY = CONTENTS_SOLID | CONTENTS_MOVEABLE | CONTENTS_WINDOW | CONTENTS_DEBRIS,
    MASK_SHOT_HULL = CONTENTS_SOLID | CONTENTS_MOVEABLE | CONTENTS_MONSTER | CONTENTS_WINDOW | CONTENTS_DEBRIS | CONTENTS_GRATE,
    MASK_SHOT_PORTAL = CONTENTS_SOLID | CONTENTS_MOVEABLE | CONTENTS_WINDOW | CONTENTS_MONSTER,
    MASK_SOLID_BRUSHONLY = CONTENTS_SOLID | CONTENTS_MOVEABLE | CONTENTS_WINDOW | CONTENTS_GRATE,
    MASK_PLAYERSOLID_BRUSHONLY = CONTENTS_SOLID | CONTENTS_MOVEABLE | CONTENTS_WINDOW | CONTENTS_PLAYERCLIP | CONTENTS_GRATE,
    MASK_NPCSOLID_BRUSHONLY = CONTENTS_SOLID | CONTENTS_MOVEABLE | CONTENTS_WINDOW | CONTENTS_MONSTERCLIP | CONTENTS_GRATE,
    MASK_NPCWORLDSTATIC = CONTENTS_SOLID | CONTENTS_WINDOW | CONTENTS_MONSTERCLIP | CONTENTS_GRATE,
    MASK_NPCWORLDSTATIC_FLUID = CONTENTS_SOLID | CONTENTS_WINDOW | CONTENTS_MONSTERCLIP,
    MASK_SPLITAREPORTAL = CONTENTS_WATER | CONTENTS_SLIME,
    MASK_CURRENT = CONTENTS_CURRENT_0 | CONTENTS_CURRENT_90 | CONTENTS_CURRENT_180 | CONTENTS_CURRENT_270 | CONTENTS_CURRENT_UP | CONTENTS_CURRENT_DOWN,
    MASK_DEADSOLID = CONTENTS_SOLID | CONTENTS_PLAYERCLIP | CONTENTS_WINDOW | CONTENTS_GRATE,
};
static bool is_visible(c_cs_player_pawn* entity, vec3_t& pos)
{
    if(!g_ctx.local)
		return false;
    const vec3_t local_eyes = g_ctx.local->get_eye_pos();

    ray_t ray{};
    game_trace_t trace{};
    trace_filter_t filter{};

    interfaces::engine_trace->init_trace(filter, g_ctx.local, MASK_SHOT, 0x3, 0x7);
    interfaces::engine_trace->trace_shape(&ray, local_eyes, pos, &filter, &trace);

    return trace.m_fraction > 0.97f || trace.m_hit_entity == entity;
}

static bool is_player_visible(c_cs_player_pawn* local_pawn, c_cs_player_pawn* target_pawn)
{
    if (!local_pawn || !target_pawn)
        return false;

    vec3_t local_eyes = local_pawn->get_eye_pos();
    vec3_t target_pos = target_pawn->game_scene_node()->vec_abs_origin();

    vec3_t points[] = {
        target_pos + vec3_t(0, 0, 64.f), target_pos + vec3_t(0, 0, 36.f), target_pos
    };

    game_trace_t trace;
    trace_filter_t filter(0x1C3003, local_pawn, nullptr, 4);
    ray_t ray;

    for (const auto& point : points)
    {
        interfaces::engine_trace->trace_shape(&ray, local_eyes, point, &filter, &trace);
        if (trace.m_hit_entity == target_pawn && trace.m_fraction >= 0.97f)
            return true;
    }

    return false;
}
static int get_active_weapon_def_index()
{
    if (!g_ctx.local)
        return {};
    if (!g_ctx.local->is_alive())
        return {};
    if (!interfaces::engine_client->is_in_game() && !interfaces::engine_client->is_connected())
		return {};
    c_player_weapon_services* weapon_services = g_ctx.local->weapon_services();
    if (!weapon_services)
        return {};
    if (uintptr_t(weapon_services) == 0xc000002000000000) return {};

    c_base_handle active_weapon_handle = weapon_services->active_weapon();
    if (!active_weapon_handle.is_valid())
        return {};

    auto weapon = reinterpret_cast<c_econ_entity*>(interfaces::entity_system->get_base_entity(active_weapon_handle.get_entry_index()));
    if (!weapon)
        return {};

    auto attribute_manager = weapon->attribute_manager();
    if (!attribute_manager)
        return {};

    auto item = attribute_manager->item();
    if (!item)
        return {};
    auto item_def_index = item->item_definition_index();

    return item_def_index;
}
static weapon_category_t get_weapon_category_by_defindex(int defindex)
{
    if (defindex == 43 || defindex == 44 || defindex == 45 || defindex == 46 || defindex == 47 || defindex == 48 || defindex == 49)
        return weapon_category_t::invalid;
    else if (defindex == 500 || defindex == 503 || defindex == 505 || defindex == 506 || defindex == 507 || defindex == 508 || defindex == 509 || defindex == 512 || defindex == 514 || defindex == 515 || defindex == 516 || defindex == 517 || defindex == 518 || defindex == 519 || defindex == 520 || defindex == 521 || defindex == 522 || defindex == 523 || defindex == 525 || defindex == 526 || defindex == 42 || defindex == 59)
        return weapon_category_t::knife;
    else if (defindex == 2 || defindex == 3 || defindex == 4 || defindex == 30 || defindex == 36 || defindex == 61 || defindex == 63 || defindex == 32)
        return weapon_category_t::pistols;
    else if (defindex == 1 || defindex == 64)
        return weapon_category_t::deagle;
    else if (defindex == 7 || defindex == 8 || defindex == 10 || defindex == 13 || defindex == 16 || defindex == 39 || defindex == 60)
        return weapon_category_t::rifles;
    else if (defindex == 14 || defindex == 28 || defindex == 27 || defindex == 35 || defindex == 29 || defindex == 25 || defindex == 17 || defindex == 19 || defindex == 24 || defindex == 26 || defindex == 33 || defindex == 34 || defindex == 23)
        return weapon_category_t::heavy;
    else if (defindex == 11 || defindex == 38)
        return weapon_category_t::auto_sniper;
    else if (defindex == 40 || defindex == 9)
        return weapon_category_t::scout;
    else
        return weapon_category_t::default_wpn;
}


static legit_settings get_legit_settings(weapon_category_t category) {
    legit_settings settings{};

    if (category == weapon_category_t::pistols)
        settings = {
            (float)config.legit.fov_pistols,
            (float)config.legit.smooth_pistols,
            (float)config.legit.delay_pistols,
            config.legit.humanize_float_pistols,
            (float)config.legit.rcs_smooth_pistols,
            (float)config.legit.rcs_fov_pistols,
            config.legit.rcs_after_shots_pistols,
            config.legit.triggerbot_deley_before_shoot_pistols,
            config.legit.triggerbot_delay_between_shots_pistols,
            config.legit.triggerbot_hitchance_pistols,
            config.legit.enable_rcs_pistols,
            config.legit.min_damage_pistols,
            config.legit.min_damage_override_pistols
    };
    else if (category == weapon_category_t::heavy)
        settings = {
            (float)config.legit.fov_heavy,
            (float)config.legit.smooth_heavy,
            (float)config.legit.delay_heavy,
            config.legit.humanize_float_heavy,
            (float)config.legit.rcs_smooth_heavy,
            (float)config.legit.rcs_fov_heavy,
            config.legit.rcs_after_shots_heavy,
            config.legit.triggerbot_deley_before_shoot_heavy,
            config.legit.triggerbot_delay_between_shots_heavy,
            config.legit.triggerbot_hitchance_heavy,
            config.legit.enable_rcs_heavy,
            config.legit.min_damage_heavy,
            config.legit.min_damage_override_heavy
    };
    else if (category == weapon_category_t::rifles)
        settings = {
            (float)config.legit.fov_rifles,
            (float)config.legit.smooth_rifles,
            (float)config.legit.delay_rifles,
            config.legit.humanize_float_rifles,
            (float)config.legit.rcs_smooth_rifles,
            (float)config.legit.rcs_fov_rifles,
            config.legit.rcs_after_shots_rifles,
            config.legit.triggerbot_deley_before_shoot_rifles,
            config.legit.triggerbot_delay_between_shots_rifles,
            config.legit.triggerbot_hitchance_rifles,
            config.legit.enable_rcs_rifles,
            config.legit.min_damage_rifles,
            config.legit.min_damage_override_rifles
    };
    else if (category == weapon_category_t::auto_sniper)
        settings = {
            (float)config.legit.fov_auto,
            (float)config.legit.smooth_auto,
            (float)config.legit.delay_auto,
            config.legit.humanize_float_auto,
            (float)config.legit.rcs_smooth_auto,
            (float)config.legit.rcs_fov_auto,
            config.legit.rcs_after_shots_auto,
            config.legit.triggerbot_deley_before_shoot_auto,
            config.legit.triggerbot_delay_between_shots_auto,
            config.legit.triggerbot_hitchance_auto,
            config.legit.enable_rcs_auto,
            config.legit.min_damage_auto,
            config.legit.min_damage_override_auto
    };
    else if (category == weapon_category_t::scout)
        settings = {
            (float)config.legit.fov_scout,
            (float)config.legit.smooth_scout,
            (float)config.legit.delay_scout,
            config.legit.humanize_float_scout,
            (float)config.legit.rcs_smooth_scout,
            (float)config.legit.rcs_fov_scout,
            config.legit.rcs_after_shots_scout,
            config.legit.triggerbot_deley_before_shoot_scout,
            config.legit.triggerbot_delay_between_shots_scout,
            config.legit.triggerbot_hitchance_scout,
            config.legit.enable_rcs_scout,
            config.legit.min_damage_scout,
            config.legit.min_damage_override_scout
    };
    else if (category == weapon_category_t::deagle)
        settings = {
            (float)config.legit.fov_deagle,
            (float)config.legit.smooth_deagle,
            (float)config.legit.delay_deagle,
            config.legit.humanize_float_deagle,
            (float)config.legit.rcs_smooth_deagle,
            (float)config.legit.rcs_fov_deagle,
            config.legit.rcs_after_shots_deagle,
            config.legit.triggerbot_deley_before_shoot_deagle,
            config.legit.triggerbot_delay_between_shots_deagle,
            config.legit.triggerbot_hitchance_deagle,
            config.legit.enable_rcs_deagle,
            config.legit.min_damage_deagle,
            config.legit.min_damage_override_deagle
    };

    return settings;
}

static int calc_hc(c_cs_player_pawn* target_pawn, vec3_t view_angles, c_base_player_weapon* active_weapon, c_cs_weapon_base_v_data* weapon_data, bool no_spread)
{
    if(hc_calc_legit_debug)
        std::printf("===[calc_hc START]===\n");

    if (!g_ctx.local->is_alive()) {
        if (hc_calc_legit_debug)
            std::printf("[Error] Local player is not alive\n");
        return 0;
    }

    if (!target_pawn) {
        if (hc_calc_legit_debug)
            std::printf("[Error] Target pawn is null\n");
        return 0;
    }

    auto local_data = g_prediction->get_local_data();
    if (!local_data) {
        if (hc_calc_legit_debug)
            std::printf("[Error] Local prediction data is null\n");
        return 0;
    }

    if (no_spread) {
        if (hc_calc_legit_debug)
            std::printf("[Info] no_spread is true -> returning 100\n");
        return 100;
    }

    if (hc_calc_legit_debug)
        std::printf("[Debug] Eye pos: %.2f %.2f %.2f\n", local_data->m_eye_pos.x, local_data->m_eye_pos.y, local_data->m_eye_pos.z);
    if (hc_calc_legit_debug)
        std::printf("[Debug] View angles: %.2f %.2f %.2f\n", view_angles.x, view_angles.y, view_angles.z);

    constexpr auto round_accuracy = [](float accuracy) { return floorf(accuracy * 170.f) / 170.f; };
    constexpr auto round_duck_accuracy = [](float accuracy) { return floorf(accuracy * 300.f) / 300.f; };

    float speed = g_ctx.local->abs_velocity().length();
    if (hc_calc_legit_debug)
        std::printf("[Debug] Player speed: %.2f\n", speed);

    bool is_scoped = (weapon_data->weapon_type() == WEAPONTYPE_SNIPER_RIFLE)
        && !(g_ctx.user_cmd->buttons.button_state & IN_ZOOM)
        && !g_ctx.local->scoped();

    bool is_ducking = (g_ctx.local->flags() & FL_DUCKING) || (g_ctx.user_cmd->buttons.button_state & IN_DUCK);
    bool on_ground = g_ctx.local->flags() & FL_ONGROUND;

    if (hc_calc_legit_debug)
        std::printf("[Debug] Scoped: %d | Ducking: %d | On Ground: %d\n", is_scoped, is_ducking, on_ground);

    float raw_inaccuracy = active_weapon->get_inaccuracy();
    float rounded_accuracy = round_accuracy(raw_inaccuracy);
    float rounded_duck_accuracy = round_duck_accuracy(raw_inaccuracy);

    if (hc_calc_legit_debug)
        std::printf("[Debug] Weapon inaccuracy: %.5f\n", raw_inaccuracy);
    if (hc_calc_legit_debug)
        std::printf("[Debug] Rounded: normal = %.5f | duck = %.5f | local_data = %.5f\n",
        rounded_accuracy, rounded_duck_accuracy, local_data->m_inaccuracy);

    if ((is_ducking && is_scoped && on_ground && (rounded_duck_accuracy < local_data->m_inaccuracy)) ||
        (speed <= 0 && is_scoped && on_ground && (rounded_accuracy < local_data->m_inaccuracy)))
    {
        if (hc_calc_legit_debug)
            std::printf("[Info] Perfect accuracy conditions met -> returning 100\n");
        return 100;
    }

    float dist_to_target = local_data->m_eye_pos.dist(target_pawn->get_eye_pos());
    if (hc_calc_legit_debug)
        std::printf("[Debug] Distance to view_angles: %.2f | Weapon range: %.2f\n", dist_to_target, weapon_data->range());

    if (dist_to_target > weapon_data->range()) {
        if (hc_calc_legit_debug)
            std::printf("[Info] Target out of range -> returning 0\n");
        return 0;
    }

    active_weapon->update_accuracy_penality();

    const float spread = local_data->m_spread;
    const float inaccuracy = local_data->m_inaccuracy;
    vec3_t shoot_pos = local_data->m_eye_pos;

    if (hc_calc_legit_debug)
        std::printf("[Debug] Spread: %.6f | Inaccuracy: %.6f\n", spread, inaccuracy);

    int hits = 0;

    auto calculate_spread_angles = [](vec3_t angle, int random_seed, float weapon_inaccuracy, float weapon_spread) -> vec3_t {
        interfaces::random_seed(random_seed + 1);

        float r1 = interfaces::random_float(0.f, 1.f);
        float r2 = interfaces::random_float(0.f, _pi2);
        float r3 = interfaces::random_float(0.f, 1.f);
        float r4 = interfaces::random_float(0.f, _pi2);

        float c1 = std::cos(r2), c2 = std::cos(r4);
        float s1 = std::sin(r2), s2 = std::sin(r4);

        vec3_t spread_vec{
            (c1 * (r1 * weapon_inaccuracy)) + (c2 * (r3 * weapon_spread)),
            (s1 * (r1 * weapon_inaccuracy)) + (s2 * (r3 * weapon_spread))
        };

        vec3_t forward, right, up;
        math::angle_vectors(angle, forward, right, up);

        return (forward + (right * spread_vec.x) + (up * spread_vec.y)).normalized();
        };

    for (int seed = 0; seed < 315; seed++) {
        vec3_t spread_angle = calculate_spread_angles(view_angles, seed, inaccuracy, spread);
        vec3_t end_pos = spread_angle * weapon_data->range() + shoot_pos;

        ray_t ray{};
        game_trace_t trace{};
        trace_filter_t filter{};
        interfaces::engine_trace->init_trace(filter, g_ctx.local, 0x46004003, 0x3, 0x7);
        interfaces::engine_trace->trace_shape(&ray, shoot_pos, end_pos, &filter, &trace);
        interfaces::engine_trace->clip_ray_entity(&ray, shoot_pos, end_pos, target_pawn, &filter, &trace);

        if (trace.m_hit_entity && trace.m_hit_entity->is_player_pawn() &&
            trace.m_hit_entity->get_ref_ehandle().get_entry_index() == target_pawn->get_ref_ehandle().get_entry_index())
        {
            hits++;
            //if (hc_calc_legit_debug)
            //    std::printf("[Seed %3d] HIT\n", seed);
        }
        else {
            //if (hc_calc_legit_debug)
            //    std::printf("[Seed %3d] MISS\n", seed);
        }
    }

    float result = (static_cast<float>(hits) / 315.0f) * 100.0f;
    if (hc_calc_legit_debug)
        std::printf("[calc_hc END] Hits: %d / 315 -> HitChance = %.2f%%\n", hits, result);
    return static_cast<int>(result);
}

inline void auto_stop_legit(c_user_cmd* user_cmd, c_cs_player_pawn* local_player, c_base_player_weapon* active_weapon, bool no_spread)
{
    if (!config.legit.auto_stop || !local_player || !active_weapon || no_spread)
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
static vec3_t oldPunch{};
static std::atomic<bool> triggerbot_shooting = false;
static auto last_trigger_time = std::chrono::steady_clock::now();
static bool can_shoot = true;
inline const auto g_legitbot = std::make_unique<c_legitbot>();
inline void legit_store_hitboxes() {
    if (config.legit.m_hitboxes[0])
        g_legitbot->m_hitboxes_legit.emplace_back(legit_hitbox_head);

    if (config.legit.m_hitboxes[1]) {
        g_legitbot->m_hitboxes_legit.emplace_back(legit_hitbox_chest);
        g_legitbot->m_hitboxes_legit.emplace_back(legit_hitbox_lower_chest);
        g_legitbot->m_hitboxes_legit.emplace_back(legit_hitbox_upper_chest);
    }

    if (config.legit.m_hitboxes[2]) {
        g_legitbot->m_hitboxes_legit.emplace_back(legit_hitbox_pelvis);
        g_legitbot->m_hitboxes_legit.emplace_back(legit_hitbox_stomach);
    }

    if (config.legit.m_hitboxes[3]) {
        g_legitbot->m_hitboxes_legit.emplace_back(legit_hitbox_right_hand);
        g_legitbot->m_hitboxes_legit.emplace_back(legit_hitbox_left_hand);
        g_legitbot->m_hitboxes_legit.emplace_back(legit_hitbox_right_upper_arm);
        g_legitbot->m_hitboxes_legit.emplace_back(legit_hitbox_right_forearm);
        g_legitbot->m_hitboxes_legit.emplace_back(legit_hitbox_left_upper_arm);
        g_legitbot->m_hitboxes_legit.emplace_back(legit_hitbox_left_forearm);
    }

    if (config.legit.m_hitboxes[4]) {
        g_legitbot->m_hitboxes_legit.emplace_back(legit_hitbox_right_thigh);
        g_legitbot->m_hitboxes_legit.emplace_back(legit_hitbox_left_thigh);

        g_legitbot->m_hitboxes_legit.emplace_back(legit_hitbox_right_calf);
        g_legitbot->m_hitboxes_legit.emplace_back(legit_hitbox_left_calf);
    }

    if (config.legit.m_hitboxes[5]) {
        g_legitbot->m_hitboxes_legit.emplace_back(legit_hitbox_right_foot);
        g_legitbot->m_hitboxes_legit.emplace_back(legit_hitbox_left_foot);
    }
}