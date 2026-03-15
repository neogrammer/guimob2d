#ifndef COMPONENTS_H__
#define COMPONENTS_H__

#include <memory>
#include <string>
#include "raymob.h"

class Component
{
public:
    virtual ~Component() = default;
};

class AIComponent : public Component
{
public:
    AIComponent(float speed = 0.0f, bool isPaddle = false);

    void setVelocity(const Vector2& velocity);
    Vector2 getVelocity() const;
    float getSpeed() const;
    bool getIsPaddle() const;

private:
    float speed;
    bool isPaddle;
    Vector2 velocity;
};

class BoundingBoxComponent : public Component
{
public:
    Vector2 min;
    Vector2 max;

    BoundingBoxComponent(const Vector2& min = { 0.0f, 0.0f }, const Vector2& max = { 0.0f, 0.0f });
};

class CollisionComponent : public Component
{
public:
    explicit CollisionComponent(const Rectangle& bounds = { 0.0f, 0.0f, 0.0f, 0.0f });

    Rectangle getBounds() const;
    void setBounds(const Rectangle& bounds);

private:
    Rectangle bounds;
};

class InputComponent : public Component
{
public:
    int keyUp;
    int keyDown;
    int keyLeft;
    int keyRight;

    bool useHardwareKeys;
    float speed;
    Vector2 moveIntent;

    InputComponent();
    void setMoveIntent(float x, float y);
};

class SpriteComponent : public Component
{
public:
    std::string textureId;
    Rectangle source;
    Vector2 size;
    Color tint;
    bool visible;

    SpriteComponent();
    SpriteComponent(const std::string& textureId, float width, float height);

    void setTextureId(const std::string& id);
    void setSize(float width, float height);
    void setSource(const Rectangle& src);
};

class TransformComponent : public Component
{
public:
    Vector2 position;
    Vector2 scale;
    float rotationDegrees;

    TransformComponent(float x = 0.0f, float y = 0.0f);

    void setPosition(float x, float y);
    void setPosition(const Vector2& pos);
    void translate(float x, float y);
    void translate(const Vector2& offset);
    Vector2 getPosition() const;
};

#endif
