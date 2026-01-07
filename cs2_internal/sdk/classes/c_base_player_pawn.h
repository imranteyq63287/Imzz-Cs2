#pragma once

#include "../utils/schema/schema.h"
#include "../utils/vector.h"
#include "../utils/typedefs/c_handle.h"
#include "../utils/virual.h"
#include "../classes/c_entity_instance.h"
#include "../classes/c_baseentity.h"

class c_econ_item_view
{
public:
	SCHEMA(inventory_image_rgba_requested, bool, "C_EconItemView", "m_bInventoryImageRgbaRequested");
	SCHEMA(inventory_image_tried_cache, bool, "C_EconItemView", "m_bInventoryImageTriedCache");
	SCHEMA(inventory_image_rgba_width, int32_t, "C_EconItemView", "m_nInventoryImageRgbaWidth");
	SCHEMA(inventory_image_rgba_height, int32_t, "C_EconItemView", "m_nInventoryImageRgbaHeight");
	SCHEMA(current_load_cached_file_name, char, "C_EconItemView", "m_szCurrentLoadCachedFileName");
	SCHEMA(restore_custom_material_after_precache, bool, "C_EconItemView", "m_bRestoreCustomMaterialAfterPrecache");
	SCHEMA(item_definition_index, uint16_t, "C_EconItemView", "m_iItemDefinitionIndex");
	SCHEMA(entity_quality, int32_t, "C_EconItemView", "m_iEntityQuality");
	SCHEMA(entity_level, uint32_t, "C_EconItemView", "m_iEntityLevel");
	SCHEMA(item_id, uint64_t, "C_EconItemView", "m_iItemID");
	SCHEMA(item_id_high, uint32_t, "C_EconItemView", "m_iItemIDHigh");
	SCHEMA(item_id_low, uint32_t, "C_EconItemView", "m_iItemIDLow");
	SCHEMA(account_id, uint32_t, "C_EconItemView", "m_iAccountID");
	SCHEMA(inventory_position, uint32_t, "C_EconItemView", "m_iInventoryPosition");
	SCHEMA(initialized, bool, "C_EconItemView", "m_bInitialized");
	SCHEMA(disallow_soc, bool, "C_EconItemView", "m_bDisallowSOC");
	SCHEMA(is_store_item, bool, "C_EconItemView", "m_bIsStoreItem");
	SCHEMA(is_trade_item, bool, "C_EconItemView", "m_bIsTradeItem");
	SCHEMA(entity_quantity, int32_t, "C_EconItemView", "m_iEntityQuantity");
	SCHEMA(rarity_override, int32_t, "C_EconItemView", "m_iRarityOverride");
	SCHEMA(quality_override, int32_t, "C_EconItemView", "m_iQualityOverride");
	SCHEMA(origin_override, int32_t, "C_EconItemView", "m_iOriginOverride");
	SCHEMA(client_flags, uint8_t, "C_EconItemView", "m_unClientFlags");
	SCHEMA(override_style, uint8_t, "C_EconItemView", "m_unOverrideStyle");
	SCHEMA(attribute_list, void*, "C_EconItemView", "m_AttributeList"); // CAttributeList
	SCHEMA(networked_dynamic_attributes, void*, "C_EconItemView", "m_NetworkedDynamicAttributes"); // CAttributeList
	SCHEMA(custom_name, char, "C_EconItemView", "m_szCustomName");
	SCHEMA(custom_name_override, char, "C_EconItemView", "m_szCustomNameOverride");
	SCHEMA(initialized_tags, bool, "C_EconItemView", "m_bInitializedTags");

	auto get_custom_paint_kit_index() 
	{
		return vfunc(this, int, 2);
	}

	c_econ_item_definition* get_static_data()
	{ 
		return vfunc(this, c_econ_item_definition*, 13);
	}
};

class c_attribute_container
{
public:
	SCHEMA_ARRAY(item, c_econ_item_view, "C_AttributeContainer", "m_Item");
	SCHEMA(external_item_provider_registered_token, int32_t, "C_AttributeContainer", "m_iExternalItemProviderRegisteredToken");
	SCHEMA(registered_as_item_id, uint64_t, "C_AttributeContainer", "m_ullRegisteredAsItemID");
};

class c_cs_player_item_services 
{
public:
	SCHEMA(has_defuser, bool, "CCSPlayer_ItemServices", "m_bHasDefuser");
	SCHEMA(has_helmet, bool, "CCSPlayer_ItemServices", "m_bHasHelmet");
	SCHEMA(has_heavy_armor, bool, "CCSPlayer_ItemServices", "m_bHasHeavyArmor");
};

class c_player_observer_services : public c_entity_instance
{
public:
	SCHEMA(observer_mode, uint8_t, "CPlayer_ObserverServices", "m_iObserverMode");
	SCHEMA(observer_target, c_base_handle, "CPlayer_ObserverServices", "m_hObserverTarget"); // CHandle<C_BaseEntity>
	SCHEMA(observer_last_mode, int, "CPlayer_ObserverServices", "m_iObserverLastMode"); // ObserverMode_t
	SCHEMA(forced_observer_mode, bool, "CPlayer_ObserverServices", "m_bForcedObserverMode");
	SCHEMA(observer_chase_distance, float, "CPlayer_ObserverServices", "m_flObserverChaseDistance");
	SCHEMA(observer_chase_distance_calc_time, float, "CPlayer_ObserverServices", "m_flObserverChaseDistanceCalcTime"); // GameTime_t

	c_base_handle get_observer_target() 
	{
		return observer_target();
	}
};

class c_player_camera_services
{
public:
	SCHEMA(cs_view_punch_angle, vec3_t, "CPlayer_CameraServices", "m_vecCsViewPunchAngle");
	SCHEMA(cs_view_punch_angle_tick, int, "CPlayer_CameraServices", "m_nCsViewPunchAngleTick"); // GameTick_t
	SCHEMA(cs_view_punch_angle_tick_ratio, float, "CPlayer_CameraServices", "m_flCsViewPunchAngleTickRatio");
	SCHEMA(player_fog, void*, "CPlayer_CameraServices", "m_PlayerFog"); // C_fogplayerparams_t
	SCHEMA(color_correction_ctrl, void*, "CPlayer_CameraServices", "m_hColorCorrectionCtrl"); // CHandle<C_ColorCorrection>
	SCHEMA(view_entity, c_base_handle*, "CPlayer_CameraServices", "m_hViewEntity"); // CHandle<C_BaseEntity>
	SCHEMA(tonemap_controller, void*, "CPlayer_CameraServices", "m_hTonemapController"); // CHandle<C_TonemapController2>
	SCHEMA(audio, void*, "CPlayer_CameraServices", "m_audio"); // audioparams_t
	SCHEMA(post_processing_volumes, void*, "CPlayer_CameraServices", "m_PostProcessingVolumes"); // C_NetworkUtlVectorBase<CHandle<C_PostProcessingVolume>>
	SCHEMA(old_player_z, float, "CPlayer_CameraServices", "m_flOldPlayerZ");
	SCHEMA(old_player_view_offset_z, float, "CPlayer_CameraServices", "m_flOldPlayerViewOffsetZ");
	SCHEMA(current_fog, void*, "CPlayer_CameraServices", "m_CurrentFog"); // fogparams_t
	SCHEMA(old_fog_controller, void*, "CPlayer_CameraServices", "m_hOldFogController"); // CHandle<C_FogController>
	SCHEMA(override_fog_color, bool, "CPlayer_CameraServices", "m_bOverrideFogColor"); // bool[5]
	SCHEMA(override_fog_color_value, c_color, "CPlayer_CameraServices", "m_OverrideFogColor"); // Color[5]
	SCHEMA(override_fog_start_end, bool*, "CPlayer_CameraServices", "m_bOverrideFogStartEnd"); // bool[5]
	SCHEMA(override_fog_start, float*, "CPlayer_CameraServices", "m_fOverrideFogStart"); // float32[5]
	SCHEMA(override_fog_end, float*, "CPlayer_CameraServices", "m_fOverrideFogEnd"); // float32[5]
	SCHEMA(active_post_processing_volume, void*, "CPlayer_CameraServices", "m_hActivePostProcessingVolume"); // CHandle<C_PostProcessingVolume>
	SCHEMA(demo_view_angles, vec3_t, "CPlayer_CameraServices", "m_angDemoViewAngles");
};

class c_player_weapon_services 
{
public:
	SCHEMA(my_weapons, c_network_utl_vector<c_base_handle>, "CPlayer_WeaponServices", "m_hMyWeapons"); // C_NetworkUtlVectorBase<CHandle<C_BasePlayerWeapon>>
	SCHEMA(active_weapon, c_base_handle, "CPlayer_WeaponServices", "m_hActiveWeapon"); // CHandle<C_BasePlayerWeapon>
	SCHEMA(last_weapon, c_base_handle, "CPlayer_WeaponServices", "m_hLastWeapon"); // CHandle<C_BasePlayerWeapon>
	SCHEMA(ammo_array, uint16_t*, "CPlayer_WeaponServices", "m_iAmmo"); // uint16_t[32]
	
	vec3_t shoot_pos()
	{
		vec3_t temp;

		vfunc(this, void, 24, &temp);

		return temp;
	}

	bool weapon_owns_this_type(const char* name);

	SCHEMA(next_attack, float, "CCSPlayer_WeaponServices", "m_flNextAttack"); // GameTime_t
	SCHEMA(is_looking_at_weapon, bool, "CCSPlayer_WeaponServices", "m_bIsLookingAtWeapon");
	SCHEMA(is_holding_look_at_weapon, bool, "CCSPlayer_WeaponServices", "m_bIsHoldingLookAtWeapon");
	SCHEMA(old_shoot_position_history_count, uint32_t, "CCSPlayer_WeaponServices", "m_nOldShootPositionHistoryCount");
	SCHEMA(old_input_history_count, uint32_t, "CCSPlayer_WeaponServices", "m_nOldInputHistoryCount");
};

class c_base_player_pawn : public c_base_entity
{
public:
	SCHEMA(camera_services, c_player_camera_services*, "C_BasePlayerPawn", "m_pCameraServices");
	SCHEMA(weapon_services, c_player_weapon_services*, "C_BasePlayerPawn", "m_pWeaponServices");
	SCHEMA(item_services, c_cs_player_item_services*, "C_BasePlayerPawn", "m_pItemServices");
	SCHEMA(observer_service, c_player_observer_services*, "C_BasePlayerPawn", "m_pObserverServices");
	SCHEMA(get_controller_handle, c_base_handle, ("C_BasePlayerPawn"), ("m_hController"));
};