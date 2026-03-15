#ifndef SYSTEMS_H__
#define SYSTEMS_H__

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "raymob.h"
#include "Entity.h"
#include "Globs.h"

class System
{
public:
    virtual ~System() = default;
    void addEntity(sptent entity);
    void clearEntities();
    virtual void update(float deltaTime) = 0;

protected:
    std::vector<sptent> entities;
};

class RenderSystem : public System
{
public:
    RenderSystem();
    ~RenderSystem() override;

    void render(const std::unordered_map<std::string, Texture2D>& textures) const;
    void update(float deltaTime) override;
};

class CollisionSystem : public System
{
public:
    CollisionSystem();
    ~CollisionSystem() override;

    void update(float deltaTime) override;

private:
    void handleCollision(const sptent& entityA, const sptent& entityB);
};

class AISystem : public System
{
public:
    AISystem();
    ~AISystem() override;

    void update(float deltaTime) override;
    void update(float deltaTime, const sptent& ball);
};

class InputSystem : public System
{
public:
    InputSystem();
    ~InputSystem() override;

    void update(float deltaTime) override;
};

#endif
