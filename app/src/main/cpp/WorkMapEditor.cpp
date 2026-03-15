//
// Created by jlhar on 3/10/2026.
//
#include "WorkMapEditor.h"

#include <algorithm>
#include <cstdint>
#include <fstream>
#include <sstream>
#include <cmath>

static std::string ReadWholeTextFile(const char* path)
{
    // First try normal filesystem I/O.
    // This is what we want for internalDataPath/current_work_map.txt
    {
        std::ifstream in(path, std::ios::binary);
        if (in.is_open())
        {
            std::ostringstream ss;
            ss << in.rdbuf();
            return ss.str();
        }
    }

    // Fallback to raylib asset loading.
    // This is what we want for assets like "maps/work_map.txt"
    char* raw = LoadFileText(path);
    if (!raw) return {};

    std::string text = raw;
    UnloadFileText(raw);
    return text;
}

WorkMapEditor::~WorkMapEditor()
{
    if (m_HasTilesetTexture)
    {
        UnloadTexture(m_TilesetTexture);
        m_HasTilesetTexture = false;
    }
}

bool WorkMapEditor::LoadFromFile(const char* mapPath)
{
    std::string text = ReadWholeTextFile(mapPath);
    if (text.empty()) return false;

    if (!ParseText(text)) return false;

    if (m_HasTilesetTexture)
    {
        UnloadTexture(m_TilesetTexture);
        m_HasTilesetTexture = false;
    }

    std::string tilesetPath = std::string("textures/tilesets/spritesheet1.png");
    m_TilesetTexture = LoadTexture(tilesetPath.c_str());
    m_HasTilesetTexture = (m_TilesetTexture.id != 0);

    m_SelectedIndex = std::clamp(m_DefaultIndex, 0, MaxTileIndex());
    RecomputeOrigin();

    return m_HasTilesetTexture;
}

bool WorkMapEditor::SaveToFile(const char* outPath) const
{
    TraceLog(LOG_INFO, "SaveToFile: %s", outPath);

    std::ofstream out(outPath, std::ios::binary);
    if (!out.is_open())
    {
        TraceLog(LOG_ERROR, "Failed to open save file: %s", outPath);
        return false;
    }

    out << SerializeText();

    const bool ok = out.good();
    TraceLog(ok ? LOG_INFO : LOG_ERROR, ok ? "Save OK" : "Save FAILED");
    return ok;
}
bool WorkMapEditor::ParseText(const std::string& text)
{
    std::istringstream in(text);

    std::string tilesetFile;
    int tilesetCols = 0;
    int tilesetRows = 0;
    int tileSize    = 0;
    int defaultIdx  = 0;
    int mapWidth    = 0;
    int mapHeight   = 0;

    if (!(in >> tilesetFile)) return false;
    if (!(in >> tilesetCols >> tilesetRows >> tileSize >> defaultIdx >> mapWidth >> mapHeight)) return false;

    if (tilesetCols <= 0 || tilesetRows <= 0 || tileSize <= 0 || mapWidth <= 0 || mapHeight <= 0)
        return false;

    m_TilesetFileName = tilesetFile;
    m_TilesetColumns  = tilesetCols;
    m_TilesetRows     = tilesetRows;
    m_TileSize        = tileSize;
    m_DefaultIndex    = defaultIdx;
    m_MapWidth        = mapWidth;
    m_MapHeight       = mapHeight;

    m_Tiles.assign(m_MapWidth * m_MapHeight, m_DefaultIndex);

    for (int i = 0; i < (int)m_Tiles.size(); ++i)
    {
        int value = m_DefaultIndex;
        if (!(in >> value)) break;
        m_Tiles[i] = value;
    }

    ClampSelectedIndex();
    return true;
}

void WorkMapEditor::SetIsometric(bool value)
{
    m_DrawIsometric = value;
    RecomputeOrigin();
}

void WorkMapEditor::ToggleIsometric()
{
    m_DrawIsometric = !m_DrawIsometric;
    RecomputeOrigin();
}

std::string WorkMapEditor::SerializeText() const {
    std::ostringstream out;

    out << m_TilesetFileName << "\n";
    out << m_TilesetColumns << " "
        << m_TilesetRows << " "
        << m_TileSize << " "
        << m_DefaultIndex << " "
        << m_MapWidth << " "
        << m_MapHeight << "\n";

    for (int y = 0; y < m_MapHeight; ++y) {
        for (int x = 0; x < m_MapWidth; ++x) {
            if (x > 0) out << " ";
            out << m_Tiles[FlatIndex(x, y)];
        }
        out << "\n";
    }

    return out.str();
}

void WorkMapEditor::ClampMapOffset()
{
    // First version: clamp only for grid mode.
    // Isometric can still pan, but we won't try to smart-clamp/cull it here.
    if (m_DrawIsometric)
    {
        RecomputeOrigin();
        return;
    }

    const float screenW = (float)GetScreenWidth();
    const float screenH = (float)GetScreenHeight();

    const float mapPixelW = (float)(m_MapWidth * m_TileSize);
    const float mapPixelH = (float)(m_MapHeight * m_TileSize);

    float minX = screenW - mapPixelW;
    float minY = screenH - mapPixelH;

    // If the map is smaller than the screen, keep it anchored at 0.
    if (mapPixelW <= screenW) minX = 0.0f;
    if (mapPixelH <= screenH) minY = 0.0f;

    m_MapOffset.x = std::clamp(m_MapOffset.x, minX, 0.0f);
    m_MapOffset.y = std::clamp(m_MapOffset.y, minY, 0.0f);

    RecomputeOrigin();
}

void WorkMapEditor::RecomputeOrigin()
{
    m_MapOrigin.x = m_MapOffset.x;
    m_MapOrigin.y = m_MapOffset.y;
}

//}
//void WorkMapEditor::RecomputeOrigin()
//{
//    if (!m_DrawIsometric)
//    {
//        m_MapOrigin.x = 0.0f;
//        m_MapOrigin.y = 0.0f;
//        return;
//    }
//
//    const float tw = (float)m_TileSize;
//    const float th = (float)m_TileSize;
//
//    const float a = tw * 0.5f;
//    const float b = th * 0.25f;
//
//    const float totalWidth  = ((float)(m_MapWidth + m_MapHeight - 2) * a) + tw;
//    const float totalHeight = ((float)(m_MapWidth + m_MapHeight - 2) * b) + th;
//
//    const float left = 0.f; ((float)GetScreenWidth()  - totalWidth)  * 0.5f;
//    const float top  = 0.f; ((float)GetScreenHeight() - totalHeight) * 0.5f;
//
//    m_MapOrigin.x = left + ((float)(m_MapHeight - 1) * a);
//    m_MapOrigin.y = top;
//}
void WorkMapEditor::ClampSelectedIndex()
{
    m_SelectedIndex = std::clamp(m_SelectedIndex, 0, MaxTileIndex());
}

bool WorkMapEditor::InBounds(int x, int y) const
{
    return (x >= 0 && x < m_MapWidth && y >= 0 && y < m_MapHeight);
}

int WorkMapEditor::FlatIndex(int x, int y) const
{
    return y * m_MapWidth + x;
}

int WorkMapEditor::MaxTileIndex() const
{
    return (m_TilesetColumns * m_TilesetRows) - 1;
}

Rectangle WorkMapEditor::GetSrcRectForTile(int tileIndex) const
{
    int clamped = std::clamp(tileIndex, 0, MaxTileIndex());

    int tx = clamped % m_TilesetColumns;
    int ty = clamped / m_TilesetColumns;

    Rectangle src{};
    src.x = (float)(tx * m_TileSize);
    src.y = (float)(ty * m_TileSize);
    src.width  = (float)m_TileSize;
    src.height = (float)m_TileSize;
    return src;
}

Vector2 WorkMapEditor::GetTileDrawPosition(int x, int y) const
{
    if (!m_DrawIsometric)
    {
        return Vector2{
                m_MapOrigin.x + (float)(x * m_TileSize),
                m_MapOrigin.y + (float)(y * m_TileSize)
        };
    }

    const float tw = (float)m_TileSize;
    const float th = (float)m_TileSize;

    const float xIso = (float)(x - y) * (tw * 0.5f);
    const float yIso = (float)(x + y) * (th * 0.25f);

    return Vector2{
            m_MapOrigin.x + xIso,
            m_MapOrigin.y + yIso
    };
}
void WorkMapEditor::DrawMap() const
{
    if (!m_HasTilesetTexture) return;

    // Fast visible-only path for normal grid mode
    if (!m_DrawIsometric)
    {
        const float tileW = (float)m_TileSize;
        const float tileH = (float)m_TileSize;

        const int startX = std::max(0, (int)floorf((-m_MapOrigin.x) / tileW));
        const int startY = std::max(0, (int)floorf((-m_MapOrigin.y) / tileH));

        const int endX = std::min(
                m_MapWidth,
                (int)ceilf(((float)GetScreenWidth() - m_MapOrigin.x) / tileW) + 1
        );

        const int endY = std::min(
                m_MapHeight,
                (int)ceilf(((float)GetScreenHeight() - m_MapOrigin.y) / tileH) + 1
        );

        for (int y = startY; y < endY; ++y)
        {
            for (int x = startX; x < endX; ++x)
            {
                const int tileIndex = m_Tiles[FlatIndex(x, y)];
                Rectangle src = GetSrcRectForTile(tileIndex);

                Vector2 pos = GetTileDrawPosition(x, y);

                Rectangle dst{};
                dst.x = pos.x;
                dst.y = pos.y;
                dst.width = (float)m_TileSize;
                dst.height = (float)m_TileSize;

                DrawTexturePro(
                        m_TilesetTexture,
                        src,
                        dst,
                        Vector2{ 0.0f, 0.0f },
                        0.0f,
                        WHITE
                );
            }
        }

        return;
    }

    // Existing full draw for isometric mode
    for (int y = 0; y < m_MapHeight; ++y)
    {
        for (int x = 0; x < m_MapWidth; ++x)
        {
            const int tileIndex = m_Tiles[FlatIndex(x, y)];
            Rectangle src = GetSrcRectForTile(tileIndex);

            Vector2 pos = GetTileDrawPosition(x, y);

            Rectangle dst{};
            dst.x = pos.x;
            dst.y = pos.y;
            dst.width = (float)m_TileSize;
            dst.height = (float)m_TileSize;

            DrawTexturePro(
                    m_TilesetTexture,
                    src,
                    dst,
                    Vector2{ 0.0f, 0.0f },
                    0.0f,
                    WHITE
            );
        }
    }
}

bool WorkMapEditor::ScreenToTile(Vector2 screenPos, int& outX, int& outY) const
{
    if (!m_DrawIsometric)
    {
        int cellX = (int)((screenPos.x - m_MapOrigin.x) / (float)m_TileSize);
        int cellY = (int)((screenPos.y - m_MapOrigin.y) / (float)m_TileSize);

        if (!InBounds(cellX, cellY))
            return false;

        outX = cellX;
        outY = cellY;
        return true;
    }

    const float tw = (float)m_TileSize;
    const float th = (float)m_TileSize;

    const float a = tw * 0.5f;
    const float b = th * 0.25f;

    const float localX = screenPos.x - m_MapOrigin.x;
    const float localY = screenPos.y - m_MapOrigin.y;

    const float dx = localX / a;
    const float dy = localY / b;

    const float fx = (dx + dy) * 0.5f;
    const float fy = (dy - dx) * 0.5f;

    int cellX = (int)floorf(fx + 0.5f);
    int cellY = (int)floorf(fy + 0.5f);

    if (!InBounds(cellX, cellY))
        return false;

    outX = cellX;
    outY = cellY;
    return true;
}

void WorkMapEditor::DrawEditorUI()
{
    ImGuiIO& io = ImGui::GetIO();

    const ImVec2 windowSize(420.0f, 480.0f);
    const float margin = 16.0f;

    ImGui::SetNextWindowPos(
            ImVec2(io.DisplaySize.x - margin, io.DisplaySize.y - margin),
            ImGuiCond_Always,
            ImVec2(1.0f, 1.0f)
    );
    ImGui::SetNextWindowSize(windowSize, ImGuiCond_Always);
    ImGui::SetNextWindowBgAlpha(0.94f);

    const ImGuiWindowFlags flags =
            ImGuiWindowFlags_NoMove |
            ImGuiWindowFlags_NoResize |
            ImGuiWindowFlags_NoCollapse |
            ImGuiWindowFlags_NoSavedSettings;

    if (ImGui::Button("Fill Visible Defaults", ImVec2(-1.0f, 48.0f)))
    {
        const int changed = FillVisibleDefaultTiles();

        if (changed > 0)
            m_LastSaveMessage = "Filled " + std::to_string(changed) + " visible default tiles.";
        else
            m_LastSaveMessage = "No visible default tiles to fill.";
    }

    ImGui::Begin("Tile Editor###tile_editor", nullptr, flags);

    ImGui::Text("Tileset: %s", m_TilesetFileName.c_str());
    ImGui::Text("Tileset grid: %d x %d", m_TilesetColumns, m_TilesetRows);
    ImGui::Text("Tile size: %d", m_TileSize);
    ImGui::Text("Map size: %d x %d", m_MapWidth, m_MapHeight);
    ImGui::Text("Default index: %d", m_DefaultIndex);

    ImGui::SliderInt("Index", &m_SelectedIndex, 0, MaxTileIndex());
    ImGui::InputInt("Set index", &m_SelectedIndex, 1, 10);

    if (ImGui::ArrowButton("##tile_prev", ImGuiDir_Left))
        --m_SelectedIndex;

    ImGui::SameLine();

    if (ImGui::ArrowButton("##tile_next", ImGuiDir_Right))
        ++m_SelectedIndex;

    ImGui::SameLine();

    if (ImGui::Button("Use Default"))
        m_SelectedIndex = m_DefaultIndex;

    ClampSelectedIndex();

//    ImGui::Separator();
//    ImGui::Text("Selected tile: %d", m_SelectedIndex);
//    if (m_HasTilesetTexture)
//    {
//        Rectangle src = GetSrcRectForTile(m_SelectedIndex);
//
//        ImVec2 uv0(
//                src.x / (float)m_TilesetTexture.width,
//                src.y / (float)m_TilesetTexture.height
//        );
//
//        ImVec2 uv1(
//                (src.x + src.width) / (float)m_TilesetTexture.width,
//                (src.y + src.height) / (float)m_TilesetTexture.height
//        );
//
//        const ImVec2 previewSize(96.0f, 96.0f);
//        const ImVec2 previewMin = ImGui::GetCursorScreenPos();
//        const ImVec2 previewMax(previewMin.x + previewSize.x, previewMin.y + previewSize.y);
//
//        ImGui::InvisibleButton("##selected_tile_preview", previewSize);
//
//        ImDrawList* drawList = ImGui::GetWindowDrawList();
//        drawList->AddImage(
//                (ImTextureID)(intptr_t)m_TilesetTexture.id,
//                previewMin,
//                previewMax,
//                uv0,
//                uv1
//        );
//        drawList->AddRect(previewMin, previewMax, IM_COL32(255, 255, 255, 255), 0.0f, 0, 2.0f);
//
//        ImGui::Separator();
//        ImGui::Text("Touch action:");
//
//        if (m_EraseMode)
//        {
//            if (ImGui::Button("Switch to PAINT", ImVec2(-1.0f, 44.0f)))
//                m_EraseMode = false;
//            ImGui::Text("Current mode: ERASE");
//        }
//        else
//        {
//            if (ImGui::Button("Switch to ERASE", ImVec2(-1.0f, 44.0f)))
//                m_EraseMode = true;
//            ImGui::Text("Current mode: PAINT");
//        }
//
//        if (m_EraseMode)
//            ImGui::TextWrapped("Dragging will erase tiles back to the default index.");
//        else
//            ImGui::TextWrapped("Dragging will paint the selected tile index.");
//
//
//
//        if (ImGui::IsItemHovered())
//            ImGui::SetTooltip("Tap to choose a tile from the tilesheet");
//
//        if (ImGui::IsItemClicked(ImGuiMouseButton_Left))
//            ImGui::OpenPopup("Tile Picker");
//    }
//
//    DrawTilePickerPopup();

    ImGui::Separator();
    ImGui::Text("Selected tile: %d", m_SelectedIndex);

    if (m_HasTilesetTexture)
    {
        Rectangle src = GetSrcRectForTile(m_SelectedIndex);

        ImVec2 uv0(
                src.x / (float)m_TilesetTexture.width,
                src.y / (float)m_TilesetTexture.height
        );

        ImVec2 uv1(
                (src.x + src.width) / (float)m_TilesetTexture.width,
                (src.y + src.height) / (float)m_TilesetTexture.height
        );

        const ImVec2 previewSize(96.0f, 96.0f);
        const ImVec2 previewMin = ImGui::GetCursorScreenPos();
        const ImVec2 previewMax(previewMin.x + previewSize.x, previewMin.y + previewSize.y);

        ImGui::InvisibleButton("##selected_tile_preview", previewSize);

        // IMPORTANT: grab these NOW, before any other ImGui widgets
        const bool previewHovered = ImGui::IsItemHovered();
        const bool previewClicked = ImGui::IsItemClicked(ImGuiMouseButton_Left);

        ImDrawList* drawList = ImGui::GetWindowDrawList();
        drawList->AddImage(
                (ImTextureID)(intptr_t)m_TilesetTexture.id,
                previewMin,
                previewMax,
                uv0,
                uv1
        );
        drawList->AddRect(previewMin, previewMax, IM_COL32(255, 255, 255, 255), 0.0f, 0, 2.0f);

        if (previewHovered)
            ImGui::SetTooltip("Tap to choose a tile from the tilesheet");

        if (previewClicked)
            ImGui::OpenPopup("Tile Picker");
    }

    DrawTilePickerPopup();

  //  ImGui::Separator();
 //   ImGui::TextWrapped("Tap a map cell to paint the selected index.");
//    ImGui::TextWrapped("Tap a cell already using that tile to erase it back to the default tile.");

    // Push the save section to the bottom of the editor window.
    const float saveButtonHeight = 56.0f;
    const float statusHeight = 44.0f;
//    float remaining = ImGui::GetContentRegionAvail().y - saveButtonHeight - statusHeight;
//    if (remaining > 0.0f)
//        ImGui::Dummy(ImVec2(0.0f, remaining));
//
//    ImGui::Separator();

    if (m_DrawIsometric)
    {
        if (ImGui::Button("Draw Mode: ISOMETRIC", ImVec2(-1.0f, 48.0f)))
            ToggleIsometric();
    }
    else
    {
        if (ImGui::Button("Draw Mode: GRID", ImVec2(-1.0f, 48.0f)))
            ToggleIsometric();
    }

    ImGui::Spacing();

    if (ImGui::Button("Save Map", ImVec2(-1.0f, 56.0f)))
    {
        bool ok = SaveToFile(m_SavePath.c_str());
        if (ok)
            m_LastSaveMessage = "Saved: " + m_SavePath;
        else
            m_LastSaveMessage = "Save failed: " + m_SavePath;
    }


    ImGui::Separator();

    ImGui::Text("Touch action:");

    if (!m_EraseMode)
    {
        if (ImGui::Button("Mode: PAINT", ImVec2(-1.0f, 44.0f)))
            m_EraseMode = true;
    }
    else
    {
        if (ImGui::Button("Mode: ERASE", ImVec2(-1.0f, 44.0f)))
            m_EraseMode = false;
    }



    if (!m_LastSaveMessage.empty())
        ImGui::TextWrapped("%s", m_LastSaveMessage.c_str());

    ImGui::End();
}

void WorkMapEditor::PaintCell(int x, int y)
{
    if (!InBounds(x, y)) return;

    int& cell = m_Tiles[FlatIndex(x, y)];
    const int newValue = m_EraseMode ? m_DefaultIndex : m_SelectedIndex;

    if (cell != newValue)
        cell = newValue;
}

void WorkMapEditor::PaintAtScreen(Vector2 screenPos)
{
    int cellX = 0;
    int cellY = 0;

    if (ScreenToTile(screenPos, cellX, cellY))
        PaintCell(cellX, cellY);
}

void WorkMapEditor::PaintStroke(Vector2 fromScreen, Vector2 toScreen)
{
    const float dx = toScreen.x - fromScreen.x;
    const float dy = toScreen.y - fromScreen.y;
    const float dist = sqrtf(dx * dx + dy * dy);

    // Sample along the drag path so fast swipes do not skip tiles.
    const float stepSize = std::max(1.0f, (float)m_TileSize * 0.25f);
    const int steps = std::max(1, (int)ceilf(dist / stepSize));

    for (int i = 0; i <= steps; ++i)
    {
        const float t = (float)i / (float)steps;

        Vector2 p{
                fromScreen.x + dx * t,
                fromScreen.y + dy * t
        };

        PaintAtScreen(p);
    }
}


void WorkMapEditor::HandlePanInput()
{
    ImGuiIO& io = ImGui::GetIO();

    bool panDown = false;
    Vector2 panPointer{ 0.0f, 0.0f };

    if (!io.WantCaptureMouse)
    {
        const int touchCount = GetTouchPointCount();

        // Android-friendly: 2-finger drag pans the map
        if (touchCount >= 2)
        {
            Vector2 t0 = GetTouchPosition(0);
            Vector2 t1 = GetTouchPosition(1);

            panPointer.x = (t0.x + t1.x) * 0.5f;
            panPointer.y = (t0.y + t1.y) * 0.5f;
            panDown = true;
        }
            // Desktop-friendly: middle or right mouse drag pans the map
        else if (IsMouseButtonDown(MOUSE_BUTTON_MIDDLE) || IsMouseButtonDown(MOUSE_BUTTON_RIGHT))
        {
            panPointer = GetMousePosition();
            panDown = true;
        }
    }

    m_IsPanning = panDown;

    if (panDown)
    {
        if (m_PanWasDown)
        {
            Vector2 delta{
                    panPointer.x - m_LastPanPointer.x,
                    panPointer.y - m_LastPanPointer.y
            };

            m_MapOffset.x += delta.x;
            m_MapOffset.y += delta.y;

            ClampMapOffset();
        }

        m_LastPanPointer = panPointer;
    }

    m_PanWasDown = panDown;

    // Optional keyboard pan too
    if (!io.WantCaptureKeyboard)
    {
        const float keyPan = (IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT)) ? 16.0f : 8.0f;
        bool moved = false;

        // Move map opposite of camera intent
        if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT))
        {
            m_MapOffset.x += keyPan;
            moved = true;
        }
        if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT))
        {
            m_MapOffset.x -= keyPan;
            moved = true;
        }
        if (IsKeyDown(KEY_W) || IsKeyDown(KEY_UP))
        {
            m_MapOffset.y += keyPan;
            moved = true;
        }
        if (IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN))
        {
            m_MapOffset.y -= keyPan;
            moved = true;
        }

        if (moved)
            ClampMapOffset();
    }
}

//
//void WorkMapEditor::HandlePaintInput()
//{
//    ImGuiIO& io = ImGui::GetIO();
//
//    if (m_IsPanning)
//    {
//        m_PointerWasDown = false;
//        m_HasLastPaintPointer = false;
//        return;
//    }
//
//    bool pointerDown = false;
//    Vector2 pointer{};
//
//    const int touchCount = GetTouchPointCount();
//
//    if (touchCount == 1)
//    {
//        pointer = GetTouchPosition(0);
//        pointerDown = true;
//    }
//    else if (touchCount == 0 && IsMouseButtonDown(MOUSE_BUTTON_LEFT))
//    {
//        pointer = GetMousePosition();
//        pointerDown = true;
//    }
//
//    if (io.WantCaptureMouse)
//    {
//        m_PointerWasDown = pointerDown;
//        m_HasLastPaintPointer = false;
//        return;
//    }
//
//    if (pointerDown)
//    {
//        if (!m_PointerWasDown)
//        {
//            // Initial tap/down: paint immediately
//            PaintAtScreen(pointer);
//            m_LastPaintPointer = pointer;
//            m_HasLastPaintPointer = true;
//        }
//        else
//        {
//            // Drag: fill in the whole stroke path
//            if (m_HasLastPaintPointer)
//                PaintStroke(m_LastPaintPointer, pointer);
//            else
//                PaintAtScreen(pointer);
//
//            m_LastPaintPointer = pointer;
//            m_HasLastPaintPointer = true;
//        }
//    }
//    else
//    {
//        m_HasLastPaintPointer = false;
//    }
//
//    m_PointerWasDown = pointerDown;
//}

void WorkMapEditor::HandlePaintInput()
{
    ImGuiIO& io = ImGui::GetIO();

    if (m_IsPanning)
    {
        m_PointerWasDown = false;
        m_HasLastPaintPointer = false;
        return;
    }

    bool pointerDown = false;
    Vector2 pointer{};

    const int touchCount = GetTouchPointCount();

    // One-finger touch = paint/erase
    if (touchCount == 1)
    {
        pointer = GetTouchPosition(0);
        pointerDown = true;
    }
        // Desktop mouse left = paint/erase
    else if (touchCount == 0 && IsMouseButtonDown(MOUSE_BUTTON_LEFT))
    {
        pointer = GetMousePosition();
        pointerDown = true;
    }

    if (io.WantCaptureMouse)
    {
        m_PointerWasDown = pointerDown;
        m_HasLastPaintPointer = false;
        return;
    }

    if (pointerDown)
    {
        if (!m_PointerWasDown)
        {
            // First touch/down: paint immediately
            PaintAtScreen(pointer);
            m_LastPaintPointer = pointer;
            m_HasLastPaintPointer = true;
        }
        else
        {
            // While dragging: paint the line between last point and current point
            if (m_HasLastPaintPointer)
                PaintStroke(m_LastPaintPointer, pointer);
            else
                PaintAtScreen(pointer);

            m_LastPaintPointer = pointer;
            m_HasLastPaintPointer = true;
        }
    }
    else
    {
        m_HasLastPaintPointer = false;
    }

    m_PointerWasDown = pointerDown;
}



//void WorkMapEditor::HandlePaintInput()
//{
//    ImGuiIO& io = ImGui::GetIO();
//
//    // If we're currently panning, do not also paint.
//    if (m_IsPanning)
//    {
//        m_PointerWasDown = false;
//        return;
//    }
//
//    bool pointerDown = false;
//    Vector2 pointer{};
//
//    const int touchCount = GetTouchPointCount();
//
//    // One-finger touch = paint
//    if (touchCount == 1)
//    {
//        pointer = GetTouchPosition(0);
//        pointerDown = true;
//    }
//        // Mouse left button = paint
//    else if (touchCount == 0 && IsMouseButtonDown(MOUSE_BUTTON_LEFT))
//    {
//        pointer = GetMousePosition();
//        pointerDown = true;
//    }
//
//    if (io.WantCaptureMouse)
//    {
//        m_PointerWasDown = pointerDown;
//        return;
//    }
//
//    // Tap once = place/overwrite
//    if (pointerDown && !m_PointerWasDown)
//    {
//        int cellX = 0;
//        int cellY = 0;
//
//        if (ScreenToTile(pointer, cellX, cellY))
//        {
//            int& cell = m_Tiles[FlatIndex(cellX, cellY)];
//
//            cell = m_EraseMode ? m_DefaultIndex : m_SelectedIndex;
//
//
//        }
//    }
//
//    m_PointerWasDown = pointerDown;
//}

int WorkMapEditor::FillVisibleDefaultTiles()
{
    int changedCount = 0;

    // Fast path for normal grid mode
    if (!m_DrawIsometric)
    {
        const float tileW = (float)m_TileSize;
        const float tileH = (float)m_TileSize;

        const int startX = std::max(0, (int)floorf((-m_MapOrigin.x) / tileW));
        const int startY = std::max(0, (int)floorf((-m_MapOrigin.y) / tileH));

        const int endX = std::min(
                m_MapWidth,
                (int)ceilf(((float)GetScreenWidth() - m_MapOrigin.x) / tileW) + 1
        );

        const int endY = std::min(
                m_MapHeight,
                (int)ceilf(((float)GetScreenHeight() - m_MapOrigin.y) / tileH) + 1
        );

        for (int y = startY; y < endY; ++y)
        {
            for (int x = startX; x < endX; ++x)
            {
                int& cell = m_Tiles[FlatIndex(x, y)];
                if (cell == m_DefaultIndex)
                {
                    cell = m_SelectedIndex;
                    ++changedCount;
                }
            }
        }

        return changedCount;
    }

    // Safe fallback for isometric mode
    for (int y = 0; y < m_MapHeight; ++y)
    {
        for (int x = 0; x < m_MapWidth; ++x)
        {
            if (!IsTileVisibleOnScreen(x, y))
                continue;

            int& cell = m_Tiles[FlatIndex(x, y)];
            if (cell == m_DefaultIndex)
            {
                cell = m_SelectedIndex;
                ++changedCount;
            }
        }
    }

    return changedCount;
}

bool WorkMapEditor::IsTileVisibleOnScreen(int x, int y) const {
    Vector2 pos = GetTileDrawPosition(x, y);

    const float tileW = (float) m_TileSize;
    const float tileH = (float) m_TileSize;

    const float screenW = (float) GetScreenWidth();
    const float screenH = (float) GetScreenHeight();

    // Simple screen-rect overlap test
    if (pos.x + tileW <= 0.0f) return false;
    if (pos.y + tileH <= 0.0f) return false;
    if (pos.x >= screenW) return false;
    if (pos.y >= screenH) return false;

    return true;

}

void WorkMapEditor::DrawTilePickerPopup()
{
    if (!m_HasTilesetTexture) return;

    ImGuiIO& io = ImGui::GetIO();

    const ImVec2 modalSize(io.DisplaySize.x * 0.88f, io.DisplaySize.y * 0.88f);
    const ImVec2 modalCenter(io.DisplaySize.x * 0.50f, io.DisplaySize.y * 0.50f);

    ImGui::SetNextWindowPos(modalCenter, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
    ImGui::SetNextWindowSize(modalSize, ImGuiCond_Appearing);

    const ImGuiWindowFlags flags =
            ImGuiWindowFlags_NoCollapse |
            ImGuiWindowFlags_NoResize |
            ImGuiWindowFlags_NoSavedSettings;

    if (ImGui::BeginPopupModal("Tile Picker", nullptr, flags))
    {
        ImGui::Text("Tap a tile to select it.");
        ImGui::Text("Selected index: %d", m_SelectedIndex);
        ImGui::Separator();

        const float texW = (float)m_TilesetTexture.width;
        const float texH = (float)m_TilesetTexture.height;

        // Leave room for footer buttons/text
        ImVec2 avail = ImGui::GetContentRegionAvail();
        const float footerReserve = 72.0f;
        const float fitW = avail.x;
        const float fitH = std::max(64.0f, avail.y - footerReserve);

        float scaleX = fitW / texW;
        float scaleY = fitH / texH;
        float scale = std::min(scaleX, scaleY);

        // Let small tilesheets scale up so they are finger-friendly.
        scale = std::max(scale, 1.0f);

        const ImVec2 imageSize(texW * scale, texH * scale);

        ImVec2 imageMin = ImGui::GetCursorScreenPos();
        ImVec2 imageMax(imageMin.x + imageSize.x, imageMin.y + imageSize.y);

        ImGui::InvisibleButton("##tilesheet_picker", imageSize);

        ImDrawList* drawList = ImGui::GetWindowDrawList();

        drawList->AddImage(
                (ImTextureID)(intptr_t)m_TilesetTexture.id,
                imageMin,
                imageMax,
                ImVec2(0.0f, 0.0f),
                ImVec2(1.0f, 1.0f)
        );

        // Draw tilesheet grid
        const float scaledTile = (float)m_TileSize * scale;

        for (int x = 0; x <= m_TilesetColumns; ++x)
        {
            const float px = imageMin.x + x * scaledTile;
            drawList->AddLine(
                    ImVec2(px, imageMin.y),
                    ImVec2(px, imageMax.y),
                    IM_COL32(255, 255, 255, 80)
            );
        }

        for (int y = 0; y <= m_TilesetRows; ++y)
        {
            const float py = imageMin.y + y * scaledTile;
            drawList->AddLine(
                    ImVec2(imageMin.x, py),
                    ImVec2(imageMax.x, py),
                    IM_COL32(255, 255, 255, 80)
            );
        }

        // Highlight current selection
        {
            const int selX = m_SelectedIndex % m_TilesetColumns;
            const int selY = m_SelectedIndex / m_TilesetColumns;

            ImVec2 selMin(
                    imageMin.x + selX * scaledTile,
                    imageMin.y + selY * scaledTile
            );
            ImVec2 selMax(
                    selMin.x + scaledTile,
                    selMin.y + scaledTile
            );

            drawList->AddRect(selMin, selMax, IM_COL32(255, 255, 0, 255), 0.0f, 0, 3.0f);
        }

        int hoveredIndex = -1;

        if (ImGui::IsItemHovered())
        {
            ImVec2 mouse = io.MousePos;

            const float localX = mouse.x - imageMin.x;
            const float localY = mouse.y - imageMin.y;

            const int tx = (int)(localX / scaledTile);
            const int ty = (int)(localY / scaledTile);

            if (tx >= 0 && tx < m_TilesetColumns &&
                ty >= 0 && ty < m_TilesetRows)
            {
                hoveredIndex = ty * m_TilesetColumns + tx;

                ImVec2 hoverMin(
                        imageMin.x + tx * scaledTile,
                        imageMin.y + ty * scaledTile
                );
                ImVec2 hoverMax(
                        hoverMin.x + scaledTile,
                        hoverMin.y + scaledTile
                );

                drawList->AddRect(hoverMin, hoverMax, IM_COL32(0, 255, 255, 255), 0.0f, 0, 2.0f);

                if (ImGui::IsItemClicked(ImGuiMouseButton_Left))
                {
                    m_SelectedIndex = hoveredIndex;
                    ClampSelectedIndex();
                    ImGui::CloseCurrentPopup();
                }
            }
        }

        ImGui::Spacing();

        if (hoveredIndex >= 0)
            ImGui::Text("Hover tile: %d", hoveredIndex);
        else
            ImGui::Text("Hover tile: none");

        ImGui::SameLine();

        if (ImGui::Button("Cancel", ImVec2(140.0f, 44.0f)))
            ImGui::CloseCurrentPopup();

        ImGui::EndPopup();
    }
}