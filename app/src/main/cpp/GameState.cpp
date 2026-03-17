//
// Created by jlhar on 3/15/2026.
//

#include "GameState.h"
#include "GameStateMgr.h"

GameState::GameState(GameStateMgr& gStateMgr)
  : mGStateMgr{&gStateMgr}
{
  camera = {};
  camera.target = {(float)GetScreenWidth() / 2.f,(float)GetScreenHeight() / 2.f};
  camera.offset = {0.f,0.f};
  camera.rotation = 0.0f;
  camera.zoom = 1.0f;
}

GameState::~GameState()
{

}

void GameState::Enter()
{

}

void GameState::Leave()
{

}

void GameState::readyUpState(gstate state, bool stackIt, bool updateClearStop, gstate nextClearState)
{
  mGStateMgr->readyUpState(state, stackIt, updateClearStop, nextClearState);
}

GameStateMgr &GameState::getGStateMgr() {
    return *mGStateMgr;
}
