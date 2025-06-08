#pragma once

#include "Scene.hpp"
#include "scene base/EditorState.hpp"
#include "scene base/EditorUI.hpp"
#include "scenes/Scene_MainMenu.hpp"
#include <memory>

class Scene_Editor : public Scene
{
public:
    enum class ActionTypes
    {
        null,
        toggleFS,
        toggleGrid,
        toggleBrushes,
        place,
        remove,
        deselect,
        rotate,
        save,
        load,
        exit,
        endScene,
    };

    Scene_Editor() = default;
    Scene_Editor(GameEngine* game) : Scene(game) { init(); }

    void init() override;
    void onExit() override;
    void update() override;
    void sDoAction(const Action& action) override;
    void sRender() override;

    // TODO: need to remove this in favour of the assetList in the editorState
    //  struct
    Assets& getAssets() { return m_assets; }

private:
    void drawUI();

    void placeSelectedTile(const sf::Vector2f& pos);
    void placeSelectedBrush(const sf::Vector2f& pos);
    void spawnBrush(const GameMap::MapTile& tile, const TileEffect& effect);
    void select(const sf::Vector2f& pos);
    void deSelect();

    std::unordered_map<TileEffect::Effects, std::string_view> m_effectTextures;

    EditorState m_state;
    EditorUI m_editorUI{this};

    //WARN: maybe temp:
    Grid* m_mapGrid;

    sf::Vector2u m_gridSize;
};
