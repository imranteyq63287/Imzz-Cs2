#include "c_engine_trace.h"
#include "../../classes/c_cs_player_pawn.h"

surface_data_t* game_trace_t::get_surface_data_t()
{
	using fn_get_surface_data_t = std::uint64_t(__fastcall*)(void*);
	static fn_get_surface_data_t o_get_surface_data = reinterpret_cast<fn_get_surface_data_t>(utils::find_pattern_rel(g_modules.client, "E8 ? ? ? ? 48 85 C0 74 ? 44 38 68", 0x1));

	return reinterpret_cast<surface_data_t*>(o_get_surface_data(m_surface));
}

bool game_trace_t::did_hit_world()
{
	c_base_entity* world_entity = interfaces::entity_system->get_base_entity(0);

	if (!world_entity)
		return false;

	return reinterpret_cast<c_base_entity*>(m_hit_entity) == world_entity;
}

trace_filter_t::trace_filter_t(std::uint64_t mask, c_cs_player_pawn* entity, c_cs_player_pawn* player, int layer)
{
	m_mask = mask;
	m_ptr[0] = m_ptr[1] = 0;
	m_ptr2 = 7;
	m_ptr3 = layer;
	m_ptr4 = 0x49;
	m_ptr5 = 0;

	if (entity != nullptr)
	{
		m_skip_handles[0] = entity->get_ref_ehandle().get_entry_index();
		m_skip_handles[2] = entity->get_owner_handle_index();
		m_arr_collisions[0] = entity->collision()->collision_mask();
	}

	if (player != nullptr)
	{
		m_skip_handles[0] = player->get_ref_ehandle().get_entry_index();
		m_skip_handles[2] = player->get_owner_handle_index();
		m_arr_collisions[0] = player->collision()->collision_mask();
	}
}

void c_engine_trace::init_trace(trace_filter_t& filter, c_cs_player_pawn* pawn, uint64_t mask, uint8_t layer, uint16_t unknown)
{
	static fn_init_trace_t fn = reinterpret_cast<fn_init_trace_t>(utils::find_pattern(g_modules.client, "48 89 5C 24 ?? 48 89 74 24 ?? 57 48 83 EC ?? 0F B6 41 ?? 33 FF 24"));

	fn(filter, pawn, mask, layer, unknown);
}

void c_engine_trace::clip_trace_to_player(vec3_t& start, vec3_t& end, trace_filter_t* filter, game_trace_t* trace, float min, int length, float max) 
{
	static fn_clip_trace_to_player_t fn = reinterpret_cast<fn_clip_trace_to_player_t>(utils::find_pattern(g_modules.client, "48 8B C4 55 57 48 8D A8 ? ? ? ? 48 81 EC ? ? ? ? 48 89 58 ? 49 8B F8"));

	fn(start, end, filter, trace, min, length, max);
}

void c_engine_trace::create_trace(trace_data_t* const trace, vec3_t start, vec3_t end, const trace_filter_t& filter, int penetration_count) 
{
	static fn_create_trace_t fn = reinterpret_cast<fn_create_trace_t>(utils::find_pattern(g_modules.client, "48 89 5C 24 ? 48 89 6C 24 ? 48 89 74 24 ? 57 41 56 41 57 48 83 EC ? F2 0F 10 02"));

	fn(trace, start, end, filter, nullptr, nullptr, nullptr, nullptr, penetration_count);
}

void c_engine_trace::init_trace_info(game_trace_t* const hit)
{
	static fn_init_trace_info fn = reinterpret_cast<fn_init_trace_info>(utils::find_pattern(g_modules.client, "48 89 5C 24 ? 57 48 83 EC ? 48 8B D9 33 FF 48 8B 0D ? ? ? ? 48 85 C9"));

	fn(hit);
}

void c_engine_trace::get_trace_info(trace_data_t* const trace, game_trace_t* const hit, const float unknown_float, void* unknown)
{
	static fn_get_trace_info fn = reinterpret_cast<fn_get_trace_info>(utils::find_pattern(g_modules.client, "48 89 5C 24 ? 48 89 6C 24 ? 48 89 74 24 ? 57 48 83 EC ? 48 8B E9 0F 29 74 24 ? 48 8B CA"));

	fn(trace, hit, unknown_float, unknown);
}

bool c_engine_trace::trace_shape(ray_t* ray, vec3_t start, vec3_t end, trace_filter_t* filter, game_trace_t* game_trace)
{
	static fn_trace_shape o_trace_shape = reinterpret_cast<fn_trace_shape>(utils::find_pattern(g_modules.client, "48 89 5C 24 ? 48 89 4C 24 ? 55 57"));

	return o_trace_shape(this, ray, &start, &end, filter, game_trace);
}

bool c_engine_trace::clip_ray_entity(ray_t* ray, vec3_t start, vec3_t end, c_cs_player_pawn* pawn, trace_filter_t* filter, game_trace_t* game_trace)
{
	using fn_clip_ray_entity = bool(__fastcall*)(c_engine_trace*, ray_t*, vec3_t*, vec3_t*, c_cs_player_pawn*, trace_filter_t*, game_trace_t*);
	static fn_clip_ray_entity o_clip_ray_entity = reinterpret_cast<fn_clip_ray_entity>(utils::find_pattern(g_modules.client, "48 8B C4 48 89 58 ? 48 89 48 ? 55 56 57 41 54 41 56"));

	return o_clip_ray_entity(this, ray, &start, &end, pawn, filter, game_trace); // 123
}