#include "skins.h"

inline std::string wstring_to_cp1251(const wchar_t* wstr) {
    if (!wstr || !*wstr) return "";
    int len = WideCharToMultiByte(1251, 0, wstr, -1, nullptr, 0, nullptr, nullptr);
    std::string str(len - 1, 0);
    WideCharToMultiByte(1251, 0, wstr, -1, &str[0], len, nullptr, nullptr);
    return str;
}

//static bool is_paint_kit_for_weapon(c_paint_kit* paint_kit, const char* weapon_id)
//{
//    auto path = "panorama/images/econ/default_generated/" + std::string(weapon_id) + "_" + paint_kit->m_name + "_light_png.vtex_c";
//    return interfaces::file_system->exists(path.c_str(), "GAME");
//}
void c_skins::dump()
{
    auto item_schema = interfaces::client
        ->get_econ_item_system()
        ->get_econ_item_schema();

    if (!item_schema || !m_dumped_items.empty())
        return;

    const auto& items = item_schema->m_sorted_item_definition_map;
    const auto& paint_kits = item_schema->m_paint_kits;

    for (const auto& it : items)
    {
        auto* item = it.m_value;
        if (!item)
            continue;

        if (!item->is_weapon() && !item->is_knife(true) && !item->is_glove(true))
            continue;

        const char* base = item->m_item_base_name;
        if (!base || !*base)
            continue;

        auto wide = reinterpret_cast<const wchar_t*>(interfaces::localize->find_key(base));

        if (!wide || !*wide)
            continue;

        c_dumped_item dumped{};
        const std::string name_str = wstring_to_cp1251(wide);
        dumped.m_name = name_str.c_str();
        dumped.m_def_index = item->m_definition_index;
        dumped.m_rarity = item->m_item_rarity;
        dumped.m_unusual_item = item->is_knife(true) || item->is_glove(true);

        if (item->is_knife(true))
        {
            m_knives.m_dumped_knife_models.emplace_back(item->get_model_name());
            m_knives.m_dumped_knife_name.emplace_back(dumped.m_name);
        }

        if (item->is_glove(true))
        {
            m_gloves.m_dumped_glove_models.emplace_back(item->get_model_name());
            m_gloves.m_dumped_glove_name.emplace_back(dumped.m_name);
            m_gloves.m_glove_idxs.emplace_back(item->m_definition_index);
        }

        for (const auto& kit_it : paint_kits)
        {
            auto* kit = kit_it.m_value;
            if (!kit || kit->m_id == 0 || kit->m_id == 9001)
                continue;

            auto skin_wide = reinterpret_cast<const wchar_t*>(
                interfaces::localize->find_key(kit->m_description_tag)
                );
            if (!skin_wide || !*skin_wide)
                continue;

            c_dumped_skins skin{};
            const std::string skin_name_str = wstring_to_cp1251(skin_wide);
			skin.m_name = skin_name_str.c_str();
            skin.m_id = kit->m_id;
            skin.m_rarity = kit->m_rarity;

            dumped.m_dumped_skins.emplace_back(std::move(skin));
        }

        std::ranges::sort(
            dumped.m_dumped_skins,
            [](const c_dumped_skins& a, const c_dumped_skins& b)
            {
                return a.m_rarity > b.m_rarity;
            }
        );

        m_dumped_items.emplace_back(std::move(dumped));
    }
}

void c_skins::knifes(int stage)
{
    if (stage != 6)
        return;

    if (!g_ctx.local)
        return;

    auto weapon_services = g_ctx.local->weapon_services();
    if (!weapon_services)
        return;

    auto view_model_services = g_ctx.local->view_model_services();
    if (!view_model_services)
        return;

    auto view_model = reinterpret_cast<c_base_view_model*>(
        interfaces::entity_system->get_base_entity(
            view_model_services->view_model().get_entry_index()
        )
        );

    auto& weapons = weapon_services->my_weapons();

    for (int i = 0; i < weapons.m_size; ++i)
    {
        auto weapon = reinterpret_cast<c_econ_entity*>(
            interfaces::entity_system->get_base_entity(
                weapons.m_elements[i].get_entry_index()
            )
            );
        if (!weapon)
            continue;

        auto attr = weapon->attribute_manager();
        if (!attr)
            continue;

        auto item_view = attr->item();
        if (!item_view)
            continue;

        auto item_def = item_view->get_static_data();
        if (!item_def || !item_def->is_knife(false))
            continue;

        const bool is_ct = g_ctx.local->team_num() == 3;

        if ((is_ct && !config.extra.knife_changer_ct) ||
            (!is_ct && !config.extra.knife_changer_t))
            continue;

        const int selected =
            is_ct ? config.extra.m_selected_knife_ct
            : config.extra.m_selected_knife_t;

        if (selected < 0 ||
            selected >= (int)m_knives.m_dumped_knife_models.size())
            continue;

        const std::string& model = m_knives.m_dumped_knife_models[selected];
        const int def_index = m_knives.m_knife_idxs[selected];

        if (item_view->item_definition_index() == def_index)
            continue;

        std::string fixed_model = model;

        if (fixed_model.find(".vmdl") == std::string::npos)
            fixed_model += ".vmdl";

        if (
            fixed_model.find("karambit") != std::string::npos ||
            fixed_model.find("butterfly") != std::string::npos ||
            fixed_model.find("flip") != std::string::npos
            )
        {
            if (fixed_model.find("_ag2") == std::string::npos)
            {
                auto pos = fixed_model.rfind(".vmdl");
                if (pos != std::string::npos)
                    fixed_model.insert(pos, "_ag2");
            }
        }

        weapon->set_model(fixed_model.c_str());
        item_view->item_definition_index() = def_index;

        item_view->item_id_high() = -1;
        item_view->item_id_low() = -1;

        if (auto node = weapon->game_scene_node())
            node->set_mesh_group_mask(2);

        if (view_model && view_model->weapon() == weapon->get_handle())
        {
            view_model->set_model(fixed_model.c_str());

            if (auto vm_node = view_model->game_scene_node())
                vm_node->set_mesh_group_mask(2);
        }
    }
}

void c_skins::gloves(c_base_view_model* vm)
{
    return;
}

void c_skins::agents(int stage) {
    return;
}

c_models_data c_skins::custom_models(const std::string& folder)
{
    c_models_data data;

    if (!std::filesystem::is_directory(folder))
        return data;

    for (const auto& entry : std::filesystem::directory_iterator(folder))
    {
        if (!entry.is_directory())
            continue;

        const std::string category_name = entry.path().filename().string();
        std::unordered_map<std::string, std::vector<std::string>> packs;

        for (const auto& pack : std::filesystem::directory_iterator(entry.path()))
        {
            if (!pack.is_directory())
                continue;

            std::vector<std::string> model_list;

            for (const auto& model : std::filesystem::directory_iterator(pack.path()))
            {
                if (!model.is_regular_file())
                    continue;

                std::string model_name = model.path().filename().string();

                // только .vmdl_c
                if (!model_name.ends_with(".vmdl_c"))
                    continue;

                // пропускаем arms
                if (model_name.find("_arms") != std::string::npos ||
                    model_name.find("_arm") != std::string::npos)
                    continue;

                // убираем "_c"
                model_name.erase(model_name.size() - 2);
                model_list.emplace_back(model_name);
            }

            if (!model_list.empty())
            {
                const std::string pack_name = pack.path().filename().string();
                packs.emplace(pack_name, std::move(model_list));
            }
        }

        if (!packs.empty())
        {
            data.emplace(category_name, std::move(packs));
        }
    }

    return data;
}