#include "Components.h"

AIComponent::AIComponent(float speed, bool isPaddle)
    : speed(speed), isPaddle(isPaddle), velocity{ 0.0f, 0.0f }
{
}

void AIComponent::setVelocity(const Vector2& velocity)
{
    this->velocity = velocity;
}

Vector2 AIComponent::getVelocity() const
{
    return velocity;
}

float AIComponent::getSpeed() const
{
    return speed;
}

bool AIComponent::getIsPaddle() const
{
    return isPaddle;
}
