#pragma once

#include "../../../includes.h"
#include "../typedefs/c_color.h"

#include "../bytes.h"
#include "../vector.h"

enum e_font_flags
{
	font_flags_center = 1 << 0,
	font_flags_outline = 1 << 1,
	font_flags_dropshadow = 1 << 2
};

struct Image_t
{
	ID3D11ShaderResourceView* resource;

	vec3_t size;
};

struct c_icons
{
public:
	Image_t* cheat_logo;
};

struct c_fonts
{
public:
	ImFont* roboto_14;
	ImFont* roboto_m13;
	ImFont* roboto_m14;
	ImFont* roboto_m15;
	ImFont* roboto_m16;
	ImFont* roboto_m17;
	ImFont* roboto_m18;
	ImFont* weapon_icons;
	ImFont* verdana;
	ImFont* verdana_small;
	ImFont* onetap_pixel;
};

class c_render {
public:
	vec3_t m_screen_size;
	vec3_t m_screen_center;

	float m_aspect_ratio;

	ImDrawList* m_background_draw_list;

	c_icons icons;
	c_fonts fonts;

	void update_screen_size(ImGuiIO);
	void initialize();

	void update_background_drawlist(ImDrawList*);
	void granade_draw(std::string nade, ImVec2 pos, ImU32 color, ImU32 bg_color, float current_time = 0.f, float max_time = 0.f, bool is_timer = false, int radius = 25, int alpha = 255);
	
	bool world_to_screen(vec3_t&, vec3_t&);

	void line(vec3_t start_pos, vec3_t end_pos, c_color color);
	void rect_filled(vec3_t start_pos, vec3_t end_pos, c_color color, float rounding = .0f, int flags = 0.f);
	void rect_filled_multi_color(vec3_t start_pos, vec3_t end_pos, c_color top_left_clr, c_color top_right_clr, c_color bot_left_color, c_color bot_right_color, float rounding = .0f, int flags = 0);
	void rect(vec3_t start_pos, vec3_t end_pos, c_color color, float thickness = 1.f, float rounding = 0.0f);
	void autopick_cricle();
	void triangle(float x1, float y1, float x2, float y2, float x3, float y3, c_color clr, float thickness = 1.f);
	void filled_triangle(float x1, float y1, float x2, float y2, float x3, float y3, c_color clr);

	void text(vec3_t txt_pos, const std::string_view& message, c_color color, ImFont* font, const int font_size, const int flags = 0);
	void radial_gradient_3d(vec3_t pos, float radius, c_color in, c_color out);
	
	vec3_t measure_text(const std::string& text, ImFont* font);

	Image_t* load_texture_from_file_in_memory(unsigned char* data, int data_size);
	
	float linear_animate(float current, float next, float speed = 10.f);
};

inline const auto g_render = std::make_unique<c_render>();