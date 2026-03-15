#include "Systems.h"
#include "Components.h"
#include "Utils.h"

CollisionSystem::CollisionSystem() = default;
CollisionSystem::~CollisionSystem() = default;

void CollisionSystem::update(float deltaTime)
{
    (void)deltaTime;

    for (size_t i = 0; i < entities.size(); ++i)
    {
        auto entityA = entities[i];
        auto transformA = entityA->getComponent<TransformComponent>();
        auto collisionA = entityA->getComponent<CollisionComponent>();
        if (!transformA || !collisionA)
        {
            continue;
        }

        const Rectangle boundsA = MakeWorldBounds(transformA, collisionA);

        for (size_t j = i + 1; j < entities.size(); ++j)
        {
            auto entityB = entities[j];
            auto transformB = entityB->getComponent<TransformComponent>();
            auto collisionB = entityB->getComponent<CollisionComponent>();
            if (!transformB || !collisionB)
            {
                continue;
            }

            const Rectangle boundsB = MakeWorldBounds(transformB, collisionB);
            if (CheckCollisionRecs(boundsA, boundsB))
            {
                handleCollision(entityA, entityB);
            }
        }
    }
}

void CollisionSystem::handleCollision(const sptent& entityA, const sptent& entityB)
{
    auto transformA = entityA->getComponent<TransformComponent>();
    auto transformB = entityB->getComponent<TransformComponent>();
    auto collisionA = entityA->getComponent<CollisionComponent>();
    auto collisionB = entityB->getComponent<CollisionComponent>();
    if (!transformA || !transformB || !collisionA || !collisionB)
    {
        return;
    }

    const bool aIsBall = entityA->hasComponent<AIComponent>() && !entityA->getComponent<AIComponent>()->getIsPaddle();
    const bool bIsBall = entityB->hasComponent<AIComponent>() && !entityB->getComponent<AIComponent>()->getIsPaddle();

    sptent ballEntity = aIsBall ? entityA : (bIsBall ? entityB : nullptr);
    sptent otherEntity = aIsBall ? entityB : (bIsBall ? entityA : nullptr);
    if (!ballEntity || !otherEntity)
    {
        return;
    }

    auto ballTransform = ballEntity->getComponent<TransformComponent>();
    auto ballCollision = ballEntity->getComponent<CollisionComponent>();
    auto ballAI = ballEntity->getComponent<AIComponent>();
    auto otherTransform = otherEntity->getComponent<TransformComponent>();
    auto otherCollision = otherEntity->getComponent<CollisionComponent>();
    if (!ballTransform || !ballCollision || !ballAI || !otherTransform || !otherCollision)
    {
        return;
    }

    const Rectangle ballRect = MakeWorldBounds(ballTransform, ballCollision);
    const Rectangle otherRect = MakeWorldBounds(otherTransform, otherCollision);
    const Rectangle overlap = GetCollisionRec(ballRect, otherRect);
    Vector2 velocity = ballAI->getVelocity();

    const Vector2 ballCenter = RectCenter(ballRect);
    const Vector2 otherCenter = RectCenter(otherRect);

    if (overlap.width < overlap.height)
    {
        if (ballCenter.x < otherCenter.x)
        {
            ballTransform->position.x -= overlap.width;
        }
        else
        {
            ballTransform->position.x += overlap.width;
        }
        velocity.x = -velocity.x;

        const float offset = (ballCenter.y - otherCenter.y) / (otherRect.height * 0.5f);
        velocity.y += offset * 140.0f;
    }
    else
    {
        if (ballCenter.y < otherCenter.y)
        {
            ballTransform->position.y -= overlap.height;
        }
        else
        {
            ballTransform->position.y += overlap.height;
        }
        velocity.y = -velocity.y;
    }

    if (velocity.y > 420.0f) velocity.y = 420.0f;
    if (velocity.y < -420.0f) velocity.y = -420.0f;

    ballAI->setVelocity(velocity);
}
