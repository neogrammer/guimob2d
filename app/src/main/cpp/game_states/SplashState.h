//
// Created by jlhar on 3/15/2026.
//

#ifndef RAYMOB1_SPLASHSTATE_H
#define RAYMOB1_SPLASHSTATE_H

#include "../GameState.h"

class SplashState : public GameState {
public:
    void Enter() override final;
    void Leave() override final;

    SplashState() = delete;
    explicit SplashState(GameStateMgr& gStateMgr);
    ~SplashState() override;

    void Input() override final;
    void Update(float deltaTime) override final;
    void Render() override final;
};


#endif //RAYMOB1_SPLASHSTATE_H
