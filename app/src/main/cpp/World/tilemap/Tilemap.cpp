//
// Created by jlhar on 3/21/2026.
//

#include "Tilemap.h"

#include <algorithm>
#include <cctype>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

namespace map {
namespace {

std::string trim(const std::string& s)
{
    std::size_t start = 0;
    while (start < s.size() && std::isspace(static_cast<unsigned char>(s[start]))) {
        ++start;
    }

    std::size_t end = s.size();
    while (end > start && std::isspace(static_cast<unsigned char>(s[end - 1]))) {
        --end;
    }

    return s.substr(start, end - start);
}

std::string readWholeTextFile(const std::string& path)
{
    {
        std::ifstream in(path, std::ios::binary);
        if (in.is_open()) {
            std::ostringstream ss;
            ss << in.rdbuf();
            return ss.str();
        }
    }

    char* fileText = LoadFileText(path.c_str());
    if (fileText != nullptr) {
        std::string result(fileText);
        UnloadFileText(fileText);
        return result;
    }

    return {};
}

class LineReader {
public:
    explicit LineReader(std::string text)
        : mStream(std::move(text))
    {
    }

    bool nextLine(std::string& out)
    {
        std::string line;
        while (std::getline(mStream, line)) {
            const std::string cleaned = trim(line);
            if (cleaned.empty()) {
                continue;
            }
            if (!cleaned.empty() && cleaned[0] == '#') {
                continue;
            }
            out = cleaned;
            return true;
        }
        return false;
    }

    int nextInt()
    {
        std::string line;
        if (!nextLine(line)) {
            throw std::runtime_error("Missing integer line in tile file.");
        }
        return std::stoi(line);
    }

    std::string nextString()
    {
        std::string line;
        if (!nextLine(line)) {
            throw std::runtime_error("Missing string line in tile file.");
        }
        return line;
    }

    std::vector<int> nextIntBlock(int expectedCount)
    {
        std::vector<int> values;
        values.reserve(expectedCount);

        std::string line;
        while (static_cast<int>(values.size()) < expectedCount && nextLine(line)) {
            std::istringstream iss(line);
            int value = 0;
            while (iss >> value) {
                values.push_back(value);
                if (static_cast<int>(values.size()) == expectedCount) {
                    break;
                }
            }
        }

        if (static_cast<int>(values.size()) != expectedCount) {
            throw std::runtime_error("Not enough values in tile data block.");
        }

        return values;
    }

private:
    std::istringstream mStream;
};

int clampStart(int value)
{
    return std::max(0, value);
}

} // namespace

Tilemap::Tilemap()
    : mTileset(std::make_unique<Tileset>())
{
}

Tilemap::Tilemap(const std::string& tilesetFileName, const std::string& tilemapFileName)
    : Tilemap()
{
    if (!loadTilesetFromFile(tilesetFileName)) {
        throw std::runtime_error("Failed to load tileset file: " + tilesetFileName);
    }

    if (!loadTilemapFromFile(tilemapFileName)) {
        throw std::runtime_error("Failed to load tilemap file: " + tilemapFileName);
    }
}

std::string Tilemap::ReadTilesetFileText(const std::string& fileName)
{
    return readWholeTextFile("data/tilesets/" + fileName);
}

std::string Tilemap::ReadTilemapFileText(const std::string& fileName)
{
    return readWholeTextFile("data/tilemaps/" + fileName);
}

bool Tilemap::loadTilesetFromFile(const std::string& fileName)
{
    const std::string text = ReadTilesetFileText(fileName);
    if (text.empty()) {
        return false;
    }

    try {
        LineReader reader(text);

        mTileset = std::make_unique<Tileset>();
        mTileset->clear();

        mTileset->mTextureAssetPath = reader.nextString();
        mTileset->mPitch = reader.nextInt();
        mTileset->mNumTilesP = reader.nextInt();

        const int tileWidth = reader.nextInt();
        const int tileHeight = reader.nextInt();
        mTileset->mTileSize = Vector2{static_cast<float>(tileWidth), static_cast<float>(tileHeight)};

        const int numTiles = mTileset->mNumTilesP;
        const std::vector<int> solidBlock = reader.nextIntBlock(numTiles);
        const std::vector<int> visibleBlock = reader.nextIntBlock(numTiles);
        const std::vector<int> interactibleBlock = reader.nextIntBlock(numTiles);

        mTileset->mTiles.clear();
        mTileset->mTiles.reserve(numTiles);

        for (int i = 0; i < numTiles; ++i) {
            auto tile = std::make_unique<Tile>(
                i,
                mTileset->mPitch,
                mTileset->mTileSize,
                interactibleBlock[i] != 0,
                visibleBlock[i] != 0,
                true,
                solidBlock[i] != 0,
                i
            );
            mTileset->mTiles.emplace_back(std::move(tile));
        }

        mTilesetFileName = fileName;
        mTileSize = mTileset->mTileSize;
        return true;
    }
    catch (...) {
        mTileset = std::make_unique<Tileset>();
        return false;
    }
}

bool Tilemap::loadTilemapFromFile(const std::string& fileName)
{
    const std::string text = ReadTilemapFileText(fileName);
    if (text.empty() || !mTileset) {
        return false;
    }

    try {
        LineReader reader(text);

        width = reader.nextInt();
        height = reader.nextInt();
        const int mapTileWidth = reader.nextInt();
        const int mapTileHeight = reader.nextInt();
        const std::string referencedTilesetFile = reader.nextString();

        if (!mTilesetFileName.empty() && referencedTilesetFile != mTilesetFileName) {
            return false;
        }

        const int numMapTiles = width * height;
        const std::vector<int> sourceIndices = reader.nextIntBlock(numMapTiles);

        mTileSize = Vector2{static_cast<float>(mapTileWidth), static_cast<float>(mapTileHeight)};
        mTiles.clear();
        mTiles.reserve(numMapTiles);

        for (int i = 0; i < numMapTiles; ++i) {
            const int sourceIndex = sourceIndices[i];
            if (sourceIndex < 0 || sourceIndex >= mTileset->mNumTilesP) {
                return false;
            }

            std::unique_ptr<Tile> tile = mTileset->getCopyTile(sourceIndex);
            if (!tile) {
                return false;
            }

            tile->mOwnerType = Tile::OwnerType::Tilemap;
            tile->mOwnerPitch = width;
            tile->mOwnerIndex = i;
            tile->mSourceTileIndex = sourceIndex;
            tile->mTilesize = mTileSize;
            mTiles.emplace_back(std::move(tile));
        }

        return true;
    }
    catch (...) {
        width = 0;
        height = 0;
        mTiles.clear();
        return false;
    }
}

int Tilemap::getWidth() const noexcept
{
    return width;
}

int Tilemap::getHeight() const noexcept
{
    return height;
}

int Tilemap::getTilesetPitch() const noexcept
{
    return mTileset ? mTileset->mPitch : 0;
}

const Tileset& Tilemap::getTileset() const noexcept
{
    return *mTileset;
}

int Tilemap::getNumTilesInTileset() const noexcept
{
    return mTileset ? mTileset->mNumTilesP : 0;
}

std::vector<Tile*> Tilemap::getScreenTiles(int left, int top, int viewWidth, int viewHeight)
{
    std::vector<Tile*> result;
    if (mTiles.empty() || mTileSize.x <= 0.0f || mTileSize.y <= 0.0f) {
        return result;
    }

    const int tileWidth = static_cast<int>(mTileSize.x);
    const int tileHeight = static_cast<int>(mTileSize.y);
    const int startCol = clampStart(left / tileWidth);
    const int startRow = clampStart(top / tileHeight);
    const int endCol = std::min(width, (left + viewWidth + tileWidth - 1) / tileWidth);
    const int endRow = std::min(height, (top + viewHeight + tileHeight - 1) / tileHeight);

    for (int y = startRow; y < endRow; ++y) {
        for (int x = startCol; x < endCol; ++x) {
            Tile* tile = getTile(x, y);
            if (tile != nullptr) {
                result.push_back(tile);
            }
        }
    }

    return result;
}

std::vector<Tile*> Tilemap::getSolidTiles(int left, int top, int viewWidth, int viewHeight)
{
    std::vector<Tile*> result;
    for (Tile* tile : getScreenTiles(left, top, viewWidth, viewHeight)) {
        if (tile != nullptr && tile->mSolid) {
            result.push_back(tile);
        }
    }
    return result;
}

std::vector<Tile*> Tilemap::getVisibleTiles(int left, int top, int viewWidth, int viewHeight)
{
    std::vector<Tile*> result;
    for (Tile* tile : getScreenTiles(left, top, viewWidth, viewHeight)) {
        if (tile != nullptr && tile->mVisible) {
            result.push_back(tile);
        }
    }
    return result;
}

std::vector<std::unique_ptr<Tile>>& Tilemap::getAllTiles()
{
    return mTiles;
}

Tile* Tilemap::getTile(int x, int y)
{
    if (x < 0 || y < 0 || x >= width || y >= height) {
        return nullptr;
    }

    const int index = y * width + x;
    if (index < 0 || index >= static_cast<int>(mTiles.size())) {
        return nullptr;
    }

    return mTiles[index].get();
}

const Tile* Tilemap::getTile(int x, int y) const
{
    if (x < 0 || y < 0 || x >= width || y >= height) {
        return nullptr;
    }

    const int index = y * width + x;
    if (index < 0 || index >= static_cast<int>(mTiles.size())) {
        return nullptr;
    }

    return mTiles[index].get();
}

void Tilemap::RenderMap()
{
if (!mTileset) return;
    if (mTileset->mTexture.id == 0) return;

    for (const auto& tilePtr : mTiles)
    {
    if (!tilePtr) continue;

    const Tile& tile = *tilePtr;

        if (!tile.mVisible) continue;
        if (tile.mSourceTileIndex < 0) continue;
        if (tile.mSourceTileIndex >= mTileset->mNumTilesP) continue;

        Rectangle src = mTileset->getSourceRect(tile.mSourceTileIndex);
        Vector2 pos = tile.GetRelativeLocation();

        Rectangle dst{
        pos.x,
        pos.y,
        tile.mTilesize.x,
        tile.mTilesize.y
        };

        DrawTexturePro(
        mTileset->mTexture,
        src,
        dst,
        Vector2{0.0f, 0.0f},
        0.0f,
        WHITE
        );
        }
        }

        void Tilemap::RenderMap_Screen(int left, int top, int width, int height)
        {
        if (!mTileset) return;
        if (mTileset->mTexture.id == 0) return;

        Rectangle screenRect{
        (float)left,
        (float)top,
        (float)width,
        (float)height
        };

        for (const auto& tilePtr : mTiles)
        {
        if (!tilePtr) continue;

        const Tile& tile = *tilePtr;

        if (!tile.mVisible) continue;
        if (tile.mSourceTileIndex < 0) continue;
        if (tile.mSourceTileIndex >= mTileset->mNumTilesP) continue;

        Rectangle dst = tile.GetRelativeBounds();

        if (!CheckCollisionRecs(dst, screenRect))
        continue;

        Rectangle src = mTileset->getSourceRect(tile.mSourceTileIndex);

        DrawTexturePro(
        mTileset->mTexture,
        src,
        dst,
        Vector2{0.0f, 0.0f},
        0.0f,
        WHITE
        );
        }
        }

} // map
