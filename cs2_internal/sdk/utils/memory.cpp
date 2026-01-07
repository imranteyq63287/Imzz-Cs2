#pragma once

#include "memory.h"
#include "utils.h"
#include "../modules/modules.h"

void* FindHudElement(const char* element)
{
    using fn_t = void* (*)(const char* element);

    using fnFindHud = std::uintptr_t* (__fastcall*)(const char* str);
    static fnFindHud FindHud = reinterpret_cast<fnFindHud>(utils::find_pattern(g_modules.client, "4C 8B DC 53 48 83 EC ? 48 8B 05"));

    return FindHud(element);
}

void c_memory::sent_message_to_local_chat(const char* msg)
{
    using fn_t = void(*)(void* chat, int unk, const char* msg);

    static fn_t fn = (fn_t)utils::find_pattern(g_modules.client, "4C 89 44 24 ? 4C 89 4C 24 ? 53 B8");
    void* chat = FindHudElement("HudChatDelegate");
  
    fn(chat, -1, msg);
}
