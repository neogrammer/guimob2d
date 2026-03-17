//
// Created by jlhar on 3/15/2026.
//

#ifndef RAYMOB1_GAMESTATE_H
#define RAYMOB1_GAMESTATE_H

#include "raymob.h"
#include "Globs.h"
#include "Enums.h"
class GameStateMgr;

class GameState {
    GameStateMgr* mGStateMgr;
protected:
    Camera2D camera;
public:
    GameState() = delete;
    explicit GameState(GameStateMgr& gStateMgr);
    virtual ~GameState() = 0;

    virtual void Enter();
    virtual void Leave();

    virtual void Input() = 0;
    virtual void Update(float deltaTime) = 0;
    virtual void Render() = 0;


    void readyUpState(gstate state, bool stackIt = false, bool updateClearStop = true, gstate nextClearState = gstate::None);
    GameStateMgr& getGStateMgr();
};


#endif //RAYMOB1_GAMESTATE_H
