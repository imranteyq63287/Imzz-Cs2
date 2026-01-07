#pragma once
#include "../../utils/virual.h"
#include "../../utils/vector.h"
#include "../../utils/utils.h"

#include <array>

struct ray_t
{
public:
	vec3_t m_start{};
	vec3_t m_end{};
	vec3_t m_mins{};
	vec3_t m_maxs{};
	std::byte pad_001[0x4];
	std::uint8_t type{};
};

static_assert(sizeof(ray_t) == 0x38);

struct surface_data_t
{
public:
	std::byte pad_001[0x8];
	float m_pen_data_modifier{};
	float m_dmg_modifier{};
	std::byte pad_002[0x4];
	int m_material{};
};

static_assert(sizeof(surface_data_t) == 0x18);

struct trace_hitbox_data_t
{
public:
	std::byte pad_001[0x38];
	int m_hit_group{};
	std::byte pad_002[0x4];
	int m_hitbox_id{};
};

static_assert(sizeof(trace_hitbox_data_t) == 0x44);

struct trace_array_element_t 
{
	std::byte pad_001[0x30];
};

struct trace_data_t 
{
	std::int32_t uk1{ };
	float uk2{ 52.0f };
	void* arr_pointer{ };
	std::int32_t uk3{ 128 };
	std::int32_t uk4{ static_cast<std::int32_t>(0x80000000) };
	std::array< trace_array_element_t, 0x80 > arr = { };
	std::byte pad_001[0x8];
	std::int64_t num_update{ };
	void* pointer_update_value{ };
	std::byte pad_002[0xC8];
	vec3_t start{ }, end{ };
	std::byte pad_003[0x50];
};

struct update_value_t 
{
	float m_previous_length_mod{ };
	float m_current_length_mod{ };
	std::byte pad_001[0x8];
	short m_handle_index{ };
	std::byte pad_002[0x6];
};

class c_cs_player_pawn;

struct game_trace_t
{
public:
	game_trace_t() = default;

	surface_data_t* get_surface_data_t();
	bool did_hit_world();

	void* m_surface{};

	c_cs_player_pawn* m_hit_entity{};
	trace_hitbox_data_t* m_hitbox_data{};

	std::byte pad_001[0x38];
	std::uint32_t m_contents{};
	std::byte pad_002[0x24];
	vec3_t m_start{};
	vec3_t m_end{};
	vec3_t m_normal{};
	vec3_t m_pos{};
	std::byte pad_003[0x4];
	float m_fraction{};
	std::byte pad_004[0x6];
	bool m_all_solid{};
	std::byte pad_005[0x4D];
};

static_assert(sizeof(game_trace_t) == 0x108);

struct trace_filter_t
{
public:
	std::byte pad_001[0x8];
	std::int64_t m_mask{};
	std::array<std::int64_t, 2> m_ptr{};
	std::array<std::int32_t, 4> m_skip_handles{};
	std::array<std::int16_t, 2> m_arr_collisions{};
	std::int16_t m_ptr2{};
	std::uint8_t m_ptr3{};
	std::uint8_t m_ptr4{};
	std::uint8_t m_ptr5{};

	trace_filter_t() = default;
	trace_filter_t(std::uint64_t mask, c_cs_player_pawn* entity, c_cs_player_pawn* player, int layer);
};

static_assert(sizeof(trace_filter_t) == 0x40);

class c_engine_trace
{
	using fn_init_trace_info = void(__fastcall*) (game_trace_t*);
	using fn_get_trace_info = void(__fastcall*) (trace_data_t*, game_trace_t*, float, void*);
	using fn_init_trace_t = trace_filter_t * (__fastcall*)(trace_filter_t&, void*, uint64_t, uint8_t, uint16_t);
	using fn_clip_trace_to_player_t = void(__fastcall*)(vec3_t&, vec3_t&, trace_filter_t*, game_trace_t*, float, int, float);
	using fn_create_trace_t = void(__fastcall*) (trace_data_t*, vec3_t, vec3_t, trace_filter_t, void*, void*, void*, void*, int);
	using fn_trace_shape = bool(__fastcall*)(c_engine_trace*, ray_t*, vec3_t*, vec3_t*, trace_filter_t*, game_trace_t*);
	using fn_clip_ray_entity = bool(__fastcall*)(c_engine_trace*, ray_t*, vec3_t*, vec3_t*, c_cs_player_pawn*, trace_filter_t*, game_trace_t*);
public:
	void init_trace(trace_filter_t& filter, c_cs_player_pawn* pawn, uint64_t mask, uint8_t layer, uint16_t unknown);
	void clip_trace_to_player(vec3_t& start, vec3_t& end, trace_filter_t* filter, game_trace_t* trace, float min, int length, float max);
	void create_trace(trace_data_t* const trace, vec3_t start, vec3_t end, const trace_filter_t& filter, int penetration_count);

	void init_trace_info(game_trace_t* const hit);
	void get_trace_info(trace_data_t* const trace, game_trace_t* const hit, const float unknown_float, void* unknown);

	bool trace_shape(ray_t* ray, vec3_t start, vec3_t end, trace_filter_t* filter, game_trace_t* game_trace);
	bool clip_ray_entity(ray_t* ray, vec3_t start, vec3_t end, c_cs_player_pawn* pawn, trace_filter_t* filter, game_trace_t* game_trace);
};