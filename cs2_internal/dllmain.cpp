#include "includes.h"
#include "features/config_system.h"
#include "sdk/interfaces/interfaces.h"
#include "sdk/entity/entity.h"
#include "sdk/modules/modules.h"
#include "sdk/hooks/hooks.h"
#include "sdk/hooks/directx.h"
#include "features/menu/menu.h"
#include "features/visuals/other/event.hpp"
#include "auth/auth.h"
#include "features/visuals/main/weather.hpp"

#ifdef _DEBUG
void destroy(HMODULE module)
{
    debug_text(RED_COLOR, "Destroying.. \n");

    entity::level_shutdown();
    directx::unitialize();
    hooks::destroy();

    PostMessageA(GetConsoleWindow(), WM_CLOSE, 0, 0);

    FreeConsole();
    FreeLibraryAndExitThread(module, 0);
}
#endif

static bool release_mode = false;

void init(HMODULE module)
{
    if (!release_mode)
    {
        AllocConsole();
        SetConsoleTitleA((g_ctx.cheat_name + " bustit!").c_str());

        freopen_s(reinterpret_cast<FILE**>(stdout), "CONOUT$", "w", stdout);
        freopen_s(reinterpret_cast<FILE**>(stderr), "CONOUT$", "w", stderr);
        freopen_s(reinterpret_cast<FILE**>(stdin), "CONIN$", "r", stdin);
    }

    debug_text(BLUE_COLOR, "Welcome to debug mode \n");
    debug_text(BLUE_COLOR, "Build date: %s \n\n", __DATE__);

    g_modules.parse();
    debug_text(WHITE_COLOR, "Modules initialized \n");

    interfaces::initialize();
    //interfaces::cvar->unlock_hidden_vars();

    debug_text(WHITE_COLOR, "Interfaces initialized \n");
    g_events->initialize();
    debug_text(WHITE_COLOR, "Event System initialized \n");

    entity::initialize();
    debug_text(WHITE_COLOR, "Entity system initialized \n");

    directx::initialize();
    debug_text(WHITE_COLOR, "DirectX initialized \n");



    if (release_mode)
    {
        if (g_authorization->CheckCredentialsMatch())
        {
            g_authorization->LoadCredentials(g_authorization->username, g_authorization->password, g_authorization->uid);
            debug_text(WHITE_COLOR, "Authorization complided \n");

            hooks::initialize();
            debug_text(WHITE_COLOR, "Hooks initialized \n");
           
            //weather::init(interfaces::c_game_particle_manager_system, interfaces::m_particle_manager);
            //weather::set_type(weather_type::RAIN);

            menu::blur_active = true;
            menu::blur_start_time = std::chrono::steady_clock::now();

            debug_text(GREEN_COLOR, (g_ctx.cheat_name + " was successfully injected!\n").c_str());
        }
    }
    else if (!release_mode)
    {
		g_authorization->username = "DEVELOPER";
		g_authorization->password = "DEVELOPER";
        g_authorization->uid = "1488";

        hooks::initialize();

        debug_text(WHITE_COLOR, "Hooks initialized \n");

        //weather::init(interfaces::c_game_particle_manager_system, interfaces::m_particle_manager);
        //weather::set_type(weather_type::RAIN);
        // 
        //menu::blur_active = true;
        //menu::blur_start_time = std::chrono::steady_clock::now();

        menu::was_opened = true;

        debug_text(GREEN_COLOR, (g_ctx.cheat_name + " was successfully injected!\n").c_str());
    }

#ifdef _DEBUG
    while (!GetAsyncKeyState(VK_END))
        Sleep(200);

    destroy(module);
#endif
}

bool __stdcall DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
    if (ul_reason_for_call == DLL_PROCESS_ATTACH)
    {
        std::thread([hModule, lpReserved]()
        {
            CreateThread(0, 0, reinterpret_cast<LPTHREAD_START_ROUTINE>(init), hModule, 0, 0);
        }).detach();
    }

    return true;
}
