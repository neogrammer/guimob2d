//
// Created by jlhar on 3/15/2026.
//

#include "SplashState.h"
#include "GameStateMgr.h"
#include "Game.h"
#include "input/TouchInput.h"

SplashState::SplashState(GameStateMgr& gStateMgr)
        : GameState{gStateMgr}
{
    TraceLog(LOG_INFO, "HIT SplashState::Update");
}

SplashState::~SplashState()
{

}

void SplashState::Input()
{
    auto& touch = getGStateMgr().getGame().getTouch();

    if (touch.primaryPressed())
    {
        getGStateMgr().readyUpState(gstate::Title);
    }
}

void SplashState::Update(float deltaTime)
{
    if (mLeaving)
    {
        return;
    }

    switch (mPhase)
    {




        case Phase::FadeIn:
        {
            mTimer += deltaTime;

            float t = mTimer / mFadeInDuration;
            if (t > 1.0f) t = 1.0f;

            mAlpha = t;

            if (mTimer >= mFadeInDuration)
            {
                mPhase = Phase::Hold;
                mTimer = 0.0f;
                mAlpha = 1.0f;
            }
        } break;

        case Phase::Hold:
        {
            mTimer += deltaTime;
            mAlpha = 1.0f;

            if (mTimer >= mHoldDuration)
            {
                mPhase = Phase::FadeOut;
                mTimer = 0.0f;
            }
        } break;

        case Phase::FadeOut:
        {
            mTimer += deltaTime;

            float t = mTimer / mFadeOutDuration;
            if (t > 1.0f) t = 1.0f;

            mAlpha = 1.0f - t;

            if (mTimer >= mFadeOutDuration)
            {
                mAlpha = 0.0f;
                mPhase = Phase::Done;
                goToTitle();
            }
        } break;

        case Phase::Done:
        default:
            break;
    }
}

void SplashState::goToTitle()
{
    if (mLeaving)
    {
        return;
    }

    mLeaving = true;

    getGStateMgr().readyUpState(gstate::Title);
}

void SplashState::Render()
{
    auto& textures =getGStateMgr().getGame().getTextures();

    const auto bgIt = getGStateMgr().getGame().getTextures().find("splash");

    if (bgIt != textures.end() && bgIt->second.id != 0) {
        const Texture2D &bg = bgIt->second;
        Rectangle src{0.0f, 0.0f, static_cast<float>(bg.width), static_cast<float>(bg.height)};
        Rectangle dst{0.0f, 0.0f, static_cast<float>(glb::WW), static_cast<float>(glb::WH)};
        const float x = (static_cast<float>(glb::WW) - static_cast<float>(mLogo.width)) * 0.5f;
        const float y = (static_cast<float>(glb::WH) - static_cast<float>(mLogo.height)) * 0.5f;

        Color tint = WHITE;
        tint.a = static_cast<unsigned char>(mAlpha * 255.0f);
        DrawTexturePro(bg, src, dst, {0.0f, 0.0f}, 0.0f, tint);
    }
}

void SplashState::Enter() {
    GameState::Enter();


    auto& texs =getGStateMgr().getGame().getTextures();

    getGStateMgr().getGame().getTextures().emplace("splash", LoadTexture("textures/Logo.png"));

    TraceLog(LOG_INFO, "paddle id=%u w=%d h=%d", texs["splash"].id, texs["splash"].width, texs["splash"].height);

    mPhase = Phase::FadeIn;
    mAlpha = 0.0f;
    mTimer = 0.0f;
    mLeaving = false;

}

void SplashState::Leave() {
    GameState::Leave();
    auto& sndmgr = getGStateMgr().getGame().getSoundMgr();
    sndmgr.loadTag("gameplay");
    sndmgr.stopMusic("gameplay_music");


}

void SplashState::RenderUI()
{

}

