#pragma once

#include "../utils/schema/schema.h"
#include "../interfaces/interfaces.h"
#include "../utils/vector.h"
#include "../utils/typedefs/c_utl_vector.h"
#include "sellback_purchase_entry.h"

class c_cs_player_buy_services 
{
public:
	SCHEMA(sellback_purchase_entries, c_utl_vector<sellback_purchase_entry_t>, "CCSPlayer_BuyServices", "m_vecSellbackPurchaseEntries");
};