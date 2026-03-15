#include "Components.h"

TransformComponent::TransformComponent(float x, float y)
    : position{ x, y }
    , scale{ 1.0f, 1.0f }
    , rotationDegrees(0.0f)
{
}

void TransformComponent::setPosition(float x, float y)
{
    position = { x, y };
}

void TransformComponent::setPosition(const Vector2& pos)
{
    position = pos;
}

void TransformComponent::translate(float x, float y)
{
    position.x += x;
    position.y += y;
}

void TransformComponent::translate(const Vector2& offset)
{
    position.x += offset.x;
    position.y += offset.y;
}

Vector2 TransformComponent::getPosition() const
{
    return position;
}
