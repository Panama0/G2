#pragma once

#include "scene base/EditorState.hpp"
#include "scene base/EditorUI.hpp"
#include "Scene.hpp"

#include "scenes/Scene_MainMenu.hpp"

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
    Scene_Editor(GameEngine* game, uint32_t id) : Scene(game, id) { init(); }

    void init() override;
    void update() override;
    void sDoAction(const Action& action) override;
    void sRender() override;
    void end() override { m_game->changeScene<Scene_MainMenu>(); }

    //! need to remove this in favour of the assetList in the editorState
    //! struct
    Assets& getAssets() { return m_assets; }

private:
    void drawUI();

    void placeSelectedTile(const sf::Vector2f& pos);
    void placeSelectedBrush(const sf::Vector2f& pos);
    void spawnBrush(const GameMap::MapTile& tile, const TileEffect& effect);
    void select(const sf::Vector2f& pos);
    void deSelect();

    //* maybe temporary
    std::unordered_map<TileEffect::Effects, std::string_view> m_effectTextures;

    EditorState m_state;
    EditorUI m_editorUI{this};

    sf::Vector2u m_gridSize;
};