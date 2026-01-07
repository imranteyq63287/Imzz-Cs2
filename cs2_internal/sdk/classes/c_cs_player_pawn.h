#pragma once

#include "c_cs_player_pawn_base.h"
#include "c_base_player_weapon.h"
#include "c_player_movement_services.h"
#include "c_cs_player_hostage_services.h"
#include "c_cs_player_buy_services.h"
#include "c_cs_player_controller_action_tracking_services.h"


enum EHitGroupIndex : int
{
	HITGROUP_GENERIC = 0,
	HITGROUP_HEAD,
	HITGROUP_CHEST,
	HITGROUP_STOMACH,
	HITGROUP_LEFTARM,
	HITGROUP_RIGHTARM,
	HITGROUP_LEFTLEG,
	HITGROUP_RIGHTLEG,
	HITGROUP_NECK,
	HITGROUP_GEAR = 10
};

struct c_bullet_info
{
	c_bullet_info()
	{
		position = vec3_t(0, 0, 0);
		time_stamp = 0;
		expire_time = -1;
	}

	c_bullet_info(vec3_t position, float time_stamp, float expire_time)
	{
		position = position;
		time_stamp = time_stamp;
		expire_time = expire_time;
	}

	vec3_t	position;
	float	time_stamp;
	float	expire_time;
};

class c_bullet_service
{
public:
	int bullet_count()
	{
		return *reinterpret_cast<int*>(uintptr_t(this) + 0x48);
	}

	c_utl_vector<c_bullet_info>& bullet_data()
	{
		return *reinterpret_cast<c_utl_vector<c_bullet_info>*>(uintptr_t(this) + 0x48);
	}
};

class c_cs_player_pawn : public c_cs_player_pawn_base
{
public:
	SCHEMA(movement_services, c_player_movement_service*, "C_BasePlayerPawn", "m_pMovementServices");
	SCHEMA(bullet_services_int, int, "C_CSPlayerPawn", "m_pBulletServices");
	SCHEMA(bullet_services, c_bullet_service*, "C_CSPlayerPawn", "m_pBulletServices");
	SCHEMA(hostage_services, c_cs_player_hostage_services*, "C_CSPlayerPawn", "m_pHostageServices");
	SCHEMA(buy_services, c_cs_player_buy_services*, "C_CSPlayerPawn", "m_pBuyServices");
	SCHEMA(action_tracking_services, c_cs_player_controller_action_tracking_services*, "C_CSPlayerPawn", "m_pActionTrackingServices");
	SCHEMA(healthshot_boost_expiration, float, "C_CSPlayerPawn", "m_flHealthShotBoostExpirationTime");
	SCHEMA(last_fired_weapon_time, float, "C_CSPlayerPawn", "m_flLastFiredWeaponTime");
	SCHEMA(has_female_voice, bool, "C_CSPlayerPawn", "m_bHasFemaleVoice");
	SCHEMA(landing_time_seconds, float, "C_CSPlayerPawn", "m_flLandingTimeSeconds");
	SCHEMA(old_fall_velocity, float, "C_CSPlayerPawn", "m_flOldFallVelocity");
	SCHEMA(last_place_name, char, "C_CSPlayerPawn", "m_szLastPlaceName");
	SCHEMA(prev_defuser, bool, "C_CSPlayerPawn", "m_bPrevDefuser");
	SCHEMA(prev_helmet, bool, "C_CSPlayerPawn", "m_bPrevHelmet");
	SCHEMA(prev_armor_value, int, "C_CSPlayerPawn", "m_nPrevArmorVal");
	SCHEMA(prev_grenade_ammo_count, int, "C_CSPlayerPawn", "m_nPrevGrenadeAmmoCount");
	SCHEMA(previous_weapon_hash, uint32_t, "C_CSPlayerPawn", "m_unPreviousWeaponHash");
	SCHEMA(weapon_hash, uint32_t, "C_CSPlayerPawn", "m_unWeaponHash");
	SCHEMA(in_buy_zone, bool, "C_CSPlayerPawn", "m_bInBuyZone");
	SCHEMA(previously_in_buy_zone, bool, "C_CSPlayerPawn", "m_bPreviouslyInBuyZone");
	SCHEMA(aim_punch_angle, vec3_t, "C_CSPlayerPawn", "m_aimPunchAngle");
	SCHEMA(aim_punch_angle_vel, vec3_t, "C_CSPlayerPawn", "m_aimPunchAngleVel");
	SCHEMA(aim_punch_tick_base, int, "C_CSPlayerPawn", "m_aimPunchTickBase");
	SCHEMA(aim_punch_tick_fraction, float, "C_CSPlayerPawn", "m_aimPunchTickFraction");
	SCHEMA(aim_punch_cache, c_utl_vector<vec3_t>, "C_CSPlayerPawn", "m_aimPunchCache");
	SCHEMA(in_landing, bool, "C_CSPlayerPawn", "m_bInLanding");
	SCHEMA(landing_start_time, float, "C_CSPlayerPawn", "m_flLandingStartTime");
	SCHEMA(in_hostage_rescue_zone, bool, "C_CSPlayerPawn", "m_bInHostageRescueZone");
	SCHEMA(in_bomb_zone, bool, "C_CSPlayerPawn", "m_bInBombZone");
	SCHEMA(is_buy_menu_open, bool, "C_CSPlayerPawn", "m_bIsBuyMenuOpen");
	SCHEMA(time_of_last_injury, float, "C_CSPlayerPawn", "m_flTimeOfLastInjury");
	SCHEMA(next_spray_decal_time, float, "C_CSPlayerPawn", "m_flNextSprayDecalTime");
	SCHEMA(retakes_offering, int, "C_CSPlayerPawn", "m_iRetakesOffering");
	SCHEMA(retakes_offering_card, int, "C_CSPlayerPawn", "m_iRetakesOfferingCard");
	SCHEMA(retakes_has_defuse_kit, bool, "C_CSPlayerPawn", "m_bRetakesHasDefuseKit");
	SCHEMA(retakes_mvp_last_round, bool, "C_CSPlayerPawn", "m_bRetakesMVPLastRound");
	SCHEMA(retakes_mvp_boost_item, int, "C_CSPlayerPawn", "m_iRetakesMVPBoostItem");
	SCHEMA(retakes_mvp_boost_extra_utility, int, "C_CSPlayerPawn", "m_RetakesMVPBoostExtraUtility");
	SCHEMA(need_to_reapply_gloves, bool, "C_CSPlayerPawn", "m_bNeedToReApplyGloves");
	SCHEMA(m_spawn_time_index, float, "C_CSPlayerPawnBase", "m_flLastSpawnTimeIndex");
	SCHEMA(econ_gloves, c_econ_item_view, "C_CSPlayerPawn", "m_EconGloves");
	SCHEMA(econ_gloves_changed, uint8_t, "C_CSPlayerPawn", "m_nEconGlovesChanged");
	SCHEMA(must_sync_ragdoll_state, bool, "C_CSPlayerPawn", "m_bMustSyncRagdollState");
	SCHEMA(ragdoll_damage_bone, int, "C_CSPlayerPawn", "m_nRagdollDamageBone");
	SCHEMA(ragdoll_damage_force, vec3_t, "C_CSPlayerPawn", "m_vRagdollDamageForce");
	SCHEMA(ragdoll_damage_position, vec3_t, "C_CSPlayerPawn", "m_vRagdollDamagePosition");
	SCHEMA(ragdoll_damage_weapon_name, char, "C_CSPlayerPawn", "m_szRagdollDamageWeaponName");
	SCHEMA(ragdoll_damage_headshot, bool, "C_CSPlayerPawn", "m_bRagdollDamageHeadshot");
	SCHEMA(ragdoll_server_origin, vec3_t, "C_CSPlayerPawn", "m_vRagdollServerOrigin");
	SCHEMA(last_head_bone_valid, bool, "C_CSPlayerPawn", "m_bLastHeadBoneTransformIsValid");
	SCHEMA(last_land_time, float, "C_CSPlayerPawn", "m_lastLandTime");
	SCHEMA(on_ground_last_tick, bool, "C_CSPlayerPawn", "m_bOnGroundLastTick");
	SCHEMA(death_eye_angles, vec3_t, "C_CSPlayerPawn", "m_qDeathEyeAngles");
	SCHEMA(skip_one_head_constraint_update, bool, "C_CSPlayerPawn", "m_bSkipOneHeadConstraintUpdate");
	SCHEMA(left_handed, bool, "C_CSPlayerPawn", "m_bLeftHanded");
	SCHEMA(switched_handedness_time, float, "C_CSPlayerPawn", "m_fSwitchedHandednessTime");
	SCHEMA(viewmodel_offset_x, float, "C_CSPlayerPawn", "m_flViewmodelOffsetX");
	SCHEMA(viewmodel_offset_y, float, "C_CSPlayerPawn", "m_flViewmodelOffsetY");
	SCHEMA(viewmodel_offset_z, float, "C_CSPlayerPawn", "m_flViewmodelOffsetZ");
	SCHEMA(viewmodel_fov, float, "C_CSPlayerPawn", "m_flViewmodelFOV");
	SCHEMA(player_patch_econ_indices, int, "C_CSPlayerPawn", "m_vecPlayerPatchEconIndices");
	SCHEMA(gun_game_immunity_color, c_color, "C_CSPlayerPawn", "m_GunGameImmunityColor");
	SCHEMA(bullet_hit_models, c_utl_vector<void*>, "C_CSPlayerPawn", "m_vecBulletHitModels");
	SCHEMA(is_walking, bool, "C_CSPlayerPawn", "m_bIsWalking");
	SCHEMA(third_person_heading, vec3_t, "C_CSPlayerPawn", "m_thirdPersonHeading");
	SCHEMA(slope_drop_offset, float, "C_CSPlayerPawn", "m_flSlopeDropOffset");
	SCHEMA(slope_drop_height, float, "C_CSPlayerPawn", "m_flSlopeDropHeight");
	SCHEMA(head_constraint_offset, vec3_t, "C_CSPlayerPawn", "m_vHeadConstraintOffset");
	SCHEMA_WITH_OFFSET(entity_spotted_state, int, "C_CSPlayerPawn", "m_entitySpottedState", 0x8);
	SCHEMA(scoped, bool, "C_CSPlayerPawn", "m_bIsScoped");
	SCHEMA(resume_zoom, bool, "C_CSPlayerPawn", "m_bResumeZoom");
	SCHEMA(is_defusing, bool, "C_CSPlayerPawn", "m_bIsDefusing");
	SCHEMA(is_grabbing_hostage, bool, "C_CSPlayerPawn", "m_bIsGrabbingHostage");
	SCHEMA(blocking_use_action_in_progress, int, "C_CSPlayerPawn", "m_iBlockingUseActionInProgress");
	SCHEMA(emit_sound_time, float, "C_CSPlayerPawn", "m_flEmitSoundTime");
	SCHEMA(in_no_defuse_area, bool, "C_CSPlayerPawn", "m_bInNoDefuseArea");
	SCHEMA(which_bomb_zone, int, "C_CSPlayerPawn", "m_nWhichBombZone");
	SCHEMA(shots_fired, int, "C_CSPlayerPawn", "m_iShotsFired");
	SCHEMA(flinch_stack, float, "C_CSPlayerPawn", "m_flFlinchStack");
	SCHEMA(velocity_modifier, float, "C_CSPlayerPawn", "m_flVelocityModifier");
	SCHEMA(hit_heading, float, "C_CSPlayerPawn", "m_flHitHeading");
	SCHEMA(hit_body_part, int, "C_CSPlayerPawn", "m_nHitBodyPart");
	SCHEMA(wait_for_no_attack, bool, "C_CSPlayerPawn", "m_bWaitForNoAttack");
	SCHEMA(ignore_ladder_jump_time, float, "C_CSPlayerPawn", "m_ignoreLadderJumpTime");
	SCHEMA(killed_by_headshot, bool, "C_CSPlayerPawn", "m_bKilledByHeadshot");
	SCHEMA(armor_value, int, "C_CSPlayerPawn", "m_ArmorValue");
	SCHEMA(current_equipment_value, uint16_t, "C_CSPlayerPawn", "m_unCurrentEquipmentValue");
	SCHEMA(round_start_equipment_value, uint16_t, "C_CSPlayerPawn", "m_unRoundStartEquipmentValue");
	SCHEMA(freezetime_end_equipment_value, uint16_t, "C_CSPlayerPawn", "m_unFreezetimeEndEquipmentValue");
	SCHEMA(last_killer_index, int, "C_CSPlayerPawn", "m_nLastKillerIndex");
	SCHEMA(old_is_scoped, bool, "C_CSPlayerPawn", "m_bOldIsScoped");
	SCHEMA(has_death_info, bool, "C_CSPlayerPawn", "m_bHasDeathInfo");
	SCHEMA(death_info_time, float, "C_CSPlayerPawn", "m_flDeathInfoTime");
	SCHEMA(death_info_origin, vec3_t, "C_CSPlayerPawn", "m_vecDeathInfoOrigin");
	SCHEMA(grenade_stash_time, float, "C_CSPlayerPawn", "m_grenadeParameterStashTime");
	SCHEMA(grenade_parameters_stashed, bool, "C_CSPlayerPawn", "m_bGrenadeParametersStashed");
	SCHEMA(stashed_shoot_angles, vec3_t, "C_CSPlayerPawn", "m_angStashedShootAngles");
	SCHEMA(stashed_throw_position, vec3_t, "C_CSPlayerPawn", "m_vecStashedGrenadeThrowPosition");
	SCHEMA(stashed_velocity, vec3_t, "C_CSPlayerPawn", "m_vecStashedVelocity");
	SCHEMA(shoot_angle_history, vec3_t, "C_CSPlayerPawn", "m_angShootAngleHistory");
	SCHEMA(throw_position_history, vec3_t, "C_CSPlayerPawn", "m_vecThrowPositionHistory");
	SCHEMA(velocity_history, vec3_t, "C_CSPlayerPawn", "m_vecVelocityHistory");
	SCHEMA(predicted_damage_tags, c_utl_vector<void*>, "C_CSPlayerPawn", "m_PredictedDamageTags");
	SCHEMA(prev_highest_received_damage_tag_tick, int, "C_CSPlayerPawn", "m_nPrevHighestReceivedDamageTagTick");
	SCHEMA(highest_applied_damage_tag_tick, int, "C_CSPlayerPawn", "m_nHighestAppliedDamageTagTick");

	vec3_t get_eye_pos() 
	{
		vec3_t view_;

		vmt::call_virtual<void>(this, 172, &view_);

		return view_;
	}

	bool is_enemy()
	{
		const auto local = interfaces::entity_system->get_local_pawn();
		if (!local)
			return false;

		return local->team_num() != this->team_num();
	}

	vec3_t get_bone_position(int index)
	{
		using GetBonePosition_t = int(__fastcall*)(void*, int, vec3_t&, vec3_t&);
		static GetBonePosition_t fn = reinterpret_cast<GetBonePosition_t>(utils::find_pattern(g_modules.client, "48 89 6C 24 ? 48 89 74 24 ? 48 89 7C 24 ? 41 56 48 83 EC ? 4D 8B F1 49 8B E8"));

		vec3_t position;
		vec3_t rotation;

		fn(this, index, position, rotation);

		return position;
	}

	void build_bones(uint32_t bones_to_build) 
	{
		using fnBuildBones = void(__fastcall*)(void*, uint32_t);
		static fnBuildBones oBuildBone = reinterpret_cast<fnBuildBones>(utils::find_pattern(g_modules.client, "48 89 6C 24 ? 48 89 74 24 ? 48 89 7C 24 ? 41 56 48 83 EC ? 4D 8B F1 49 8B E8"));

		oBuildBone(this, bones_to_build);
	}

	bool has_armor(int hitgroup)
	{
		if (hitgroup == 1)
			return this->item_services()->has_helmet();

		return !!this->armor_value();
	}

	std::uint32_t get_owner_handle_index()
	{
		std::uint32_t result = -1;

		if (this && collision() && !(collision()->solid_flags() & 4))
			result = this->owner_entity().get_entry_index();

		return result;
	}

	bool has_c4()
	{
		return this->weapon_services() && this->weapon_services()->weapon_owns_this_type("weapon_c4");
	}

	c_hitboxsets* get_hitbox_sets(unsigned int hitbox_set_index)
	{
		using GetHitboxSets_t = std::int64_t(__fastcall*) (void*, unsigned int);
		static GetHitboxSets_t GetHitboxSets = reinterpret_cast<GetHitboxSets_t>(utils::find_pattern(g_modules.client, "48 89 5C 24 ? 48 89 74 24 ? 57 48 81 EC ? ? ? ? 8B DA 48 8B F9 E8 ? ? ? ? 48 8B F0 48 85 C0 0F 84"));

		return reinterpret_cast<c_hitboxsets*>(GetHitboxSets(this, hitbox_set_index));
	}

	c_base_player_weapon* get_active_weapon() {
		c_player_weapon_services* weapon_servicess = this->weapon_services();
		if (!weapon_servicess)
			return nullptr;

		if (!weapon_servicess->active_weapon().is_valid())
			return nullptr;

		c_base_player_weapon* active_weapon = reinterpret_cast<c_base_player_weapon*>(interfaces::entity_system->get_base_entity(weapon_servicess->active_weapon().get_entry_index()));
		
		if (active_weapon == nullptr)
			return nullptr;
		
		if (!active_weapon->is_weapon())
			return nullptr;

		return active_weapon;
	}

	bool is_throwing()
	{
		c_base_player_weapon* active_weapon = this->get_active_weapon();

		if (!active_weapon)
			return false;

		c_base_cs_grenade* grenade = reinterpret_cast<c_base_cs_grenade*>(active_weapon);
		c_cs_weapon_base_v_data* weapon_data = active_weapon->get_weapon_data();

		if (!grenade || !weapon_data)
			return false;

		if (weapon_data->weapon_type() == WEAPONTYPE_GRENADE && grenade->throw_time() != 0.f)
			return true;

		return false;
	}

	bool is_alive() { return health() > 0; }

	int get_bone_index(const char* name)
	{
		using get_bone_index_t = int(__fastcall*)(void*, const char*);
		static get_bone_index_t get_bone_index = reinterpret_cast<get_bone_index_t>(utils::find_pattern_rel(g_modules.client, "E8 ? ? ? ? 85 C0 78 ? 4C 8D 4D", 0x1));

		return get_bone_index(this, name);
	}
};