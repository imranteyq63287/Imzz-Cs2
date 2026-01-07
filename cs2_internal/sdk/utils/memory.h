#pragma once

#include <memory>
#include <unordered_map>
#include <string_view>
#include <WTypesbase.h>
#include <iostream>

#define CLIENT_DLL (L"client.dll")
#define ENGINE2_DLL (L"engine2.dll")
#define SCHEMASYSTEM_DLL (L"schemasystem.dll")
#define INPUTSYSTEM_DLL (L"inputsystem.dll")
#define SDL3_DLL (L"SDL3.dll")
#define TIER0_DLL (L"tier0.dll")
#define NAVSYSTEM_DLL (L"navsystem.dll")
#define RENDERSYSTEM_DLL (L"rendersystemdx11.dll")
#define LOCALIZE_DLL (L"localize.dll")
#define DBGHELP_DLL (L"dbghelp.dll")
#define GAMEOVERLAYRENDERER_DLL (L"GameOverlayRenderer64.dll")
#define RESOURCE_HANDLE_UTILS ("ResourceHandleUtils001")
#define PARTICLES_DLL (L"particles.dll")
#define SCENESYSTEM_DLL (L"scenesystem.dll")
#define MATERIAL_SYSTEM2_DLL (L"materialsystem2.dll")
#define MATCHMAKING_DLL (L"matchmaking.dll")
#define RESOURCESYSTEM_DLL (L"resourcesystem.dll")
#define SOUNDSYSTEM_DLL (L"soundsystem.dll")
#define filesystem_stdio_dll (L"filesystem_stdio.dll")

#pragma region memory_definitions
#pragma warning(push)
#pragma warning(disable: 6255)
#define MEM_STACKALLOC(SIZE) _malloca(SIZE)
#pragma warning(pop)
#define MEM_STACKFREE(MEMORY) static_cast<void>(0)
#define _CS_INTERNAL_CONCATENATE(LEFT, RIGHT) LEFT##RIGHT
#define CS_CONCATENATE(LEFT, RIGHT) _CS_INTERNAL_CONCATENATE(LEFT, RIGHT)

#define MEM_PAD(SIZE)										\
private:												\
	char CS_CONCATENATE(pad_0, __COUNTER__)[SIZE]; \
public:

#pragma endregion

class CUtlBuffer;
class CGCClientSystem;
class CCSInventoryManager;
class CInterfaceRegister;

class c_memory
{
public:
	void sent_message_to_local_chat(const char* msg);

    template <typename T = std::uint8_t>
    T* GetAbsoluteAddress(T* pRelativeAddress, int nPreOffset = 0x0, int nPostOffset = 0x0)
    {
        pRelativeAddress += nPreOffset;
        pRelativeAddress += sizeof(std::int32_t) + *reinterpret_cast<std::int32_t*>(pRelativeAddress);
        pRelativeAddress += nPostOffset;
       
        return pRelativeAddress;
    }
};

inline auto g_memory = std::make_unique<c_memory>();
