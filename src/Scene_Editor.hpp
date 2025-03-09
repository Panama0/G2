#pragma once

#include "Scene.hpp"
#include "GameMap.hpp"

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
    
    bool m_gridVisible {false};
    
    //* --- FOR UI ---
    sf::Texture m_selectedSpr;
    // not sure the type here yet
    sf::Angle rotation;
    GameMap m_gameMap;
    
    sf::Vector2u m_gridSize;
};