//
// Created by jlhar on 3/10/2026.
//

#ifndef RAYMOB1_IMGUI_RAYMOB_H
#define RAYMOB1_IMGUI_RAYMOB_H
struct ImFont;
void ImGuiRaymob_Init();
void ImGuiRaymob_NewFrame();
void ImGuiRaymob_Render();
void ImGuiRaymob_Shutdown();

ImFont* ImGuiRaymob_GetUIFont();
#endif //RAYMOB1_IMGUI_RAYMOB_H
