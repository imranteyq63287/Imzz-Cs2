#pragma once

#include "../../../dependencies/imgui/imgui.h"

#include <cmath>
#include <algorithm>
#include <intrin.h>

constexpr auto _pi = 3.14159265358979323846;

constexpr auto _pi2 = _pi * 2;

constexpr auto _rad_pi = 180 / _pi;

constexpr auto _deg_pi = _pi / 180;

// convert angle in degrees to radians
#define deg2rad(degrees) (degrees * _deg_pi)
// convert angle in radians to degrees
#define rad2deg(radians) (radians * _rad_pi)

class vec4_t {
public:
	float x, y, z, w;

	vec4_t(float _x = 0, float _y = 0, float _z = 0, float _w = 0) {
		x = _x;
		y = _y;
		z = _z;
		w = _w;
	}

	vec4_t& operator+=(const vec4_t& v)
	{
		x += v.x; y += v.y; z += v.z; w += v.w;
		return *this;
	}

	vec4_t& operator-=(const vec4_t& v)
	{
		x -= v.x; y -= v.y; z -= v.z; w -= v.w;
		return *this;
	}

	vec4_t& operator*=(float fl)
	{
		x *= fl;
		y *= fl;
		z *= fl;
		w *= fl;
		return *this;
	}

	vec4_t& operator*=(const vec4_t& v)
	{
		x *= v.x;
		y *= v.y;
		z *= v.z;
		w *= v.w;
		return *this;
	}

	vec4_t& operator/=(const vec4_t& v)
	{
		x /= v.x;
		y /= v.y;
		z /= v.z;
		w /= v.w;
		return *this;
	}

	vec4_t& operator+=(float fl)
	{
		x += fl;
		y += fl;
		z += fl;
		w += fl;
		return *this;
	}

	vec4_t& operator/=(float fl)
	{
		x /= fl;
		y /= fl;
		z /= fl;
		w /= fl;
		return *this;
	}
	vec4_t& operator-=(float fl)
	{
		x -= fl;
		y -= fl;
		z -= fl;
		w -= fl;
		return *this;
	}
};

class matrix3x4_t;
struct vec2_t {
	float x{ }, y{ };

	vec2_t() { x = y = 0.0f; }

	vec2_t(const float fx, const float fy) {
		x = fx;
		y = fy;
	}

	vec2_t(float* v) {
		x = v[0];
		y = v[1];
	}

	vec2_t(const float* v) {
		x = v[0];
		y = v[1];
	}

	vec2_t(const vec2_t& v) {
		x = v.x;
		y = v.y;
	}

	ImVec2 im_vec2() const {
		return ImVec2(x, y);
	}

	vec2_t& operator=(const vec2_t& v) = default;
	float& operator[](const int i) { return reinterpret_cast<float*>(this)[i]; }
	float operator[](const int i) const { return ((float*)this)[i]; }

	vec2_t& operator+=(const vec2_t& v) {
		x += v.x;
		y += v.y;
		return *this;
	}

	vec2_t& operator-=(const vec2_t& v) {
		x -= v.x;
		y -= v.y;
		return *this;
	}

	vec2_t& operator*=(const vec2_t& v) {
		x *= v.x;
		y *= v.y;
		return *this;
	}

	vec2_t& operator/=(const vec2_t& v) {
		x /= v.x;
		y /= v.y;
		return *this;
	}

	vec2_t& operator+=(const float v) {
		x += v;
		y += v;
		return *this;
	}

	vec2_t& operator-=(const float v) {
		x -= v;
		y -= v;
		return *this;
	}

	vec2_t& operator*=(const float v) {
		x *= v;
		y *= v;
		return *this;
	}

	vec2_t& operator/=(const float v) {
		x /= v;
		y /= v;
		return *this;
	}

	vec2_t operator+(const vec2_t& v) const { return { x + v.x, y + v.y }; }
	vec2_t operator-(const vec2_t& v) const { return { x - v.x, y - v.y }; }
	vec2_t operator*(const vec2_t& v) const { return { x * v.x, y * v.y }; }
	vec2_t operator/(const vec2_t& v) const { return { x / v.x, y / v.y }; }
	vec2_t operator+(const float v) const { return { x + v, y + v }; }
	vec2_t operator-(const float v) const { return { x - v, y - v }; }
	vec2_t operator*(const float v) const { return { x * v, y * v }; }
	vec2_t operator/(const float v) const { return { x / v, y / v }; }
	operator bool() const noexcept { return !is_zero(); }

	[[nodiscard]] float length() const { return std::sqrt(x * x + y * y); }
	[[nodiscard]] float length_sqr() const { return (x * x + y * y); }
	[[nodiscard]] float dist_to(const vec2_t& v) const { return (*this - v).length(); }
	[[nodiscard]] float dist_to_sqr(const vec2_t& v) const { return (*this - v).length_sqr(); }
	[[nodiscard]] float dot(const vec2_t& v) const { return x * v.x + y * v.y; }

	[[nodiscard]] bool is_zero() const {
		return x > -FLT_EPSILON && x < FLT_EPSILON && y > -FLT_EPSILON && y < FLT_EPSILON;
	}
};

class vec3_t 
{
public:
	float x, y, z;

	vec3_t(float _x = 0, float _y = 0, float _z = 0) {
		x = _x;
		y = _y;
		z = _z;
	}

	vec3_t operator+(vec3_t& vec) {
		return vec3_t(x + vec.x, y + vec.y, z + vec.z);
	}

	vec3_t operator+(const vec3_t& vec) {
		return vec3_t(x + vec.x, y + vec.y, z + vec.z);
	}

	vec3_t operator+(const vec3_t& vec) const {
		return vec3_t(x + vec.x, y + vec.y, z + vec.z);
	}

	vec3_t operator+(float n) {
		return vec3_t(x + n, y + n, z + n);
	}

	vec3_t operator+=(vec3_t vec) {
		x += vec.x;
		y += vec.y;
		z += vec.z;

		return *this;
	}

	vec3_t operator+=(float n) {
		x += n;
		y += n;
		z += n;

		return *this;
	}

	vec3_t operator-(const vec3_t& vec) const {
		return vec3_t(x - vec.x, y - vec.y, z - vec.z);
	}

	vec3_t operator-(vec3_t vec) {
		return vec3_t(x - vec.x, y - vec.y, z - vec.z);
	}

	vec3_t operator-(float n) {
		return vec3_t(x - n, y - n, z - n);
	}

	vec3_t operator-=(vec3_t vec) {
		x -= vec.x;
		y -= vec.y;
		z -= vec.z;

		return *this;
	}

	vec3_t operator-=(float n) {
		x -= n;
		y -= n;
		z -= n;

		return *this;
	}

	vec3_t operator/(vec3_t vec) {
		return vec3_t(x / vec.x, y / vec.y, z / vec.z);
	}

	vec3_t operator/(float n) {
		return vec3_t(x / n, y / n, z / n);
	}

	vec3_t operator/(int n) {
		return vec3_t(x / n, y / n, z / n);
	}

	vec3_t operator/=(vec3_t vec) {
		x /= vec.x;
		y /= vec.y;
		z /= vec.z;

		return *this;
	}

	vec3_t operator/=(float n) {
		x /= n;
		y /= n;
		z /= n;

		return *this;
	}

	vec3_t operator*(vec3_t vec) {
		return vec3_t(x * vec.x, y * vec.y, z * vec.z);
	}

	vec3_t operator*(float n) {
		return vec3_t(x * n, y * n, z * n);
	}

	vec3_t operator*=(vec3_t vec) {
		x *= vec.x;
		y *= vec.y;
		z *= vec.z;

		return *this;
	}

	vec3_t operator*=(float n) {
		x *= n;
		y *= n;
		z *= n;

		return *this;
	}

	bool operator==(vec3_t vec) {
		return x == vec.x && y == vec.y && z == vec.z;
	}

	bool operator!=(vec3_t vec) {
		return !(*this == vec);
	}

	ImVec2 im() const
	{
		return ImVec2(x, y);
	}

	float dot(const vec3_t& other) const
	{
		return (this->x * other.x + this->y * other.y + this->z * other.z);
	}

	vec3_t normalize_no_clamp()
	{
		this->x -= floorf(this->x / 360.0f + 0.5f) * 360.0f;

		this->y -= floorf(this->y / 360.0f + 0.5f) * 360.0f;

		this->z -= floorf(this->z / 360.0f + 0.5f) * 360.0f;

		return *this;
	}

	bool is_valid() const
	{
		return std::isfinite(this->x) && std::isfinite(this->y) && std::isfinite(this->z);
	}

	//void clamp(float min_val, float max_val)
	//{
	//	x = (x < min_val) ? min_val : (x > max_val) ? max_val : x;
	//	y = (y < min_val) ? min_val : (y > max_val) ? max_val : y;
	//	z = (z < min_val) ? min_val : (z > max_val) ? max_val : z;
	//}
	void clamp() {
		// pitch
		x = std::clamp(x, -89.f, 89.f);

		// yaw
		y = std::fmod(y + 180.f, 360.f);
		if (y < 0.f) y += 360.f;
		y -= 180.f;

		// roll
		z = 0.f;
	}

	float dot(float* v)
	{
		return this->x * v[0] + this->y * v[1] + this->z * v[2];
	}

	float dot(vec3_t v, bool additional)
	{
		if (additional)
			return this->x * v.y + this->y * v.x + this->z * v.z;

		return this->x * v.x + this->y * v.y + this->z * v.z;
	}

	void vector_cross_product(vec3_t a, vec3_t b, vec3_t& result)
	{
		result.x = a.y * b.z - a.z * b.y;
		result.y = a.z * b.x - a.x * b.z;
		result.z = a.x * b.y - a.y * b.x;
	}

	vec3_t cross(vec3_t other)
	{
		vec3_t res;
		vector_cross_product(*this, other, res);

		return res;
	}

	//vec3_t transform(matrix3x4_t in)
	//{
	//	return { dot(in[0]) + in[0][3], dot(in[1]) + in[1][3], dot(in[2]) + in[2][3] };
	//}

	float normalize_in_place()
	{
		const float length = this->length();
		const float radius = 1.0f / (length + FLT_EPSILON);

		x *= radius;
		y *= radius;
		z *= radius;

		return length;
	}

	bool is_zero() 
	{
		return (x == 0.f && y == 0.f && z == 0.f);
	}
	bool is_zero_const() const
	{
		return (x == 0.f && y == 0.f && z == 0.f);
	}
	vec3_t angles() 
	{
		float distance2D = this->length_2d();

		float pitch = rad2deg(std::atan2f(z, distance2D));
		float yaw = rad2deg(std::atan2f(y, x));

		return vec3_t(-pitch, yaw, 0);
	}

	vec3_t angles(float pitch, float yaw)
	{
		float pitchRadians = deg2rad(pitch);
		float yawRadians = deg2rad(yaw);

		float x = std::cosf(pitchRadians) * std::cosf(yawRadians);
		float y = std::cosf(pitchRadians) * std::sinf(yawRadians);
		float z = -std::sinf(pitchRadians);

		return vec3_t(x, y, z);
	}

	vec3_t angles(vec3_t vec) 
	{
		return this->angles(vec.x, vec.y);
	}

	float dist(vec3_t vec) 
	{
		return (*this - vec).length();
	}

	float dist_sqr(vec3_t vec) 
	{
		return (*this - vec).length_sqr();
	}

	float dist_2d(vec3_t vec) 
	{
		return (*this - vec).length_2d();
	}

	float dist_2d_sqr(vec3_t vec)
	{
		return (*this - vec).length_2d_sqr();
	}

	float length() const
	{
		return std::sqrtf(this->length_sqr());
	}

	float length_sqr() const 
	{
		return x * x + y * y + z * z;
	}

	const float length_2d() const
	{
		return std::sqrtf(this->length_2d_sqr());
	}

	const float length_2d_sqr() const
	{
		return x * x + y * y;
	}

	vec3_t normalized() const
	{
		float l = length();
		if (l > 0.f)
			return vec3_t(x / l, y / l, z / l);

		return vec3_t(0.f, 0.f, 0.f);
	}


	vec3_t normalize_angle() {

		while (x < -180.0f)
			x += 360.0f;
		while (x > 180.0f)
			x -= 360.0f;

		while (y < -180.0f)
			y += 360.0f;
		while (y > 180.0f)
			y -= 360.0f;

		while (z < -180.0f)
			z += 360.0f;
		while (z > 180.0f)
			z -= 360.0f;

		return *this;
	}

	float normalize_movement()
	{
		vec3_t res = *this;
		float l = res.length();
		if (l != .0f)
			res /= l;
		else
			res.x = res.y = res.z = .0f;

		return l;
	}
};

struct alignas(16) vec_aligned : vec3_t
{
	vec_aligned() = default;

	explicit vec_aligned(const vec3_t& vecBase)
	{
		this->x = vecBase.x;
		this->y = vecBase.y;
		this->z = vecBase.z;
		this->w = 0.0f;
	}

	constexpr vec_aligned& operator=(const vec3_t& vecBase)
	{
		this->x = vecBase.x;
		this->y = vecBase.y;
		this->z = vecBase.z;
		this->w = 0.0f;
		return *this;
	}

	float w = 0.0f;
};

#pragma pack(push, 4)
using matrix3x3_t = float[3][3];
class matrix2x4_t {
public:
	vec3_t get_origin() {
		return vec3_t(_11, _12, _13);
	}

	void set_origin(int nIndex, vec3_t vecValue) {
		this[nIndex]._11 = vecValue.x;
		this[nIndex]._12 = vecValue.y;
		this[nIndex]._13 = vecValue.z;
	}

	union
	{
		struct
		{
			float _11, _12, _13, _14;
			float _21, _22, _23, _24;
		};
	};
};
class matrix3x4_t {
public:
	matrix3x4_t() = default;

	constexpr matrix3x4_t(
		const float m00, const float m01, const float m02, const float m03,
		const float m10, const float m11, const float m12, const float m13,
		const float m20, const float m21, const float m22, const float m23)
	{
		arr_data[0][0] = m00;
		arr_data[0][1] = m01;
		arr_data[0][2] = m02;
		arr_data[0][3] = m03;
		arr_data[1][0] = m10;
		arr_data[1][1] = m11;
		arr_data[1][2] = m12;
		arr_data[1][3] = m13;
		arr_data[2][0] = m20;
		arr_data[2][1] = m21;
		arr_data[2][2] = m22;
		arr_data[2][3] = m23;
	}

	constexpr matrix3x4_t(const vec3_t& vecForward, const vec3_t& vecLeft, const vec3_t& vecUp, const vec3_t& vecOrigin)
	{
		set_forward(vecForward);
		set_left(vecLeft);
		set_up(vecUp);
		set_origin(vecOrigin);
	}

	float* operator[](const int nIndex)
	{
		return arr_data[nIndex];
	}

	const float* operator[](const int nIndex) const
	{
		return arr_data[nIndex];
	}

	constexpr void set_forward(const vec3_t& vecForward)
	{
		arr_data[0][0] = vecForward.x;
		arr_data[1][0] = vecForward.y;
		arr_data[2][0] = vecForward.z;
	}

	constexpr void set_left(const vec3_t& vecLeft)
	{
		arr_data[0][1] = vecLeft.x;
		arr_data[1][1] = vecLeft.y;
		arr_data[2][1] = vecLeft.z;
	}

	constexpr void set_up(const vec3_t& vecUp)
	{
		arr_data[0][2] = vecUp.x;
		arr_data[1][2] = vecUp.y;
		arr_data[2][2] = vecUp.z;
	}

	constexpr void set_origin(const vec3_t& vecOrigin)
	{
		arr_data[0][3] = vecOrigin.x;
		arr_data[1][3] = vecOrigin.y;
		arr_data[2][3] = vecOrigin.z;
	}

	vec3_t get_forward() const
	{
		return { arr_data[0][0], arr_data[1][0], arr_data[2][0] };
	}

	vec3_t get_left() const
	{
		return { arr_data[0][1], arr_data[1][1], arr_data[2][1] };
	}

	vec3_t get_up() const
	{
		return { arr_data[0][2], arr_data[1][2], arr_data[2][2] };
	}

	vec3_t get_origin() const
	{
		return { arr_data[0][3], arr_data[1][3], arr_data[2][3] };
	}

	void angle_matrix(const vec3_t& angles)
	{
		float sr, sp, sy, cr, cp, cy;

		sy = std::sin(deg2rad(angles.y));
		cy = std::cos(deg2rad(angles.y));

		sp = std::sin(deg2rad(angles.x));
		cp = std::cos(deg2rad(angles.x));

		sr = std::sin(deg2rad(angles.z));
		cr = std::cos(deg2rad(angles.z));

		arr_data[0][0] = cp * cy;
		arr_data[1][0] = cp * sy;
		arr_data[2][0] = -sp;

		float crcy = cr * cy;
		float crsy = cr * sy;
		float srcy = sr * cy;
		float srsy = sr * sy;
		arr_data[0][1] = sp * srcy - crsy;
		arr_data[1][1] = sp * srsy + crcy;
		arr_data[2][1] = sr * cp;

		arr_data[0][2] = (sp * crcy + srsy);
		arr_data[1][2] = (sp * crsy - srcy);
		arr_data[2][2] = cr * cp;

		arr_data[0][3] = 0.0f;
		arr_data[1][3] = 0.0f;
		arr_data[2][3] = 0.0f;
	}

	constexpr matrix3x4_t concat_transforms(const matrix3x4_t& mat_other) const
	{
		return {
			arr_data[0][0] * mat_other.arr_data[0][0] + arr_data[0][1] * mat_other.arr_data[1][0] + arr_data[0][2] * mat_other.arr_data[2][0],
			arr_data[0][0] * mat_other.arr_data[0][1] + arr_data[0][1] * mat_other.arr_data[1][1] + arr_data[0][2] * mat_other.arr_data[2][1],
			arr_data[0][0] * mat_other.arr_data[0][2] + arr_data[0][1] * mat_other.arr_data[1][2] + arr_data[0][2] * mat_other.arr_data[2][2],
			arr_data[0][0] * mat_other.arr_data[0][3] + arr_data[0][1] * mat_other.arr_data[1][3] + arr_data[0][2] * mat_other.arr_data[2][3] + arr_data[0][3],

			arr_data[1][0] * mat_other.arr_data[0][0] + arr_data[1][1] * mat_other.arr_data[1][0] + arr_data[1][2] * mat_other.arr_data[2][0],
			arr_data[1][0] * mat_other.arr_data[0][1] + arr_data[1][1] * mat_other.arr_data[1][1] + arr_data[1][2] * mat_other.arr_data[2][1],
			arr_data[1][0] * mat_other.arr_data[0][2] + arr_data[1][1] * mat_other.arr_data[1][2] + arr_data[1][2] * mat_other.arr_data[2][2],
			arr_data[1][0] * mat_other.arr_data[0][3] + arr_data[1][1] * mat_other.arr_data[1][3] + arr_data[1][2] * mat_other.arr_data[2][3] + arr_data[1][3],

			arr_data[2][0] * mat_other.arr_data[0][0] + arr_data[2][1] * mat_other.arr_data[1][0] + arr_data[2][2] * mat_other.arr_data[2][0],
			arr_data[2][0] * mat_other.arr_data[0][1] + arr_data[2][1] * mat_other.arr_data[1][1] + arr_data[2][2] * mat_other.arr_data[2][1],
			arr_data[2][0] * mat_other.arr_data[0][2] + arr_data[2][1] * mat_other.arr_data[1][2] + arr_data[2][2] * mat_other.arr_data[2][2],
			arr_data[2][0] * mat_other.arr_data[0][3] + arr_data[2][1] * mat_other.arr_data[1][3] + arr_data[2][2] * mat_other.arr_data[2][3] + arr_data[2][3]
		};
	}

	float arr_data[3][4] = {};
};

#pragma pack(pop)

struct quaternion_t {
	constexpr quaternion_t(const float x = 0.0f, const float y = 0.0f, const float z = 0.0f, const float w = 0.0f) :
		x(x), y(y), z(z), w(w) {
	}

	bool is_valid() const
	{
		return (std::isfinite(x) && std::isfinite(y) && std::isfinite(z) && std::isfinite(w));
	}

	matrix3x4_t to_matrix(const vec3_t& vec_origin = {}) const
	{
		matrix3x4_t mat_out;

#ifdef _DEBUG // precalculate common multiplications
		const float x2 = this->x + this->x, y2 = this->y + this->y, z2 = this->z + this->z;
		const float xx = this->x * x2, xy = this->x * y2, xz = this->x * z2;
		const float yy = this->y * y2, yz = this->y * z2;
		const float zz = this->z * z2;
		const float wx = this->w * x2, wy = this->w * y2, wz = this->w * z2;

		mat_out[0][0] = 1.0f - (yy + zz);
		mat_out[1][0] = xy + wz;
		mat_out[2][0] = xz - wy;

		mat_out[0][1] = xy - wz;
		mat_out[1][1] = 1.0f - (xx + zz);
		mat_out[2][1] = yz + wx;

		mat_out[0][2] = xz + wy;
		mat_out[1][2] = yz - wx;
		mat_out[2][2] = 1.0f - (xx + yy);
#else // let the compiler optimize calculations itself
		mat_out[0][0] = 1.0f - 2.0f * this->y * this->y - 2.0f * this->z * this->z;
		mat_out[1][0] = 2.0f * this->x * this->y + 2.0f * this->w * this->z;
		mat_out[2][0] = 2.0f * this->x * this->z - 2.0f * this->w * this->y;

		mat_out[0][1] = 2.0f * this->x * this->y - 2.0f * this->w * this->z;
		mat_out[1][1] = 1.0f - 2.0f * this->x * this->x - 2.0f * this->z * this->z;
		mat_out[2][1] = 2.0f * this->y * this->z + 2.0f * this->w * this->x;

		mat_out[0][2] = 2.0f * this->x * this->z + 2.0f * this->w * this->y;
		mat_out[1][2] = 2.0f * this->y * this->z - 2.0f * this->w * this->x;
		mat_out[2][2] = 1.0f - 2.0f * this->x * this->x - 2.0f * this->y * this->y;
#endif

		mat_out[0][3] = vec_origin.x;
		mat_out[1][3] = vec_origin.y;
		mat_out[2][3] = vec_origin.z;
		return mat_out;
	}

	float x, y, z, w;
};

struct alignas(16) quaternion_aligned : quaternion_t
{
	quaternion_aligned& operator=(const quaternion_t& quat_other)
	{
		this->x = quat_other.x;
		this->y = quat_other.y;
		this->z = quat_other.z;
		this->w = quat_other.w;

		return *this;
	}
};