#pragma once

#include "../utils/schema/schema.h"
#include "../interfaces/interfaces.h"
#include "../utils/vector.h"
#include "../utils/typedefs/c_utl_vector.h"

class c_point_client_ui_hud
{
public:
	SCHEMA(check_css_classes, bool, "C_PointClientUIHUD", "m_bCheckCSSClasses");
	SCHEMA(ignore_input, bool, "C_PointClientUIHUD", "m_bIgnoreInput");
	SCHEMA(width, float, "C_PointClientUIHUD", "m_flWidth");
	SCHEMA(height, float, "C_PointClientUIHUD", "m_flHeight");
	SCHEMA(dpi, float, "C_PointClientUIHUD", "m_flDPI");
	SCHEMA(interact_distance, float, "C_PointClientUIHUD", "m_flInteractDistance");
	SCHEMA(depth_offset, float, "C_PointClientUIHUD", "m_flDepthOffset");
	SCHEMA(owner_context, uint32_t, "C_PointClientUIHUD", "m_unOwnerContext");
	SCHEMA(horizontal_align, uint32_t, "C_PointClientUIHUD", "m_unHorizontalAlign");
	SCHEMA(vertical_align, uint32_t, "C_PointClientUIHUD", "m_unVerticalAlign");
	SCHEMA(orientation, uint32_t, "C_PointClientUIHUD", "m_unOrientation");
	SCHEMA(allow_interaction_from_all_scene_worlds, bool, "C_PointClientUIHUD", "m_bAllowInteractionFromAllSceneWorlds");
	SCHEMA(css_classes, c_utl_vector<const char*>*, "C_PointClientUIHUD", "m_vecCSSClasses"); 
};