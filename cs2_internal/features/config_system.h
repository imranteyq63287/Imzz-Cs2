#pragma once

#include <vector>
#include <string>
#include <fstream>
#include <memory>

#include "../includes.h"
#include "../dependencies/json/json.h"

#include "menu/gui_base/hashes.hpp"
#include "menu/gui_base/notification.hpp"

#include "skins/skins.h"

using namespace std;
using json = nlohmann::json;

static std::vector<std::string> configNames = { "default" };
inline std::string folderPath = "./imzz/config";

static int selected = 0;

enum class weapon_category_t 
{
    invalid,
    knife,
    default_wpn,
    pistols,
    deagle,
    rifles,
    heavy,
    auto_sniper,
    scout
};

static bool fov_hook_debug = false;
static bool legit_debug = false;
static bool hc_calc_legit_debug = false;
static bool radarhack_debug = false;
static bool esp_debug = false;

inline void RefreshConfigs()
{
    configNames.clear();
    configNames.push_back("default");

    if (!std::filesystem::exists(folderPath))
        std::filesystem::create_directory(folderPath);

    for (const auto& entry : std::filesystem::directory_iterator(folderPath))
    {
        if (entry.is_regular_file()) 
        {
            auto path = entry.path();

            if (path.extension() == ".cfg") 
                configNames.push_back(path.stem().string());
        }
    }

    if (selected >= configNames.size())
        selected = 0;
}

inline void OpenFolderCFG()
{
    if (!std::filesystem::exists(folderPath))
        std::filesystem::create_directory(folderPath);

    char absPath[MAX_PATH];

    if (!_fullpath(absPath, folderPath.c_str(), MAX_PATH))
        return;

    ShellExecuteA(NULL, "open", absPath, NULL, NULL, SW_SHOW);
}

class c_config_system
{
public:
    std::string folder = XOR_STR("");

    void create_config_folder();
};

inline auto save_color = [](nlohmann::json& node, const c_color& color) {
    node = {
      {"r", color.r},
      {"g", color.g},
      {"b", color.b},
      {"a", color.a}
    };
};

inline auto load_color = [](const nlohmann::json& node, c_color& color) {
    if (!node.is_object() || !node.contains("r") || !node.contains("g") || !node.contains("b") || !node.contains("a")) 
    {
        color = c_color(1.0f, 1.0f, 1.0f, 1.0f);
        return;
    }

    color.r = node.value("r", 1.0f);
    color.g = node.value("g", 1.0f);
    color.b = node.value("b", 1.0f);
    color.a = node.value("a", 1.0f);
};

inline const auto configuration = std::make_unique<c_config_system>();

inline float viewX;
inline float viewY;
inline float viewZ;
inline float viewFov;

struct rage_weapon_t
{
    bool enable_config;

    int hitboxes;
    int multipoints;
    int point_scale = { 50 };
    int min_damage = 10;
    int min_damage_override = 1;
    int hit_chance = -1;
};

struct rage_bot_t
{
    bool baim = false;
    bool team_check = true;
    bool enable;
    bool doubletap;

    float maximum_fov = 180.f;

    bool silent_aim;
    bool automatic_fire;
    bool auto_stop;
    bool auto_scope;
    bool auto_wall;
    bool m_hitboxes[6];
    bool m_multi_points[6];

    int multipointscale = 0;
    int group_type;
    int min_damage, min_damage_keybind, min_damage_keybind_style, min_damage_override;
    int force_shoot, force_shoot_style;
    int hit_chance, hit_chance_keybind, hit_chance_keybind_style, hit_chance_override;
};

struct legit_bot_t
{
    bool enable;
    bool draw_fov;
    bool triggerbot;
    int triggerbot_keybind, triggerbot_keybind_style;
    bool rcs;
    bool team_check;
    bool aimlock;
    int aimlock_keybind, aimlock_keybind_style;
    bool m_hitboxes[6];
    bool autowall;
    int nearest_hitbox = 1;
    bool auto_stop;
    bool auto_scope;
    bool aim_checks[5] = { false, false, false, false, false };
    int min_damage_keybind, min_damage_keybind_style;

    // Pistols
    int fov_pistols = 5;
    int smooth_pistols = 30;
    int delay_pistols = 50;
    float humanize_float_pistols = 0.0f;
    int triggerbot_deley_before_shoot_pistols;
    int triggerbot_delay_between_shots_pistols;
    int triggerbot_hitchance_pistols = 0;
    int rcs_smooth_pistols = 30;
    int rcs_fov_pistols = 5;
    int rcs_after_shots_pistols = 1;
    bool enable_rcs_pistols = true;
    int min_damage_pistols;
    int min_damage_override_pistols = 0;

    // Heavy
    int fov_heavy = 5;
    int smooth_heavy = 30;
    int delay_heavy = 50;
    float humanize_float_heavy = 0.0f;
    int triggerbot_deley_before_shoot_heavy;
    int triggerbot_delay_between_shots_heavy;
    int triggerbot_hitchance_heavy = 0;
    int rcs_smooth_heavy = 30;
    int rcs_fov_heavy = 5;
    int rcs_after_shots_heavy = 1;
    bool enable_rcs_heavy = true;
    int min_damage_heavy = 0;
    int min_damage_keybind_heavy = 0;
    int min_damage_keybind_style_heavy = 0;
    int min_damage_override_heavy = 0;

    // Rifles
    int fov_rifles = 5;
    int smooth_rifles = 30;
    int delay_rifles = 50;
    float humanize_float_rifles = 0.0f;
    int triggerbot_deley_before_shoot_rifles;
    int triggerbot_delay_between_shots_rifles;
    int triggerbot_hitchance_rifles = 0;
    int rcs_smooth_rifles = 30;
    int rcs_fov_rifles = 5;
    int rcs_after_shots_rifles = 1;
    bool enable_rcs_rifles = true;
    int min_damage_rifles = 0;
    int min_damage_keybind_rifles = 0;
    int min_damage_keybind_style_rifles = 0;
    int min_damage_override_rifles = 0;

    // Auto Sniper
    int fov_auto = 5;
    int smooth_auto = 30;
    int delay_auto = 50;
    float humanize_float_auto = 0.0f;
    int triggerbot_deley_before_shoot_auto;
    int triggerbot_delay_between_shots_auto;
    int triggerbot_hitchance_auto = 0;
    int rcs_smooth_auto = 30;
    int rcs_fov_auto = 5;
    int rcs_after_shots_auto = 1;
    bool enable_rcs_auto = true;
    int min_damage_auto = 0;
    int min_damage_keybind_auto = 0;
    int min_damage_keybind_style_auto = 0;
    int min_damage_override_auto = 0;

    // Scout
    int fov_scout = 5;
    int smooth_scout = 30;
    int delay_scout = 50;
    float humanize_float_scout = 0.0f;
    int triggerbot_deley_before_shoot_scout;
    int triggerbot_delay_between_shots_scout;
    int triggerbot_hitchance_scout = 0;
    int rcs_smooth_scout = 30;
    int rcs_fov_scout = 5;
    int rcs_after_shots_scout = 1;
    bool enable_rcs_scout = true;
    int min_damage_scout = 0;
    int min_damage_keybind_scout = 0;
    int min_damage_keybind_style_scout = 0;
    int min_damage_override_scout = 0;

    // Deagle
    int fov_deagle = 5;
    int smooth_deagle = 30;
    int delay_deagle = 50;
    float humanize_float_deagle = 0.0f;
    int triggerbot_deley_before_shoot_deagle;
    int triggerbot_delay_between_shots_deagle;
    int triggerbot_hitchance_deagle = 0;
    int rcs_smooth_deagle = 30;
    int rcs_fov_deagle = 5;
    int rcs_after_shots_deagle = 1;
    bool enable_rcs_deagle = true;
    int min_damage_deagle = 0;
    int min_damage_keybind_deagle = 0;
    int min_damage_keybind_style_deagle = 0;
    int min_damage_override_deagle = 0;
};

struct anti_aim_t
{
    bool enable;
    int pitch_item = 0;

    int pitch;
    bool fpitchup;
    bool fpitchdown;
    int firstpitch;
    int secondpitch;
    int yaw;
    int yaw_item = 0;
    bool hideshots;
    bool manualleft;
    int manualleft_keybind, manualleft_keybind_style;
    bool manualright;
    int manualright_keybind, manualright_keybind_style;
    bool manualback;
    int manualback_keybind, manualback_keybind_style;
};

struct esp_settings_t 
{
    bool enable = false;
    bool ragdoll = false;

    bool show_box = false;
    bool box_outline = true;

    bool show_health = false;
    bool hp_bar_text = true;
    bool hp_bar_text_outline = true;
    bool health_outline = true;

    bool show_name = false;
    bool name_outline = true;

    bool show_weapon = false;
    bool weapon_name = false;
    bool weapon_icon = false;
    bool weapon_name_outline = true;

    bool show_skeleton = false;
    bool skeleton_outline = false;

    bool show_ammo = false;
    bool ammo_outline = true;
    bool ammo_bar_text = true;
    bool ammo_bar_text_outline = true;

    bool show_flags = false;
    bool flags_outline = true;

    bool show_snapline = false;
    bool snapline_outline = false;

    int boxtype = 0;
    int snaplinetype = 0;
    int skeletontype = 0;

    int hp_bar_width = 2;
    int ammo_bar_width = 2;

    bool glow;
    bool localglow;

    bool chamsvis = false;
    bool chamsinvis = false;
    bool chamsragdoll = false;
    bool chamsonhit = false;

    c_color chamsvis_color{ 1.f, 1.f, 1.f, 1.f };
    c_color chamsinvis_color{ 1.f, 1.f, 1.f, 1.f };
    c_color chamsonhit_color{ 1.f, 1.f, 1.f, 1.f };

    int chamstypevis = 0;
    int chamstypeinvis = 0;
    bool chicken = false;
    bool chickenname = false;
    bool chickenchams = false;
    bool chickenbox = false;
    bool chicken_outline_name = false;
    bool chicken_outline_icon = false;
    bool chicken_outline_box = false;
    int chickentypechams = 0;

    c_color chicken_box_color{ 1.f, 1.f, 1.f, 1.f };
    c_color chicken_box_outline_color{ 1.f, 1.f, 1.f, 1.f };

    c_color chicken_chams_color{ 1.f, 1.f, 1.f, 1.f };
    c_color chicken_chams_outline_color{ 1.f, 1.f, 1.f, 1.f };

    c_color chicken_name_color{ 1.f, 1.f, 1.f, 1.f };
    c_color chicken_name_outline_color{ 1.f, 1.f, 1.f, 1.f };

    c_color chicken_icon_color{ 1.f, 1.f, 1.f, 1.f };
    c_color chicken_icon_outline_color{ 1.f, 1.f, 1.f, 1.f };

    bool hostage = false;
    bool hostagename = false;
    bool hostagechams = false;
    bool hostagebox = false;
    bool hostage_outline_name = false;
    bool hostage_outline_icon = false;
    bool hostage_outline_box = false;
    int hostagetypechams = 0;

    c_color hostage_box_color{ 1.f, 1.f, 1.f, 1.f };
    c_color hostage_chams_color{ 1.f, 1.f, 1.f, 1.f };
    c_color hostage_box_outline_color{ 1.f, 1.f, 1.f, 1.f };

    c_color hostage_name_color{ 1.f, 1.f, 1.f, 1.f };
    c_color hostage_name_outline_color{ 1.f, 1.f, 1.f, 1.f };

    c_color hostage_icon_color{ 1.f, 1.f, 1.f, 1.f };
    c_color hostage_icon_outline_color{ 1.f, 1.f, 1.f, 1.f };
    c_color box_color{ 1.f, 1.f, 1.f, 1.f };
    c_color box_outline_color{ 0.f, 0.f, 0.f, 1.f };

    c_color health_color{ 1.f, 1.f, 1.f, 1.f };
    c_color health_color_outline{ 0.f, 0.f, 0.f, 1.f };
    c_color health_text_color_outline{ 0.f, 0.f, 0.f, 1.f };
    c_color hp_bar_text_color{ 1.f, 1.f, 1.f, 1.f };

    c_color name_color{ 1.f, 1.f, 1.f, 1.f };
    c_color name_color_outline{ 0.f, 0.f, 0.f, 1.f };

    c_color weapon_color{ 1.f, 1.f, 1.f, 1.f };
    c_color weapon_color_outline{ 0.f, 0.f, 0.f, 1.f };

    c_color ammo_color{ 1.f, 1.f, 1.f, 1.f };
    c_color ammo_color_outline{ 0.f, 0.f, 0.f, 1.f };
    c_color ammo_bar_text_color{ 1.f, 1.f, 1.f, 1.f };
    c_color ammo_text_color_outline{ 0.f, 0.f, 0.f, 1.f };

    c_color flags_color{ 1.f, 1.f, 1.f, 1.f };
    c_color flags_text_color_outline{ 0.f, 0.f, 0.f, 1.f };

    c_color skeleton_color{ 1.f, 1.f, 1.f, 1.f };
    c_color skeleton_outline_color{ 0.f, 0.f, 0.f, 1.f };

    c_color snapline_color{ 1.f, 1.f, 1.f, 1.f };
    c_color snapline_outline_color{ 0.f, 0.f, 0.f, 1.f };

    c_color glow_color{ 1.f, 1.f, 1.f, 1.f };
    c_color local_glow_color{ 1.f, 1.f, 1.f, 1.f };

    bool onlyvisible;
    bool radarhack;

    bool show_sound = false;
    c_color sound_color{ 1.f, 1.f, 1.f, 1.f };

};

struct local_settings_t 
{

    bool chamsvis = false;
    bool chamsinvis = false;
    bool chamsragdoll = false;
    bool chamsviewmodelvis = false;
    bool chamsviewmodelinvis = false;
    bool chamsweaponvis = false;
    bool chamsweaponinvis = false;
    c_color chamsvis_color{ 1.f, 1.f, 1.f, 1.f };
    c_color chamsinvis_color{ 1.f, 1.f, 1.f, 1.f };
    c_color chamsviewmodelvis_color{ 1.f, 1.f, 1.f, 1.f };
    c_color chamsviewmodelinvis_color{ 1.f, 1.f, 1.f, 1.f };
    c_color chamsweaponvis_color{ 1.f, 1.f, 1.f, 1.f };
    c_color chamsweaponinvis_color{ 1.f, 1.f, 1.f, 1.f };
    int chamstype = 0;
};

struct extra_settings_t
{
    bool handswitch;
    bool removals[6] = { false, false, false, false, false, false};
    bool pusto1;
	bool pusto2;
	bool pusto3;
	bool pusto4;
	bool pusto5;
	bool pusto6;

    bool model;
	bool model_ct;
	bool model_t;
    bool custommodel;
	int custommodel_ct;
	int custommodel_t;

    bool thirdperson = false;
    int thirdpersondist = 70;
    int thirdperson_keybind, thirdperson_keybind_style;

    int thirdpersonalpha = 255;
    bool knife_changer = false;
	bool knife_changer_ct = false;
	bool knife_changer_t = false;
    bool glove_changer = false;
    int m_selected = 0;
    int m_selected_ct = 0;
    int m_selected_knife = 0;
    int m_selected_knife_t = 0;
	int m_selected_knife_ct = 0;
    int m_selected_skin_knife = 0;
    int m_selected_skin_glove = 0;

    int knifestatrack = 0;
    float wearskins = 0.01f;
    float aspect_ratio = 3.f;
    bool custom_models;
	bool custom_models_ct;
	bool custom_models_t;
    bool pencross;

    struct skin_data_t
    {
        int m_paint_kit = 0;
        int m_item_def_index = 0;

        int m_prev_skin = -1;
    } m_skin_settings[100];
    inline void to_json(nlohmann::json& j, const skin_data_t& s) {
        j = nlohmann::json{
            {"paint_kit", s.m_paint_kit},
            {"item_def_index", s.m_item_def_index},
            {"prev_skin", s.m_prev_skin}
        };
    }

    inline void from_json(const nlohmann::json& j, skin_data_t& s) {
        s.m_paint_kit = j.value("paint_kit", 0);
        s.m_item_def_index = j.value("item_def_index", 0);
        s.m_prev_skin = j.value("prev_skin", -1);
    }


    int m_selected_type = 0;
    int m_selected_item = 0;
    int m_selected_weapon = 0;
    int seed = 0;
    int glove_selected_paintkit = 0;
    bool nade_traces = false;
    bool nade_camera = false;
    bool hit_effect = false;
    bool trashtalk = false;

    float desiredfov = 90.f;
    float desired_fov_zoom_level_1 = 90.f;
    float desired_fov_zoom_level_2 = 90.f;
    bool viewmodel = false;
    bool overrideview = false;
    float viewmodel_fov = 68;
    float viewmodel_offset_x = 0;
    float viewmodel_offset_y = 0;
    float viewmodel_offset_z = 0;


    bool auto_accept = false;
    bool sniper_cross = false;
};
struct world_modulation_t
{
    bool ambience;
    bool customsky;
    bool sparkeffects;
    c_color sky_clr{ 1.f, 1.f, 1.f, 1.f },
        sun_clr{ 1.f, 1.f, 1.f, 1.f },
        clouds_clr{ 1.f, 1.f, 1.f, 1.f },
        props_clr{ 1.f, 1.f, 1.f, 1.f },
        world_clr{ 1.f, 1.f, 1.f, 1.f },
        lighting_clr{ 1.f, 1.f, 1.f, 1.f };

    int exposure = 70;

    bool fog_modulation;
    c_color fog_clr{ 1.f, 1.f, 1.f, 1.f };
    int fog_start = 0;
    int fog_end = 0;
};

struct chams_t
{
    bool enable;
    int material = 0;
    int overlay_material = 0;

    c_color material_color{ 1.f, 1.f, 1.f, 1.f };
    c_color overlay_material_color{ 1.f, 1.f, 1.f, 1.f };
};
struct movement_t
{
    bool walkbot = false;
    bool bunnyhop = false;
    bool auto_strafe = false;
    bool air_strafe = false;
    bool autopeek = false;
    bool air_duck;
    int autopeek_keybind = 0, autopeek_keybind_style = 0;
    struct {
        bool m_autopeeking;
        bool m_should_retrack;
        bool did_shoot;
        vec3_t m_old_origin;
    } m_autopeek;
};
struct gui_t
{
    bool watermark = false;
    bool indicators[9];
    bool hitlog = false;
    bool misslog = false;
    int hitlog_position = 0;
    int hsound = 0;
    int hsoundvol = 50;
    int ksound = 0;
    int ksoundvol = 50;
    int animspeed = 100;
    int menu_opacity = 120;
    int glow_strength = 6;
    bool chatlogs;
    bool doubletapready;
    bool doubletapbroken;


};
struct world_t
{
    bool chamsweaponvis = false;
    bool chamsweaponinvis = false;
    c_color chamsweaponvis_color{ 1.f, 1.f, 1.f, 1.f };
    c_color chamsweaponinvis_color{ 1.f, 1.f, 1.f, 1.f };
    int weapontypechams = 0;
    bool bomb = false;
    bool bombname = false;
    bool bombicon = false;
    bool bombbox = false;
    bool bombtimertext = false;
    bool bomb_outline_name = false;
    bool bomb_outline_icon = false;
    bool bomb_outline_box = false;
    bool bomb_outline_timertext = false;
    c_color bomb_box_color{ 1.f, 1.f, 1.f, 1.f };
    c_color bomb_box_outline_color{ 0.f, 0.f, 0.f, 1.f };

    c_color bomb_name_color{ 1.f, 1.f, 1.f, 1.f };
    c_color bomb_name_outline_color{ 0.f, 0.f, 0.f, 1.f };

    c_color bomb_icon_color{ 1.f, 1.f, 1.f, 1.f };
    c_color bomb_icon_outline_color{ 0.f, 0.f, 0.f, 1.f };

    c_color bomb_timer_text_color{ 1.f, 1.f, 1.f, 1.f };
    c_color bomb_timer_text_outline_color{ 0.f, 0.f, 0.f, 1.f };

    bool weapon = false;
    bool weaponname = false;
    bool weaponicon = false;
    bool weaponbox = false;
    bool weapon_outline_name = false;
    bool weapon_outline_icon = false;
    bool weapon_outline_box = false;

    c_color weapon_box_color{ 1.f, 1.f, 1.f, 1.f };
    c_color weapon_box_outline_color{ 0.f, 0.f, 0.f, 1.f };

    c_color weapon_name_color{ 1.f, 1.f, 1.f, 1.f };
    c_color weapon_name_outline_color{ 0.f, 0.f, 0.f, 1.f };

    c_color weapon_icon_color{ 1.f, 1.f, 1.f, 1.f };
    c_color weapon_icon_outline_color{ 0.f, 0.f, 0.f, 1.f };

    bool granades;



};
struct config_t
{
    rage_bot_t ragebot;
    legit_bot_t legit;

    anti_aim_t antiaim;

    world_modulation_t world_modulation;
    world_t world;
    esp_settings_t esp;
    local_settings_t local;
    extra_settings_t extra;
    movement_t movement;
    gui_t gui;
};

inline config_t config;





inline void SaveConfig(const std::string& filename)
{
    json root;
    // RRagebot
    root["ragebot_enable"] = config.ragebot.enable;
    root["ragebot_doubletap"] = config.ragebot.doubletap;
    root["ragebot_baim"] = config.ragebot.baim;
    root["ragebot_team_check"] = config.ragebot.team_check;

    root["ragebot_maximum_fov"] = config.ragebot.maximum_fov;
    root["ragebot_silent_aim"] = config.ragebot.silent_aim;
    root["ragebot_automatic_fire"] = config.ragebot.automatic_fire;
    root["ragebot_auto_stop"] = config.ragebot.auto_stop;
    root["ragebot_auto_scope"] = config.ragebot.auto_scope;

    root["ragebot_group_type"] = config.ragebot.group_type;
    root["ragebot_min_damage"] = config.ragebot.min_damage;
    root["ragebot_min_damage_keybind"] = config.ragebot.min_damage_keybind;
    root["ragebot_min_damage_keybind_style"] = config.ragebot.min_damage_keybind_style;
    root["ragebot_min_damage_override"] = config.ragebot.min_damage_override;
    root["ragebot_force_shoot"] = config.ragebot.force_shoot;
    root["ragebot_force_shoot_style"] = config.ragebot.force_shoot_style;
    root["ragebot_hit_chance"] = config.ragebot.hit_chance;
    root["ragebot_hit_chance_keybind"] = config.ragebot.hit_chance_keybind;
    root["ragebot_hit_chance_keybind_style"] = config.ragebot.hit_chance_keybind_style;
    root["ragebot_hit_chance_override"] = config.ragebot.hit_chance_override;
    for (int i = 0; i < 6; ++i)
        root["ragebot_hitboxes"][i] = config.ragebot.m_hitboxes[i];
    for (int i = 0; i < 6; ++i)
        root["ragebot_multi_points"][i] = config.ragebot.m_multi_points[i];
    root["ragebot_multipointscale"] = config.ragebot.multipointscale;
    // Antimaims
    root["antiaim_enabled"] = config.antiaim.enable; // bool
    root["antiaim_pitch"] = config.antiaim.pitch; // int
    root["antiaim_pitch_item"] = config.antiaim.pitch_item; // int

    root["antiaim_fpitchup"] = config.antiaim.fpitchup; // bool
    root["antiaim_fpitchdown"] = config.antiaim.fpitchdown; // bool
    root["antiaim_yaw"] = config.antiaim.yaw; // int
    root["antiaim_yaw_item"] = config.antiaim.yaw_item; // int
    root["antiaim_hideshots"] = config.antiaim.hideshots; // bool
    root["antiaim_manualleft"] = config.antiaim.manualleft; // bool
    root["antiaim_manualleft_keybind"] = config.antiaim.manualleft_keybind; // int
    root["antiaim_manualleft_keybind_style"] = config.antiaim.manualleft_keybind_style; // int
    root["antiaim_manualright"] = config.antiaim.manualright; // bool
    root["antiaim_manualright_keybind"] = config.antiaim.manualright_keybind; // int
    root["antiaim_manualright_keybind_style"] = config.antiaim.manualright_keybind_style; // int
    root["antiaim_manualback"] = config.antiaim.manualback; // bool
    root["antiaim_manualback_keybind"] = config.antiaim.manualback_keybind; // int
    root["antiaim_manualback_keybind_style"] = config.antiaim.manualback_keybind_style; // int


    // Legitbot
    root["legit_enable"] = config.legit.enable;
    root["legit_draw_fov"] = config.legit.draw_fov;
    root["legit_triggerbot"] = config.legit.triggerbot;
    root["legit_triggerbot_keybind"] = config.legit.triggerbot_keybind;
    root["legit_triggerbot_keybind_style"] = config.legit.triggerbot_keybind_style;
    root["legit_rcs"] = config.legit.rcs;
    root["legit_team_check"] = config.legit.team_check;
    root["legit_aimlock"] = config.legit.aimlock;
    root["legit_aimlock_keybind"] = config.legit.aimlock_keybind;
    root["legit_aimlock_keybind_style"] = config.legit.aimlock_keybind_style;

    // Pistols
    root["legit_delay_pistols"] = config.legit.delay_pistols;
    root["legit_fov_pistols"] = config.legit.fov_pistols;
    root["legit_smooth_pistols"] = config.legit.smooth_pistols;
    root["legit_rcs_smooth_pistols"] = config.legit.rcs_smooth_pistols;
    root["legit_rcs_fov_pistols"] = config.legit.rcs_fov_pistols;
    root["legit_rcs_after_shots_pistols"] = config.legit.rcs_after_shots_pistols;
    root["triggerbot_delay_before_shoot_pistols"] = config.legit.triggerbot_deley_before_shoot_pistols;
    root["triggerbot_delay_between_shots_pistols"] = config.legit.triggerbot_delay_between_shots_pistols;
    root["triggerbot_hitchance_pistols"] = config.legit.triggerbot_hitchance_pistols;


    // Heavy
    root["legit_delay_heavy"] = config.legit.delay_heavy;
    root["legit_fov_heavy"] = config.legit.fov_heavy;
    root["legit_smooth_heavy"] = config.legit.smooth_heavy;
    root["legit_rcs_smooth_heavy"] = config.legit.rcs_smooth_heavy;
    root["legit_rcs_fov_heavy"] = config.legit.rcs_fov_heavy;
    root["legit_rcs_after_shots_heavy"] = config.legit.rcs_after_shots_heavy;
    root["triggerbot_delay_before_shoot_heavy"] = config.legit.triggerbot_deley_before_shoot_heavy;
    root["triggerbot_delay_between_shots_heavy"] = config.legit.triggerbot_delay_between_shots_heavy;
    root["triggerbot_hitchance_heavy"] = config.legit.triggerbot_hitchance_heavy;

    // Rifles
    root["legit_delay_rifles"] = config.legit.delay_rifles;
    root["legit_fov_rifles"] = config.legit.fov_rifles;
    root["legit_smooth_rifles"] = config.legit.smooth_rifles;
    root["legit_rcs_smooth_rifles"] = config.legit.rcs_smooth_rifles;
    root["legit_rcs_fov_rifles"] = config.legit.rcs_fov_rifles;
    root["legit_rcs_after_shots_rifles"] = config.legit.rcs_after_shots_rifles;
    root["triggerbot_delay_before_shoot_rifles"] = config.legit.triggerbot_deley_before_shoot_rifles;
    root["triggerbot_delay_between_shots_rifles"] = config.legit.triggerbot_delay_between_shots_rifles;
    root["triggerbot_hitchance_rifles"] = config.legit.triggerbot_hitchance_rifles;

    // Auto Sniper
    root["legit_delay_auto"] = config.legit.delay_auto;
    root["legit_fov_auto"] = config.legit.fov_auto;
    root["legit_smooth_auto"] = config.legit.smooth_auto;
    root["legit_rcs_smooth_auto"] = config.legit.rcs_smooth_auto;
    root["legit_rcs_fov_auto"] = config.legit.rcs_fov_auto;
    root["legit_rcs_after_shots_auto"] = config.legit.rcs_after_shots_auto;
    root["triggerbot_delay_before_shoot_auto"] = config.legit.triggerbot_deley_before_shoot_auto;
    root["triggerbot_delay_between_shots_auto"] = config.legit.triggerbot_delay_between_shots_auto;
    root["triggerbot_hitchance_auto"] = config.legit.triggerbot_hitchance_auto;

    // Scout
    root["legit_delay_scout"] = config.legit.delay_scout;
    root["legit_fov_scout"] = config.legit.fov_scout;
    root["legit_smooth_scout"] = config.legit.smooth_scout;
    root["legit_rcs_smooth_scout"] = config.legit.rcs_smooth_scout;
    root["legit_rcs_fov_scout"] = config.legit.rcs_fov_scout;
    root["legit_rcs_after_shots_scout"] = config.legit.rcs_after_shots_scout;
    root["triggerbot_delay_before_shoot_scout"] = config.legit.triggerbot_deley_before_shoot_scout;
    root["triggerbot_delay_between_shots_scout"] = config.legit.triggerbot_delay_between_shots_scout;
    root["triggerbot_hitchance_scout"] = config.legit.triggerbot_hitchance_scout;

    // Deagle
    root["legit_delay_deagle"] = config.legit.delay_deagle;
    root["legit_fov_deagle"] = config.legit.fov_deagle;
    root["legit_smooth_deagle"] = config.legit.smooth_deagle;
    root["legit_rcs_smooth_deagle"] = config.legit.rcs_smooth_deagle;
    root["legit_rcs_fov_deagle"] = config.legit.rcs_fov_deagle;
    root["legit_rcs_after_shots_deagle"] = config.legit.rcs_after_shots_deagle;
    root["triggerbot_delay_before_shoot_deagle"] = config.legit.triggerbot_deley_before_shoot_deagle;
    root["triggerbot_delay_between_shots_deagle"] = config.legit.triggerbot_delay_between_shots_deagle;
    root["triggerbot_hitchance_deagle"] = config.legit.triggerbot_hitchance_deagle;


    //esp_settings
    root["esp_enable"] = config.esp.enable;

    root["esp_show_box"] = config.esp.show_box;
    root["esp_box_outline"] = config.esp.box_outline;

    root["esp_show_health"] = config.esp.show_health;
    root["esp_hp_bar_text"] = config.esp.hp_bar_text;
    root["esp_hp_bar_text_outline"] = config.esp.hp_bar_text_outline;
    root["esp_health_outline"] = config.esp.health_outline;

    root["esp_show_name"] = config.esp.show_name;
    root["esp_name_outline"] = config.esp.name_outline;

    root["esp_show_weapon"] = config.esp.show_weapon;
    root["esp_weapon_name"] = config.esp.weapon_name;
    root["esp_weapon_icon"] = config.esp.weapon_icon;
    root["esp_weapon_name_outline"] = config.esp.weapon_name_outline;

    root["esp_show_skeleton"] = config.esp.show_skeleton;
    root["esp_skeleton_outline"] = config.esp.skeleton_outline;

    root["esp_show_ammo"] = config.esp.show_ammo;
    root["esp_ammo_outline"] = config.esp.ammo_outline;
    root["esp_ammo_bar_text"] = config.esp.ammo_bar_text;
    root["esp_ammo_bar_text_outline"] = config.esp.ammo_bar_text_outline;

    root["esp_show_flags"] = config.esp.show_flags;
    root["esp_flags_outline"] = config.esp.flags_outline;

    root["esp_show_snapline"] = config.esp.show_snapline;
    root["esp_snapline_outline"] = config.esp.snapline_outline;

    root["esp_boxtype"] = config.esp.boxtype;
    root["esp_snaplinetype"] = config.esp.snaplinetype;
    root["esp_skeletontype"] = config.esp.skeletontype;

    root["esp_hp_bar_width"] = config.esp.hp_bar_width;
    root["esp_ammo_bar_width"] = config.esp.ammo_bar_width;

    root["esp_chamsvis"] = config.esp.chamsvis;
    root["esp_chamsinvis"] = config.esp.chamsinvis;
    root["esp_chamsragdoll"] = config.esp.chamsragdoll;
    save_color(root["esp_chamsvis_color"], config.esp.chamsvis_color);
    save_color(root["esp_chamsinvis_color"], config.esp.chamsinvis_color);
    root["esp_chamstypevis"] = config.esp.chamstypevis;
    root["esp_chamstypeinvis"] = config.esp.chamstypeinvis;

    save_color(root["esp_box_color"], config.esp.box_color);
    save_color(root["esp_box_outline_color"], config.esp.box_outline_color);

    save_color(root["esp_health_color"], config.esp.health_color);
    save_color(root["esp_health_color_outline"], config.esp.health_color_outline);
    save_color(root["esp_health_text_color_outline"], config.esp.health_text_color_outline);
    save_color(root["esp_hp_bar_text_color"], config.esp.hp_bar_text_color);

    save_color(root["esp_name_color"], config.esp.name_color);
    save_color(root["esp_name_color_outline"], config.esp.name_color_outline);

    save_color(root["esp_weapon_color"], config.esp.weapon_color);
    save_color(root["esp_weapon_color_outline"], config.esp.weapon_color_outline);

    save_color(root["esp_ammo_color"], config.esp.ammo_color);
    save_color(root["esp_ammo_color_outline"], config.esp.ammo_color_outline);
    save_color(root["esp_ammo_bar_text_color"], config.esp.ammo_bar_text_color);
    save_color(root["esp_ammo_text_color_outline"], config.esp.ammo_text_color_outline);

    save_color(root["esp_flags_color"], config.esp.flags_color);
    save_color(root["esp_flags_text_color_outline"], config.esp.flags_text_color_outline);


    save_color(root["esp_skeleton_color"], config.esp.skeleton_color);
    save_color(root["esp_skeleton_outline_color"], config.esp.skeleton_outline_color);

    save_color(root["esp_snapline_color"], config.esp.snapline_color);
    save_color(root["esp_snapline_outline_color"], config.esp.snapline_outline_color);
    save_color(root["esp_distance_color"], config.esp.flags_color);
    save_color(root["esp_distance_text_color_outline"], config.esp.flags_text_color_outline);
    // local
    root["local_chamsvis"] = config.local.chamsvis;
    root["local_chamsinvis"] = config.local.chamsinvis;
    root["local_chamsragdoll"] = config.local.chamsragdoll;
    root["local_chamsviewmodelvis"] = config.local.chamsviewmodelvis;
    root["local_chamsviewmodelinvis"] = config.local.chamsviewmodelinvis;
    root["local_chamsweaponvis"] = config.local.chamsweaponvis;
    root["local_chamsweaponinvis"] = config.local.chamsweaponinvis;
    root["local_chamstype"] = config.local.chamstype;

    save_color(root["local_chamsvis_color"], config.local.chamsvis_color);
    save_color(root["local_chamsinvis_color"], config.local.chamsinvis_color);
    save_color(root["local_chamsviewmodelvis_color"], config.local.chamsviewmodelvis_color);
    save_color(root["local_chamsviewmodelinvis_color"], config.local.chamsviewmodelinvis_color);
    save_color(root["local_chamsweaponvis_color"], config.local.chamsweaponvis_color);
    save_color(root["local_chamsweaponinvis_color"], config.local.chamsweaponinvis_color);

    // DROPPED WEAPONS
    root["world_weapon"] = config.world.weapon;
    root["world_weapon_name"] = config.world.weaponname;
    root["world_weapon_icon"] = config.world.weaponicon;
    root["world_weapon_chamsvis"] = config.world.chamsweaponvis;
    root["world_weapon_chamsinvis"] = config.world.chamsweaponinvis;
    root["world_weapon_chams_type"] = config.world.weapontypechams;
    save_color(root["world_chamsweaponvis_color"], config.world.chamsweaponvis_color);
    save_color(root["world_chamsweaponinvis_color"], config.world.chamsweaponinvis_color);
    root["world_granades"] = config.world.granades;
    // EXTRA
    for (int i = 0; i < 6; ++i)
        root["extra_removals"][i] = config.extra.removals[i];

    root["extra_model"] = config.extra.model;
    root["extra_model_ct"] = config.extra.model_ct;
    root["extra_model_t"] = config.extra.model_t;

    root["extra_custommodel"] = config.extra.custommodel;
    root["extra_custommodel_ct"] = config.extra.custommodel_ct;
    root["extra_custommodel_t"] = config.extra.custommodel_t;
    root["extra_handswitch"] = config.extra.handswitch;

    root["extra_thirdperson"] = config.extra.thirdperson;
    root["extra_thirdpersondist"] = config.extra.thirdpersondist;
    root["extra_thirdpersonalpha"] = config.extra.thirdpersonalpha;
    root["extra_thirdperson_keybind"] = config.extra.thirdperson_keybind;
    root["extra_thirdperson_keybind_style"] = config.extra.thirdperson_keybind_style;
    root["extra_m_selected"] = config.extra.m_selected;
    root["extra_m_selected_ct"] = config.extra.m_selected_ct;
    root["extra_m_selected_knife"] = config.extra.m_selected_knife;
    root["extra_m_selected_knife_t"] = config.extra.m_selected_knife_t;
    root["extra_m_selected_knife_ct"] = config.extra.m_selected_knife_ct;
    root["extra_knifestatrack"] = config.extra.knifestatrack;
    root["extra_custom_models"] = config.extra.custom_models;
    root["extra_custom_models_t"] = config.extra.custom_models_t;
    root["extra_custom_models_ct"] = config.extra.custom_models_ct;

    root["extra_knife_changer"] = config.extra.knife_changer;
    root["extra_knife_changer_ct"] = config.extra.knife_changer_ct;
    root["extra_knife_changer_t"] = config.extra.knife_changer_t;

    root["extra_m_selected_weapon"] = config.extra.m_selected_weapon;
    root["extra_seed"] = config.extra.seed;


    root["extra_nade_traces"] = config.extra.nade_traces;
    root["extra_nade_camera"] = config.extra.nade_camera;
    root["extra_hit_effect"] = config.extra.hit_effect;

    root["extra_desiredfov"] = config.extra.desiredfov;
    root["extra_desired_fov_zoom_level_1"] = config.extra.desired_fov_zoom_level_1;
    root["extra_desired_fov_zoom_level_2"] = config.extra.desired_fov_zoom_level_2;
    root["extra_viewmodel"] = config.extra.viewmodel;
    root["extra_viewmodel_fov"] = config.extra.viewmodel_fov;
    root["extra_viewmodel_offset_x"] = config.extra.viewmodel_offset_x;
    root["extra_viewmodel_offset_y"] = config.extra.viewmodel_offset_y;
    root["extra_viewmodel_offset_z"] = config.extra.viewmodel_offset_z;
    root["extra_overrideview"] = config.extra.overrideview;
    root["extra_aspect_ratio"] = config.extra.aspect_ratio;
    root["extra_trashtalk"] = config.extra.trashtalk;
    root["extra_auto_accept"] = config.extra.auto_accept;
    root["extra_sniper_crosshair"] = config.extra.sniper_cross;
    nlohmann::json skin_array = nlohmann::json::array();

    for (int i = 0; i < 100; ++i) {
        nlohmann::json skin_json;
        skin_json["skins_paint_kit"] = config.extra.m_skin_settings[i].m_paint_kit;
        skin_json["skins_item_def_index"] = config.extra.m_skin_settings[i].m_item_def_index;
        skin_json["skins_prev_skin"] = config.extra.m_skin_settings[i].m_prev_skin;

        skin_array.push_back(skin_json);
    }

    root["skin_settings"] = skin_array;


    // World Modulation
    root["worldmod_ambience"] = config.world_modulation.ambience;
    root["worldmod_exposure"] = config.world_modulation.exposure;
    root["worldmod_sparkeffects"] = config.world_modulation.sparkeffects;
    save_color(root["worldmod_sky_color"], config.world_modulation.sky_clr);
    save_color(root["worldmod_sun_color"], config.world_modulation.sun_clr);
    save_color(root["worldmod_clouds_color"], config.world_modulation.clouds_clr);
    save_color(root["worldmod_props_color"], config.world_modulation.props_clr);
    save_color(root["worldmod_world_color"], config.world_modulation.world_clr);
    save_color(root["worldmod_lighting_color"], config.world_modulation.lighting_clr);

    root["worldmod_fog_modulation"] = config.world_modulation.fog_modulation;
    save_color(root["worldmod_fog_color"], config.world_modulation.fog_clr);
    root["worldmod_fog_start"] = config.world_modulation.fog_start;
    root["worldmod_fog_end"] = config.world_modulation.fog_end;


    // Movement
    root["movement_bunnyhop"] = config.movement.bunnyhop;
    root["movement_auto_strafe"] = config.movement.auto_strafe;
    root["movement_air_strafe"] = config.movement.air_strafe;
    root["movement_air_duck"] = config.movement.air_duck;
    root["movement_walkbot"] = config.movement.walkbot;
    root["movement_autopeek"] = config.movement.autopeek;
    root["movement_autopeek_keybind"] = config.movement.autopeek_keybind;
    root["movement_autopeek_keybind_style"] = config.movement.autopeek_keybind_style;

    // GUI Settings
    root["gui_watermark"] = config.gui.watermark;
    for (int i = 0; i < 7; ++i)
        root["gui_indicators"][i] = config.gui.indicators[i];
    root["gui_hitlog"] = config.gui.hitlog;
    root["gui_misslog"] = config.gui.misslog;
    root["gui_chatlogs"] = config.gui.misslog;
    root["gui_hitlog_position"] = config.gui.hitlog_position;
    root["gui_hsound"] = config.gui.hsound;
    root["gui_hsoundvol"] = config.gui.hsoundvol;
    root["gui_ksound"] = config.gui.ksound;
    root["gui_ksoundvol"] = config.gui.ksoundvol;
    root["gui_animspeed"] = config.gui.animspeed;
    root["gui_menu_opacity"] = config.gui.menu_opacity;
    ofstream file(filename);
    if (file.is_open())
    {
        file << root.dump(4); // Красивое форматирование
        file.close();
        // Успешно
        g_notification->AddNotificationTopLeft(reinterpret_cast<const char*>(ICON_FA_CHECK), "Config Successfuly saved");
    }
    else
    {
        // Ошибка открытия файла
        g_notification->AddNotificationTopLeft(reinterpret_cast<const char*>(ICON_FA_TIMES), "Config Failed to save");
    }
}
inline void LoadConfig(const string& filename)
{
    ifstream file(filename);
    if (file.is_open())
    {
        json root;
        file >> root;
        file.close();

        config.ragebot.enable = root.value("ragebot_enable", false);
        config.ragebot.doubletap = root.value("ragebot_doubletap", false);
        config.ragebot.baim = root.value("ragebot_baim", false);
        config.ragebot.team_check = root.value("ragebot_team_check", true);
        config.ragebot.maximum_fov = root.value("ragebot_maximum_fov", 180.f);
        config.ragebot.silent_aim = root.value("ragebot_silent_aim", false);
        config.ragebot.automatic_fire = root.value("ragebot_automatic_fire", false);
        config.ragebot.auto_stop = root.value("ragebot_auto_stop", false);
        config.ragebot.auto_scope = root.value("ragebot_auto_scope", false);
        config.ragebot.group_type = root.value("ragebot_group_type", 0);
        config.ragebot.min_damage = root.value("ragebot_min_damage", 0);
        config.ragebot.min_damage_keybind = root.value("ragebot_min_damage_keybind", 0);
        config.ragebot.min_damage_keybind_style = root.value("ragebot_min_damage_keybind_style", 0);
        config.ragebot.min_damage_override = root.value("ragebot_min_damage_override", 0);
        config.ragebot.force_shoot = root.value("ragebot_force_shoot", 0);
        config.ragebot.force_shoot_style = root.value("ragebot_force_shoot_style", 0);
        config.ragebot.hit_chance = root.value("ragebot_hit_chance", 0);
        config.ragebot.hit_chance_keybind = root.value("ragebot_hit_chance_keybind", 0);
        config.ragebot.hit_chance_keybind_style = root.value("ragebot_hit_chance_keybind_style", 0);
        config.ragebot.hit_chance_override = root.value("ragebot_hit_chance_override", 0);
        if (root.contains("ragebot_hitboxes") && root["ragebot_hitboxes"].is_array()) {
            for (int i = 0; i < 6; ++i) {
                const auto& arr = root["ragebot_hitboxes"];
                if (i < arr.size() && arr[i].is_boolean())
                    config.ragebot.m_hitboxes[i] = arr[i].get<bool>();
                else
                    config.ragebot.m_hitboxes[i] = false;
            }
        }
        else {
            std::fill(std::begin(config.ragebot.m_hitboxes), std::end(config.ragebot.m_hitboxes), false);
        }
        if (root.contains("ragebot_multi_points") && root["ragebot_multi_points"].is_array()) {
            for (int i = 0; i < 6; ++i) {
                const auto& arr = root["ragebot_multi_points"];
                if (i < arr.size() && arr[i].is_boolean())
                    config.ragebot.m_multi_points[i] = arr[i].get<bool>();
                else
                    config.ragebot.m_multi_points[i] = false;
            }
        }
        else {
            std::fill(std::begin(config.ragebot.m_multi_points), std::end(config.ragebot.m_multi_points), false);
        }
        config.ragebot.multipointscale = root.value("ragebot_multipointscale", 0);
        // Antimaims
        config.antiaim.enable = root.value("antiaim_enabled", false);
        config.antiaim.pitch = root.value("antiaim_pitch", 0);
        config.antiaim.pitch_item = root.value("antiaim_pitch_item", 0);
        config.antiaim.fpitchup = root.value("antiaim_fpitchup", false);
        config.antiaim.fpitchdown = root.value("antiaim_fpitchdown", false);
        config.antiaim.yaw = root.value("antiaim_yaw", 0);
        config.antiaim.yaw_item = root.value("antiaim_yaw_item", 0);
        config.antiaim.hideshots = root.value("antiaim_hideshots", false);
        config.antiaim.manualleft = root.value("antiaim_manualleft", false);
        config.antiaim.manualleft_keybind = root.value("antiaim_manualleft_keybind", 0);
        config.antiaim.manualleft_keybind_style = root.value("antiaim_manualleft_keybind_style", 0);
        config.antiaim.manualright = root.value("antiaim_manualright", false);
        config.antiaim.manualright_keybind = root.value("antiaim_manualright_keybind", 0);
        config.antiaim.manualright_keybind_style = root.value("antiaim_manualright_keybind_style", 0);
        config.antiaim.manualback = root.value("antiaim_manualback", false);
        config.antiaim.manualback_keybind = root.value("antiaim_manualback_keybind", 0);
        config.antiaim.manualback_keybind_style = root.value("antiaim_manualback_keybind_style", 0);


        // Legitbot

        // Pistols
        config.legit.enable = root.value("legit_enable", false);
        config.legit.draw_fov = root.value("legit_draw_fov", false);
        config.legit.triggerbot = root.value("legit_triggerbot", false);
        config.legit.triggerbot_keybind = root.value("legit_triggerbot_keybind", 0);
        config.legit.triggerbot_keybind_style = root.value("legit_triggerbot_keybind_style", 0);
        config.legit.rcs = root.value("legit_rcs", false);
        config.legit.team_check = root.value("legit_team_check", false);
        config.legit.aimlock = root.value("legit_aimlock", false);
        config.legit.aimlock_keybind = root.value("legit_aimlock_keybind", 0);
        config.legit.aimlock_keybind_style = root.value("legit_aimlock_keybind_style", 0);

        // Pistols
        config.legit.fov_pistols = root.value("legit_fov_pistols", 2.5f);
        config.legit.smooth_pistols = root.value("legit_smooth_pistols", 5.0f);
        config.legit.delay_pistols = root.value("legit_delay_pistols", 0.0f);
        config.legit.rcs_smooth_pistols = root.value("legit_rcs_smooth_pistols", 50);
        config.legit.rcs_fov_pistols = root.value("legit_rcs_fov_pistols", 5);
        config.legit.rcs_after_shots_pistols = root.value("legit_rcs_after_shots_pistols", 1);
        config.legit.triggerbot_deley_before_shoot_pistols = root.value("triggerbot_delay_before_shoot_pistols", 0);
        config.legit.triggerbot_delay_between_shots_pistols = root.value("triggerbot_delay_between_shots_pistols", 0);
        config.legit.triggerbot_hitchance_pistols = root.value("triggerbot_hitchance_pistols", 0);



        // Heavy
        config.legit.fov_heavy = root.value("legit_fov_heavy", 2.5f);
        config.legit.smooth_heavy = root.value("legit_smooth_heavy", 5.0f);
        config.legit.delay_heavy = root.value("legit_delay_heavy", 0.0f);
        config.legit.rcs_smooth_heavy = root.value("legit_rcs_smooth_heavy", 50);
        config.legit.rcs_fov_heavy = root.value("legit_rcs_fov_heavy", 5);
        config.legit.rcs_after_shots_heavy = root.value("legit_rcs_after_shots_heavy", 1);
        config.legit.triggerbot_deley_before_shoot_heavy = root.value("triggerbot_delay_before_shoot_heavy", 0);
        config.legit.triggerbot_delay_between_shots_heavy = root.value("triggerbot_delay_between_shots_heavy", 0);
        config.legit.triggerbot_hitchance_heavy = root.value("triggerbot_hitchance_heavy", 0);

        // Rifles
        config.legit.fov_rifles = root.value("legit_fov_rifles", 2.5f);
        config.legit.smooth_rifles = root.value("legit_smooth_rifles", 5.0f);
        config.legit.delay_rifles = root.value("legit_delay_rifles", 0.0f);
        config.legit.rcs_smooth_rifles = root.value("legit_rcs_smooth_rifles", 50);
        config.legit.rcs_fov_rifles = root.value("legit_rcs_fov_rifles", 5);
        config.legit.rcs_after_shots_rifles = root.value("legit_rcs_after_shots_rifles", 1);
        config.legit.triggerbot_deley_before_shoot_rifles = root.value("triggerbot_delay_before_shoot_rifles", 0);
        config.legit.triggerbot_delay_between_shots_rifles = root.value("triggerbot_delay_between_shots_rifles", 0);
        config.legit.triggerbot_hitchance_rifles = root.value("triggerbot_hitchance_rifles", 0);

        // Auto Sniper
        config.legit.fov_auto = root.value("legit_fov_auto", 2.5f);
        config.legit.smooth_auto = root.value("legit_smooth_auto", 5.0f);
        config.legit.delay_auto = root.value("legit_delay_auto", 0.0f);
        config.legit.rcs_smooth_auto = root.value("legit_rcs_smooth_auto", 50);
        config.legit.rcs_fov_auto = root.value("legit_rcs_fov_auto", 5);
        config.legit.rcs_after_shots_auto = root.value("legit_rcs_after_shots_auto", 1);
        config.legit.triggerbot_deley_before_shoot_auto = root.value("triggerbot_delay_before_shoot_auto", 0);
        config.legit.triggerbot_delay_between_shots_auto = root.value("triggerbot_delay_between_shots_auto", 0);
        config.legit.triggerbot_hitchance_auto = root.value("triggerbot_hitchance_auto", 0);

        // Scout
        config.legit.fov_scout = root.value("legit_fov_scout", 2.5f);
        config.legit.smooth_scout = root.value("legit_smooth_scout", 5.0f);
        config.legit.delay_scout = root.value("legit_delay_scout", 0.0f);
        config.legit.rcs_smooth_scout = root.value("legit_rcs_smooth_scout", 50);
        config.legit.rcs_fov_scout = root.value("legit_rcs_fov_scout", 5);
        config.legit.rcs_after_shots_scout = root.value("legit_rcs_after_shots_scout", 1);
        config.legit.triggerbot_deley_before_shoot_scout = root.value("triggerbot_delay_before_shoot_scout", 0);
        config.legit.triggerbot_delay_between_shots_scout = root.value("triggerbot_delay_between_shots_scout", 0);
        config.legit.triggerbot_hitchance_scout = root.value("triggerbot_hitchance_scout", 0);


        // Deagle
        config.legit.fov_deagle = root.value("legit_fov_deagle", 2.5f);
        config.legit.smooth_deagle = root.value("legit_smooth_deagle", 5.0f);
        config.legit.delay_deagle = root.value("legit_delay_deagle", 0.0f);
        config.legit.rcs_smooth_deagle = root.value("legit_rcs_smooth_deagle", 50);
        config.legit.rcs_fov_deagle = root.value("legit_rcs_fov_deagle", 5);
        config.legit.rcs_after_shots_deagle = root.value("legit_rcs_after_shots_deagle", 1);
        config.legit.triggerbot_deley_before_shoot_deagle = root.value("triggerbot_delay_before_shoot_deagle", 0);
        config.legit.triggerbot_delay_between_shots_deagle = root.value("triggerbot_delay_between_shots_deagle", 0);
        config.legit.triggerbot_hitchance_deagle = root.value("triggerbot_hitchance_deagle", 0);





        // ESP Settings
        config.esp.enable = root.value("esp_enable", false);

        config.esp.show_box = root.value("esp_show_box", false);
        config.esp.box_outline = root.value("esp_box_outline", false);

        config.esp.show_health = root.value("esp_show_health", false);
        config.esp.hp_bar_text = root.value("esp_hp_bar_text", true);
        config.esp.hp_bar_text_outline = root.value("esp_hp_bar_text_outline", false);
        config.esp.health_outline = root.value("esp_health_outline", true);

        config.esp.show_name = root.value("esp_show_name", false);
        config.esp.name_outline = root.value("esp_name_outline", false);

        config.esp.show_weapon = root.value("esp_show_weapon", false);
        config.esp.weapon_name = root.value("esp_weapon_name", false);
        config.esp.weapon_icon = root.value("esp_weapon_icon", false);
        config.esp.weapon_name_outline = root.value("esp_weapon_name_outline", false);

        config.esp.show_skeleton = root.value("esp_show_skeleton", false);
        config.esp.skeleton_outline = root.value("esp_skeleton_outline", false);

        config.esp.show_ammo = root.value("esp_show_ammo", false);
        config.esp.ammo_outline = root.value("esp_ammo_outline", true);
        config.esp.ammo_bar_text = root.value("esp_ammo_bar_text", true);
        config.esp.ammo_bar_text_outline = root.value("esp_ammo_bar_text_outline", false);


        config.esp.show_flags = root.value("esp_show_flags", false);
        config.esp.flags_outline = root.value("esp_flags_outline", false);

        config.esp.show_snapline = root.value("esp_show_snapline", false);
        config.esp.snapline_outline = root.value("esp_snapline_outline", false);

        config.esp.boxtype = root.value("esp_boxtype", 0);
        config.esp.snaplinetype = root.value("esp_snaplinetype", 0);
        config.esp.skeletontype = root.value("esp_skeletontype", 0);

        config.esp.hp_bar_width = root.value("esp_hp_bar_width", 2.f);
        config.esp.ammo_bar_width = root.value("esp_ammo_bar_width", 2.f);

        config.esp.chamsvis = root.value("esp_chamsvis", false);
        config.esp.chamsinvis = root.value("esp_chamsinvis", false);
        config.esp.chamsragdoll = root.value("esp_chamsragdoll", false);

        config.esp.chamstypevis = root.value("esp_chamstypevis", 1);
        config.esp.chamstypeinvis = root.value("esp_chamstypeinvis", 1);

        load_color(root["esp_chamsvis_color"], config.esp.chamsvis_color);
        load_color(root["esp_chamsinvis_color"], config.esp.chamsinvis_color);

        load_color(root["esp_box_color"], config.esp.box_color);
        load_color(root["esp_box_outline_color"], config.esp.box_outline_color);

        load_color(root["esp_health_color"], config.esp.health_color);
        load_color(root["esp_health_color_outline"], config.esp.health_color_outline);
        load_color(root["esp_health_text_color_outline"], config.esp.health_text_color_outline);
        load_color(root["esp_hp_bar_text_color"], config.esp.hp_bar_text_color);

        load_color(root["esp_name_color"], config.esp.name_color);
        load_color(root["esp_name_color_outline"], config.esp.name_color_outline);

        load_color(root["esp_weapon_color"], config.esp.weapon_color);
        load_color(root["esp_weapon_color_outline"], config.esp.weapon_color_outline);

        load_color(root["esp_ammo_color"], config.esp.ammo_color);
        load_color(root["esp_ammo_color_outline"], config.esp.ammo_color_outline);
        load_color(root["esp_ammo_bar_text_color"], config.esp.ammo_bar_text_color);
        load_color(root["esp_ammo_text_color_outline"], config.esp.ammo_text_color_outline);

        load_color(root["esp_distance_color"], config.esp.flags_color);
        load_color(root["esp_distance_text_color_outline"], config.esp.flags_text_color_outline);

        load_color(root["esp_skeleton_color"], config.esp.skeleton_color);
        load_color(root["esp_skeleton_outline_color"], config.esp.skeleton_outline_color);

        load_color(root["esp_snapline_color"], config.esp.snapline_color);
        load_color(root["esp_snapline_outline_color"], config.esp.snapline_outline_color);
        // Local
        config.local.chamsvis = root.value("local_chamsvis", false);
        config.local.chamsinvis = root.value("local_chamsinvis", false);
        config.local.chamsragdoll = root.value("local_chamsragdoll", false);
        config.local.chamsviewmodelvis = root.value("local_chamsviewmodelvis", false);
        config.local.chamsviewmodelinvis = root.value("local_chamsviewmodelinvis", false);
        config.local.chamsweaponvis = root.value("local_chamsweaponvis", false);
        config.local.chamsweaponinvis = root.value("local_chamsweaponinvis", false);
        config.local.chamstype = root.value("local_chamstype", 0);

        load_color(root["local_chamsvis_color"], config.local.chamsvis_color);
        load_color(root["local_chamsinvis_color"], config.local.chamsinvis_color);
        load_color(root["local_chamsviewmodelvis_color"], config.local.chamsviewmodelvis_color);
        load_color(root["local_chamsviewmodelinvis_color"], config.local.chamsviewmodelinvis_color);
        load_color(root["local_chamsweaponvis_color"], config.local.chamsweaponvis_color);
        load_color(root["local_chamsweaponinvis_color"], config.local.chamsweaponinvis_color);

        // DROPPED WEAPONS
        config.world.weapon = root.value("world_weapon", false);
        config.world.weaponname = root.value("world_weapon_name", false);
        config.world.weaponicon = root.value("world_weapon_icon", false);
        config.world.chamsweaponvis = root.value("world_weapon_chamsvis", false);
        config.world.chamsweaponinvis = root.value("world_weapon_chamsinvis", false);
        config.world.weapontypechams = root.value("world_weapon_chams_type", 0);
        config.world.granades = root.value("world_granades", false);
        load_color(root["world_chamsweaponvis_color"], config.world.chamsweaponvis_color);
        load_color(root["world_chamsweaponinvis_color"], config.world.chamsweaponinvis_color);

        // EXTRA
        if (root.contains("extra_removals") && root["extra_removals"].is_array()) {
            for (int i = 0; i < 6; ++i) {
                const auto& arr = root["extra_removals"];
                if (i < arr.size() && arr[i].is_boolean())
                    config.extra.removals[i] = arr[i].get<bool>();
                else
                    config.extra.removals[i] = false; // безопасная инициализация
            }
        }
        else {
            std::fill(std::begin(config.extra.removals), std::end(config.extra.removals), false);
        }
        config.extra.model = root.value("extra_model", false);
        config.extra.model_ct = root.value("extra_model_ct", false);
        config.extra.model_t = root.value("extra_model_t", false);

        config.extra.custommodel = root.value("extra_custommodel", false);
        config.extra.handswitch = root.value("extra_handswitch", false);
        config.extra.thirdperson = root.value("extra_thirdperson", false);
        config.extra.thirdpersondist = root.value("extra_thirdpersondist", 70);
        config.extra.thirdpersonalpha = root.value("extra_thirdpersonalpha", 255);
        config.extra.thirdperson_keybind = root.value("extra_thirdperson_keybind", 0);
        config.extra.thirdperson_keybind_style = root.value("extra_thirdperson_keybind_style", 0);
        config.extra.m_selected = root.value("extra_m_selected", 0);
        config.extra.m_selected_ct = root.value("extra_m_selected_ct", 0);
        config.extra.m_selected_knife = root.value("extra_m_selected_knife", 0);
        config.extra.m_selected_knife_t = root.value("extra_m_selected_knife_t", 0);
        config.extra.m_selected_knife_ct = root.value("extra_m_selected_knife_ct", 0);

        config.extra.knifestatrack = root.value("extra_knifestatrack", 0);
        config.extra.custom_models = root.value("extra_custom_models", false);
        config.extra.custom_models_ct = root.value("extra_custom_models_ct", false);
        config.extra.custom_models_t = root.value("extra_custom_models_t", false);

        config.extra.knife_changer = root.value("extra_knife_changer", false);
        config.extra.knife_changer_ct = root.value("extra_knife_changer_ct", false);
        config.extra.knife_changer_t = root.value("extra_knife_changer_t", false);

        config.extra.m_selected_weapon = root.value("extra_m_selected_weapon", 0);
        config.extra.seed = root.value("extra_seed", 0);


        config.extra.nade_traces = root.value("extra_nade_traces", false);
        config.extra.nade_camera = root.value("extra_nade_camera", false);
        config.extra.hit_effect = root.value("extra_hit_effect", false);

        config.extra.desiredfov = root.value("extra_desiredfov", 90.f);
        config.extra.desired_fov_zoom_level_1 = root.value("extra_desired_fov_zoom_level_1", 90.f);
        config.extra.desired_fov_zoom_level_2 = root.value("extra_desired_fov_zoom_level_2", 90.f);
        config.extra.viewmodel = root.value("extra_viewmodel", false);
        config.extra.viewmodel_fov = root.value("extra_viewmodel_fov", 0);
        config.extra.viewmodel_offset_x = root.value("extra_viewmodel_offset_x", 0);
        config.extra.viewmodel_offset_y = root.value("extra_viewmodel_offset_y", 0);
        config.extra.viewmodel_offset_z = root.value("extra_viewmodel_offset_z", 0);
        config.extra.trashtalk = root.value("extra_trashtalk", false);
        config.extra.overrideview = root.value("extra_overrideview", false);
        config.extra.aspect_ratio = root.value("extra_aspect_ratio", 3.0f);
        config.extra.auto_accept = root.value("extra_auto_accept", false);
        config.extra.sniper_cross = root.value("extra_sniper_crosshair", false);
        if (root.contains("skin_settings") && root["skin_settings"].is_array()) {
            const auto& skin_array = root["skin_settings"];
            int count = std::min<int>(skin_array.size(), 100);

            for (int i = 0; i < count; ++i) {
                const auto& skin_json = skin_array[i];
                config.extra.m_skin_settings[i].m_paint_kit = skin_json.value("skins_paint_kit", 0);
                config.extra.m_skin_settings[i].m_item_def_index = skin_json.value("skins_item_def_index", 0);
                config.extra.m_skin_settings[i].m_prev_skin = skin_json.value("skins_prev_skin", -1);
            }
        }

        // World Modulation
        config.world_modulation.ambience = root.value("worldmod_ambience", false);
        config.world_modulation.exposure = root.value("worldmod_exposure", 70);
        config.world_modulation.sparkeffects = root.value("worldmod_sparkeffects", false);
        load_color(root["worldmod_sky_color"], config.world_modulation.sky_clr);
        load_color(root["worldmod_sun_color"], config.world_modulation.sun_clr);
        load_color(root["worldmod_clouds_color"], config.world_modulation.clouds_clr);
        load_color(root["worldmod_props_color"], config.world_modulation.props_clr);
        load_color(root["worldmod_world_color"], config.world_modulation.world_clr);
        load_color(root["worldmod_lighting_color"], config.world_modulation.lighting_clr);

        config.world_modulation.fog_modulation = root.value("worldmod_fog_modulation", false);
        load_color(root["worldmod_fog_color"], config.world_modulation.fog_clr);
        config.world_modulation.fog_start = root.value("worldmod_fog_start", 0);
        config.world_modulation.fog_end = root.value("worldmod_fog_end", 0);


        // Movement
        config.movement.bunnyhop = root.value("movement_bunnyhop", false);
        config.movement.auto_strafe = root.value("movement_auto_strafe", false);
        config.movement.air_strafe = root.value("movement_air_strafe", false);
        config.movement.air_duck = root.value("movement_air_duck", false);
        config.movement.walkbot = root.value("movement_walkbot", false);
        config.movement.autopeek = root.value("movement_autopeek", false);
        config.movement.autopeek_keybind = root.value("movement_autopeek_keybind", 0);
        config.movement.autopeek_keybind_style = root.value("movement_autopeek_keybind_style", 0);

        // GUI Settings
        config.gui.watermark = root.value("gui_watermark", false);
        if (root.contains("gui_indicators") && root["gui_indicators"].is_array()) {
            for (int i = 0; i < 7; ++i) {
                const auto& arr = root["gui_indicators"];
                if (i < arr.size() && arr[i].is_boolean())
                    config.gui.indicators[i] = arr[i].get<bool>();
                else
                    config.gui.indicators[i] = false; // безопасная инициализация
            }
        }
        else {
            std::fill(std::begin(config.gui.indicators), std::end(config.gui.indicators), false);
        }
        config.gui.hitlog = root.value("gui_hitlog", false);
        config.gui.misslog = root.value("gui_misslog", false);
        config.gui.chatlogs = root.value("gui_chatlogs", false);
        config.gui.hitlog_position = root.value("gui_hitlog_position", 0);
        config.gui.hsound = root.value("gui_hsound", 0);
        config.gui.hsoundvol = root.value("gui_hsoundvol", 50);
        config.gui.ksound = root.value("gui_ksound", 0);
        config.gui.ksoundvol = root.value("gui_ksoundvol", 50);
        config.gui.animspeed = root.value("gui_animspeed", 100);
        config.gui.menu_opacity = root.value("gui_menu_opacity", 120);
        g_notification->AddNotificationTopLeft(reinterpret_cast<const char*>(ICON_FA_CHECK), "Config Successfuly load");
    }
    else
    {
        // Сообщение: ошибка загрузки
        g_notification->AddNotificationTopLeft(reinterpret_cast<const char*>(ICON_FA_TIMES), "Config Failed to load");
    }
}

inline void updatepos()
{
    viewX = config.extra.viewmodel_offset_x;
    viewY = config.extra.viewmodel_offset_y;
    viewZ = config.extra.viewmodel_offset_z;
    viewFov = config.extra.viewmodel_fov;
}