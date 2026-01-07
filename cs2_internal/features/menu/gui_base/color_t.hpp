#pragma once
#include "../../../dependencies/imgui/imgui.h"
using namespace ImGui;

struct color_t {

public:
    float r, g, b, a;
    color_t(float r = 0.0f, float g = 0.0f, float b = 0.0f, float a = 1.0f)
        : r(r), g(g), b(b), a(a) {
    }

    inline ImColor to_im_color(float alpha = 1.f, bool use_gl_alpha = true) {

        return ImColor(r, g, b, (a * (use_gl_alpha ? GetStyle().Alpha : 1.f)) * alpha);
    }

    inline ImVec4 to_vec4(float alpha = 1.f, bool use_gl_alpha = true) {

        return ImVec4(r, g, b, (a * (use_gl_alpha ? GetStyle().Alpha : 1.f)) * alpha);
    }

};
