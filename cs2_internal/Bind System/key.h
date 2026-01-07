#pragma once
#include "../includes.h"
#include <Windows.h>
#include <memory>
#include "../dependencies/imgui/imgui_internal.h"
#include "../features/menu/gui_base/gui.hpp"
using namespace ImGui;
inline const char* keys[] =
{
    "None",
    "Mouse 1",
    "Mouse 2",
    "CN",
    "Mouse 3",
    "Mouse 4",
    "Mouse 5",
    "-",
    "Back",
    "Tab",
    "-",
    "-",
    "CLR",
    "Enter",
    "-",
    "-",
    "Shift",
    "CTL",
    "Menu",
    "Pause",
    "Caps Lock",
    "KAN",
    "-",
    "JUN",
    "FIN",
    "KAN",
    "-",
    "Escape",
    "CON",
    "NCO",
    "ACC",
    "MAD",
    "Space",
    "PGU",
    "PGD",
    "End",
    "Home",
    "Left",
    "Up",
    "Right",
    "Down",
    "SEL",
    "PRI",
    "EXE",
    "PRI",
    "INS",
    "Delete",
    "HEL",
    "0",
    "1",
    "2",
    "3",
    "4",
    "5",
    "6",
    "7",
    "8",
    "9",
    "-",
    "-",
    "-",
    "-",
    "-",
    "-",
    "-",
    "A",
    "B",
    "C",
    "D",
    "E",
    "F",
    "G",
    "H",
    "I",
    "J",
    "K",
    "L",
    "M",
    "N",
    "O",
    "P",
    "Q",
    "R",
    "S",
    "T",
    "U",
    "V",
    "W",
    "X",
    "Y",
    "Z",
    "WIN",
    "WIN",
    "APP",
    "-",
    "SLE",
    "Numpad 0",
    "Numpad 1",
    "Numpad 2",
    "Numpad 3",
    "Numpad 4",
    "Numpad 5",
    "Numpad 6",
    "Numpad 7",
    "Numpad 8",
    "Numpad 9",
    "MUL",
    "ADD",
    "SEP",
    "MIN",
    "Delete",
    "DIV",
    "F1",
    "F2",
    "F3",
    "F4",
    "F5",
    "F6",
    "F7",
    "F8",
    "F9",
    "F10",
    "F11",
    "F12",
    "F13",
    "F14",
    "F15",
    "F16",
    "F17",
    "F18",
    "F19",
    "F20",
    "F21",
    "F22",
    "F23",
    "F24",
    "-",
    "-",
    "-",
    "-",
    "-",
    "-",
    "-",
    "-",
    "NUM",
    "SCR",
    "EQU",
    "MAS",
    "TOY",
    "OYA",
    "OYA",
    "-",
    "-",
    "-",
    "-",
    "-",
    "-",
    "-",
    "-",
    "-",
    "Shift",
    "Shift",
    "Ctrl",
    "Ctrl",
    "Alt",
    "Alt"
};
inline void DrawCustomSlider(const char* label, int* value, int min, int max, int changevalue = -99999, const char* changetext = "AUTO", int binded = 0, int key = 0, int key_style = 0, int override = 0, int step = 1)
{
    ImGuiWindow* window = ImGui::GetCurrentWindow();
    if (window->SkipItems)
        return;

    ImGuiContext& g = *ImGui::GetCurrentContext();
    ImGuiStyle& style = g.Style;

    ImGui::PushID(label);

    // === Первая строка: Label и значение ===
    const char* id_pos = strstr(label, "##");
    if (id_pos) {
        ImGui::TextUnformatted(std::string(label, id_pos - label).c_str());
    }
    else {
        ImGui::TextUnformatted(label);
    }
    float label_width = ImGui::CalcTextSize(label).x;

    ImGui::SameLine();

    // Значение справа
    char value_buf[16];
    if (*value == changevalue && changetext) {
        snprintf(value_buf, sizeof(value_buf), "%s", changetext);
    }
    else {
        snprintf(value_buf, sizeof(value_buf), "%d", *value);
    }
    float value_width = ImGui::CalcTextSize(value_buf).x;

    ImGui::SetCursorPosX(ImGui::GetContentRegionMax().x - value_width);
    ImGui::TextUnformatted(value_buf);

    // === Вторая строка: Прогрессбар на всю ширину ===
    float bar_height = 10.0f;
    float full_width = ImGui::GetContentRegionAvail().x;
    ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 4);

    ImVec2 bar_pos = ImGui::GetCursorScreenPos();
    ImVec2 bar_end = ImVec2(bar_pos.x + full_width, bar_pos.y + bar_height);

    // Фон полоски
    window->DrawList->AddRectFilled(bar_pos, bar_end, gui.bg.to_im_color(), 4.0f);

    // === Плавность через статическую переменную ===
    static std::unordered_map<ImGuiID, float> animated_fill;
    ImGuiID id = window->GetID("##custom_slider");

    float target_t = float(*value - min) / float(max - min);
    float& anim_t = animated_fill[id];

    float speed = 10.0f * g.IO.DeltaTime; // скорость интерполяции
    anim_t = ImLerp(anim_t, target_t, ImClamp(speed, 0.0f, 1.0f));

    float anim_fill_width = full_width * anim_t;

    if (*value != min && anim_fill_width > 0.0f)
    {
        window->DrawList->AddRectFilled(
            bar_pos,
            ImVec2(bar_pos.x + anim_fill_width, bar_end.y),
            gui.accent_color.to_im_color(),
            4.0f
        );
    }

    // Область взаимодействия
    ImGui::SetCursorScreenPos(ImVec2(bar_pos.x, bar_pos.y));
    ImGui::InvisibleButton("##custom_slider", ImVec2(full_width, bar_height));
    if (ImGui::IsItemActive() && ImGui::IsMouseDragging(ImGuiMouseButton_Left))
    {
        float mouse_x = ImGui::GetIO().MousePos.x;
        float rel_x = ImClamp(mouse_x - bar_pos.x, 0.0f, full_width);

        int raw_value = min + int((rel_x / full_width) * (max - min));

        // Применяем шаг — округляем raw_value к ближайшему кратному step
        int stepped_value = ((raw_value + step / 2) / step) * step;

        // Ограничиваем диапазон
        *value = ImClamp(stepped_value, min, max);
    }

    ImGui::PopID();
}

namespace key
{
    inline void Bind(const char* label, bool* value, int* key, int* key_style) {
        const char* modes[] = { "Hold", "Toggle", "Always"};
        ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
        ImGui::Combo((std::string("##mode_") + label).c_str(), key_style, modes, IM_ARRAYSIZE(modes));
        ImGui::Keybind((label), key, key_style);
	}
    inline void BindSlider(const char* label, int* value, int min, int max, int* key, int* key_style, int* override, int changevalue = -99999, const char* changetext = "AUTO") {
        const char* modes[] = { "Hold", "Toggle", "Always" };
        ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
        ImGui::Combo((std::string("##mode_") + label).c_str(), key_style, modes, IM_ARRAYSIZE(modes));
        ImGui::Keybind(label, key, key_style);
        DrawCustomSlider("Override", override, min, max, changevalue, changetext);
    }

};
#include <unordered_map>

class c_key_handler {
private:
    std::unordered_map<int, bool> toggled_state;
    std::unordered_map<int, bool> was_pressed;

public:
    bool is_pressed(int key, int key_style) {
        if (key <= 0 || key >= 256)
            return false;

        bool is_down = (GetAsyncKeyState(key) & 0x8000);

        switch (key_style) {
        case 0: // Hold
            return is_down;

        case 1: // Toggle
            if (is_down && !was_pressed[key]) {
                toggled_state[key] = !toggled_state[key]; // Переключить состояние
            }

            was_pressed[key] = is_down; // Обновляем предыдущее состояние

            return toggled_state[key];

        case 2: // Always
            return true;

        default:
            return false;
        }
    }
};


const auto g_key_handler = std::make_unique< c_key_handler >();
