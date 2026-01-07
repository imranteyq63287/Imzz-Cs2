#include "../../../dependencies/imgui/imgui_internal.h"
#include <unordered_map>
#include "gui.hpp"
using namespace ImGui;

bool ComboBox(const char* label, int* current_item, const char* const items[], int items_count, int height_in_items = -1)
{
    // Лейбл слева
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 14); // Отступ слева
    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 4);

    ImGui::Text(label);
    ImGui::SameLine();
    ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 5);
    NewLine();

    float combo_width = 200; // Фиксированная ширина, как в MultiComba
    // Комбо-бокс справа с фиксированной шириной
    float right_margin = 0.0f;  // Отступ от правого края

    float combo_x = ImGui::GetWindowWidth() - right_margin - combo_width;
    ImGui::SetCursorPosX(12);
    //ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 20);
    float full_width = ImMin(ImGui::GetContentRegionAvail().x, 188.0f);

    ImGui::SetNextItemWidth(full_width);
    // Стили бордера и цветов
    ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 0.5f); // Толщина бордера
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 3.0f);   // Скругление углов

    ImGui::PushStyleColor(ImGuiCol_Border, gui.border.to_vec4()); // Цвет бордера
    ImGui::PushStyleColor(ImGuiCol_Button, gui.button.to_vec4());
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, gui.button.to_vec4());
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, gui.button.to_vec4());
    ImGui::PushStyleColor(ImGuiCol_FrameBg, gui.button.to_vec4());
    ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, gui.button.to_vec4());
    ImGui::PushStyleColor(ImGuiCol_FrameBgActive, gui.button.to_vec4());
    ImGui::PushStyleColor(ImGuiCol_Header, gui.frame_active.to_vec4());
    ImGui::PushStyleColor(ImGuiCol_HeaderHovered, gui.frame_inactive.to_vec4());
    ImGui::PushStyleColor(ImGuiCol_HeaderActive, gui.frame_active.to_vec4());
    bool value_changed = ImGui::Combo(("##" + std::string(label)).c_str(), current_item, items, items_count, height_in_items);

    // Сброс стилей
    ImGui::PopStyleVar(2);     // Снимаем FrameBorderSize и FrameRounding
    ImGui::PopStyleColor(10);   // Снимаем цвета

    ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 3);

    return value_changed;
}
bool ComboBoxWG(int* current_item, const char* const items[], int items_count, const char* label = "sosal", int height_in_items = -1)
{

    ImGui::SetCursorPosX(12);
    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 3);
    float full_width = ImMin(ImGui::GetContentRegionAvail().x, 188.0f);

    ImGui::SetNextItemWidth(full_width);
    // Стили бордера и цветов
    ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 0.5f); // Толщина бордера
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 3.0f);   // Скругление углов

    ImGui::PushStyleColor(ImGuiCol_Border, gui.border.to_vec4()); // Цвет бордера
    ImGui::PushStyleColor(ImGuiCol_Button, gui.button.to_vec4());
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, gui.button.to_vec4());
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, gui.button.to_vec4());
    ImGui::PushStyleColor(ImGuiCol_FrameBg, gui.button.to_vec4());
    ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, gui.button.to_vec4());
    ImGui::PushStyleColor(ImGuiCol_FrameBgActive, gui.button.to_vec4());
    ImGui::PushStyleColor(ImGuiCol_Header, gui.frame_active.to_vec4());
    ImGui::PushStyleColor(ImGuiCol_HeaderHovered, gui.frame_inactive.to_vec4());
    ImGui::PushStyleColor(ImGuiCol_HeaderActive, gui.frame_active.to_vec4());
    bool value_changed = ImGui::Combo(("##" + std::string(label)).c_str(), current_item, items, items_count, height_in_items);

    // Сброс стилей
    ImGui::PopStyleVar(2);     // Снимаем FrameBorderSize и FrameRounding
    ImGui::PopStyleColor(10);   // Снимаем цвета

    ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 9);

    return value_changed;
}
void MultiComba(const char* label, bool combos[], const char* items[], int items_count)
{
    ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 0.5f); // Толщина бордера
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 3.0f);   // Скругление углов

    ImGui::PushStyleColor(ImGuiCol_Border, gui.border.to_vec4()); // Цвет бордера
    ImGui::PushStyleColor(ImGuiCol_Button, gui.button.to_vec4());
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, gui.button.to_vec4());
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, gui.button.to_vec4());
    ImGui::PushStyleColor(ImGuiCol_FrameBg, gui.button.to_vec4());
    ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, gui.button.to_vec4());
    ImGui::PushStyleColor(ImGuiCol_FrameBgActive, gui.button.to_vec4());
    ImGui::PushStyleColor(ImGuiCol_Header, gui.frame_active.to_vec4());
    ImGui::PushStyleColor(ImGuiCol_HeaderHovered, gui.frame_inactive.to_vec4());
    ImGui::PushStyleColor(ImGuiCol_HeaderActive, gui.frame_active.to_vec4());
    static std::unordered_map<std::string, std::string> previews;
    std::string label_id = std::string("##") + label; // Уникальный идентификатор

    if (previews.find(label_id) == previews.end())
        previews[label_id] = "None"; // Инициализируем превью для нового label

    std::vector<std::string> vec;
    std::string& preview = previews[label_id]; // Получаем превью для конкретного label

    int selected_count = 0;
    for (int i = 0; i < items_count; i++)
    {
        if (combos[i])
        {
            vec.push_back(items[i]);
            selected_count++;

            if (selected_count > 3)
                preview = vec[0] + ", " + vec[1] + ", " + vec[2] + ", ...";
            else if (selected_count > 1)
                preview += ", " + std::string(items[i]);
            else
                preview = items[i];
        }
    }

    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 14); // Отступ слева
    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 4);

    ImGui::Text(label);
    ImGui::NewLine();
    ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 21);
    float combo_width = ImGui::GetContentRegionAvail().x; // Фиксированная ширина, как в MultiComba
    float right_margin = 0.0f;
    float combo_x = ImGui::GetWindowWidth() - right_margin - combo_width;
    ImGui::SetCursorPosX(12);
    float full_width = ImMin(ImGui::GetContentRegionAvail().x, 188.0f);

    ImGui::SetNextItemWidth(full_width);
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 3.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 0.5f);
    ImGui::PushStyleColor(ImGuiCol_Border, gui.border.to_vec4()); // Цвет бордера

    ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 7);

    if (ImGui::BeginCombo(label_id.c_str(), preview.c_str()))
    {
        for (int i = 0; i < items_count; i++)
        {
            ImGui::Selectable(items[i], &combos[i], ImGuiSelectableFlags_DontClosePopups);
        }
        ImGui::EndCombo();
    }

    ImGui::PopStyleVar(4);
    ImGui::PopStyleColor(11);
    ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 3);

    if (selected_count == 0)
        preview = "None";
}