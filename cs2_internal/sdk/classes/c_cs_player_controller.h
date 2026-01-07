#pragma once

#include "../utils/schema/schema.h"
#include "../utils/typedefs/c_handle.h"
#include "c_entity_instance.h"

class c_base_player_controller : public c_entity_instance
{
public:
	SCHEMA(is_local_player_controller, bool, "CBasePlayerController", "m_bIsLocalPlayerController");
};

class c_cs_per_round_stats
{
public:
	SCHEMA(kills, int, "CSPerRoundStats_t", "m_iKills");
	SCHEMA(deaths, int, "CSPerRoundStats_t", "m_iDeaths");
	SCHEMA(assists, int, "CSPerRoundStats_t", "m_iAssists");
	SCHEMA(damage, int, "CSPerRoundStats_t", "m_iDamage");
	SCHEMA(equipment_value, int, "CSPerRoundStats_t", "m_iEquipmentValue");
	SCHEMA(money_saved, int, "CSPerRoundStats_t", "m_iMoneySaved");
	SCHEMA(kill_reward, int, "CSPerRoundStats_t", "m_iKillReward");
	SCHEMA(live_time, int, "CSPerRoundStats_t", "m_iLiveTime");
	SCHEMA(headshot_kills, int, "CSPerRoundStats_t", "m_iHeadShotKills");
	SCHEMA(objective, int, "CSPerRoundStats_t", "m_iObjective");
	SCHEMA(cash_earned, int, "CSPerRoundStats_t", "m_iCashEarned");
	SCHEMA(utility_damage, int, "CSPerRoundStats_t", "m_iUtilityDamage");
	SCHEMA(enemies_flashed, int, "CSPerRoundStats_t", "m_iEnemiesFlashed");

};

class c_cs_player_controller : public c_base_player_controller
{
public:
	SCHEMA(pawn_is_alive, bool, "CCSPlayerController", "m_bPawnIsAlive");
	SCHEMA(observer_pawn_handle, c_base_handle, ("CCSPlayerController"), ("m_hObserverPawn"));
	SCHEMA(in_game_money_services, void*, "CCSPlayerController", "m_pInGameMoneyServices");
	SCHEMA(inventory_services, void*, "CCSPlayerController", "m_pInventoryServices");
	SCHEMA(action_tracking_services, void*, "CCSPlayerController", "m_pActionTrackingServices");
	SCHEMA(damage_services, void*, "CCSPlayerController", "m_pDamageServices");
	SCHEMA(communication_abuse_mute, bool, "CCSPlayerController", "m_bHasCommunicationAbuseMute");
	SCHEMA(communication_mute_flags, uint32_t, "CCSPlayerController", "m_uiCommunicationMuteFlags");
	SCHEMA(crosshair_codes, const char*, "CCSPlayerController", "m_szCrosshairCodes");
	SCHEMA(pending_team_num, uint8_t, "CCSPlayerController", "m_iPendingTeamNum");
	SCHEMA(force_team_time, float, "CCSPlayerController", "m_flForceTeamTime");
	SCHEMA(comp_teammate_color, int, "CCSPlayerController", "m_iCompTeammateColor");
	SCHEMA(ping, int, "CCSPlayerController", "m_iPing");
	SCHEMA(ever_played_on_team, bool, "CCSPlayerController", "m_bEverPlayedOnTeam");
	SCHEMA(previous_force_join_team_time, float, "CCSPlayerController", "m_flPreviousForceJoinTeamTime");
	SCHEMA(clan_tag, const char*, "CCSPlayerController", "m_szClan");
	SCHEMA(coaching_team, int, "CCSPlayerController", "m_iCoachingTeam");
	SCHEMA(player_dominated, uint64_t, "CCSPlayerController", "m_nPlayerDominated");
	SCHEMA(player_dominating_me, uint64_t, "CCSPlayerController", "m_nPlayerDominatingMe");
	SCHEMA(competitive_ranking, int, "CCSPlayerController", "m_iCompetitiveRanking");
	SCHEMA(competitive_wins, int, "CCSPlayerController", "m_iCompetitiveWins");
	SCHEMA(competitive_rank_type, int8_t, "CCSPlayerController", "m_iCompetitiveRankType");
	SCHEMA(predicted_ranking_win, int, "CCSPlayerController", "m_iCompetitiveRankingPredicted_Win");
	SCHEMA(predicted_ranking_loss, int, "CCSPlayerController", "m_iCompetitiveRankingPredicted_Loss");
	SCHEMA(predicted_ranking_tie, int, "CCSPlayerController", "m_iCompetitiveRankingPredicted_Tie");
	SCHEMA(end_match_vote, int, "CCSPlayerController", "m_nEndMatchNextMapVote");
	SCHEMA(active_quest_id, uint16_t, "CCSPlayerController", "m_unActiveQuestId");
	SCHEMA(active_mission_period, uint32_t, "CCSPlayerController", "m_rtActiveMissionPeriod");
	SCHEMA(quest_progress_reason, int, "CCSPlayerController", "m_nQuestProgressReason");
	SCHEMA(tv_control_flags, uint32_t, "CCSPlayerController", "m_unPlayerTvControlFlags");
	SCHEMA(draft_index, int, "CCSPlayerController", "m_iDraftIndex");
	SCHEMA(queued_mode_disconnection_timestamp, uint32_t, "CCSPlayerController", "m_msQueuedModeDisconnectionTimestamp");
	SCHEMA(abandon_reason, uint32_t, "CCSPlayerController", "m_uiAbandonRecordedReason");
	SCHEMA(disconnection_reason, uint32_t, "CCSPlayerController", "m_eNetworkDisconnectionReason");
	SCHEMA(cannot_be_kicked, bool, "CCSPlayerController", "m_bCannotBeKicked");
	SCHEMA(ever_fully_connected, bool, "CCSPlayerController", "m_bEverFullyConnected");
	SCHEMA(abandon_allows_surrender, bool, "CCSPlayerController", "m_bAbandonAllowsSurrender");
	SCHEMA(abandon_offers_instant_surrender, bool, "CCSPlayerController", "m_bAbandonOffersInstantSurrender");
	SCHEMA(warning_printed, bool, "CCSPlayerController", "m_bDisconnection1MinWarningPrinted");
	SCHEMA(score_reported, bool, "CCSPlayerController", "m_bScoreReported");
	SCHEMA(disconnection_tick, int, "CCSPlayerController", "m_nDisconnectionTick");
	SCHEMA(controlling_bot, bool, "CCSPlayerController", "m_bControllingBot");
	SCHEMA(controlled_bot_this_round, bool, "CCSPlayerController", "m_bHasControlledBotThisRound");
	SCHEMA(been_controlled_by_player_this_round, bool, "CCSPlayerController", "m_bHasBeenControlledByPlayerThisRound");
	SCHEMA(bots_controlled_this_round, int, "CCSPlayerController", "m_nBotsControlledThisRound");
	SCHEMA(can_control_observed_bot, bool, "CCSPlayerController", "m_bCanControlObservedBot");
	SCHEMA(player_pawn_handle, c_base_handle, "CCSPlayerController", "m_hPlayerPawn");
	SCHEMA(pawn_health, int, "CCSPlayerController", "m_iPawnHealth");
	SCHEMA(pawn_armor, int, "CCSPlayerController", "m_iPawnArmor");
	SCHEMA(pawn_has_defuser, bool, "CCSPlayerController", "m_bPawnHasDefuser");
	SCHEMA(pawn_has_helmet, bool, "CCSPlayerController", "m_bPawnHasHelmet");
	SCHEMA(pawn_character_defindex, uint16_t, "CCSPlayerController", "m_nPawnCharacterDefIndex");
	SCHEMA(pawn_lifetime_start, int, "CCSPlayerController", "m_iPawnLifetimeStart");
	SCHEMA(pawn_lifetime_end, int, "CCSPlayerController", "m_iPawnLifetimeEnd");
	SCHEMA(pawn_bot_difficulty, int, "CCSPlayerController", "m_iPawnBotDifficulty");
	SCHEMA(original_controller_of_pawn, c_base_handle, "CCSPlayerController", "m_hOriginalControllerOfCurrentPawn");
	SCHEMA(score, int, "CCSPlayerController", "m_iScore");
	SCHEMA(mvp_reason, int, "CCSPlayerController", "m_eMvpReason");
	SCHEMA(mvp_music_kit_id, int, "CCSPlayerController", "m_iMusicKitID");
	SCHEMA(mvp_music_kit_mvp_count, int, "CCSPlayerController", "m_iMusicKitMVPs");
	SCHEMA(mvp_total, int, "CCSPlayerController", "m_iMVPs");
	SCHEMA(is_name_dirty, bool, "CCSPlayerController", "m_bIsPlayerNameDirty");
	SCHEMA(fire_bullet_seed_synced, bool, "CCSPlayerController", "m_bFireBulletsSeedSynchronized");
	SCHEMA(final_predicted_tick, int, "CBasePlayerController", "m_nFinalPredictedTick");
	SCHEMA(command_context, void*, "CBasePlayerController", "m_CommandContext"); // C_CommandContext*
	SCHEMA(button_toggles, uint64_t, "CBasePlayerController", "m_nInButtonsWhichAreToggles");
	SCHEMA(tick_base, uint32_t, "CBasePlayerController", "m_nTickBase");
	SCHEMA(pawn, c_base_handle, "CBasePlayerController", "m_hPawn");
	SCHEMA(known_team_mismatch, bool, "CBasePlayerController", "m_bKnownTeamMismatch");
	SCHEMA(predicted_pawn, c_base_handle, "CBasePlayerController", "m_hPredictedPawn");
	SCHEMA(splitscreen_slot, int, "CBasePlayerController", "m_nSplitScreenSlot");
	SCHEMA(splitscreen_owner, c_base_handle, "CBasePlayerController", "m_hSplitOwner");
	SCHEMA(splitscreen_players, void*, "CBasePlayerController", "m_hSplitScreenPlayers"); // c_utl_vector<c_base_handle>
	SCHEMA(is_hltv, bool, "CBasePlayerController", "m_bIsHLTV");
	SCHEMA(connected_state, int, "CBasePlayerController", "m_iConnected"); // PlayerConnectedState
	SCHEMA(bad_player_name, const char*, "CBasePlayerController", "m_iszPlayerName");
	SCHEMA(steam_id, uint64_t, "CBasePlayerController", "m_steamID");
	SCHEMA(is_local_controller, bool, "CBasePlayerController", "m_bIsLocalPlayerController");
	SCHEMA(desiredfov, uint32_t, "CBasePlayerController", "m_iDesiredFOV");
	SCHEMA(player_name, const char*, "CCSPlayerController", "m_sSanitizedPlayerName");

	c_cs_player_pawn* get_observer_pawn()
	{
		if (!observer_pawn_handle().is_valid())
			return nullptr;

		int index = observer_pawn_handle().get_entry_index();

		return interfaces::entity_system->get_base_entity<c_cs_player_pawn>(index);
	}

	void physics_run_think()
	{
		static auto physics_run_think_ = reinterpret_cast<void* (__fastcall*)(void*)>(utils::find_pattern_rel(g_modules.client, "E8 ? ? ? ? 48 8B D5 48 8B CE E8 ? ? ? ? 48 8B 06", 0x1));
	
		physics_run_think_(this);
	}
};