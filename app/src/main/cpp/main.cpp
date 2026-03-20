#include "raymob.h"
#include <string>
#include "rlgl.h"
#include "imgui/imgui.h"
#include "imgui_raymob.h"
#include "WorkMapEditor.h"
#include "Game.h"
#include "Globs.h"

int main(void)
{
    Game gGame{};
    InitWindow(1280, 800, "Raymob + ImGui");
    SetTargetFPS(60);
    glb::WW = (unsigned)GetScreenWidth();
    glb::WH = (unsigned)GetScreenHeight();
    gGame.Initialize();
    ImGuiRaymob_Init();

    while (!WindowShouldClose())
    {
        const float dt = GetFrameTime();
        gGame.Input();
        gGame.Update(dt);

        BeginDrawing();
        ClearBackground(BLUE);
        gGame.Render();
        // Flush queued raylib draw calls before ImGui GL rendering
        rlDrawRenderBatchActive();
        ImGuiRaymob_NewFrame();

        ImFont* uiFont = ImGuiRaymob_GetUIFont();
        if (uiFont) ImGui::PushFont(uiFont);
        gGame.RenderUI();
        if (uiFont) ImGui::PopFont();
        ImGuiRaymob_Render();
        EndDrawing();
    }

    gGame.Shutdown();
    ImGuiRaymob_Shutdown();
    CloseWindow();
    return 0;
}


