#include "auto_wall.h"

void c_auto_wall::scale_damage(int hitgroup, c_cs_player_pawn* entity, c_cs_weapon_base_v_data* weapon_data, float& damage)
{
	static convar_t* mp_damage_scale_ct_head = interfaces::cvar->get_by_name("mp_damage_scale_ct_head");
	static convar_t* mp_damage_scale_t_head = interfaces::cvar->get_by_name("mp_damage_scale_t_head");
	static convar_t* mp_damage_scale_ct_body = interfaces::cvar->get_by_name("mp_damage_scale_ct_body");
	static convar_t* mp_damage_scale_t_body = interfaces::cvar->get_by_name("mp_damage_scale_t_body");

	float damage_scale_ct_head = mp_damage_scale_ct_head->get_float();
	float damage_scale_t_head = mp_damage_scale_t_head->get_float();
	float damage_scale_ct_body = mp_damage_scale_ct_body->get_float();
	float damage_scale_t_body = mp_damage_scale_t_body->get_float();

	const bool is_ct = entity->team_num() == 3, is_t = entity->team_num() == 2;

	float head_damage_scale = is_ct ? damage_scale_ct_head : is_t ? damage_scale_t_head : 1.0f;
	const float body_damage_scale = is_ct ? damage_scale_ct_body : is_t ? damage_scale_t_body : 1.0f;

	switch (hitgroup)
	{
	case hitgroup_head:
		damage *= weapon_data->headshot_multiplier() * head_damage_scale;
		break;
	case hitgroup_chest:
	case hitgroup_left_hand:
	case hitgroup_right_hand:
	case hitgroup_neck:
		damage *= body_damage_scale;
		break;
	case hitgroup_stomach:
		damage *= 1.25f * body_damage_scale;
		break;
	case hitgroup_left_leg:
	case hitgroup_right_leg:
		damage *= 0.75f * body_damage_scale;
		break;
	default:
		break;
	}

	if (!entity->has_armor(hitgroup))
		return;

	float heavy_armor_bonus = 1.0f, armor_bonus = 0.5f, armor_ratio = weapon_data->armor_ratio() * 0.5f;
	float damage_to_health = damage * armor_ratio;
	const float damage_to_armor = (damage - damage_to_health) * (heavy_armor_bonus * armor_bonus);

	if (damage_to_armor > static_cast<float>(entity->armor_value()))
		damage_to_health = damage - static_cast<float>(entity->armor_value()) / armor_bonus;

	damage = damage_to_health;
}
struct BulletData {
	float damage;
	float penetration;
	float range_modifier;
	float range;
	int penetration_count;
	bool failed;

	BulletData(float dmg, float pen, float range_mod, float rng, int pen_count) noexcept
		: damage(dmg)
		, penetration(pen)
		, range_modifier(range_mod)
		, range(rng)
		, penetration_count(pen_count)
		, failed(false) {
	}
};
bool c_auto_wall::fire_bullet(vec3_t start, vec3_t end, c_cs_player_pawn* target, c_cs_weapon_base_v_data* weapon_data, penetration_data_t& pen_data, bool is_taser)
{
	c_cs_player_pawn* local_player = g_ctx.local;

	if (!local_player)
		return false;

	trace_data_t trace_data = trace_data_t();
	trace_data.arr_pointer = &trace_data.arr;

	vec3_t direction = end - start;

	direction.normalized();

	vec3_t end_pos = direction * weapon_data->range();

	const trace_filter_t filter(0x1C300B, local_player, nullptr, 3);
	interfaces::engine_trace->create_trace(&trace_data, start, end_pos, filter, 4);

	handle_bulllet_penetration_data_t handle_bullet_data = handle_bulllet_penetration_data_t(
		static_cast<float>(weapon_data->damage()),
		weapon_data->penetration(),
		weapon_data->range(),
		weapon_data->range_modifier(),
		4, false
	);

	float max_range = weapon_data->range();
	float trace_length = 0.0f;
	pen_data.m_damage = static_cast<float>(weapon_data->damage());

	for (int i = 0; i < trace_data.num_update; i++)
	{
		update_value_t* modulate_values = reinterpret_cast<update_value_t* const>(reinterpret_cast<std::uintptr_t>(trace_data.pointer_update_value) + i * sizeof(update_value_t));

		game_trace_t game_trace;
		interfaces::engine_trace->init_trace_info(&game_trace);
		interfaces::engine_trace->get_trace_info(&trace_data, &game_trace, 0.0f, reinterpret_cast<void*>(reinterpret_cast<std::uintptr_t>(trace_data.arr.data()) + sizeof(trace_array_element_t) * (modulate_values->m_handle_index & ENT_ENTRY_MASK)));

		max_range -= trace_length;

		if (game_trace.m_fraction == 1.0f)
			break;

		trace_length += game_trace.m_fraction * max_range;
		pen_data.m_damage *= std::powf(weapon_data->range_modifier(), trace_length / 500.f);

		if (trace_length > 3000.f)
			break;

		if (game_trace.m_hit_entity && game_trace.m_hit_entity->is_player_pawn() && game_trace.m_hit_entity->team_num() != g_ctx.local->team_num()) {

			if (pen_data.m_damage < 1.0f)
				continue;

			scale_damage(game_trace.m_hitbox_data->m_hit_group, game_trace.m_hit_entity, weapon_data, pen_data.m_damage);

			pen_data.m_hitbox = game_trace.m_hitbox_data->m_hitbox_id;
			pen_data.m_penetrated = i == 0;

			if (is_taser && pen_data.m_penetrated)
				break;

			return true;
		}

		if (handle_bullet_penetration(&trace_data, &handle_bullet_data, modulate_values, false))
			return false;

		pen_data.m_damage = handle_bullet_data.m_damage;
	}

	return false;
}
float c_auto_wall::penetration_damage(
    const vec3_t& local_pos,
    const vec3_t& target_pos,
    c_base_entity* local,
    c_cs_player_pawn* local_pawn,
    c_base_entity* target,
    c_cs_player_pawn* target_pawn,
    bool has_heavy_armor,
    int target_team, c_cs_weapon_base_v_data* weapon_data)
{
    constexpr float MAX_TRACE_LENGTH = 3000.f;
    constexpr float DAMAGE_SCALE = 500.f;
    constexpr uint32_t TARGET_CONTENTS = 268828672;

    trace_data_t trace_data{};
    trace_data.arr_pointer = &trace_data.arr;

    __m128 start = _mm_set_ps(0, local_pos.z, local_pos.y, local_pos.x);
    __m128 end = _mm_set_ps(0, target_pos.z, target_pos.y, target_pos.x);
    __m128 dir = _mm_sub_ps(end, start);

    __m128 length_sq = _mm_dp_ps(dir, dir, 0x7F);
    __m128 inv_length = _mm_rsqrt_ps(length_sq);
    dir = _mm_mul_ps(dir, inv_length);
    if (!weapon_data)
        return 0;

    if ((uintptr_t)weapon_data <= 0x0000000000000001)
        return 0;

    __m128 scaled_dir = _mm_mul_ps(dir, _mm_set1_ps(weapon_data->range()));
    vec3_t end_pos;
    alignas(16) float temp[4];
    _mm_storeu_ps(temp, scaled_dir);
    end_pos = vec3_t(temp[0], temp[1], temp[2]);

    trace_filter_t filter(0x1C300B, local_pawn, nullptr, 4);

    interfaces::engine_trace->create_trace(&trace_data, local_pos, end_pos, filter, 4);

    BulletData bullet_data(
        static_cast<float>(weapon_data->damage()),
        weapon_data->penetration(),
        weapon_data->range_modifier(),
        weapon_data->range(),
        4
    );

    float current_damage = static_cast<float>(weapon_data->damage());
    float trace_length = 0.f;
    float max_range = weapon_data->range();

    __m128 damage_vec = _mm_set1_ps(current_damage);
    __m128 range_mod_vec = _mm_set1_ps(weapon_data->range_modifier());
    __m128 trace_scale = _mm_set1_ps(1.0f / DAMAGE_SCALE);

    if (trace_data.num_update > 0) {
        for (int i = 0; i < trace_data.num_update; ++i) {
            auto* value = reinterpret_cast<update_value_t*>(
                reinterpret_cast<std::uintptr_t>(trace_data.pointer_update_value) +
                i * sizeof(update_value_t)
                );

            game_trace_t game_trace{};
            interfaces::engine_trace->init_trace_info(&game_trace);
			interfaces::engine_trace->get_trace_info(
                &trace_data,
                &game_trace,
                0.0f,
                reinterpret_cast<void*>(
                    reinterpret_cast<std::uintptr_t>(trace_data.arr.data()) +
                    sizeof(trace_array_element_t) * (value->m_handle_index & 0x7fffu)
                    )
            );

            max_range -= trace_length;
            if (game_trace.m_fraction == 1.0f) break;

            trace_length += game_trace.m_fraction * max_range;

            __m128 length_factor = _mm_mul_ps(_mm_set1_ps(trace_length), trace_scale);
            __m128 power = _mm_pow_ps(range_mod_vec, length_factor);
            damage_vec = _mm_mul_ps(damage_vec, power);
            _mm_store_ss(&current_damage, damage_vec);

            if (trace_length > MAX_TRACE_LENGTH) break;

            if (game_trace.m_hit_entity &&
                game_trace.m_hit_entity->get_ref_ehandle().get_entry_index() == target_pawn->get_ref_ehandle().get_entry_index() &&
                game_trace.m_contents == TARGET_CONTENTS)
            {
				scale_damage(
					static_cast<int>(game_trace.m_hitbox_data->m_hit_group),
					target_pawn,
					weapon_data,
					current_damage
				);

                return current_damage;
            }

            if (handle_bullet_penetration(&trace_data, reinterpret_cast<handle_bulllet_penetration_data_t*>(&bullet_data), value, false)) return 0.0f;

            current_damage = bullet_data.damage;
            damage_vec = _mm_set1_ps(current_damage);
        }
    }

    return 0.0f;
}
