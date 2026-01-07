#pragma once
#include "menu.h"
#include "../../sdk/utils/render/render.h"
#include "../../dependencies/imgui/imgui_internal.h"
#include "gui_base/ui.h"
#include "gui_base/checkbox.hpp"
#include "gui_base/multicombo.hpp"
#include "../skins/skins.h"
#include "../Misc/other/other.h"
#include "gui_base/slider.hpp"
#include "skins_dumped.h"
#include "../../Bind System/key.h"
#include "gui_base/shader.hpp"
#include "../../sdk/hooks/directx.h"
#include "../visuals/main/custom_sky.h"
#include "../Aimbot/Rage/ragebot.h"
#include "../Aimbot/Legit/legit.h"
float alphablur = 255.0f;
float blur_radius = 20.0f;
float blur_strength = 20.0f;
float blur_offset = 20.0f;
static float font = 30;
bool save_requested = false;
c_subtab< std::string > rage_subtabs = {
    XOR_STR("Rage"),
    XOR_STR("Antiaim"),
};
inline const char* hitboxes[] = { "Head", "Chest", "Stomach", "Arms", "Legs", "Feet" };
static const char* targettype[] = {
"Postition",
"Best damage"
};
static const char* Chamsnames[] = {
"Flat",
"Latex",
"Bloom",
"Solid",
"Metallic",
"Ghost",
"Glow",
"Illuminate"
};
static const char* positions[] = {
    "Center",
    "Top Left"
};
static const char* indicatornames[] = {
    "Doubletap",
    "Hideshot",
    "Bodyaim",
    "Ping warning",
    "Hitchance",
    "Damage",
    "Bomb",
    "Manuals"
};
static const char* hsound[] = {
    "None",
    "Money", // \sounds\ui\coin_pickup_01.vsnd_c
    "Balloon", // \sounds\ambient\atmosphere\balloon_pop_01.vsnd_c
    "Cod", // https://github.com/tickcount/hitsounds/blob/master/cod.vsnd_c
    "Click", // https://github.com/tickcount/hitsounds/blob/master/click.vsnd_c
    "Bubble", // https://github.com/tickcount/hitsounds/blob/master/bubble.vsnd_c
    "Bell", // https://github.com/tickcount/hitsounds/blob/master/bell.vsnd_c
    "Switch", // https://github.com/tickcount/hitsounds/blob/master/arena_switch.vsnd_c
    "Apple Pay", // https://github.com/tickcount/hitsounds/blob/master/applepay.vsnd_c
    "Agpa", // https://github.com/tickcount/hitsounds/blob/master/agpa2.vsnd_c
    "Bonk", // https://github.com/tickcount/hitsounds/blob/master/bonk.vsnd_c
    "Fatality", // https://github.com/tickcount/hitsounds/blob/master/fatality.vsnd_c
    "Hentai1", // https://github.com/tickcount/hitsounds/blob/master/hentai1.vsnd_c
    "Hentai2", // https://github.com/tickcount/hitsounds/blob/master/hentai2.vsnd_c
    "Hentai3", // https://github.com/tickcount/hitsounds/blob/master/hentai3.vsnd_c
    "MC Hit", // https://github.com/tickcount/hitsounds/blob/master/minecraft_hit.vsnd_c
    "MC XP Gain", // https://github.com/tickcount/hitsounds/blob/master/minecraft_xp_gain.vsnd_c
    "Rust Headshot", // https://github.com/tickcount/hitsounds/blob/master/rust_headshot.vsnd_c
    "Zelda", // https://github.com/tickcount/hitsounds/blob/master/zelda.vsnd_c
};

c_subtab< std::string > visual_subtabs = {
    XOR_STR("Player"),
    XOR_STR("World"),
    XOR_STR("Extra"),
};
static int wg = 0;
c_subtab< std::string > misc_subtabs = {
  XOR_STR("Movement"),
  XOR_STR("Events"),
  XOR_STR("Other"),
};

void menu::draw()
{
    auto& io = ImGui::GetIO();

    if (!menu::was_opened)
    {
        io.WantCaptureMouse = false;
        io.WantCaptureKeyboard = false;
        io.MouseDrawCursor = false;
        blur_active = false;          // выключаем блюр когда меню закрыто
        blur_radius = 0.f;
        return;
    }

    // Меню открыто → можно включить блюр, если хочешь
    blur_active = true;               // ← или false, если всё равно не нужен
    blur_radius = 20.f;               // ← твоё значение по умолчанию

    io.WantCaptureMouse = true;
    io.WantCaptureKeyboard = true;
    io.MouseDrawCursor = true;
    g_notification->RenderNotifications();
    g_notification->RenderTopLeftNotifications();
    static bool notifinjected = false;
    static bool connotif = false;
    static bool connotifsuccess = false;
    auto viewmodel_fov = interfaces::cvar->get_by_name("viewmodel_fov")->get_float();
    auto viewmodel_offset_x = /*interfaces::cvar->get_by_name("viewmodel_offset_x")->get_float()*/0.f;
    auto viewmodel_offset_y = /*interfaces::cvar->get_by_name("viewmodel_offset_y")->get_float()*/0.f;
    auto viewmodel_offset_z = /*interfaces::cvar->get_by_name("viewmodel_offset_z")->get_float()*/0.f;
    static bool viewmodelupdate = false;
    config.antiaim.manualleft = false;
    config.antiaim.manualright = false;
    config.antiaim.manualback = false;
    if (g_key_handler->is_pressed(config.antiaim.manualleft_keybind, config.antiaim.manualleft_keybind_style)) {
        config.antiaim.manualleft = true;
        config.antiaim.manualright = false;
        config.antiaim.manualback = false;

    }

    if (g_key_handler->is_pressed(config.antiaim.manualright_keybind, config.antiaim.manualright_keybind_style)) {
        config.antiaim.manualright = true;
        config.antiaim.manualleft = false;
        config.antiaim.manualback = false;

    }
    if (g_key_handler->is_pressed(config.antiaim.manualback_keybind, config.antiaim.manualback_keybind_style)) {
        config.antiaim.manualright = false;
        config.antiaim.manualleft = false;
        config.antiaim.manualback = true;

    }

    if (!viewmodelupdate)
    {
        config.extra.viewmodel_fov = interfaces::cvar->get_by_name("viewmodel_fov")->get_float();
        config.extra.viewmodel_offset_x = interfaces::cvar->get_by_name("viewmodel_offset_x")->get_float();
        config.extra.viewmodel_offset_y = interfaces::cvar->get_by_name("viewmodel_offset_y")->get_float();
        config.extra.viewmodel_offset_z = interfaces::cvar->get_by_name("viewmodel_offset_z")->get_float();
        viewmodelupdate = true;
    }
    if (config.gui.watermark)
    {
        watermark();
        hotkeysdraw();

    }
    if (interfaces::engine_client && interfaces::engine_client->is_in_game())
    {

        if (!notifinjected)
        {
            g_notification->AddNotification(reinterpret_cast<const char*>(ICON_FA_CHECK), "Cheat Injected successfully");
            interfaces::engine_client->client_cmd_unrestricted("play\\sounds\\wasted.vsnd_c");
            notifinjected = true;
        }
        if (g_ctx.local) {
           /* if (g_ctx.local->is_alive())
            {
                g_render->autopick_cricle();
                if (g_ctx.local->scoped() && config.extra.removals[2])
                {
                    ImGui::GetBackgroundDrawList()->AddLine(ImVec2(io.DisplaySize.x / 2, 0), ImVec2(io.DisplaySize.x / 2, io.DisplaySize.y), IM_COL32(0, 0, 0, 255), 1.f);
                    ImGui::GetBackgroundDrawList()->AddLine(ImVec2(0, io.DisplaySize.y / 2), ImVec2(io.DisplaySize.x, io.DisplaySize.y / 2), IM_COL32(0, 0, 0, 255), 1.f);
                }
                if (config.extra.hit_effect)
                {
                    render_hearts_imgui();
                }
                if (config.gui.indicators)
                {
                    indicator();
                }
            }*/
        }
    }
    else
    {
        connotifsuccess = false;
        connotif = false;
    }
    if (!notifinjected)
    {
        g_notification->AddNotificationTopLeft(reinterpret_cast<const char*>(ICON_FA_CHECK), "Cheat Injected successfully");
        interfaces::engine_client->client_cmd_unrestricted("play \\sounds\\wasted.vsnd_c");

        notifinjected = true;
    }
    if (safemode)
    {
        config.ragebot.enable = false;
        config.antiaim.enable = false;
        config.ragebot.doubletap = false;
        config.antiaim.hideshots = false;
        config.movement.auto_strafe = false;
        config.movement.air_strafe = false;
        config.movement.walkbot = false;
        config.movement.bunnyhop = false;
    }
    // Убрано: if (!was_opened) { ... } — рисуем всегда
    was_opened = true;  // Forced open

    static int selected_tab = 0;
    const ImVec2 window_size(500, 450);

    ImGui::SetNextWindowPos(ImVec2((g_render->m_screen_size.x - window_size.x) * 0.5f, (g_render->m_screen_size.y - window_size.y) * 0.5f), ImGuiCond_Once);
    //ImGui::SetNextWindowSize(window_size, ImGuiCond_Once);
    ImVec2 display_size = ImGui::GetIO().DisplaySize;
    ImGui::SetNextWindowSizeConstraints(ImVec2(500, 450), display_size);
    ImGui::Begin(XOR_STR("##base_window"), nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoBackground);

    float speed = config.gui.animspeed / 100.f;

    if (speed <= 0.f)
    {
        gui.m_anim = 1.f;
        gui.f_anim = 1.f;
    }
    else
    {
        float lerp_speed = std::clamp(speed * 0.05f, 0.f, 1.f);
        gui.m_anim = ImLerp(gui.m_anim, 1.f, lerp_speed);
        gui.f_anim = ImLerp(gui.f_anim, 1.f, lerp_speed);
    }


    auto window = GetCurrentWindow();
    auto draw = window->DrawList;
    auto poss = window->Pos;
    auto pos = window->Pos;

    auto sizes = window->Size;
    auto size = window->Size;

    ImVec2 offset = ImVec2(2.0f, 2.0f);
    ImVec2 rect_min = ImVec2(pos.x - offset.x, pos.y - offset.y);
    ImVec2 rect_max = ImVec2(pos.x + size.x + offset.x, pos.y + size.y + offset.y);
    float alpha = gui.f_anim;

    ImGui::GetBackgroundDrawList()->AddRectFilled(
        rect_min, rect_max,
        IM_COL32(44, 44, 44, static_cast<int>(alpha * 255.f))
    );
    DrawRainbowLine(ImGui::GetWindowDrawList(), ImVec2(pos.x + 5, pos.y + 4), ImVec2(pos.x + ImGui::GetWindowWidth() - 5, pos.y + 4));

    ImGui::GetWindowDrawList()->AddRectFilled(
        ImVec2(pos.x + 65, pos.y + 5),
        ImVec2(pos.x + ImGui::GetWindowWidth() - 5, pos.y + ImGui::GetWindowHeight() - 5),
        IM_COL32(17, 17, 17, static_cast<int>(alpha * 255.f))
    );
    ImGui::GetWindowDrawList()->AddRectFilled(
        ImVec2(pos.x + 5, pos.y + 5),
        ImVec2(pos.x + 65, pos.y + ImGui::GetWindowHeight() - 5),
        IM_COL32(12, 12, 12, static_cast<int>(alpha * 255.f))
    );    ImVec4 accent = gui.accent_color.to_im_color();
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(8, 6));
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(2, 0));
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 0);    //draw->AddRectFilled(ImVec2(poss.x, poss.y), ImVec2(poss.x + 540, poss.y + 485), IM_COL32(25, 25, 25, 255), 0.0f);
    //draw->AddRectFilled(ImVec2(poss.x, poss.y), ImVec2(poss.x + 540, poss.y + 30.8), IM_COL32(static_cast<int>(accent.x * 255.f),
        //static_cast<int>(accent.y * 255.f),
        //static_cast<int>(accent.z * 255.f), 255), 0.0f);
    //draw->AddRectFilled(ImVec2(poss.x, poss.y), ImVec2(poss.x + 540, poss.y + 30), IM_COL32(33, 33, 33, 255), 0.0f);
    //draw->AddRectFilled(ImVec2(poss.x, poss.y + 462.2), ImVec2(poss.x + 540, poss.y + 485), IM_COL32(static_cast<int>(accent.x * 255.f),
        //static_cast<int>(accent.y * 255.f),
        //static_cast<int>(accent.z * 255.f), 255), 0.0f);
    //draw->AddRectFilled(ImVec2(poss.x, poss.y + 463), ImVec2(poss.x + 540, poss.y + 485), IM_COL32(33, 33, 33, 255), 0.0f);
    std::string textS = std::string("build date->") + __DATE__;

    //draw->AddText(ssfi_small, 14, ImVec2(poss.x + 10, poss.y + 465), IM_COL32(255, 255, 255, 255), textS.c_str());
    std::string text = "Welcome, " + g_authorization->username + "!";
    ImVec2 text_size = ImGui::CalcTextSize(text.c_str());

    // pos.x — это левый край меню
    // width — это ширина меню
    float right_edge_x = poss.x + 485;
    float text_x = right_edge_x - text_size.x + 40;


    //draw->AddText(ssfi_small, 14, ImVec2(text_x, poss.y + 465), IM_COL32(255, 255, 255, 255), text.c_str());

    const char* tabs[] = {
                reinterpret_cast<const char*>(ICON_FA_CROSSHAIRS),
                reinterpret_cast<const char*>(ICON_FA_CIRCLE_NOTCH),
                reinterpret_cast<const char*>(ICON_FA_GLOBE),
                reinterpret_cast<const char*>(ICON_FA_COGS),
                reinterpret_cast<const char*>(ICON_FA_MAGIC),
                reinterpret_cast<const char*>(ICON_FA_SAVE)
    };
    const int tab_count = IM_ARRAYSIZE(tabs);

    ImVec2 button_size = ImVec2(60, 60);
    ImVec2 button_min = ImVec2(pos.x + 5, pos.y + 25); // НЕ меняем, draw остаётся точным
    ImVec2 button_mindraw = ImVec2(pos.x + 5, pos.y + 15); // НЕ меняем, draw остаётся точным

    for (int i = 0; i < tab_count; ++i) {
        ImVec2 current_min = ImVec2(button_min.x, button_min.y + i * button_size.y);
        ImVec2 current_max = ImVec2(current_min.x + button_size.x, current_min.y + button_size.y);
        ImVec2 current_mindraw = ImVec2(button_mindraw.x, button_mindraw.y + i * button_size.y);
        ImVec2 current_maxdraw = ImVec2(current_mindraw.x + button_size.x, current_mindraw.y + button_size.y);

        ImGui::SetCursorScreenPos(current_min);
        std::string id = "tab_button_" + std::to_string(i);

        if (ImGui::InvisibleButton(id.c_str(), button_size)) {
            if (active_tab != i) {
                active_tab = i;
                gui.m_anim = 0.f;
                gui.f_anim = 1.f;
            }
        }
        auto fontsize = font;

        if (active_tab == i) {
            float alpha = gui.f_anim;
            ImColor color = gui.bg.to_im_color();
            color.Value.w *= alpha;

            draw->AddRectFilled(current_mindraw, current_maxdraw, color, 0.0f);
        }

        ImVec2 text_size = fa_icon->CalcTextSizeA(fa_icon->FontSize, FLT_MAX, 0.0f, tabs[i]);

        // Центрирование по центру кнопки
        float text_offset_x = current_min.x + (button_size.x / 2.0f) - (text_size.x / 2.0f);
        float text_offset_y = current_min.y + 18;
        ImU32 text_color = active_tab == i
            ? IM_COL32(255, 255, 255, static_cast<int>(alpha * 255.f))
            : IM_COL32(150, 150, 150, static_cast<int>(alpha * 255.f));


        // Рисуем текст с Font Awesome шрифтом
        draw->AddText(fa_icon, fa_icon->FontSize, ImVec2(text_offset_x, text_offset_y), text_color, tabs[i]);
    }

    SetCursorPos(ImVec2(75, 15 - (5 * gui.m_anim)));

    ImGui::BeginChild(
        "##subtabs",
        ImVec2(0, 0),
        ImGuiWindowFlags_None,
        ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse
    );
    if (active_tab == 0)
    {
        menu::rage_tab();
        //c_base_player_weapon* weapon = g_ctx.local ? g_ctx.local->get_active_weapon() : nullptr;
        //if (weapon)
        //{
        //    int new_wg = category_to_wg_index(get_weapon_category_by_defindex(get_active_weapon_def_index()));
        //    if (new_wg >= 0 && new_wg < std::size(fov_vars)) {
        //        wg = new_wg;
        //    }
        //}
    }
    if (active_tab == 1)
    {
        menu::legit_tab(pos, draw);
    }
    if (active_tab == 2)
    {

        menu::visuals_tab(pos, draw);
        //c_base_player_weapon* weapon = g_ctx.local ? g_ctx.local->get_active_weapon() : nullptr;
        //if (weapon)
        //{
        //    int new_wg = category_to_wg_index(get_weapon_category_by_defindex(get_active_weapon_def_index()));
        //    if (new_wg >= 0 && new_wg < std::size(fov_vars)) {
        //        wg = new_wg;
        //    }
        //}
    }
    if (active_tab == 3)
    {
        menu::misc_tab();
        //c_base_player_weapon* weapon = g_ctx.local ? g_ctx.local->get_active_weapon() : nullptr;
        //if (weapon)
        //{
        //    int new_wg = category_to_wg_index(get_weapon_category_by_defindex(get_active_weapon_def_index()));
        //    if (new_wg >= 0 && new_wg < std::size(fov_vars)) {
        //        wg = new_wg;
        //    }
        //}
    }
    if (active_tab == 4)
    {
        menu::skin_tab();
        //c_base_player_weapon* weapon = g_ctx.local ? g_ctx.local->get_active_weapon() : nullptr;
        //if (weapon)
        //{
        //    int new_wg = category_to_wg_index(get_weapon_category_by_defindex(get_active_weapon_def_index()));
        //    if (new_wg >= 0 && new_wg < std::size(fov_vars)) {
        //        wg = new_wg;
        //    }
        //}
    }
    if (active_tab == 5)
    {
        menu::config_tab();
        //c_base_player_weapon* weapon = g_ctx.local ? g_ctx.local->get_active_weapon() : nullptr;
        //if (weapon)
        //{
        //    int new_wg = category_to_wg_index(get_weapon_category_by_defindex(get_active_weapon_def_index()));
        //    if (new_wg >= 0 && new_wg < std::size(fov_vars)) {
        //        wg = new_wg;
        //    }
        //}
    }
    ImGui::EndChild();
    ImGui::PopStyleVar(3);
    ImGui::End();
}
void menu::legit_tab(ImVec2 pos, ImDrawList* draw)
{


    ImVec2 box_size(255, 200);
    ImVec2 small_box_size(255, 100);
    float available_height = ImGui::GetContentRegionAvail().y - 7.0f;
    float available_widght = ImGui::GetContentRegionAvail().x / 2 - 10;
    float available_x = ImGui::GetContentRegionAvail().x;

    if (available_height < 0.0f) available_height = 0.0f;
    gui.group_box("Weapon Category", ImVec2(available_widght * 2 + 10, 70));
    {
        const char* tabs[] = {
            weaponOptionsIcon[0],
            weaponOptionsIcon[1],
            weaponOptionsIcon[2],
            weaponOptionsIcon[3],
            weaponOptionsIcon[4],
            weaponOptionsIcon[5]
        };
        const int tab_count = IM_ARRAYSIZE(tabs);

        ImVec2 start_pos = ImVec2(pos.x + 80, pos.y + 35);

        float available_width = ImGui::GetContentRegionAvail().x - (start_pos.x - ImGui::GetCursorScreenPos().x);
        float button_height = 40.0f;
        float button_width = available_width / tab_count;

        ImVec2 button_min = start_pos;
        ImVec2 button_mindraw = ImVec2(start_pos.x, start_pos.y - 10);

        for (int i = 0; i < tab_count; ++i) {
            ImVec2 current_min = ImVec2(button_min.x + i * button_width, button_min.y);
            ImVec2 current_max = ImVec2(current_min.x + button_width, current_min.y + button_height);
            ImVec2 current_mindraw = ImVec2(button_mindraw.x + i * button_width, button_mindraw.y);
            ImVec2 current_maxdraw = ImVec2(current_mindraw.x + button_width, current_mindraw.y + button_height);

            ImGui::SetCursorScreenPos(current_min);
            std::string id = "tab_button_" + std::to_string(i);
            ImVec2 size = ImVec2(button_width, button_height);

            if (ImGui::InvisibleButton(id.c_str(), size)) {
                if (wg != i) {
                    wg = i;
                    gui.m_anim = 0.f;
                }
            }

            if (wg == i) {
                //ImGui::GetWindowDrawList()->AddRectFilled(current_mindraw, current_maxdraw, gui.bg.to_im_color(), 4.0f);
            }

            // Получаем размер текста с заданным шрифтом
            ImVec2 text_size = gun_iconbig->CalcTextSizeA(gun_iconbig->FontSize, FLT_MAX, 0.0f, tabs[i]);

            // Центрирование текста
            float text_offset_x = current_min.x + (button_width / 2.0f) - (text_size.x / 2.0f);
            float text_offset_y = current_min.y + (button_height / 2.0f) - (text_size.y / 2.0f) + 3 - (5 * gui.m_anim);

            ImU32 text_color = wg == i
                ? IM_COL32(255, 255, 255, static_cast<int>(gui.m_anim * 255.f))
                : IM_COL32(150, 150, 150, static_cast<int>(gui.m_anim * 255.f));


            ImGui::GetWindowDrawList()->AddText(gun_iconbig, gun_iconbig->FontSize, ImVec2(text_offset_x, text_offset_y), text_color, tabs[i]);
        }
    }
    gui.end_group_box();
    SetCursorPosY(70);
    gui.group_box("Globals", ImVec2(available_widght - 2, available_height - 70));
    {
        if (wg >= 0 && wg < std::size(fov_vars)) {
            DrawCustomSliderInt("Fov", fov_vars[wg], 0, 30);
            DrawCustomSliderInt("Smooth", smooth_vars[wg], 1, 30);
            DrawCustomSliderInt("Delay", delay_vars[wg], 0, 200);

            // Включение RCS под оружие
            checkbox("Recoil Control System", rcs_enable_vars[wg]);

            if (*rcs_enable_vars[wg]) {
                DrawCustomSliderInt("RCS Fov", rcs_fov_vars[wg], 0, 30);
                DrawCustomSliderInt("RCS Smooth", rcs_smooth[wg], 1, 30);
                DrawCustomSliderInt("RCS starts after X bullets", rcs_after_shot_vars[wg], 1, 150);
            }
            MultiComba("Hitboxes", config.legit.m_hitboxes, hitboxes, IM_ARRAYSIZE(hitboxes));
            // Мин дамаг и связанные настройки
            DrawCustomSliderInt("Minimum Damage", min_damage_vars[wg], 0, 100, true, 0, "LETHAL", 1, &config.legit.min_damage_keybind, &config.legit.min_damage_keybind, min_damage_override_vars[wg]);
        }
    }
    gui.end_group_box();

    // Правая колонка
    SameLine(available_widght + 10, 0.0f);
    SetCursorPosY(70);

    float half_height = available_height / 2.0f;

    gui.group_box("Globals II", ImVec2(available_widght, half_height));
    {
        checkbox("Enable", &config.legit.enable);
        checkbox("Draw FOV", &config.legit.draw_fov);
        checkbox("Auto Wall", &config.legit.autowall);
        checkbox("Auto Stop", &config.legit.auto_stop);
        checkbox("Auto Scope", &config.legit.auto_scope);
        checkbox("Aim Assist (afire)", &config.legit.aimlock, 1, &config.legit.aimlock_keybind, &config.legit.aimlock_keybind_style);
        MultiComba("Checks", config.legit.aim_checks, aim_checks_legit, IM_ARRAYSIZE(aim_checks_legit));

    }
    gui.end_group_box();
    SameLine(available_widght + 10, 0.0f);

    SetCursorPosY(half_height + 70); // Сдвигаемся вниз для следующего блока

    gui.group_box("Trigger-bot", ImVec2(available_widght, half_height - 70));
    {
        checkbox("Enable", &config.legit.triggerbot, 1, &config.legit.triggerbot_keybind, &config.legit.triggerbot_keybind_style);

        if (wg >= 0 && wg < std::size(tb_delay_vars))
            DrawCustomSliderInt("Delay Between Shots", tb_delay_vars[wg], 0, 500);
        if (wg >= 0 && wg < std::size(tb_hc_vars))
            DrawCustomSliderInt("Hit-Chance", tb_hc_vars[wg], 0, 100);
    }
    gui.end_group_box();
}

void menu::rage_tab()
{

    float available_height = ImGui::GetContentRegionAvail().y - 7.0f;
    float available_widght = ImGui::GetContentRegionAvail().x / 2 - 10;
    float available_x = ImGui::GetContentRegionAvail().x;

    if (available_height < 0.0f) available_height = 0.0f;

    gui.group_box("Weapon type", ImVec2(available_widght - 2, 70));
    {

    }
    gui.end_group_box();


    SetCursorPosY(70);
    gui.group_box("Aimbot", ImVec2(available_widght - 2, available_height - 70));
    {
        if (checkbox("Enable", &config.ragebot.enable))
        {
            if (safemode)
            {
                g_notification->AddNotification(reinterpret_cast<const char*>(ICON_FA_ENGINE_WARNING), "Ragebot is disabled in Anti-untrusted!");
            }
        }
        DrawCustomSliderInt("HitChance", &config.ragebot.hit_chance, 0, 100, true, 0, "AUTO", 1, &config.ragebot.hit_chance_keybind, &config.ragebot.hit_chance_keybind_style, &config.ragebot.hit_chance_override);
        DrawCustomSliderInt("Minimim Damage", &config.ragebot.min_damage, 0, 100, true, 0, "LETHAL", 1, &config.ragebot.min_damage_keybind, &config.ragebot.min_damage_keybind_style, &config.ragebot.min_damage_override);
        MultiComba("Hitboxes", config.ragebot.m_hitboxes, hitboxes, IM_ARRAYSIZE(hitboxes));
        MultiComba("Multipoint", config.ragebot.m_multi_points, hitboxes, IM_ARRAYSIZE(hitboxes));
        DrawCustomSliderInt("Pointscale", &config.ragebot.multipointscale, 0, 100);
        DrawCustomSliderInt("Target", &config.ragebot.group_type, 0, 1);


        /*if (config.ragebot.min_damage_keybind != 0)
        {
            ImGui::Text("Bind: %s", keys[config.ragebot.min_damage_keybind]);
        }
        else
        {
            ImGui::Text("Bind: None");
        }

        ImGui::Text("Mode: %s",
            config.ragebot.min_damage_keybind_style == 0 ? "Hold" :
            config.ragebot.min_damage_keybind_style == 1 ? "Toggle" :
            config.ragebot.min_damage_keybind_style == 2 ? "Always" : "Unknown");
        if (g_key_handler->is_pressed(config.ragebot.min_damage_keybind, config.ragebot.min_damage_keybind_style)) {
            ImGui::TextColored(ImVec4(0, 1, 0, 1), "State: ON");
        }
        else {
            ImGui::TextColored(ImVec4(1, 0, 0, 1), "State: OFF");
        }*/
        checkbox("Autostop", &config.ragebot.auto_stop);
    }
    gui.end_group_box();

    SameLine(available_widght + 10, 0.0f);
    SetCursorPosY(0);

    float half_height = available_height / 2.0f;

    gui.group_box("Other", ImVec2(available_widght, half_height));
    {

        checkbox("Silent", &config.ragebot.silent_aim);
        checkbox("Autofire", &config.ragebot.automatic_fire);
        checkbox("Autowall", &config.ragebot.auto_wall);
        checkbox("Doubletap", &config.ragebot.doubletap);
    }
    gui.end_group_box();
    SameLine(available_widght + 10, 0.0f);

    SetCursorPosY(half_height); // Сдвигаемся вниз для следующего блока

    gui.group_box("Anti-Aimbot angles", ImVec2(available_widght, half_height));
    {
        if (checkbox("Enable", &config.antiaim.enable))
        {
            if (safemode)
            {
                g_notification->AddNotification(reinterpret_cast<const char*>(ICON_FA_ENGINE_WARNING), "Anti Aim is disabled in Anti-untrusted!");
            }
        }

        const char* pitchOptions[] = { "None", "Zero", "Up", "Down", "Fake Up", "Fake Down", "Custom", "Jitter", "Random", "Switch", "3-Way" };
        ComboBox("Pitch", &config.antiaim.pitch_item, pitchOptions, IM_ARRAYSIZE(pitchOptions));
        if (config.antiaim.pitch_item == 6)
        {
            DrawCustomSliderInt("Pitch", &config.antiaim.pitch, -89, 89);
        }
        else if (config.antiaim.pitch_item == 7)
        {
            DrawCustomSliderInt("First Pitch", &config.antiaim.firstpitch, -89, 89);
            DrawCustomSliderInt("Second Pitch", &config.antiaim.secondpitch, -89, 89);
        }
        else if (config.antiaim.pitch_item == 8)
        {
            DrawCustomSliderInt("First Pitch", &config.antiaim.firstpitch, -89, 89);
            DrawCustomSliderInt("Second Pitch", &config.antiaim.secondpitch, -89, 89);
        }
        else if (config.antiaim.pitch_item == 9)
        {
            DrawCustomSliderInt("First Pitch", &config.antiaim.firstpitch, -89, 89);
            DrawCustomSliderInt("Second Pitch", &config.antiaim.secondpitch, -89, 89);
        }
        else if (config.antiaim.pitch_item == 10)
        {
            DrawCustomSliderInt("First Pitch", &config.antiaim.firstpitch, -89, 89);
            DrawCustomSliderInt("Second Pitch", &config.antiaim.secondpitch, -89, 89);
        }
        static int yaw_item = 0;
        const char* yawOptions[] = { "None", "Backward", "Custom" };
        ComboBox("Yaw", &config.antiaim.yaw_item, yawOptions, 3);
        if (config.antiaim.yaw_item == 0)
        {
            config.antiaim.yaw = 0;
        }
        if (config.antiaim.yaw_item == 1)
        {
            config.antiaim.yaw = 180;
        }
        if (config.antiaim.yaw_item == 2)
        {
            DrawCustomSliderInt("Yaw", &config.antiaim.yaw, -180, 180);
        }
        checkbox("Hide Shots", &config.antiaim.hideshots);
        checkbox("Manual left", &config.antiaim.manualleft, 1, &config.antiaim.manualleft_keybind, &config.antiaim.manualleft_keybind_style);
        checkbox("Manual back", &config.antiaim.manualback, 1, &config.antiaim.manualback_keybind, &config.antiaim.manualback_keybind_style);
        checkbox("Manual right", &config.antiaim.manualright, 1, &config.antiaim.manualright_keybind, &config.antiaim.manualright_keybind_style);

    }
    gui.end_group_box();
}
void menu::antiaim_tab()
{
    SetCursorPosY(37);

    gui.group_box(XOR_STR("Globals"), ImVec2(255, 418)); {

    }gui.end_group_box();

}

void menu::visuals_tab(ImVec2 pos, ImDrawList* draw)
{


    static int index = 0;
    float available_height = ImGui::GetContentRegionAvail().y - 7.0f;
    float available_widght = ImGui::GetContentRegionAvail().x / 2 - 10;
    float available_x = ImGui::GetContentRegionAvail().x;

    if (available_height < 0.0f) available_height = 0.0f;

    gui.group_box("Category", ImVec2(available_widght * 2 + 10, 70));
    {
        const char* tabs[] = {
            reinterpret_cast<const char*>(ICON_FA_USER_LOCK),
            reinterpret_cast<const char*>(ICON_FA_USER_PLUS),
            reinterpret_cast<const char*>(ICON_FA_GLOBE_EUROPE),
            reinterpret_cast<const char*>(ICON_FA_EYE)
        };
        const int tab_count = IM_ARRAYSIZE(tabs);

        ImVec2 start_pos = ImVec2(pos.x + 80, pos.y + 37);

        float available_width = ImGui::GetContentRegionAvail().x - (start_pos.x - ImGui::GetCursorScreenPos().x);
        float button_height = 40.0f;
        float button_width = available_width / tab_count;

        ImVec2 button_min = start_pos;
        ImVec2 button_mindraw = ImVec2(start_pos.x, start_pos.y - 10);

        for (int i = 0; i < tab_count; ++i) {
            ImVec2 current_min = ImVec2(button_min.x + i * button_width, button_min.y);
            ImVec2 current_max = ImVec2(current_min.x + button_width, current_min.y + button_height);
            ImVec2 current_mindraw = ImVec2(button_mindraw.x + i * button_width, button_mindraw.y);
            ImVec2 current_maxdraw = ImVec2(current_mindraw.x + button_width, current_mindraw.y + button_height);

            ImGui::SetCursorScreenPos(current_min);
            std::string id = "tab_button_" + std::to_string(i);
            ImVec2 size = ImVec2(button_width, button_height);

            if (ImGui::InvisibleButton(id.c_str(), size)) {
                if (index != i) {
                    index = i;
                    gui.m_anim = 0.f;
                }
            }

            if (index == i) {
                //ImGui::GetWindowDrawList()->AddRectFilled(current_mindraw, current_maxdraw, gui.bg.to_im_color(), 4.0f);
            }

            // Получаем размер текста с заданным шрифтом
            ImVec2 text_size = fa_icon->CalcTextSizeA(fa_icon->FontSize, FLT_MAX, 0.0f, tabs[i]);

            // Центрирование текста
            float text_offset_x = current_min.x + (button_width / 2.0f) - (text_size.x / 2.0f);
            float text_offset_y = current_min.y + (button_height / 2.0f) - (text_size.y / 2.0f) + 3 - (5 * gui.m_anim);

            ImU32 text_color = index == i
                ? IM_COL32(255, 255, 255, static_cast<int>(gui.m_anim * 255.f))
                : IM_COL32(150, 150, 150, static_cast<int>(gui.m_anim * 255.f));


            ImGui::GetWindowDrawList()->AddText(fa_icon, fa_icon->FontSize, ImVec2(text_offset_x, text_offset_y), text_color, tabs[i]);
        }
    }
    gui.end_group_box();
    SetCursorPosY(70);

    if (index == 0)
    {
        gui.group_box(XOR_STR("Enemy ESP"), ImVec2(available_widght - 2, available_height - 70)); {
            checkbox(XOR_STR("Enable"), &config.esp.enable);
            checkbox(XOR_STR("Only Visible"), &config.esp.onlyvisible);
            checkbox_with_dual_color_picker(XOR_STR("Bounding box"), &config.esp.show_box, reinterpret_cast<ImVec4*>(&config.esp.box_color), reinterpret_cast<ImVec4*>(&config.esp.box_outline_color), default_picker);
            checkbox_with_dual_color_picker(XOR_STR("Health bar"), &config.esp.show_health, reinterpret_cast<ImVec4*>(&config.esp.health_color), reinterpret_cast<ImVec4*>(&config.esp.health_color_outline), default_picker);
            checkbox_with_dual_color_picker(XOR_STR("Name"), &config.esp.show_name, reinterpret_cast<ImVec4*>(&config.esp.name_color), reinterpret_cast<ImVec4*>(&config.esp.name_color_outline), default_picker);
            checkbox_with_dual_color_picker(XOR_STR("Skeleton"), &config.esp.show_skeleton, reinterpret_cast<ImVec4*>(&config.esp.skeleton_color), reinterpret_cast<ImVec4*>(&config.esp.skeleton_outline_color), default_picker);



            // Weapon
            checkbox_with_dual_color_picker(XOR_STR("Weapon"), &config.esp.show_weapon, reinterpret_cast<ImVec4*>(&config.esp.weapon_color), reinterpret_cast<ImVec4*>(&config.esp.weapon_color_outline), default_picker);
            if (config.esp.show_weapon)
            {
                checkbox(XOR_STR("Weapon name"), &config.esp.weapon_name);
                checkbox(XOR_STR("Weapon icon"), &config.esp.weapon_icon);
            }
            checkbox_with_dual_color_picker(XOR_STR("Ammo"), &config.esp.show_ammo, reinterpret_cast<ImVec4*>(&config.esp.ammo_color), reinterpret_cast<ImVec4*>(&config.esp.ammo_color_outline), default_picker);

            checkbox_with_dual_color_picker(XOR_STR("Flags"), &config.esp.show_flags, reinterpret_cast<ImVec4*>(&config.esp.flags_color), reinterpret_cast<ImVec4*>(&config.esp.flags_text_color_outline), default_picker);

            checkbox_with_color_picker("Glow", &config.esp.glow, reinterpret_cast<ImVec4*>(&config.esp.glow_color), default_picker);
            // Snapline
            checkbox_with_color_picker(XOR_STR("Snapline"), &config.esp.show_snapline, reinterpret_cast<ImVec4*>(&config.esp.snapline_color), default_picker);
            if (config.esp.show_snapline)
            {
                checkbox_with_color_picker(XOR_STR("Snapline outline"), &config.esp.snapline_outline, reinterpret_cast<ImVec4*>(&config.esp.snapline_outline_color), default_picker);
                ComboBox(XOR_STR("Snapline type"), &config.esp.snaplinetype, snaplinelist, IM_ARRAYSIZE(snaplinelist));
            }
            checkbox_with_color_picker(XOR_STR("Sound ESP"), &config.esp.show_sound, reinterpret_cast<ImVec4*>(&config.esp.sound_color), default_picker);
        }gui.end_group_box();
        SameLine(available_widght + 10, 0.0f);
        gui.group_box("Enemy models", ImVec2(available_widght - 2, available_height - 70));
        {
            checkbox_with_color_picker(XOR_STR("Player"), &config.esp.chamsvis, reinterpret_cast<ImVec4*>(&config.esp.chamsvis_color), default_picker);
            ComboBoxWG(&config.esp.chamstypevis, Chamsnames, IM_ARRAYSIZE(Chamsnames), "Material vis");
            checkbox_with_color_picker(XOR_STR("Player through walls"), &config.esp.chamsinvis, reinterpret_cast<ImVec4*>(&config.esp.chamsinvis_color), default_picker);
            ComboBoxWG(&config.esp.chamstypeinvis, Chamsnames, IM_ARRAYSIZE(Chamsnames), "Material invis");
            checkbox(XOR_STR("Ragdoll"), &config.esp.chamsragdoll);
            checkbox_with_color_picker(XOR_STR("On Shot"), &config.esp.chamsonhit, reinterpret_cast<ImVec4*>(&config.esp.chamsonhit_color), default_picker);

            /*checkbox_with_popup("Chams", reinterpret_cast<const char*>(ICON_FA_SUN), []()
                {
                    checkbox_with_color_picker(XOR_STR("Enable"), &config.local.chamsvis, reinterpret_cast<ImVec4*>(&config.local.chamsvis_color), default_picker);
                    checkbox_with_color_picker(XOR_STR("Ignore-Z"), &config.local.chamsinvis, reinterpret_cast<ImVec4*>(&config.local.chamsinvis_color), default_picker);
                    checkbox(XOR_STR("Ragdoll"), &config.local.chamsragdoll);
                    checkbox_with_color_picker(XOR_STR("Viewmodel"), &config.local.chamsviewmodelvis, reinterpret_cast<ImVec4*>(&config.local.chamsviewmodelvis_color), default_picker);
                    checkbox_with_color_picker(XOR_STR("Weapon"), &config.local.chamsweaponvis, reinterpret_cast<ImVec4*>(&config.local.chamsweaponvis_color), default_picker);

                    ComboBox("Material", &config.local.chamstype, Chamsnames, IM_ARRAYSIZE(Chamsnames));



                });
            checkbox_with_color_picker("Local Glow", &config.esp.localglow, reinterpret_cast<ImVec4*>(&config.esp.local_glow_color), default_picker);*/
        }gui.end_group_box();
    }
    else if (index == 1)
    {
        gui.group_box(XOR_STR("Other ESP"), ImVec2(available_widght - 2, available_height - 70)); {

            checkbox(XOR_STR("Chicken"), &config.esp.chicken);
            if (config.esp.chicken)
            {
                checkbox_with_color_picker(XOR_STR("Chicken name"), &config.esp.chickenname, reinterpret_cast<ImVec4*>(&config.esp.chicken_name_color), default_picker);
                if (config.esp.chickenname)
                {
                    checkbox_with_color_picker(XOR_STR("Chicken name outline"), &config.esp.chicken_outline_name, reinterpret_cast<ImVec4*>(&config.esp.chicken_name_outline_color), default_picker);
                }
                checkbox_with_color_picker(XOR_STR("Chicken bounding box"), &config.esp.chickenbox, reinterpret_cast<ImVec4*>(&config.esp.chicken_box_color), default_picker);
                if (config.esp.chickenbox)
                {
                    checkbox_with_color_picker(XOR_STR("Chicken bounding box outline"), &config.esp.chicken_outline_box, reinterpret_cast<ImVec4*>(&config.esp.chicken_box_outline_color), default_picker);
                }
                checkbox_with_color_picker(XOR_STR("Chicken model"), &config.esp.chickenchams, reinterpret_cast<ImVec4*>(&config.esp.chicken_chams_color), default_picker);
                ComboBoxWG(&config.esp.chickentypechams, Chamsnames, IM_ARRAYSIZE(Chamsnames));
            }
            checkbox(XOR_STR("Hostage"), &config.esp.hostage);
            if (config.esp.hostage)
            {
                checkbox_with_color_picker(XOR_STR("Hostage name"), &config.esp.hostagename, reinterpret_cast<ImVec4*>(&config.esp.hostage_name_color), default_picker);
                if (config.esp.hostagename)
                {
                    checkbox_with_color_picker(XOR_STR("Hostage name outline"), &config.esp.hostage_outline_name, reinterpret_cast<ImVec4*>(&config.esp.hostage_name_outline_color), default_picker);
                }
                checkbox_with_color_picker(XOR_STR("Hostage bounding box"), &config.esp.hostagebox, reinterpret_cast<ImVec4*>(&config.esp.hostage_box_color), default_picker);
                if (config.esp.hostagebox)
                {
                    checkbox_with_color_picker(XOR_STR("Hostage bounding box outline"), &config.esp.hostage_outline_box, reinterpret_cast<ImVec4*>(&config.esp.hostage_box_outline_color), default_picker);
                }
                checkbox_with_color_picker(XOR_STR("Hostage model"), &config.esp.hostagechams, reinterpret_cast<ImVec4*>(&config.esp.hostage_chams_color), default_picker);
                ComboBoxWG(&config.esp.hostagetypechams, Chamsnames, IM_ARRAYSIZE(Chamsnames), "12323");

            }
        }gui.end_group_box();
    }
    else if (index == 2)
    {
        {
            gui.group_box(XOR_STR("World"), ImVec2(available_widght - 2, available_height - 70)); {

                checkbox(XOR_STR("World Modulation"), &config.world_modulation.ambience);
                if (config.world_modulation.ambience)
                {
                    bool yasosal;
                    color_picker(XOR_STR("World"), reinterpret_cast<ImVec4*>(&config.world_modulation.world_clr), default_picker);
                    color_picker(XOR_STR("Props"), reinterpret_cast<ImVec4*>(&config.world_modulation.props_clr), default_picker);
                    color_picker(XOR_STR("Skybox"), reinterpret_cast<ImVec4*>(&config.world_modulation.sky_clr), default_picker);
                    color_picker(XOR_STR("Sun"), reinterpret_cast<ImVec4*>(&config.world_modulation.sun_clr), default_picker);
                    color_picker(XOR_STR("Clouds"), reinterpret_cast<ImVec4*>(&config.world_modulation.clouds_clr), default_picker);
                    color_picker(XOR_STR("Lighting"), reinterpret_cast<ImVec4*>(&config.world_modulation.lighting_clr), default_picker);

                }
                checkbox(XOR_STR("Kill effect"), &config.world_modulation.sparkeffects);
                checkbox(XOR_STR("Dropped Weapons"), &config.world.weapon);
                if (config.world.weapon)
                {
                    checkbox_with_color_picker(XOR_STR("Weapon Name"), &config.world.weaponname, reinterpret_cast<ImVec4*>(&config.world.weapon_name_color), default_picker);
                    if (config.world.weaponname)
                    {
                        checkbox_with_color_picker(XOR_STR("Weapon Name Outline"), &config.world.weapon_outline_name, reinterpret_cast<ImVec4*>(&config.world.weapon_name_outline_color), default_picker);
                    }
                    checkbox_with_color_picker(XOR_STR("Weapon Icon"), &config.world.weaponicon, reinterpret_cast<ImVec4*>(&config.world.weapon_icon_color), default_picker);
                    if (config.world.weaponicon)
                    {
                        checkbox_with_color_picker(XOR_STR("Weapon Icon Outline"), &config.world.weapon_outline_icon, reinterpret_cast<ImVec4*>(&config.world.weapon_icon_outline_color), default_picker);
                    }
                    checkbox_with_color_picker(XOR_STR("Weapon Box "), &config.world.weaponbox, reinterpret_cast<ImVec4*>(&config.world.weapon_box_color), default_picker);
                    if (config.world.weaponbox)
                    {
                        checkbox_with_color_picker(XOR_STR("Weapon Box Outline"), &config.world.weapon_outline_box, reinterpret_cast<ImVec4*>(&config.world.weapon_box_outline_color), default_picker);
                    }
                    checkbox_with_color_picker(XOR_STR("Weapon Chams"), &config.world.chamsweaponvis, reinterpret_cast<ImVec4*>(&config.world.chamsweaponvis_color), default_picker);
                    if (config.world.chamsweaponvis)
                    {
                        checkbox_with_color_picker(XOR_STR("Ignore-Z"), &config.world.chamsweaponinvis, reinterpret_cast<ImVec4*>(&config.world.chamsweaponinvis_color), default_picker);
                        ComboBox("Weapon Material", &config.world.weapontypechams, Chamsnames, IM_ARRAYSIZE(Chamsnames));
                    }
                }
                checkbox(XOR_STR("Planted C4"), &config.world.bomb);
                if (config.world.bomb)
                {
                    checkbox_with_color_picker(XOR_STR("C4 Name"), &config.world.bombname, reinterpret_cast<ImVec4*>(&config.world.bomb_name_color), default_picker);
                    if (config.world.bombname)
                    {
                        checkbox_with_color_picker(XOR_STR("C4 Name Outline"), &config.world.bomb_outline_name, reinterpret_cast<ImVec4*>(&config.world.bomb_name_outline_color), default_picker);
                    }
                    checkbox_with_color_picker(XOR_STR("C4 Icon"), &config.world.bombicon, reinterpret_cast<ImVec4*>(&config.world.bomb_icon_color), default_picker);
                    if (config.world.bombicon)
                    {
                        checkbox_with_color_picker(XOR_STR("C4 Icon Outline"), &config.world.bomb_outline_icon, reinterpret_cast<ImVec4*>(&config.world.bomb_icon_outline_color), default_picker);
                    }
                    checkbox_with_color_picker(XOR_STR("C4 Box "), &config.world.bombbox, reinterpret_cast<ImVec4*>(&config.world.bomb_box_color), default_picker);
                    if (config.world.bombbox)
                    {
                        checkbox_with_color_picker(XOR_STR("C4 Box Outline"), &config.world.bomb_outline_box, reinterpret_cast<ImVec4*>(&config.world.bomb_box_outline_color), default_picker);
                    }
                    checkbox_with_color_picker(XOR_STR("C4 Timer"), &config.world.bombtimertext, reinterpret_cast<ImVec4*>(&config.world.bomb_timer_text_color), default_picker);
                    if (config.world.bombtimertext)
                    {
                        checkbox_with_color_picker(XOR_STR("C4 Timer Outline"), &config.world.bomb_outline_timertext, reinterpret_cast<ImVec4*>(&config.world.bomb_timer_text_outline_color), default_picker);
                    }

                }
                checkbox("Granades", &config.world.granades);
                if (config.world.granades)
                {
                    checkbox(XOR_STR("Grenade Trail"), &config.extra.nade_traces);
                    checkbox(XOR_STR("Grenade Camera"), &config.extra.nade_camera);
                }

            } gui.end_group_box();
        }
    }
    else if (index == 3)
    {
        gui.group_box(XOR_STR("Other"), ImVec2(available_widght - 2, available_height - 70)); {

            {
                checkbox("Switch Knife Hand", &config.extra.handswitch);
                checkbox(XOR_STR("Third Person"), &config.extra.thirdperson, 1, &config.extra.thirdperson_keybind, &config.extra.thirdperson_keybind_style);
                if (config.extra.thirdperson)
                {
                    DrawCustomSliderInt(XOR_STR("Distance"), &config.extra.thirdpersondist, 30, 200);
                    DrawCustomSliderInt(XOR_STR("Alpha When Scoped"), &config.extra.thirdpersonalpha, 0, 255);
                }
                checkbox("Override Viewmodel", &config.extra.viewmodel);
                if (config.extra.viewmodel)
                {
                    int viewFOVSliderInt = static_cast<int>(config.extra.viewmodel_fov);
                    DrawCustomSliderInt("Viewmodel FOV", &viewFOVSliderInt, 30, 110);
                    config.extra.viewmodel_fov = static_cast<float>(viewFOVSliderInt);

                    int viewXSliderInt = static_cast<int>(config.extra.viewmodel_offset_x);
                    DrawCustomSliderInt("X", &viewXSliderInt, -10, 10);
                    config.extra.viewmodel_offset_x = static_cast<float>(viewXSliderInt);

                    int viewYSliderInt = static_cast<int>(config.extra.viewmodel_offset_y);
                    DrawCustomSliderInt("Y", &viewYSliderInt, -10, 10);
                    config.extra.viewmodel_offset_y = static_cast<float>(viewYSliderInt);

                    int viewZSliderInt = static_cast<int>(config.extra.viewmodel_offset_z);
                    DrawCustomSliderInt("Z", &viewZSliderInt, -10, 10);
                    config.extra.viewmodel_offset_z = static_cast<float>(viewZSliderInt);
                }
                checkbox("Override View", &config.extra.overrideview);
                if (config.extra.overrideview)
                {
                    DrawCustomSliderFloat(XOR_STR("Override FOV"), &config.extra.desiredfov, 30, 160);
                    DrawCustomSliderFloat(XOR_STR("FOV in First Zoom"), &config.extra.desired_fov_zoom_level_1, 30, 160);
                    DrawCustomSliderFloat(XOR_STR("FOV in Second Zoom"), &config.extra.desired_fov_zoom_level_2, 30, 160);
                }
                MultiComba("Removals", config.extra.removals, removalslist, IM_ARRAYSIZE(removalslist));
                checkbox(XOR_STR("Hit marker"), &config.extra.hit_effect);
                checkbox(XOR_STR("Radar"), &config.esp.radarhack);
                checkbox("Sniper crosshair", &config.extra.sniper_cross);

            }
        } gui.end_group_box();
    }
    else
    {

    }
}
void menu::misc_tab()
{
    float available_height = ImGui::GetContentRegionAvail().y - 7.0f;
    float available_widght = ImGui::GetContentRegionAvail().x / 2 - 10;
    float available_x = ImGui::GetContentRegionAvail().x;

    if (available_height < 0.0f) available_height = 0.0f;
    gui.group_box("Miscellaneous", ImVec2(available_widght - 2, available_height - 75));
    {
        checkbox(XOR_STR("Watermark"), &config.gui.watermark);
        checkbox(XOR_STR("Draw logs"), &config.gui.hitlog);
        if (config.gui.hitlog)
        {
            ComboBoxWG(&config.gui.hitlog_position, positions, IM_ARRAYSIZE(positions), "Position");
            checkbox(XOR_STR("Logs animation"), &config.gui.misslog);
            checkbox(XOR_STR("Logs in chat"), &config.gui.chatlogs);
            //DrawCustomSliderFloat(XOR_STR("Spacing"), &g_notification->spacing, -10, 10);
        }
        checkbox(XOR_STR("Kill-Say"), &config.extra.trashtalk);
        ComboBox(XOR_STR("Hit sound"), &config.gui.hsound, hsound, IM_ARRAYSIZE(hsound));
        if (config.gui.hsound != 0)
        {
            DrawCustomSliderInt("Hitsound volume", &config.gui.hsoundvol, 0, 100);
        }
        ComboBox(XOR_STR("Kill sound"), &config.gui.ksound, hsound, IM_ARRAYSIZE(hsound));
        if (config.gui.ksound != 0)
        {
            DrawCustomSliderInt("Killsound volume", &config.gui.ksoundvol, 0, 100);
        }
    }
    gui.end_group_box();


    SetCursorPosY(available_height - 75);
    gui.group_box("Matchmaking", ImVec2(available_widght - 2, 75));
    {

        checkbox("Auto-accept mathmaking", &config.extra.auto_accept);
    }
    gui.end_group_box();

    SameLine(available_widght + 10, 0.0f);
    SetCursorPosY(0);

    float half_height = available_height / 2.0f;

    gui.group_box("Movement", ImVec2(available_widght, half_height));
    {
        {
            if (checkbox(XOR_STR("Bunny hop"), &config.movement.bunnyhop))
            {
                if (safemode) {
                    g_notification->AddNotification(reinterpret_cast<const char*>(ICON_FA_ENGINE_WARNING), "Bunny hop is disabled in Anti-untrusted!");
                }
            }
            if (checkbox(XOR_STR("Auto strafe"), &config.movement.auto_strafe))
            {
                if (safemode) {
                    g_notification->AddNotification(reinterpret_cast<const char*>(ICON_FA_ENGINE_WARNING), "Auto-Strafer is disabled in Anti-untrusted!");
                }
            }
            if (checkbox(XOR_STR("Air strafe"), &config.movement.air_strafe))
            {
                if (safemode)
                {
                    g_notification->AddNotification(reinterpret_cast<const char*>(ICON_FA_ENGINE_WARNING), "Air-Strafer is disabled in Anti-untrusted!");
                }
            }
            if (checkbox(XOR_STR("Walk bot"), &config.movement.walkbot))
            {
                if (safemode)
                {
                    g_notification->AddNotification(reinterpret_cast<const char*>(ICON_FA_ENGINE_WARNING), "Walk Bot is disabled in Anti-untrusted!");
                }
            }
            checkbox(XOR_STR("Quick peek assist"), &config.movement.autopeek, 1, &config.movement.autopeek_keybind, &config.movement.autopeek_keybind_style);
        }
    }
    gui.end_group_box();
    SameLine(available_widght + 10, 0.0f);

    SetCursorPosY(half_height); // Сдвигаемся вниз для следующего блока

    gui.group_box("Settings", ImVec2(available_widght, half_height));
    {
        color_picker(XOR_STR("Menu color"), reinterpret_cast<ImVec4*>(&gui.accent_color), default_picker);
        DrawCustomSliderInt(XOR_STR("UI opacity"), &config.gui.menu_opacity, 0, 255);
        DrawCustomSliderInt(XOR_STR("Menu animation speed"), &config.gui.animspeed, 0, 255, true, 0, "OFF");
        MultiComba(XOR_STR("On screen indicators"), config.gui.indicators, indicatornames, IM_ARRAYSIZE(indicatornames));

        checkbox(XOR_STR("Anti-untrusted"), &safemode);

    }
    gui.end_group_box();
}

void menu::skin_tab()
{
    float available_height = ImGui::GetContentRegionAvail().y - 7.0f;
    float available_widght = ImGui::GetContentRegionAvail().x / 2 - 10;
    float available_x = ImGui::GetContentRegionAvail().x;

    if (available_height < 0.0f) available_height = 0.0f;


    if (!g_ctx.local)
    {
        gui.group_box("Model Customization (?)", ImVec2(available_widght - 2, available_height));
        {
            ImGui::TextWrapped("You need to be alive and in a team to customize models. Please respawn or join a team to access model customization.");
        }
        gui.end_group_box();
    }
    else if (g_ctx.local->team_num() == 2)
    {
        gui.group_box("Model Customization (T)", ImVec2(available_widght - 2, available_height));
        {
            checkbox(("Agent"), &config.extra.model_t);
            if (config.extra.model_t)
            {
                ComboBoxWG(&config.extra.m_selected, g_skins->m_agents.m_dumped_agent_name.data(), g_skins->m_agents.m_dumped_agent_name.size(), "##agents t");
                checkbox(("Custom"), &config.extra.custom_models_t);
                static std::string base_path = []() {
                    const char* steam_path = std::getenv(("SteamPath"));
                    return steam_path ? std::string(steam_path) + ("/steamapps/common/Counter-Strike Global Offensive/csgo/models") : ("");
                    }();

                static auto m_data = g_skins->custom_models(base_path);
                static int m_selected_model = 0;

                std::vector< std::string > m_all_models = { ("none") };

                for (const auto& [author, packs] : m_data)
                {
                    for (const auto& [pack, models] : packs)
                    {
                        for (const auto& model : models)
                        {
                            int slash = model.find_last_of(("/\\"));
                            int dot = model.find_last_of('.');

                            m_all_models.push_back((slash != std::string::npos && dot != std::string::npos) ? model.substr(slash + 1, dot - slash - 1) : model);
                        }
                    }
                }
                if (config.extra.custom_models_t)
                {
                    // Подготовим массив const char*
                    std::vector<const char*> model_items;
                    model_items.reserve(m_all_models.size());

                    for (const auto& model : m_all_models)
                        model_items.push_back(model.c_str());

                    // Заменили ImGui::Combo на ComboBoxWG
                    if (ComboBoxWG(&m_selected_model, model_items.data(), static_cast<int>(model_items.size()), "##select_model"))
                    {
                        g_skins->m_model_path_t = (m_selected_model == 0) ? ("") : [&]()
                            {
                                std::string mod_name = m_all_models[m_selected_model];

                                for (const auto& [author, packs] : m_data)
                                {
                                    for (const auto& [pack, models] : packs)
                                    {
                                        for (const auto& model : models)
                                        {
                                            int slash = model.find_last_of(("/\\"));
                                            int dot = model.find_last_of('.');

                                            std::string model_name = (slash != std::string::npos && dot != std::string::npos)
                                                ? model.substr(slash + 1, dot - slash - 1)
                                                : model;

                                            if (model_name == mod_name)
                                                return base_path + "/" + author + "/" + pack + "/" + model;
                                        }
                                    }
                                }

                                return std::string();
                            }();
                    }

                }
            }
            checkbox(("Knife"), &config.extra.knife_changer_t);
            if (config.extra.knife_changer_t)
            {
                ComboBoxWG(&config.extra.m_selected_knife_t, g_skins->m_knives.m_dumped_knife_name.data(), g_skins->m_knives.m_dumped_knife_name.size(), "##knifes");
            }
        }
        gui.end_group_box();
    }
    else if (g_ctx.local->team_num() == 3)
    {
        gui.group_box("Model Customization (CT)", ImVec2(available_widght - 2, available_height));
        {
            checkbox(("Agent"), &config.extra.model_ct);
            if (config.extra.model_ct)
            {
                ComboBoxWG(&config.extra.m_selected_ct, g_skins->m_agents.m_dumped_agent_name.data(), g_skins->m_agents.m_dumped_agent_name.size(), "##agents ct");
                checkbox(("Custom"), &config.extra.custom_models_ct);
                static std::string base_path = []() {
                    const char* steam_path = std::getenv(("SteamPath"));
                    return steam_path ? std::string(steam_path) + ("/steamapps/common/Counter-Strike Global Offensive/csgo/models") : ("");
                    }();

                static auto m_data = g_skins->custom_models(base_path);
                static int m_selected_model = config.extra.custommodel_ct;

                std::vector< std::string > m_all_models = { ("none") };

                for (const auto& [author, packs] : m_data)
                {
                    for (const auto& [pack, models] : packs)
                    {
                        for (const auto& model : models)
                        {
                            int slash = model.find_last_of(("/\\"));
                            int dot = model.find_last_of('.');

                            m_all_models.push_back((slash != std::string::npos && dot != std::string::npos) ? model.substr(slash + 1, dot - slash - 1) : model);
                        }
                    }
                }
                if (config.extra.custom_models_ct)
                {
                    // Подготовим массив const char*
                    std::vector<const char*> model_items;
                    model_items.reserve(m_all_models.size());

                    for (const auto& model : m_all_models)
                        model_items.push_back(model.c_str());

                    // Заменили ImGui::Combo на ComboBoxWG
                    if (ComboBoxWG(&m_selected_model, model_items.data(), static_cast<int>(model_items.size()), "##select_model"))
                    {
                        g_skins->m_model_path_ct = (m_selected_model == 0) ? ("") : [&]()
                            {
                                std::string mod_name = m_all_models[m_selected_model];

                                for (const auto& [author, packs] : m_data)
                                {
                                    for (const auto& [pack, models] : packs)
                                    {
                                        for (const auto& model : models)
                                        {
                                            int slash = model.find_last_of(("/\\"));
                                            int dot = model.find_last_of('.');

                                            std::string model_name = (slash != std::string::npos && dot != std::string::npos)
                                                ? model.substr(slash + 1, dot - slash - 1)
                                                : model;

                                            if (model_name == mod_name)
                                                return base_path + "/" + author + "/" + pack + "/" + model;
                                        }
                                    }
                                }

                                return std::string();
                            }();
                    }

                }
            }
            checkbox(("Knife"), &config.extra.knife_changer_ct);
            if (config.extra.knife_changer_ct)
            {
                ComboBoxWG(&config.extra.m_selected_knife_ct, g_skins->m_knives.m_dumped_knife_name.data(), g_skins->m_knives.m_dumped_knife_name.size(), "##knifes");
            }
        }
        gui.end_group_box();
    }
    else
    {
        gui.group_box("Model Customization (?)", ImVec2(available_widght - 2, available_height));
        {
            ImGui::TextWrapped("You are not in a team, so you can't customize models. Please join a team to access this feature.");
        }
        gui.end_group_box();
    }
    SameLine(available_widght + 10, 0.0f);
    gui.group_box("Weapon skin", ImVec2(available_widght, available_height));
    {
        {
            if (config.extra.m_selected_type == 1) {
                auto& selected_entry = config.extra.m_skin_settings[config.extra.m_selected_weapon];
                auto& selected_weapon_entry = g_skins->m_dumped_items[config.extra.m_selected_weapon];
                static std::vector<const char*> weapon_names{};

                if (weapon_names.size() < g_skins->m_dumped_items.size())
                    for (auto& item : g_skins->m_dumped_items)
                        weapon_names.emplace_back(item.m_name);

                static char search_buf[128] = "";

                ImGui::Combo("Weapons", &config.extra.m_selected_weapon, weapon_names.data(), weapon_names.size());

                ImGui::InputTextWithHint("##sosal", "search", search_buf, IM_ARRAYSIZE(search_buf));

                if (ImGui::BeginListBox("##skins"))
                {
                    for (auto& skin : selected_weapon_entry.m_dumped_skins)
                    {
                        // Фильтр по поиску
                        if (strlen(search_buf) > 0 && std::string(skin.m_name).find(search_buf) == std::string::npos)
                            continue;

                        ImGui::PushID(&skin);
                        if (ImGui::Selectable(skin.m_name, selected_entry.m_paint_kit == skin.m_id))
                        {
                            if (selected_weapon_entry.m_selected_skin == &skin)
                                selected_weapon_entry.m_selected_skin = nullptr;
                            else
                            {
                                selected_weapon_entry.m_selected_skin = &skin;
                                selected_entry.m_paint_kit = skin.m_id;
                            }
                        }
                        ImGui::PopID();
                    }
                    ImGui::EndListBox();
                }

            }


        }
    }
    gui.end_group_box();
}

void menu::config_tab()
{
    // Вызываем RefreshConfigs() ТОЛЬКО один раз при первом открытии вкладки
    static bool configs_loaded = false;
    if (!configs_loaded) {
        RefreshConfigs();
        configs_loaded = true;
    }

    float available_height = ImGui::GetContentRegionAvail().y - 7.0f;
    float available_widght = ImGui::GetContentRegionAvail().x / 2 - 10;

    gui.group_box("Settings", ImVec2(available_widght - 2, available_height));
    {
        std::vector<const char*> items;
        items.reserve(configNames.size());
        for (const auto& name : configNames) {
            items.push_back(name.c_str());
        }

        ComboBox("Config list", &selected, items.data(), static_cast<int>(items.size()));

        std::string selectedConfig = "./imzz/config/" + configNames[selected] + ".cfg";

        if (ImGui::Button("Load", ImVec2(ImGui::GetContentRegionAvail().x, 20))) {
            LoadConfig(selectedConfig);
        }

        if (ImGui::Button("Save", ImVec2(ImGui::GetContentRegionAvail().x, 20))) {
            save_requested = true;  // флаг, сохранение в OnCreateMove
            g_notification->AddNotificationTopLeft(reinterpret_cast<const char*>(ICON_FA_INFO_CIRCLE), "Save requested...");
        }

        // Кнопка Refresh — теперь безопасно, можно нажимать много раз
        if (ImGui::Button("Refresh", ImVec2(ImGui::GetContentRegionAvail().x, 20))) {
            RefreshConfigs();
            g_notification->AddNotificationTopLeft(reinterpret_cast<const char*>(ICON_FA_RECYCLE), "Configs refreshed");
        }

        if (ImGui::Button("Open Folder", ImVec2(ImGui::GetContentRegionAvail().x, 20))) {
            OpenFolderCFG();
        }

        // Твои остальные кнопки...
        if (ImGui::Button("Testing cheat", ImVec2(ImGui::GetContentRegionAvail().x, 20))) {
            interfaces::engine_client->client_cmd_unrestricted("sv_cheats 1;bot_stop 1;bot_add;sv_quantize_movement_input 0");
        }

        // Check sync — оставляем как есть, он в отдельном потоке
        static bool sync_thread_started = false;
        static std::mutex sync_mutex;
        static std::condition_variable sync_cv;
        static bool sync_requested = false;

        if (ImGui::Button("Check sync", ImVec2(ImGui::GetContentRegionAvail().x, 20))) {
            std::unique_lock lock(sync_mutex);
            if (!sync_thread_started) {
                sync_thread_started = true;
                std::thread([] {
                    while (true) {
                        std::unique_lock lock(sync_mutex);
                        sync_cv.wait(lock, [] { return sync_requested; });
                        sync_requested = false;
                        lock.unlock();

                        // Действия синхронизации
                        g_authorization->send_identity();
                        g_authorization->load_known_players();
                    }
                    }).detach();
            }
            sync_requested = true;
            sync_cv.notify_one();
        }
    }
    gui.end_group_box();

    SameLine(available_widght + 10, 0.0f);

    gui.group_box("General", ImVec2(available_widght, available_height));
    {
        if (ImGui::Button("INVITE TO NAVI", ImVec2(0, 0))) {
            interfaces::m_legacy_game_ui->show_message_box("INVAYT OT NAVI", "ZAHODI NA pornhub.com I AKTIVIRUY KONTRAKT PRYAM SHCHAS!!!");
            was_opened = false;
        }
    }
    gui.end_group_box();
}

void menu::on_create_move()
{
    if (!was_opened) return;

    g_ctx.user_cmd->buttons.button_state &= ~(IN_ATTACK | IN_ATTACK2);
    g_ctx.user_cmd->buttons.button_state2 &= ~(IN_ATTACK | IN_ATTACK2);
    g_ctx.user_cmd->buttons.button_state3 &= ~(IN_ATTACK | IN_ATTACK2);

    if (save_requested) {
        std::string selectedConfig = "./imzz/config/" + configNames[selected] + ".cfg";
        SaveConfig(selectedConfig);
        save_requested = false;
        g_notification->AddNotificationTopLeft(reinterpret_cast<const char*>(ICON_FA_CHECK_CIRCLE), "Config saved safely!");
    }
}
