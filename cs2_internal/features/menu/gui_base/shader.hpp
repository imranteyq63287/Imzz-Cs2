#pragma once

#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#ifdef data
#undef data
#endif

#define IMGUI_DEFINE_MATH_OPERATORS
#include <d3d11.h>
#include <d3dcompiler.h>
#include "../../../dependencies/imgui/imgui.h"


struct float2
{
    float x, y;
};

inline ID3D11ShaderResourceView* effectResourceView;

struct alignas(16) BlurInputBufferT {
    float2 textureSize;
    float blurDirections;
    float blurQuality;
    float blurRadius;
};

namespace shaders {
    void blurBegin(const ImDrawList* drawList, const ImDrawCmd* drawCmd);
    void blurEnd(const ImDrawList* drawList, const ImDrawCmd* drawCmd);
    void createDeviceObjects(IDXGISwapChain* swapChain);
    void drawToScreenUV(ImVec2 pos, ImVec2 size, ImColor color, float rounding, ImDrawFlags flags);
    void applyBlur(float size, float quality, float directions);
    void beginFrame(IDXGISwapChain* swapChain);
}

inline void blur(const ImVec2& pos, const ImVec2& size, ImColor color, float rounding, ImDrawFlags flags, bool background) {
    const ImVec2& scaledPos{ pos };
    const ImVec2& scaledSize{ size };
    const ImVec2& finalPos{ scaledPos - (scaledSize / 2.f) };
    shaders::drawToScreenUV(finalPos, finalPos + scaledSize, color, rounding, flags);
}
inline void renderFrame(IDXGISwapChain* swapChain) {
    shaders::beginFrame(swapChain);

    // применяем блюр к фону
    shaders::applyBlur(20.0f, 8.0f, 8.0f);

    // отрисовываем блюр на весь экран
    shaders::drawToScreenUV(
        ImVec2(0, 0),
        ImGui::GetIO().DisplaySize,
        ImColor(255, 255, 255, 255),
        0.0f,
        0
    );

    // теперь можешь рисовать обычный ImGui интерфейс
    ImGui::Begin("Test Window");
    ImGui::Text("Блюр работает!");
    ImGui::End();
}