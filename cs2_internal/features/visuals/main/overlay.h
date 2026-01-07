#include "../../../includes.h"
#include "../../../sdk/entity/entity.h"
#include "../../../sdk/utils/render/render.h"
#include "../../config_system.h"
#include "../other/bone.hpp"
#include "../../menu/gui_base/Source.hpp"
class c_dropped_weapons {
public:
    c_cs_weapon_base* weapon;
    bool is_invalid;
    c_base_handle handle;
    vec3_t abs_origin;
    vec3_t mins;
    vec3_t maxs;
    std::string weapon_name;
    std::string szweapon_name;

    int item_index;
};
static struct sound_info_t {
    vec3_t player_origin;
    bool has_changed;
};
static std::vector<sound_info_t> sound_info;
inline const char* GunIcon(const std::string& weapon)
{
    std::map<std::string, const char*> gunIcons = {
      { "nova", "T" },
      { "ak47", "A" },
      { "awp", "C" },
      { "m4a4", "M" },
      { "deagle", "F" },
      { "mp9", "R" },
      { "ump45", "b" },
      { "glock", "g" },
    };

    /*if (sdk::m_local_pawn->m_team_num() == 2 && weapon.compare("weapon_knife"))
      return "]";
    else if (sdk::m_local_pawn->m_team_num() == 1 && weapon.compare("weapon_knife"))
      return "[";*/
    gunIcons[("weapon_knife")] = "]";
    gunIcons[("weapon_knife")] = "[";
    gunIcons[("weapon_deagle")] = "A";
    gunIcons[("weapon_elite")] = "B";
    gunIcons[("weapon_fiveseven")] = "C";
    gunIcons[("weapon_five-seven")] = "C";
    gunIcons[("weapon_glock")] = "D";
    gunIcons[("weapon_hkp2000")] = "E";
    gunIcons[("weapon_p2000")] = "E";
    gunIcons[("weapon_p250")] = "F";
    gunIcons[("weapon_usp_silencer")] = "G";
    gunIcons[("weapon_tec9")] = "H";
    gunIcons[("weapon_cz75a")] = "I";
    gunIcons[("weapon_revolver")] = "J";
    gunIcons[("weapon_mac10")] = "K";
    gunIcons[("weapon_mac-10")] = "K";
    gunIcons[("weapon_ump45")] = "L";
    gunIcons[("weapon_bizon")] = "M";
    gunIcons[("weapon_mp7")] = "N";
    gunIcons[("weapon_mp9")] = "O";
    gunIcons[("weapon_mp5sd")] = "x";
    gunIcons[("weapon_p90")] = "P";
    gunIcons[("weapon_galilar")] = "Q";
    gunIcons[("weapon_galil")] = "Q";
    gunIcons[("weapon_famas")] = "R";
    gunIcons[("weapon_m4a4")] = "S";
    gunIcons[("weapon_m4a1_silencer")] = "T";
    gunIcons[("weapon_m4a1")] = "T";
    gunIcons[("weapon_aug")] = "U";
    gunIcons[("weapon_sg556")] = "V";
    gunIcons[("weapon_ak47")] = "W";
    gunIcons[("weapon_g3sg1")] = "X";
    gunIcons[("weapon_scar20")] = "Y";
    gunIcons[("weapon_awp")] = "Z";
    gunIcons[("weapon_ssg08")] = "a";
    gunIcons[("weapon_ssg-08")] = "a";
    gunIcons[("weapon_xm1014")] = "b";
    gunIcons[("weapon_sawedoff")] = "c";
    gunIcons[("weapon_mag7")] = "d";
    gunIcons[("weapon_nova")] = "e";
    gunIcons[("weapon_negev")] = "f";
    gunIcons[("weapon_m249")] = "g";
    gunIcons[("weapon_taser")] = "h";
    gunIcons[("weapon_flashbang")] = "i";
    gunIcons[("weapon_hegrenade")] = "j";
    gunIcons[("weapon_smokegrenade")] = "k";
    gunIcons[("weapon_molotov")] = "l";
    gunIcons[("weapon_decoy")] = "m";
    gunIcons[("weapon_incgrenade")] = "n";
    gunIcons[("weapon_c4")] = "o";
    gunIcons[("defuse kit")] = "r";

    auto it = gunIcons.find(weapon);
    if (it != gunIcons.end())
    {
        return it->second;
    }

    return "";
}
inline std::string LocalWeaponToInternal(const std::string& localized_name)
{
    static const std::unordered_map<std::string, std::string> name_map = {
        { "Glock-18", "weapon_glock" },
        { "USP-S", "weapon_usp_silencer" },
        { "USP", "weapon_usp_silencer" },
        { "P2000", "weapon_hkp2000" },
        { "P250", "weapon_p250" },
        { "Desert Eagle", "weapon_deagle" },
        { "R8 Revolver", "weapon_revolver" },
        { "Five-SeveN", "weapon_fiveseven" },
        { "CZ75-Auto", "weapon_cz75a" },
        { "Dual Berettas", "weapon_elite" },
        { "Tec-9", "weapon_tec9" },

        { "Flashbang", "weapon_flashbang" },
        { "HE Grenade", "weapon_hegrenade" },
        { "Smoke Grenade", "weapon_smokegrenade" },
        { "Molotov", "weapon_molotov" },
        { "Incendiary Grenade", "weapon_incgrenade" },
        { "Decoy Grenade", "weapon_decoy" },

        { "C4 Explosive", "weapon_c4" },
        { "Взрывчатка C4", "weapon_c4" },
        { "Explosion c4", "weapon_c4" },
        { "Defuse Kit", "defuse kit" },
        { "Набор сапера", "defuse kit" },

        { "MAC-10", "weapon_mac10" },
        { "MP9", "weapon_mp9" },
        { "MP7", "weapon_mp7" },
        { "MP5-SD", "weapon_mp5sd" },
        { "UMP-45", "weapon_ump45" },
        { "P90", "weapon_p90" },
        { "PP-Bizon", "weapon_bizon" },

        { "Galil AR", "weapon_galilar" },
        { "FAMAS", "weapon_famas" },
        { "AK-47", "weapon_ak47" },
        { "M4A4", "weapon_m4a4" },
        { "M4A1-S", "weapon_m4a1_silencer" },
        { "AUG", "weapon_aug" },
        { "SG 553", "weapon_sg556" },

        { "SSG 08", "weapon_ssg08" },
        { "AWP", "weapon_awp" },
        { "SCAR-20", "weapon_scar20" },
        { "G3SG1", "weapon_g3sg1" },

        { "M249", "weapon_m249" },
        { "Negev", "weapon_negev" },

        { "Nova", "weapon_nova" },
        { "XM1014", "weapon_xm1014" },
        { "MAG-7", "weapon_mag7" },
        { "Sawed-Off", "weapon_sawedoff" },

        { "Taser", "weapon_taser" },
    };

    auto it = name_map.find(localized_name);
    if (it != name_map.end())
        return it->second;

    return ""; // Не найдено
}
static float current_y;
struct bbox_t {
	bool m_found = false;

	float x;
	float y;
	float width;
	float height;
};
namespace ESP
{
	bbox_t calculate_bbox(c_cs_player_pawn* entity);
	void esp();
	void draw_boxes(bbox_t box, vec3_t left_top, vec3_t bottom_right, c_cs_player_pawn* player_pawn, c_cs_player_controller* player_controller);
	void draw_names(bbox_t box, vec3_t left_top, vec3_t bottom_right, c_cs_player_pawn* player_pawn, c_cs_player_controller* player_controller);
	void draw_healthbar(bbox_t box, vec3_t left_top, vec3_t bottom_right, c_cs_player_pawn* player_pawn, c_cs_player_controller* player_controller);
	void draw_ammobar(bbox_t box, vec3_t left_top, vec3_t bottom_right, c_cs_player_pawn* player_pawn, c_cs_player_controller* player_controller);
	void draw_weapons(bbox_t box, vec3_t left_top, vec3_t bottom_right, c_cs_player_pawn* player_pawn, c_cs_player_controller* player_controller);
	void draw_flags(bbox_t box, vec3_t left_top, vec3_t bottom_right, c_cs_player_pawn* player_pawn, c_cs_player_controller* player_controller);
	void draw_skeleton(bbox_t box, vec3_t left_top, vec3_t bottom_right, c_cs_player_pawn* player_pawn, c_cs_player_controller* player_controller);
	void draw_sounds(bbox_t box, vec3_t left_top, vec3_t bottom_right, c_cs_player_pawn* player_pawn, c_cs_player_controller* player_controller);
	void draw_snaplines(bbox_t box, vec3_t left_top, vec3_t bottom_right, c_cs_player_pawn* player_pawn, c_cs_player_controller* player_controller);
	void draw_weapon_in_world(bbox_t box, vec3_t left_top, vec3_t bottom_right, c_cs_player_pawn* player_pawn, c_cs_player_controller* player_controller);
	void draw_c4(bbox_t box, vec3_t left_top, vec3_t bottom_right, c_cs_player_pawn* player_pawn, c_cs_player_controller* player_controller);
	void draw_chicken(bbox_t box, vec3_t left_top, vec3_t bottom_right, c_cs_player_pawn* player_pawn, c_cs_player_controller* player_controller);
	void draw_hostage(bbox_t box, vec3_t left_top, vec3_t bottom_right, c_cs_player_pawn* player_pawn, c_cs_player_controller* player_controller);
	void radarhack();
    std::vector<int> smooth_change(int from, int to);

    extern std::vector< c_dropped_weapons> m_dropped_weapons;
    inline bool is_planted;
    inline bool weapon_offset;
	inline int bomb_site;
    inline float time_to_explode;
}
static std::unordered_map<uintptr_t, float> animated_health_map;
static std::unordered_map<uintptr_t, float> animated_ammo_map;
static std::unordered_map<uintptr_t, float> esp_alpha_map;
static std::unordered_map<uintptr_t, float> animated_damage_map;
static std::unordered_map<uintptr_t, float> last_health_map;
static std::unordered_map<uintptr_t, float> start_health_map;

class c_specs
{
public:
    std::string name;
    ImTextureID image;
};
static bbox_t ESP::calculate_bbox(c_cs_player_pawn* player)
{
    bbox_t box{};
    if (!player)
    {
        debug_text(RED_COLOR, "calculate_bbox: player is null");
        return {};
    }

    auto scene_node = player->game_scene_node();
    if (!scene_node)
    {
        debug_text(RED_COLOR, "calculate_bbox: game_scene_node is null");
        return {};
    }

    vec3_t origin = scene_node->vec_abs_origin();
    if (origin.is_zero())
    {
        debug_text(RED_COLOR, "calculate_bbox: origin is zero");
        return {};
    }

    vec3_t bottom = origin - vec3_t(0.f, 0.f, 1.f);
    if (bottom.is_zero())
    {
        debug_text(RED_COLOR, "calculate_bbox: bottom is zero");
        return {};
    }

    vec3_t vec_maxs = player->collision()->vec_maxs();
    if (vec_maxs.is_zero())
    {
        debug_text(RED_COLOR, "calculate_bbox: vec_maxs is zero");
        return {};
    }

    vec3_t top = bottom + vec3_t(0.f, 0.f, vec_maxs.z);
    if (top.is_zero())
    {
        debug_text(RED_COLOR, "calculate_bbox: top is zero");
        return {};
    }

    vec3_t screen_points[2]{};
    if (!g_render->world_to_screen(top, screen_points[1]) || !g_render->world_to_screen(bottom, screen_points[0]))
    {
        return {};
    }

    box.height = std::fabsf((screen_points[1] - screen_points[0]).y + 6);
    if (!box.height)
    {
        debug_text(RED_COLOR, "calculate_bbox: box height is zero");
        return {};
    }

    box.width = box.height * 0.5f;
    if (!box.width)
    {
        debug_text(RED_COLOR, "calculate_bbox: box width is zero");
        return {};
    }

    box.x = screen_points[1].x - box.width * 0.5f;
    if (!box.x)
    {
        debug_text(RED_COLOR, "calculate_bbox: box x is zero");
        return {};
    }

    box.y = screen_points[1].y;
    if (!box.y)
    {
        debug_text(RED_COLOR, "calculate_bbox: box y is zero");
        return {};
    }

    box.m_found = true;
    return box;
}
static bbox_t calculate_bbox_weapon(c_base_entity* entity)
{
    bbox_t box{};

    if (!entity)
        return {};

    auto scene_node = entity->game_scene_node();
    if (!scene_node)
        return {};

    vec3_t origin = scene_node->vec_abs_origin();
    vec3_t mins = entity->collision()->vec_mins();
    vec3_t maxs = entity->collision()->vec_maxs();

    vec3_t points[] = {
        { origin.x + mins.x, origin.y + mins.y, origin.z + mins.z },
        { origin.x + mins.x, origin.y + maxs.y, origin.z + mins.z },
        { origin.x + maxs.x, origin.y + maxs.y, origin.z + mins.z },
        { origin.x + maxs.x, origin.y + mins.y, origin.z + mins.z },
        { origin.x + maxs.x, origin.y + maxs.y, origin.z + maxs.z },
        { origin.x + mins.x, origin.y + maxs.y, origin.z + maxs.z },
        { origin.x + mins.x, origin.y + mins.y, origin.z + maxs.z },
        { origin.x + maxs.x, origin.y + mins.y, origin.z + maxs.z },
    };

    vec3_t screen_points[8];
    for (int i = 0; i < 8; i++) {
        if (!g_render->world_to_screen(points[i], screen_points[i]))
            return {};
    }

    float left = screen_points[0].x;
    float top = screen_points[0].y;
    float right = screen_points[0].x;
    float bottom = screen_points[0].y;

    for (int i = 1; i < 8; i++) {
        if (screen_points[i].x < left) left = screen_points[i].x;
        if (screen_points[i].y < top) top = screen_points[i].y;
        if (screen_points[i].x > right) right = screen_points[i].x;
        if (screen_points[i].y > bottom) bottom = screen_points[i].y;
    }

    box.x = left;
    box.y = top;
    box.width = right - left;
    box.height = bottom - top;
    box.m_found = true;

    return box;
}
static std::vector<int> ESP::smooth_change(int from, int to)
{
    std::vector<int> values;
    int step = (to > from) ? 1 : -1;

    for (int i = from; i != to; i += step)
    {
        values.push_back(i);
    }
    values.push_back(to);
    return values;
}
static bool is_player_visible_esp(c_cs_player_pawn* local_pawn, c_cs_player_pawn* target_pawn)
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