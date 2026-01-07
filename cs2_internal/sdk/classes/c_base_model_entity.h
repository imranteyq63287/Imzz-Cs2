#pragma once

#include "../utils/schema/schema.h"
#include "../interfaces/interfaces.h"
#include "../utils/vector.h"

class c_base_model_entity 
{
public:
	SCHEMA(render_component, void*, "C_BaseModelEntity", "m_CRenderComponent");
	SCHEMA(hitbox_component, void*, "C_BaseModelEntity", "m_CHitboxComponent");
	SCHEMA(last_hit_group, int, "C_BaseModelEntity", "m_LastHitGroup");
	SCHEMA(init_model_effects, bool, "C_BaseModelEntity", "m_bInitModelEffects");
	SCHEMA(is_static_prop, bool, "C_BaseModelEntity", "m_bIsStaticProp");
	SCHEMA(last_add_decal, int32_t, "C_BaseModelEntity", "m_nLastAddDecal");
	SCHEMA(decals_added, int32_t, "C_BaseModelEntity", "m_nDecalsAdded");
	SCHEMA(old_health, int32_t, "C_BaseModelEntity", "m_iOldHealth");
	SCHEMA(render_mode, int, "C_BaseModelEntity", "m_nRenderMode");
	SCHEMA(render_fx, int, "C_BaseModelEntity", "m_nRenderFX");
	SCHEMA(allow_fade_in_view, bool, "C_BaseModelEntity", "m_bAllowFadeInView");
	SCHEMA(clr_render, c_color, "C_BaseModelEntity", "m_clrRender");
	SCHEMA(vec_render_attributes, void*, "C_BaseModelEntity", "m_vecRenderAttributes");
	SCHEMA(render_to_cubemaps, bool, "C_BaseModelEntity", "m_bRenderToCubemaps");
	SCHEMA(no_interpolate, bool, "C_BaseModelEntity", "m_bNoInterpolate");
	SCHEMA(collision, void*, "C_BaseModelEntity", "m_Collision");
	SCHEMA(glow, void*, "C_BaseModelEntity", "m_Glow");
	SCHEMA(glow_backface_mult, float, "C_BaseModelEntity", "m_flGlowBackfaceMult");
	SCHEMA(fade_min_dist, float, "C_BaseModelEntity", "m_fadeMinDist");
	SCHEMA(fade_max_dist, float, "C_BaseModelEntity", "m_fadeMaxDist");
	SCHEMA(fade_scale, float, "C_BaseModelEntity", "m_flFadeScale");
	SCHEMA(shadow_strength, float, "C_BaseModelEntity", "m_flShadowStrength");
	SCHEMA(object_culling, uint8_t, "C_BaseModelEntity", "m_nObjectCulling");
	SCHEMA(add_decal, int32_t, "C_BaseModelEntity", "m_nAddDecal");
	SCHEMA(decal_position, vec3_t, "C_BaseModelEntity", "m_vDecalPosition");
	SCHEMA(decal_forward_axis, vec3_t, "C_BaseModelEntity", "m_vDecalForwardAxis");
	SCHEMA(decal_heal_blood_rate, float, "C_BaseModelEntity", "m_flDecalHealBloodRate");
	SCHEMA(decal_heal_height_rate, float, "C_BaseModelEntity", "m_flDecalHealHeightRate");
	SCHEMA(config_entities_propagate_material_decals_to, void*, "C_BaseModelEntity", "m_ConfigEntitiesToPropagateMaterialDecalsTo");
	SCHEMA(view_offset, vec3_t, "C_BaseModelEntity", "m_vecViewOffset");
	SCHEMA(client_alpha_property, void*, "C_BaseModelEntity", "m_pClientAlphaProperty");
	SCHEMA(client_override_tint, c_color, "C_BaseModelEntity", "m_ClientOverrideTint");
	SCHEMA(use_client_override_tint, bool, "C_BaseModelEntity", "m_bUseClientOverrideTint");
};