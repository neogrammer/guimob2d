#include "Systems.h"
#include "Components.h"

#include <algorithm>

RenderSystem::RenderSystem() = default;
RenderSystem::~RenderSystem() = default;

void RenderSystem::render(const std::unordered_map<std::string, Texture2D>& textures) const
{
    for (const auto& entity : entities)
    {
        auto sprite = entity->getComponent<SpriteComponent>();
        auto transform = entity->getComponent<TransformComponent>();
        if (!sprite || !transform || !sprite->visible)
        {
            continue;
        }

        Vector2 drawSize = sprite->size;
        if (drawSize.x <= 0.0f) drawSize.x = 32.0f;
        if (drawSize.y <= 0.0f) drawSize.y = 32.0f;

        const auto found = textures.find(sprite->textureId);
        if (found != textures.end() && found->second.id != 0)
        {
            const Texture2D& texture = found->second;
            Rectangle src = sprite->source;
            if (src.width <= 0.0f || src.height <= 0.0f)
            {
                src = { 0.0f, 0.0f, static_cast<float>(texture.width), static_cast<float>(texture.height) };
            }

            Rectangle dst{
                transform->position.x,
                transform->position.y,
                drawSize.x * transform->scale.x,
                drawSize.y * transform->scale.y
            };

            DrawTexturePro(texture, src, dst, { 0.0f, 0.0f }, transform->rotationDegrees, sprite->tint);
            continue;
        }

        if (sprite->textureId == "ball")
        {
            const float radius = std::min(drawSize.x, drawSize.y) * 0.5f;
            DrawCircleV(
                { transform->position.x + radius, transform->position.y + radius },
                radius,
                sprite->tint.a == 0 ? WHITE : sprite->tint
            );
        }
        else
        {
            DrawRectangleRounded(
                Rectangle{ transform->position.x, transform->position.y, drawSize.x, drawSize.y },
                0.25f,
                8,
                sprite->tint.a == 0 ? WHITE : sprite->tint
            );
        }
    }
}

void RenderSystem::update(float deltaTime)
{
    (void)deltaTime;
}
