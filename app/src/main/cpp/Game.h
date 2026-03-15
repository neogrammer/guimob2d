#ifndef GAME_H___
#define GAME_H___

#include <memory>
#include <string>
#include <unordered_map>
#include <utility>

#include "raymob.h"
#include "Globs.h"
#include "Entity.h"
#include "Systems.h"

class Game
{
public:
    Game();
    ~Game();

    bool Initialize();
    void Shutdown();

    void Update(float deltaTime);
    void Render();

    void SetPlayerMoveIntent(float x, float y);
    void SetPlayerTouchY(float touchY);

private:
    Texture2D TryLoadTexture(const char* primaryPath, const char* fallbackPath);
    void loadTextures();
    void unloadTextures();
    void createEntities();

    bool mInitialized;

    sptent player;
    sptent ball;
    sptent opponentPaddle;

    RenderSystem renderSystem;
    InputSystem inputSystem;
    AISystem aiSystem;
    CollisionSystem collisionSystem;

    std::unordered_map<std::string, Texture2D> textures;
};

#endif
