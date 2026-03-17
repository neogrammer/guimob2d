//
// Created by jlhar on 3/15/2026.
//

#ifndef RAYMOB1_TITLESTATE_H
#define RAYMOB1_TITLESTATE_H

#include "../GameState.h"
class Game;

class TitleState : public GameState {
public:
    void Enter() override final;
    void Leave() override final;

    TitleState() = delete;
    explicit TitleState(GameStateMgr& gStateMgr);
    ~TitleState() override;

    void Input() override final;
    void Update(float deltaTime) override final;
    void Render() override final;
};


#endif //RAYMOB1_TITLESTATE_H
