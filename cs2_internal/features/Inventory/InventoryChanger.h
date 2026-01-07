#pragma once

#include "InventoryManager.h"
#include "../Skins/skins.h"

class InventoryChanger {
public:
    InventoryManager manager;
    bool enabled = true;

    void Initialize();
    void OnFrameStageNotify(int stage);

private:
    void ApplyKnife(c_econ_entity* weapon, c_base_view_model* vm);
    void ApplyWeaponSkin(c_econ_entity* weapon);
};

extern InventoryChanger g_inventory_changer;