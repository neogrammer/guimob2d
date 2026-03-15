//
// Created by jlhar on 3/10/2026.
//

#ifndef RAYMOB1_WORKMAPEDITOR_H
#define RAYMOB1_WORKMAPEDITOR_H

#include "raylib.h"
#include "imgui/imgui.h"

#include <string>
#include <vector>

class WorkMapEditor
{
public:
    WorkMapEditor() = default;
    ~WorkMapEditor();

    bool LoadFromFile(const char* mapPath);
    bool SaveToFile(const char* outPath) const;

    void DrawMap() const;
    void DrawEditorUI();
    void HandlePaintInput();

    int GetSelectedIndex() const { return m_SelectedIndex; }
    int GetDefaultIndex()  const { return m_DefaultIndex; }

    void SetSavePath(const std::string& path) { m_SavePath = path; }
    bool IsIsometric() const { return m_DrawIsometric; }
    void SetIsometric(bool value);
    void ToggleIsometric();

    int m_viewTileX = 0;
    int m_viewTileY = 0;

    void HandlePanInput();
    int FillVisibleDefaultTiles();
    bool IsTileVisibleOnScreen(int x, int y) const;
    void PaintCell(int x, int y);
    void PaintAtScreen(Vector2 screenPos);
    void PaintStroke(Vector2 fromScreen, Vector2 toScreen);void DrawTilePickerPopup();
    bool m_EraseMode = false;
    bool m_HasLastPaintPointer = false;
    Vector2 m_LastPaintPointer{ 0.0f, 0.0f };
private:
    void ClampMapOffset();

    bool m_PanWasDown = false;
    bool m_IsPanning = false;
    Vector2 m_LastPanPointer{ 0.0f, 0.0f };

private:
    bool ParseText(const std::string& text);
    std::string SerializeText() const;

    void RecomputeOrigin();
    void ClampSelectedIndex();

    bool InBounds(int x, int y) const;
    int  FlatIndex(int x, int y) const;
    int  MaxTileIndex() const;

    Rectangle GetSrcRectForTile(int tileIndex) const;
    Vector2 GetTileDrawPosition(int x, int y) const;
    bool ScreenToTile(Vector2 screenPos, int& outX, int& outY) const;

private:
    std::string m_TilesetFileName;
    std::string m_SavePath = "work_map_saved.txt";
    std::string m_LastSaveMessage;
    int m_TilesetColumns = 0;
    int m_TilesetRows    = 0;
    int m_TileSize       = 64;
    int m_DefaultIndex   = 0;
    bool m_DrawIsometric = false;
    int m_MapWidth  = 0;
    int m_MapHeight = 0;

    std::vector<int> m_Tiles;

    Texture2D m_TilesetTexture{};
    bool m_HasTilesetTexture = false;

    int  m_SelectedIndex = 0;
    bool m_PointerWasDown = false;

    Vector2 m_MapOrigin{ 0.0f, 0.0f };
    Vector2 m_MapOffset{ 0.0f, 0.0f };
};

#endif //RAYMOB1_WORKMAPEDITOR_H
