#pragma once

#include <Windows.h>
#include <unordered_map>
#include <memory>

struct c_modules
{
	HMODULE client{};
	HMODULE engine2{};
	HMODULE schemasystem{};
	HMODULE tier0{};
	HMODULE navsystem{};
	HMODULE rendersystem{};
	HMODULE localize{};
	HMODULE scenesystem{};
	HMODULE materialsystem2{};
	HMODULE resourcesystem{};
	HMODULE particle{};
	HMODULE game_overlay_renderer64_dll{};

	HMODULE input_system{};
	HMODULE animation_system{};

	void parse();
};

inline c_modules g_modules{};