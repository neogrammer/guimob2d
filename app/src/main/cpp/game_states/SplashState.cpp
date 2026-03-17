//
// Created by jlhar on 3/15/2026.
//

#include "SplashState.h"
#include "GameStateMgr.h"
#include "Game.h"

SplashState::SplashState(GameStateMgr& gStateMgr)
        : GameState{gStateMgr}
{

}

SplashState::~SplashState()
{

}

void SplashState::Input()
{

}

void SplashState::Update(float deltaTime)
{

}

void SplashState::Render()
{

}

void SplashState::Enter() {
    GameState::Enter();
    auto& sndmgr = getGStateMgr().getGame().getSoundMgr();
//    sndmgr.loadTag("gameplay");
//    sndmgr.playMusic("gameplay_music");
    sndmgr.loadTag("title");
    sndmgr.playMusic("title_music");
}

void SplashState::Leave() {
    GameState::Leave();
    auto& sndmgr = getGStateMgr().getGame().getSoundMgr();
    sndmgr.loadTag("gameplay");
    sndmgr.stopMusic("gameplay_music");
}
