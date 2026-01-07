#include "movement.h"
constexpr float M_PI = 3.14159265358979323846f;
constexpr float M_PI_F = static_cast<float>(M_PI);
constexpr float RAD2DEG = 180.f / M_PI_F;
constexpr float DEG2RAD = M_PI_F / 180.f;

void c_movement::duck_jump(c_user_cmd* user_cmd)
{
	if (!config.movement.air_duck)
		return;
	if (!interfaces::engine_client->is_connected() && !interfaces::engine_client->is_in_game())
		return;
	if (!g_ctx.local)
		return;
	if (!g_ctx.local_controller)
		return;
	if (!g_ctx.local->is_alive() || g_ctx.local->water_level() >= 2 || g_ctx.local->move_type() == 1794 || g_ctx.local->move_type() == 2313)
		return;
	if (!(g_ctx.local->flags() & FL_ONGROUND))
	{
		user_cmd->buttons.button_state &= ~IN_DUCK;
	}
}
void c_movement::bunnyhop(c_user_cmd* user_cmd)
{
	if (!config.movement.bunnyhop)
		return;
	if (!interfaces::engine_client->is_connected() && !interfaces::engine_client->is_in_game())
		return;
	if(!g_ctx.local)
		return;
	if (!g_ctx.local_controller)
		return;
	if (!g_ctx.local->is_alive() || g_ctx.local->water_level() >= 2 || g_ctx.local->move_type() == 1794 || g_ctx.local->move_type() == 2313)
		return;
	if (g_ctx.local->flags() & FL_ONGROUND) {

		user_cmd->buttons.button_state &= ~IN_JUMP;
	}
}
__forceinline float normalize(float value)
{
	while (value > 180.f)
		value -= 360.f;

	while (value < -180.f)
		value += 360.f;

	return value;
}

void c_movement::auto_strafe(c_user_cmd* user_cmd, float o_yaw)
{
	if (!interfaces::engine_client->is_connected() && !interfaces::engine_client->is_in_game())
		return;

	if (!g_ctx.local || !g_ctx.local_controller->pawn_is_alive())
		return;

	if (g_ctx.local->flags() & FL_ONGROUND)
		return;

	if (g_ctx.local->move_type() == 1794 || g_ctx.local->move_type() == 2313)
		return;

	if (!config.movement.auto_strafe)
		return;

	if (g_ctx.user_cmd->buttons.button_state & (1 << 16))
		return;

	auto movement_services = g_ctx.local->movement_services();
	if (!movement_services)
		return;

	vec3_t vel = g_ctx.local->abs_velocity();
	vec3_t move = movement_services->last_movement_impulses();
	float yaw = normalize(interfaces::csgo_input->get_view_angles().y);

	static uint64_t last_pressed = 0;
	static uint64_t last_buttons = 0;
	const auto current_buttons = g_ctx.user_cmd->buttons.button_state;

	auto check_button = [&](const uint64_t button) {
		if (current_buttons & button && (!(last_buttons & button) ||
			(button & IN_MOVELEFT && !(last_pressed & IN_MOVERIGHT)) ||
			(button & IN_MOVERIGHT && !(last_pressed & IN_MOVELEFT)) ||
			(button & IN_FORWARD && !(last_pressed & IN_BACK)) ||
			(button & IN_BACK && !(last_pressed & IN_FORWARD)))) {
			if (button & IN_MOVELEFT)
				last_pressed &= ~IN_MOVERIGHT;
			else if (button & IN_MOVERIGHT)
				last_pressed &= ~IN_MOVELEFT;
			else if (button & IN_FORWARD)
				last_pressed &= ~IN_BACK;
			else if (button & IN_BACK)
				last_pressed &= ~IN_FORWARD;

			last_pressed |= button;
		}
		else if (!(current_buttons & button))
			last_pressed &= ~button;
		};

	check_button(IN_MOVELEFT);
	check_button(IN_MOVERIGHT);
	check_button(IN_FORWARD);
	check_button(IN_BACK);
	last_buttons = current_buttons;

	float offset = 0.f;
	if (last_pressed & IN_MOVELEFT)
		offset += 90.f;
	if (last_pressed & IN_MOVERIGHT)
		offset -= 90.f;
	if (last_pressed & IN_FORWARD)
		offset *= 0.5f;
	else if (last_pressed & IN_BACK)
		offset = -offset * 0.5f + 180.f;

	yaw += offset;

	float velocity_angle = RAD2DEG * std::atan2f(vel.y, vel.x);
	if (velocity_angle < 0.0f)
		velocity_angle += 360.0f;
	velocity_angle -= floorf(velocity_angle / 360.0f + 0.5f) * 360.0f;

	float speed = vel.length_2d();
	float ideal = speed > 0.f ? std::clamp(RAD2DEG * std::atan2(20.f / speed, 1.f), 0.0f, 90.0f) : 70.0f;
	float correct = (100.f - 100) * 0.01f * ideal;

	auto rotate_movement = [](float target_yaw) {
		float rot = DEG2RAD * (interfaces::csgo_input->get_view_angles().y - target_yaw);
		float new_forward = std::cos(rot) * g_ctx.user_cmd->pb.mutable_base()->forwardmove() -
			std::sin(rot) * g_ctx.user_cmd->pb.mutable_base()->leftmove();
		float new_side = std::sin(rot) * g_ctx.user_cmd->pb.mutable_base()->forwardmove() +
			std::cos(rot) * g_ctx.user_cmd->pb.mutable_base()->leftmove();

		g_ctx.user_cmd->buttons.button_state &= ~(IN_BACK | IN_FORWARD | IN_MOVELEFT | IN_MOVERIGHT);
		g_ctx.user_cmd->buttons.button_state2 &= ~(IN_BACK | IN_FORWARD | IN_MOVELEFT | IN_MOVERIGHT);

		g_ctx.user_cmd->pb.mutable_base()->set_forwardmove(std::clamp(new_forward, -1.f, 1.f));
		g_ctx.user_cmd->pb.mutable_base()->set_leftmove(std::clamp(new_side * -1.f, -1.f, 1.f));

		if (g_ctx.user_cmd->pb.mutable_base()->forwardmove() > 0.0f)
			g_ctx.user_cmd->buttons.button_state |= IN_FORWARD;
		else if (g_ctx.user_cmd->pb.mutable_base()->forwardmove() < 0.0f)
			g_ctx.user_cmd->buttons.button_state |= IN_BACK;
		if (g_ctx.user_cmd->pb.mutable_base()->leftmove() > 0.0f)
			g_ctx.user_cmd->buttons.button_state |= IN_MOVELEFT;
		else if (g_ctx.user_cmd->pb.mutable_base()->leftmove() < 0.0f)
			g_ctx.user_cmd->buttons.button_state |= IN_MOVERIGHT;
		};

	float velocity_delta = normalize(yaw - velocity_angle);
	if (fabsf(velocity_delta) > 90.f)
		velocity_delta = std::copysignf(90.f, velocity_delta);

	g_ctx.user_cmd->pb.mutable_base()->set_forwardmove(0.f);
	g_ctx.user_cmd->pb.mutable_base()->set_leftmove(0.f);

	if (speed <= 80.f) {
		yaw += ideal * 3.0f;
		g_ctx.user_cmd->pb.mutable_base()->set_leftmove(1.0f);
	}
	else if (velocity_delta > correct) {
		yaw = velocity_angle + correct * 4.0f;
		g_ctx.user_cmd->pb.mutable_base()->set_leftmove(-1.0f);
	}
	else if (velocity_delta < -correct) {
		yaw = velocity_angle - correct * 4.0f;
		g_ctx.user_cmd->pb.mutable_base()->set_leftmove(1.0f);
	}
	else {
		yaw += ideal * 4.0f;
		g_ctx.user_cmd->pb.mutable_base()->set_leftmove(1.0f);
	}

	rotate_movement(normalize(yaw));
}
#include <cmath> 
#include "../../../Bind System/key.h"
#define DEG2RAD(x) ((x) * (3.14159265358979323846f / 180.0f))

inline void angle_vectors(const vec3_t& angles, vec3_t* forward, vec3_t* right = nullptr, vec3_t* up = nullptr)
{
	float pitch = DEG2RAD(angles.x);
	float yaw = DEG2RAD(angles.y);
	float roll = DEG2RAD(angles.z);

	float sp = std::sin(pitch);
	float cp = std::cos(pitch);
	float sy = std::sin(yaw);
	float cy = std::cos(yaw);
	float sr = std::sin(roll);
	float cr = std::cos(roll);

	if (forward)
	{
		forward->x = cp * cy;
		forward->y = cp * sy;
		forward->z = -sp;
	}

	if (right)
	{
		right->x = -1 * sr * sp * cy + -1 * cr * -sy;
		right->y = -1 * sr * sp * sy + -1 * cr * cy;
		right->z = -1 * sr * cp;
	}

	if (up)
	{
		up->x = cr * sp * cy + -sr * -sy;
		up->y = cr * sp * sy + -sr * cy;
		up->z = cr * cp;
	}
}
void air_accelerate(vec3_t& wish_vel, const vec3_t& wish_angles, float friction, const vec3_t& move, float frame_time, float max_speed)
{
	vec3_t fwd{}, right{};
	angle_vectors(wish_angles, &fwd, &right);

	fwd.z = 0.f;
	right.z = 0.f;
	fwd = fwd.normalized();
	right = right.normalized();

	vec3_t dir{};
	dir.x = (fwd.x * move.x + right.x * move.y) * max_speed;
	dir.y = (fwd.y * move.x + right.y * move.y) * max_speed;
	dir.z = 0.f;

	float wish_speed = dir.length();
	wish_speed = std::min(wish_speed, max_speed);

	float air_max = interfaces::cvar->get_by_name("sv_air_max_wishspeed")->get_float();
	float current_speed = wish_vel.dot(dir.normalized());
	float speed = std::min(wish_speed, air_max - current_speed);

	if (speed > 0.f)
	{
		float accel = (wish_speed * frame_time) * air_max * friction;
		float accel_speed = std::min(speed, accel);
		wish_vel += dir.normalized() * accel_speed;
	}
}
void c_movement::air_strafe(c_user_cmd* cmd)
{
	if (!config.movement.air_strafe || g_ctx.local->flags() & FL_ONGROUND)
		return;

	auto move_services = g_ctx.local->movement_services();
	if (!move_services)
		return;

	auto& abs_velocity = g_ctx.local->abs_velocity();
	const float friction = move_services->surface_friction();
	const int ticks = std::clamp(12 - cmd->pb.mutable_base()->subtick_moves().size(), 0, 12);

	if (ticks <= 0)
		return;

	const float interval_per_tick = 1.0f / 64.0f;
	const float frame_time = interval_per_tick / static_cast<float>(ticks);

	const float sv_gravity = interfaces::cvar->get_by_name("sv_gravity")->get_float();
	const float max_speed = 250.f;

	vec3_t view_angles = interfaces::csgo_input->get_view_angles();

	vec3_t move = {
		cmd->pb.base().forwardmove(),
		cmd->pb.base().leftmove(),
		0.f
	};

	for (int i = 0; i < ticks; ++i)
	{
		abs_velocity.z -= (sv_gravity * frame_time) * 0.5f;
		air_accelerate(abs_velocity, view_angles, friction, move, frame_time, max_speed);
		abs_velocity.z += (sv_gravity * frame_time);

		cmd->pb.mutable_base()->set_forwardmove(move.x);
		cmd->pb.mutable_base()->set_leftmove(move.y);

	}
}
//void c_movement::air_strafe(c_user_cmd* cmd)
//{
//	if (!config.movement.air_strafe || g_ctx.local->flags() & FL_ONGROUND)
//		return;
//
//	const auto move_services = g_ctx.local->movement_services();
//	if (!move_services)
//		return;
//
//	auto& abs_velocity = g_ctx.local->abs_velocity();
//	const auto speed = abs_velocity.length();
//	const auto friction = move_services->surface_friction();
//
//	const auto ticks = std::clamp(12 - cmd->pb.mutable_base()->subtick_moves_size(), 0, 12);
//	const auto frame_time = INTERVAL_PER_TICK / static_cast<float>(ticks);
//
//	auto move = move_services->last_movement_impulses();
//	const auto last_move = cmd->pb.mutable_base()->upmove();
//
//	/* iterate all possible ticks for that shit */
//	for (int i{}; i < ticks; ++i)
//	{
//		cmd->pb.mutable_base()->set_upmove(last_move);
//		const float sv_gravity = interfaces::cvar->get_by_name("sv_gravity")->get_float();
//		/* predict next velocity via air_move */
//		abs_velocity.z -= (sv_gravity * frame_time) * 0.5f;
//		air_accelerate(abs_velocity, interfaces::csgo_input->get_view_angles(), friction, move, frame_time, 250.f);
//		abs_velocity.z += (sv_gravity * frame_time);
//
//		if (const auto& sub_tick = cmd->pb.mutable_base()->add_subtick_moves(); sub_tick)
//		{
//			sub_tick->set_when((static_cast<float>(i) / static_cast<float>(ticks)));
//			sub_tick->set_analog_forward_delta((cmd->pb.mutable_base()->forwardmove() - last_move));
//			sub_tick->set_analog_left_delta((cmd->pb.mutable_base()->leftmove() - last_move));
//
//			move.x += sub_tick->analog_forward_delta();
//			move.y += sub_tick->analog_left_delta();
//
//			//cmd->pb.mutable_base()->mutable_subtick_moves()->Add();
//		}
//	}
//}
void c_movement::auto_pick(c_user_cmd* pCmd) {
	if (!g_ctx.local || !g_ctx.local->is_alive())
		return;
	auto m_data = &config.movement.m_autopeek;
	if (config.movement.autopeek && (g_key_handler->is_pressed(config.movement.autopeek_keybind, config.movement.autopeek_keybind_style))) {
		if (!m_data->m_autopeeking) {
			m_data->m_should_retrack = false;
			m_data->m_old_origin = g_ctx.local->game_scene_node()->vec_abs_origin();

			game_trace_t trace = game_trace_t();
			trace_filter_t filter = trace_filter_t(0x3001, nullptr, nullptr, 3);

			ray_t ray{};
			interfaces::engine_trace->trace_shape(&ray, g_ctx.local->get_eye_pos(), g_ctx.local->get_eye_pos() - vec3_t(0, 0, 1000.f), &filter, &trace);

			if (trace.m_fraction < 1.0f)
				m_data->m_old_origin = trace.m_end + vec3_t(0.0f, 0.0f, 2.0f);

		}
		if (m_data->m_should_retrack) {
			auto distance = g_ctx.local->game_scene_node()->vec_abs_origin().dist(m_data->m_old_origin);
			if (distance <= 16.0) {
				m_data->m_autopeeking = false;
				m_data->m_should_retrack = false;
			}
			else {
				vec3_t ang_to_pos;
				vec3_t local_angle = interfaces::csgo_input->get_view_angles();
				auto difference = m_data->m_old_origin - g_ctx.local->game_scene_node()->vec_abs_origin();
				math::vector_angles(difference, ang_to_pos);
				auto v45 = cos(ang_to_pos.x * 0.017453292);

				pCmd->pb.mutable_base()->set_forwardmove(0.0);
				pCmd->pb.mutable_base()->set_leftmove(0.0);

				auto v48 = deg2rad((local_angle.y - ang_to_pos.y));
				auto v66 = deg2rad((local_angle.y - ang_to_pos.y));
				auto cos_v48 = cos(v48);
				auto sin_v48 = sin(v66);

				auto v50 = cos_v48 * v45;

				pCmd->pb.mutable_base()->set_forwardmove(fmaxf(fminf(1.0, v50), -1.0));
				pCmd->pb.mutable_base()->set_leftmove(-fmaxf(fminf(1.0, (float)(sin_v48 * v45)), -1.0));
			}
		}
		if (m_data->did_shoot) {
			m_data->m_should_retrack = true;
		}
		m_data->m_autopeeking = true;
	}
	else {
		m_data->m_autopeeking = false;
		m_data->m_should_retrack = false;
	}
}