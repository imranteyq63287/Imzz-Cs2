#include "../../../includes.h"
#include "../../../sdk/utils/math.h"
#include <wininet.h>

#pragma comment(lib, "wininet.lib")
class c_other {
public:
    void spectator(c_cs_player_controller* player_controller, c_cs_player_pawn* player_pawn);
    void RunSpectator();
    void handswitch();
    void hit_logs();
    int handswitched;
    int needed_to_switch;
    void hidehud();
    void unhidehud();
    void penitrations(ImDrawList* draw);
    void active_smoke(ImDrawList* draw);
    void active_grenade_timers(ImDrawList* draw);
    void active_molotov(ImDrawList* draw);

    void debug_inferno_log();
    static std::string GetDirectory()
    {
        HMODULE hClientDll = GetModuleHandleA("client.dll");
        if (!hClientDll)
            return "";

        char path[MAX_PATH]{};
        if (GetModuleFileNameA(hClientDll, path, MAX_PATH) == 0)
            return "";

        std::string fullPath = path;

        // Удалим всё после "csgo\\" (включая "bin\\win64\\client.dll")
        size_t pos = fullPath.find("\\csgo\\");
        if (pos != std::string::npos)
        {
            // Обрезаем до "csgo\\" (включительно)
            fullPath = fullPath.substr(0, pos + strlen("\\csgo\\"));
        }

        return fullPath;
    }
    bool download_file(const std::string& url, const std::string& save_path) {
        HINTERNET hInternet;
#ifdef _RELEASE
        hInternet = InternetOpen(L"Downloader", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
#else
        hInternet = InternetOpen("Downloader", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
#endif

        if (!hInternet) return false;

        HINTERNET hFile = InternetOpenUrlA(hInternet, url.c_str(), NULL, 0, INTERNET_FLAG_RELOAD, 0);
        if (!hFile) {
            InternetCloseHandle(hInternet);
            return false;
        }

        char buffer[4096];
        DWORD bytesRead;
        std::ofstream outFile(save_path, std::ios::binary);

        while (InternetReadFile(hFile, buffer, sizeof(buffer), &bytesRead) && bytesRead != 0) {
            outFile.write(buffer, bytesRead);
        }

        outFile.close();
        InternetCloseHandle(hFile);
        InternetCloseHandle(hInternet);

        return true;
    }
    void downloadsound(std::string& path);
    std::string get_filename_from_url(const std::string& url) {
        size_t last_slash = url.find_last_of('/');
        if (last_slash == std::string::npos)
            return ""; 

        return url.substr(last_slash + 1); 
    }

};


inline const auto g_other = std::make_unique<c_other>();
