//
// Created by jlhar on 3/15/2026.
//

#ifndef RAYMOB1_GAMESTATEMGR_H
#define RAYMOB1_GAMESTATEMGR_H

#include <unordered_map>
#include <memory>
#include <queue>
#include <stack>
#include <utility>
#include <string>
#include "Globs.h"
#include "Enums.h"
class Game;
class GameState;

class GameStateMgr {
    Game* mGame;

    friend class GameState;
    std::unordered_map<gstate, spstate> mGameStateMap;
    std::stack<weak> mHelperStack;
    std::stack<weak> mStateStack;
    weak mStateInWait;
    bool mStackNextState;
    weak mClearStopState;
    weak mClearStopStateInWait;


    bool isStateWaiting();
    bool existsClearStopState();
    bool isClearStopStateWaiting();



public:
    GameStateMgr() = delete;
    GameStateMgr(Game& game);
    ~GameStateMgr();

    void Input();
    void Update(float deltaTime);
    void Render();
    void RenderUI();


    Game& getGame();
    void readyUpState(gstate state, bool stackIt = false, bool updateClearStop = true, gstate nextClearState = gstate::None);


};


#endif //RAYMOB1_GAMESTATEMGR_H
