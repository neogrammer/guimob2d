//
// Created by jlhar on 3/21/2026.
//

#include "Tile.h"
#include <memory>
#include <algorithm>
#include <utility>

namespace map {

    Tile::Tile()
        : Tile{-1, 1, {64, 64} }
    {

    }

    Tile::~Tile() {

    }

    Tile::Tile(const Tile & o)
            : mTilesize{o.mTilesize}, mOwnerPitch{o.mOwnerPitch}, mInteractible{o.mInteractible}, mVisible{o.mVisible}, mOwnerType{o.mOwnerType},mOwnerIndex{o.mOwnerIndex}
    {

    }

    Tile &Tile::operator=(const Tile & o)
            {
                mTilesize=o.mTilesize;
                mInteractible=o.mInteractible;
                mVisible=o.mVisible;
                mOwnerType=o.mOwnerType;
                mOwnerIndex=o.mOwnerIndex;
                mOwnerPitch=o.mOwnerPitch;
              return *this;
    }

    Tile::Tile(Tile && o)
            : mTilesize{std::move(o.mTilesize)},  mOwnerPitch{std::move(o.mOwnerPitch)}, mInteractible{std::move(o.mInteractible)}, mVisible{std::move(o.mVisible)}, mOwnerType{std::move(o.mOwnerType)},mOwnerIndex{std::move(o.mOwnerIndex)}
    {

    }

    Tile &Tile::operator=(Tile&& o)
             {
                 mTilesize=std::move(o.mTilesize);
                 mInteractible=std::move(o.mInteractible);
                 mVisible=std::move(o.mVisible);
                 mOwnerType=std::move(o.mOwnerType);
                 mOwnerIndex=std::move(o.mOwnerIndex);
                 mOwnerPitch=std::move(o.mOwnerPitch);
                 return *this;
    }

    Tile::Tile(int ownerIndex, int ownerPitch, Vector2 tileSize,  bool interactible, bool visible, bool tilesetIsOwner, bool solid)
     : mTilesize{tileSize}, mOwnerPitch{ownerPitch}, mInteractible{interactible}, mVisible{visible}, mOwnerType{((tilesetIsOwner) ? OwnerType::Tileset : OwnerType::Tilemap )}, mOwnerIndex{ownerIndex}
    {
    }

    Vector2 Tile::GetRelativeLocation() {

        int x = mOwnerIndex % mOwnerPitch;
        int y = std::ceil((float)mOwnerIndex / (float)mOwnerPitch);

        return Vector2{x*mTilesize.x, y*mTilesize.y };

    }


} // map