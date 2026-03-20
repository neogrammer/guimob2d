//
// Created by jlhar on 3/15/2026.
//

#include "TitleState.h"
#include "GameStateMgr.h"
#include "Game.h"
#include "imgui/imgui.h"

TitleState::TitleState(GameStateMgr& gStateMgr)
 : GameState{gStateMgr}
{

}

TitleState::~TitleState()
{

}

void TitleState::Input()
{
    auto& touch = getGStateMgr().getGame().getTouch();

    if (!touch.isPointerBlockedByUi() && touch.primaryPressed())
    {
       // do nothing
    }
}

void TitleState::Update(float deltaTime)
{
    getGStateMgr().getGame().getSoundMgr().update();
}

void TitleState::drawMenuPanel()
{
    ImGuiViewport* vp = ImGui::GetMainViewport();

    const float panelW = 360.0f;
    const float panelH = 220.0f;

    const ImVec2 panelPos(
            vp->WorkPos.x + (vp->WorkSize.x - panelW) * 0.5f,
            vp->WorkPos.y + vp->WorkSize.y * 0.62f
    );

    ImGui::SetNextWindowPos(panelPos, ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(panelW, panelH), ImGuiCond_Always);
    ImGui::SetNextWindowBgAlpha(0.45f);

    ImGuiWindowFlags flags =
            ImGuiWindowFlags_NoTitleBar |
            ImGuiWindowFlags_NoResize |
            ImGuiWindowFlags_NoMove |
            ImGuiWindowFlags_NoCollapse |
            ImGuiWindowFlags_NoSavedSettings;

    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 18.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(20.0f, 20.0f));
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(12.0f, 14.0f));
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 12.0f);

    if (ImGui::Begin("TitleMenuPanel", nullptr, flags))
    {
        ImGui::PushStyleColor(ImGuiCol_Button,        ImVec4(0.18f, 0.18f, 0.22f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.28f, 0.28f, 0.35f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive,  ImVec4(0.12f, 0.45f, 0.85f, 1.0f));
        const float buttonW = -1.0f;
        const float buttonH = 48.0f;

        if (ImGui::Button("Single Player", ImVec2(buttonW, buttonH)))
        {
            getGStateMgr().getGame().getSoundMgr().playSfx("menu_confirm");
            getGStateMgr().readyUpState(gstate::Play);
        }

        if (ImGui::Button("Online Battle", ImVec2(buttonW, buttonH)))
        {
            getGStateMgr().getGame().getSoundMgr().playSfx("menu_confirm");
            TraceLog(LOG_INFO, "Online Battle pressed");
            // later:
            // getGStateMgr().readyUpState(gstate::OnlineBattle);
        }


        if (ImGui::Button("Leaderboards", ImVec2(buttonW, buttonH)))
        {
            getGStateMgr().getGame().getSoundMgr().playSfx("menu_confirm");
            TraceLog(LOG_INFO, "Leaderboards pressed");
            // later:
            // getGStateMgr().readyUpState(gstate::Leaderboards);
        }
    }
    ImGui::PopStyleColor(3);
    ImGui::PopStyleVar(4);
    ImGui::End();


}

void TitleState::Render()
{
    auto& textures =getGStateMgr().getGame().getTextures();

    const auto bgIt = getGStateMgr().getGame().getTextures().find("title");

    if (bgIt != textures.end() && bgIt->second.id != 0) {
        const Texture2D &bg = bgIt->second;
        Rectangle src{0.0f, 0.0f, static_cast<float>(bg.width), static_cast<float>(bg.height)};
        Rectangle dst{0.0f, 0.0f, static_cast<float>(glb::WW), static_cast<float>(glb::WH)};
        DrawTexturePro(bg, src, dst, {0.0f, 0.0f}, 0.0f, WHITE);
    }


}

void TitleState::Enter() {
    GameState::Enter();
//    auto& sndmgr = getGStateMgr().getGame().getSoundMgr();
//    sndmgr.loadTag("gameplay");
//    sndmgr.playMusic("gameplay_music");
    auto& sndmgr = getGStateMgr().getGame().getSoundMgr();
    sndmgr.loadTag("title");
    sndmgr.playMusic("title_music");


    auto& texs =getGStateMgr().getGame().getTextures();

    getGStateMgr().getGame().getTextures().emplace("title", LoadTexture("textures/register_rush_title.png"));

    TraceLog(LOG_INFO, "paddle id=%u w=%d h=%d", texs["title"].id, texs["title"].width, texs["title"].height);

}

void TitleState::Leave() {
    GameState::Leave();
    auto& sndmgr = getGStateMgr().getGame().getSoundMgr();
    sndmgr.loadTag("title");
    sndmgr.stopMusic("title_music");
}


void TitleState::RenderUI()
{
    drawMenuPanel();
}
