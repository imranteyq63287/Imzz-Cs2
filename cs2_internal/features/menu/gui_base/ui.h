#pragma once

#include <unordered_map>
#include <string>
#include "hashes.hpp"
#include "notification.hpp"
#include "source.hpp"
#include "gui.hpp"
#include "../../../auth/auth.h"
#include "../../../sdk/classes//c_cs_gamerules.h"
#include "../../visuals/other/event.hpp"
#include "../../../sdk/utils/render/render.h"
#include "../../config_system.h"
#include "../../../Bind System/key.h"
#include "../../visuals/main/overlay.h"
static bool dt = false;
static bool hs = false;
static bool ping = false;
static bool ns = false;
static bool bombindi = false;

inline void DrawGradientRect(ImVec2 pos, ImVec2 size, ImU32 color, ImU32 gradient_color, ImDrawList* draw_list = ImGui::GetBackgroundDrawList()) {

    ImVec2 p1 = pos;
    ImVec2 p2 = ImVec2(pos.x + size.x, pos.y + size.y);

    draw_list->AddRectFilled(ImVec2(p1.x + 20, p1.y), ImVec2(p2.x - 20, p2.y), gradient_color);

    draw_list->AddRectFilledMultiColor(ImVec2(p1.x + 20, p2.y), p1, gradient_color, IM_COL32(0, 0, 0, 0), IM_COL32(0, 0, 0, 0), gradient_color);
    draw_list->AddRectFilledMultiColor(p2, ImVec2(p2.x - 20, p1.y), IM_COL32(0, 0, 0, 0), gradient_color, gradient_color, IM_COL32(0, 0, 0, 0));

}
static ULARGE_INTEGER last_idle_time, last_kernel_time, last_user_time;


inline int GetCPUUsage()
{
    static ULARGE_INTEGER last_idle_time = {}, last_kernel_time = {}, last_user_time = {};
    static auto last_update_time = std::chrono::steady_clock::now();
    static float cached_cpu_usage = 0.0f;

    auto now = std::chrono::steady_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - last_update_time);

    if (elapsed.count() < 1000) {
        return static_cast<int>(cached_cpu_usage + 0.5f);
    }

    last_update_time = now;

    FILETIME idle_time, kernel_time, user_time;
    if (!GetSystemTimes(&idle_time, &kernel_time, &user_time))
        return static_cast<int>(cached_cpu_usage + 0.5f);

    ULARGE_INTEGER idle, kernel, user;
    idle.LowPart = idle_time.dwLowDateTime;
    idle.HighPart = idle_time.dwHighDateTime;
    kernel.LowPart = kernel_time.dwLowDateTime;
    kernel.HighPart = kernel_time.dwHighDateTime;
    user.LowPart = user_time.dwLowDateTime;
    user.HighPart = user_time.dwHighDateTime;

    ULONGLONG sys_idle = idle.QuadPart - last_idle_time.QuadPart;
    ULONGLONG sys_kernel = kernel.QuadPart - last_kernel_time.QuadPart;
    ULONGLONG sys_user = user.QuadPart - last_user_time.QuadPart;
    ULONGLONG sys_total = sys_kernel + sys_user;

    last_idle_time = idle;
    last_kernel_time = kernel;
    last_user_time = user;

    if (sys_total == 0)
        return static_cast<int>(cached_cpu_usage + 0.5f);

    cached_cpu_usage = (float)(100.0 - ((sys_idle * 100.0) / sys_total));
    return static_cast<int>(cached_cpu_usage + 0.5f);
}


inline int velocity()
{
    int speed = 0;
    if (interfaces::engine_client && interfaces::engine_client->is_in_game())
    {
        auto local = g_ctx.local;
        if (!local) return 0; //
            //if (local->team_num() == 2 || local->team_num() == 3)

            {
                vec3_t velocityxyz = local->velocity();
                speed = sqrt(velocityxyz.x * velocityxyz.x + velocityxyz.y * velocityxyz.y + velocityxyz.z * velocityxyz.z);
            }
        
    }
    else
    {
        speed = 0;
    }
    return speed;
}
inline int pingin()
{
    int ping = 0; 
    if (interfaces::engine_client && interfaces::engine_client->is_in_game())
    {
        auto local = g_ctx.local;
        if (!local) return 0;
        c_cs_player_controller* player_controller = g_ctx.local_controller;

            if (player_controller)
            {
                ping = player_controller->ping();
            }

        
    }
    else
    {
        ping = 0;
    }
    return ping;
}
inline void DrawRainbowLine(ImDrawList* draw_list, ImVec2 start, ImVec2 end, float thickness = 2.0f, int segments = 400) {
    float time = ImGui::GetTime();

    ImVec2 direction = ImVec2(end.x - start.x, end.y - start.y);

    ImVec2 step = ImVec2(direction.x / segments, direction.y / segments);

    for (int i = 0; i < segments; ++i) {
        float t = (float)i / segments;
        float hue = fmodf(t + time * 0.1f, 1.0f);

        ImColor color = ImColor::HSV(hue, 1.0f, 1.0f, 0.5f);

        ImVec2 p1 = ImVec2(start.x + step.x * i, start.y + step.y * i);
        ImVec2 p2 = ImVec2(start.x + step.x * (i + 1), start.y + step.y * (i + 1));

        draw_list->AddLine(p1, p2, color, thickness);
    }
}

inline void watermark()
{
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);
    ImGui::Begin("##watermark", nullptr,
        ImGuiWindowFlags_NoTitleBar |
        ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoScrollbar |
        ImGuiWindowFlags_NoScrollWithMouse |
        ImGuiWindowFlags_NoBackground |
        ImGuiWindowFlags_NoBringToFrontOnFocus);

    const char* text1 = g_ctx.cheat_name1.c_str();
    const char* text2 = g_ctx.cheat_name2.c_str();
    static float last_fps = 0.0f;
    static float time_accumulator = 0.0f;

    // Прибавляем дельту времени каждого кадра
    time_accumulator += ImGui::GetIO().DeltaTime;

    if (time_accumulator >= 1.0f) // Прошла 1 секунда
    {
        last_fps = ImGui::GetIO().Framerate;
        time_accumulator = 0.0f;
    }
    auto now = std::chrono::system_clock::now();
    static bool ingame;
    if (interfaces::engine_client && interfaces::engine_client->is_in_game())
    {
        ingame = true;
    }
    else
    {
        ingame = false;
    }
    std::time_t now_c = std::chrono::system_clock::to_time_t(now);
    char time_str[64];
    strftime(time_str, sizeof(time_str), "%H:%M:%S", std::localtime(&now_c));
    std::string suffix = "  " +
        std::to_string((int)last_fps) + " fps  " +
        std::to_string((int)GetCPUUsage()) + "% cpu  ";

    if (ingame)
    {
        suffix += std::to_string((int)pingin()) + " ms  ";
        suffix += std::to_string((int)velocity()) + " speed  ";
    }
    suffix += std::string(time_str);



    ImVec2 text1_size = ImGui::CalcTextSize(text1);
    ImVec2 text2_size = ImGui::CalcTextSize(text2);
    ImVec2 user_size = ImGui::CalcTextSize(g_authorization->username.c_str());
    ImVec2 suffix_size = ImGui::CalcTextSize(suffix.c_str());

    // Общая ширина текста
    float total_width = text1_size.x + text2_size.x + user_size.x + suffix_size.x + 10;
    float total_height = text1_size.y;

    float pos_x = ImGui::GetIO().DisplaySize.x - total_width - 15;
    float pos_y = 15;

    ImVec2 bg_pos = ImVec2(pos_x - 5, pos_y - 2.5);
    ImVec2 bg_size = ImVec2(total_width + 10, total_height + 6);

    auto draw_list = ImGui::GetBackgroundDrawList();

    // Градиентный фон
    DrawGradientRect(bg_pos, bg_size, IM_COL32(0, 0, 0, 0), IM_COL32(0, 0, 0, config.gui.menu_opacity));

    // Отрисовка текста по частям
    float current_x = pos_x;

    draw_list->AddText(ImVec2(current_x, pos_y - 1), IM_COL32(255, 255, 255, 255), text1); 
    current_x += text1_size.x;

    draw_list->AddText(ImVec2(current_x - 0.7, pos_y - 1), gui.accent_color.to_im_color(), text2); 
    current_x += text2_size.x;

    draw_list->AddText(ImVec2(current_x + 6, pos_y - 1), IM_COL32(255, 255, 255, 255), g_authorization->username.c_str()); // ник
    current_x += user_size.x + 3;

    draw_list->AddText(ImVec2(current_x + 3, pos_y - 1), IM_COL32(255, 255, 255, 255), suffix.c_str()); // "sosal"

    ImGui::End();
}
static float bombtimer = 35.2f;
static bool hcindicator = true;
static bool mdmgindicator = true;
inline
ImVec2 GetMaxTextSizeOfActiveHotkeys(const char* const* names, const bool* active, int count) {
    ImVec2 max_size = ImVec2(0, 0);

    for (int i = 0; i < count; ++i) {
        if (!active[i])
            continue;

        ImVec2 size = ImGui::CalcTextSize(names[i]);

        if (size.x > max_size.x)
            max_size.x = size.x;

        if (size.y > max_size.y)
            max_size.y = size.y;
    }

    return max_size;
}

inline void hotkeysdraw() {
    ImGui::SetNextWindowSize(ImVec2(200, 0));
    ImGui::Begin("Hotkeys", nullptr,
        ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoCollapse |
        ImGuiWindowFlags_NoBackground |
        ImGuiWindowFlags_NoDecoration);
    auto draw = ImGui::GetWindowDrawList();
    auto wight = 0;
    const char* title = "Hotkeylist";
    ImVec2 title_size = ImGui::CalcTextSize(title);
    wight = 30;
    ImVec2 title_pos = ImGui::GetCursorScreenPos();
    float padding_x = 7.0f;
    float padding_y = 2.0f;


    const char* hotkey_names[] = {
        "Trigger bot",
        "Aim assist",
        "Hitchance override",
        "Minimum damage override",
        "Thirdperson"
    };
	static bool trigger = false;
    if (g_key_handler->is_pressed(config.legit.triggerbot_keybind, config.legit.triggerbot_keybind_style)) trigger = true;
	else trigger = false;
    static bool aimlock = false;
    if (g_key_handler->is_pressed(config.legit.aimlock_keybind, config.legit.aimlock_keybind_style)) aimlock = true;
    else aimlock = false;
    static bool hcovr = false;
    if (g_key_handler->is_pressed(config.ragebot.hit_chance_keybind, config.ragebot.hit_chance_keybind_style)) hcovr = true;
    else hcovr = false;
    static bool mdovr = false;
    if (g_key_handler->is_pressed(config.ragebot.min_damage_keybind, config.ragebot.min_damage_keybind_style)) mdovr = true;
    else mdovr = false;
    static bool tperson = false;
    if (g_key_handler->is_pressed(config.extra.thirdperson_keybind, config.extra.thirdperson_keybind_style)) tperson = true;
    else tperson = false;
    const bool active[] = {
        trigger,   // Double Tap
        aimlock,  // Hide Shots
        hcovr,   // Thirdperson
        mdovr,   // Fake Duck
        tperson   // Auto Peek
    };
    int count = IM_ARRAYSIZE(hotkey_names);
    static float title_x_anim = 0.0f;
    float delta = ImGui::GetIO().DeltaTime;
    wight = GetMaxTextSizeOfActiveHotkeys(hotkey_names, active, count).x;
    float gradient_width = wight + padding_x * 2 + 25;
    float target_x = ((gradient_width - title_size.x) * 0.5f);
    title_x_anim = ImLerp(title_x_anim, target_x, delta * 10.0f);
    DrawGradientRect(
        ImVec2(title_pos.x - padding_x, title_pos.y - padding_y),
        ImVec2(gradient_width, title_size.y + padding_y * 2),
        IM_COL32(0, 0, 0, 0),
        IM_COL32(0, 0, 0, 140),
        draw
    );
    float center_offset_x = ((gradient_width - title_size.x) * 0.5f);
    ImGui::SetCursorScreenPos(ImVec2(title_pos.x + title_x_anim, title_pos.y));

    ImGui::TextUnformatted("Hotkey");
    ImGui::Dummy(ImVec2(0, 0));
    for (int i = 0; i < IM_ARRAYSIZE(hotkey_names); i++) {
        if (!active[i])
            continue;

        ImVec2 text_pos = ImGui::GetCursorScreenPos();
        ImVec2 text_size = ImGui::CalcTextSize(hotkey_names[i]);

        float gradient_height = text_size.y + padding_y * 2;

        DrawGradientRect(
            ImVec2(text_pos.x - padding_x, text_pos.y - padding_y),
            ImVec2(gradient_width, gradient_height),
            IM_COL32(0, 0, 0, 0),
            IM_COL32(0, 0, 0, 140),
            draw
        );

        ImGui::TextUnformatted(hotkey_names[i]);

        // Позиция для "on"
        ImVec2 after_text_pos = ImVec2(
            text_pos.x + wight + 7, // ← отступ от конца max текста
            text_pos.y
        );

        draw->AddText(after_text_pos, gui.accent_color.to_im_color(), "on");

        // Отступ вниз
        ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 2.0f); // Отступ между строками
        ImGui::Dummy(ImVec2(0, 0));
    }


    ImGui::End();
}
inline void indicator()
{
	std::string bomb_site = "";
    if(ESP::bomb_site == 0)
    {
        bomb_site = "A";
    }
    else if(ESP::bomb_site == 1)
    {
        bomb_site = "B";
    }
    ImGuiIO& io = ImGui::GetIO();
    auto draw_list = ImGui::GetBackgroundDrawList(); // Получаем список отрисовки для всего экрана
    std::string explode_timer_str = std::format("{:.1f}", ESP::time_to_explode);
    std::string bombindis = bomb_site + " - " + explode_timer_str + "s\n" + "FATAL";
    const char* texts[] = { "DT", "HS", "BAIM", "PING", "NS", bombindis.c_str(), "DMG", "LEFT", "RIGHT", "BACK"};
    bool active[] = {
    config.gui.indicators[0] && config.ragebot.doubletap,
    config.gui.indicators[1] && config.antiaim.hideshots,
    config.gui.indicators[2] && config.ragebot.baim,
    config.gui.indicators[3] && ping,
    config.gui.indicators[4] && hcindicator,
    config.gui.indicators[6] && ESP::is_planted,
    config.gui.indicators[5] && mdmgindicator,
    config.gui.indicators[7] && config.antiaim.manualleft,
    config.gui.indicators[7] && config.antiaim.manualright,
    config.gui.indicators[7] && config.antiaim.manualback
    };
    const char* icons[] = {
        reinterpret_cast<const char*>(ICON_FA_BATTERY_HALF),
        reinterpret_cast<const char*>(ICON_FA_SKI_JUMP),
        reinterpret_cast<const char*>(ICON_FA_BULLSEYE),
        reinterpret_cast<const char*>(ICON_FA_ENGINE_WARNING),
        reinterpret_cast<const char*>(ICON_FA_CROSSHAIRS),
        reinterpret_cast<const char*>(ICON_FA_BOMB),
        reinterpret_cast<const char*>(ICON_FA_HEART),
        reinterpret_cast<const char*>(ICON_FA_ARROW_LEFT),
        reinterpret_cast<const char*>(ICON_FA_ARROW_RIGHT),
        reinterpret_cast<const char*>(ICON_FA_ARROW_DOWN)




    };

    // Храним позиции и альфа-значение (прозрачность) для анимации
    static std::unordered_map<std::string, float> animated_y;
    static std::unordered_map<std::string, float> alpha_values;

    float start_x = 15;
    float start_y = io.DisplaySize.y / 2.0f + 30;
    float spacing = 10.0f;
    float offset_y = 0.0f;
    float speed = 0.1f;       // Скорость анимации движения
    float fade_speed = 0.08f; // Скорость изменения прозрачности
    int actualping = pingin();
    if (actualping >= 100)
    {
        ping = true;
    }
    if (actualping < 100)
    {
        ping = false;
    }
    if (interfaces::engine_client && interfaces::engine_client->is_in_game())
    {
        bool no_spread = /*interfaces::cvar->get_by_name(("weapon_accuracy_nospread"))->get_bool()*/ false;
		//c_cs_gamerules* gamerules = get_gamerules();
  //      std::cout << "gamerules ptr: " << std::hex << gamerules << std::endl;
  //      std::cout << gamerules->bomb_dropped() << std::endl;
  //      std::cout << *(bool*)((uintptr_t)gamerules + 0x9A4) << std::endl;
        if (no_spread)
        {
            ns = true;
        }
        else
        {
            ns = false;
        }
        auto local = g_ctx.local;
        {
            for (int i = 0; i < 10; i++) {
                float target_y = start_y + offset_y;

                if (animated_y.find(texts[i]) == animated_y.end()) {
                    animated_y[texts[i]] = target_y;
                    alpha_values[texts[i]] = active[i] ? 1.0f : 0.0f;
                }

                animated_y[texts[i]] = animated_y[texts[i]] * (1.0f - speed) + target_y * speed;

                if (active[i]) {
                    alpha_values[texts[i]] += fade_speed;
                    if (alpha_values[texts[i]] > 1.0f) alpha_values[texts[i]] = 1.0f;
                }
                else {
                    alpha_values[texts[i]] -= fade_speed;
                    if (alpha_values[texts[i]] <= 0.0f) {
                        alpha_values.erase(texts[i]);
                        animated_y.erase(texts[i]);
                        continue;
                    }
                }

                int gradient_alpha = static_cast<int>(config.gui.menu_opacity * alpha_values[texts[i]]);
                if (gradient_alpha > config.gui.menu_opacity) gradient_alpha = config.gui.menu_opacity;

                ImVec2 text_size = ssfi_mid->CalcTextSizeA(ssfi_mid->FontSize, FLT_MAX, 0, texts[i]);
				static int target_damage = 0;
                static int target_hc = 0;

                if (target_hc == 0 && !ns && i == 4)
                {
					text_size = ssfi_mid->CalcTextSizeA(ssfi_mid->FontSize, FLT_MAX, 0, "AUTO");
                }
                if (target_hc != 0 && !ns && i == 4)
                {
                    text_size = ssfi_mid->CalcTextSizeA(ssfi_mid->FontSize, FLT_MAX, 0, std::to_string(target_hc).c_str());
                }
                if (target_damage == 0 && i == 6)
                {
                    text_size = ssfi_mid->CalcTextSizeA(ssfi_mid->FontSize, FLT_MAX, 0, "LETHAL");
                }
                else if(target_damage != 0 && i == 6)
                {
                    text_size = ssfi_mid->CalcTextSizeA(ssfi_mid->FontSize, FLT_MAX, 0, std::to_string(target_damage).c_str());
				}

                ImVec2 pos = ImVec2(start_x, animated_y[texts[i]]);
                ImVec2 on_size = ssfi_mid->CalcTextSizeA(ssfi_mid->FontSize, FLT_MAX, 0, "ON");
                ImVec2 on_pos = ImVec2(pos.x + on_size.x, pos.y);
                ImVec2 bg_size = ImVec2(text_size.x + 5 + on_pos.x, text_size.y + 6);

                ImU32 color = IM_COL32(0, 0, 0, 0);
                ImU32 gradient = IM_COL32(0, 0, 0, gradient_alpha);

                DrawGradientRect(ImVec2(pos.x - 5, pos.y - 3), bg_size, color, gradient);
                ImColor accent = gui.accent_color.to_im_color();
                ImU32 text_color = IM_COL32(255, 255, 255, static_cast<int>(255 * alpha_values[texts[i]]));
                ImU32 icon_color = IM_COL32(
                    static_cast<int>(accent.Value.x * 255.0f),
                    static_cast<int>(accent.Value.y * 255.0f),
                    static_cast<int>(accent.Value.z * 255.0f),
                    static_cast<int>(255 * alpha_values[texts[i]])
                );
                ImU32 orange = IM_COL32(255, 180, 0, static_cast<int>(255 * alpha_values[texts[i]]));
                ImU32 red = IM_COL32(255, 0, 0, static_cast<int>(255 * alpha_values[texts[i]]));
                int y = 2;
                // Добавляем "ON" слева от основного текста
                if (i == 3)
                {
                    float time = ImGui::GetTime();
                    float alpha = 0.2f + 0.8f * (0.5f + 0.5f * sinf(time * 5.f)); // = 0.2 .. 1.0

                    ImColor pulsating_color = ImColor(255, 255, 0, static_cast<int>(alpha * 255.f)); // Жёлтый с динамичной прозрачностью

                    draw_list->AddText(fa_icon, fa_icon->FontSize, ImVec2(pos.x + 5, pos.y + 8 + y), pulsating_color, icons[i]);
                    draw_list->AddText(ssfi_mid, ssfi_mid->FontSize, ImVec2(on_pos.x + 6, on_pos.y), pulsating_color, texts[i]);

                }
                else
                {
                    if (i == 5)
                    {
                        draw_list->AddText(ImVec2(pos.x + 5, pos.y + 8 + y), orange, icons[i]);
                        draw_list->AddText(ssfi_mid, ssfi_mid->FontSize, ImVec2(on_pos.x + 6, on_pos.y), text_color, texts[i]);
                        draw_list->AddText(ssfi_mid, ssfi_mid->FontSize, ImVec2(on_pos.x + 6, on_pos.y + 23), red, "FATAL");

                    }
                    else
                    {
                        if (i == 0)
                        {
                            static ImVec4 current_icon_color = ImVec4(1.0f, 0.0f, 0.0f, 1.0f);  
                            static ImVec4 current_text_color = ImVec4(1.0f, 0.0f, 0.0f, 1.0f);
                            static float icon_alpha_ready = 0.0f;
                            static float icon_alpha_broken = 0.0f;

                            float lerp_speed = 6.0f * ImGui::GetIO().DeltaTime;
                            float fade_speed = lerp_speed;

                            if (config.gui.doubletapready)
                            {
                                icon_alpha_ready = ImClamp(icon_alpha_ready + fade_speed, 0.0f, 1.0f);
                                icon_alpha_broken = ImClamp(icon_alpha_broken - fade_speed, 0.0f, 1.0f);

                                ImVec4 target_icon_color = accent;
                                ImVec4 target_text_color = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);

                                current_icon_color = ImLerp(current_icon_color, target_icon_color, lerp_speed);
                                current_text_color = ImLerp(current_text_color, target_text_color, lerp_speed);
                            }
                            else if (config.gui.doubletapbroken)
                            {
                                icon_alpha_ready = ImClamp(icon_alpha_ready - fade_speed, 0.0f, 1.0f);
                                icon_alpha_broken = ImClamp(icon_alpha_broken + fade_speed, 0.0f, 1.0f);

                                ImVec4 target_color = ImVec4(1.0f, 0.0f, 0.0f, 1.0f); 

                                current_icon_color = ImLerp(current_icon_color, target_color, lerp_speed);
                                current_text_color = ImLerp(current_text_color, target_color, lerp_speed);
                            }

                            ImU32 icon_color_ready = ImColor(current_icon_color.x, current_icon_color.y, current_icon_color.z, icon_alpha_ready * alpha_values[texts[i]]);
                            ImU32 icon_color_broken = ImColor(current_icon_color.x, current_icon_color.y, current_icon_color.z, icon_alpha_broken * alpha_values[texts[i]]);
                            ImU32 text_color = ImColor(current_text_color.x, current_text_color.y, current_text_color.z, alpha_values[texts[i]]);

                            if (g_ctx.local->get_active_weapon()->i_clip1() <= 1)
                            {
                                draw_list->AddText(fa_icon, fa_icon->FontSize, ImVec2(pos.x + 5, pos.y + 8 + y),
                                    icon_color_broken, reinterpret_cast<const char*>(ICON_FA_BATTERY_EMPTY));
                            }
                            else
                            {
                                draw_list->AddText(fa_icon, fa_icon->FontSize, ImVec2(pos.x + 5, pos.y + 8 + y),
                                    icon_color_broken, icons[i]);
                            }

                            draw_list->AddText(fa_icon, fa_icon->FontSize, ImVec2(pos.x + 5, pos.y + 8 + y),
                                icon_color_ready, reinterpret_cast<const char*>(ICON_FA_BATTERY_FULL));

                            const char* display_text = config.gui.doubletapready ? "2x" : "1x";
                            draw_list->AddText(ssfi_mid, ssfi_mid->FontSize, ImVec2(on_pos.x + 6, on_pos.y), text_color, display_text);




                        }
                        else
                        {
                            if (i == 4)
                            {
                                draw_list->AddText(fa_icon, fa_icon->FontSize, ImVec2(pos.x + 5, pos.y + 8 + y), icon_color, icons[i]);
                                if (ns)
                                {
                                    draw_list->AddText(ssfi_mid, ssfi_mid->FontSize, ImVec2(on_pos.x + 6, on_pos.y), text_color, texts[i]);
                                }
                                else
                                {
                                    if (config.ragebot.enable)
                                    {
                                        draw_list->AddText(fa_icon, fa_icon->FontSize, ImVec2(pos.x + 5, pos.y + 8 + y), icon_color, icons[i]);
                                        if (g_key_handler->is_pressed(config.ragebot.hit_chance_keybind, config.ragebot.hit_chance_keybind_style)) {
                                            target_hc = config.ragebot.hit_chance_override;
                                            std::cout << "MIN Target hitchance set to: " << target_hc << "\n";
                                        }
                                        else
                                        {
                                            target_hc = config.ragebot.hit_chance;
                                        }
                                        std::string text = target_hc > 0
                                            ? std::to_string(target_hc)
                                            : "AUTO";

                                        draw_list->AddText(ssfi_mid, ssfi_mid->FontSize, ImVec2(on_pos.x + 6, on_pos.y), text_color, text.c_str());
                                    }
                                    else if (config.legit.enable)
                                    {
                                        draw_list->AddText(fa_icon, fa_icon->FontSize, ImVec2(pos.x + 5, pos.y + 8 + y), icon_color, icons[i]);
                                        //if (g_key_handler->is_pressed(config.ragebot.hit_chance_keybind, config.ragebot.hit_chance_keybind_style)) {
                                            //target_hc = config.ragebot.hit_chance_override;
                                            //std::cout << "MIN Target hitchance set to: " << target_hc << "\n";
                                        //}
                                        //else
                                        //{
                                            target_hc = config.ragebot.hit_chance;
                                        //}
                                        std::string text = target_hc > 0
                                            ? std::to_string(target_hc)
                                            : "AUTO";

                                        draw_list->AddText(ssfi_mid, ssfi_mid->FontSize, ImVec2(on_pos.x + 6, on_pos.y), text_color, text.c_str());
                                    }
                                }
                            }
                            else
                            {
                                if (i == 6)
                                {
                                    if (config.ragebot.enable)
                                    {
                                        draw_list->AddText(fa_icon, fa_icon->FontSize, ImVec2(pos.x + 5, pos.y + 8 + y), icon_color, icons[i]);
                                        if (g_key_handler->is_pressed(config.ragebot.min_damage_keybind, config.ragebot.min_damage_keybind_style)) {
                                            target_damage = config.ragebot.min_damage_override;
                                            std::cout << "MIN Target damage set to: " << target_damage << "\n";
                                        }
                                        else
                                        {
                                            target_damage = config.ragebot.min_damage;
                                        }
                                        std::string text = target_damage > 0
                                            ? std::to_string(target_damage)
                                            : "LETHAL";

                                        draw_list->AddText(ssfi_mid, ssfi_mid->FontSize, ImVec2(on_pos.x + 6, on_pos.y), text_color, text.c_str());
                                    }
                                    else if (config.legit.enable)
                                    {
                                        draw_list->AddText(fa_icon, fa_icon->FontSize, ImVec2(pos.x + 5, pos.y + 8 + y), icon_color, icons[i]);
                                        target_damage = config.ragebot.min_damage;
                                        
                                        std::string text = target_damage > 0
                                            ? std::to_string(target_damage)
                                            : "LETHAL";

                                        draw_list->AddText(ssfi_mid, ssfi_mid->FontSize, ImVec2(on_pos.x + 6, on_pos.y), text_color, text.c_str());
                                    }
                                }
                                else
                                {
                                    draw_list->AddText(fa_icon, fa_icon->FontSize, ImVec2(pos.x + 5, pos.y + 8 + y), icon_color, icons[i]);
                                    draw_list->AddText(ssfi_mid, ssfi_mid->FontSize, ImVec2(on_pos.x + 6, on_pos.y), text_color, texts[i]);
                                }
                            }
                        }
                    }
                }

                if (active[i]) {
                    offset_y += text_size.y + spacing;
                }
            }
        }
    }

}


inline void hit_logs() {
    if (!g_ctx.local)
        return;

    auto pBulletServices = g_ctx.local->bullet_services();
    if (!pBulletServices)
        return;

    static int hit = 0;
    //int totalHits = pBulletServices->bullet_count();
    int totalHits = *(int*)((uintptr_t)pBulletServices + 0x40);


    if (totalHits != hit) {
        std::cout << "Hit, total hits: " << totalHits << std::endl;

        char buf[64];
        sprintf_s(buf, "Hit detection, total hits: %d", totalHits);
        g_notification->AddNotification(reinterpret_cast<const char*>(ICON_FA_CROSSHAIRS), buf);
        hit = totalHits;
    }
}
inline void render_hearts_imgui() {
    auto& draw = *ImGui::GetBackgroundDrawList();
    float cur_time = interfaces::global_vars->current_time;

    for (int i = 0; i < active_effects.size(); ) {
        const auto& effect = active_effects[i];
        float elapsed = cur_time - effect.start_time;

        if (elapsed > heart_duration) {
            active_effects.erase(active_effects.begin() + i);
            continue;
        }

        vec3_t screen;
        vec3_t pos = effect.position;
        if (!g_render->world_to_screen(pos, screen)) {
            ++i;
            continue;
        }

        float alpha = 1.f - (elapsed / heart_duration);
        ImColor color(
            custom_color.r,
            custom_color.g,
            custom_color.b,
            static_cast<int>(custom_color.a * alpha)
        );

        draw_heart_imgui(screen.x, screen.y, color);
        ++i;
    }
}


