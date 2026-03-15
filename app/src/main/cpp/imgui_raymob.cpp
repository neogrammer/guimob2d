//
// Created by jlhar on 3/10/2026.
//
#include "imgui_raymob.h"

#include "raylib.h"
#include "imgui/imgui.h"
#include "imgui/backends/imgui_impl_opengl3.h"
#include <algorithm>
#include <cfloat>

static ImFont* gUiFont = nullptr;
static float g_ImGuiTouchBiasY = 4.0f;
ImFont* ImGuiRaymob_GetUIFont()
{
    return gUiFont;
}

void ImGuiRaymob_Init()
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGuiIO& io = ImGui::GetIO();
    (void)io;

    // Keep first bring-up simple.
    io.IniFilename = nullptr;

    ImGuiStyle& style = ImGui::GetStyle();
    style = ImGuiStyle();          // reset to default style first
    style.ScaleAllSizes(1.0f);     // buttons, padding, rounding, spacing, etc.

#if IMGUI_VERSION_NUM >= 19200
    style.FontScaleDpi = 1.0f;     // current official example path
#else
    ImGui::GetIO().FontGlobalScale = 2.0f;   // older ImGui path
#endif

    ImGui::StyleColorsDark();
    ImGui_ImplOpenGL3_Init("#version 300 es");

    int fontBytes = 0;
    unsigned char* fontData = LoadFileData("fonts/Steel.otf", &fontBytes);

    if (fontData != nullptr && fontBytes > 0)
    {
        ImFontConfig cfg{};
        cfg.FontDataOwnedByAtlas = true;   // default behavior, stated explicitly

        gUiFont = io.Fonts->AddFontFromMemoryTTF(
                fontData,
                fontBytes,
                12.0f,      // start here; try 32 or 40 if needed
                &cfg
        );
    }

    if (gUiFont == nullptr)
    {
        ImFontConfig fallbackCfg{};
        gUiFont = io.Fonts->AddFontDefault(&fallbackCfg);
    }
}



void ImGuiRaymob_NewFrame()
{
    ImGuiIO& io = ImGui::GetIO();
    const float screenW = (float)GetScreenWidth();
    const float screenH = (float)GetScreenHeight();
    const float renderW = (float)GetRenderWidth();
    const float renderH = (float)GetRenderHeight();

    io.DisplaySize = ImVec2(screenW, screenH);
    io.DisplayFramebufferScale = ImVec2(
            (screenW > 0.0f) ? (renderW / screenW) : 1.0f,
            (screenH > 0.0f) ? (renderH / screenH) : 1.0f
    );
    io.DeltaTime = std::max(GetFrameTime(), 1.0f / 1000.0f);



    if (GetTouchPointCount() > 0)
    {
        Vector2 p = GetTouchPosition(0);

        // Global ImGui-only touch correction
        p.y += g_ImGuiTouchBiasY;

        io.AddMousePosEvent(p.x, p.y);
        io.AddMouseButtonEvent(0, true);
    }
    else
    {
        io.AddMouseButtonEvent(0, false);
        io.AddMousePosEvent(-FLT_MAX, -FLT_MAX);
    }

    ImGui_ImplOpenGL3_NewFrame();
    ImGui::NewFrame();

}

void ImGuiRaymob_Render()
{
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void ImGuiRaymob_Shutdown()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui::DestroyContext();
}