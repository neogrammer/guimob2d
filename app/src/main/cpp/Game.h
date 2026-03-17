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
#include "audio/SoundManager.h"
#include "audio/GameAudio.h"
#include "GameStateMgr.h"

class Game
{
public:
    Game();
    ~Game();

    bool Initialize();
    void Shutdown();

    void Input();
    void Update(float deltaTime);
    void Render();

    void SetPlayerMoveIntent(float x, float y);
    void SetPlayerTouchY(float touchY);

    sfx::SoundManager& getSoundMgr();

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

    std::unique_ptr<sfx::SoundManager> soundManager;
    std::unique_ptr<GameStateMgr> gStateMgr;
};

#endif
