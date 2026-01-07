#pragma once

#include <memory>
#include "../utils/schema/schema.h"

enum e_weapon_type : uint32_t
{
	WEAPONTYPE_KNIFE = 0x0,
	WEAPONTYPE_PISTOL = 0x1,
	WEAPONTYPE_SUBMACHINEGUN = 0x2,
	WEAPONTYPE_RIFLE = 0x3,
	WEAPONTYPE_SHOTGUN = 0x4,
	WEAPONTYPE_SNIPER_RIFLE = 0x5,
	WEAPONTYPE_MACHINEGUN = 0x6,
	WEAPONTYPE_C4 = 0x7,
	WEAPONTYPE_TASER = 0x8,
	WEAPONTYPE_GRENADE = 0x9,
	WEAPONTYPE_EQUIPMENT = 0xa,
	WEAPONTYPE_STACKABLEITEM = 0xb,
	WEAPONTYPE_FISTS = 0xc,
	WEAPONTYPE_BREACHCHARGE = 0xd,
	WEAPONTYPE_BUMPMINE = 0xe,
	WEAPONTYPE_TABLET = 0xf,
	WEAPONTYPE_MELEE = 0x10,
	WEAPONTYPE_SHIELD = 0x11,
	WEAPONTYPE_ZONE_REPULSOR = 0x12,
	WEAPONTYPE_UNKNOWN = 0x13,
};

using firing_float_t = float[2];

class c_cs_weapon_base_v_data
{
public:
	SCHEMA(world_model, const char*, "CBasePlayerWeaponVData", "m_szWorldModel"); // CResourceNameTyped<CWeakHandle<InfoForResourceTypeCModel>>
	SCHEMA(tools_only_owner_model_name, const char*, "CBasePlayerWeaponVData", "m_sToolsOnlyOwnerModelName"); // CResourceNameTyped<CWeakHandle<InfoForResourceTypeCModel>>
	SCHEMA(built_right_handed, bool, "CBasePlayerWeaponVData", "m_bBuiltRightHanded");
	SCHEMA(allow_flipping, bool, "CBasePlayerWeaponVData", "m_bAllowFlipping");
	SCHEMA(muzzle_attachment, const char*, "CBasePlayerWeaponVData", "m_sMuzzleAttachment"); // CAttachmentNameSymbolWithStorage
	SCHEMA(muzzle_flash_particle, const char*, "CBasePlayerWeaponVData", "m_szMuzzleFlashParticle"); // CResourceNameTyped<CWeakHandle<InfoForResourceTypeIParticleSystemDefinition>>
	SCHEMA(linked_cooldowns, bool, "CBasePlayerWeaponVData", "m_bLinkedCooldowns");
	SCHEMA(flags, int, "CBasePlayerWeaponVData", "m_iFlags"); // ItemFlagTypes_t
	SCHEMA(primary_ammo_type, int, "CBasePlayerWeaponVData", "m_nPrimaryAmmoType"); // AmmoIndex_t
	SCHEMA(secondary_ammo_type, int, "CBasePlayerWeaponVData", "m_nSecondaryAmmoType"); // AmmoIndex_t
	SCHEMA(max_clip1, int, "CBasePlayerWeaponVData", "m_iMaxClip1");
	SCHEMA(max_clip2, int, "CBasePlayerWeaponVData", "m_iMaxClip2");
	SCHEMA(default_clip1, int, "CBasePlayerWeaponVData", "m_iDefaultClip1");
	SCHEMA(default_clip2, int, "CBasePlayerWeaponVData", "m_iDefaultClip2");
	SCHEMA(reserve_ammo_as_clips, bool, "CBasePlayerWeaponVData", "m_bReserveAmmoAsClips");
	SCHEMA(weight, int, "CBasePlayerWeaponVData", "m_iWeight");
	SCHEMA(auto_switch_to, bool, "CBasePlayerWeaponVData", "m_bAutoSwitchTo");
	SCHEMA(auto_switch_from, bool, "CBasePlayerWeaponVData", "m_bAutoSwitchFrom");
	SCHEMA(rumble_effect, int, "CBasePlayerWeaponVData", "m_iRumbleEffect"); // RumbleEffect_t
	SCHEMA(slot, int, "CBasePlayerWeaponVData", "m_iSlot");
	SCHEMA(position, int, "CBasePlayerWeaponVData", "m_iPosition");
	SCHEMA(shoot_sounds, void*, "CBasePlayerWeaponVData", "m_aShootSounds"); // CUtlOrderedMap<WeaponSound_t, CSoundEventName>
	SCHEMA(weapon_type, int, "CCSWeaponBaseVData", "m_WeaponType"); // CSWeaponType
	SCHEMA(weapon_category, int, "CCSWeaponBaseVData", "m_WeaponCategory"); // CSWeaponCategory
	SCHEMA(view_model, const char*, "CCSWeaponBaseVData", "m_szViewModel"); // CResourceNameTyped<CWeakHandle<InfoForResourceTypeCModel>>
	SCHEMA(player_model, const char*, "CCSWeaponBaseVData", "m_szPlayerModel"); // CResourceNameTyped<CWeakHandle<InfoForResourceTypeCModel>>
	SCHEMA(world_dropped_model, const char*, "CCSWeaponBaseVData", "m_szWorldDroppedModel"); // CResourceNameTyped<CWeakHandle<InfoForResourceTypeCModel>>
	SCHEMA(aimsight_lens_mask_model, const char*, "CCSWeaponBaseVData", "m_szAimsightLensMaskModel"); // CResourceNameTyped<CWeakHandle<InfoForResourceTypeCModel>>
	SCHEMA(magazine_model, const char*, "CCSWeaponBaseVData", "m_szMagazineModel"); // CResourceNameTyped<CWeakHandle<InfoForResourceTypeCModel>>
	SCHEMA(heat_effect, const char*, "CCSWeaponBaseVData", "m_szHeatEffect"); // CResourceNameTyped<CWeakHandle<InfoForResourceTypeIParticleSystemDefinition>>
	SCHEMA(eject_brass_effect, const char*, "CCSWeaponBaseVData", "m_szEjectBrassEffect"); // CResourceNameTyped<CWeakHandle<InfoForResourceTypeIParticleSystemDefinition>>
	SCHEMA(muzzle_flash_particle_alt, const char*, "CCSWeaponBaseVData", "m_szMuzzleFlashParticleAlt"); // CResourceNameTyped<CWeakHandle<InfoForResourceTypeIParticleSystemDefinition>>
	SCHEMA(muzzle_flash_thirdperson, const char*, "CCSWeaponBaseVData", "m_szMuzzleFlashThirdPersonParticle"); // CResourceNameTyped<CWeakHandle<InfoForResourceTypeIParticleSystemDefinition>>
	SCHEMA(muzzle_flash_thirdperson_alt, const char*, "CCSWeaponBaseVData", "m_szMuzzleFlashThirdPersonParticleAlt"); // CResourceNameTyped<CWeakHandle<InfoForResourceTypeIParticleSystemDefinition>>
	SCHEMA(tracer_particle, const char*, "CCSWeaponBaseVData", "m_szTracerParticle"); // CResourceNameTyped<CWeakHandle<InfoForResourceTypeIParticleSystemDefinition>>
	SCHEMA(gear_slot, int, "CCSWeaponBaseVData", "m_GearSlot"); // gear_slot_t
	SCHEMA(gear_slot_position, int, "CCSWeaponBaseVData", "m_GearSlotPosition");
	SCHEMA(default_loadout_slot, int, "CCSWeaponBaseVData", "m_DefaultLoadoutSlot"); // loadout_slot_t
	SCHEMA(wrong_team_msg, const char*, "CCSWeaponBaseVData", "m_sWrongTeamMsg");
	SCHEMA(price, int, "CCSWeaponBaseVData", "m_nPrice");
	SCHEMA(kill_award, int, "CCSWeaponBaseVData", "m_nKillAward");
	SCHEMA(primary_reserve_ammo_max, int, "CCSWeaponBaseVData", "m_nPrimaryReserveAmmoMax");
	SCHEMA(secondary_reserve_ammo_max, int, "CCSWeaponBaseVData", "m_nSecondaryReserveAmmoMax");
	SCHEMA(melee_weapon, bool, "CCSWeaponBaseVData", "m_bMeleeWeapon");
	SCHEMA(has_burst_mode, bool, "CCSWeaponBaseVData", "m_bHasBurstMode");
	SCHEMA(is_revolver, bool, "CCSWeaponBaseVData", "m_bIsRevolver");
	SCHEMA(cannot_shoot_underwater, bool, "CCSWeaponBaseVData", "m_bCannotShootUnderwater");
	SCHEMA(name, const char*, "CCSWeaponBaseVData", "m_szName"); // CGlobalSymbol
	SCHEMA(anim_extension, const char*, "CCSWeaponBaseVData", "m_szAnimExtension");
	SCHEMA(silencer_type, int, "CCSWeaponBaseVData", "m_eSilencerType"); // CSWeaponSilencerType
	SCHEMA(crosshair_min_dist, int, "CCSWeaponBaseVData", "m_nCrosshairMinDistance");
	SCHEMA(crosshair_delta_dist, int, "CCSWeaponBaseVData", "m_nCrosshairDeltaDistance");
	SCHEMA(is_full_auto, bool, "CCSWeaponBaseVData", "m_bIsFullAuto");
	SCHEMA(num_bullets, int, "CCSWeaponBaseVData", "m_nNumBullets");
	SCHEMA(firing_cycle_time, float, "CCSWeaponBaseVData", "m_flCycleTime"); // CFiringModeFloat
	SCHEMA(firing_max_speed, firing_float_t, "CCSWeaponBaseVData", "m_flMaxSpeed");
	SCHEMA(spread, float, "CCSWeaponBaseVData", "m_flSpread");
	SCHEMA(inacc_crouch, float, "CCSWeaponBaseVData", "m_flInaccuracyCrouch");
	SCHEMA(inacc_stand, float, "CCSWeaponBaseVData", "m_flInaccuracyStand");
	SCHEMA(inacc_jump, float, "CCSWeaponBaseVData", "m_flInaccuracyJump");
	SCHEMA(inacc_land, float, "CCSWeaponBaseVData", "m_flInaccuracyLand");
	SCHEMA(inacc_ladder, float, "CCSWeaponBaseVData", "m_flInaccuracyLadder");
	SCHEMA(inacc_fire, float, "CCSWeaponBaseVData", "m_flInaccuracyFire");
	SCHEMA(inacc_move, float, "CCSWeaponBaseVData", "m_flInaccuracyMove");
	SCHEMA(recoil_angle, float, "CCSWeaponBaseVData", "m_flRecoilAngle");
	SCHEMA(recoil_angle_var, float, "CCSWeaponBaseVData", "m_flRecoilAngleVariance");
	SCHEMA(recoil_magnitude, float, "CCSWeaponBaseVData", "m_flRecoilMagnitude");
	SCHEMA(recoil_magnitude_var, float, "CCSWeaponBaseVData", "m_flRecoilMagnitudeVariance");
	SCHEMA(tracer_frequency, int, "CCSWeaponBaseVData", "m_nTracerFrequency"); // CFiringModeInt
	SCHEMA(inacc_jump_initial, float, "CCSWeaponBaseVData", "m_flInaccuracyJumpInitial");
	SCHEMA(inacc_jump_apex, float, "CCSWeaponBaseVData", "m_flInaccuracyJumpApex");
	SCHEMA(inacc_reload, float, "CCSWeaponBaseVData", "m_flInaccuracyReload");
	SCHEMA(recoil_seed, int, "CCSWeaponBaseVData", "m_nRecoilSeed");
	SCHEMA(spread_seed, int, "CCSWeaponBaseVData", "m_nSpreadSeed");
	SCHEMA(time_to_idle_after_fire, float, "CCSWeaponBaseVData", "m_flTimeToIdleAfterFire");
	SCHEMA(idle_interval, float, "CCSWeaponBaseVData", "m_flIdleInterval");
	SCHEMA(attack_movespeed_factor, float, "CCSWeaponBaseVData", "m_flAttackMovespeedFactor");
	SCHEMA(heat_per_shot, float, "CCSWeaponBaseVData", "m_flHeatPerShot");
	SCHEMA(inaccuracy_pitch_shift, float, "CCSWeaponBaseVData", "m_flInaccuracyPitchShift");
	SCHEMA(inaccuracy_alt_sound_threshold, float, "CCSWeaponBaseVData", "m_flInaccuracyAltSoundThreshold");
	SCHEMA(bot_audible_range, float, "CCSWeaponBaseVData", "m_flBotAudibleRange");
	SCHEMA(use_radio_subtitle, const char*, "CCSWeaponBaseVData", "m_szUseRadioSubtitle");
	SCHEMA(unzoom_after_shot, bool, "CCSWeaponBaseVData", "m_bUnzoomsAfterShot");
	SCHEMA(hide_view_model_zoomed, bool, "CCSWeaponBaseVData", "m_bHideViewModelWhenZoomed");
	SCHEMA(zoom_levels, int, "CCSWeaponBaseVData", "m_nZoomLevels");
	SCHEMA(zoom_fov1, int, "CCSWeaponBaseVData", "m_nZoomFOV1");
	SCHEMA(zoom_fov2, int, "CCSWeaponBaseVData", "m_nZoomFOV2");
	SCHEMA(zoom_time0, float, "CCSWeaponBaseVData", "m_flZoomTime0");
	SCHEMA(zoom_time1, float, "CCSWeaponBaseVData", "m_flZoomTime1");
	SCHEMA(zoom_time2, float, "CCSWeaponBaseVData", "m_flZoomTime2");
	SCHEMA(iron_sight_pull_up_speed, float, "CCSWeaponBaseVData", "m_flIronSightPullUpSpeed");
	SCHEMA(iron_sight_put_down_speed, float, "CCSWeaponBaseVData", "m_flIronSightPutDownSpeed");
	SCHEMA(iron_sight_fov, float, "CCSWeaponBaseVData", "m_flIronSightFOV");
	SCHEMA(iron_sight_pivot_forward, float, "CCSWeaponBaseVData", "m_flIronSightPivotForward");
	SCHEMA(iron_sight_looseness, float, "CCSWeaponBaseVData", "m_flIronSightLooseness");
	SCHEMA(iron_sight_pivot_angle, vec3_t, "CCSWeaponBaseVData", "m_angPivotAngle"); // QAngle
	SCHEMA(iron_sight_eye_pos, vec3_t, "CCSWeaponBaseVData", "m_vecIronSightEyePos");
	SCHEMA(damage, int, "CCSWeaponBaseVData", "m_nDamage");
	SCHEMA(headshot_multiplier, float, "CCSWeaponBaseVData", "m_flHeadshotMultiplier");
	SCHEMA(armor_ratio, float, "CCSWeaponBaseVData", "m_flArmorRatio");
	SCHEMA(m_armor_ratio, float, ("CCSWeaponBaseVData"), ("m_flArmorRatio"));
	SCHEMA(penetration, float, "CCSWeaponBaseVData", "m_flPenetration");
	SCHEMA(range, float, "CCSWeaponBaseVData", "m_flRange");
	SCHEMA(range_modifier, float, "CCSWeaponBaseVData", "m_flRangeModifier");
	SCHEMA(flinch_vel_modifier_large, float, "CCSWeaponBaseVData", "m_flFlinchVelocityModifierLarge");
	SCHEMA(flinch_vel_modifier_small, float, "CCSWeaponBaseVData", "m_flFlinchVelocityModifierSmall");
	SCHEMA(recovery_time_crouch, float, "CCSWeaponBaseVData", "m_flRecoveryTimeCrouch");
	SCHEMA(recovery_time_stand, float, "CCSWeaponBaseVData", "m_flRecoveryTimeStand");
	SCHEMA(recovery_time_crouch_final, float, "CCSWeaponBaseVData", "m_flRecoveryTimeCrouchFinal");
	SCHEMA(recovery_time_stand_final, float, "CCSWeaponBaseVData", "m_flRecoveryTimeStandFinal");
	SCHEMA(recovery_bullet_start, int, "CCSWeaponBaseVData", "m_nRecoveryTransitionStartBullet");
	SCHEMA(recovery_bullet_end, int, "CCSWeaponBaseVData", "m_nRecoveryTransitionEndBullet");
	SCHEMA(throw_velocity, float, "CCSWeaponBaseVData", "m_flThrowVelocity");
	SCHEMA(smoke_color, vec3_t, "CCSWeaponBaseVData", "m_vSmokeColor");
	SCHEMA(anim_class, const char*, "CCSWeaponBaseVData", "m_szAnimClass");
};