#pragma once

#include "../utils/vector.h"
#include "../utils/utils.h"
#include "../utils/schema/schema.h"
#include "../interfaces/vtables/c_csgo_input.h"

class c_player_movement_service 
{
public:
	SCHEMA(impulse, int, "CPlayer_MovementServices", "m_nImpulse");
	SCHEMA(queued_button_down_mask, uint64_t, "CPlayer_MovementServices", "m_nQueuedButtonDownMask");
	SCHEMA(queued_button_change_mask, uint64_t, "CPlayer_MovementServices", "m_nQueuedButtonChangeMask");
	SCHEMA(button_double_pressed, uint64_t, "CPlayer_MovementServices", "m_nButtonDoublePressed");
	SCHEMA(button_pressed_cmd_number, uint32_t*, "CPlayer_MovementServices", "m_pButtonPressedCmdNumber"); // uint32[64]
	SCHEMA(last_command_number_processed, uint32_t, "CPlayer_MovementServices", "m_nLastCommandNumberProcessed");
	SCHEMA(toggle_button_down_mask, uint64_t, "CPlayer_MovementServices", "m_nToggleButtonDownMask");
	SCHEMA(maxspeed, float, "CPlayer_MovementServices", "m_flMaxspeed");
	SCHEMA(force_subtick_move_when, float*, "CPlayer_MovementServices", "m_arrForceSubtickMoveWhen"); // float[4]
	SCHEMA(forward_move, float, "CPlayer_MovementServices", "m_flForwardMove");
	SCHEMA(left_move, float, "CPlayer_MovementServices", "m_flLeftMove");
	SCHEMA(up_move, float, "CPlayer_MovementServices", "m_flUpMove");
	SCHEMA(last_movement_impulses, vec3_t, "CPlayer_MovementServices", "m_vecLastMovementImpulses"); // Vector
	SCHEMA(old_view_angles, vec3_t, "CPlayer_MovementServices", "m_vecOldViewAngles"); // QAngle
	SCHEMA(step_sound_time, float, "CPlayer_MovementServices_Humanoid", "m_flStepSoundTime");
	SCHEMA(fall_velocity, float, "CPlayer_MovementServices_Humanoid", "m_flFallVelocity");
	SCHEMA(in_crouch, bool, "CPlayer_MovementServices_Humanoid", "m_bInCrouch");
	SCHEMA(crouch_state, uint32_t, "CPlayer_MovementServices_Humanoid", "m_nCrouchState");
	SCHEMA(crouch_transition_start_time, float, "CPlayer_MovementServices_Humanoid", "m_flCrouchTransitionStartTime"); // GameTime_t
	SCHEMA(ducked, bool, "CPlayer_MovementServices_Humanoid", "m_bDucked");
	SCHEMA(ducking, bool, "CPlayer_MovementServices_Humanoid", "m_bDucking");
	SCHEMA(in_duck_jump, bool, "CPlayer_MovementServices_Humanoid", "m_bInDuckJump");
	SCHEMA(ground_normal, vec3_t, "CPlayer_MovementServices_Humanoid", "m_groundNormal"); // Vector
	SCHEMA(surface_friction, float, "CPlayer_MovementServices_Humanoid", "m_flSurfaceFriction");
	SCHEMA(surface_props, uint32_t, "CPlayer_MovementServices_Humanoid", "m_surfaceProps"); // CUtlStringToken
	SCHEMA(stepside, int, "CPlayer_MovementServices_Humanoid", "m_nStepside");
	SCHEMA(ladder_normal, vec3_t, "CCSPlayer_MovementServices", "m_vecLadderNormal"); // Vector
	SCHEMA(ladder_surface_prop_index, int, "CCSPlayer_MovementServices", "m_nLadderSurfacePropIndex");
	SCHEMA(duck_amount, float, "CCSPlayer_MovementServices", "m_flDuckAmount");
	SCHEMA(duck_speed, float, "CCSPlayer_MovementServices", "m_flDuckSpeed");
	SCHEMA(duck_override, bool, "CCSPlayer_MovementServices", "m_bDuckOverride");
	SCHEMA(desires_duck, bool, "CCSPlayer_MovementServices", "m_bDesiresDuck");
	SCHEMA(duck_offset, float, "CCSPlayer_MovementServices", "m_flDuckOffset");
	SCHEMA(duck_time_msecs, uint32_t, "CCSPlayer_MovementServices", "m_nDuckTimeMsecs");
	SCHEMA(duck_jump_time_msecs, uint32_t, "CCSPlayer_MovementServices", "m_nDuckJumpTimeMsecs");
	SCHEMA(jump_time_msecs, uint32_t, "CCSPlayer_MovementServices", "m_nJumpTimeMsecs");
	SCHEMA(last_duck_time, float, "CCSPlayer_MovementServices", "m_flLastDuckTime");
	SCHEMA(last_position_at_full_crouch_speed, vec3_t, "CCSPlayer_MovementServices", "m_vecLastPositionAtFullCrouchSpeed"); // Vector2D
	SCHEMA(duck_until_on_ground, bool, "CCSPlayer_MovementServices", "m_duckUntilOnGround");
	SCHEMA(has_walk_moved_since_last_jump, bool, "CCSPlayer_MovementServices", "m_bHasWalkMovedSinceLastJump");
	SCHEMA(in_stuck_test, bool, "CCSPlayer_MovementServices", "m_bInStuckTest");
	SCHEMA(stuck_check_time, float, "CCSPlayer_MovementServices", "m_flStuckCheckTime");
	SCHEMA(trace_count, int, "CCSPlayer_MovementServices", "m_nTraceCount");
	SCHEMA(stuck_last, int, "CCSPlayer_MovementServices", "m_StuckLast");
	SCHEMA(speed_cropped, bool, "CCSPlayer_MovementServices", "m_bSpeedCropped");
	SCHEMA(ground_move_efficiency, float, "CCSPlayer_MovementServices", "m_flGroundMoveEfficiency");
	SCHEMA(old_water_level, int, "CCSPlayer_MovementServices", "m_nOldWaterLevel");
	SCHEMA(water_entry_time, float, "CCSPlayer_MovementServices", "m_flWaterEntryTime");
	SCHEMA(vec_forward, vec3_t, "CCSPlayer_MovementServices", "m_vecForward");
	SCHEMA(vec_left, vec3_t, "CCSPlayer_MovementServices", "m_vecLeft");
	SCHEMA(vec_up, vec3_t, "CCSPlayer_MovementServices", "m_vecUp");
	SCHEMA(gamecode_has_moved_player_after_command, int, "CCSPlayer_MovementServices", "m_nGameCodeHasMovedPlayerAfterCommand");
	SCHEMA(old_jump_pressed, bool, "CCSPlayer_MovementServices", "m_bOldJumpPressed");
	SCHEMA(jump_pressed_time, float, "CCSPlayer_MovementServices", "m_flJumpPressedTime");
	SCHEMA(stash_grenade_param_when, float, "CCSPlayer_MovementServices", "m_fStashGrenadeParameterWhen"); // GameTime_t
	SCHEMA(button_down_mask_prev, uint64_t, "CCSPlayer_MovementServices", "m_nButtonDownMaskPrev");
	SCHEMA(offset_tick_complete_time, float, "CCSPlayer_MovementServices", "m_flOffsetTickCompleteTime");
	SCHEMA(offset_tick_stashed_speed, float, "CCSPlayer_MovementServices", "m_flOffsetTickStashedSpeed");
	SCHEMA(stamina, float, "CCSPlayer_MovementServices", "m_flStamina");
	SCHEMA(height_at_jump_start, float, "CCSPlayer_MovementServices", "m_flHeightAtJumpStart");
	SCHEMA(max_jump_height_this_jump, float, "CCSPlayer_MovementServices", "m_flMaxJumpHeightThisJump");
	SCHEMA(max_jump_height_last_jump, float, "CCSPlayer_MovementServices", "m_flMaxJumpHeightLastJump");
	SCHEMA(stamina_at_jump_start, float, "CCSPlayer_MovementServices", "m_flStaminaAtJumpStart");
	SCHEMA(accumulated_jump_error, float, "CCSPlayer_MovementServices", "m_flAccumulatedJumpError");

	void run_command(c_user_cmd* user_cmd)
	{
		using fn_run_command = void(__fastcall*)(c_player_movement_service*, c_user_cmd*);
		static fn_run_command run_command = reinterpret_cast<fn_run_command>(utils::find_pattern(g_modules.client, "48 8B C4 48 81 EC ? ? ? ? 48 89 58 ? 48 89 70 ? 48 8B F1"));

		run_command(this, user_cmd);
	}

	void set_prediction_command(c_user_cmd* user_cmd)
	{
		using fn_set_prediction_command = void(__fastcall*)(c_player_movement_service*, c_user_cmd*);
		static auto set_prediction_command = reinterpret_cast<fn_set_prediction_command>(utils::find_pattern(g_modules.client, "48 89 5C 24 ? 57 48 83 EC ? 48 8B DA E8 ? ? ? ? 48 8B F8 48 85 C0 74"));

		set_prediction_command(this, user_cmd);
	}

	void reset_prediction_command()
	{
		using fn_reset_prediction_command = void(__fastcall*)(c_player_movement_service*);
		static auto reset_prediction_command = reinterpret_cast<fn_reset_prediction_command>(utils::find_pattern(g_modules.client, "48 83 EC ? B9 ? ? ? ? E8 ? ? ? ? 48 C7 05"));

		reset_prediction_command(this);
	}
};