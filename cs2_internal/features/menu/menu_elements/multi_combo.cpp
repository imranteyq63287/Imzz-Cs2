#include "../menu.h"

void c_menu_elements::draw_multicombo(std::string name, int& variable, std::vector<std::string> labels)
{
    std::string label = "None";

    static auto howmuchsel = [](int flags, int count) -> int
        {
            int s = 0;
            for (int i = 0; i < count; i++)
                if (flags & (1 << i))
                    s++;
            return s;
        };

    if (howmuchsel(variable, (int)labels.size()))
    {
        label = "";
        for (int i = 0; i < labels.size(); i++)
            if (variable & (1 << i))
                label += labels[i] + ", ";

        if (!label.empty())
            label.erase(label.size() - 2);
    }

    if (ImGui::BeginCombo(name.c_str(), label.c_str(), ImGuiComboFlags_HeightLarge))
    {
        ImGui::BeginGroup();
        {
            for (int i = 0; i < labels.size(); i++)
            {
                bool selected = variable & (1 << i);
                if (ImGui::Selectable(labels[i].c_str(), &selected, ImGuiSelectableFlags_DontClosePopups))
                {
                    if (selected)
                        variable |= (1 << i);
                    else
                        variable &= ~(1 << i);
                }
            }
        }
        ImGui::EndGroup();
        ImGui::EndCombo();
    }
}