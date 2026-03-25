//
// Created by jlhar on 3/21/2026.
//

#include "Tileset.h"

namespace map {

std::unique_ptr<Tile> Tileset::getCopyTile(int index) const
{
    if (index < 0 || index >= static_cast<int>(mTiles.size()) || !mTiles[index]) {
        return nullptr;
    }

    return std::make_unique<Tile>(*mTiles[index]);
}
Tileset::~Tileset()
{
if (mTexture.id != 0)
{
UnloadTexture(mTexture);
mTexture = Texture2D{};
}
}


bool Tileset::loadTexture()
{
if (mTexture.id != 0)
{
UnloadTexture(mTexture);
mTexture = Texture2D{};
}

mTexture = LoadTexture(mTextureAssetPath.c_str());
return mTexture.id != 0;
}

Rectangle Tileset::getSourceRect(int tileIndex) const
{
const int col = tileIndex % mPitch;
const int row = tileIndex / mPitch;

return Rectangle{
(float)(col * mTileSize.x),
(float)(row * mTileSize.y),
(float)mTileSize.x,
(float)mTileSize.y
};
}


void Tileset::initialize(const std::vector<std::unique_ptr<Tile>>& tiles)
{
    clear();

    mTiles.reserve(tiles.size());
    for (const auto& tile : tiles)
    {
        if (!tile) {
            mTiles.emplace_back(std::make_unique<Tile>());
            continue;
        }

        auto copy = std::make_unique<Tile>(*tile);
        copy->mOwnerType = Tile::OwnerType::Tileset;
        copy->mOwnerPitch = mPitch;
        mTiles.emplace_back(std::move(copy));
    }

    mNumTilesP = static_cast<int>(mTiles.size());
}

void Tileset::clear()
{
    mTiles.clear();
    mPitch = 0;
    mNumTilesP = 0;
    mTileSize = Vector2{64.0f, 64.0f};
    mTextureAssetPath.clear();
}

} // map
