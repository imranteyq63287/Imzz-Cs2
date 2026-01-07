#pragma once

#define NOMINMAX
#define IMGUI_DEFINE_MATH_OPERATORS

#include <Windows.h>
#include <cstdint>
#include <cstdio>
#include <intrin.h>
#include <xmmintrin.h>
#include <array>
#include <vector>
#include <algorithm>
#include <cctype>
#include <thread>
#include <memory>
#include <sstream>
#include <unordered_map>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <deque>
#include <functional>
#include <map>
#include <shlobj.h>
#include <filesystem>
#include <streambuf>
#include <mutex>
#include <strsafe.h>
#include <Psapi.h>
#include <dxgi.h>
#include <d3d11.h>
#include <DirectXMath.h>
#include "chrono"
#include <utility>

#include "dependencies/xor.h"
#include "dependencies/fnv1a.h"
#include "dependencies/minhook/MinHook.h"
#include "dependencies/imgui/imgui.h"
#include "dependencies/imgui/imgui_impl_dx11.h"
#include "dependencies/imgui/imgui_impl_win32.h"

#include "context.h"

#include <cmath>

#define WHITE_COLOR FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED
#define BLUE_COLOR FOREGROUND_INTENSITY | FOREGROUND_BLUE
#define RED_COLOR FOREGROUND_INTENSITY | FOREGROUND_RED
#define GREEN_COLOR FOREGROUND_INTENSITY | FOREGROUND_GREEN

#ifdef _DEBUG
#define debug_text(color, text, ...) SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color); printf(text, __VA_ARGS__)
#else
#define debug_text
#endif