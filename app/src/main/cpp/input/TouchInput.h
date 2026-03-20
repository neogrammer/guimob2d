//
// Created by jlhar on 3/17/2026.
//

#ifndef RAYMOB1_TOUCHINPUT_H
#define RAYMOB1_TOUCHINPUT_H


#include "raylib.h"

namespace input
{
    struct TouchPointState
    {
        bool Active{ false };
        bool Pressed{ false };
        bool Down{ false };
        bool Released{ false };

        Vector2 Screen{ 0.0f, 0.0f };
        Vector2 PrevScreen{ 0.0f, 0.0f };
        Vector2 Delta{ 0.0f, 0.0f };

        float HeldSeconds{ 0.0f };
    };

    class TouchInput
    {
    public:
        void update(float dt);

        bool primaryActive() const;
        bool primaryPressed() const;
        bool primaryDown() const;
        bool primaryReleased() const;

        Vector2 primaryScreen() const;
        Vector2 primaryDelta() const;
        Vector2 primaryWorld(const Camera2D& camera) const;

        float primaryHeldSeconds() const;
        bool primaryMoved(float threshold = 8.0f) const;

        bool isPointerBlockedByUi() const;

    private:
        TouchPointState mPrimary{};
    };
}


#endif //RAYMOB1_TOUCHINPUT_H
