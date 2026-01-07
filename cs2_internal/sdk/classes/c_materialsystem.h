#pragma once

#include <cstdint>
#include "../../includes.h"
#include "c_agregatescene.h"

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

struct MaterialParamater 
{
	vec4_t vecValue; // 0x0
	void* textureValue; // 0x10
	char pad001[0x10]; // 0x18
	const char* parameterName; // 0x28
	const char* textValue; // 0x30
	int64_t intValue; // 0x38
};

// idk
struct MaterialKeyVar_t
{
	std::uint64_t uKey;
	const char* szName;

	MaterialKeyVar_t(std::uint64_t uKey, const char* szName) : uKey(uKey), szName(szName) { }

	MaterialKeyVar_t(const char* szName, bool bShouldFindKey = false) : szName(szName)
	{
		uKey = bShouldFindKey ? FindKey(szName) : 0x0;
	}

	std::uint64_t FindKey(const char* szName)
	{
		using fnFindKeyVar = std::uint64_t(__fastcall*)(const char*, unsigned int, int);
		static auto oFindKeyVar = reinterpret_cast<fnFindKeyVar>(utils::find_pattern(g_modules.particle, "48 89 5C 24 ? 57 48 81 EC ? ? ? ? 33 C0 8B DA"));

#ifdef CS_PARANOID
		CS_ASSERT(oFindKeyVar != nullptr);
#endif

		// idk those enum flags, just saw it called like that soooo yea
		return oFindKeyVar(szName, 0x12, 0x31415926);
	}
};

class CObjectInfo
{
	MEM_PAD(0xB0);
	int nId;
};

class CSceneAnimatableObject
{
	MEM_PAD(0xB8);
	c_base_handle hOwner;
};

// the naming is incorrect but i dont care atm
class CMeshData
{
public:
	void SetShaderType(const char* szShaderName)
	{
		// @ida: #STR: shader, spritecard.vfx
		using fnSetMaterialShaderType = void(__fastcall*)(void*, MaterialKeyVar_t, const char*, int);
		static auto oSetMaterialShaderType = reinterpret_cast<fnSetMaterialShaderType>(utils::find_pattern(g_modules.particle, "48 89 5C 24 ? 48 89 6C 24 ? 56 57 41 54 41 56 41 57 48 83 EC ? 0F B6 01 45 33 E4 8B 32 45 0F B6 F1 48 8B 6A ? 4D 8B F8 C0 E8 ? 48 8B D9 24 ? 3C ? 74 ? 44 88 64 24 ? 45 33 C9 44 89 64 24 ? 41 B0 ? B2 ? 4C 89 64 24 ? E8 ? ? ? ? 0F B6 03 C0 E8 ? 24 ? 3C ? 75 ? 48 8B 7B ? EB ? 49 8B FC 4C 8D 84 24 ? ? ? ? C7 84 24 ? ? ? ? ? ? ? ? 48 8D 54 24 ? 89 74 24 ? 48 8B CF 48 89 6C 24 ? E8 ? ? ? ? 44 8B C8 83 F8 ? 75 ? 45 33 C9 89 74 24 ? 4C 8D 44 24 ? 48 89 6C 24 ? 48 8B D3 48 8B CF E8 ? ? ? ? 44 8B C8 0F B6 0B C0 E9 ? 80 E1 ? 80 F9 ? 75 ? 48 8B 4B ? EB ? 49 8B CC 4C 8D 41 ? 44 38 61 ? 74 ? 4D 8B 00 8B 41 ? 48 8D 1D"));

#ifdef CS_PARANOID
		CS_ASSERT(oSetMaterialShaderType != nullptr);
#endif

		MaterialKeyVar_t shaderVar(0x162C1777, "shader");

		oSetMaterialShaderType(this, shaderVar, szShaderName, 0x18);
	}

	void SetMaterialFunction(const char* szFunctionName, int nValue)
	{
		using fnSetMaterialFunction = void(__fastcall*)(void*, MaterialKeyVar_t, int, int);
		static auto oSetMaterialFunction = reinterpret_cast<fnSetMaterialFunction>(utils::find_pattern(g_modules.particle, "48 89 5C 24 ? 48 89 6C 24 ? 56 57 41 54 41 56 41 57 48 83 EC ? 0F B6 01 45 33 E4 8B 32 45 0F B6 F1 48 8B 6A ? 48 8B D9"));

#ifdef CS_PARANOID
		CS_ASSERT(oSetMaterialFunction != nullptr);
#endif

		MaterialKeyVar_t functionVar(szFunctionName, true);

		oSetMaterialFunction(this, functionVar, nValue, 0x18);
	}

	MEM_PAD(0x18); // 0x0
	CSceneAnimatableObject* pSceneAnimatableObject; // 0x18
	CMaterial2* pMaterial; // 0x20
	MEM_PAD(0x28); // 0x28
	c_color colValue; // 0x50
	MEM_PAD(0x14); // 0x48
	CObjectInfo* pObjectInfo; // 0x48
	MEM_PAD(0x4); // 0x52		
	std::uint32_t m_kakieto_flagi{ };
};