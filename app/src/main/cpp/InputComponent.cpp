#include "Components.h"

InputComponent::InputComponent()
    : keyUp(KEY_UP)
    , keyDown(KEY_DOWN)
    , keyLeft(KEY_LEFT)
    , keyRight(KEY_RIGHT)
    , useHardwareKeys(true)
    , speed(520.0f)
    , moveIntent{ 0.0f, 0.0f }
{
}

void InputComponent::setMoveIntent(float x, float y)
{
    moveIntent = { x, y };
}
