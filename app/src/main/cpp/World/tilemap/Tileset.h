//
// Created by jlhar on 3/21/2026.
//

#ifndef RAYMOB1_TILESET_H
#define RAYMOB1_TILESET_H

#include <memory>
#include <string>
#include <vector>

#include "Tile.h"
#include "raymob.h"

namespace map {

    class Tileset {
    public:
        std::vector<std::unique_ptr<Tile>> mTiles{};

        Texture2D mTexture{};
        int mPitch{1};
        int mNumTilesP{0};

        Vector2 mTileSize{32.0f, 32.0f};
        std::string mTextureAssetPath{};

        bool loadTexture();
        Rectangle getSourceRect(int tileIndex) const;


        std::unique_ptr<Tile> getCopyTile(int index) const;
        void initialize(const std::vector<std::unique_ptr<Tile>>& tiles);
        void clear();

        ~Tileset();
    };

} // map

#endif //RAYMOB1_TILESET_H
