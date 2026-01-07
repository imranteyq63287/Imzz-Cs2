#include "../../../dependencies/imgui/imgui_internal.h"
using namespace ImGui;
bool oldcheckbox(const char* label, bool* v)
{
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    const ImGuiID id = window->GetID(label);
    const ImVec2 label_size = CalcTextSize(label, NULL, true);

    const float square_sz = 10.0f; // Размер квадрата
    const ImVec2 pos = window->DC.CursorPos;
    const float text_offset_y = (label_size.y - square_sz) * 0.5f; // Центрируем квадрат

    const ImRect total_bb(ImVec2(pos.x, pos.y), ImVec2(pos.x + square_sz + style.ItemInnerSpacing.x, label_size.y + style.FramePadding.y * 2.0f));
    ItemSize(ImVec2(20, 10), 12);
    if (!ItemAdd(total_bb, id))
        return false;

    bool hovered, held;
    bool pressed = ButtonBehavior(total_bb, id, &hovered, &held);
    if (pressed)
    {
        *v = !(*v);
        MarkItemEdited(id);
    }

    // Позиция квадрата ниже, чтобы был на уровне текста
    const ImVec2 check_pos = ImVec2(pos.x, pos.y + text_offset_y + 0.5f);
    const ImRect check_bb(ImVec2(check_pos.x, check_pos.y), ImVec2(check_pos.x + square_sz, check_pos.y + square_sz));

    ImU32 check_col = *v ? (ImU32)gui.accent_color.to_im_color() : IM_COL32(70, 70, 70, 255); // Зеленый, если активен

    window->DrawList->AddRectFilled(check_bb.Min, check_bb.Max, check_col, 1.0f);

    // Смещаем текст ближе к квадрату
    const ImVec2 label_pos = ImVec2(check_bb.Max.x + style.ItemInnerSpacing.x, pos.y);
    if (label_size.x > 0.0f)
        RenderText(label_pos, label);

    return pressed;
}
static std::unordered_map<ImGuiID, float> checkbox_anim;

bool checkbox(const char* label, bool* v, int binded = 0, int* key = 0, int* key_style = 0)
{
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    const ImGuiID id = window->GetID(label);
    const ImVec2 label_size = CalcTextSize(label, NULL, true);
    const float square_sz = 10.f;
    const ImVec2 pos = window->DC.CursorPos;

    const ImVec2 total_size = ImVec2(
        square_sz + (label_size.x > 0.0f ? style.ItemInnerSpacing.x + label_size.x : 0.0f),
        square_sz - 5
    );

    const ImRect total_bb(ImVec2(pos.x, pos.y + 7), ImVec2(pos.x + total_size.x, pos.y + total_size.y + 11));
    const ImRect total_bbs(ImVec2(pos.x, pos.y + 7), ImVec2(pos.x + total_size.x, pos.y + total_size.y + 14));
    ItemSize(total_bb, 0);
    if (!ItemAdd(total_bb, id))
        return false;

    bool hovered, held;
    bool pressed = ButtonBehavior(total_bbs, id, &hovered, &held);
    if (pressed)
    {
        *v = !*v;
        MarkItemEdited(id);
    }

    ImVec2 check_bb_min = ImVec2(pos.x, pos.y + 8);
    ImVec2 check_bb_max = ImVec2(check_bb_min.x + square_sz, check_bb_min.y + square_sz);
    ImRect check_bb(check_bb_min, check_bb_max);

    window->DrawList->AddRectFilled(check_bb.Min, check_bb.Max, gui.bg.to_im_color(), 0.f);
    window->DrawList->AddRect(check_bb.Min, check_bb.Max, gui.bordercb.to_im_color(), 0.f);

    if (*v)
    {
        ImU32 check_col = ImGui::ColorConvertFloat4ToU32(gui.accent_color.to_im_color());

        float pad = square_sz / 6.0f;
        if (pad < 1.0f) pad = 1.0f;
        window->DrawList->AddRectFilled(check_bb.Min, check_bb.Max, gui.accent_color.to_im_color(), 0.f);

        ImVec2 checkmark_pos = ImVec2(check_bb.Min.x + pad, check_bb.Min.y + pad);
        RenderCheckMark(window->DrawList, checkmark_pos, check_col, square_sz - pad * 2.0f);
    }

    if (label_size.x > 0.0f)
    {
        ImVec2 text_pos = ImVec2(check_bb.Max.x + style.ItemInnerSpacing.x, pos.y + style.FramePadding.y);
        RenderText(text_pos, label);
    }
    if (binded == 1)
    {
        if (IsItemClicked(ImGuiMouseButton_Right)) {
            OpenPopup(label);
        }
        if (BeginPopup(label, ImGuiWindowFlags_NoMove)) {

            SetCursorPos(ImVec2(0, 0));
            gui.group_box(("Hotkey"), ImVec2(150, 110));
            {
                key::Bind(label, v, key, key_style);
            }gui.end_group_box();
            EndPopup();

        }
    }

    return pressed;
}
bool checkbox_with_popup(const char* popup_id, const char* icon, const std::function<void()>& popup_content, ImVec2 icon_size = ImVec2(15, 15))
{
    auto sizee = ImVec2(200, 250);
    ImGuiWindow* window = ImGui::GetCurrentWindow();
    if (window->SkipItems)
        return false;

    ImGuiIO& io = ImGui::GetIO();


    // Кнопка с иконкой
    bool clicked = ImGui::InvisibleButton(popup_id, icon_size);
    ImVec2 pos = ImGui::GetItemRectMin();
    ImVec2 size = ImGui::GetItemRectSize();
    ImVec2 text_size = ImGui::CalcTextSize(icon);

    ImDrawList* draw = ImGui::GetWindowDrawList();
    draw->AddText(fa_icon16, 10,ImVec2(pos.x + 1, pos.y + 5), ImGui::GetColorU32(ImGuiCol_Text), icon); // s

    ImGui::SameLine();
    ImGui::Text("%s", popup_id);

    if (clicked)
        ImGui::OpenPopup(popup_id);

    bool open = false;

    ImGui::SetNextWindowSize(sizee, ImGuiCond_Always); // фиксированный размер
    if (ImGui::BeginPopup(popup_id, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse))
    {
        open = true;

        ImVec2 win_pos = ImGui::GetWindowPos();
        ImVec2 win_size = ImGui::GetWindowSize();
        ImDrawList* draw_list = ImGui::GetBackgroundDrawList();

        // Мягкое внешнее свечение
        float glow_thickness = 20.0f; // больше = мягче
        float max_alpha = 0.20f; // прозрачность свечения
        ImColor temp = gui.accent_color.to_im_color();
        ImColor glow_color = ImColor(temp.Value.x, temp.Value.y, temp.Value.z, max_alpha);

        float rounding = 0.0f; // скругление углов

        for (int i = 0; i < glow_thickness; ++i)
        {
            float t = (float)i / glow_thickness;
            float alpha = max_alpha * (1.0f - t); // затухание к краю
            ImColor col = ImColor(glow_color.Value.x, glow_color.Value.y, glow_color.Value.z, alpha);

            draw_list->AddRect(
                ImVec2(win_pos.x - i, win_pos.y - i),
                ImVec2(win_pos.x + win_size.x + i, win_pos.y + win_size.y + i),
                col,
                rounding + i, // скругление увеличивается
                0,
                1.0f
            );
        }

        ImGui::SetCursorPos(ImVec2(0, 0));
        gui.group_box(popup_id, sizee);
        {
            popup_content();
        }
        gui.end_group_box();
        ImGui::EndPopup();
    }

    return open;
}

bool checkbox_with_color_picker(const char* label, bool* v, ImVec4* color, ImGuiColorEditFlags flags)
{
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    const ImGuiID id = window->GetID(label);
    const ImVec2 label_size = CalcTextSize(label, NULL, true);
    const float square_sz = 10.f;
    const ImVec2 pos = window->DC.CursorPos;

    const ImVec2 total_size = ImVec2(
        square_sz + (label_size.x > 0.0f ? style.ItemInnerSpacing.x + label_size.x : 0.0f),
        square_sz - 5
    );

    const ImRect total_bb(ImVec2(pos.x, pos.y + 7), ImVec2(pos.x + total_size.x, pos.y + total_size.y + 11));
    const ImRect total_bbs(ImVec2(pos.x, pos.y + 7), ImVec2(pos.x + total_size.x, pos.y + total_size.y + 14));

    ItemSize(total_bb, 0);
    if (!ItemAdd(total_bb, id))
        return false;

    bool hovered, held;
    bool pressed = ButtonBehavior(total_bbs, id, &hovered, &held);
    if (pressed)
    {
        *v = !*v;
        MarkItemEdited(id);
    }

    ImVec2 check_bb_min = ImVec2(pos.x, pos.y + 8);
    ImVec2 check_bb_max = ImVec2(check_bb_min.x + square_sz, check_bb_min.y + square_sz);
    ImRect check_bb(check_bb_min, check_bb_max);

    window->DrawList->AddRectFilled(check_bb.Min, check_bb.Max, gui.bg.to_im_color(), 0.f);
    window->DrawList->AddRect(check_bb.Min, check_bb.Max, gui.bordercb.to_im_color(), 0.f);

    if (*v)
    {
        ImU32 check_col = ImGui::ColorConvertFloat4ToU32(gui.accent_color.to_im_color());

        float pad = square_sz / 6.0f;
        if (pad < 1.0f) pad = 1.0f;
        window->DrawList->AddRectFilled(check_bb.Min, check_bb.Max, gui.accent_color.to_im_color(), 0.f);

        ImVec2 checkmark_pos = ImVec2(check_bb.Min.x + pad, check_bb.Min.y + pad);
        RenderCheckMark(window->DrawList, checkmark_pos, check_col, square_sz - pad * 2.0f);
    }

    if (label_size.x > 0.0f)
    {
        ImVec2 text_pos = ImVec2(check_bb.Max.x + style.ItemInnerSpacing.x, pos.y + style.FramePadding.y);
        RenderText(text_pos, label);
    }
    SetCursorPosX(ImGui::GetContentRegionAvail().x - 20);
    ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 11);  // Сдвигаем по Y на 20 пикселей вверх

    // Сохраняем старое значение
    ImGuiStyle& styles = ImGui::GetStyle();
    float old_rounding = styles.FrameRounding;

    // Устанавливаем новое округление только для этого элемента
    styles.FrameRounding = 50.f;

    // Рисуем ColorEdit4
    ImVec2 mask_size = ImVec2(20, 10);
    ImVec2 cursor_pos = ImGui::GetCursorScreenPos();
    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    draw_list->AddRectFilled(cursor_pos, ImVec2(cursor_pos.x + mask_size.x, cursor_pos.y + mask_size.y),
        ImGui::ColorConvertFloat4ToU32(*color));

    std::string mask_id = std::string("##color_mask_") + label;
    std::string popup_id = std::string("##color_popup_") + label;
    std::string picker_id = std::string("##picker_") + label;

    if (ImGui::InvisibleButton(mask_id.c_str(), mask_size)) {
        ImGui::OpenPopup(popup_id.c_str());
    }


    if (ImGui::BeginPopup(popup_id.c_str())) {
        ImGui::ColorPicker4(picker_id.c_str(), (float*)color,
            ImGuiColorEditFlags_NoSidePreview |
            ImGuiColorEditFlags_NoSmallPreview |
            ImGuiColorEditFlags_NoInputs |
            ImGuiColorEditFlags_AlphaBar);



        ImGui::EndPopup();
    }
    // Возвращаем старое значение
    styles.FrameRounding = old_rounding;
    ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 9); // Поднимаем курсор вверх на 3 пикселя

    return pressed;
}
bool checkbox_with_dual_color_picker(const char* label, bool* v, ImVec4* color2, ImVec4* color1, ImGuiColorEditFlags flags)
{
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    const ImGuiID id = window->GetID(label);
    const ImVec2 label_size = CalcTextSize(label, NULL, true);
    const float square_sz = 10.f;
    const ImVec2 pos = window->DC.CursorPos;

    const ImVec2 total_size = ImVec2(
        square_sz + (label_size.x > 0.0f ? style.ItemInnerSpacing.x + label_size.x : 0.0f),
        square_sz - 5
    );

    const ImRect total_bb(ImVec2(pos.x, pos.y + 7), ImVec2(pos.x + total_size.x, pos.y + total_size.y + 11));
    const ImRect total_bbs(ImVec2(pos.x, pos.y + 7), ImVec2(pos.x + total_size.x, pos.y + total_size.y + 14));

    ItemSize(total_bb, 0);
    if (!ItemAdd(total_bb, id))
        return false;

    bool hovered, held;
    bool pressed = ButtonBehavior(total_bbs, id, &hovered, &held);
    if (pressed)
    {
        *v = !*v;
        MarkItemEdited(id);
    }

    ImVec2 check_bb_min = ImVec2(pos.x, pos.y + 8);
    ImVec2 check_bb_max = ImVec2(check_bb_min.x + square_sz, check_bb_min.y + square_sz);
    ImRect check_bb(check_bb_min, check_bb_max);

    window->DrawList->AddRectFilled(check_bb.Min, check_bb.Max, gui.bg.to_im_color(), 0.f);
    window->DrawList->AddRect(check_bb.Min, check_bb.Max, gui.bordercb.to_im_color(), 0.f);

    if (*v)
    {
        ImU32 check_col = ImGui::ColorConvertFloat4ToU32(gui.accent_color.to_im_color());

        float pad = square_sz / 6.0f;
        if (pad < 1.0f) pad = 1.0f;
        window->DrawList->AddRectFilled(check_bb.Min, check_bb.Max, gui.accent_color.to_im_color(), 0.f);

        ImVec2 checkmark_pos = ImVec2(check_bb.Min.x + pad, check_bb.Min.y + pad);
        RenderCheckMark(window->DrawList, checkmark_pos, check_col, square_sz - pad * 2.0f);
    }

    if (label_size.x > 0.0f)
    {
        ImVec2 text_pos = ImVec2(check_bb.Max.x + style.ItemInnerSpacing.x, pos.y + style.FramePadding.y);
        RenderText(text_pos, label);
    }

    SetCursorPosX(ImGui::GetContentRegionAvail().x - 45); // смещаем на 45px влево для 2 масок
    ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 11);

    ImGuiStyle& styles = ImGui::GetStyle();
    float old_rounding = styles.FrameRounding;
    styles.FrameRounding = 50.f;

    ImVec2 mask_size = ImVec2(20, 10);
    ImDrawList* draw_list = ImGui::GetWindowDrawList();

    // Первая маска
    ImVec2 cursor_pos = ImGui::GetCursorScreenPos();
    draw_list->AddRectFilled(cursor_pos, ImVec2(cursor_pos.x + mask_size.x, cursor_pos.y + mask_size.y),
        ImGui::ColorConvertFloat4ToU32(*color1));

    std::string mask_id1 = std::string("##color_mask1_") + label;
    std::string popup_id1 = std::string("##color_popup1_") + label;
    std::string picker_id1 = std::string("##picker1_") + label;

    if (ImGui::InvisibleButton(mask_id1.c_str(), mask_size)) {
        ImGui::OpenPopup(popup_id1.c_str());
    }

    ImGui::SameLine(0, 5.f); // Отступ между цветами

    // Вторая маска
    cursor_pos = ImGui::GetCursorScreenPos();
    draw_list->AddRectFilled(cursor_pos, ImVec2(cursor_pos.x + mask_size.x, cursor_pos.y + mask_size.y),
        ImGui::ColorConvertFloat4ToU32(*color2));

    std::string mask_id2 = std::string("##color_mask2_") + label;
    std::string popup_id2 = std::string("##color_popup2_") + label;
    std::string picker_id2 = std::string("##picker2_") + label;

    if (ImGui::InvisibleButton(mask_id2.c_str(), mask_size)) {
        ImGui::OpenPopup(popup_id2.c_str());
    }

    // Попапы
    if (ImGui::BeginPopup(popup_id1.c_str())) {
        ImGui::ColorPicker4(picker_id1.c_str(), (float*)color1,
            ImGuiColorEditFlags_NoSidePreview |
            ImGuiColorEditFlags_NoSmallPreview |
            ImGuiColorEditFlags_NoInputs |
            ImGuiColorEditFlags_AlphaBar);
        ImGui::EndPopup();
    }

    if (ImGui::BeginPopup(popup_id2.c_str())) {
        ImGui::ColorPicker4(picker_id2.c_str(), (float*)color2,
            ImGuiColorEditFlags_NoSidePreview |
            ImGuiColorEditFlags_NoSmallPreview |
            ImGuiColorEditFlags_NoInputs |
            ImGuiColorEditFlags_AlphaBar);
        ImGui::EndPopup();
    }

    styles.FrameRounding = old_rounding;
    ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 9);

    return pressed;
}

bool color_picker(const char* label, ImVec4* color, ImGuiColorEditFlags flags)
{
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    const ImGuiID id = window->GetID(label);
    const ImVec2 label_size = CalcTextSize(label, NULL, true);
    const float square_sz = 10.f;
    const ImVec2 pos = window->DC.CursorPos;

    const ImVec2 total_size = ImVec2(
        square_sz + (label_size.x > 0.0f ? style.ItemInnerSpacing.x + label_size.x : 0.0f),
        square_sz - 5
    );

    const ImRect total_bb(ImVec2(pos.x, pos.y + 7), ImVec2(pos.x + total_size.x, pos.y + total_size.y + 11));
    const ImRect total_bbs(ImVec2(pos.x, pos.y + 7), ImVec2(pos.x + total_size.x, pos.y + total_size.y + 14));

    ItemSize(total_bb, 0);
    if (!ItemAdd(total_bb, id))
        return false;

    bool hovered, held;
    bool pressed = ButtonBehavior(total_bbs, id, &hovered, &held);
    if (pressed)
    {
        //*v = !*v;
        MarkItemEdited(id);
    }

    ImVec2 check_bb_min = ImVec2(pos.x, pos.y + 8);
    ImVec2 check_bb_max = ImVec2(check_bb_min.x + square_sz, check_bb_min.y + square_sz);
    ImRect check_bb(check_bb_min, check_bb_max);

    

    if (label_size.x > 0.0f)
    {
        ImVec2 text_pos = ImVec2(check_bb.Max.x + style.ItemInnerSpacing.x, pos.y + style.FramePadding.y);
        RenderText(text_pos, label);
    }
    SetCursorPosX(ImGui::GetContentRegionAvail().x - 20);
    ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 11);  // Сдвигаем по Y на 20 пикселей вверх

    // Сохраняем старое значение
    ImGuiStyle& styles = ImGui::GetStyle();
    float old_rounding = styles.FrameRounding;

    // Устанавливаем новое округление только для этого элемента
    styles.FrameRounding = 50.f;

    // Рисуем ColorEdit4
    ImVec2 mask_size = ImVec2(20, 10);
    ImVec2 cursor_pos = ImGui::GetCursorScreenPos();
    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    draw_list->AddRectFilled(cursor_pos, ImVec2(cursor_pos.x + mask_size.x, cursor_pos.y + mask_size.y),
        ImGui::ColorConvertFloat4ToU32(*color));

    std::string mask_id = std::string("##color_mask_") + label;
    std::string popup_id = std::string("##color_popup_") + label;
    std::string picker_id = std::string("##picker_") + label;

    if (ImGui::InvisibleButton(mask_id.c_str(), mask_size)) {
        ImGui::OpenPopup(popup_id.c_str());
    }


    if (ImGui::BeginPopup(popup_id.c_str())) {
        ImGui::ColorPicker4(picker_id.c_str(), (float*)color,
            ImGuiColorEditFlags_NoSidePreview |
            ImGuiColorEditFlags_NoSmallPreview |
            ImGuiColorEditFlags_NoInputs |
            ImGuiColorEditFlags_AlphaBar);



        ImGui::EndPopup();
    }
    // Возвращаем старое значение
    styles.FrameRounding = old_rounding;
    ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 9); // Поднимаем курсор вверх на 3 пикселя

    return pressed;
}
bool dual_color_picker(const char* label, ImVec4* color2, ImVec4* color1, ImGuiColorEditFlags flags)
{
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    const ImGuiID id = window->GetID(label);
    const ImVec2 label_size = CalcTextSize(label, NULL, true);
    const float square_sz = 10.f;
    const ImVec2 pos = window->DC.CursorPos;

    const ImVec2 total_size = ImVec2(
        (square_sz * 2) + 5.f + (label_size.x > 0.0f ? style.ItemInnerSpacing.x + label_size.x : 0.0f),
        square_sz - 5
    );

    const ImRect total_bb(ImVec2(pos.x, pos.y + 7), ImVec2(pos.x + total_size.x, pos.y + total_size.y + 11));
    ItemSize(total_bb, 0);
    if (!ItemAdd(total_bb, id))
        return false;

    ImGuiStyle& styles = ImGui::GetStyle();
    float old_rounding = styles.FrameRounding;
    styles.FrameRounding = 50.f;

    // Размер и отрисовка прямоугольников цветов
    ImVec2 mask_size = ImVec2(20, 10);
    ImVec2 cursor_pos = ImGui::GetCursorScreenPos();
    ImDrawList* draw_list = ImGui::GetWindowDrawList();

    // Первая кнопка цвета
    draw_list->AddRectFilled(cursor_pos, ImVec2(cursor_pos.x + mask_size.x, cursor_pos.y + mask_size.y),
        ImGui::ColorConvertFloat4ToU32(*color1));

    std::string mask_id1 = std::string("##color_mask1_") + label;
    std::string popup_id1 = std::string("##color_popup1_") + label;
    std::string picker_id1 = std::string("##picker1_") + label;

    bool changed = false;

    if (ImGui::InvisibleButton(mask_id1.c_str(), mask_size)) {
        ImGui::OpenPopup(popup_id1.c_str());
    }

    ImGui::SameLine(0, 5.f); // Отступ между двумя цветами

    // Вторая кнопка цвета
    cursor_pos = ImGui::GetCursorScreenPos();
    draw_list->AddRectFilled(cursor_pos, ImVec2(cursor_pos.x + mask_size.x, cursor_pos.y + mask_size.y),
        ImGui::ColorConvertFloat4ToU32(*color2));

    std::string mask_id2 = std::string("##color_mask2_") + label;
    std::string popup_id2 = std::string("##color_popup2_") + label;
    std::string picker_id2 = std::string("##picker2_") + label;

    if (ImGui::InvisibleButton(mask_id2.c_str(), mask_size)) {
        ImGui::OpenPopup(popup_id2.c_str());
    }

    ImGui::SameLine(0, 5.f);
    ImGui::TextUnformatted(label);

    // Первый попап
    if (ImGui::BeginPopup(popup_id1.c_str())) {
        if (ImGui::ColorPicker4(picker_id1.c_str(), (float*)color1,
            ImGuiColorEditFlags_NoSidePreview |
            ImGuiColorEditFlags_NoSmallPreview |
            ImGuiColorEditFlags_NoInputs |
            ImGuiColorEditFlags_AlphaBar)) {
            changed = true;
        }
        ImGui::EndPopup();
    }

    // Второй попап
    if (ImGui::BeginPopup(popup_id2.c_str())) {
        if (ImGui::ColorPicker4(picker_id2.c_str(), (float*)color2,
            ImGuiColorEditFlags_NoSidePreview |
            ImGuiColorEditFlags_NoSmallPreview |
            ImGuiColorEditFlags_NoInputs |
            ImGuiColorEditFlags_AlphaBar)) {
            changed = true;
        }
        ImGui::EndPopup();
    }

    styles.FrameRounding = old_rounding;

    return changed;
}
