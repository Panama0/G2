#pragma once

#include "scene base/Scene.hpp"
#include "scene base/EditorUI.hpp"
#include "scene base/EditorState.hpp"


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
    
    //! need to remove this in favour of the assetList in the editorState struct
    Assets& getAssets() { return m_assets; }
private:
    void drawUI();
    
    void placeSelectedTile(const sf::Vector2f& pos);
    
    EditorState m_state;   
    EditorUI m_editorUI {this};
    
    sf::Vector2u m_gridSize;
};