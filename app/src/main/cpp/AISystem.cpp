#include "Systems.h"
#include "Components.h"
#include "Utils.h"

#include <cstdlib>

AISystem::AISystem() = default;
AISystem::~AISystem() = default;

void AISystem::update(float deltaTime)
{
    (void)deltaTime;
}

void AISystem::update(float deltaTime, const sptent& ball)
{
    auto ballTransform = ball ? ball->getComponent<TransformComponent>() : nullptr;
    auto ballCollision = ball ? ball->getComponent<CollisionComponent>() : nullptr;

    Rectangle ballRect{};
    if (ballTransform && ballCollision)
    {
        ballRect = MakeWorldBounds(ballTransform, ballCollision);
    }

    for (const auto& entity : entities)
    {
        auto transform = entity->getComponent<TransformComponent>();
        auto ai = entity->getComponent<AIComponent>();
        auto sprite = entity->getComponent<SpriteComponent>();
        auto collision = entity->getComponent<CollisionComponent>();
        auto bounds = entity->getComponent<BoundingBoxComponent>();

        if (!transform || !ai || !sprite || !collision)
        {
            continue;
        }

        if (ai->getIsPaddle())
        {
            if (!ballTransform || !ballCollision)
            {
                continue;
            }

            const Rectangle paddleRect = MakeWorldBounds(transform, collision);
            const float ballCenterY = ballRect.y + ballRect.height * 0.5f;
            const float paddleCenterY = paddleRect.y + paddleRect.height * 0.5f;

            if (ballCenterY < paddleCenterY)
            {
                transform->translate(0.0f, -ai->getSpeed() * deltaTime);
            }
            else if (ballCenterY > paddleCenterY)
            {
                transform->translate(0.0f, ai->getSpeed() * deltaTime);
            }

            if (bounds)
            {
                ClampToBounds(transform, sprite, bounds);
            }
            continue;
        }

        Vector2 velocity = ai->getVelocity();
        if (velocity.x == 0.0f && velocity.y == 0.0f)
        {
            velocity.x = (std::rand() % 2 == 0) ? -360.0f : 360.0f;
            velocity.y = (std::rand() % 2 == 0) ? -220.0f : 220.0f;
        }

        transform->translate(velocity.x * deltaTime, velocity.y * deltaTime);

        Rectangle worldRect = MakeWorldBounds(transform, collision);
        if (worldRect.x < 0.0f)
        {
            transform->position.x -= worldRect.x;
            velocity.x = -velocity.x;
        }
        else if (worldRect.x + worldRect.width > static_cast<float>(glb::WW))
        {
            transform->position.x -= (worldRect.x + worldRect.width - static_cast<float>(glb::WW));
            velocity.x = -velocity.x;
        }

        worldRect = MakeWorldBounds(transform, collision);
        if (worldRect.y < 0.0f)
        {
            transform->position.y -= worldRect.y;
            velocity.y = -velocity.y;
        }
        else if (worldRect.y + worldRect.height > static_cast<float>(glb::WH))
        {
            transform->position.y -= (worldRect.y + worldRect.height - static_cast<float>(glb::WH));
            velocity.y = -velocity.y;
        }

        ai->setVelocity(velocity);
    }
}
