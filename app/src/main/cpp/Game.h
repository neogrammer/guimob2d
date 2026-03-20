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
#include "input/TouchInput.h"

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

    void RenderUI();
    void SetPlayerMoveIntent(float x, float y);
    void SetPlayerTouchY(float touchY);

    sfx::SoundManager& getSoundMgr();

    input::TouchInput& getTouch();
    const input::TouchInput& getTouch() const;

    std::unordered_map<std::string, Texture2D>& getTextures();

private:
    Texture2D TryLoadTexture(const char* primaryPath, const char* fallbackPath);
    void loadTextures();
    friend class GameState;
    void unloadTextures();
    void createEntities();

    bool mInitialized;

    RenderSystem renderSystem;
    InputSystem inputSystem;
    AISystem aiSystem;
    CollisionSystem collisionSystem;

    std::unordered_map<std::string, Texture2D> textures;

    std::unique_ptr<sfx::SoundManager> soundManager;
    std::unique_ptr<input::TouchInput> touchInput;
    std::unique_ptr<GameStateMgr> gStateMgr;
};

#endif
