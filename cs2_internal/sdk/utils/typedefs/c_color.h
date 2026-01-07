#pragma once

#include "../../dependencies/imgui/imgui.h"

#include <array>
#include <algorithm>
#include <cmath>

class c_color_rgb {
public:
	unsigned char r, g, b;

	c_color_rgb(float _r = 0, float _g = 0, float _b = 0) {
		r = static_cast<unsigned char>(_r);
		g = static_cast<unsigned char>(_g);
		b = static_cast<unsigned char>(_b);
	}

	c_color_rgb operator+(c_color_rgb color) {
		return c_color_rgb(r + color.r, g + color.g, b + color.b);
	}

	c_color_rgb operator+(unsigned char n) {
		return c_color_rgb(r + n, g + n, b + n);
	}

	c_color_rgb operator+=(c_color_rgb color) {
		r += color.r;
		g += color.g;
		b += color.b;

		return *this;
	}

	c_color_rgb operator+=(unsigned char n) {
		r += n;
		g += n;
		b += n;

		return *this;
	}

	c_color_rgb operator-(c_color_rgb color) {
		return c_color_rgb(r - color.r, g - color.g, b - color.b);
	}

	c_color_rgb operator-(unsigned char n) {
		return c_color_rgb(r - n, g - n, b - n);
	}

	c_color_rgb operator-=(c_color_rgb color) {
		r -= color.r;
		g -= color.g;
		b -= color.b;

		return *this;
	}

	c_color_rgb operator-=(unsigned char n) {
		r -= n;
		g -= n;
		b -= n;

		return *this;
	}

	c_color_rgb operator/(c_color_rgb color) {
		return c_color_rgb(r / color.r, g / color.g, b / color.b);
	}

	c_color_rgb operator/(unsigned char n) {
		return c_color_rgb(r / n, g / n, b / n);
	}

	c_color_rgb operator/=(c_color_rgb color) {
		r /= color.r;
		g /= color.g;
		b /= color.b;

		return *this;
	}

	c_color_rgb operator/=(unsigned char n) {
		r /= n;
		g /= n;
		b /= n;

		return *this;
	}

	c_color_rgb operator*(c_color_rgb color) {
		return c_color_rgb(r * color.r, g * color.g, b * color.b);
	}

	c_color_rgb operator*(unsigned char n) {
		return c_color_rgb(r * n, g * n, b * n);
	}

	c_color_rgb operator*=(c_color_rgb color) {
		r *= color.r;
		g *= color.g;
		b *= color.b;

		return *this;
	}

	c_color_rgb operator*=(unsigned char n) {
		r *= n;
		g *= n;
		b *= n;

		return *this;
	}

	bool operator==(c_color_rgb color) {
		return r == color.r && g == color.g && b == color.b;
	}

	bool operator!=(c_color_rgb color) {
		return !(*this == color);
	}
};

class c_color_rgba {
public:
	unsigned char r, g, b, a;

	c_color_rgba(float _r = 0, float _g = 0, float _b = 0, float _a = 0) {
		r = static_cast<unsigned char>(_r);
		g = static_cast<unsigned char>(_g);
		b = static_cast<unsigned char>(_b);
		a = static_cast<unsigned char>(_a);
	}

	c_color_rgba operator+(c_color_rgba color) {
		return c_color_rgba(r + color.r, g + color.g, b + color.b, a + color.a);
	}

	c_color_rgba operator+(unsigned char n) {
		return c_color_rgba(r + n, g + n, b + n, a + n);
	}

	c_color_rgba operator+=(c_color_rgba color) {
		r += color.r;
		g += color.g;
		b += color.b;
		a += color.a;

		return *this;
	}

	c_color_rgba operator+=(unsigned char n) {
		r += n;
		g += n;
		b += n;
		a += n;

		return *this;
	}

	c_color_rgba operator-(c_color_rgba color) {
		return c_color_rgba(r - color.r, g - color.g, b - color.b, a - color.a);
	}

	c_color_rgba operator-(unsigned char n) {
		return c_color_rgba(r - n, g - n, b - n, a - n);
	}

	c_color_rgba operator-=(c_color_rgba color) {
		r -= color.r;
		g -= color.g;
		b -= color.b;
		a -= color.a;

		return *this;
	}

	c_color_rgba operator-=(unsigned char n) {
		r -= n;
		g -= n;
		b -= n;
		a -= n;

		return *this;
	}

	c_color_rgba operator/(c_color_rgba color) {
		return c_color_rgba(r / color.r, g / color.g, b / color.b, a / color.a);
	}

	c_color_rgba operator/(unsigned char n) {
		return c_color_rgba(r / n, g / n, b / n, a / n);
	}

	c_color_rgba operator/=(c_color_rgba color) {
		r /= color.r;
		g /= color.g;
		b /= color.b;
		a /= color.a;

		return *this;
	}

	c_color_rgba operator/=(unsigned char n) {
		r /= n;
		g /= n;
		b /= n;
		a /= n;

		return *this;
	}

	c_color_rgba operator*(c_color_rgba color) {
		return c_color_rgba(r * color.r, g * color.g, b * color.b, a * color.a);
	}

	c_color_rgba operator*(unsigned char n) {
		return c_color_rgba(r * n, g * n, b * n, a * n);
	}

	c_color_rgba operator*=(c_color_rgba color) {
		r *= color.r;
		g *= color.g;
		b *= color.b;
		a *= color.a;

		return *this;
	}

	c_color_rgba operator*=(unsigned char n) {
		r *= n;
		g *= n;
		b *= n;
		a *= n;

		return *this;
	}

	bool operator==(c_color_rgba color) {
		return r == color.r && g == color.g && b == color.b && a == color.a;
	}

	bool operator!=(c_color_rgba color) {
		return !(*this == color);
	}
};

class c_color {
public:
	float r, g, b, a;

	c_color(float _r = 1.f, float _g = 1.f, float _b = 1.f, float _a = 1.f) {
		r = _r;
		g = _g;
		b = _b;
		a = _a;
	}

	c_color operator+(c_color color) {
		return c_color(r + color.r, g + color.g, b + color.b, a + color.a);
	}

	c_color operator+(float n) {
		return c_color(r + n, g + n, b + n, a + n);
	}

	c_color operator+=(c_color color) {
		r += color.r;
		g += color.g;
		b += color.b;
		a += color.a;

		return *this;
	}

	c_color operator+=(float n) {
		r += n;
		g += n;
		b += n;
		a += n;

		return *this;
	}

	c_color operator-(c_color color) {
		return c_color(r - color.r, g - color.g, b - color.b, a - color.a);
	}

	c_color operator-(float n) {
		return c_color(r - n, g - n, b - n, a - n);
	}

	c_color operator-=(c_color color) {
		r -= color.r;
		g -= color.g;
		b -= color.b;
		a -= color.a;

		return *this;
	}

	c_color operator-=(float n) {
		r -= n;
		g -= n;
		b -= n;
		a -= n;

		return *this;
	}

	c_color operator/(c_color color) {
		return c_color(r / color.r, g / color.g, b / color.b, a / color.a);
	}

	c_color operator/(float n) {
		return c_color(r / n, g / n, b / n, a / n);
	}

	c_color operator/=(c_color color) {
		r /= color.r;
		g /= color.g;
		b /= color.b;
		a /= color.a;

		return *this;
	}

	c_color operator/=(float n) {
		r /= n;
		g /= n;
		b /= n;
		a /= n;

		return *this;
	}

	c_color operator*(c_color color) {
		return c_color(r * color.r, g * color.g, b * color.b, a * color.a);
	}

	c_color operator*(float n) {
		return c_color(r * n, g * n, b * n, a * n);
	}

	c_color operator*=(c_color color) {
		r *= color.r;
		g *= color.g;
		b *= color.b;
		a *= color.a;

		return *this;
	}

	c_color operator*=(float n) {
		r *= n;
		g *= n;
		b *= n;
		a *= n;

		return *this;
	}

	bool operator==(c_color color) {
		return r == color.r && g == color.g && b == color.b && a == color.a;
	}

	bool operator!=(c_color color) {
		return !(*this == color);
	}

	c_color to_float() {
		r /= 255.f;
		g /= 255.f;
		b /= 255.f;
		a /= 255.f;
		return *this;
	}

	c_color lerp(c_color& other, float step) {
		c_color lerped_color{
			std::lerp(r, other.r, step),
			std::lerp(g, other.g, step),
			std::lerp(b, other.b, step),
			std::lerp(a, other.a, step)
		};

		return lerped_color;
	}

	c_color set_alpha(int alpha) const {
		return c_color(r, g, b, (unsigned char)(alpha));
	}

	c_color set_alphaf(float alpha) const {
		return c_color(r, g, b, a * alpha);
	}

	c_color& self_modulate(int alpha) {
		a = alpha;
		return *this;
	}

	c_color& self_modulatef(float alpha) {
		a *= alpha;
		return *this;
	}

	c_color_rgb to_rgb() {
		return c_color_rgb(r * 255.f, g * 255.f, b * 255.f);
	}

	c_color_rgba to_rgba() {
		return c_color_rgba(r * 255.f, g * 255.f, b * 255.f, a * 255.f);
	}

	[[nodiscard]] constexpr ImU32 GetU32(const float flAlphaMultiplier = 1.0f) const
	{
		return IM_COL32(r, g, b, a * flAlphaMultiplier);
	}

	ImColor im()
	{
		return ImColor(r, g, b, a);
	}
};