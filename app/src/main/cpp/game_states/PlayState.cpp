//
// Created by jlhar on 3/15/2026.
//

#include "PlayState.h"
#include "GameStateMgr.h"

PlayState::PlayState(GameStateMgr& gStateMgr)
        : GameState{gStateMgr}
        , tilemap{"tset1.tst", "tmap1.map" }
{
}

PlayState::~PlayState()
{

}
void PlayState::Input()
{

}

void PlayState::Update(float deltaTime)
{

}

void PlayState::Render()
{
    tilemap.RenderMap();
}

void PlayState::Enter() {
    GameState::Enter();
}

void PlayState::Leave() {
    GameState::Leave();
}

void PlayState::RenderUI()
{

}


