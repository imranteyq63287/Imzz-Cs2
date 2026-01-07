#pragma once

#include "InventoryItem.h"
#include <vector>

class InventoryManager {
public:
    std::vector<InventoryItem> items;

    void AddItem(const InventoryItem& item);
    void RemoveItem(size_t index);
    void SetActiveKnife(int index);
    void SetActiveWeaponSkin(int def_index, int paint_kit);

    InventoryItem* GetActiveKnife();
    InventoryItem* FindItem(int def_index, ItemType type);
};