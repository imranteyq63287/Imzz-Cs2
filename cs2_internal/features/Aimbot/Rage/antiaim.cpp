#include "antiaim.h"
#include "../../config_system.h"
#include "../../../sdk/classes/c_cs_gamerules.h"
#define DEG2RAD(x) ((x) * (3.14159265358979323846f / 180.0f))
std::mt19937 rng{ std::random_device{}() };

float c_antiaim::jitter(float min, float max, bool s) {
	s = !s;
	return s ? min : max;
}
float c_antiaim::third_way_jitter(float min, float max) {
	static int state = 0;
	float half = (min + max) / 2.0f;

	float result = (state == 0) ? min :
		(state == 1) ? half : max;

	state = (state + 1) % 3;
	return result;
}

float c_antiaim::random_jitter(float min, float max) {
	std::uniform_real_distribution<float> dist(min, max);
	return dist(rng);
}



float c_antiaim::switch_jitter(float min, float max, float lerp_step) {
	static float jit_val = min;
	static bool to_max = true;

	if (to_max) {
		jit_val += lerp_step;
		if (jit_val >= max) {
			jit_val = max;
			to_max = false;
		}
	}
	else {
		jit_val -= lerp_step;
		if (jit_val <= min) {
			jit_val = min;
			to_max = true;
		}
	}
	return jit_val;
}

void c_antiaim::fix_movement(c_user_cmd* user_cmd, vec3_t angle)
{
	vec3_t wish_angle{ 0.f, user_cmd->pb.mutable_base()->viewangles().y(), user_cmd->pb.mutable_base()->viewangles().z() };
	int sign = wish_angle.x > 89.f ? -1.f : 1.f;
	wish_angle.clamp();

	vec3_t forward, right, up, old_forward, old_right, old_up;
	vec3_t view_angles = angle;

	math::angle_vectors(wish_angle, forward, right, up);

	forward.z = right.z = up.x = up.y = 0.f;

	forward.normalize_in_place();
	right.normalize_in_place();
	up.normalize_in_place();

	math::angle_vectors(view_angles, old_forward, old_right, old_up);

	old_forward.z = old_right.z = old_up.x = old_up.y = 0.f;

	old_forward.normalize_in_place();
	old_right.normalize_in_place();
	old_up.normalize_in_place();

	forward *= user_cmd->pb.mutable_base()->forwardmove();
	right *= user_cmd->pb.mutable_base()->leftmove();
	up *= user_cmd->pb.mutable_base()->upmove();

	float fixed_forward_move = old_forward.dot(right) + old_forward.dot(forward) + old_forward.dot(up, true);

	float fixed_side_move = old_right.dot(right) + old_right.dot(forward) + old_right.dot(up, true);

	float fixed_up_move = old_up.dot(right, true) + old_up.dot(forward, true) + old_up.dot(up);

	user_cmd->pb.mutable_base()->set_forwardmove(fixed_forward_move);
	user_cmd->pb.mutable_base()->set_leftmove(fixed_side_move);
	user_cmd->pb.mutable_base()->set_upmove(fixed_up_move);

	fixed_forward_move = sign * (old_forward.dot(right) + old_forward.dot(forward));
	fixed_side_move = old_right.dot(right) + old_right.dot(forward);

	user_cmd->pb.mutable_base()->set_forwardmove(std::clamp(fixed_forward_move, -1.f, 1.f));
	user_cmd->pb.mutable_base()->set_leftmove(std::clamp(fixed_side_move, -1.f, 1.f));
}



void c_antiaim::antiaim(c_user_cmd* user_cmd)
{
	if (!config.antiaim.enable)
		return;
    if (!g_ctx.local || !g_ctx.local->is_alive()
        || g_ctx.local->move_type() == 1794
        || g_ctx.local->move_type() == 2313
        || g_ctx.local->is_throwing())
        return;
	c_cs_gamerules* gamerules = get_gamerules();
	if (!gamerules)
		return;
	bool freeze = *(bool*)((uintptr_t)gamerules + 0x40);
	bool warmup = *(bool*)((uintptr_t)gamerules + 0x41);

	debug_text(BLUE_COLOR, "freeze period: %s\n", freeze ? "true" : "false");
	debug_text(BLUE_COLOR, "warmup period: %s\n", warmup ? "true" : "false");


	if (freeze)
		return;
	if (user_cmd->buttons.button_state & IN_USE)
		return;
    if (user_cmd->buttons.button_state & IN_ATTACK && !config.antiaim.hideshots)
        return;
	static float pitch;
	static float yaw = config.antiaim.yaw;

    vec3_t angles(config.antiaim.pitch, yaw, 0.f);
	if (config.antiaim.manualleft) {
		yaw = 90.f;
	}
	else if (config.antiaim.manualright) {
		yaw = -90.f;
	}
	else if (config.antiaim.manualback) {
		yaw = -180.f;
	}
	else {
		yaw = config.antiaim.yaw;
	}

	if (config.antiaim.pitch_item == 0)
		pitch = user_cmd->pb.mutable_base()->viewangles().x();
	else if (config.antiaim.pitch_item == 1)
		pitch = 0.f;
	else if (config.antiaim.pitch_item == 2)
		pitch = -89.f;
	else if (config.antiaim.pitch_item == 3)
		pitch = 89.f;
	else if (config.antiaim.pitch_item == 4)
		pitch = -2182423346297399750336966557899.f;
	else if (config.antiaim.pitch_item == 5)
		pitch = 1928233885.f;
	else if (config.antiaim.pitch_item == 6)
		pitch = config.antiaim.pitch;
	else if (config.antiaim.pitch_item == 7)
		pitch = c_antiaim::jitter(config.antiaim.firstpitch, config.antiaim.secondpitch);
	else if (config.antiaim.pitch_item == 8)
		pitch = c_antiaim::random_jitter(config.antiaim.firstpitch, config.antiaim.secondpitch);
	else if (config.antiaim.pitch_item == 9)
		pitch = c_antiaim::switch_jitter(config.antiaim.firstpitch, config.antiaim.secondpitch);
	else if (config.antiaim.pitch_item == 10)
		pitch = c_antiaim::third_way_jitter(config.antiaim.firstpitch, config.antiaim.secondpitch);
    user_cmd->pb.mutable_base()->mutable_viewangles()->set_x(pitch);
    float actual_yaw = user_cmd->pb.mutable_base()->viewangles().y();

    user_cmd->pb.mutable_base()->mutable_viewangles()->set_y(yaw + actual_yaw);
}
