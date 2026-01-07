#pragma once

#include "../utils/schema/schema.h"
#include "../interfaces/interfaces.h"
#include "../utils/vector.h"

class c_point_camera 
{
public:
	SCHEMA(fov, float, "C_PointCamera", "m_FOV");
	SCHEMA(resolution, float, "C_PointCamera", "m_Resolution");
	SCHEMA(fog_enable, bool, "C_PointCamera", "m_bFogEnable");
	SCHEMA(fog_color, c_color, "C_PointCamera", "m_FogColor");
	SCHEMA(fog_start, float, "C_PointCamera", "m_flFogStart");
	SCHEMA(fog_end, float, "C_PointCamera", "m_flFogEnd");
	SCHEMA(fog_max_density, float, "C_PointCamera", "m_flFogMaxDensity");
	SCHEMA(active, bool, "C_PointCamera", "m_bActive");
	SCHEMA(use_screen_aspect_ratio, bool, "C_PointCamera", "m_bUseScreenAspectRatio");
	SCHEMA(aspect_ratio, float, "C_PointCamera", "m_flAspectRatio");
	SCHEMA(no_sky, bool, "C_PointCamera", "m_bNoSky");
	SCHEMA(brightness, float, "C_PointCamera", "m_fBrightness");
	SCHEMA(z_far, float, "C_PointCamera", "m_flZFar");
	SCHEMA(z_near, float, "C_PointCamera", "m_flZNear");
	SCHEMA(can_hltv_use, bool, "C_PointCamera", "m_bCanHLTVUse");
	SCHEMA(align_with_parent, bool, "C_PointCamera", "m_bAlignWithParent");
	SCHEMA(dof_enabled, bool, "C_PointCamera", "m_bDofEnabled");
	SCHEMA(dof_near_blurry, float, "C_PointCamera", "m_flDofNearBlurry");
	SCHEMA(dof_near_crisp, float, "C_PointCamera", "m_flDofNearCrisp");
	SCHEMA(dof_far_crisp, float, "C_PointCamera", "m_flDofFarCrisp");
	SCHEMA(dof_far_blurry, float, "C_PointCamera", "m_flDofFarBlurry");
	SCHEMA(dof_tilt_to_ground, float, "C_PointCamera", "m_flDofTiltToGround");
	SCHEMA(target_fov, float, "C_PointCamera", "m_TargetFOV");
	SCHEMA(degrees_per_second, float, "C_PointCamera", "m_DegreesPerSecond");
	SCHEMA(is_on, bool, "C_PointCamera", "m_bIsOn");
	SCHEMA(next, c_point_camera*, "C_PointCamera", "m_pNext");
};