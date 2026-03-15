#include "Utils.h"

#include <algorithm>

Rectangle MakeWorldBounds(const std::shared_ptr<TransformComponent>& transform,
                          const std::shared_ptr<CollisionComponent>& collision)
{
    Rectangle rect = collision->getBounds();
    rect.x += transform->position.x;
    rect.y += transform->position.y;
    return rect;
}

void CenterEntityInWorld(const std::shared_ptr<SpriteComponent>& sprite,
                         const std::shared_ptr<TransformComponent>& transform,
                         float worldWidth,
                         float worldHeight)
{
    const float w = sprite->size.x;
    const float h = sprite->size.y;
    transform->setPosition((worldWidth - w) * 0.5f, (worldHeight - h) * 0.5f);
}

void ClampToBounds(const std::shared_ptr<TransformComponent>& transform,
                   const std::shared_ptr<SpriteComponent>& sprite,
                   const std::shared_ptr<BoundingBoxComponent>& boundingBox)
{
    if (!transform || !sprite || !boundingBox)
    {
        return;
    }

    const float width = sprite->size.x * transform->scale.x;
    const float height = sprite->size.y * transform->scale.y;

    Vector2 pos = transform->getPosition();
    pos.x = std::max(boundingBox->min.x, std::min(pos.x, boundingBox->max.x - width));
    pos.y = std::max(boundingBox->min.y, std::min(pos.y, boundingBox->max.y - height));

    transform->setPosition(pos);
}

Vector2 RectCenter(const Rectangle& rect)
{
    return { rect.x + rect.width * 0.5f, rect.y + rect.height * 0.5f };
}
