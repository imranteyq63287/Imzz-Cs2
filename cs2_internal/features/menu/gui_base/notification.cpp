#include "notification.hpp"
#include "Source.hpp"
#include "../../../dependencies/imgui/imgui_internal.h"
#include "gui.hpp"
#include "ui.h"

struct Notification {
    std::string icon;
    std::string message;
    std::function<std::string()> dynamic_message; // ← функция для обновляемого текста
    float time_created = 0.f;
    float duration = 3.f;
    float alpha = 1.f;
    float y_offset = 0.f;
};


std::vector<Notification> notifications;
std::vector<Notification> notifications_topleft;
void c_notification::AddAnimatedNotification(const char* icon, const std::string& name, const std::string& hitgroup, int damage, int remaining_health) {
    auto notif = Notification{};
    notif.icon = icon;
    notif.time_created = ImGui::GetTime();
    notif.duration = 3.f;

    auto animated_dmg = std::make_shared<std::atomic<int>>(0);
    auto animated_health = std::make_shared<std::atomic<int>>(0);


    std::thread([animated_dmg, damage]() {
        for (int i = 0; i <= damage; ++i) {
            animated_dmg->store(i);
            std::this_thread::sleep_for(std::chrono::milliseconds(25));
        }
        }).detach();
    std::thread([animated_health, remaining_health]() {
        for (int i = 0; i <= remaining_health; ++i) {
            animated_health->store(i);
            std::this_thread::sleep_for(std::chrono::milliseconds(25));
        }
        }).detach();

    notif.dynamic_message = [animated_dmg, name, hitgroup, animated_health]() -> std::string {
        return tfm::format("Hit %s in the %s for %d damage (%d remaining)",
            name, hitgroup, animated_dmg->load(), animated_health->load());
        };

    notifications.insert(notifications.begin(), notif);
}
void c_notification::AddNotification(const char* icon, const std::string& msg) {
    Notification n;
    n.message = msg;
    n.icon = icon;
    n.time_created = ImGui::GetTime();
    notifications.insert(notifications.begin(), n);
}
void c_notification::AddNotificationTopLeft(const char* icon, const std::string& msg) {
    Notification n;
    n.message = msg;
    n.icon = icon;
    n.time_created = ImGui::GetTime();
    notifications_topleft.insert(notifications_topleft.begin(), n);
}
static int iconsize = 8;

void c_notification::RenderNotifications() {
    const float current_time = ImGui::GetTime();

    ImVec2 display_size = ImGui::GetIO().DisplaySize;
    float base_y = display_size.y / 2.f + 270.f;
    float center_x = display_size.x / 2.f;

    float spacing = c_notification::spacing;
    float total_height = 0.0f;

    for (int i = 0; i < notifications.size();) {
        Notification& n = notifications[i];
        float elapsed = current_time - n.time_created;

        if (elapsed > n.duration) {
            notifications.erase(notifications.begin() + i);
            continue;
        }

        if (elapsed < 0.2f)
            n.alpha = elapsed / 0.2f;
        else if (elapsed > n.duration - 0.5f)
            n.alpha = 1.0f - ((elapsed - (n.duration - 0.5f)) / 0.5f);
        else
            n.alpha = 1.0f;

        float height = 30.f;
        float icon_spacing = 8.f;

        ImVec2 icon_size = fa_icon16->CalcTextSizeA(iconsize, FLT_MAX, 0.0f, n.icon.c_str());
        std::string displayed_msg = n.dynamic_message ? n.dynamic_message() : n.message;
        ImVec2 text_size = ImGui::CalcTextSize(displayed_msg.c_str());

        float width = icon_size.x + icon_spacing + text_size.x + 40.0f;

        float target_y = base_y + total_height;
        n.y_offset = ImLerp(n.y_offset, target_y, 0.1f);
        total_height += height + spacing;

        ImVec2 pos = ImVec2(center_x - width / 2.f, n.y_offset);

        ImDrawList* draw = ImGui::GetBackgroundDrawList();
        ImU32 bg_color = IM_COL32(20, 20, 20, (int)(n.alpha * 255));
        ImU32 text_color = IM_COL32(255, 255, 255, (int)(n.alpha * 255));
        ImVec4 accent = gui.accent_color.to_vec4(); // допустим это ImVec4 (0.0–1.0)
        ImU32 icon_color = IM_COL32(
            static_cast<int>(accent.x * 255),
            static_cast<int>(accent.y * 255),
            static_cast<int>(accent.z * 255),
            static_cast<int>(n.alpha * 255)
        );
        //draw->AddRectFilled(
        //    pos,
        //    ImVec2(pos.x + width, pos.y + height),
        //    bg_color,
        //    15.f
        //);
        ImVec2 bg_pos = ImVec2(pos.x,pos.y + 3);
        ImVec2 bg_size = ImVec2(width,height - 6);

        int alpha = static_cast<int>(n.alpha * 255);
        alpha = std::min(alpha, config.gui.menu_opacity);

        // Градиентный фон
        DrawGradientRect(bg_pos, bg_size, IM_COL32(0, 0, 0, 0), IM_COL32(0, 0, 0, alpha));    
        draw->AddText(fa_icon16, iconsize,
            ImVec2(pos.x + 20, pos.y + 14),
            icon_color,
            n.icon.c_str()
        );

        draw->AddText(
            ImVec2(pos.x + 20 + icon_size.x + icon_spacing, pos.y + 8),
            text_color,
            displayed_msg.c_str()
        );

        ++i;
    }

}
void c_notification::RenderTopLeftNotifications() {
    const float current_time = ImGui::GetTime();

    float start_x = 10.f;
    float base_y = 15.f;

    float spacing = c_notification::spacing;
    float total_height = 0.0f;

    for (int i = 0; i < notifications_topleft.size();) {
        Notification& n = notifications_topleft[i];
        float elapsed = current_time - n.time_created;

        if (elapsed > n.duration) {
            notifications_topleft.erase(notifications_topleft.begin() + i);
            continue;
        }

        if (elapsed < 0.2f)
            n.alpha = elapsed / 0.2f;
        else if (elapsed > n.duration - 0.5f)
            n.alpha = 1.0f - ((elapsed - (n.duration - 0.5f)) / 0.5f);
        else
            n.alpha = 1.0f;

        float height = 30.f;
        float icon_spacing = 8.f;

        ImVec2 icon_size = fa_icon16->CalcTextSizeA(iconsize, FLT_MAX, 0.0f, n.icon.c_str());
        ImVec2 text_size = ImGui::CalcTextSize(n.message.c_str());
        float width = icon_size.x + icon_spacing + text_size.x + 40.0f;

        float target_y = base_y + total_height;
        n.y_offset = ImLerp(n.y_offset, target_y, 0.1f);
        total_height += height + spacing;

        ImVec2 pos = ImVec2(start_x, n.y_offset);

        ImDrawList* draw = ImGui::GetBackgroundDrawList();
        ImU32 bg_color = IM_COL32(20, 20, 20, (int)(n.alpha * 255));
        ImU32 text_color = IM_COL32(255, 255, 255, (int)(n.alpha * 255));
        ImVec4 accent = gui.accent_color.to_vec4();
        ImU32 icon_color = IM_COL32(
            static_cast<int>(accent.x * 255),
            static_cast<int>(accent.y * 255),
            static_cast<int>(accent.z * 255),
            static_cast<int>(n.alpha * 255)
        );

        //draw->AddRectFilled(pos, ImVec2(pos.x + width, pos.y + height), bg_color, 15.f);
        ImVec2 bg_pos = ImVec2(pos.x, pos.y + 3);
        ImVec2 bg_size = ImVec2(width, height - 6);

        int alpha = static_cast<int>(n.alpha * 255);
        alpha = std::min(alpha, config.gui.menu_opacity);

        // Градиентный фон
        DrawGradientRect(bg_pos, bg_size, IM_COL32(0, 0, 0, 0), IM_COL32(0, 0, 0, alpha));
        draw->AddText(fa_icon16, iconsize, ImVec2(pos.x + 20, pos.y + 14), icon_color, n.icon.c_str());
        draw->AddText(ImVec2(pos.x + 20 + icon_size.x + icon_spacing, pos.y + 8), text_color, n.message.c_str());

        ++i;
    }
}