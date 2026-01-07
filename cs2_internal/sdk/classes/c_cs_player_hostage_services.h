#pragma once

#include "c_baseentity.h"
#include "../utils/schema/schema.h"
#include "../interfaces/interfaces.h"
#include "../utils/vector.h"
#include "../utils/typedefs/c_utl_vector.h"

class c_cs_player_hostage_services 
{
public:
	SCHEMA(carried_hostage, c_base_handle*, "CCSPlayer_HostageServices", "m_hCarriedHostage");
	SCHEMA(carried_hostage_prop, c_base_handle*, "CCSPlayer_HostageServices", "m_hCarriedHostageProp");
};
