#pragma once

#include "../utils/schema/schema.h"
#include "c_econ_entity.h"

enum ItemDefinitionIndex_t : short
{
	WEAPON_DEAGLE = 1,
	WEAPON_DUAL_BERETTAS = 2,
	WEAPON_FIVE_SEVEN = 3,
	WEAPON_GLOCK = 4,
	WEAPON_AK47 = 7,
	WEAPON_AUG = 8,
	WEAPON_AWP = 9,
	WEAPON_FAMAS = 10,
	WEAPON_G3SG1 = 11,
	WEAPON_GALIL = 13,
	WEAPON_M249 = 14,
	WEAPON_M4A4 = 16,
	WEAPON_MAC10 = 17,
	WEAPON_P90 = 19,
	WEAPON_UMP = 24,
	WEAPON_MP5SD = 23,
	WEAPON_XM1024 = 25,
	WEAPON_BIZON = 26,
	WEAPON_MAG7 = 27,
	WEAPON_NEGEV = 28,
	WEAPON_SAWEDOFF = 29,
	WEAPON_TEC9 = 30,
	WEAPON_TASER = 31,
	WEAPON_HKP2000 = 32,
	WEAPON_MP7 = 33,
	WEAPON_MP9 = 34,
	WEAPON_NOVA = 35,
	WEAPON_P250 = 36,
	WEAPON_SCAR20 = 38,
	WEAPON_SG553 = 39,
	WEAPON_SSG08 = 40,
	WEAPON_FLASHBANG = 43,
	WEAPON_HEGRENADE = 44,
	WEAPON_SMOKE = 45,
	WEAPON_MOLOTOV = 46,
	WEAPON_DECOY = 47,
	WEAPON_INCDENDIARY = 48,
	WEAPON_HEALTHSHOT = 57,
	WEAPON_M4A1_S = 60,
	WEAPON_USP_S = 61,
	WEAPON_CZ75 = 63,
	WEAPON_REVOLVER = 64,
	WEAPON_BAYONET = 500,
	WEAPON_KNIFECSS = 503,
	WEAPON_KNIFEFLIP = 505,
	WEAPON_KNIFEGUT = 506,
	WEAPON_KNIFEKARAMBIT = 507,
	WEAPON_KNIFEM9BAYONET = 508,
	WEAPON_KNIFETACTICAL = 509,
	WEAPON_KNIFEFALCHION = 512,
	WEAPON_KNIFESURVIVALBOWIE = 514,
	WEAPON_KNIFEBUTTERFLY = 515,
	WEAPON_KNIFEPUSH = 516,
	WEAPON_KNIFECORD = 517,
	WEAPON_KNIFECANIS = 518,
	WEAPON_KNIFEURSUS = 519,
	WEAPON_KNIFEGYPSYJACKKNIFE = 520,
	WEAPON_KNIFEOUTDOOR = 521,
	WEAPON_KNIFESTILETTO = 522,
	WEAPON_KNIFEWIDOWMAKER = 523,
	WEAPON_KNIFESKELETON = 525,
	WEAPON_KNIFEKUKRI = 526,
};

class c_base_player_weapon : public c_econ_entity
{
public:
	SCHEMA(n_next_primary_attack_tick, int, "C_BasePlayerWeapon", "m_nNextPrimaryAttackTick"); 
	SCHEMA(fl_next_primary_attack_tick_ratio, float, "C_BasePlayerWeapon", "m_flNextPrimaryAttackTickRatio");
	SCHEMA(n_next_secondary_attack_tick, int, "C_BasePlayerWeapon", "m_nNextSecondaryAttackTick"); 
	SCHEMA(fl_next_secondary_attack_tick_ratio, float, "C_BasePlayerWeapon", "m_flNextSecondaryAttackTickRatio");
	SCHEMA(i_clip1, int, "C_BasePlayerWeapon", "m_iClip1");
	SCHEMA(i_clip2, int, "C_BasePlayerWeapon", "m_iClip2");
	SCHEMA(p_reserve_ammo, int, "C_BasePlayerWeapon", "m_pReserveAmmo");
	SCHEMA(fl_fire_sequence_start_time, float, "C_CSWeaponBase", "m_flFireSequenceStartTime");
	SCHEMA(n_fire_sequence_start_time_change, int, "C_CSWeaponBase", "m_nFireSequenceStartTimeChange");
	SCHEMA(n_fire_sequence_start_time_ack, int, "C_CSWeaponBase", "m_nFireSequenceStartTimeAck");
	SCHEMA(e_player_fire_event, int, "C_CSWeaponBase", "m_ePlayerFireEvent"); // PlayerAnimEvent_t (enum/int)
	SCHEMA(e_player_fire_event_attack_type, int, "C_CSWeaponBase", "m_ePlayerFireEventAttackType"); // WeaponAttackType_t (enum/int)
	SCHEMA(seq_idle, int, "C_CSWeaponBase", "m_seqIdle"); // HSequence (handle/int)
	SCHEMA(seq_fire_primary, int, "C_CSWeaponBase", "m_seqFirePrimary"); // HSequence
	SCHEMA(seq_fire_secondary, int, "C_CSWeaponBase", "m_seqFireSecondary"); // HSequence
	SCHEMA(third_person_fire_sequences, void*, "C_CSWeaponBase", "m_thirdPersonFireSequences"); // CUtlVector<HSequence>
	SCHEMA(h_current_third_person_sequence, int, "C_CSWeaponBase", "m_hCurrentThirdPersonSequence"); // HSequence
	SCHEMA(n_silencer_bone_index, int, "C_CSWeaponBase", "m_nSilencerBoneIndex");
	SCHEMA(third_person_sequences, int, "C_CSWeaponBase", "m_thirdPersonSequences"); // HSequence[7]
	SCHEMA(client_previous_weapon_state, int, "C_CSWeaponBase", "m_ClientPreviousWeaponState"); // CSWeaponState_t
	SCHEMA(i_state, int, "C_CSWeaponBase", "m_iState"); // CSWeaponState_t
	SCHEMA(fl_crosshair_distance, float, "C_CSWeaponBase", "m_flCrosshairDistance");
	SCHEMA(i_ammo_last_check, int, "C_CSWeaponBase", "m_iAmmoLastCheck");
	SCHEMA(i_alpha, int, "C_CSWeaponBase", "m_iAlpha");
	SCHEMA(i_scope_texture_id, int, "C_CSWeaponBase", "m_iScopeTextureID");
	SCHEMA(i_crosshair_texture_id, int, "C_CSWeaponBase", "m_iCrosshairTextureID");
	SCHEMA(fl_gun_accuracy_position_deprecated, float, "C_CSWeaponBase", "m_flGunAccuracyPositionDeprecated");
	SCHEMA(n_last_empty_sound_cmd_num, int, "C_CSWeaponBase", "m_nLastEmptySoundCmdNum");
	SCHEMA(n_view_model_index, unsigned int, "C_CSWeaponBase", "m_nViewModelIndex");
	SCHEMA(b_reloads_with_clips, bool, "C_CSWeaponBase", "m_bReloadsWithClips");
	SCHEMA(fl_time_weapon_idle, int, "C_CSWeaponBase", "m_flTimeWeaponIdle"); // GameTime_t (int)
	SCHEMA(b_fire_on_empty, bool, "C_CSWeaponBase", "m_bFireOnEmpty");
	SCHEMA(on_player_pickup, void*, "C_CSWeaponBase", "m_OnPlayerPickup"); // CEntityIOOutput (unknown type, void* as placeholder)
	SCHEMA(weapon_mode, int, "C_CSWeaponBase", "m_weaponMode"); // CSWeaponMode (enum/int)
	SCHEMA(fl_turning_inaccuracy_delta, float, "C_CSWeaponBase", "m_flTurningInaccuracyDelta");
	SCHEMA(vec_turning_inaccuracy_eye_dir_last, vec3_t, "C_CSWeaponBase", "m_vecTurningInaccuracyEyeDirLast");
	SCHEMA(fl_turning_inaccuracy, float, "C_CSWeaponBase", "m_flTurningInaccuracy");
	SCHEMA(f_accuracy_penalty, float, "C_CSWeaponBase", "m_fAccuracyPenalty");
	SCHEMA(fl_last_accuracy_update_time, int, "C_CSWeaponBase", "m_flLastAccuracyUpdateTime"); // GameTime_t (int)
	SCHEMA(f_accuracy_smoothed_for_zoom, float, "C_CSWeaponBase", "m_fAccuracySmoothedForZoom");
	SCHEMA(f_scope_zoom_end_time, int, "C_CSWeaponBase", "m_fScopeZoomEndTime"); // GameTime_t (int)
	SCHEMA(i_recoil_index, int, "C_CSWeaponBase", "m_iRecoilIndex");
	SCHEMA(fl_recoil_index, float, "C_CSWeaponBase", "m_flRecoilIndex");
	SCHEMA(b_burst_mode, bool, "C_CSWeaponBase", "m_bBurstMode");
	SCHEMA(fl_last_burst_mode_change_time, int, "C_CSWeaponBase", "m_flLastBurstModeChangeTime"); // GameTime_t (int)
	SCHEMA(n_postpone_fire_ready_ticks, int, "C_CSWeaponBase", "m_nPostponeFireReadyTicks"); // GameTick_t (int)
	SCHEMA(fl_postpone_fire_ready_frac, float, "C_CSWeaponBase", "m_flPostponeFireReadyFrac");
	SCHEMA(b_in_reload, bool, "C_CSWeaponBase", "m_bInReload");
	SCHEMA(b_reload_visually_complete, bool, "C_CSWeaponBase", "m_bReloadVisuallyComplete");
	SCHEMA(fl_dropped_at_time, int, "C_CSWeaponBase", "m_flDroppedAtTime"); // GameTime_t (int)
	SCHEMA(b_is_hauledback, bool, "C_CSWeaponBase", "m_bIsHauledBack");
	SCHEMA(b_silencer_on, bool, "C_CSWeaponBase", "m_bSilencerOn");
	SCHEMA(fl_time_silencer_switch_complete, int, "C_CSWeaponBase", "m_flTimeSilencerSwitchComplete"); // GameTime_t (int)
	SCHEMA(i_original_team_number, int, "C_CSWeaponBase", "m_iOriginalTeamNumber");
	SCHEMA(i_most_recent_team_number, int, "C_CSWeaponBase", "m_iMostRecentTeamNumber");
	SCHEMA(b_dropped_near_buy_zone, bool, "C_CSWeaponBase", "m_bDroppedNearBuyZone");
	SCHEMA(fl_next_attack_render_time_offset, float, "C_CSWeaponBase", "m_flNextAttackRenderTimeOffset");
	SCHEMA(b_clear_weapon_identifying_ugc, bool, "C_CSWeaponBase", "m_bClearWeaponIdentifyingUGC");
	SCHEMA(b_visuals_data_set, bool, "C_CSWeaponBase", "m_bVisualsDataSet");
	SCHEMA(b_old_first_person_spectated_state, bool, "C_CSWeaponBase", "m_bOldFirstPersonSpectatedState");
	SCHEMA(b_ui_weapon, bool, "C_CSWeaponBase", "m_bUIWeapon");
	SCHEMA(n_custom_econ_reload_event_id, int, "C_CSWeaponBase", "m_nCustomEconReloadEventId");
	SCHEMA(next_prev_owner_use_time, int, "C_CSWeaponBase", "m_nextPrevOwnerUseTime"); // GameTime_t (int)
	SCHEMA(h_prev_owner, void*, "C_CSWeaponBase", "m_hPrevOwner"); // CHandle<C_CSPlayerPawn>
	SCHEMA(n_drop_tick, int, "C_CSWeaponBase", "m_nDropTick"); // GameTick_t (int)
	SCHEMA(donated, bool, "C_CSWeaponBase", "m_donated");
	SCHEMA(f_last_shot_time, int, "C_CSWeaponBase", "m_fLastShotTime"); // GameTime_t (int)
	SCHEMA(b_was_owned_by_ct, bool, "C_CSWeaponBase", "m_bWasOwnedByCT");
	SCHEMA(b_was_owned_by_terrorist, bool, "C_CSWeaponBase", "m_bWasOwnedByTerrorist");
	SCHEMA(gun_heat, float, "C_CSWeaponBase", "m_gunHeat");
	SCHEMA(smoke_attachments, unsigned int, "C_CSWeaponBase", "m_smokeAttachments");
	SCHEMA(last_smoke_time, int, "C_CSWeaponBase", "m_lastSmokeTime"); // GameTime_t (int)
	SCHEMA(fl_next_client_fire_bullet_time, float, "C_CSWeaponBase", "m_flNextClientFireBulletTime");
	SCHEMA(fl_next_client_fire_bullet_time_repredict, float, "C_CSWeaponBase", "m_flNextClientFireBulletTime_Repredict");
	SCHEMA(iron_sight_controller, void*, "C_CSWeaponBase", "m_IronSightController"); // C_IronSightController*
	SCHEMA(i_iron_sight_mode, int, "C_CSWeaponBase", "m_iIronSightMode");
	SCHEMA(fl_last_los_trace_failure_time, int, "C_CSWeaponBase", "m_flLastLOSTraceFailureTime"); // GameTime_t (int)
	SCHEMA(i_num_empty_attacks, int, "C_CSWeaponBase", "m_iNumEmptyAttacks");
	SCHEMA(fl_last_mag_drop_request_time, int, "C_CSWeaponBase", "m_flLastMagDropRequestTime"); // GameTime_t (int)
	SCHEMA(fl_wat_tick_offset, float, "C_CSWeaponBase", "m_flWatTickOffset");
	SCHEMA(zoom_level, int, "C_CSWeaponBaseGun", "m_zoomLevel");
	SCHEMA(i_burst_shots_remaining, int, "C_CSWeaponBaseGun", "m_iBurstShotsRemaining");
	SCHEMA(i_silencer_bodygroup, int, "C_CSWeaponBaseGun", "m_iSilencerBodygroup");
	SCHEMA(silenced_model_index, int, "C_CSWeaponBaseGun", "m_silencedModelIndex");
	SCHEMA(in_precache, bool, "C_CSWeaponBaseGun", "m_inPrecache");
	SCHEMA(b_needs_bolt_action, bool, "C_CSWeaponBaseGun", "m_bNeedsBoltAction");

	c_cs_weapon_base_v_data* get_weapon_data()
	{
		return static_cast<c_cs_weapon_base_v_data*>(GetVData());
	}

	float get_max_speed()
	{
		return vmt::call_virtual<float>(this, 340);
	}

	float get_inaccuracy()
	{
		float x = .0f, y = .0f;

		using fn_get_inaccuracy_t = float(__fastcall*)(void*, float*, float*);
		static fn_get_inaccuracy_t fn = reinterpret_cast<fn_get_inaccuracy_t>(utils::find_pattern(g_modules.client, "48 89 5C 24 ? 55 56 57 48 81 EC ? ? ? ? 44 0F 29 84 24"));
	
		return fn(this, &x, &y);
	}

	float get_spread()
	{
		using fn_get_spread_t = float(__fastcall*)(void*);
		static fn_get_spread_t fn = reinterpret_cast<fn_get_spread_t>(utils::find_pattern(g_modules.client, "48 83 EC ? 48 63 91"));

		return fn(this);
	}

	void update_accuracy_penality()
	{
		using fn_update_accuracy_penality_t = void(__fastcall*)(void*);
		static fn_update_accuracy_penality_t fn = reinterpret_cast<fn_update_accuracy_penality_t>(utils::find_pattern(g_modules.client, "40 53 57 48 83 EC ? 48 8B F9 E8"));

		fn(this);
	}

	short get_item_definition_index()
	{
		if (!this)
			return 0;

		auto econ_item_view = attribute_manager()->item();
		if (econ_item_view == nullptr)
			return 0;

		auto index = econ_item_view->item_definition_index();

		return index;
	}

	bool is_heavy_pistols()
	{
		short idx = this->get_item_definition_index();

		return idx == WEAPON_DEAGLE || idx == WEAPON_REVOLVER;
	}

	bool is_pistols()
	{
		auto info = this->get_weapon_data();

		if (!info)
			return false;

		int type = info->weapon_type();

		return type == WEAPONTYPE_PISTOL && !is_heavy_pistols();
	}

	bool is_knife()
	{
		auto info = this->get_weapon_data();

		if (!info)
			return false;

		short idx = this->get_item_definition_index();

		return info->weapon_type() == WEAPONTYPE_KNIFE && idx != WEAPON_TASER;
	}

	bool is_auto_sniper()
	{
		short idx = this->get_item_definition_index();

		switch (idx)
		{
		case WEAPON_G3SG1:
		case WEAPON_SCAR20:
			return true;
		default:
			return false;
		}
	}
};

class c_cs_weapon_base : public c_base_player_weapon 
{
public:
	SCHEMA(burst_mode, bool, "C_CSWeaponBase", "m_bBurstMode");
};

class c_base_cs_grenade : public c_cs_weapon_base 
{
public:
	SCHEMA(held_by_player, bool, "C_BaseCSGrenade", "m_bIsHeldByPlayer");
	SCHEMA(pin_pulled, bool, "C_BaseCSGrenade", "m_bPinPulled");
	SCHEMA(throw_time, float, "C_BaseCSGrenade", "m_fThrowTime");
	SCHEMA(throw_strength, float, "C_BaseCSGrenade", "m_flThrowStrength");
};

class c_smoke_grenade_projectile {
public:
	SCHEMA(m_did_smoke_effect, bool, "C_SmokeGrenadeProjectile", "m_bDidSmokeEffect");
	SCHEMA(get_smoke_pos, vec3_t, "C_SmokeGrenadeProjectile", "m_vSmokeDetonationPos");
	SCHEMA(get_smoke_tick, int32_t, "C_SmokeGrenadeProjectile", "m_nSmokeEffectTickBegin");
};

class c_inferno : public c_base_entity
{
public:
	SCHEMA(fire_damage_effect, int, "C_Inferno", "m_nfxFireDamageEffect");
	SCHEMA(inferno_points_snapshot, void*, "C_Inferno", "m_hInfernoPointsSnapshot");
	SCHEMA(inferno_filler_points_snapshot, void*, "C_Inferno", "m_hInfernoFillerPointsSnapshot");
	SCHEMA(inferno_outline_points_snapshot, void*, "C_Inferno", "m_hInfernoOutlinePointsSnapshot");
	SCHEMA(inferno_climbing_outline_points_snapshot, void*, "C_Inferno", "m_hInfernoClimbingOutlinePointsSnapshot");
	SCHEMA(inferno_decals_snapshot, void*, "C_Inferno", "m_hInfernoDecalsSnapshot");
	SCHEMA(fire_positions, vec3_t, "C_Inferno", "m_firePositions");
	SCHEMA(fire_parent_positions, vec3_t, "C_Inferno", "m_fireParentPositions");
	SCHEMA(fire_is_burning, bool, "C_Inferno", "m_bFireIsBurning");
	SCHEMA(burn_normal, vec3_t, "C_Inferno", "m_BurnNormal");
	SCHEMA(fire_count, int, "C_Inferno", "m_fireCount");
	SCHEMA(inferno_type, int, "C_Inferno", "m_nInfernoType");
	SCHEMA(fire_lifetime, float, "C_Inferno", "m_nFireLifetime");
	SCHEMA(in_post_effect_time, bool, "C_Inferno", "m_bInPostEffectTime");
	SCHEMA(last_fire_count, int, "C_Inferno", "m_lastFireCount");
	SCHEMA(fire_effect_tick_begin, int, "C_Inferno", "m_nFireEffectTickBegin");
	SCHEMA(drawable_count, int, "C_Inferno", "m_drawableCount");
	SCHEMA(blos_check, bool, "C_Inferno", "m_blosCheck");
	SCHEMA(nlos_period, int, "C_Inferno", "m_nlosperiod");
	SCHEMA(max_fire_half_width, float, "C_Inferno", "m_maxFireHalfWidth");
	SCHEMA(max_fire_height, float, "C_Inferno", "m_maxFireHeight");
	SCHEMA(bounds_min, vec3_t, "C_Inferno", "m_minBounds");
	SCHEMA(bounds_max, vec3_t, "C_Inferno", "m_maxBounds");
	SCHEMA(last_grass_burn_think, float, "C_Inferno", "m_flLastGrassBurnThink");
};