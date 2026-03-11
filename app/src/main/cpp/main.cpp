///*******************************************************************************************
//*
//*   raylib [core] example - Basic window
//*
//*   Welcome to raylib!
//*
//*   To test examples, just press Shift+F10 for Android Studio.
//*
//*   raylib official webpage: www.raylib.com
//*
//*   Enjoy using raylib. :)
//*
//*   Example licensed under an unmodified zlib/libpng license, which is an OSI-certified,
//*   BSD-like license that allows static linking with closed source software
//*
//*   Copyright (c) 2013-2023 Ramon Santamaria (@raysan5) and reviewed by Victor Le Juez
//*
//********************************************************************************************/
//
//#include "raymob.h" // This header can replace 'raylib.h' and includes additional functions related to Android.
//
////------------------------------------------------------------------------------------
//// Program main entry point
////------------------------------------------------------------------------------------
//int main(void)
//{
//
//    int screenWidth  = GetScreenWidth();
//    int screenHeight = GetScreenHeight();
//    // Initialization
//    //--------------------------------------------------------------------------------------
//    InitWindow(screenWidth, screenHeight, "raylib [core] example - basic window");
//    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
//    //--------------------------------------------------------------------------------------
//
//    // Main game loop
//    while (!WindowShouldClose())    // Detect window close button or ESC key
//    {
//        // Update
//        //----------------------------------------------------------------------------------
//        // TODO: Update your variables here
//        //----------------------------------------------------------------------------------
//
//        // Draw
//        //----------------------------------------------------------------------------------
//        BeginDrawing();
//
//        ClearBackground(RAYWHITE);
//
//        DrawText("Congrats! You created your first window!", 190, 200, 20, LIGHTGRAY);
//
//        EndDrawing();
//        //----------------------------------------------------------------------------------
//    }
//
//    // De-Initialization
//    //--------------------------------------------------------------------------------------
//    CloseWindow();        // Close window and OpenGL context
//    //--------------------------------------------------------------------------------------
//
//    return 0;
//}

//
//#include "raymob.h"
//
//int main(void)
//{
//    InitWindow(1280, 720, "Raymob test");
//    SetTargetFPS(60);
//
//
//
//   // bool showDemo = true;
//    float sliderValue = 0.5f;
//
//
//    while (!WindowShouldClose())
//    {
//        BeginDrawing();
//        ClearBackground(BLUE);
//
//
//        DrawText("RAYMOB RENDER TEST", 40, 80, 40, WHITE);
//        DrawFPS(40, 140);
//
//    EndDrawing();
//    }
//
//    CloseWindow();
//    return 0;
//}

#include "raymob.h"
#include <string>
#include "rlgl.h"
#include "imgui.h"
#include "imgui_raymob.h"
#include "WorkMapEditor.h"

static std::string GetWritableBasePath()
{
    return std::string(GetAndroidApp()->activity->internalDataPath);
}

static std::string GetResumeMapPath()
{
    return GetWritableBasePath() + "/current_work_map.txt";
}

enum class DebugModalResult
{
    None,
    Back,
    Continue
};
static DebugModalResult ShowDebugModal(bool& request_open)
{
    DebugModalResult result = DebugModalResult::None;

    // OpenPopup() should not be called every frame forever.
    if (request_open)
    {
        ImGui::OpenPopup("MessageBox");
        request_open = false;
    }

    ImGuiIO& io = ImGui::GetIO();

    const ImVec2 modal_size(io.DisplaySize.x * 0.80f, io.DisplaySize.y * 0.80f);
    const ImVec2 modal_center(io.DisplaySize.x * 0.50f, io.DisplaySize.y * 0.50f);

    ImGui::SetNextWindowPos(modal_center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
    ImGui::SetNextWindowSize(modal_size, ImGuiCond_Appearing);
    ImGui::SetNextWindowBgAlpha(0.88f);

    ImGui::PushStyleColor(ImGuiCol_PopupBg, ImVec4(0.62f, 0.80f, 1.00f, 0.88f));   // light blue, slightly transparent
    ImGui::PushStyleColor(ImGuiCol_Border,  ImVec4(0.85f, 0.93f, 1.00f, 1.00f));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 18.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 2.0f);

    const ImGuiWindowFlags flags =
            ImGuiWindowFlags_NoMove |
            ImGuiWindowFlags_NoResize |
            ImGuiWindowFlags_NoCollapse |
            ImGuiWindowFlags_NoSavedSettings;

    // nullptr = no close X button from p_open
    if (ImGui::BeginPopupModal("MessageBox", nullptr, flags))
    {
        const ImGuiStyle& style = ImGui::GetStyle();

        const float button_w = 280.0f;
        const float button_h = 70.0f;

        // Reserve bottom area for the buttons
        const float footer_h = button_h + style.WindowPadding.y;
        const float body_h = ImGui::GetContentRegionAvail().y - footer_h;

        ImGui::BeginChild("DebugModalBody", ImVec2(0.0f, body_h), ImGuiChildFlags_None);
        ImGui::Spacing();
        ImGui::TextWrapped("Debug modal message goes here.");
        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Spacing();

        ImGui::TextWrapped("Put your message, warning, tutorial text, or confirmation text in here.");
        ImGui::TextWrapped("This modal stays centered, fixed-size, and cannot be resized or collapsed.");
        ImGui::EndChild();

        // Bottom-left: Back
        ImGui::SetCursorPosY(ImGui::GetWindowHeight() - style.WindowPadding.y - button_h);

        ImGui::SetCursorPosX(style.WindowPadding.x);
        ImGui::PushStyleColor(ImGuiCol_Button,        ImVec4(0.75f, 0.18f, 0.18f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.86f, 0.24f, 0.24f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive,  ImVec4(0.60f, 0.12f, 0.12f, 1.0f));
        if (ImGui::Button("Back", ImVec2(button_w, button_h)))
        {
            result = DebugModalResult::Back;
            ImGui::CloseCurrentPopup();
        }
        ImGui::PopStyleColor(3);

        // Bottom-right: Continue
        ImGui::SetCursorPosY(ImGui::GetWindowHeight() - (style.WindowPadding.y) - button_h);
        ImGui::SetCursorPosX(ImGui::GetWindowWidth() - (style.WindowPadding.x) - button_w);
        ImGui::PushStyleColor(ImGuiCol_Button,        ImVec4(0.18f, 0.68f, 0.22f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.24f, 0.80f, 0.28f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive,  ImVec4(0.12f, 0.56f, 0.16f, 1.0f));
        if (ImGui::Button("Continue", ImVec2(button_w, button_h)))
        {
            result = DebugModalResult::Continue;
            ImGui::CloseCurrentPopup();
        }
        ImGui::PopStyleColor(3);

        ImGui::EndPopup();
    }

    ImGui::PopStyleVar(2);
    ImGui::PopStyleColor(2);

    return result;
}

std::string savePath{};

int main(void)
{
    InitWindow(1280, 800, "Raymob + ImGui");
    SetTargetFPS(60);

    Texture2D tileset = LoadTexture("textures/tilesets/tileset1.png");

    ImGuiRaymob_Init();

    bool showDemo = false;
    float slider = 0.5f;

//    WorkMapEditor mapEditor;
//    mapEditor.LoadFromFile("maps/work_map.txt");
//    android_app* app = GetAndroidApp();
//    savePath = std::string(app->activity->internalDataPath) + "/work_map.txt";
//    mapEditor.SetSavePath("work_map_saved.txt");

    WorkMapEditor mapEditor;

    std::string resumeMapPath = GetResumeMapPath();
    TraceLog(LOG_INFO, "Resume map path: %s", resumeMapPath.c_str());
    TraceLog(LOG_INFO, "Resume exists: %s", FileExists(resumeMapPath.c_str()) ? "YES" : "NO");
// save to the same file we want to resume from
    mapEditor.SetSavePath(resumeMapPath);

// load resumed file if it exists, otherwise load default asset
    if (FileExists(resumeMapPath.c_str()))
    {
        //mapEditor.LoadFromFile("maps/work_map.txt");

        mapEditor.LoadFromFile(resumeMapPath.c_str());
    }
    else
    {
        mapEditor.LoadFromFile("maps/work_map.txt");
    }
  //  android_app* app = GetAndroidApp();
//    savePath = std::string(app->activity->internalDataPath) + "/work_map.txt";
//    mapEditor.SetSavePath("work_map_saved.txt");

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(BLUE);

       // DrawText("RAYMOB + IMGUI TEST", 40, 180, 40, WHITE);
       // DrawFPS(40, 240);

        Rectangle src = { 0.0f, 0.0f, 64.0f, 64.0f };

        Rectangle dst = {
                0.f,
                0.f,
                64.0f,
                64.0f
        };

        Vector2 origin = { 0.0f, 0.0f };

//        DrawTexturePro(
//                tileset,
//                src,
//                dst,
//                origin,
//                0.0f,
//                WHITE
//        );

        mapEditor.DrawMap();

        // Flush queued raylib draw calls NOW, before ImGui direct GL rendering
        rlDrawRenderBatchActive();



        static bool open_debug_modal = false;

        ImGuiRaymob_NewFrame();
        mapEditor.DrawEditorUI();
        mapEditor.HandlePaintInput();


        DebugModalResult modal_result = ShowDebugModal(open_debug_modal);

        if (modal_result == DebugModalResult::Back)
        {

        }

        if (modal_result == DebugModalResult::Continue)
        {

        }

        ImFont* uiFont = ImGuiRaymob_GetUIFont();
        if (uiFont) ImGui::PushFont(uiFont);

//        ImGui::Begin("Debug");
//        ImGui::Text("ImGui overlay is alive.");
//        ImGui::SliderFloat("slider", &slider, 0.0f, 1.0f);
//        ImGui::Text("touch the screen and drag");
//        ImGui::End();

        if (showDemo)
            ImGui::ShowDemoWindow(&showDemo);

        if (uiFont) ImGui::PopFont();

        ImGuiRaymob_Render();

        EndDrawing();


    }

    UnloadTexture(tileset);
    ImGuiRaymob_Shutdown();
    CloseWindow();
    return 0;
}