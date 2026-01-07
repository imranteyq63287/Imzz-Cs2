#pragma once

#include "../utils/schema/schema.h"
#include "../interfaces/interfaces.h"
#include "../utils/vector.h"

class c_cs_gamerules
{
public:
	SCHEMA(freeze_period, bool, "C_CSGameRules", "m_bFreezePeriod");
	SCHEMA(warmup_period, bool, "C_CSGameRules", "m_bWarmupPeriod");
	SCHEMA(warmup_period_end, float, "C_CSGameRules", "m_fWarmupPeriodEnd");
	SCHEMA(warmup_period_start, float, "C_CSGameRules", "m_fWarmupPeriodStart");
	SCHEMA(server_paused, bool, "C_CSGameRules", "m_bServerPaused");
	SCHEMA(terrorist_time_out_active, bool, "C_CSGameRules", "m_bTerroristTimeOutActive");
	SCHEMA(ct_time_out_active, bool, "C_CSGameRules", "m_bCTTimeOutActive");
	SCHEMA(terrorist_time_out_remaining, float, "C_CSGameRules", "m_flTerroristTimeOutRemaining");
	SCHEMA(ct_time_out_remaining, float, "C_CSGameRules", "m_flCTTimeOutRemaining");
	SCHEMA(terrorist_time_outs, int32_t, "C_CSGameRules", "m_nTerroristTimeOuts");
	SCHEMA(ct_time_outs, int32_t, "C_CSGameRules", "m_nCTTimeOuts");
	SCHEMA(technical_time_out, bool, "C_CSGameRules", "m_bTechnicalTimeOut");
	SCHEMA(match_waiting_for_resume, bool, "C_CSGameRules", "m_bMatchWaitingForResume");
	SCHEMA(round_time, int32_t, "C_CSGameRules", "m_iRoundTime");
	SCHEMA(match_start_time, float, "C_CSGameRules", "m_fMatchStartTime");
	SCHEMA(round_start_time, float, "C_CSGameRules", "m_fRoundStartTime");
	SCHEMA(restart_round_time, float, "C_CSGameRules", "m_flRestartRoundTime");
	SCHEMA(game_restart, bool, "C_CSGameRules", "m_bGameRestart");
	SCHEMA(game_start_time, float, "C_CSGameRules", "m_flGameStartTime");
	SCHEMA(time_until_next_phase_starts, float, "C_CSGameRules", "m_timeUntilNextPhaseStarts");
	SCHEMA(game_phase, int32_t, "C_CSGameRules", "m_gamePhase");
	SCHEMA(total_rounds_played, int32_t, "C_CSGameRules", "m_totalRoundsPlayed");
	SCHEMA(rounds_played_this_phase, int32_t, "C_CSGameRules", "m_nRoundsPlayedThisPhase");
	SCHEMA(overtime_playing, int32_t, "C_CSGameRules", "m_nOvertimePlaying");
	SCHEMA(hostages_remaining, int32_t, "C_CSGameRules", "m_iHostagesRemaining");
	SCHEMA(any_hostage_reached, bool, "C_CSGameRules", "m_bAnyHostageReached");
	SCHEMA(map_has_bomb_target, bool, "C_CSGameRules", "m_bMapHasBombTarget");
	SCHEMA(map_has_rescue_zone, bool, "C_CSGameRules", "m_bMapHasRescueZone");
	SCHEMA(map_has_buy_zone, bool, "C_CSGameRules", "m_bMapHasBuyZone");
	SCHEMA(is_queued_matchmaking, bool, "C_CSGameRules", "m_bIsQueuedMatchmaking");
	SCHEMA(queued_matchmaking_mode, int32_t, "C_CSGameRules", "m_nQueuedMatchmakingMode");
	SCHEMA(is_valve_ds, bool, "C_CSGameRules", "m_bIsValveDS");
	SCHEMA(logo_map, bool, "C_CSGameRules", "m_bLogoMap");
	SCHEMA(play_all_step_sounds_on_server, bool, "C_CSGameRules", "m_bPlayAllStepSoundsOnServer");
	SCHEMA(spectator_slot_count, int32_t, "C_CSGameRules", "m_iSpectatorSlotCount");
	SCHEMA(match_device, int32_t, "C_CSGameRules", "m_MatchDevice");
	SCHEMA(has_match_started, bool, "C_CSGameRules", "m_bHasMatchStarted");
	SCHEMA(next_map_in_mapgroup, int32_t, "C_CSGameRules", "m_nNextMapInMapgroup");
	SCHEMA(tournament_event_name, char, "C_CSGameRules", "m_szTournamentEventName");
	SCHEMA(tournament_event_stage, char, "C_CSGameRules", "m_szTournamentEventStage");
	SCHEMA(match_stat_txt, char, "C_CSGameRules", "m_szMatchStatTxt");
	SCHEMA(tournament_predictions_txt, char, "C_CSGameRules", "m_szTournamentPredictionsTxt");
	SCHEMA(tournament_predictions_pct, int32_t, "C_CSGameRules", "m_nTournamentPredictionsPct");
	SCHEMA(cmm_item_drop_reveal_start_time, float, "C_CSGameRules", "m_flCMMItemDropRevealStartTime");
	SCHEMA(cmm_item_drop_reveal_end_time, float, "C_CSGameRules", "m_flCMMItemDropRevealEndTime");
	SCHEMA(is_dropping_items, bool, "C_CSGameRules", "m_bIsDroppingItems");
	SCHEMA(is_quest_eligible, bool, "C_CSGameRules", "m_bIsQuestEligible");
	SCHEMA(is_hltv_active, bool, "C_CSGameRules", "m_bIsHltvActive");
	SCHEMA(prohibited_item_indices, uint16_t, "C_CSGameRules", "m_arrProhibitedItemIndices");
	SCHEMA(tournament_active_caster_accounts, uint32_t, "C_CSGameRules", "m_arrTournamentActiveCasterAccounts");
	SCHEMA(num_best_of_maps, int32_t, "C_CSGameRules", "m_numBestOfMaps");
	SCHEMA(halloween_mask_list_seed, int32_t, "C_CSGameRules", "m_nHalloweenMaskListSeed");
	SCHEMA(bomb_dropped, bool*, "C_CSGameRules", "m_bBombDropped");
	SCHEMA(bomb_planted, bool, "C_CSGameRules", "m_bBombPlanted");
	SCHEMA(round_win_status, int32_t, "C_CSGameRules", "m_iRoundWinStatus");
	SCHEMA(round_win_reason, int32_t, "C_CSGameRules", "m_eRoundWinReason");
	SCHEMA(t_cant_buy, bool, "C_CSGameRules", "m_bTCantBuy");
	SCHEMA(ct_cant_buy, bool, "C_CSGameRules", "m_bCTCantBuy");
	SCHEMA(match_stats_round_results, int32_t, "C_CSGameRules", "m_iMatchStats_RoundResults");
	SCHEMA(match_stats_players_alive_ct, int32_t, "C_CSGameRules", "m_iMatchStats_PlayersAlive_CT");
	SCHEMA(match_stats_players_alive_t, int32_t, "C_CSGameRules", "m_iMatchStats_PlayersAlive_T");
	SCHEMA(team_respawn_wave_times, float, "C_CSGameRules", "m_TeamRespawnWaveTimes");
	SCHEMA(next_respawn_wave, float, "C_CSGameRules", "m_flNextRespawnWave");
	SCHEMA(minimap_mins, vec3_t, "C_CSGameRules", "m_vMinimapMins");
	SCHEMA(minimap_maxs, vec3_t, "C_CSGameRules", "m_vMinimapMaxs");
	SCHEMA(minimap_vertical_section_heights, float, "C_CSGameRules", "m_MinimapVerticalSectionHeights");
	SCHEMA(spawned_terror_hunt_heavy, bool, "C_CSGameRules", "m_bSpawnedTerrorHuntHeavy");
	SCHEMA(local_match_id, uint64_t, "C_CSGameRules", "m_ullLocalMatchID");
	SCHEMA(end_match_map_group_vote_types, int32_t, "C_CSGameRules", "m_nEndMatchMapGroupVoteTypes");
	SCHEMA(end_match_map_group_vote_options, int32_t, "C_CSGameRules", "m_nEndMatchMapGroupVoteOptions");
	SCHEMA(end_match_map_vote_winner, int32_t, "C_CSGameRules", "m_nEndMatchMapVoteWinner");
	SCHEMA(num_consecutive_ct_loses, int32_t, "C_CSGameRules", "m_iNumConsecutiveCTLoses");
	SCHEMA(num_consecutive_terrorist_loses, int32_t, "C_CSGameRules", "m_iNumConsecutiveTerroristLoses");
	SCHEMA(mark_client_stop_record_at_round_end, bool, "C_CSGameRules", "m_bMarkClientStopRecordAtRoundEnd");
	SCHEMA(match_aborted_early_reason, int32_t, "C_CSGameRules", "m_nMatchAbortedEarlyReason");
	SCHEMA(has_triggered_round_start_music, bool, "C_CSGameRules", "m_bHasTriggeredRoundStartMusic");
	SCHEMA(switching_teams_at_round_reset, bool, "C_CSGameRules", "m_bSwitchingTeamsAtRoundReset");
	SCHEMA(game_mode_rules, void*, "C_CSGameRules", "m_pGameModeRules");
	SCHEMA(retake_rules, void*, "C_CSGameRules", "m_RetakeRules");
	SCHEMA(match_end_count, uint8_t, "C_CSGameRules", "m_nMatchEndCount");
	SCHEMA(t_team_intro_variant, int32_t, "C_CSGameRules", "m_nTTeamIntroVariant");
	SCHEMA(ct_team_intro_variant, int32_t, "C_CSGameRules", "m_nCTTeamIntroVariant");
	SCHEMA(team_intro_period, bool, "C_CSGameRules", "m_bTeamIntroPeriod");
	SCHEMA(round_end_winner_team, int32_t, "C_CSGameRules", "m_iRoundEndWinnerTeam");
	SCHEMA(round_end_reason, int32_t, "C_CSGameRules", "m_eRoundEndReason");
	SCHEMA(round_end_show_timer_defend, bool, "C_CSGameRules", "m_bRoundEndShowTimerDefend");
	SCHEMA(round_end_timer_time, int32_t, "C_CSGameRules", "m_iRoundEndTimerTime");
	SCHEMA(round_end_fun_fact_token, const char*, "C_CSGameRules", "m_sRoundEndFunFactToken");
	SCHEMA(round_end_fun_fact_player_slot, int32_t, "C_CSGameRules", "m_iRoundEndFunFactPlayerSlot");
	SCHEMA(round_end_fun_fact_data1, int32_t, "C_CSGameRules", "m_iRoundEndFunFactData1");
	SCHEMA(round_end_fun_fact_data2, int32_t, "C_CSGameRules", "m_iRoundEndFunFactData2");
	SCHEMA(round_end_fun_fact_data3, int32_t, "C_CSGameRules", "m_iRoundEndFunFactData3");
	SCHEMA(round_end_message, const char*, "C_CSGameRules", "m_sRoundEndMessage");
	SCHEMA(round_end_player_count, int32_t, "C_CSGameRules", "m_iRoundEndPlayerCount");
	SCHEMA(round_end_no_music, bool, "C_CSGameRules", "m_bRoundEndNoMusic");
	SCHEMA(round_end_legacy, int32_t, "C_CSGameRules", "m_iRoundEndLegacy");
	SCHEMA(round_end_count, uint8_t, "C_CSGameRules", "m_nRoundEndCount");
	SCHEMA(round_start_round_number, int32_t, "C_CSGameRules", "m_iRoundStartRoundNumber");
	SCHEMA(round_start_count, uint8_t, "C_CSGameRules", "m_nRoundStartCount");
	SCHEMA(last_perf_sample_time, double, "C_CSGameRules", "m_flLastPerfSampleTime");
};

inline c_cs_gamerules* get_gamerules() 
{
	static auto gamerules = reinterpret_cast<c_cs_gamerules*>(interfaces::schema_system->find_type_scope_for_module("client.dll")->find_declared_class("C_CSGameRules"));

	return gamerules;
}