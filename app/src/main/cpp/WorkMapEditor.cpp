//
// Created by jlhar on 3/10/2026.
//
#include "WorkMapEditor.h"

#include <algorithm>
#include <cstdint>
#include <fstream>
#include <sstream>

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

    std::string tilesetPath = std::string("textures/tilesets/") + m_TilesetFileName;
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
            dst.width  = (float)m_TileSize;
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

    ImGui::Begin("Tile Editor###tile_editor", nullptr, flags);

    ImGui::Text("Tileset: %s", m_TilesetFileName.c_str());
    ImGui::Text("Tileset grid: %d x %d", m_TilesetColumns, m_TilesetRows);
    ImGui::Text("Tile size: %d", m_TileSize);
    ImGui::Text("Map size: %d x %d", m_MapWidth, m_MapHeight);
    ImGui::Text("Default index: %d", m_DefaultIndex);

    ImGui::Separator();

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

#if IMGUI_VERSION_NUM >= 19198
        ImTextureRef texRef = (ImTextureID)(intptr_t)m_TilesetTexture.id;
        ImGui::Image(texRef, ImVec2(96.0f, 96.0f), uv0, uv1);
#else
        ImGui::Image((ImTextureID)(intptr_t)m_TilesetTexture.id, ImVec2(96.0f, 96.0f), uv0, uv1);
#endif
    }

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

    if (!m_LastSaveMessage.empty())
        ImGui::TextWrapped("%s", m_LastSaveMessage.c_str());

    ImGui::End();
}

void WorkMapEditor::HandlePaintInput()
{
    // Dear ImGui says this is the right way to know whether input should go
    // to the UI or to the underlying app/editor.
    ImGuiIO& io = ImGui::GetIO();

    bool pointerDown = false;
    Vector2 pointer{};

    if (GetTouchPointCount() > 0)
    {
        pointer = GetTouchPosition(0);
        pointerDown = true;
    }
    else if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
    {
        pointer = GetMousePosition();
        pointerDown = true;
    }

    if (io.WantCaptureMouse)
    {
        m_PointerWasDown = pointerDown;
        return;
    }

    // Tap once = place/overwrite.
    if (pointerDown && !m_PointerWasDown)
    {
        int cellX = 0;
        int cellY = 0;

        if (ScreenToTile(pointer, cellX, cellY))
        {
            int& cell = m_Tiles[FlatIndex(cellX, cellY)];

            if (cell == m_SelectedIndex)
                cell = m_DefaultIndex;
            else
                cell = m_SelectedIndex;
        }
    }

    m_PointerWasDown = pointerDown;
}