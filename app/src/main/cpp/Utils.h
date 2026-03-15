#ifndef UTILS_H__
#define UTILS_H__

#include <memory>

#include "raymob.h"
#include "Components.h"

Rectangle MakeWorldBounds(const std::shared_ptr<TransformComponent>& transform,
                          const std::shared_ptr<CollisionComponent>& collision);

void CenterEntityInWorld(const std::shared_ptr<SpriteComponent>& sprite,
                         const std::shared_ptr<TransformComponent>& transform,
                         float worldWidth,
                         float worldHeight);

void ClampToBounds(const std::shared_ptr<TransformComponent>& transform,
                   const std::shared_ptr<SpriteComponent>& sprite,
                   const std::shared_ptr<BoundingBoxComponent>& boundingBox);

Vector2 RectCenter(const Rectangle& rect);

#endif
