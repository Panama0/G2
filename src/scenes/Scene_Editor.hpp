#pragma once

#include "scene base/Scene.hpp"
#include "scene base/GameMap.hpp"

class Scene_Editor : public Scene
{
public:
    enum class ActionTypes
    {
        null,
        toggleFS,
        toggleGrid,
        place,
        remove,
        rotate,
        save,
        load,
    };
    
    enum class TileTypes
    {
        null,
        ground,
        water,
        rock,
        path,
        spawner
    };
    
    Scene_Editor() = default;
    Scene_Editor(GameEngine* game)
        :Scene(game)
    { init(); }
    
    void init();
    void update();
    void sDoAction(const Action& action);
    void sRender();
private:
    void drawUI();
    void drawTilesUI();
    void drawMainMenuBarUI();
    void drawSaveLoadUI();
    
    void placeSelectedTile(const sf::Vector2f& pos);
    
    bool m_gridVisible {true};
    std::string m_selectedTile;
    sf::Angle m_rotation;
    GameMap m_gameMap;
    bool m_placing {true};
    
    //* --- FOR UI ---
    sf::RenderTexture m_currentTileRenderTex {{200,200}};
    bool m_showSaveLoad {false};
    bool m_showTilesUI {true};
    bool m_showToolsUI {false};
    std::string m_fileName {"Unnamed Level"};
    std::filesystem::path m_dir {"../../res/saves/"};
    
    sf::Vector2u m_gridSize;
};