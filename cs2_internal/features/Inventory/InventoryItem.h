#pragma once

#include <string>

enum class ItemType {
    Weapon,
    Knife
};

struct InventoryItem {
    ItemType type;
    bool active = false;

    int def_index = 0;              // 7 = AK-47, 507 = Karambit и т.д.
    std::string name;               // "AK-47 | Fire Serpent" или "Karambit | Doppler P4"

    int paint_kit = 0;
    float wear = 0.0001f;           // Factory New
    int seed = 661;
    int stattrak = -1;              // -1 = без, >0 = значение

    // “олько дл€ ножей
    int knife_model_idx = -1;       // индекс в g_skins->m_knives.m_dumped_knife_models
};