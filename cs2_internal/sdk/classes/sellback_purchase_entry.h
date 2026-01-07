#pragma once

#include "../utils/schema/schema.h"
#include "../utils/typedefs/c_handle.h"

class sellback_purchase_entry_t 
{
public:
    SCHEMA(def_index, std::uint16_t, "SellbackPurchaseEntry_t", "m_unDefIdx");
    SCHEMA(cost, std::int32_t, "SellbackPurchaseEntry_t", "m_nCost");
    SCHEMA(prev_armor, std::int32_t, "SellbackPurchaseEntry_t", "m_nPrevArmor");
    SCHEMA(prev_helmet, bool, "SellbackPurchaseEntry_t", "m_bPrevHelmet");
    SCHEMA(item_handle, c_base_handle*, "SellbackPurchaseEntry_t", "m_hItem");
};