//
// Created by jlhar on 3/21/2026.
//

#include "Tile.h"

namespace map {

    Tile::Tile()
        : Tile{-1, 1, {64.0f, 64.0f}}
    {
    }

    Tile::~Tile() = default;

    Tile::Tile(const Tile& o)
        : mSolid{o.mSolid},
          mVisible{o.mVisible},
          mInteractible{o.mInteractible},
          mTilesize{o.mTilesize},
          mOwnerType{o.mOwnerType},
          mOwnerPitch{o.mOwnerPitch},
          mOwnerIndex{o.mOwnerIndex},
          mSourceTileIndex{o.mSourceTileIndex},
          mTouchEventType{o.mTouchEventType},
          mTouchArg{o.mTouchArg}
    {
    }

    Tile& Tile::operator=(const Tile& o)
    {
        if (this == &o) {
            return *this;
        }

        mSolid = o.mSolid;
        mVisible = o.mVisible;
        mInteractible = o.mInteractible;
        mTilesize = o.mTilesize;
        mOwnerType = o.mOwnerType;
        mOwnerPitch = o.mOwnerPitch;
        mOwnerIndex = o.mOwnerIndex;
        mSourceTileIndex = o.mSourceTileIndex;
        mTouchEventType = o.mTouchEventType;
        mTouchArg = o.mTouchArg;
        return *this;
    }

    Tile::Tile(Tile&& o) noexcept
        : mSolid{o.mSolid},
          mVisible{o.mVisible},
          mInteractible{o.mInteractible},
          mTilesize{o.mTilesize},
          mOwnerType{o.mOwnerType},
          mOwnerPitch{o.mOwnerPitch},
          mOwnerIndex{o.mOwnerIndex},
          mSourceTileIndex{o.mSourceTileIndex},
          mTouchEventType{o.mTouchEventType},
          mTouchArg{o.mTouchArg}
    {
    }

    Tile& Tile::operator=(Tile&& o) noexcept
    {
        if (this == &o) {
            return *this;
        }

        mSolid = o.mSolid;
        mVisible = o.mVisible;
        mInteractible = o.mInteractible;
        mTilesize = o.mTilesize;
        mOwnerType = o.mOwnerType;
        mOwnerPitch = o.mOwnerPitch;
        mOwnerIndex = o.mOwnerIndex;
        mSourceTileIndex = o.mSourceTileIndex;
        mTouchEventType = o.mTouchEventType;
        mTouchArg = o.mTouchArg;
        return *this;
    }

    Tile::Tile(int ownerIndex,
               int ownerPitch,
               Vector2 tileSize,
               bool interactible,
               bool visible,
               bool tilesetIsOwner,
               bool solid,
               int sourceTileIndex)
        : mSolid{solid},
          mVisible{visible},
          mInteractible{interactible},
          mTilesize{tileSize},
          mOwnerType{tilesetIsOwner ? OwnerType::Tileset : OwnerType::Tilemap},
          mOwnerPitch{ownerPitch},
          mOwnerIndex{ownerIndex},
          mSourceTileIndex{sourceTileIndex}
    {
    }

    Vector2 Tile::GetRelativeLocation() const
    {
        if (mOwnerPitch <= 0 || mOwnerIndex < 0) {
            return Vector2{0.0f, 0.0f};
        }

        const int x = mOwnerIndex % mOwnerPitch;
        const int y = mOwnerIndex / mOwnerPitch;

        return Vector2{
            static_cast<float>(x) * mTilesize.x,
            static_cast<float>(y) * mTilesize.y
        };
    }

    Rectangle Tile::GetRelativeBounds() const
    {
        Vector2 pos = GetRelativeLocation();
        return Rectangle{pos.x, pos.y, mTilesize.x, mTilesize.y};
    }

} // map
