#include "gui.hpp"
#include "Source.hpp"
using namespace ImGui;

void c_gui::render_circle_for_horizontal_bar(ImVec2 pos, ImColor color, float alpha) {

    auto draw = GetWindowDrawList();
    draw->AddCircleFilled(pos, 6, ImColor(color.Value.x, color.Value.y, color.Value.z, alpha * GetStyle().Alpha));
}
void c_gui::render_active_bar() {
    if (!bar_initialized)
        return;

    auto window = ImGui::GetCurrentWindow();
    ImVec2 pos = window->DC.CursorPos;
    ImVec2 size = ImVec2(ImGui::GetWindowWidth() - 85, 52);  // Высота полоски 52 пикселя
    ImRect bb(pos, pos + size);

    ImDrawList* draw = ImGui::GetWindowDrawList();

    // Устанавливаем фиксированную позицию полоски по оси X
    active_bar_pos.x = pos.x;  // фиксируем X в зависимости от положения окна

    // Плавно двигаем полоску по оси Y к целевой позиции
    active_bar_pos.y = ImLerp(active_bar_pos.y, target_bar_pos.y, 0.15f);

    // Фон/рамка всей полоски
    draw->AddRectFilled(
        ImVec2(active_bar_pos.x + 5, active_bar_pos.y),
        ImVec2(active_bar_pos.x + ImGui::GetWindowWidth() - 85, active_bar_pos.y + 52),
        gui.frame_active.to_im_color(0.5f),
        10.0f
    );

    // Новые размеры для синей полоски
    float blue_bar_height = 30.0f;
    float vertical_offset = (52.0f - blue_bar_height) / 2.0f;  // Центрирование полоски по оси Y

    // Синяя полоска с фиксированным положением по оси X и плавным движением по оси Y
    draw->AddRectFilled(
        ImVec2(active_bar_pos.x, active_bar_pos.y + vertical_offset),
        ImVec2(active_bar_pos.x + active_bar_size.x, active_bar_pos.y + vertical_offset + blue_bar_height),
        gui.accent_color.to_im_color(),
        4.0f
    );
}



bool c_gui::tab(const char* icon, const char* label, bool selected) {
    auto window = ImGui::GetCurrentWindow();
    auto id = window->GetID(label);

    auto icon_size = ImGui::CalcTextSize(icon);
    auto label_size = ImGui::CalcTextSize(label, 0, true);

    ImVec2 pos = window->DC.CursorPos;
    ImDrawList* draw = window->DrawList;

    ImVec2 size = ImVec2(ImGui::GetWindowWidth() - 85, 55);
    ImRect bb(pos, pos + size);

    ImGui::ItemAdd(bb, id);
    ImGui::ItemSize(bb, ImGui::GetStyle().FramePadding.y);

    bool hovered, held;
    bool pressed = ImGui::ButtonBehavior(bb, id, &hovered, &held);

    // Анимация фона вкладки
    static std::unordered_map<ImGuiID, float> values;
    float& val = values[id];
    val = ImLerp(val, selected ? 1.f : 0.f, 0.07f);

    //draw->AddRectFilled(bb.Min, bb.Max, gui.frame_active.to_im_color(0.5f * val), 10.0f);

    // Обновляем target для синей полоски
    if (selected) {
        target_bar_pos = ImVec2(bb.Min.x, bb.Min.y);
        active_bar_size.y = bb.GetHeight();

        if (!bar_initialized) {
            active_bar_pos = target_bar_pos;
            bar_initialized = true;
        }
    }

    // Цвет иконки с плавной анимацией
    ImVec4 active_color = gui.accent_color.to_im_color();
    ImVec4 start_color = gui.text_disabled.to_im_color();
    ImVec4 lerped = ImVec4(
        ImLerp(start_color.x, active_color.x, val),
        ImLerp(start_color.y, active_color.y, val),
        ImLerp(start_color.z, active_color.z, val),
        (selected == 0.0f) ? 0.5f : 1.0f // Если значение val = 0 (начало), то альфа = 0.5, иначе 1.0
    );


    draw->AddText(ImVec2(bb.Min.x + 18, bb.GetCenter().y - label_size.y / 2 + 4), ImColor(lerped), icon);

    return pressed;
}



bool c_gui::subtab(const char* label, bool selected, int size, ImDrawFlags flags) {

    auto window = GetCurrentWindow();
    auto id = window->GetID(label);

    auto label_size = CalcTextSize(label, 0, 1);

    auto pos = window->DC.CursorPos;
    auto draw = window->DrawList;

    ImRect bb(pos, pos + ImVec2(GetWindowWidth() / size, GetWindowHeight()));
    ItemAdd(bb, id);
    ItemSize(bb, GetStyle().FramePadding.y);

    bool hovered, held;
    bool pressed = ButtonBehavior(bb, id, &hovered, &held);

    static std::unordered_map < ImGuiID, float > values;
    auto value = values.find(id);
    if (value == values.end()) {

        values.insert({ id, 0.f });
        value = values.find(id);
    }

    value->second = ImLerp(value->second, (selected ? 1.f : 0.f), 0.05f);

    draw->AddRectFilled(bb.Min, bb.Max, gui.frame_active.to_im_color(0.8f * value->second), 4, flags);

    draw->AddText(bb.GetCenter() - label_size / 2, selected ? gui.text.to_im_color() : gui.text_disabled.to_im_color(), label);

    return pressed;
}
void c_gui::group_box(const char* name, ImVec2 size_arg, ImGuiWindowFlags flags) {
    auto window = GetCurrentWindow();
    auto pos = window->DC.CursorPos;

    BeginChild(std::string(name).append(".main").c_str(), size_arg, false, ImGuiWindowFlags_NoScrollbar | flags);

    GetWindowDrawList()->AddRectFilled(pos + ImVec2(0, 13), pos + size_arg, IM_COL32(23, 23, 23, static_cast<int>(gui.f_anim * 255.f)), 0);
    GetWindowDrawList()->AddRect(pos + ImVec2(0, 13), pos + size_arg, gui.border.to_im_color(), 0);

    if (!strchr(name, '#')) {
        GetWindowDrawList()->AddText(pos + ImVec2(13, 5), GetColorU32(ImGuiCol_Text, 1.0f), name);
    }

    SetCursorPos(ImVec2(12, 10));
    PushStyleVar(ImGuiStyleVar_WindowPadding, { 0, 10 });
    BeginChild(name, { size_arg.x - 24, size_arg.y - 21 }, 0, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_AlwaysUseWindowPadding | flags);

    BeginGroup();
    PushStyleVar(ImGuiStyleVar_ItemSpacing, { 8, 10 });
    PushStyleVar(ImGuiStyleVar_Alpha, gui.m_anim);
}



// Завершение всех групп, дочерних окон и стилей
void c_gui::end_group_box() {
    PopStyleVar(2); // Закрытие `ItemSpacing` и `Alpha`
    EndGroup();
    EndChild(); // Завершение дочернего окна содержимого
    PopStyleVar(); // Закрытие `WindowPadding`
    EndChild(); // Завершение основного дочернего окна группы
}

