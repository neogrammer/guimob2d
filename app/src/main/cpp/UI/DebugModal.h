//
// Created by jlhar on 3/20/2026.
//

#ifndef RAYMOB1_DEBUGMODAL_H
#define RAYMOB1_DEBUGMODAL_H
#include <string>
#include "../WorkMapEditor.h"
#include "raymob.h"

namespace ui {
    enum class DebugModalResult {
        None,
        Back,
        Continue
    };
    extern std::string GetWritableBasePath();
    extern std::string GetResumeMapPath();
    extern DebugModalResult ShowDebugModal(bool &request_open);
}
//std::string savePath{};
#endif //RAYMOB1_DEBUGMODAL_H




// Texture2D tileset = LoadTexture("textures/tilesets/spritesheet1.png");

//    WorkMapEditor mapEditor;
//    mapEditor.LoadFromFile("maps/work_map.txt");
//    android_app* app = GetAndroidApp();
//    savePath = std::string(app->activity->internalDataPath) + "/work_map.txt";
//    mapEditor.SetSavePath("work_map_saved.txt");

//  WorkMapEditor mapEditor;

//std::string resumeMapPath = GetResumeMapPath();
//  TraceLog(LOG_INFO, "Resume map path: %s", resumeMapPath.c_str());
//   TraceLog(LOG_INFO, "Resume exists: %s", FileExists(resumeMapPath.c_str()) ? "YES" : "NO");
// save to the same file we want to resume from
//   mapEditor.SetSavePath(resumeMapPath);

// load resumed file if it exists, otherwise load default asset
///   if (FileExists(resumeMapPath.c_str()))
//   {
//mapEditor.LoadFromFile("maps/work_map.txt");
//      mapEditor.LoadFromFile(resumeMapPath.c_str());
//  }
//  else
// {
//       mapEditor.LoadFromFile("maps/work_map.txt");
//  }
//  android_app* app = GetAndroidApp();
//    savePath = std::string(app->activity->internalDataPath) + "/work_map.txt";
//    mapEditor.SetSavePath("work_map_saved.txt");

// Simple Android touch control:
// touching upper half moves paddle up, lower half moves paddle down
//        float moveY = 0.0f;
//
//        if (GetTouchPointCount() > 0)
//        {
//            Vector2 touch = GetTouchPosition(0);
//            moveY = (touch.y < (float)GetScreenHeight() * 0.5f) ? -1.0f : 1.0f;
//        }
//
//        gGame.SetPlayerMoveIntent(0.0f, moveY);
//        BeginDrawing();
//        ClearBackground(BLUE);
//
//        ImGuiRaymob_NewFrame();
//
//        mapEditor.HandlePanInput();
//        mapEditor.HandlePaintInput();
//
//        mapEditor.DrawMap();
//
//        // Flush queued raylib draw calls before ImGui GL rendering
//        rlDrawRenderBatchActive();
//
//        static bool open_debug_modal = false;
//
//        mapEditor.DrawEditorUI();
//
//        DebugModalResult modal_result = ShowDebugModal(open_debug_modal);
//
//        if (modal_result == DebugModalResult::Back)
//        {
//        }
//
//        if (modal_result == DebugModalResult::Continue)
//        {
//        }
//
//        ImFont* uiFont = ImGuiRaymob_GetUIFont();
//        if (uiFont) ImGui::PushFont(uiFont);
//
//        if (showDemo)
//            ImGui::ShowDemoWindow(&showDemo);
//
//        if (uiFont) ImGui::PopFont();
//
//        ImGuiRaymob_Render();
//
//        EndDrawing();
// UnloadTexture(tileset);