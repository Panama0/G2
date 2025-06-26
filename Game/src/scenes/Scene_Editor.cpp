#include "scenes/Scene_Editor.hpp"
#include "Buttons.hpp"
#include "SFML/Graphics/Sprite.hpp"
#include "SFML/System/Vector2.hpp"
#include "Vec2.hpp"
#include "imgui-SFML.h"
#include "imgui.h"
#include "scene base/Components.hpp"
#include "scene base/EditorState.hpp"
#include "scene base/TileEffect.hpp"
#include "scenes/Scene_MainMenu.hpp"

void Scene_Editor::init()
{
    // NOTE: temp
    m_gridSize = {32, 32};
    m_worldSize = {1280.f, 720.f};
    m_hasGui = true;

    m_state.map.init(static_cast<Vec2i>(m_worldSize), m_gridSize, &m_assets);

    registerAction(Buttons::F, static_cast<int>(ActionTypes::toggleFS));
    registerAction(Buttons::G, static_cast<int>(ActionTypes::toggleGrid));
    registerAction(Buttons::B, static_cast<int>(ActionTypes::toggleBrushes));
    registerAction(Buttons::MouseLeft,
                   static_cast<int>(ActionTypes::toggleLeftC));
    registerAction(Buttons::MouseRight,
                   static_cast<int>(ActionTypes::toggleRightC));
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
    // place/remove any tiles if applicable
    handleMouse();

    // update entities
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
    switch(static_cast<ActionTypes>(action.type()))
    {
    case ActionTypes::toggleFS:
        if(action.status() == Action::end)
        {
            m_game->getWindow().toggleFullscreen();
        }
        break;

    case ActionTypes::toggleGrid:
        if(action.status() == Action::start)
        {
            m_state.map.toggleGrid();
        }
        break;

    case ActionTypes::toggleBrushes:
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

    case ActionTypes::toggleLeftC:
        if(action.status() == Action::start)
        {
            m_state.mouseState.state = EditorState::Mouse::leftDown;
        }
        else if(action.status() == Action::end)
        {
            m_state.mouseState.reset();
        }
        break;

    case ActionTypes::toggleRightC:
        if(action.status() == Action::start)
        {
            m_state.mouseState.state = EditorState::Mouse::rightDown;
        }
        else if(action.status() == Action::end)
        {
            m_state.mouseState.reset();
        }
        break;

    case ActionTypes::deselect:
        if(action.status() == Action::end)
        {
            deSelect();
        }
        break;

    case ActionTypes::save:
        if(action.status() == Action::end)
        {
            m_state.map.save(m_state.savePath / m_state.saveName);
        }
        break;

    case ActionTypes::load:
        if(action.status() == Action::end)
        {
            m_state.map.load(m_state.savePath / m_state.saveName);

            // remove old brushes and add the new ones

            for(auto ent : m_entities.getEntities<cEffect>())
            {
                m_entities.killEntity(ent);
            }

            for(const auto& tile : m_state.map.getTiles())
            {
                if(tile && !tile->staticEffects.empty())
                {
                    // there may be multiple effects, so we loop
                    for(const auto& brush : tile->staticEffects)
                    {
                        spawnBrush(*tile, brush);
                    }
                }
            }
        }
        break;

    case ActionTypes::exit:
        if(action.status() == Action::end)
        {
            m_game->quit();
        }
        break;

    case ActionTypes::endScene:
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
        for(auto& ent : m_entities.getEntities<cSprite>())
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

void Scene_Editor::handleMouse()
{
    if(m_state.mouseState.state != EditorState::Mouse::none)
    {
        auto& window = m_game->getWindow();
        auto mousePos = window.pixelToCoords(window.getMousePos());

        // if we left the view area, we dont need to continue processing
        if(!window.isInsideView(mousePos))
        {
            return;
        }
        auto gridPos = m_state.map.toGridPos(mousePos);

        if(gridPos != m_state.mouseState.lastPlaced
           || !m_state.mouseState.lastPlaced)
        {
            switch(m_state.mouseState.state)
            {
            case EditorState::Mouse::leftDown:
                place(mousePos);
                break;

            case EditorState::Mouse::rightDown:
                remove(mousePos);
                break;

            case EditorState::Mouse::none:
                break;
            }

            m_state.mouseState.lastPlaced = gridPos;
        }
    }
}

void Scene_Editor::remove(const Vec2f& pos)
{
    if(m_state.currentMode == EditorState::Mode::tilePlaceTileRemove)
    {
        sUpdateBrushes(pos);
        m_state.map.clearTile(pos);
    }
    else if(m_state.currentMode == EditorState::Mode::brushPlaceBrushRemove)
    {
        if(m_state.map.getTile(pos))
        {
            m_state.map.clearBrushes(pos);
            sUpdateBrushes(pos);
        }
    }
}

void Scene_Editor::place(const Vec2f& pos)
{
    switch(m_state.currentMode)
    {
    case EditorState::Mode::none:
        break;
    case EditorState::Mode::tilePlaceTileRemove:
        placeSelectedTile(pos);
        break;
    case EditorState::Mode::brushPlaceBrushRemove:
        placeSelectedBrush(pos);
        break;
    case EditorState::Mode::selectNone:
        select(pos);
        break;
    }
}

void Scene_Editor::placeSelectedTile(const Vec2f& pos)
{
    if(!m_game->getWindow().isInsideView(pos))
    {
        return;
    }

    m_state.map.placeTile(pos, m_state.angle, m_state.tileTexture);
}

void Scene_Editor::placeSelectedBrush(const Vec2f& pos)
{
    if(!m_game->getWindow().isInsideView(pos))
    {
        return;
    }

    auto tile = m_state.map.getTile(pos);
    if(tile)
    {
        // place on the map
        m_state.map.placeBrush(m_state.brushType, pos);

        // spawn the entity
        spawnBrush(*tile, m_state.brushType);
    }
}

void Scene_Editor::spawnBrush(const GameMap::MapTile& tile,
                              TileEffect::Effects effect)
{
    auto entitiy = m_entities.addEntity();

    if(m_effectTextures.find(effect) == m_effectTextures.end())
    {
        std::cerr << "Could not find effect texture!\n";
        return;
    }

    std::string_view texName = m_effectTextures.at(effect);

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

    m_entities.addComponent<cEffect>(entitiy).effect = effect;
}

void Scene_Editor::sUpdateBrushes(const Vec2f& pos)
{
    for(auto& ent : m_entities.getEntities<cEffect, cTransform>())
    {
        auto& entityPos = m_entities.getComponent<cTransform>(ent).pos;

        if(m_state.map.getTile(entityPos)->pos == m_state.map.toGridPos(pos))
        {
            m_entities.killEntity(ent);
        }
    }
}

void Scene_Editor::select(const Vec2f& pos)
{
    m_state.selectedTile = m_state.map.getTile(pos);
    if(m_state.selectedTile)
    {
        m_state.selectedTilePos
            = static_cast<Vec2f>(m_game->getWindow().coordsToPixel(pos));
        // offset to the right edge of the tile
        m_state.selectedTilePos.x += m_gridSize.x;
    }
}

void Scene_Editor::deSelect() { m_state.selectedTile = nullptr; }
