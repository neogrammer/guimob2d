//
// Created by jlhar on 3/17/2026.
//

#include "TouchInput.h"

#include "imgui/imgui.h"

namespace input
{
    namespace
    {
        float LengthSq(Vector2 v)
        {
            return v.x * v.x + v.y * v.y;
        }
    }

    void TouchInput::update(float dt)
    {
        const bool wasDown = mPrimary.Down;

        mPrimary.PrevScreen = mPrimary.Screen;

        const bool hasTouch = (GetTouchPointCount() > 0);

        mPrimary.Active = hasTouch;
        mPrimary.Down = hasTouch;

        if (hasTouch)
        {
            mPrimary.Screen = GetTouchPosition(0);
        }

        mPrimary.Delta = {
                mPrimary.Screen.x - mPrimary.PrevScreen.x,
                mPrimary.Screen.y - mPrimary.PrevScreen.y
        };

        mPrimary.Pressed = (!wasDown && hasTouch);
        mPrimary.Released = (wasDown && !hasTouch);

        if (mPrimary.Pressed)
        {
            mPrimary.HeldSeconds = 0.0f;
        }
        else if (mPrimary.Down)
        {
            mPrimary.HeldSeconds += dt;
        }
        else
        {
            mPrimary.HeldSeconds = 0.0f;
            mPrimary.Delta = { 0.0f, 0.0f };
        }
    }

    bool TouchInput::primaryActive() const
    {
        return mPrimary.Active;
    }

    bool TouchInput::primaryPressed() const
    {
        return mPrimary.Pressed;
    }

    bool TouchInput::primaryDown() const
    {
        return mPrimary.Down;
    }

    bool TouchInput::primaryReleased() const
    {
        return mPrimary.Released;
    }

    Vector2 TouchInput::primaryScreen() const
    {
        return mPrimary.Screen;
    }

    Vector2 TouchInput::primaryDelta() const
    {
        return mPrimary.Delta;
    }

    Vector2 TouchInput::primaryWorld(const Camera2D& camera) const
    {
        return GetScreenToWorld2D(mPrimary.Screen, camera);
    }

    float TouchInput::primaryHeldSeconds() const
    {
        return mPrimary.HeldSeconds;
    }

    bool TouchInput::primaryMoved(float threshold) const
    {
        return LengthSq(mPrimary.Delta) >= (threshold * threshold);
    }

    bool TouchInput::isPointerBlockedByUi() const
    {
        return ImGui::GetCurrentContext() && ImGui::GetIO().WantCaptureMouse;
    }
}
