#include "auth.h"
#include <wininet.h>
#include "../features/config_system.h"

#define NO_AUTH

void debug_t(const char* text) {
    debug_text(WHITE_COLOR, text);
}
std::string xor_encrypt(const std::string& input, const std::string& key) {
    std::string output = input;
    for (size_t i = 0; i < input.size(); ++i)
        output[i] ^= key[i % key.size()];
    return output;
}
std::string xor_decrypt(const std::string& data, const std::string& key) {
    return xor_encrypt(data, key);
}
const char* b64_chars = ("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/");
std::string base64_encode(const std::string& input) {
    std::string ret;
    int val = 0, valb = -6;
    for (uint8_t c : input) {
        val = (val << 8) + c;
        valb += 8;
        while (valb >= 0) {
            ret.push_back(b64_chars[(val >> valb) & 0x3F]);
            valb -= 6;
        }
    }
    if (valb > -6) ret.push_back(b64_chars[((val << 8) >> (valb + 8)) & 0x3F]);
    while (ret.size() % 4) ret.push_back('=');
    return ret;
}

std::string base64_decode(const std::string& input) {
    std::vector<int> T(256, -1);
    for (int i = 0; i < 64; i++) T[b64_chars[i]] = i;

    std::string output;
    int val = 0, valb = -8;
    for (uint8_t c : input) {
        if (T[c] == -1) break;
        val = (val << 6) + T[c];
        valb += 6;
        if (valb >= 0) {
            output.push_back(char((val >> valb) & 0xFF));
            valb -= 8;
        }
    }
    return output;
}

bool c_authorization::LoadCredentialsFromFile(std::string& login, std::string& password, std::string& uid) {
    // Отключено: не грузим из файла, просто фейковые данные и true
    login = "fake_user";
    password = "fake_pass";
    uid = "fake_uid";
    debug_t(XOR_STR("[✔] FAKE DATA LOADED (auth off)\n"));
    return true;
}
bool c_authorization::LoadCredentials(std::string& login, std::string& password, std::string& uid) {
    // Отключено: не грузим из реестра, фейковые данные
    login = "fake_user";
    password = "fake_pass";
    uid = "fake_uid";
    debug_t(XOR_STR("FAKE auth loaded (auth off)\n"));
    return true;
}
bool c_authorization::CheckCredentialsMatch() {
    // Отключено: всегда true, без проверок
    username = "fake_user";
    password = "fake_pass";
    uid = "fake_uid";
    debug_t(XOR_STR("Credentials match (auth off)\n"));
    return true;
}
inline std::string download_url(const std::string& url) {
    std::string result;

    HINTERNET hInternet = InternetOpenA("download", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
    if (!hInternet)
        return "";

    HINTERNET hUrl = InternetOpenUrlA(
        hInternet,
        url.c_str(),
        NULL,
        0,
        INTERNET_FLAG_RELOAD | INTERNET_FLAG_NO_CACHE_WRITE | INTERNET_FLAG_PRAGMA_NOCACHE,
        0
    );

    if (!hUrl) {
        InternetCloseHandle(hInternet);
        return "";
    }

    char buffer[4096];
    DWORD bytesRead;

    while (InternetReadFile(hUrl, buffer, sizeof(buffer) - 1, &bytesRead) && bytesRead > 0) {
        buffer[bytesRead] = 0;
        result.append(buffer, bytesRead);
    }

    InternetCloseHandle(hUrl);
    InternetCloseHandle(hInternet);

    return result;
}
void c_authorization::send_identity(const std::string& steamid, const std::string& nickname) {
    // Отключено: не шлём ничего на сервер
    // std::async(std::launch::async, [steamid, nickname]() { ... });
    debug_t(XOR_STR("Sync skipped (auth off)\n"));
}
std::future<std::unordered_map<std::string, std::string>> c_authorization::load_known_players() {
    return std::async(std::launch::async, []() {
        std::unordered_map<std::string, std::string> result;
        // Отключено: не грузим JSON с сервера
        // std::string json_data = download_url("https://furryware.pw/players.json");
        // ... parsing ...
        debug_t(XOR_STR("[+] Known players skipped (auth off)\n"));
        return result;  // пустой мап
        });
}