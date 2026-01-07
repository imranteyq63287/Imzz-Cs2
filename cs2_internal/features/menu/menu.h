#pragma once
#include "../config_system.h"

#include <memory>
#include "../../includes.h"
#include "../menu/gui_base/Source.hpp"
static bool safemode = true;
extern bool save_requested;
static bool pending_safemode_change = false;
static bool safemode_toggle_value = true;
class c_menu_elements
{
public:
    bool tab(const char* label, const ImVec2& size_arg, const bool selected);
	void draw_multicombo(std::string name, int& variable, std::vector<std::string> labels);
}; 
inline float Lerp(float a, float b, float t) {
    return a + (b - a) * t;
}
static constexpr auto default_picker = ImGuiColorEditFlags_PickerHueBar | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoOptions | ImGuiColorEditFlags_NoTooltip | ImGuiColorEditFlags_NoSidePreview |
ImGuiColorEditFlags_AlphaPreview | ImGuiColorEditFlags_DisplayRGB;

static constexpr auto no_alpha_picker =
ImGuiColorEditFlags_PickerHueBar | ImGuiColorEditFlags_NoAlpha | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoOptions | ImGuiColorEditFlags_NoTooltip | ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_DisplayRGB;

inline const char* GunIcons(const std::string& weapon)
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
static const int TAB_COUNT = 6;
static const char* TAB_NAMES[TAB_COUNT] = { "Rage", "Legit", "Visuals", "Misc", "Skins", "Config" };
static int active_tab = 0;
static float tab_animation[TAB_COUNT] = { 0.0f, 0.0f, 0.0f };
static const char* flagOptions[] = { "Armor", "Scoped", "Air", "Ping" };
static const char* boxtypelist[] = { "Standart", "Corner" };
static const char* skeletontypelist[] = { "Standart", "Head Dot" };
static const char* painttype[] = { "Standart", "Pulse", "Rainbow" };
static const char* removalslist[] = { "Legs", "Scope Zoom", "Scope Overlay", "Smoke", "Intro", "Flash", "Hud"};
static const char* aim_checks_legit[] = { "Scope Check", "Flashed Check", "Smoked Check", "Local In Air Check", "Enemy In Air Check"};
static const char* snaplinelist[] = { "Bottom", "Top", "Center"};
static const char* weaponOptions[] = { "Pistols", "Deagle" , "Revolver", "SMG", "Heavy", "Rifles", "Auto-Sniper", "Scout", "AWP" };
static const char* weaponOptionsIcon[] = { GunIcons("weapon_usp_silencer"), GunIcons("weapon_deagle"), GunIcons("weapon_negev"), GunIcons("weapon_ak47"), GunIcons("weapon_ssg08"), GunIcons("weapon_scar20") };

static int* fov_vars[] = {
    &config.legit.fov_pistols,
    &config.legit.fov_deagle,
    &config.legit.fov_heavy,
    &config.legit.fov_rifles,
    &config.legit.fov_scout,
    &config.legit.fov_auto
};
static int* rcs_fov_vars[] = {
    &config.legit.rcs_fov_pistols,
    &config.legit.rcs_fov_deagle,
    &config.legit.rcs_fov_heavy,
    &config.legit.rcs_fov_rifles,
    &config.legit.rcs_fov_scout,
    &config.legit.rcs_fov_auto
};
static int* rcs_after_shot_vars[] = {
    &config.legit.rcs_after_shots_pistols,
    &config.legit.rcs_after_shots_deagle,
    &config.legit.rcs_after_shots_heavy,
    &config.legit.rcs_after_shots_rifles,
    &config.legit.rcs_after_shots_scout,
    &config.legit.rcs_after_shots_auto
};
static int* smooth_vars[] = {
    &config.legit.smooth_pistols,
    &config.legit.smooth_deagle,
    &config.legit.smooth_heavy,
    &config.legit.smooth_rifles,
    &config.legit.smooth_scout,
    &config.legit.smooth_auto
};

static int* delay_vars[] = {
    &config.legit.delay_pistols,
    &config.legit.delay_deagle,
    &config.legit.delay_heavy,
    &config.legit.delay_rifles,
    &config.legit.delay_scout,
    &config.legit.delay_auto
};

static float* humanize_vars[] = {
    &config.legit.humanize_float_pistols,
    &config.legit.humanize_float_deagle,
    &config.legit.humanize_float_heavy,
    &config.legit.humanize_float_rifles,
    &config.legit.humanize_float_scout,
    &config.legit.humanize_float_auto
};
static int* rcs_smooth[] = {
    &config.legit.rcs_smooth_pistols,
    &config.legit.rcs_smooth_deagle,
    &config.legit.rcs_smooth_heavy,
    &config.legit.rcs_smooth_rifles,
    &config.legit.rcs_smooth_scout,
    &config.legit.rcs_smooth_auto
};
static int* tb_delay_vars[] =
{
    &config.legit.triggerbot_delay_between_shots_pistols,
    &config.legit.triggerbot_delay_between_shots_deagle,
    &config.legit.triggerbot_delay_between_shots_heavy,
    &config.legit.triggerbot_delay_between_shots_rifles,
    &config.legit.triggerbot_delay_between_shots_scout,
    &config.legit.triggerbot_delay_between_shots_auto
};
static int* min_damage_vars[] =
{
    &config.legit.min_damage_pistols,
    &config.legit.min_damage_deagle,
    &config.legit.min_damage_heavy,
    &config.legit.min_damage_rifles,
    &config.legit.min_damage_scout,
    &config.legit.min_damage_auto
};
static int* min_damage_override_vars[] =
{
    &config.legit.min_damage_override_pistols,
    &config.legit.min_damage_override_deagle,
    &config.legit.min_damage_override_heavy,
    &config.legit.min_damage_override_rifles,
    &config.legit.min_damage_override_scout,
    &config.legit.min_damage_override_auto
};
static bool* rcs_enable_vars[] = {
    &config.legit.enable_rcs_pistols,
    &config.legit.enable_rcs_deagle,
    &config.legit.enable_rcs_heavy,
    &config.legit.enable_rcs_rifles,
    &config.legit.enable_rcs_scout,
    &config.legit.enable_rcs_auto
};

static int* tb_hc_vars[] =
{
    &config.legit.triggerbot_hitchance_pistols,
    &config.legit.triggerbot_hitchance_deagle,
    &config.legit.triggerbot_hitchance_heavy,
    &config.legit.triggerbot_hitchance_rifles,
    &config.legit.triggerbot_hitchance_scout,
    &config.legit.triggerbot_hitchance_auto
};

static int category_to_wg_index(weapon_category_t cat)
{
    if (cat == weapon_category_t::pistols)
        return 0;
    else if (cat == weapon_category_t::deagle)
        return 1;
    else if (cat == weapon_category_t::heavy)
        return 2;
    else if (cat == weapon_category_t::rifles)
        return 3;
    else if (cat == weapon_category_t::scout)
        return 4;
    else if (cat == weapon_category_t::auto_sniper)
        return 5;
    else
        return -1;
}
template<class T>
class c_subtab
{
private:
	int index{};
	std::vector<std::string> tabs{};

public:
	c_subtab(std::initializer_list<T>&& args)
		: tabs{ std::forward<std::initializer_list<T>>(args) } {
	}

	int get_index()
	{
		return index;
	}

	std::string* get_name()
	{
		return &tabs[index];
	}

	void render()
	{
        const size_t tabs_count = tabs.size();
        static std::vector<float> subtab_animation(tabs.size(), 0.0f);
        for (size_t i = 0; i < tabs_count; i++)
        {


            float target = (i == index) ? 1.0f : 0.0f;
            ImVec4 start = ImVec4(1.0f, 1.0f, 1.0f, 0.6f);

            ImVec4 end = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
            subtab_animation[i] = Lerp(subtab_animation[i], target, 0.1f);


            ImVec4 color = ImVec4(
                Lerp(start.x, end.x, subtab_animation[i]),
                Lerp(start.y, end.y, subtab_animation[i]),
                Lerp(start.z, end.z, subtab_animation[i]),
                Lerp(start.w, end.w, subtab_animation[i])
            );


            ImGui::PushStyleColor(ImGuiCol_Text, color);
            ImGui::PushFont(ssfi_small);

            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0, 0, 0, 0));
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0, 0, 0, 0));

            ImGui::PushFont(ssfi_small);
            ImGui::PushStyleColor(ImGuiCol_Text, color); // Твой анимированный цвет

            if (ImGui::Button(tabs[i].c_str(), ImVec2(0, 0)))
                index = i;

            ImGui::PopStyleColor(); // Text
            ImGui::PopFont();
            ImGui::PopStyleColor(3); // Button, Hovered, Active


            ImGui::PopFont();           // ⬅️ Обязательно сброс
            ImGui::PopStyleColor();

            if (i < tabs_count - 1)
                ImGui::SameLine();
        }
	}
};

namespace menu
{
    inline c_menu_elements* elements;
    inline bool was_opened{ };

    inline bool blur_active = false;
    inline std::chrono::steady_clock::time_point blur_start_time;
    inline bool blur_fading_out = false;

    inline float blur_radius = 20.0f;
    inline float rectroundui = 10.f;
    inline float rect_anim_progress = 0.0f; // от 0.0 до 1.0
    inline ImVec2 rect_size = ImVec2(150, 150);
    inline static ImColor rect_color = ImColor(255, 255, 255, 150);

    void legit_tab(ImVec2 pos, ImDrawList* draw);
    void rage_tab();
    void antiaim_tab();

    void skin_tab();
    void visuals_tab(ImVec2 pos, ImDrawList* draw);
    void misc_tab();

    void config_tab();

    inline auto& opened()
    {
        return was_opened;
    }

    void draw();

    void on_create_move();
}