//
// Created by jlhar on 3/20/2026.
//
#include "DebugModal.h"

namespace ui
{
    std::string GetWritableBasePath()
{
    return std::string (GetAndroidApp()->activity->internalDataPath);
}

std::string GetResumeMapPath()
{
    return GetWritableBasePath() + "/current_work_map.txt";
}

DebugModalResult ShowDebugModal(bool &request_open)
  {


        DebugModalResult result = DebugModalResult::None;

        // OpenPopup() should not be called every frame forever.
        if (request_open) {
            ImGui::OpenPopup("MessageBox");
            request_open = false;
        }

        ImGuiIO &io = ImGui::GetIO();

        const ImVec2 modal_size(io.DisplaySize.x * 0.80f, io.DisplaySize.y * 0.80f);
        const ImVec2 modal_center(io.DisplaySize.x * 0.50f, io.DisplaySize.y * 0.50f);

        ImGui::SetNextWindowPos(modal_center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
        ImGui::SetNextWindowSize(modal_size, ImGuiCond_Appearing);
        ImGui::SetNextWindowBgAlpha(0.88f);

        ImGui::PushStyleColor(ImGuiCol_PopupBg, ImVec4(0.62f, 0.80f, 1.00f,
                                                       0.88f));   // light blue, slightly transparent
        ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.85f, 0.93f, 1.00f, 1.00f));
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 18.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 2.0f);

        const ImGuiWindowFlags flags =
                ImGuiWindowFlags_NoMove |
                ImGuiWindowFlags_NoResize |
                ImGuiWindowFlags_NoCollapse |
                ImGuiWindowFlags_NoSavedSettings;

        // nullptr = no close X button from p_open
        if (ImGui::BeginPopupModal("MessageBox", nullptr, flags)) {
            const ImGuiStyle &style = ImGui::GetStyle();

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

            ImGui::TextWrapped(
                    "Put your message, warning, tutorial text, or confirmation text in here.");
            ImGui::TextWrapped(
                    "This modal stays centered, fixed-size, and cannot be resized or collapsed.");
            ImGui::EndChild();

            // Bottom-left: Back
            ImGui::SetCursorPosY(ImGui::GetWindowHeight() - style.WindowPadding.y - button_h);

            ImGui::SetCursorPosX(style.WindowPadding.x);
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.75f, 0.18f, 0.18f, 1.0f));
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.86f, 0.24f, 0.24f, 1.0f));
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.60f, 0.12f, 0.12f, 1.0f));
            if (ImGui::Button("Back", ImVec2(button_w, button_h))) {
                result = DebugModalResult::Back;
                ImGui::CloseCurrentPopup();
            }
            ImGui::PopStyleColor(3);

            // Bottom-right: Continue
            ImGui::SetCursorPosY(ImGui::GetWindowHeight() - (style.WindowPadding.y) - button_h);
            ImGui::SetCursorPosX(ImGui::GetWindowWidth() - (style.WindowPadding.x) - button_w);
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.18f, 0.68f, 0.22f, 1.0f));
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.24f, 0.80f, 0.28f, 1.0f));
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.12f, 0.56f, 0.16f, 1.0f));
            if (ImGui::Button("Continue", ImVec2(button_w, button_h))) {
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

}