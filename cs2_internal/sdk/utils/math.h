#pragma once

#include <cmath>
#include "vector.h"  

#include <DirectXMath.h>
#include <xmmintrin.h>

namespace math
{
	constexpr auto pitch_bounds = 89.f;
	constexpr auto yaw_bounds = 360.f;
	constexpr auto roll_bounds = 50.f;

	inline matrix3x4_t transform_to_matrix(CBoneData& in) {
		matrix3x4_t  matrix;

		vec4_t rot;
		rot = in.rotation;
		vec3_t pos;
		pos = in.position;

		matrix[0][0] = 1.0f - 2.0f * rot.y * rot.y - 2.0f * rot.z * rot.z;
		matrix[1][0] = 2.0f * rot.x * rot.y + 2.0f * rot.w * rot.z;
		matrix[2][0] = 2.0f * rot.x * rot.z - 2.0f * rot.w * rot.y;

		matrix[0][1] = 2.0f * rot.x * rot.y - 2.0f * rot.w * rot.z;
		matrix[1][1] = 1.0f - 2.0f * rot.x * rot.x - 2.0f * rot.z * rot.z;
		matrix[2][1] = 2.0f * rot.y * rot.z + 2.0f * rot.w * rot.x;

		matrix[0][2] = 2.0f * rot.x * rot.z + 2.0f * rot.w * rot.y;
		matrix[1][2] = 2.0f * rot.y * rot.z - 2.0f * rot.w * rot.x;
		matrix[2][2] = 1.0f - 2.0f * rot.x * rot.x - 2.0f * rot.y * rot.y;

		matrix[0][3] = pos.x;
		matrix[1][3] = pos.y;
		matrix[2][3] = pos.z;

		return matrix;
	}

	constexpr float qsqrt(float x) 
	{
		const unsigned int i = (*(unsigned int*)&x + 0x3f800000) >> 1;
		const float approx = *(float*)&i;
		return (approx + x / approx) * 0.5f;
	}

	__forceinline void sin_cos(const float rad, float& sin, float& cos) {
		sin = std::sin(rad);
		cos = std::cos(rad);
	}

	__forceinline void fast_rsqrt(float a, float* out)
	{
		const auto xx = _mm_load_ss(&a);
		auto xr = _mm_rsqrt_ss(xx);
		auto xt = _mm_mul_ss(xr, xr);
		xt = _mm_mul_ss(xt, xx);
		xt = _mm_sub_ss(_mm_set_ss(3.f), xt);
		xt = _mm_mul_ss(xt, _mm_set_ss(0.5f));
		xr = _mm_mul_ss(xr, xt);
		_mm_store_ss(out, xr);
	}

	__forceinline float fast_vec_normalize(vec3_t& vec)
	{
		const auto sqrlen = vec.length_sqr() + 1.0e-10f;
		float invlen;
		fast_rsqrt(sqrlen, &invlen);
		vec.x *= invlen;
		vec.y *= invlen;
		vec.z *= invlen;
		return sqrlen * invlen;
	}

	__forceinline float random_float(float min, float max)
	{
		return min + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (max - min)));
	}

	__forceinline float normalize_yaw(float yaw)
	{
		yaw = std::fmodf(yaw, 360.0f);

		if (yaw > 180)
			yaw -= 360;

		if (yaw < -180)
			yaw += 360;

		return yaw;
	}

	__forceinline void normalize(vec3_t& angle)
	{
		if (!angle.is_valid())
		{
			angle = { 0.f, 0.f, 0.f };
			return;
		}

		angle.x = std::clamp(angle.x, -pitch_bounds, pitch_bounds);
		angle.y = std::remainderf(angle.y, yaw_bounds);
		angle.z = std::clamp(angle.z, -roll_bounds, roll_bounds);
	}

	__forceinline vec3_t angle_vectors(const vec3_t& angles, vec3_t& forward)
	{
		float sp, sy, cp, cy;

		DirectX::XMScalarSinCos(&sp, &cp, deg2rad(angles.x));
		DirectX::XMScalarSinCos(&sy, &cy, deg2rad(angles.y));

		return vec3_t(
			forward.x = cp * cy,
			forward.y = cp * sy,
			forward.z = -sp
		);
	}

	__forceinline vec3_t angle_vectors(const vec3_t& angles)
	{
		float	sp, sy, cp, cy;

		DirectX::XMScalarSinCos(&sp, &cp, deg2rad(angles.x));
		DirectX::XMScalarSinCos(&sy, &cy, deg2rad(angles.y));

		return vec3_t(
			cp * cy,
			cp * sy,
			-sp
		);
	}

	__forceinline ImVec2 rotate_vertex(const vec3_t& p, const ImVec2& v, float angle)
	{
		float c = std::cos(deg2rad(angle));
		float s = std::sin(deg2rad(angle));

		return
		{
			p.x + (v.x - p.x) * c - (v.y - p.y) * s,
			p.y + (v.x - p.x) * s + (v.y - p.y) * c
		};
	}

	__forceinline void angle_vectors(vec3_t angles, vec3_t& forward, vec3_t& right, vec3_t& up)
	{
		float			angle;
		static float	sr, sp, sy, cr, cp, cy;

		angle = angles.y * (_pi2 / 360);
		sy = sin(angle);
		cy = cos(angle);

		angle = angles.x * (_pi2 / 360);
		sp = sin(angle);
		cp = cos(angle);

		angle = angles.z * (_pi2 / 360);
		sr = sin(angle);
		cr = cos(angle);

		forward.x = cp * cy;
		forward.y = cp * sy;
		forward.z = -sp;

		right.x = (-1 * sr * sp * cy + -1 * cr * -sy);
		right.y = (-1 * sr * sp * sy + -1 * cr * cy);
		right.z = -1 * sr * cp;

		up.x = (cr * sp * cy + -sr * -sy);
		up.y = (cr * sp * sy + -sr * cy);
		up.z = cr * cp;
	}

	__forceinline vec3_t vector_angles(const vec3_t& vec) 
	{
		vec3_t result;

		float hyp2d = qsqrt(vec.x * vec.x + vec.y * vec.y);

		result.x = -atanf(vec.z / hyp2d) * 57.295779513082f;
		result.y = atan2f(vec.y, vec.x) * 57.295779513082f;

		return result;
	}

	__forceinline void vector_angles(vec3_t vec_forward, vec3_t& vec_angles)
	{
		if (vec_forward.x == 0.f && vec_forward.y == 0.f)
		{
			vec_angles.y = 0.f;
			vec_angles.x = vec_forward.z > 0.f ? 270.f : 90.f;
		}
		else
		{
			vec_angles.y = rad2deg(std::atan2(vec_forward.y, vec_forward.x));
			if (vec_angles.y < 0.f)
				vec_angles.y += 360.f;

			vec_angles.x = rad2deg(std::atan2(-vec_forward.z, vec_forward.length_2d()));
			if (vec_angles.x < 0.f)
				vec_angles.x += 360.f;
		}

		vec_angles.x = std::remainder(vec_angles.x, 360.f);
		vec_angles.y = std::remainder(vec_angles.y, 360.f);
		vec_angles.z = std::remainder(vec_angles.z, 360.f);
	}

	__forceinline float angel_normalize(float angle)
	{
		angle = fmodf(angle, 360.0f);
		if (angle > 180)
			angle -= 360;

		if (angle < -180)
			angle += 360;

		return angle;
	}


	__forceinline float delta_angle(float first, float second)
	{
		const float delta = first - second;
		float res = std::isfinite(delta) ? std::remainder(delta, _pi2) : 0.0f;

		if (first > second)
		{
			if (res >= _pi)
				res -= _pi2;
		}
		else {
			if (res <= _pi)
				res += _pi;
		}

		return res;
	}

	__forceinline float angle_diff(float src, float dst)
	{
		return std::remainder(
			std::remainder(dst, 360.f) - std::remainder(src, 360.f),
			360.f
		);
	}

	__forceinline vec3_t aim_direction(vec3_t src, vec3_t dst)
	{
		vec3_t diff = dst - src;

		vec3_t out{};
		vector_angles(diff, out);
		return out;
	}

	__forceinline vec3_t calculate_camera_pos(vec3_t anchor_pos, float distance, vec3_t view_angles)
	{
		float yaw = DirectX::XMConvertToRadians(view_angles.y);
		float pitch = DirectX::XMConvertToRadians(view_angles.x);

		float x = anchor_pos.x + distance * cosf(yaw) * cosf(pitch);
		float y = anchor_pos.y + distance * sinf(yaw) * cosf(pitch);
		float z = anchor_pos.z + distance * sinf(pitch);

		return vec3_t{ x, y, z };
	}

	__forceinline vec3_t calculate_angles(vec3_t view_pos, vec3_t aim_pos)
	{
		static const auto ang_zero = vec3_t(0.0f, 0.0f, 0.0f);

		const auto delta = view_pos - aim_pos;
		if (delta.length() <= 0.0f)
			return ang_zero;

		if (delta.z == 0.0f && delta.length() == 0.0f)
			return ang_zero;

		if (delta.y == 0.0f && delta.x == 0.0f)
			return ang_zero;

		vec3_t angles;
		angles.x = asinf(delta.z / delta.length()) * _rad_pi;
		angles.y = atanf(delta.y / delta.x) * _rad_pi;
		angles.z = 0.0f;

		if (delta.x >= 0.0f)
			angles.y += 180.f;

		normalize(angles);
		return angles;
	}

	__forceinline void normalize_angles(vec3_t& angles)
	{
		while (angles.y < -180.f) angles.y += 360.f;
		while (angles.y > 180.f)  angles.y -= 360.f;

		while (angles.x < -89.f)  angles.x += 180.f;
		while (angles.x > 89.f)   angles.x -= 180.f;
	}

	__forceinline void clamp_angles(vec3_t& angles)
	{
		if (angles.x > 89.f)  angles.x = 89.f;
		if (angles.x < -89.f) angles.x = -89.f;

		if (angles.y > 180.f)  angles.y = 180.f;
		if (angles.y < -180.f) angles.y = -180.f;

		angles.z = 0.f;
	}

	__forceinline float get_fov(const vec3_t& current, const vec3_t& start, const vec3_t& end)
	{
		vec3_t v;
		angle_vectors(current, v);
		if ((end - start).normalized().dot(v) < .3f)
			return FLT_MAX;

		const auto target = calculate_angles(start, end);
		const auto dist = (end - start).length();
		const auto pitch = sin(deg2rad(fabsf(current.x - target.x))) * dist;
		const auto yaw = sin(deg2rad(fabsf(current.y - target.y))) * dist;
		return vec3_t(pitch, yaw).length();
	}

	__forceinline float get_fov2vec(const vec3_t& view_angle, const vec3_t& target_angle)
	{
		vec3_t delta = target_angle - view_angle;
		math::normalize_angles(delta);
		return sqrt(delta.x * delta.x + delta.y * delta.y);
	}

	__forceinline float get_fov_simple(const vec3_t& current, const vec3_t& start, const vec3_t& end)
	{
		vec3_t current_aim, target = (end - start).normalized();
		angle_vectors(current, current_aim);
		return rad2deg(acos(target.dot(current_aim)));
	}
}