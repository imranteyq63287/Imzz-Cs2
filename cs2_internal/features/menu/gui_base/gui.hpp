#pragma once

#define  IMGUI_DEFINE_MATH_OPERATORS
#include "../../../dependencies/imgui/imgui.h"
#include "../../../dependencies/imgui/imgui_internal.h"
#include "color_t.hpp"

#include <string>
#include <vector>
#include <unordered_map>
#include <functional>

using namespace std;
struct c_vector_2d {
    float x, y;
    c_vector_2d(float x = 0, float y = 0) : x(x), y(y) {}
};


class c_gui {

public:

    float m_anim = 0.f;
    float f_anim = 0.f;

    int m_tab = 0;
    ImVec2 active_bar_pos = ImVec2(10, 20);
    ImVec2 target_bar_pos = ImVec2(0, 0);
    ImVec2 active_bar_size = ImVec2(5, 55);
    bool bar_initialized = false;
    float active_value = 0.0f; // ƒл€ плавного затухани€ фона
    std::string active_icon = ""; // ƒл€ иконки (если нужно)
    std::string active_label = ""; // ƒл€ текста (если хочешь отрисовать)


    int m_rage_subtab = 0;
    color_t accent_color = { 102.f / 255.f, 171.f / 255.f, 208.f / 255.f, 1.0f }; // јкцентный цвет

    color_t text = { 1.00f, 1.00f, 1.00f, 1.00f }; // ÷вет текста
    color_t text_disabled = { 1.00f, 1.00f, 1.00f, 0.50f }; // ÷вет отключенного текста

    color_t border = { 1.00f, 1.00f, 1.00f, 0.05f }; // ÷вет границы
    color_t bordercb = { 1.00f, 1.00f, 1.00f, 0.1f }; // ÷вет границы

    color_t frame_inactive = { 0.80f, 0.80f, 0.80f, 0.30f }; // ÷вет фона рамки
    color_t frame_active = { 1.00f, 1.00f, 1.00f, 0.15f }; // ÷вет активной рамки

    color_t button = { 0.114f, 0.122f, 0.129f, 0.47f }; // ÷вет кнопки
    color_t button_hovered = { 0.114f, 0.122f, 0.129f, 0.47f }; // ÷вет кнопки при наведении
    color_t button_active = { 0.114f, 0.122f, 0.129f, 0.70f }; // ÷вет кнопки при активации

    color_t group_box_bg = { 35.f / 255.f, 35.f / 255.f, 35.f / 255.f, 1.0f };; // ÷вет фона группы

    color_t bg = { 17 / 255.f, 17 / 255.f, 17 / 255.f, 1.0f }; // ÷вет фона группы
    color_t fg = { 17 / 255.f, 17 / 255.f, 17 / 255.f, 0.4f }; // ÷вет фона группы
    color_t bg2 = { 16 / 255.f, 15 / 255.f, 21 / 255.f, 1.0f }; // ÷вет фона группы


    void render_circle_for_horizontal_bar(ImVec2 pos, ImColor color, float alpha);
    void render_active_bar();
    inline void group_title(const char* name) {

        SetCursorPosX(GetCursorPosX() + 10);
        PushStyleColor(ImGuiCol_Text, ImVec4(1.f, 1.f, 1.f, 0.5f));
        Text(name);
        PopStyleColor();
    }

    void group_box(const char* name, ImVec2 size_arg, ImGuiWindowFlags flags = ImGuiWindowFlags_None);
    void end_group_box();

    bool tab(const char* icon, const char* label, bool selected);
    bool subtab(const char* label, bool selected, int size, ImDrawFlags flags);

};

inline c_gui gui;
