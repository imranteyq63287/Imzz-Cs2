#include "render.h"
#include "../../../dependencies/imgui/imgui_freetype.h"
#include "../../hooks/directx.h"
#include "../utils.h"
#include "../../modules/modules.h"

#include "../../../features/menu/gui_base/hashes.hpp"
#include "../../../features/menu/gui_base/bytes.hpp"
#include "../../../features/menu/gui_base/Source.hpp"
#include "../../../features/menu/gui_base/color_t.hpp"
#include "../../../features/menu/gui_base/gui.hpp"

#include "../../../features/config_system.h"
#include "../../../dependencies/d3dx11/d3dx11tex.h"

void c_render::update_screen_size(ImGuiIO io)
{
	const ImVec2 display_size = io.DisplaySize;

	m_screen_size = vec3_t(display_size.x, display_size.y);
	m_screen_center = m_screen_size * 0.5;
	m_aspect_ratio = m_screen_size.x / m_screen_size.y;
}

void c_render::initialize()
{
	auto& io = ImGui::GetIO();
	io.IniFilename = nullptr;
	io.LogFilename = nullptr;
	io.ConfigFlags |= ImGuiConfigFlags_NoMouseCursorChange;

	ImFontConfig cfg;
	cfg.OversampleH = cfg.OversampleV = 1;
	cfg.PixelSnapH = true;

	icons.cheat_logo = load_texture_from_file_in_memory(cheat_logo, sizeof(cheat_logo));

	fonts.roboto_14 = io.Fonts->AddFontFromMemoryTTF(roboto_regular_raw, sizeof(roboto_regular_raw), 14, nullptr, io.Fonts->GetGlyphRangesCyrillic());
	fonts.roboto_m13 = io.Fonts->AddFontFromMemoryTTF(roboto_medium_raw, sizeof(roboto_medium_raw), 13, nullptr, io.Fonts->GetGlyphRangesCyrillic());
	fonts.roboto_m14 = io.Fonts->AddFontFromMemoryTTF(roboto_medium_raw, sizeof(roboto_medium_raw), 14, nullptr, io.Fonts->GetGlyphRangesCyrillic());
	fonts.roboto_m15 = io.Fonts->AddFontFromMemoryTTF(roboto_medium_raw, sizeof(roboto_medium_raw), 15, nullptr, io.Fonts->GetGlyphRangesCyrillic());
	fonts.roboto_m16 = io.Fonts->AddFontFromMemoryTTF(roboto_medium_raw, sizeof(roboto_medium_raw), 16, nullptr, io.Fonts->GetGlyphRangesCyrillic());
	fonts.roboto_m17 = io.Fonts->AddFontFromMemoryTTF(roboto_medium_raw, sizeof(roboto_medium_raw), 17, nullptr, io.Fonts->GetGlyphRangesCyrillic());
	fonts.roboto_m18 = io.Fonts->AddFontFromMemoryTTF(roboto_medium_raw, sizeof(roboto_medium_raw), 18, nullptr, io.Fonts->GetGlyphRangesCyrillic());

	fonts.verdana = io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\Verdana.ttf", 13.f, nullptr, io.Fonts->GetGlyphRangesCyrillic());
	fonts.verdana_small = io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\Verdana.ttf", 12.f, &cfg, io.Fonts->GetGlyphRangesCyrillic());

	static const ImWchar icon_ranges[] = { ICON_MIN_FA, ICON_MAX_FA, 0 };

	ImFontConfig icons_config;
	icons_config.MergeMode = true;
	icons_config.PixelSnapH = true;

	fa_icon = io.Fonts->AddFontFromMemoryTTF(&font_awesome_binary, sizeof font_awesome_binary, 22, &icons_config, icon_ranges);
	fa_icon16 = io.Fonts->AddFontFromMemoryTTF(&font_awesome_binary, sizeof font_awesome_binary, 14, &icons_config, icon_ranges);
	fa_icon30 = io.Fonts->AddFontFromMemoryTTF(&font_awesome_binary, sizeof font_awesome_binary, 60, &icons_config, icon_ranges);

	gun_icon10 = io.Fonts->AddFontFromMemoryTTF(&weaponicons, sizeof weaponicons, 10);
	gun_icon32 = io.Fonts->AddFontFromMemoryTTF(&weaponicons, sizeof weaponicons, 32);
	gun_iconbig = io.Fonts->AddFontFromMemoryTTF(&weaponicons, sizeof weaponicons, 20);

	ssfi_small = io.Fonts->AddFontFromMemoryTTF(sfui, sizeof sfui, 14, NULL, io.Fonts->GetGlyphRangesCyrillic());
	ssfi_mid = io.Fonts->AddFontFromMemoryTTF(museo900_binary, sizeof museo900_binary, 23, NULL, io.Fonts->GetGlyphRangesCyrillic());
	ssfi_VERYBIG = io.Fonts->AddFontFromMemoryTTF(sfui, sizeof sfui, 100, NULL, io.Fonts->GetGlyphRangesCyrillic());

	auto prev_flags = cfg.FontBuilderFlags;
	cfg.FontBuilderFlags = ImGuiFreeTypeBuilderFlags_MonoHinting | ImGuiFreeTypeBuilderFlags_Monochrome;

	fonts.onetap_pixel = io.Fonts->AddFontFromMemoryTTF(onetap_pixel_font, sizeof(onetap_pixel_font), 8.f, &cfg, io.Fonts->GetGlyphRangesCyrillic());

	cfg.FontBuilderFlags = prev_flags;

	io.FontDefault = fonts.roboto_14;
}

void c_render::granade_draw(std::string nade, ImVec2 pos, ImU32 color, ImU32 bg_color, float current_time, float max_time, bool is_timer, int radius, int alpha) {
	static std::map<std::pair<int, int>, float> progress_map;

	std::pair<int, int> key = { static_cast<int>(pos.x), static_cast<int>(pos.y) };

	std::string icon;
	auto draw = ImGui::GetBackgroundDrawList();

	ImColor main_color = ImColor(color);
	main_color.Value.w = alpha / 255.f;

	ImColor background_color = ImColor(bg_color);
	background_color.Value.w = alpha / 255.f;
	draw->AddCircleFilled(pos, radius, background_color, 100);

	if (is_timer && max_time > 0.f && current_time >= 0.f)
	{
		float progress = std::clamp(current_time / max_time, 0.f, 1.f);

		float& animated_progress = progress_map[key];

		const float lerp_speed = config.gui.animspeed > 0 ? config.gui.animspeed / 100.f : 1.0f;
		animated_progress = animated_progress + (progress - animated_progress) * std::clamp(lerp_speed, 0.f, 1.f);

		float angle_start = -IM_PI / 2.f;
		float angle_end = angle_start + IM_PI * 2.f * animated_progress;

		draw->PathArcTo(pos, radius, angle_start, angle_end, 100);
		draw->PathLineTo(pos);
		draw->PathFillConvex(main_color);
	}
	else
	{
		draw->AddCircleFilled(pos, radius, main_color, 100);
	}

	draw->AddCircleFilled(pos, radius - 2.5f, background_color, 100);

	if (nade == "SMOKE")        icon = "k";
	else if (nade == "FLASH")   icon = "i";
	else if (nade == "HE")      icon = "j";
	else if (nade == "T_MOLOTOV")  icon = "l";
	else if (nade == "DECOY")   icon = "m";
	else if (nade == "CT_MOLOTOV") icon = "n";

	ImFont* font = gun_icon32;
	float font_size = 32.0f;
	ImVec2 text_size = font->CalcTextSizeA(font_size, FLT_MAX, 0.0f, icon.c_str());

	ImVec2 text_pos = ImVec2(
		pos.x - text_size.x * 0.5f,
		pos.y - text_size.y * 0.5f
	);

	draw->AddText(font, font_size, text_pos, IM_COL32(255, 255, 255, alpha), icon.c_str());
}

void c_render::update_background_drawlist(ImDrawList* draw_list)
{
	m_background_draw_list = draw_list;
}

bool c_render::world_to_screen(vec3_t& in, vec3_t& out)
{
	static auto screen_transofrm = reinterpret_cast<bool(__fastcall*)(vec3_t&, vec3_t&)>(utils::find_pattern(g_modules.client, "48 89 74 24 ? 57 48 83 EC ? 48 8B 05 ? ? ? ? 48 8B FA"));

	if (!screen_transofrm)
		return false;

	bool on_screen = screen_transofrm(in, out);
	if (on_screen)
		return false;

	const float screen_size_x = m_screen_size.x;
	const float screen_size_y = m_screen_size.y;

	out.x = ((out.x + 1.0f) * 0.5f) * screen_size_x;
	out.y = screen_size_y - (((out.y + 1.0f) * 0.5f) * screen_size_y);

	return true;
}

#define M_PI 3.14159265358979323846

void CircleFilled3D(const vec3_t& origin, float radius, const color_t& color_center, const color_t& color_edge, int segments)
{
	if (!g_ctx.local || !g_ctx.local->is_alive())
		return;

	std::vector<ImVec2> screen_points;

	for (int i = 0; i <= segments; ++i)
	{
		float theta = (float)i / (float)segments * 2.0f * float(M_PI);

		vec3_t point(origin);

		point.x += std::cos(theta) * radius;
		point.y += std::sin(theta) * radius;

		vec3_t screen_point;

		if (g_render->world_to_screen(point, screen_point))
			screen_points.emplace_back(screen_point.x, screen_point.y);
	}

	vec3_t screen_origin;
	if (!g_render->world_to_screen(const_cast<vec3_t&>(origin), screen_origin))
		return;

	auto draw = ImGui::GetBackgroundDrawList();
	for (size_t i = 1; i < screen_points.size(); ++i)
	{
		draw->AddTriangleFilled(
			ImVec2(screen_origin.x, screen_origin.y),
			screen_points[i - 1],
			screen_points[i],
			ImColor(color_center.r, color_center.g, color_center.b, color_center.a)
		);
	}
}
void Circle3D(const vec3_t& origin, float radius, const color_t& color, int segments = 150)
{
	if (!g_ctx.local || !g_ctx.local->is_alive())
		return;

	std::vector<ImVec2> screen_points;

	for (int i = 0; i <= segments; ++i)
	{
		float theta = (float)i / (float)segments * 2.0f * float(M_PI);

		vec3_t point(origin);
		point.x += std::cos(theta) * radius;
		point.y += std::sin(theta) * radius;

		vec3_t screen_point;
		if (g_render->world_to_screen(point, screen_point))
			screen_points.emplace_back(screen_point.x, screen_point.y);
	}

	auto draw = ImGui::GetBackgroundDrawList();

	for (size_t i = 1; i < screen_points.size(); ++i)
	{
		draw->AddLine(screen_points[i - 1], screen_points[i], ImColor(color.r, color.g, color.b, color.a), 2.0f);
	}
}
void Circle3D_Glow(const vec3_t& origin, float radius, const color_t& color, int segments = 150, int glow_strength = 6)
{
	if (!g_ctx.local || !g_ctx.local->is_alive())
		return;

	std::vector<ImVec2> screen_points;

	for (int i = 0; i <= segments; ++i)
	{
		float theta = (float)i / (float)segments * 2.0f * float(M_PI);

		vec3_t point(origin);
		point.x += std::cos(theta) * radius;
		point.y += std::sin(theta) * radius;

		vec3_t screen_point;

		if (g_render->world_to_screen(point, screen_point))
			screen_points.emplace_back(screen_point.x, screen_point.y);
	}

	auto draw = ImGui::GetBackgroundDrawList();

	const int glow_layers = 20;
	const float glow_max_thickness = 12.0f;

	for (int layer = glow_layers; layer > 0; --layer)
	{
		float t = (float)layer / glow_layers;
		float thickness = t * glow_max_thickness;
		float alpha = color.a * 0.25f * (t * t);

		ImColor glow_color(color.r, color.g, color.b, alpha);

		for (size_t i = 1; i < screen_points.size(); ++i)
		{
			draw->AddLine(screen_points[i - 1], screen_points[i], glow_color, thickness);
		}
	}

	for (size_t i = 1; i < screen_points.size(); ++i)
	{
		draw->AddLine(screen_points[i - 1], screen_points[i], ImColor(color.r, color.g, color.b, color.a), 2.0f);
	}
}

void c_render::autopick_cricle()
{
	color_t color1 = gui.accent_color;
	color_t color2 = gui.accent_color;

	color2.a = 0;

	if (!config.movement.m_autopeek.m_old_origin.is_zero() && config.movement.m_autopeek.m_autopeeking && g_ctx.local && g_ctx.local->is_alive())
	{
		Circle3D(config.movement.m_autopeek.m_old_origin, 15.f, color1);
	}
}

void c_render::line(vec3_t start_pos, vec3_t end_pos, c_color color)
{
	m_background_draw_list->AddLine(start_pos.im(), end_pos.im(), color.im());
}

void c_render::rect_filled(vec3_t start_pos, vec3_t end_pos, c_color color, float rounding, int flags)
{
	m_background_draw_list->AddRectFilled(start_pos.im(), end_pos.im(), color.im(), rounding, flags);
}

void c_render::rect_filled_multi_color(vec3_t start_pos, vec3_t end_pos, c_color top_left_clr, c_color top_right_clr, c_color bot_left_color, c_color bot_right_color, float rounding, int flags)
{
	m_background_draw_list->AddRectFilledMultiColor(start_pos.im(), end_pos.im(), top_left_clr.im(), top_right_clr.im(), bot_right_color.im(), bot_left_color.im(), rounding, flags);
}

void c_render::rect(vec3_t start_pos, vec3_t end_pos, c_color color, float thickness, float rounding)
{
	m_background_draw_list->AddRect(start_pos.im(), end_pos.im(), color.im(), rounding, 0, thickness);
}

void c_render::triangle(float x1, float y1, float x2, float y2, float x3, float y3, c_color clr, float thickness)
{
	m_background_draw_list->AddTriangle(ImVec2(x1, y1), ImVec2(x2, y2), ImVec2(x3, y3), clr.im(), thickness);
}

void c_render::filled_triangle(float x1, float y1, float x2, float y2, float x3, float y3, c_color clr)
{
	m_background_draw_list->AddTriangleFilled(ImVec2(x1, y1), ImVec2(x2, y2), ImVec2(x3, y3), clr.im());
}

void c_render::text(vec3_t txt_pos, const std::string_view& message, c_color color, ImFont* font, const int font_size, const int flags)
{
	m_background_draw_list->PushTextureID(font->ContainerAtlas->TexID);

	const auto size = font->CalcTextSizeA(font_size, FLT_MAX, 0.0f, message.data());
	auto pos = ImVec2(txt_pos.x, txt_pos.y);
	auto outline_clr = c_color(0, 0, 0, color.a * 0.3f);

	if (flags & font_flags_center)
		pos = ImVec2(txt_pos.x - size.x / 2.0f, txt_pos.y);

	if (flags & font_flags_dropshadow)
		m_background_draw_list->AddText(font, font_size, ImVec2(pos.x + 1, pos.y + 1), outline_clr.im(), message.data());

	if (flags & font_flags_outline)
	{
		m_background_draw_list->AddText(font, font_size, ImVec2(pos.x + 1, pos.y - 1), outline_clr.im(), message.data());
		m_background_draw_list->AddText(font, font_size, ImVec2(pos.x - 1, pos.y + 1), outline_clr.im(), message.data());
		m_background_draw_list->AddText(font, font_size, ImVec2(pos.x - 1, pos.y - 1), outline_clr.im(), message.data());
		m_background_draw_list->AddText(font, font_size, ImVec2(pos.x + 1, pos.y + 1), outline_clr.im(), message.data());
		m_background_draw_list->AddText(font, font_size, ImVec2(pos.x, pos.y + 1), outline_clr.im(), message.data());
		m_background_draw_list->AddText(font, font_size, ImVec2(pos.x, pos.y - 1), outline_clr.im(), message.data());
		m_background_draw_list->AddText(font, font_size, ImVec2(pos.x + 1, pos.y), outline_clr.im(), message.data());
		m_background_draw_list->AddText(font, font_size, ImVec2(pos.x - 1, pos.y), outline_clr.im(), message.data());
	}

	m_background_draw_list->AddText(font, font_size, pos, color.im(), message.data());
	m_background_draw_list->PopTextureID();
}

void c_render::radial_gradient_3d(vec3_t pos, float radius, c_color in, c_color out)
{
	ImVec2 center; vec3_t g_pos;

	world_to_screen(pos, g_pos);
	center = ImVec2(g_pos.x, g_pos.y);
	m_background_draw_list->_PathArcToFastEx(center, radius, 0, 48, 0);
	const int count = m_background_draw_list->_Path.Size - 1;
	float step = (3.141592654f * 2.0f) / (count + 1);
	std::vector<ImVec2> point;

	for (float lat = 0.f; lat <= 3.141592654f * 2.0f; lat += step)
	{
		const auto& point3d = vec3_t(sin(lat), cos(lat), 0.f) * radius;
		vec3_t point2d;
		auto shit_pos = pos + point3d;
		if (world_to_screen(shit_pos, point2d))
			point.push_back(ImVec2(point2d.x, point2d.y));
	}
	if (in.a == 0 && out.a == 0 || radius < 0.5f || point.size() < count + 1)
		return;

	unsigned int vtx_base = m_background_draw_list->_VtxCurrentIdx;
	m_background_draw_list->PrimReserve(count * 3, count + 1);

	auto uv = ImGui::GetFontTexUvWhitePixel();
	m_background_draw_list->PrimWriteVtx(center, uv, ImColor(in.r, in.g, in.b, in.a));
	for (int n = 0; n < count; n++)
		m_background_draw_list->PrimWriteVtx(point[n + 1], uv, ImColor(out.r, out.g, out.b, out.a));

	for (int n = 0; n < count; n++)
	{
		m_background_draw_list->PrimWriteIdx((ImDrawIdx)(vtx_base));
		m_background_draw_list->PrimWriteIdx((ImDrawIdx)(vtx_base + 1 + n));
		m_background_draw_list->PrimWriteIdx((ImDrawIdx)(vtx_base + 1 + ((n + 1) % count)));
	}

	m_background_draw_list->_Path.Size = 0;
}

vec3_t c_render::measure_text(const std::string& text, ImFont* font)
{
	auto res = font->CalcTextSizeA(font->FontSize, FLT_MAX, 0.f, text.c_str());
	return vec3_t(res.x, res.y);
}

Image_t* c_render::load_texture_from_file_in_memory(unsigned char* data, int data_size)
{
	auto img = new Image_t;

	D3DX11CreateShaderResourceViewFromMemory(directx::m_device, data, data_size, nullptr, nullptr, &img->resource, nullptr);

	ID3D11Texture2D* pTextureInterface = 0;
	img->resource->GetResource((ID3D11Resource**)&pTextureInterface);

	D3D11_TEXTURE2D_DESC desc;
	pTextureInterface->GetDesc(&desc);

	img->size = vec3_t((int)desc.Width, (int)desc.Height);

	return img;
}

float c_render::linear_animate(float current, float next, float speed)
{
	if (next == current)
		return current;

	float delta = next - current;
	float approach = (delta < 0) ? (-speed * ImGui::GetIO().DeltaTime) : (speed * ImGui::GetIO().DeltaTime);

	approach = std::clamp(approach, -abs(delta), abs(delta));

	return current + approach;
}