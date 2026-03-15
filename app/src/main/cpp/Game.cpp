#include "Game.h"

#include <cstdlib>

#include "Components.h"
#include "Utils.h"

Game::Game()
    : mInitialized(false)
{
}

Game::~Game()
{
    Shutdown();
}

bool Game::Initialize()
{
    if (mInitialized)
    {
        return true;
    }

    std::srand(0xC0FFEE);
    loadTextures();
    createEntities();
    mInitialized = true;
    return true;
}

void Game::Shutdown()
{
    if (!mInitialized)
    {
        return;
    }

    unloadTextures();
    textures.clear();

    renderSystem.clearEntities();
    inputSystem.clearEntities();
    aiSystem.clearEntities();
    collisionSystem.clearEntities();

    player.reset();
    ball.reset();
    opponentPaddle.reset();

    mInitialized = false;
}

void Game::Update(float deltaTime)
{
    if (!mInitialized)
    {
        return;
    }

    inputSystem.update(deltaTime);
    aiSystem.update(deltaTime, ball);
    collisionSystem.update(deltaTime);
}

void Game::Render()
{
    if (!mInitialized)
    {
        return;
    }

    ClearBackground(BLACK);

    const auto bgIt = textures.find("background");
    if (bgIt != textures.end() && bgIt->second.id != 0)
    {
        const Texture2D& bg = bgIt->second;
        Rectangle src{ 0.0f, 0.0f, static_cast<float>(bg.width), static_cast<float>(bg.height) };
        Rectangle dst{ 0.0f, 0.0f, static_cast<float>(glb::WW), static_cast<float>(glb::WH) };
        DrawTexturePro(bg, src, dst, { 0.0f, 0.0f }, 0.0f, WHITE);
    }
    else
    {
        DrawRectangleGradientV(0, 0, static_cast<int>(glb::WW), static_cast<int>(glb::WH),
                               Color{ 10, 18, 36, 255 }, Color{ 0, 0, 0, 255 });
    }

    DrawLine(static_cast<int>(glb::WW / 2), 0, static_cast<int>(glb::WW / 2), static_cast<int>(glb::WH), Color{ 255, 255, 255, 40 });
    DrawFPS(20, 20);

    renderSystem.render(textures);
}

void Game::SetPlayerMoveIntent(float x, float y)
{
    if (!player)
    {
        return;
    }

    auto input = player->getComponent<InputComponent>();
    if (input)
    {
        input->setMoveIntent(x, y);
    }
}

void Game::SetPlayerTouchY(float touchY)
{
    if (!player)
    {
        return;
    }

    auto transform = player->getComponent<TransformComponent>();
    auto sprite = player->getComponent<SpriteComponent>();
    auto bounds = player->getComponent<BoundingBoxComponent>();
    auto input = player->getComponent<InputComponent>();
    if (!transform || !sprite || !bounds)
    {
        return;
    }

    if (input)
    {
        input->setMoveIntent(0.0f, 0.0f);
    }

    transform->position.y = touchY - (sprite->size.y * 0.5f);
    ClampToBounds(transform, sprite, bounds);
}

Texture2D Game::TryLoadTexture(const char* primaryPath, const char* fallbackPath)
{
    if (FileExists(primaryPath))
    {
        TraceLog(LOG_INFO, "Loading texture from primary: %s", primaryPath);
        Texture2D t = LoadTexture(primaryPath);
        TraceLog(LOG_INFO, "Loaded primary: id=%u w=%d h=%d", t.id, t.width, t.height);
        if (t.id != 0)
        {
            TraceLog(LOG_INFO, "Primary missing, loading fallback: %s", fallbackPath);
            TraceLog(LOG_INFO, "Loaded texture: %s", primaryPath);
            TraceLog(LOG_INFO, "Loaded fallback: id=%u w=%d h=%d", t.id, t.width, t.height);
            return t;
        }
    }

    if (fallbackPath && FileExists(fallbackPath))
    {
        Texture2D t = LoadTexture(fallbackPath);
        if (t.id != 0)
        {
            TraceLog(LOG_INFO, "Loaded texture: %s", fallbackPath);
            return t;
        }
    }

    if (fallbackPath)
        TraceLog(LOG_WARNING, "Texture not found: %s or %s. Using shape fallback.", primaryPath, fallbackPath);
    else
        TraceLog(LOG_WARNING, "Texture not found: %s. Using shape fallback.", primaryPath);

    return Texture2D{};
}

void Game::loadTextures()
{
    textures.emplace("paddle", LoadTexture("textures/paddle.png"));
    textures.emplace("ball", LoadTexture("textures/ball.png"));
    textures.emplace("background", LoadTexture("textures/background.png"));

    TraceLog(LOG_INFO, "paddle id=%u w=%d h=%d", textures["paddle"].id, textures["paddle"].width, textures["paddle"].height);
    TraceLog(LOG_INFO, "ball id=%u w=%d h=%d", textures["ball"].id, textures["ball"].width, textures["ball"].height);
    TraceLog(LOG_INFO, "background id=%u w=%d h=%d", textures["background"].id, textures["background"].width, textures["background"].height);
}

//void Game::loadTextures()
//{
//    textures.emplace("paddle", TryLoadTexture("textures/paddle.png", "assets/textures/paddle.png"));
//    textures.emplace("ball", TryLoadTexture("textures/ball.png", "assets/textures/ball.png"));
//    textures.emplace("background", TryLoadTexture("textures/background.png", "assets/textures/background.png"));
//}

void Game::unloadTextures()
{
    for (auto& [name, texture] : textures)
    {
        (void)name;
        if (texture.id != 0)
        {
            UnloadTexture(texture);
            texture = Texture2D{};
        }
    }
}

void Game::createEntities()
{
    const Texture2D& paddleTex = textures.at("paddle");
    const Texture2D& ballTex = textures.at("ball");

    const float paddleW = (paddleTex.id != 0) ? static_cast<float>(paddleTex.width) : 44.0f;
    const float paddleH = (paddleTex.id != 0) ? static_cast<float>(paddleTex.height) : 180.0f;
    const float ballW = (ballTex.id != 0) ? static_cast<float>(ballTex.width) : 28.0f;
    const float ballH = (ballTex.id != 0) ? static_cast<float>(ballTex.height) : 28.0f;

    player = std::make_shared<Entity>();
    {
        const float x = glb::WW / 7.0f - paddleW;
        auto transform = std::make_shared<TransformComponent>(x, (glb::WH - paddleH) * 0.5f);
        auto sprite = std::make_shared<SpriteComponent>("paddle", paddleW, paddleH);
        sprite->tint = Color{ 235, 235, 255, 255 };
        auto bounds = std::make_shared<BoundingBoxComponent>(
            Vector2{ x, 0.0f },
            Vector2{ glb::WW / 7.0f, static_cast<float>(glb::WH) });
        auto collision = std::make_shared<CollisionComponent>(Rectangle{ 0.0f, 0.0f, paddleW, paddleH });
        auto input = std::make_shared<InputComponent>();
        input->speed = 620.0f;

        player->addComponent(transform);
        player->addComponent(sprite);
        player->addComponent(bounds);
        player->addComponent(collision);
        player->addComponent(input);
    }

    ball = std::make_shared<Entity>();
    {
        auto transform = std::make_shared<TransformComponent>();
        auto sprite = std::make_shared<SpriteComponent>("ball", ballW, ballH);
        sprite->tint = Color{ 255, 255, 255, 255 };
        auto collision = std::make_shared<CollisionComponent>(Rectangle{ 0.0f, 0.0f, ballW, ballH });
        auto ai = std::make_shared<AIComponent>(0.0f, false);

        ball->addComponent(transform);
        ball->addComponent(sprite);
        ball->addComponent(collision);
        ball->addComponent(ai);

        CenterEntityInWorld(sprite, transform, static_cast<float>(glb::WW), static_cast<float>(glb::WH));
    }

    opponentPaddle = std::make_shared<Entity>();
    {
        const float leftX = glb::WW - (glb::WW / 7.0f);
        const float rightX = static_cast<float>(glb::WW);
        auto transform = std::make_shared<TransformComponent>(leftX, (glb::WH - paddleH) * 0.5f);
        auto sprite = std::make_shared<SpriteComponent>("paddle", paddleW, paddleH);
        sprite->tint = Color{ 255, 190, 190, 255 };
        auto bounds = std::make_shared<BoundingBoxComponent>(
            Vector2{ leftX, 0.0f },
            Vector2{ rightX, static_cast<float>(glb::WH) });
        auto collision = std::make_shared<CollisionComponent>(Rectangle{ 0.0f, 0.0f, paddleW, paddleH });
        auto ai = std::make_shared<AIComponent>(520.0f, true);

        opponentPaddle->addComponent(transform);
        opponentPaddle->addComponent(sprite);
        opponentPaddle->addComponent(bounds);
        opponentPaddle->addComponent(collision);
        opponentPaddle->addComponent(ai);

        ClampToBounds(transform, sprite, bounds);
    }

    renderSystem.addEntity(ball);
    renderSystem.addEntity(player);
    renderSystem.addEntity(opponentPaddle);

    inputSystem.addEntity(player);

    aiSystem.addEntity(ball);
    aiSystem.addEntity(opponentPaddle);

    collisionSystem.addEntity(ball);
    collisionSystem.addEntity(player);
    collisionSystem.addEntity(opponentPaddle);
}
