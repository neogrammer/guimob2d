//
// Created by jlhar on 3/15/2026.
//

#ifndef RAYMOB1_PLAYSTATE_H
#define RAYMOB1_PLAYSTATE_H

#include "../GameState.h"
#include "../World/tmap/Tile.h"

class PlayState : public GameState {
    map::Tile tile;
public:
    void Enter() override final;
    void Leave() override final;

    PlayState() = delete;
    explicit PlayState(GameStateMgr& gStateMgr);
    ~PlayState() override;

    void Input() override final;
    void Update(float deltaTime) override final;
    void Render() override final;
    void RenderUI() override final;
};


#endif //RAYMOB1_PLAYSTATE_H
