#include "scenes/Scene_Editor.hpp"
#include "Buttons.hpp"
#include "SFML/Graphics/Rect.hpp"
#include "SFML/Graphics/Sprite.hpp"
#include "SFML/System/Vector2.hpp"
#include "imgui-SFML.h"
#include "imgui.h"
#include "scene base/Components.hpp"

#include <optional>

void Scene_Editor::init()
{
    // WARN: temp
    m_gridSize = {32, 32};
    m_worldSize = {1280.f, 720.f};
    m_hasGui = true;

    m_state.map.init(
        static_cast<sf::Vector2u>(m_worldSize), m_gridSize, &m_assets);

    registerAction(Buttons::F, static_cast<int>(ActionTypes::toggleFS));
    registerAction(Buttons::G, static_cast<int>(ActionTypes::toggleGrid));
    registerAction(Buttons::B, static_cast<int>(ActionTypes::toggleBrushes));
    registerAction(Buttons::MouseLeft, static_cast<int>(ActionTypes::place));
    registerAction(Buttons::MouseRight, static_cast<int>(ActionTypes::remove));
    registerAction(Buttons::S, static_cast<int>(ActionTypes::save));
    registerAction(Buttons::L, static_cast<int>(ActionTypes::load));
    registerAction(Buttons::Escape, static_cast<int>(ActionTypes::deselect));

    m_assets.loadTextureDir("tiles");

    m_effectTextures = {{TileEffect::none, "tile9"},
                        {TileEffect::water, "tile9"},
                        {TileEffect::obstructed, "tile9"},
                        {TileEffect::path, "tile9"},
                        {TileEffect::spawner, "tile9"}};

    if(!m_assets.getTextureList().empty())
    {
        m_state.tileTexture = m_assets.getTextureList().front();
    }

    m_editorUI.init(m_game->getWindow());
}

void Scene_Editor::onExit()
{
    m_game->startScene(std::make_shared<Scene_MainMenu>(m_game));
}

void Scene_Editor::update()
{
    for(auto& entity : m_entities.getEntities<cTransform, cSprite>())
    {
        auto& spr = m_entities.getComponent<cSprite>(entity).sprite;
        auto& transform = m_entities.getComponent<cTransform>(entity);
        spr.setPosition(transform.pos);
        spr.setScale(transform.scale);
    }

    // update state and pass to the scene;
    m_state.textureList = m_assets.getTextureList();
    m_editorUI.updateState(&m_state);

    m_editorUI.update(m_game->getDT());
    ImGui::ShowDemoWindow();
    sRender();
}

void Scene_Editor::sDoAction(const Action& action)
{
    switch(action.type())
    {
    case static_cast<int>(ActionTypes::toggleFS):
        if(action.status() == Action::end)
        {
            m_game->getWindow().toggleFullscreen();
        }
        break;

    case static_cast<int>(ActionTypes::toggleGrid):
        if(action.status() == Action::end)
        {
                m_state.map.toggleGrid();
        }
        break;

    case static_cast<int>(ActionTypes::toggleBrushes):
        if(action.status() == Action::end)
        {
            if(m_state.brushesVisible)
            {
                m_state.brushesVisible = false;
            }
            else
            {
                m_state.brushesVisible = true;
            }
        }
        break;

    case static_cast<int>(ActionTypes::place):
        if(action.status() == Action::start)
        {
            switch(m_state.currentMode)
            {
            case EditorState::Modes::none:
                break;
            case EditorState::Modes::tilePlaceTileRemove:
                placeSelectedTile(action.mousePosition());
                break;
            case EditorState::Modes::brushPlaceBrushRemove:
                placeSelectedBrush(action.mousePosition());
                break;
            case EditorState::Modes::selectNone:
                select(action.mousePosition());
                break;
            }
        }
        break;

    case static_cast<int>(ActionTypes::remove):
        if(action.status() == Action::end)
        {
            if(m_state.currentMode == EditorState::Modes::tilePlaceTileRemove)
            {
                if(m_state.map.getTileAt(action.mousePosition()))
                {
                    m_state.map.removeTile(action.mousePosition());
                }
            }
            else if(m_state.currentMode
                    == EditorState::Modes::brushPlaceBrushRemove)
            {
                auto tile = m_state.map.getTileAt(action.mousePosition());

                if(tile)
                {
                    m_state.map.clearBrushes(action.mousePosition());

                    for(auto& ent :
                        m_entities.getEntities<cTileEffect, cTransform>())
                    {
                        auto& entityPos
                            = m_entities.getComponent<cTransform>(ent).pos;

                        if(m_state.map.getTileAt(entityPos)->pos == tile->pos)
                        {
                            m_entities.killEntity(ent);
                        }
                    }
                }
            }
        }
        break;

    case static_cast<int>(ActionTypes::deselect):
        if(action.status() == Action::end)
        {
            deSelect();
        }
        break;

    case static_cast<int>(ActionTypes::save):
        if(action.status() == Action::end)
        {
            m_state.map.save(m_state.savePath / m_state.saveName);
        }
        break;

    case static_cast<int>(ActionTypes::load):
        if(action.status() == Action::end)
        {
            m_state.map.load(m_state.savePath / m_state.saveName);

            // remove old brushes and add the new ones

            for(auto ent : m_entities.getEntities<cTileEffect>())
            {
                m_entities.killEntity(ent);
            }

            for(const auto& tile : m_state.map.getTiles())
            {
                if(!tile.effects.empty())
                {
                    // there may be multiple effects, so we loop
                    for(const auto& brush : tile.effects)
                    {
                        spawnBrush(tile, brush);
                    }
                }
            }
        }
        break;

    case static_cast<int>(ActionTypes::exit):
        if(action.status() == Action::end)
        {
            m_game->quit();
        }
        break;

    case static_cast<int>(ActionTypes::endScene):
        if(action.status() == Action::end)
        {
            m_hasEnded = true;
        }
        break;
    }
}

void Scene_Editor::sRender()
{
    auto& window = m_game->getWindow();
    window.beginDraw();

    sf::Sprite map{m_state.map.getTexture()};
    window.draw(map);

    if(m_state.brushesVisible)
    {
        for(auto& ent : m_entities.getEntities<cSprite, cTileEffect>())
        {
            auto& spr = m_entities.getComponent<cSprite>(ent);
            m_game->getWindow().draw(spr.sprite);
        }
    }

    drawUI();
    window.render();
}

void Scene_Editor::drawUI()
{
    m_editorUI.draw();
    m_editorUI.display();
}

void Scene_Editor::placeSelectedTile(const sf::Vector2f& pos)
{
    if(!m_game->getWindow().isInsideView(pos))
    {
        return;
    }

    m_state.map.placeTile(pos, m_state.angle, m_state.tileTexture);
}

void Scene_Editor::placeSelectedBrush(const sf::Vector2f& pos)
{
    if(!m_game->getWindow().isInsideView(pos))
    {
        return;
    }

    if(!m_state.map.getTileAt(pos))
    {
        std::cerr << "No tile at this location!\n";
        return;
    }
    // we know that there is a tile at this location now
    GameMap::MapTile tile = m_state.map.getTileAt(pos).value();

    // place on the map
    m_state.map.placeBrush(m_state.brushType, pos);

    // spawn the entity
    spawnBrush(tile, m_state.brushType);
}

void Scene_Editor::spawnBrush(const GameMap::MapTile& tile,
                              const TileEffect& effect)
{
    auto entitiy = m_entities.addEntity();

    if(m_effectTextures.find(effect.type) == m_effectTextures.end())
    {
        std::cerr << "Could not find effect texture!\n";
        return;
    }

    std::string_view texName = m_effectTextures.at(effect.type);

    auto& spr = m_entities.addComponent<cSprite>(entitiy).sprite;
    // set the texture and resize the shape
    spr.setTexture(m_assets.getTexture(texName), true);
    auto& tex = spr.getTexture();

    spr.setOrigin({tex.getSize().x / 2.f, tex.getSize().y / 2.f});
    // lower the opacity so that we can see the original tile
    spr.setColor(sf::Color{255, 255, 255, 150});

    auto& transform = m_entities.addComponent<cTransform>(entitiy);
    transform.scale = {0.8f, 0.8f};
    transform.pos = m_state.map.toWorldPos(tile.pos);

    auto& eff = m_entities.addComponent<cTileEffect>(entitiy).effect;
    eff = effect;
}

void Scene_Editor::select(const sf::Vector2f& pos)
{
    m_state.selectedTile = m_state.map.getTileAt(pos);
    if(m_state.selectedTile)
    {
        m_state.selectedTilePos = m_game->getWindow().coordsToPixel(pos);
        // offset to the right edge of the tile
        m_state.selectedTilePos.x += m_gridSize.x;
    }
}

void Scene_Editor::deSelect() { m_state.selectedTile = std::nullopt; }
