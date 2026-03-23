//
// Created by jlhar on 3/21/2026.
//

#ifndef RAYMOB1_TILE_H
#define RAYMOB1_TILE_H

#include "raymob.h"

namespace map
{

    class Tile
    {
        enum class OwnerType
        {
            Tileset,
            Tilemap,
            None
        };

        bool mSolid{false};
        bool mVisible{false};
        bool mInteractible{false};
        Vector2 mTilesize;
        OwnerType mOwnerType{OwnerType::None};
        int mOwnerPitch{1};
        int mOwnerIndex{-1};
    public:
        Tile();
        explicit Tile(int ownerIndex, int ownerPitch,  Vector2 tileSize, bool interactible = false, bool visible = false, bool tilesetIsOwner = false, bool solid = false);
        ~Tile();
        Tile(const Tile&);
        Tile& operator=(const Tile&);
        Tile(Tile&&);
        Tile& operator=(Tile&&);

    public:
        inline bool InTilemap() { return mOwnerType == OwnerType::Tilemap; }
        inline bool InTileset() { return mOwnerType == OwnerType::Tileset; }
        Vector2 GetRelativeLocation();
    };

} // tmap

#endif //RAYMOB1_TILE_H
