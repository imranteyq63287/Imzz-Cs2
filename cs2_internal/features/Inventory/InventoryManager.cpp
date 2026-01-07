#include "InventoryManager.h"

void InventoryManager::AddItem(const InventoryItem& item) {
    items.push_back(item);
}

void InventoryManager::RemoveItem(size_t index) {
    if (index < items.size()) {
        items.erase(items.begin() + index);
    }
}

void InventoryManager::SetActiveKnife(int index) {
    for (auto& item : items) {
        if (item.type == ItemType::Knife) {
            item.active = (&item == &items[index]);
        }
    }
}

void InventoryManager::SetActiveWeaponSkin(int def_index, int paint_kit) {
    for (auto& item : items) {
        if (item.type == ItemType::Weapon && item.def_index == def_index) {
            item.paint_kit = paint_kit;
            item.active = true;
            break;
        }
    }
}

InventoryItem* InventoryManager::GetActiveKnife() {
    for (auto& item : items) {
        if (item.type == ItemType::Knife && item.active) {
            return &item;
        }
    }
    return nullptr;
}

InventoryItem* InventoryManager::FindItem(int def_index, ItemType type) {
    for (auto& item : items) {
        if (item.def_index == def_index && item.type == type) {
            return &item;
        }
    }
    return nullptr;
}