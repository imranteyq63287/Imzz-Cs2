#include "../../../includes.h"
#include "../../config_system.h"
#include "../../../sdk/utils/math.h"
#include "../../menu/gui_base/hashes.hpp"
#include "../../../sdk/utils/memory.h"
#include "../../engine prediction/eprediction.hpp"
class c_skeleton_instace
{
public:
	std::byte pad_003[0x1CC]; //0x0000
public:
	int m_bone_count; //0x01CC
private:
	std::byte pad_002[0x18]; //0x01D0
public:
	int m_mask; //0x01E8
private:
	std::byte pad_001[0x4]; //0x01EC
public:
	matrix2x4_t* m_bone_cache; //0x01F0
	SCHEMA(m_model_state, c_model_state, ("CSkeletonInstance"), ("m_modelState"));

};
struct lag_record_t {
	vec3_t m_origin{};
	c_cs_player_pawn* m_pawn{};
	c_skeleton_instace* m_skeleton = nullptr;
	CBoneData m_bone_data[128];
	CBoneData m_bone_data_backup[128];
	float m_simulation_time{};
	vec3_t m_vec_mins{};
	vec3_t m_vec_maxs{};
	vec3_t m_vec_velocity{};

	bool m_throwing{};

	lag_record_t() {}

	inline lag_record_t(c_cs_player_pawn* pawn) {
		this->store(pawn);
	}

	inline void store(c_cs_player_pawn* pawn) {
		if (!pawn || !pawn->is_alive() || !pawn->is_player_pawn())
			return;

		auto scene_node = pawn->game_scene_node();
		if (!scene_node)
			return;

		auto skeleton_instance = pawn->game_scene_node()->get_skeleton_instance();
		if (!skeleton_instance)
			return;

		auto collision = pawn->collision();
		if (!collision)
			return;

		static auto setup_bones = reinterpret_cast<void(__fastcall*)(void*, int)>(g_memory->GetAbsoluteAddress((uint8_t*)utils::find_pattern(g_modules.client, "E8 ? ? ? ? 4C 8B 8B"), 0x1));

		m_pawn = pawn;

		m_skeleton = skeleton_instance;
		m_origin = scene_node->vec_abs_origin();

		setup_bones(m_skeleton, skeleton_instance->m_bone_count);

		std::memcpy(m_bone_data, skeleton_instance->m_bone_cache, sizeof(matrix2x4_t) * skeleton_instance->m_bone_count);

		m_simulation_time = pawn->simulation_time();
		m_vec_mins = collision->vec_mins();
		m_vec_maxs = collision->vec_maxs();
		m_vec_velocity = pawn->velocity();

		m_throwing = pawn->is_throwing();
	}

	inline void apply(c_cs_player_pawn* pawn) {
		if (!pawn || !pawn->is_alive() || !pawn->is_player_pawn())
			return;

		auto scene_node = pawn->game_scene_node();
		if (!scene_node)
			return;
		c_skeleton_instace* skeleton_instance{};

		skeleton_instance = scene_node->get_skeleton_instance();
		if (!skeleton_instance)
			return;

		std::memcpy(m_bone_data_backup, skeleton_instance->m_bone_cache, sizeof(matrix2x4_t) * skeleton_instance->m_bone_count);

		std::memcpy(skeleton_instance->m_bone_cache, m_bone_data, sizeof(matrix2x4_t) * skeleton_instance->m_bone_count);
	}

	inline void reset(c_cs_player_pawn* pawn) {
		if (!pawn || !pawn->is_alive() || !pawn->is_player_pawn())
			return;

		auto scene_node = pawn->game_scene_node();
		if (!scene_node)
			return;

		auto skeleton_instance = scene_node->get_skeleton_instance();
		if (!skeleton_instance)
			return;

		std::memcpy(skeleton_instance->m_bone_cache, m_bone_data_backup, sizeof(matrix2x4_t) * skeleton_instance->m_bone_count);
	}

	bool is_valid() {
		float dead_time = (g_prediction->get_local_data()->m_tick_base * INTERVAL_PER_TICK) - m_simulation_time;

		return dead_time < 0.2f;
	}
};
inline void vector_transform(vec3_t& in, matrix3x4_t& matrix, vec3_t& out) {
	out = {
		in.dot({ matrix[0][0], matrix[0][1], matrix[0][2] }) + matrix[0][3],
		in.dot({ matrix[1][0], matrix[1][1], matrix[1][2] }) + matrix[1][3],
		in.dot({ matrix[2][0], matrix[2][1], matrix[2][2] }) + matrix[2][3]
	};
}
inline vec3_t transform_point(matrix3x4_t matrix, vec3_t point)
{
	vec3_t result{ };
	vector_transform(point, matrix, result);
	return result;
}
enum e_hitboxes : std::uint32_t
{
	/* head */
	hitbox_head,
	hitbox_neck,
	/* body */
	hitbox_pelvis,
	hitbox_stomach,
	hitbox_chest,
	hitbox_lower_chest,
	hitbox_upper_chest,
	/* legs */
	hitbox_right_thigh,
	hitbox_left_thigh,
	hitbox_right_calf,
	hitbox_left_calf,
	hitbox_right_foot,
	hitbox_left_foot,
	/* arms */
	hitbox_right_hand,
	hitbox_left_hand,
	hitbox_right_upper_arm,
	hitbox_right_forearm,
	hitbox_left_upper_arm,
	hitbox_left_forearm,
	hitbox_max
};
struct aim_point_t {
	aim_point_t(vec3_t point, int hitbox, bool center = false) {
		this->m_point = point;
		this->m_hitbox = hitbox;
		this->m_center = center;
	}

	vec3_t m_point{};
	int m_hitbox{};
	bool m_center{};
};
inline int get_hitbox_from_menu(int hitbox) {
	switch (hitbox) {
	case hitbox_head:
		return 0;
	case hitbox_chest:
	case hitbox_lower_chest:
	case hitbox_upper_chest:
		return 1;
	case hitbox_pelvis:
	case hitbox_stomach:
		return 2;
	case hitbox_right_hand:
	case hitbox_left_hand:
	case hitbox_right_upper_arm:
	case hitbox_right_forearm:
	case hitbox_left_upper_arm:
	case hitbox_left_forearm:
		return 3;
	case hitbox_right_thigh:
	case hitbox_left_thigh:
	case hitbox_right_calf:
	case hitbox_left_calf:
		return 4;
	case hitbox_right_foot:
	case hitbox_left_foot:
		return 5;
	}

	return -1;
}
struct aim_target_t {
	lag_record_t* m_lag_record{};
	std::shared_ptr<aim_point_t> m_best_point;
	c_cs_player_pawn* m_pawn{ nullptr };

	aim_target_t() = default;

	inline aim_target_t(lag_record_t* record) {
		this->store(record);
	}

	inline void store(lag_record_t* record) {
		if (!record || !record->m_pawn || !record->m_pawn->is_player_pawn())
			return;

		m_lag_record = record;
		m_pawn = record->m_pawn;
	}

	inline void reset() {
		if (this == nullptr || (uintptr_t)this == 0xFFFFFFFFFFFFFFF7)
			return;

		m_lag_record = nullptr;
		m_pawn = nullptr;
		if (m_best_point)
			m_best_point.reset();
	}
};

struct c_points
{
	vec3_t m_vecPosition;
	bool m_bCenter;
};


struct hitbox_data_t
{
	hitbox_data_t() = default;

	int m_num_hitbox;
	int m_num_bone;
	const char* m_name_bone;
	int m_num_hitgroup;
	c_hitbox* m_hitbox_data;
	float m_radius;
	matrix3x4_t m_matrix;
	vec3_t m_mins;
	vec3_t m_maxs;
	bool m_invalid_data{ true };
};
	inline static float sostab = 0.2335f;
	inline std::vector<std::pair<vec3_t, std::string>> multipoint_debug_positions;
	inline std::mutex multipoint_mutex;
class c_ragebot {
private:
	typedef void(__fastcall* FnCalcSpread)(int16_t, int, int, std::uint32_t, float, float, float, float*, float*);
	inline static FnCalcSpread fnCalcSpread;
	//int next_cock_time = 0;
	float best_damage = 0.0f;

public:
	int next_cock_time = 0;
	std::vector<int> m_hitboxes;
	std::unordered_map<int, std::deque<lag_record_t>> m_lag_records{};
	std::unordered_map<int, std::shared_ptr<aim_target_t>> m_aim_targets;
	std::shared_ptr<aim_target_t> m_best_target;

	int radius_head = 0;
	void add_point(vec3_t pos, int bone, bool center, std::vector<aim_point_t>& point)
	{
		point.emplace_back(aim_point_t(pos, bone, center));
	}
	hitbox_data_t get_hitbox_data(c_cs_player_pawn* player_pawn, const int hitbox_id) {
		if (!player_pawn)
			return hitbox_data_t();

		auto game_scene = player_pawn->game_scene_node();
		if (!game_scene)
			return hitbox_data_t();

		auto skeleton = game_scene->get_skeleton_instance();
		if (!skeleton)
			return hitbox_data_t();

		auto& model_state = skeleton->m_model_state();
		auto& model = model_state.modelHandle;
		if (!model)
			return hitbox_data_t();

		auto hitbox_data = model->get_hitbox(hitbox_id);
		if (!hitbox_data)
			return hitbox_data_t();

		const auto bone_index = player_pawn->get_bone_index(hitbox_data->m_bone_name);
		const auto bones = model_state.bones;

		hitbox_data_t hitbox;
		hitbox.m_num_hitbox = hitbox_id;
		hitbox.m_num_bone = bone_index;
		hitbox.m_hitbox_data = hitbox_data;
		hitbox.m_radius = hitbox_data->m_shape_radius;
		hitbox.m_num_hitgroup = hitbox_data->m_hitgroup;
		hitbox.m_matrix = math::transform_to_matrix(bones[bone_index]);
		hitbox.m_mins = hitbox_data->m_vec_min;
		hitbox.m_maxs = hitbox_data->m_vec_maxs;
		hitbox.m_name_bone = hitbox_data->m_name;
		hitbox.m_invalid_data = false;

		return hitbox;
	}

	inline lag_record_t* get_last_record(int handle) {
		if (m_lag_records.find(handle) == m_lag_records.end())
			return nullptr;

		auto& records = m_lag_records[handle];

		if (records.empty())
			return nullptr;

		auto record = &records.back();
		if (record->is_valid())
			return nullptr;

		return record;
	}
	void ragebot(c_user_cmd* user_cmd); // old
	void run(c_csgo_input* pInput, c_user_cmd* pCmd); // recode
	void double_tap(c_user_cmd* user_cmd, c_base_player_weapon* weapon);
	void check_doubletap_status(c_base_player_weapon* weapon);
	lag_record_t* select_record(int handle);
	std::shared_ptr<aim_target_t> get_nearest_target();
	void find_targets();
	void select_target();
	aim_point_t select_points(lag_record_t* record, float& damage);
	bool multi_points(lag_record_t* record, int hitbox, std::vector<aim_point_t>& points);
	void store_records(int stage);
	const char* icondtbroken = reinterpret_cast<const char*>(ICON_FA_BATTERY_EMPTY);
};

inline const auto g_ragebot = std::make_unique<c_ragebot>();