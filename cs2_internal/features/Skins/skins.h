#pragma once
#include "../../includes.h"
#include "../config_system.h"
class c_dumped_skins {
public:
    const char* m_name = "";
    int m_id = 0;
    int m_rarity = 0;
};
using c_models_data = std::unordered_map< std::string, std::unordered_map< std::string, std::vector< std::string > > >;

class c_dumped_item {
public:
    const char* m_name = "";
    uint16_t m_def_index = 0;
    int m_rarity = 0;
    bool m_unusual_item = false;
    std::vector<c_dumped_skins> m_dumped_skins{};
    c_dumped_skins* m_selected_skin = nullptr;
};

enum e_material_magic_numbers : std::uint32_t {
    MATERIAL_NUMBER_GLOVES = 0xF143B82A
};

class c_material_record {
public:
    std::uint32_t m_unknown;
    e_material_magic_numbers m_magic_number;
    std::uint32_t m_handle;
    std::uint32_t m_type;
};

class c_material_data {
public:
    c_material_record* m_records;
    std::int32_t m_records_size;
};

class c_skins {
public:
    std::vector<const char*> m_paintkit_names;
    std::vector<int> m_paintkit_ids;
    std::vector<c_dumped_item> m_dumped_items;
    struct {
        std::vector<const char*> m_dumped_agent_models;
        std::vector<const char*> m_dumped_agent_name;
    } m_agents;
    std::string m_model_path_t;
    std::string m_model_path_ct;


    struct {
        std::array<int, 20> m_knife_idxs = { 500, 503, 505, 506, 507, 508, 509, 512, 514, 515, 516, 517, 518, 519, 520, 521, 522, 523, 525, 526 };
        std::vector<const char*> m_dumped_knife_models;
        std::vector<const char*> m_dumped_knife_name;
    } m_knives;

    struct {
        std::vector<const char*> m_dumped_glove_models;
        std::vector<const char*> m_dumped_glove_name;
        std::vector<int> m_glove_idxs;
    } m_gloves;
    void dump();
    c_models_data custom_models(const std::string& folder);
    void gloves(c_base_view_model* vm);
    void agents(int stage);
    void knifes(int stage);
};

inline const auto g_skins = std::make_unique<c_skins>();
enum e_skin_config_t : short
{
    CONFIG_DEAGLE,
    CONFIG_DUAL_BERETTAS,
    CONFIG_FIVE_SEVEN,
    CONFIG_GLOCK,
    CONFIG_AK47,
    CONFIG_AUG,
    CONFIG_AWP,
    CONFIG_FAMAS,
    CONFIG_G3SG1,
    CONFIG_GALIL,
    CONFIG_M249,
    CONFIG_M4A4,
    CONFIG_MAC10,
    CONFIG_P90,
    CONFIG_UMP,
    CONFIG_MP5SD,
    CONFIG_XM1024,
    CONFIG_BIZON,
    CONFIG_MAG7,
    CONFIG_NEGEV,
    CONFIG_SAWEDOFF,
    CONFIG_TEC9,
    CONFIG_TASER,
    CONFIG_HKP2000,
    CONFIG_MP7,
    CONFIG_MP9,
    CONFIG_NOVA,
    CONFIG_P250,
    CONFIG_SCAR20,
    CONFIG_SG553,
    CONFIG_SSG08,
    CONFIG_M4A1_S,
    CONFIG_USP_S,
    CONFIG_CZ75,
    CONFIG_REVOLVER,
    CONFIG_BAYONET,
    CONFIG_KNIFECSS,
    CONFIG_KNIFEFLIP,
    CONFIG_KNIFEGUT,
    CONFIG_KNIFEKARAMBIT,
    CONFIG_KNIFEM9BAYONET,
    CONFIG_KNIFETACTICAL,
    CONFIG_KNIFEFALCHION,
    CONFIG_KNIFESURVIVALBOWIE,
    CONFIG_KNIFEBUTTERFLY,
    CONFIG_KNIFEPUSH,
    CONFIG_KNIFECORD,
    CONFIG_KNIFECANIS,
    CONFIG_KNIFEURSUS,
    CONFIG_KNIFEGYPSYJACKKNIFE,
    CONFIG_KNIFEOUTDOOR,
    CONFIG_KNIFESTILETTO,
    CONFIG_KNIFEWIDOWMAKER,
    CONFIG_KNIFESKELETON,
    CONFIG_KNIFEKUKRI,


    CONFIG_UNKNOWN = 0xFFFFFFFFFF,
};
inline short get_skin_config(short item_def)
{
    switch (item_def)
    {
    case WEAPON_DEAGLE:
        return CONFIG_DEAGLE;
    case WEAPON_DUAL_BERETTAS:
        return CONFIG_DUAL_BERETTAS;
    case WEAPON_FIVE_SEVEN:
        return CONFIG_FIVE_SEVEN;
    case WEAPON_GLOCK:
        return CONFIG_GLOCK;
    case WEAPON_AK47:
        return CONFIG_AK47;
    case WEAPON_AUG:
        return CONFIG_AUG;
    case WEAPON_AWP:
        return CONFIG_AWP;
    case WEAPON_FAMAS:
        return CONFIG_FAMAS;
    case WEAPON_G3SG1:
        return CONFIG_G3SG1;
    case WEAPON_GALIL:
        return CONFIG_GALIL;
    case WEAPON_M249:
        return CONFIG_M249;
    case WEAPON_M4A4:
        return CONFIG_M4A4;
    case WEAPON_MAC10:
        return CONFIG_MAC10;
    case WEAPON_P90:
        return CONFIG_P90;
    case WEAPON_UMP:
        return CONFIG_UMP;
    case WEAPON_MP5SD:
        return CONFIG_MP5SD;
    case WEAPON_XM1024:
        return CONFIG_XM1024;
    case WEAPON_BIZON:
        return CONFIG_BIZON;
    case WEAPON_MAG7:
        return CONFIG_MAG7;
    case WEAPON_NEGEV:
        return CONFIG_NEGEV;
    case WEAPON_SAWEDOFF:
        return CONFIG_SAWEDOFF;
    case WEAPON_TEC9:
        return CONFIG_TEC9;
    case WEAPON_TASER:
        return CONFIG_TASER;
    case WEAPON_HKP2000:
        return CONFIG_HKP2000;
    case WEAPON_MP7:
        return CONFIG_MP7;
    case WEAPON_MP9:
        return CONFIG_MP9;
    case WEAPON_NOVA:
        return CONFIG_NOVA;
    case WEAPON_P250:
        return CONFIG_P250;
    case WEAPON_SCAR20:
        return CONFIG_SCAR20;
    case WEAPON_SG553:
        return CONFIG_SG553;
    case WEAPON_SSG08:
        return CONFIG_SSG08;
    case WEAPON_M4A1_S:
        return CONFIG_M4A1_S;
    case WEAPON_USP_S:
        return CONFIG_USP_S;
    case WEAPON_CZ75:
        return CONFIG_CZ75;
    case WEAPON_REVOLVER:
        return CONFIG_REVOLVER;
    case WEAPON_BAYONET:
        return CONFIG_BAYONET;
    case WEAPON_KNIFECSS:
        return CONFIG_KNIFECSS;
    case WEAPON_KNIFEFLIP:
        return CONFIG_KNIFEFLIP;
    case WEAPON_KNIFEGUT:
        return CONFIG_KNIFEGUT;
    case WEAPON_KNIFEKARAMBIT:
        return CONFIG_KNIFEKARAMBIT;
    case WEAPON_KNIFEM9BAYONET:
        return CONFIG_KNIFEM9BAYONET;
    case WEAPON_KNIFETACTICAL:
        return CONFIG_KNIFETACTICAL;
    case WEAPON_KNIFEFALCHION:
        return CONFIG_KNIFEFALCHION;
    case WEAPON_KNIFESURVIVALBOWIE:
        return CONFIG_KNIFESURVIVALBOWIE;
    case WEAPON_KNIFEBUTTERFLY:
        return CONFIG_KNIFEBUTTERFLY;
    case WEAPON_KNIFEPUSH:
        return CONFIG_KNIFEPUSH;
    case WEAPON_KNIFECORD:
        return CONFIG_KNIFECORD;
    case WEAPON_KNIFECANIS:
        return CONFIG_KNIFECANIS;
    case WEAPON_KNIFEURSUS:
        return CONFIG_KNIFEURSUS;
    case WEAPON_KNIFEGYPSYJACKKNIFE:
        return CONFIG_KNIFEGYPSYJACKKNIFE;
    case WEAPON_KNIFEOUTDOOR:
        return CONFIG_KNIFEOUTDOOR;
    case WEAPON_KNIFESTILETTO:
        return CONFIG_KNIFESTILETTO;
    case WEAPON_KNIFEWIDOWMAKER:
        return CONFIG_KNIFEWIDOWMAKER;
    case WEAPON_KNIFESKELETON:
        return CONFIG_KNIFESKELETON;
    case WEAPON_KNIFEKUKRI:
        return CONFIG_KNIFEKUKRI;
    }

    return CONFIG_UNKNOWN;
}
