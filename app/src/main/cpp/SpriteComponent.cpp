#include "Components.h"

SpriteComponent::SpriteComponent()
    : textureId("default")
    , source{ 0.0f, 0.0f, 0.0f, 0.0f }
    , size{ 0.0f, 0.0f }
    , tint(WHITE)
    , visible(true)
{
}

SpriteComponent::SpriteComponent(const std::string& textureId, float width, float height)
    : textureId(textureId)
    , source{ 0.0f, 0.0f, 0.0f, 0.0f }
    , size{ width, height }
    , tint(WHITE)
    , visible(true)
{
}

void SpriteComponent::setTextureId(const std::string& id)
{
    textureId = id;
}

void SpriteComponent::setSize(float width, float height)
{
    size = { width, height };
}

void SpriteComponent::setSource(const Rectangle& src)
{
    source = src;
}
