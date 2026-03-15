#include "Components.h"

CollisionComponent::CollisionComponent(const Rectangle& bounds)
    : bounds(bounds)
{
}

Rectangle CollisionComponent::getBounds() const
{
    return bounds;
}

void CollisionComponent::setBounds(const Rectangle& bounds)
{
    this->bounds = bounds;
}
