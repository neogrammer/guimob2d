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
    public:
        enum class OwnerType
        {
            Tileset,
            Tilemap,
            None
        };

        enum class TouchEventType
        {
            None,
            Teleport,
            Custom
        };

        bool mSolid{false};
        bool mVisible{true};
        bool mInteractible{false};
        Vector2 mTilesize{64.0f, 64.0f};
        OwnerType mOwnerType{OwnerType::None};
        int mOwnerPitch{1};
        int mOwnerIndex{-1};
        int mSourceTileIndex{-1};
        TouchEventType mTouchEventType{TouchEventType::None};
        int mTouchArg{0};

    public:
        Tile();
        explicit Tile(int ownerIndex,
                      int ownerPitch,
                      Vector2 tileSize,
                      bool interactible = false,
                      bool visible = true,
                      bool tilesetIsOwner = false,
                      bool solid = false,
                      int sourceTileIndex = -1);
        ~Tile();
        Tile(const Tile&);
        Tile& operator=(const Tile&);
        Tile(Tile&&) noexcept;
        Tile& operator=(Tile&&) noexcept;

    public:
        inline bool InTilemap() const { return mOwnerType == OwnerType::Tilemap; }
        inline bool InTileset() const { return mOwnerType == OwnerType::Tileset; }
        Vector2 GetRelativeLocation() const;
        Rectangle GetRelativeBounds() const;
    };

} // map

#endif //RAYMOB1_TILE_H
