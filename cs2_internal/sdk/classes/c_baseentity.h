#pragma once

#include "../utils/schema/schema.h"

#include "c_entity_instance.h"
#include "c_game_scene_node.h"
#include "c_model.h"

class c_collision
{
public:
	SCHEMA(vec_mins, vec3_t, "CCollisionProperty", "m_vecMins");
	SCHEMA(vec_maxs, vec3_t, "CCollisionProperty", "m_vecMaxs");
	SCHEMA(solid_flags, int, "CCollisionProperty", "m_usSolidFlags");
	SCHEMA(solid_type, int, "CCollisionProperty", "m_nSolidType");
	SCHEMA(trigger_bloat, int, "CCollisionProperty", "m_triggerBloat");
	SCHEMA(surround_type, int, "CCollisionProperty", "m_nSurroundType");
	SCHEMA(collision_group, int, "CCollisionProperty", "m_CollisionGroup");
	SCHEMA(enable_physics, int, "CCollisionProperty", "m_nEnablePhysics");
	SCHEMA(bounding_radius, float, "CCollisionProperty", "m_flBoundingRadius");
	SCHEMA(specified_surrounding_mins, vec3_t, "CCollisionProperty", "m_vecSpecifiedSurroundingMins");
	SCHEMA(specified_surrounding_maxs, vec3_t, "CCollisionProperty", "m_vecSpecifiedSurroundingMaxs");
	SCHEMA(surrounding_maxs, vec3_t, "CCollisionProperty", "m_vecSurroundingMaxs");
	SCHEMA(surrounding_mins, vec3_t, "CCollisionProperty", "m_vecSurroundingMins");
	SCHEMA(capsule_center1, vec3_t, "CCollisionProperty", "m_vCapsuleCenter1");
	SCHEMA(capsule_center2, vec3_t, "CCollisionProperty", "m_vCapsuleCenter2");
	SCHEMA(capsule_radius, float, "CCollisionProperty", "m_flCapsuleRadius");

	std::uint16_t collision_mask() { return *reinterpret_cast<std::uint16_t*>(reinterpret_cast<std::uintptr_t>(this) + 0x38); }
};

class c_base_entity : public c_entity_instance 
{
public:
	SCHEMA(last_think_tick, int, "C_BaseEntity", "m_nLastThinkTick");
	SCHEMA(game_scene_node, c_game_scene_node*, "C_BaseEntity", "m_pGameSceneNode");
	SCHEMA(collision, c_collision*, "C_BaseEntity", "m_pCollision");
	SCHEMA(max_health, int, "C_BaseEntity", "m_iMaxHealth");
	SCHEMA(health, int, "C_BaseEntity", "m_iHealth");
	SCHEMA(life_state, uint8_t, "C_BaseEntity", "m_lifeState");
	SCHEMA(takes_damage, bool, "C_BaseEntity", "m_bTakesDamage");
	SCHEMA(take_damage_flags, int, "C_BaseEntity", "m_nTakeDamageFlags");
	SCHEMA(platform_type, int, "C_BaseEntity", "m_nPlatformType");
	SCHEMA(interpolation_frame, uint8_t, "C_BaseEntity", "m_ubInterpolationFrame");
	SCHEMA(scene_object_controller, c_base_handle, "C_BaseEntity", "m_hSceneObjectController");
	SCHEMA(no_interpolation_tick, int, "C_BaseEntity", "m_nNoInterpolationTick");
	SCHEMA(visibility_no_interpolation_tick, int, "C_BaseEntity", "m_nVisibilityNoInterpolationTick");
	SCHEMA(proxy_random_value, float, "C_BaseEntity", "m_flProxyRandomValue");
	SCHEMA(eflags, int, "C_BaseEntity", "m_iEFlags");
	SCHEMA(water_type, uint8_t, "C_BaseEntity", "m_nWaterType");
	SCHEMA(interpolate_even_without_model, bool, "C_BaseEntity", "m_bInterpolateEvenWithNoModel");
	SCHEMA(prediction_eligible, bool, "C_BaseEntity", "m_bPredictionEligible");
	SCHEMA(apply_layer_match_id_to_model, bool, "C_BaseEntity", "m_bApplyLayerMatchIDToModel");
	SCHEMA(layer_match_id, uint32_t, "C_BaseEntity", "m_tokLayerMatchID");
	SCHEMA(subclass_id, uint32_t, "C_BaseEntity", "m_nSubclassID");
	
	SCHEMA_WITH_OFFSET(GetVData, void*, "C_BaseEntity", "m_nSubclassID", 0x8);

	SCHEMA(simulation_tick, int, "C_BaseEntity", "m_nSimulationTick");
	SCHEMA(current_think_context, int, "C_BaseEntity", "m_iCurrentThinkContext");
	SCHEMA(think_functions, c_utl_vector<void*>, "C_BaseEntity", "m_aThinkFunctions");
	SCHEMA(disabled_context_thinks, bool, "C_BaseEntity", "m_bDisabledContextThinks");
	SCHEMA(anim_time, float, "C_BaseEntity", "m_flAnimTime");
	SCHEMA(simulation_time, float, "C_BaseEntity", "m_flSimulationTime");
	SCHEMA(scene_object_override_flags, uint8_t, "C_BaseEntity", "m_nSceneObjectOverrideFlags");
	SCHEMA(successfully_interpolated, bool, "C_BaseEntity", "m_bHasSuccessfullyInterpolated");
	SCHEMA(added_vars_to_interpolation, bool, "C_BaseEntity", "m_bHasAddedVarsToInterpolation");
	SCHEMA(render_even_without_successful_interpolation, bool, "C_BaseEntity", "m_bRenderEvenWhenNotSuccessfullyInterpolated");
	SCHEMA(interpolation_latch_dirty_flags, int, "C_BaseEntity", "m_nInterpolationLatchDirtyFlags");
	SCHEMA(list_entry, uint16_t, "C_BaseEntity", "m_ListEntry");
	SCHEMA(create_time, float, "C_BaseEntity", "m_flCreateTime");
	SCHEMA(speed, float, "C_BaseEntity", "m_flSpeed");
	SCHEMA(ent_client_flags, uint16_t, "C_BaseEntity", "m_EntClientFlags");
	SCHEMA(client_side_ragdoll, bool, "C_BaseEntity", "m_bClientSideRagdoll");
	SCHEMA(team_num, uint8_t, "C_BaseEntity", "m_iTeamNum");
	SCHEMA(spawn_flags, uint32_t, "C_BaseEntity", "m_spawnflags");
	SCHEMA(next_think_tick, int, "C_BaseEntity", "m_nNextThinkTick");
	SCHEMA(flags, int, "C_BaseEntity", "m_fFlags");
	SCHEMA(abs_velocity, vec3_t, "C_BaseEntity", "m_vecAbsVelocity");
	SCHEMA(velocity, vec3_t, "C_BaseEntity", "m_vecVelocity");
	SCHEMA(base_velocity, vec3_t, "C_BaseEntity", "m_vecBaseVelocity");
	SCHEMA(effect_entity, c_base_handle, "C_BaseEntity", "m_hEffectEntity");
	SCHEMA(owner_entity, c_base_handle, "C_BaseEntity", "m_hOwnerEntity");
	SCHEMA(move_collide, int, "C_BaseEntity", "m_MoveCollide");
	SCHEMA(move_type, int, "C_BaseEntity", "m_MoveType");
	SCHEMA(actual_move_type, int, "C_BaseEntity", "m_nActualMoveType");
	SCHEMA(water_level, float, "C_BaseEntity", "m_flWaterLevel");
	SCHEMA(effects, uint32_t, "C_BaseEntity", "m_fEffects");
	SCHEMA(ground_entity, c_base_handle, "C_BaseEntity", "m_hGroundEntity");
	SCHEMA(ground_body_index, int, "C_BaseEntity", "m_nGroundBodyIndex");
	SCHEMA(friction, float, "C_BaseEntity", "m_flFriction");
	SCHEMA(elasticity, float, "C_BaseEntity", "m_flElasticity");
	SCHEMA(gravity_scale, float, "C_BaseEntity", "m_flGravityScale");
	SCHEMA(time_scale, float, "C_BaseEntity", "m_flTimeScale");
	SCHEMA(animated_every_tick, bool, "C_BaseEntity", "m_bAnimatedEveryTick");
	SCHEMA(nav_ignore_until_time, float, "C_BaseEntity", "m_flNavIgnoreUntilTime");
	SCHEMA(think_handle, uint16_t, "C_BaseEntity", "m_hThink");
	SCHEMA(bbox_vis_flags, uint8_t, "C_BaseEntity", "m_fBBoxVisFlags");
	SCHEMA(predictable, bool, "C_BaseEntity", "m_bPredictable");
	SCHEMA(render_with_viewmodels, bool, "C_BaseEntity", "m_bRenderWithViewModels");
	SCHEMA(split_user_prediction_slot, int, "C_BaseEntity", "m_nSplitUserPlayerPredictionSlot");
	SCHEMA(first_predictable_command, int, "C_BaseEntity", "m_nFirstPredictableCommand");
	SCHEMA(last_predictable_command, int, "C_BaseEntity", "m_nLastPredictableCommand");
	SCHEMA(old_move_parent, c_base_handle, "C_BaseEntity", "m_hOldMoveParent");
	SCHEMA(predicted_script_floats, c_utl_vector<float>, "C_BaseEntity", "m_vecPredictedScriptFloats");
	SCHEMA(predicted_script_float_ids, c_utl_vector<int>, "C_BaseEntity", "m_vecPredictedScriptFloatIDs");
	SCHEMA(next_script_var_record_id, int, "C_BaseEntity", "m_nNextScriptVarRecordID");
	SCHEMA(angular_velocity, vec3_t, "C_BaseEntity", "m_vecAngVelocity");
	SCHEMA(data_change_event_ref, int, "C_BaseEntity", "m_DataChangeEventRef");
	SCHEMA(creation_tick, int, "C_BaseEntity", "m_nCreationTick");
	SCHEMA(anim_time_changed, bool, "C_BaseEntity", "m_bAnimTimeChanged");
	SCHEMA(simulation_time_changed, bool, "C_BaseEntity", "m_bSimulationTimeChanged");
	SCHEMA(unique_hammer_id, const char*, "C_BaseEntity", "m_sUniqueHammerID");
	SCHEMA(blood_type, int, "C_BaseEntity", "m_nBloodType");

	bool is_weapon()
	{
		return vmt::call_virtual<bool>(this, 162); 
	}

	void set_body_group() 
	{
		static auto fn = reinterpret_cast<void(__fastcall*)(void*, int, unsigned int)>(utils::find_pattern(g_modules.client, "85 D2 0F 88 ? ? ? ? 55 56 57"));

		fn(this, 0, 1);
	}

	c_hitboxsets* get_hitbox_set(unsigned int index) 
	{
		using fn_get_hitbox_set = std::int64_t(__fastcall*)(void*, unsigned int);
		static auto fn = reinterpret_cast<fn_get_hitbox_set>(utils::find_pattern(g_modules.client, "48 89 5C 24 ? 48 89 74 24 ? 57 48 81 EC ? ? ? ? 8B DA 48 8B F9 E8 ? ? ? ? 48 8B F0 48 85 C0 0F 84"));

		return reinterpret_cast<c_hitboxsets*>(fn(this, index));
	}

	void set_model(const char* model_name)
	{
		if (!model_name || !*model_name)
		{
			return;
		}

		static auto fn = reinterpret_cast<void* (__fastcall*)(void*, const char*)>(utils::find_pattern_rel(g_modules.client, "E8 ? ? ? ? 44 88 A6", 0x1)); // sign -> 06.06.2025;
		if (!fn)
		{
			std::cout << "[-] Pattern fail." << std::endl;
			return;
		}

		fn(this, model_name);
	}

	void update_subclass()
	{
		using fn_update_subclass = void(__fastcall*)(void*);
		static fn_update_subclass update_subclass = reinterpret_cast<fn_update_subclass>(utils::find_pattern(g_modules.client, "40 53 48 83 EC 30 48 8B 41 10 48 8B D9 8B 50 30"));

		update_subclass(this);
	}

	c_unkown_information* get_class_information()
	{
		if ((uintptr_t)this == 0xdddddddddddddddd)
			return {};
		
		return *(c_unkown_information**)(std::uintptr_t(this) + 0x10);
	}

	bool find_class(std::uint64_t name) 
	{
		try 
		{
			auto info = get_class_information();
			
			if (!info || !info->second_class_information || !info->second_class_information->main_class_information)
				return false;

			for (auto c = info->second_class_information->main_class_information; c != nullptr; c = c->next_class) 
			{
				if (fnv1a::hash_64(c->name_class) == name)
					return true;
			}

			return false;
		}
		catch (...)
		{
			std::cout << "[ERROR] find_class" << std::endl;

			return false;
		}
	}
};
class c_planted_c4 : public c_base_entity {
public:
	SCHEMA(is_bomb_ticking, bool, ("C_PlantedC4"), ("m_bBombTicking"));                                // 0xF90
	SCHEMA(get_bomb_site, int, ("C_PlantedC4"), ("m_nBombSite"));                                      // 0xF94
	SCHEMA(get_source_soundscape_hash, int, ("C_PlantedC4"), ("m_nSourceSoundscapeHash"));             // 0xF98
	SCHEMA(get_entity_spotted_state, void*, ("C_PlantedC4"), ("m_entitySpottedState"));                // 0xFA0
	SCHEMA(get_next_glow, float, ("C_PlantedC4"), ("m_flNextGlow"));                                   // 0xFB8
	SCHEMA(get_next_beep, float, ("C_PlantedC4"), ("m_flNextBeep"));                                   // 0xFBC
	SCHEMA(get_c4_blow_time, float, ("C_PlantedC4"), ("m_flC4Blow"));                                   // 0xFC0
	SCHEMA(get_cannot_be_defused, bool, ("C_PlantedC4"), ("m_bCannotBeDefused"));                      // 0xFC4
	SCHEMA(has_exploded, bool, ("C_PlantedC4"), ("m_bHasExploded"));                                   // 0xFC5
	SCHEMA(get_timer_length, float, ("C_PlantedC4"), ("m_flTimerLength"));                             // 0xFC8
	SCHEMA(is_being_defused, bool, ("C_PlantedC4"), ("m_bBeingDefused"));                              // 0xFCC
	SCHEMA(get_trigger_warning, float, ("C_PlantedC4"), ("m_bTriggerWarning"));                        // 0xFD0
	SCHEMA(get_explode_warning, float, ("C_PlantedC4"), ("m_bExplodeWarning"));                        // 0xFD4
	SCHEMA(is_c4_activated, bool, ("C_PlantedC4"), ("m_bC4Activated"));                                // 0xFD8
	SCHEMA(is_ten_sec_warning, bool, ("C_PlantedC4"), ("m_bTenSecWarning"));                           // 0xFD9
	SCHEMA(get_defuse_length, float, ("C_PlantedC4"), ("m_flDefuseLength"));                           // 0xFDC
	SCHEMA(get_defuse_countdown, float, ("C_PlantedC4"), ("m_flDefuseCountDown"));                     // 0xFE0
	SCHEMA(if_bomb_defused, bool, ("C_PlantedC4"), ("m_bBombDefused"));                                // 0xFE4
	SCHEMA(get_defuser_handle, c_base_handle, ("C_PlantedC4"), ("m_hBombDefuser"));                    // 0xFE8
	SCHEMA(get_control_panel_handle, c_base_handle, ("C_PlantedC4"), ("m_hControlPanel"));             // 0xFEC
	SCHEMA(get_attribute_manager, void*, ("C_PlantedC4"), ("m_AttributeManager"));                     // 0xFF0
	SCHEMA(get_defuser_multimeter, c_base_handle, ("C_PlantedC4"), ("m_hDefuserMultimeter"));          // 0x1498
	SCHEMA(get_next_radar_flash, float, ("C_PlantedC4"), ("m_flNextRadarFlashTime"));                  // 0x149C
	SCHEMA(is_radar_flash, bool, ("C_PlantedC4"), ("m_bRadarFlash"));                                  // 0x14A0
	SCHEMA(get_defuser_player, c_base_handle, ("C_PlantedC4"), ("m_pBombDefuser"));                    // 0x14A4
	SCHEMA(get_last_defuse_time, float, ("C_PlantedC4"), ("m_fLastDefuseTime"));                       // 0x14A8
	SCHEMA(get_prediction_owner, void*, ("C_PlantedC4"), ("m_pPredictionOwner"));                      // 0x14B0
	SCHEMA(get_c4_pos, vec3_t, ("C_PlantedC4"), ("m_vecC4ExplodeSpectatePos"));                         // 0x14B8
	SCHEMA(get_c4_ang, vec3_t, ("C_PlantedC4"), ("m_vecC4ExplodeSpectateAng"));                         // 0x14C4
	SCHEMA(get_spectate_duration, float, ("C_PlantedC4"), ("m_flC4ExplodeSpectateDuration"));          // 0x14D0
};