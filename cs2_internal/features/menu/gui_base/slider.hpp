#include "../../../dependencies/imgui/imgui_internal.h"
#include <unordered_map>
#include "gui.hpp"
#include "../../../Bind System/key.h"

void DrawCustomSliderFloat(const char* label, float* value, float min, float max, bool has_next_element = true, float changevalue = -99999.0f, const char* changetext = "AUTO", int binded = 0, int* key = 0, int* key_style = 0, int* override = 0, float step = 0.1f)
{
    ImGuiWindow* window = ImGui::GetCurrentWindow();
    if (window->SkipItems)
        return;

    ImGuiContext& g = *ImGui::GetCurrentContext();
    ImGuiStyle& style = g.Style;

    ImGui::PushID(label);
    SetCursorPos(ImVec2(GetCursorPos().x + 15, GetCursorPos().y + 5));

    const char* id_pos = strstr(label, "##");
    if (id_pos) {
        ImGui::TextUnformatted(std::string(label, id_pos - label).c_str());
    }
    else {
        ImGui::TextUnformatted(label);
    }

    float label_width = ImGui::CalcTextSize(label).x;
    ImGui::SameLine();

    char value_buf[32];
    if (*value == changevalue && changetext) {
        snprintf(value_buf, sizeof(value_buf), "%s", changetext);
    }
    else {
        snprintf(value_buf, sizeof(value_buf), "%.2f", *value);  // float формат
    }

    float value_width = ImGui::CalcTextSize(value_buf).x;
    ImGui::SetCursorPosX(ImGui::GetContentRegionMax().x - value_width);
    ImGui::TextUnformatted("");

    float bar_height = 5.0f;
    float full_width = ImGui::GetContentRegionAvail().x - 15.0f;
    full_width = ImMin(full_width, 185.0f);

    ImVec2 bar_pos = ImVec2(ImGui::GetCursorScreenPos().x + 15.0f, ImGui::GetCursorScreenPos().y - 5);
    ImVec2 bar_end = ImVec2(bar_pos.x + full_width, bar_pos.y + bar_height);

    window->DrawList->AddRectFilled(bar_pos, bar_end, gui.bg.to_im_color(), 0.0f);

    static std::unordered_map<ImGuiID, float> animated_fill;
    ImGuiID id = window->GetID("##custom_slider");

    float target_t = (*value - min) / (max - min);
    float& anim_t = animated_fill[id];
    float speed = 10.0f * g.IO.DeltaTime;
    anim_t = ImLerp(anim_t, target_t, ImClamp(speed, 0.0f, 1.0f));

    float anim_fill_width = full_width * anim_t;
    if (anim_fill_width > 0.0f)
    {
        window->DrawList->AddRectFilled(bar_pos, ImVec2(bar_pos.x + anim_fill_width, bar_end.y), gui.accent_color.to_im_color(), 0.0f);

        ImVec2 text_size = ImGui::CalcTextSize(value_buf);
        float text_x = bar_pos.x + anim_fill_width;
        if (text_x + text_size.x > bar_end.x)
            text_x = bar_end.x - text_size.x;

        ImU32 outline_col = IM_COL32(0, 0, 0, 150);
        ImVec2 pos = ImVec2(text_x, bar_end.y - 5);

        window->DrawList->AddText(ImVec2(pos.x - 1, pos.y), outline_col, value_buf);
        window->DrawList->AddText(ImVec2(pos.x + 1, pos.y), outline_col, value_buf);
        window->DrawList->AddText(ImVec2(pos.x, pos.y - 1), outline_col, value_buf);
        window->DrawList->AddText(ImVec2(pos.x, pos.y + 1), outline_col, value_buf);
        window->DrawList->AddText(pos, gui.text.to_im_color(), value_buf);
    }

    ImGui::SetCursorScreenPos(bar_pos);
    ImGui::InvisibleButton("##custom_slider", ImVec2(full_width, bar_height));
    if (has_next_element)
        ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 5.0f);

    if (ImGui::IsItemActive() && ImGui::IsMouseDragging(ImGuiMouseButton_Left))
    {
        float mouse_x = ImGui::GetIO().MousePos.x;
        float rel_x = ImClamp(mouse_x - bar_pos.x, 0.0f, full_width);
        float raw_value = min + (rel_x / full_width) * (max - min);

        float stepped_value = roundf(raw_value / step) * step;
        *value = ImClamp(stepped_value, min, max);
    }

    ImGui::PopID();
}

void DrawCustomSliderInt(const char* label, int* value, int min, int max, bool has_next_element = true, int changevalue = -99999, const char* changetext = "AUTO", int binded = 0, int* key = 0, int* key_style = 0, int* override = 0, int step = 1)
{
    ImGuiWindow* window = ImGui::GetCurrentWindow();
    if (window->SkipItems)
        return;

    ImGuiContext& g = *ImGui::GetCurrentContext();
    ImGuiStyle& style = g.Style;

    ImGui::PushID(label);
    SetCursorPos(ImVec2(GetCursorPos().x + 15, GetCursorPos().y + 5));
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
    ImGui::TextUnformatted("");

    float bar_height = 5.0f;
    float full_width = ImGui::GetContentRegionAvail().x - 15.0f; // уменьшаем ширину на 15px
    full_width = ImMin(full_width, 185.0f);

    ImVec2 bar_pos = ImVec2(ImGui::GetCursorScreenPos().x + 15.0f, ImGui::GetCursorScreenPos().y - 5); // сдвигаем полосу на 15px
    ImVec2 bar_end = ImVec2(bar_pos.x + full_width, bar_pos.y + bar_height);

    // Фон полоски
    window->DrawList->AddRectFilled(ImVec2(bar_pos.x, bar_pos.y), bar_end, gui.bg.to_im_color(), 0.0f);

    // === Плавность через статическую переменную ===
    static std::unordered_map<ImGuiID, float> animated_fill;
    ImGuiID id = window->GetID("##custom_slider");

    float target_t = float(*value - min) / float(max - min);
    float& anim_t = animated_fill[id];

    float speed = 10.0f * g.IO.DeltaTime;
    anim_t = ImLerp(anim_t, target_t, ImClamp(speed, 0.0f, 1.0f));

    float anim_fill_width = full_width * anim_t;

    if (anim_fill_width > 0.0f)
    {
        window->DrawList->AddRectFilled(
            bar_pos,
            ImVec2(bar_pos.x + anim_fill_width, bar_end.y),
            gui.accent_color.to_im_color(),
            0.0f
        );
        ImVec2 text_size = ImGui::CalcTextSize(value_buf);
        float text_x = bar_pos.x + anim_fill_width;

        // Не даём выйти за правую границу
        if (text_x + text_size.x > bar_end.x)
            text_x = bar_end.x - text_size.x;
        ImU32 outline_col = IM_COL32(0, 0, 0, 150); // чёрная обводка
        ImVec2 pos = ImVec2(text_x, bar_end.y - 5);

        // 4 смещённых контура
        window->DrawList->AddText(ImVec2(pos.x - 1, pos.y), outline_col, value_buf);
        window->DrawList->AddText(ImVec2(pos.x + 1, pos.y), outline_col, value_buf);
        window->DrawList->AddText(ImVec2(pos.x, pos.y - 1), outline_col, value_buf);
        window->DrawList->AddText(ImVec2(pos.x, pos.y + 1), outline_col, value_buf);

        // Основной текст
        window->DrawList->AddText(pos, gui.text.to_im_color(), value_buf);


    }

    // Область взаимодействия
    ImGui::SetCursorScreenPos(ImVec2(bar_pos.x, bar_pos.y));
    ImGui::InvisibleButton("##custom_slider", ImVec2(full_width, bar_height));
    if (has_next_element)
    {
        ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 5.0f);
    }
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
    if (binded == 1)
    {
        if (IsItemClicked(ImGuiMouseButton_Right)) {
            OpenPopup(label);
        }
        if (BeginPopup(label, ImGuiWindowFlags_NoMove)) {
            SetCursorPos(ImVec2(0, 0));
            gui.group_box(("Hotkey"), ImVec2(200, 180));
            {
                key::BindSlider(label, value, min, max, key, key_style, override, changevalue, changetext);
            }gui.end_group_box();
            EndPopup();

        }
    }
    ImGui::PopID();
}
