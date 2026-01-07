#pragma once

#include "c_baseentity.h"
#include "c_base_player_pawn.h"

class c_base_view_model : public c_base_entity
{
public:
    SCHEMA(view_offset, vec3_t, "C_BaseModelEntity", "m_vecViewOffset");
    SCHEMA(weapon, c_base_handle, "C_BaseViewModel", "m_hWeapon");
};

class c_cs_player_view_model_services
{
public:
    SCHEMA(view_model, c_base_handle, "CCSPlayer_ViewModelServices", "m_hViewModel");
};

class c_cs_player_pawn_base : public c_base_player_pawn
{
public:
    SCHEMA(view_model_services, c_cs_player_view_model_services*, "C_CSPlayerPawnBase", "m_pViewModelServices");

    SCHEMA(rendering_clip_plane, float, "C_CSPlayerPawnBase", "m_fRenderingClipPlane");
    SCHEMA(last_clip_plane_frame, int, "C_CSPlayerPawnBase", "m_nLastClipPlaneSetupFrame");
    SCHEMA(last_clip_camera_pos, vec3_t, "C_CSPlayerPawnBase", "m_vecLastClipCameraPos");
    SCHEMA(last_clip_camera_forward, vec3_t, "C_CSPlayerPawnBase", "m_vecLastClipCameraForward");
    SCHEMA(clip_hit_static_world, bool, "C_CSPlayerPawnBase", "m_bClipHitStaticWorld");
    SCHEMA(cached_plane_valid, bool, "C_CSPlayerPawnBase", "m_bCachedPlaneIsValid");
    SCHEMA(is_rescuing, bool, "C_CSPlayerPawnBase", "m_bIsRescuing");
    SCHEMA(immune_gungame_time, float, "C_CSPlayerPawnBase", "m_fImmuneToGunGameDamageTime");
    SCHEMA(immune_gungame_time_last, float, "C_CSPlayerPawnBase", "m_fImmuneToGunGameDamageTimeLast");
    SCHEMA(gungame_immunity, bool, "C_CSPlayerPawn", "m_bGunGameImmunity");
    SCHEMA(has_moved_since_spawn, bool, "C_CSPlayerPawnBase", "m_bHasMovedSinceSpawn");
    SCHEMA(molotov_use_time, float, "C_CSPlayerPawnBase", "m_fMolotovUseTime");
    SCHEMA(molotov_damage_time, float, "C_CSPlayerPawnBase", "m_fMolotovDamageTime");
    SCHEMA(throw_grenade_counter, int, "C_CSPlayerPawnBase", "m_iThrowGrenadeCounter");
    SCHEMA(last_spawn_time_index, float, "C_CSPlayerPawnBase", "m_flLastSpawnTimeIndex");
    SCHEMA(progress_bar_duration, int, "C_CSPlayerPawnBase", "m_iProgressBarDuration");
    SCHEMA(progress_bar_start_time, float, "C_CSPlayerPawnBase", "m_flProgressBarStartTime");
    SCHEMA(intro_start_eye_pos, vec3_t, "C_CSPlayerPawnBase", "m_vecIntroStartEyePosition");
    SCHEMA(intro_start_forward, vec3_t, "C_CSPlayerPawnBase", "m_vecIntroStartPlayerForward");
    SCHEMA(client_death_time, float, "C_CSPlayerPawnBase", "m_flClientDeathTime");
    SCHEMA(screen_tear_frame_captured, bool, "C_CSPlayerPawnBase", "m_bScreenTearFrameCaptured");
    SCHEMA(flash_bang_time, float, "C_CSPlayerPawnBase", "m_flFlashBangTime");
    SCHEMA(flash_screenshot_alpha, float, "C_CSPlayerPawnBase", "m_flFlashScreenshotAlpha");
    SCHEMA(flash_overlay_alpha, float, "C_CSPlayerPawnBase", "m_flFlashOverlayAlpha");
    SCHEMA(flash_build_up, bool, "C_CSPlayerPawnBase", "m_bFlashBuildUp");
    SCHEMA(flash_dsp_cleared, bool, "C_CSPlayerPawnBase", "m_bFlashDspHasBeenCleared");
    SCHEMA(flash_screenshot_grabbed, bool, "C_CSPlayerPawnBase", "m_bFlashScreenshotHasBeenGrabbed");
    SCHEMA(flash_max_alpha, float, "C_CSPlayerPawnBase", "m_flFlashMaxAlpha");
    SCHEMA(flash_duration, float, "C_CSPlayerPawnBase", "m_flFlashDuration");
    SCHEMA(health_bar_render_mask_index, int, "C_CSPlayerPawnBase", "m_iHealthBarRenderMaskIndex");
    SCHEMA(health_fade_value, float, "C_CSPlayerPawnBase", "m_flHealthFadeValue");
    SCHEMA(health_fade_alpha, float, "C_CSPlayerPawnBase", "m_flHealthFadeAlpha");
    SCHEMA(death_cc_weight, float, "C_CSPlayerPawnBase", "m_flDeathCCWeight");
    SCHEMA(prev_round_end_time, float, "C_CSPlayerPawnBase", "m_flPrevRoundEndTime");
    SCHEMA(prev_match_end_time, float, "C_CSPlayerPawnBase", "m_flPrevMatchEndTime");
    SCHEMA(eye_angles, vec3_t, "C_CSPlayerPawnBase", "m_angEyeAngles");
    SCHEMA(next_think_push_away, float, "C_CSPlayerPawnBase", "m_fNextThinkPushAway");
    SCHEMA(should_autobuy_dm_weapons, bool, "C_CSPlayerPawnBase", "m_bShouldAutobuyDMWeapons");
    SCHEMA(should_autobuy_now, bool, "C_CSPlayerPawnBase", "m_bShouldAutobuyNow");
    SCHEMA(id_ent_index, int, "C_CSPlayerPawnBase", "m_iIDEntIndex");
    SCHEMA(target_item_ent_idx, int, "C_CSPlayerPawnBase", "m_iTargetItemEntIdx");
    SCHEMA(old_id_ent_index, int, "C_CSPlayerPawnBase", "m_iOldIDEntIndex");
    SCHEMA(current_music_start_time, float, "C_CSPlayerPawnBase", "m_flCurrentMusicStartTime");
    SCHEMA(music_round_start_time, float, "C_CSPlayerPawnBase", "m_flMusicRoundStartTime");
    SCHEMA(defer_start_music_warmup, bool, "C_CSPlayerPawnBase", "m_bDeferStartMusicOnWarmup");
    SCHEMA(cycle_latch, int, "C_CSPlayerPawnBase", "m_cycleLatch");
    SCHEMA(server_intended_cycle, float, "C_CSPlayerPawnBase", "m_serverIntendedCycle");
    SCHEMA(last_smoke_overlay_alpha, float, "C_CSPlayerPawnBase", "m_flLastSmokeOverlayAlpha");
    SCHEMA(last_smoke_age, float, "C_CSPlayerPawnBase", "m_flLastSmokeAge");
    SCHEMA(last_smoke_overlay_color, vec3_t, "C_CSPlayerPawnBase", "m_vLastSmokeOverlayColor");
    SCHEMA(player_smoked_fx, int, "C_CSPlayerPawnBase", "m_nPlayerSmokedFx");
    SCHEMA(player_inferno_body_fx, int, "C_CSPlayerPawnBase", "m_nPlayerInfernoBodyFx");
    SCHEMA(player_inferno_foot_fx, int, "C_CSPlayerPawnBase", "m_nPlayerInfernoFootFx");
    SCHEMA(next_mag_drop_time, float, "C_CSPlayerPawnBase", "m_flNextMagDropTime");
    SCHEMA(last_mag_drop_attachment_index, int, "C_CSPlayerPawnBase", "m_nLastMagDropAttachmentIndex");
    SCHEMA(last_alive_local_velocity, vec3_t, "C_CSPlayerPawnBase", "m_vecLastAliveLocalVelocity");
    SCHEMA(guardian_custom_xmark, bool, "C_CSPlayerPawnBase", "m_bGuardianShouldSprayCustomXMark");
    SCHEMA(original_controller, c_base_handle, "C_CSPlayerPawnBase", "m_hOriginalController");
};