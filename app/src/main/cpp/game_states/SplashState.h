//
// Created by jlhar on 3/15/2026.
//

#ifndef RAYMOB1_SPLASHSTATE_H
#define RAYMOB1_SPLASHSTATE_H

#include "../GameState.h"
#include "raylib.h"

class SplashState : public GameState {
    enum class Phase
    {
        FadeIn,
        Hold,
        FadeOut,
        Done
    };
    void goToTitle();

    Texture2D mLogo{};
    Phase mPhase{ Phase::FadeIn };

    float mAlpha{ 0.0f };
    float mTimer{ 0.0f };

    float mFadeInDuration{ 0.75f };
    float mHoldDuration{ 1.5f };
    float mFadeOutDuration{ 0.75f };

    bool mLeaving{ false };

public:
    void Enter() override final;
    void Leave() override final;

    SplashState() = delete;
    explicit SplashState(GameStateMgr& gStateMgr);
    ~SplashState() override;

    void Input() override final;
    void Update(float deltaTime) override final;
    void Render() override final;
    void RenderUI() override final;
};


#endif //RAYMOB1_SPLASHSTATE_H
