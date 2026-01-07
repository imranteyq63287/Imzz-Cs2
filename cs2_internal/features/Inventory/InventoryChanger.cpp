#include "InventoryChanger.h"

InventoryChanger g_inventory_changer;

void InventoryChanger::Initialize() {
    manager.items.clear();

    // Тестовый Karambit
    InventoryItem karambit;
    karambit.type = ItemType::Knife;
    karambit.def_index = 507;
    karambit.name = "Karambit | Doppler Phase 4";
    karambit.paint_kit = 415;           // Phase 4
    karambit.wear = 0.0001f;
    karambit.seed = 661;
    karambit.stattrak = -1;
    karambit.knife_model_idx = 507;       // ← замени на реальный индекс Karambit в твоём g_skins->m_knives
    karambit.active = true;
    manager.AddItem(karambit);

    // Тестовый AK-47
    InventoryItem ak;
    ak.type = ItemType::Weapon;
    ak.def_index = 7;
    ak.name = "AK-47 | Fire Serpent";
    ak.paint_kit = 180;
    ak.wear = 0.0001f;
    ak.seed = 661;
    ak.stattrak = 1337;
    ak.active = true;
    manager.AddItem(ak);

    debug_text(WHITE_COLOR, "[InventoryChanger] Loaded %zu items\n", manager.items.size());
}

void InventoryChanger::OnFrameStageNotify(int stage) {
    if (!enabled || stage != 6) return;

    auto local = g_ctx.local;
    if (!local) return;

    auto view_model_handle = local->view_model_services()->view_model();
    auto vm = reinterpret_cast<c_base_view_model*>(
        interfaces::entity_system->get_base_entity(view_model_handle.get_entry_index())
        );

    auto& weapons = local->weapon_services()->my_weapons();

    for (int i = 0; i < weapons.m_size; ++i) {
        auto weapon = reinterpret_cast<c_econ_entity*>(
            interfaces::entity_system->get_base_entity(weapons.m_elements[i].get_entry_index())
            );
        if (!weapon) continue;

        ApplyWeaponSkin(weapon);
        ApplyKnife(weapon, vm);
    }
}

void InventoryChanger::ApplyKnife(c_econ_entity* weapon, c_base_view_model* vm) {
    auto knife_item = manager.GetActiveKnife();
    if (!knife_item) return;

    auto item_view = weapon->attribute_manager()->item();
    auto item_def = item_view->get_static_data();
    if (!item_def || !item_def->is_knife(false)) return;

    std::string model_path = g_skins->m_knives.m_dumped_knife_models[knife_item->knife_model_idx];

    if (model_path.find(".vmdl") == std::string::npos) {
        model_path += ".vmdl";
    }
    if (model_path.find("karambit") != std::string::npos && model_path.find("_ag2") == std::string::npos) {
        size_t pos = model_path.rfind(".vmdl");
        if (pos != std::string::npos) {
            model_path.insert(pos, "_ag2");
        }
    }

    debug_text(WHITE_COLOR, "Knife path: %s | PaintKit: %d\n", model_path.c_str(), knife_item->paint_kit);

    weapon->set_model(model_path.c_str());
    item_view->item_definition_index() = knife_item->def_index;

    if (vm && vm->weapon() == weapon->get_handle()) {
        vm->set_model(model_path.c_str());
    }

    weapon->fallback_paint_kit() = knife_item->paint_kit;
    weapon->fallback_wear() = knife_item->wear;
    weapon->fallback_seed() = knife_item->seed;
    weapon->fallback_stat_trak() = knife_item->stattrak;

    item_view->item_id_high() = -1;
    item_view->item_id_low() = -1;

    uint64_t mask = 2ULL;
    if (auto scene = weapon->game_scene_node()) scene->set_mesh_group_mask(mask);
    if (vm) {
        if (auto vm_scene = vm->game_scene_node()) vm_scene->set_mesh_group_mask(mask);
    }
}

void InventoryChanger::ApplyWeaponSkin(c_econ_entity* weapon) {
    auto item_view = weapon->attribute_manager()->item();
    auto def_index = item_view->item_definition_index();

    auto skin_item = manager.FindItem(def_index, ItemType::Weapon);
    if (!skin_item || !skin_item->active) return;

    weapon->fallback_paint_kit() = skin_item->paint_kit;
    weapon->fallback_wear() = skin_item->wear;
    weapon->fallback_seed() = skin_item->seed;
    weapon->fallback_stat_trak() = skin_item->stattrak;

    item_view->item_id_high() = -1;
    item_view->item_id_low() = -1;

    debug_text(WHITE_COLOR, "Weapon %d skin applied: PaintKit %d\n", def_index, skin_item->paint_kit);
}