//
// Created by jlhar on 3/21/2026.
//

#ifndef RAYMOB1_TILEMAP_H
#define RAYMOB1_TILEMAP_H

#include <memory>
#include <string>
#include <vector>

#include "Tileset.h"

namespace map {

    class Tilemap {
    public:
        std::unique_ptr<Tileset> mTileset;
        int width{0};
        int height{0};
        std::vector<std::unique_ptr<Tile>> mTiles;
        std::string mTilesetFileName{};
        Vector2 mTileSize{64.0f, 64.0f};

        Tilemap();
        Tilemap(const std::string& tilesetFileName, const std::string& tilemapFileName);

        static std::string ReadTilesetFileText(const std::string& fileName);
        static std::string ReadTilemapFileText(const std::string& fileName);

        bool loadTilesetFromFile(const std::string& fileName);
        bool loadTilemapFromFile(const std::string& fileName);

        int getWidth() const noexcept;
        int getHeight() const noexcept;
        int getTilesetPitch() const noexcept;
        const Tileset& getTileset() const noexcept;
        int getNumTilesInTileset() const noexcept;
        std::vector<Tile*> getScreenTiles(int left, int top, int width, int height);
        std::vector<Tile*> getSolidTiles(int left, int top, int width, int height);
        std::vector<Tile*> getVisibleTiles(int left, int top, int width, int height);

        std::vector<std::unique_ptr<Tile>>& getAllTiles();
        Tile* getTile(int x, int y);
        const Tile* getTile(int x, int y) const;

        void RenderMap();
        void RenderMap_Screen(int left, int top, int width, int height);
    };

} // map

#endif //RAYMOB1_TILEMAP_H
