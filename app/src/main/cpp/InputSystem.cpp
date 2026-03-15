#include "Systems.h"
#include "Components.h"
#include "Utils.h"

#include <cmath>

InputSystem::InputSystem() = default;
InputSystem::~InputSystem() = default;

void InputSystem::update(float deltaTime)
{
    for (const auto& entity : entities)
    {
        auto transform = entity->getComponent<TransformComponent>();
        auto sprite = entity->getComponent<SpriteComponent>();
        auto bounds = entity->getComponent<BoundingBoxComponent>();
        auto input = entity->getComponent<InputComponent>();

        if (!transform || !sprite || !bounds || !input)
        {
            continue;
        }

        Vector2 move = input->moveIntent;

        if (input->useHardwareKeys)
        {
            if (IsKeyDown(input->keyUp))    move.y -= 1.0f;
            if (IsKeyDown(input->keyDown))  move.y += 1.0f;
            if (IsKeyDown(input->keyLeft))  move.x -= 1.0f;
            if (IsKeyDown(input->keyRight)) move.x += 1.0f;
        }

        const float lenSq = move.x * move.x + move.y * move.y;
        if (lenSq > 1.0f)
        {
            const float invLen = 1.0f / std::sqrt(lenSq);
            move.x *= invLen;
            move.y *= invLen;
        }

        transform->translate(move.x * input->speed * deltaTime, move.y * input->speed * deltaTime);
        ClampToBounds(transform, sprite, bounds);
    }
}
