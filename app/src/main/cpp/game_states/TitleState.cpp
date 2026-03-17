//
// Created by jlhar on 3/15/2026.
//

#include "TitleState.h"
#include "GameStateMgr.h"
#include "Game.h"

TitleState::TitleState(GameStateMgr& gStateMgr)
 : GameState{gStateMgr}
{

}

TitleState::~TitleState()
{

}

void TitleState::Input()
{

}

void TitleState::Update(float deltaTime)
{
    getGStateMgr().getGame().getSoundMgr().update();
}

void TitleState::Render()
{

}

void TitleState::Enter() {
    GameState::Enter();
    auto& sndmgr = getGStateMgr().getGame().getSoundMgr();
    sndmgr.loadTag("gameplay");
    sndmgr.playMusic("gameplay_music");

}

void TitleState::Leave() {
    GameState::Leave();
    auto& sndmgr = getGStateMgr().getGame().getSoundMgr();
    sndmgr.loadTag("gameplay");
    sndmgr.stopMusic("gameplay_music");
}
