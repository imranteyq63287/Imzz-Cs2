#include "../menu.h"
#include "../../../dependencies/imgui/imgui_internal.h"

using namespace ImGui;

struct tab_struct
{
    float size;
    bool active;
};

bool c_menu_elements::tab(const char* label, const ImVec2& size_arg, const bool selected)
{
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    const ImGuiID id = window->GetID(label);
    const ImVec2 label_size = CalcTextSize(label, NULL, true);
    ImVec2 pos = window->DC.CursorPos;

    ImVec2 size = CalcItemSize(ImVec2(size_arg.x - 20.f, size_arg.y), label_size.x + style.FramePadding.x * 2.0f, label_size.y + style.FramePadding.y * 2.0f);
    pos.x += 10.f;

    const ImRect bb(pos, ImVec2(pos.x + size.x, pos.y + size.y));
    ItemSize(ImVec2(size.x + 10, size.y), style.FramePadding.y);
    if (!ItemAdd(bb, id))
        return false;

    bool hovered, held;
    bool pressed = ButtonBehavior(bb, id, &hovered, &held, 0);

    static std::map<ImGuiID, tab_struct> selected_animation;
    auto it_selected = selected_animation.find(GetItemID());

    if (it_selected == selected_animation.end())
    {
        selected_animation.insert({ GetItemID(), { 0.0f, false } });
        it_selected = selected_animation.find(GetItemID());
    }

    it_selected->second.size = ImClamp(it_selected->second.size + (5.f * GetIO().DeltaTime * (selected || hovered ? 1.f : -1.f)), 0.f, 1.f);

    ImU32 color_rect = GetColorU32(ImLerp(ImVec4(27 / 255.f, 28 / 255.f, 33 / 255.f, 1.f), ImVec4(30 / 255.f, 31 / 255.f, 36 / 255.f, 1.f), it_selected->second.size));
    ImU32 color_text = GetColorU32(ImLerp(ImVec4(125 / 255.f, 125 / 255.f, 132 / 255.f, 1.f), ImVec4(224 / 255.f, 224 / 255.f, 224 / 255.f, 1.f), it_selected->second.size));

    window->DrawList->AddRectFilled(ImVec2(bb.Min.x, bb.Min.y + size_arg.y / 3), ImVec2(bb.Max.x, bb.Max.y - size_arg.y / 3), color_rect, 4.f);

    window->DrawList->AddText(ImVec2(bb.Min.x + size.x / 2 - CalcTextSize(label).x / 2, bb.Min.y + size.y / 2 - CalcTextSize(label).y / 2 - 1), color_text, label);

    return pressed;
}