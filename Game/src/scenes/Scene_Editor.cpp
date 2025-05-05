#include "scenes/Scene_Editor.hpp"
#include "imgui-SFML.h"
#include "imgui.h"

void Scene_Editor::init()
{
    //* temp
    m_gridSize = {32, 32};
    m_hasGui = true;

    m_assets.setResourceDir("../../../Game/res/");
    m_globalGrid.init(m_game->getWindow().getSize(), m_gridSize);

    registerAction(sf::Keyboard::Key::F,
                   static_cast<int>(ActionTypes::toggleFS));
    registerAction(sf::Keyboard::Key::G,
                   static_cast<int>(ActionTypes::toggleGrid));
    registerAction(sf::Keyboard::Key::B,
                   static_cast<int>(ActionTypes::toggleBrushes));
    registerAction(sf::Mouse::Button::Left,
                   static_cast<int>(ActionTypes::place));
    registerAction(sf::Mouse::Button::Right,
                   static_cast<int>(ActionTypes::remove));
    registerAction(sf::Keyboard::Key::S, static_cast<int>(ActionTypes::save));
    registerAction(sf::Keyboard::Key::L, static_cast<int>(ActionTypes::load));
    registerAction(sf::Keyboard::Key::Escape,
                   static_cast<int>(ActionTypes::deselect));

    m_assets.loadTextureDir("../../../Game/res/tiles/");

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

void Scene_Editor::update()
{
    m_entities.update();

    for(auto& entity : m_entities.getEntities())
    {
        if(entity->has<cTransform>() && entity->has<cSprite>())
        {
            auto& spr = entity->get<cSprite>().sprite;
            const auto& transform = entity->get<cTransform>();
            spr.setPosition(transform.pos);
            spr.setScale(transform.scale);
        }
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
            if(m_state.gridVisible)
            {
                m_state.gridVisible = false;
            }
            else
            {
                m_state.gridVisible = true;
            }
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
                placeSelectedTile(
                    m_globalGrid.getGridAt(action.position()).midPos);
                break;
            case EditorState::Modes::brushPlaceBrushRemove:
                placeSelectedBrush(
                    m_globalGrid.getGridAt(action.position()).midPos);
                break;
            case EditorState::Modes::selectNone:
                select(action.position());
                break;
            }
        }
        break;

    case static_cast<int>(ActionTypes::remove):
        if(action.status() == Action::end)
        {
            if(m_state.currentMode == EditorState::Modes::tilePlaceTileRemove)
            {
                auto pos = m_globalGrid.getGridAt(action.position());
                auto tile = m_state.map.getTileAt(pos.midPos);
                if(tile)
                {
                    m_state.map.removeTile(tile.value().id);
                }
            }
            else if(m_state.currentMode
                    == EditorState::Modes::brushPlaceBrushRemove)
            {
                auto pos = m_globalGrid.getGridAt(action.position());
                auto brushes = m_entities.getEntities("Brush");
                auto tile = m_state.map.getTileAt(pos.midPos);

                if(tile)
                {
                    for(const auto& brush : brushes)
                    {
                        if(brush->get<cTransform>().pos == pos.midPos)
                        {
                            brush->destroy();
                            m_state.map.removeEffect(tile.value(),
                                                     brush->get<cId>().id);
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

            //* must be a better way
            // remove old brushes and add the new ones
            for(const auto& brush : m_entities.getEntities("Brush"))
            {
                brush->destroy();
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

    if(m_state.gridVisible)
    {
        sf::Sprite grid{m_globalGrid.getTexture()};
        window.draw(grid);
    }

    auto tiles = m_state.map.getTiles();
    for(auto& tile : tiles)
    {
        sf::Texture tex{m_assets.getTexture(tile.textureName)};
        sf::Sprite spr{tex};
        spr.setPosition(tile.pos);
        spr.setOrigin({tex.getSize().x / 2.f, tex.getSize().y / 2.f});

        window.draw(spr);
    }

    for(const auto& entity : m_entities.getEntities())
    {
        if(entity->has<cSprite>())
        {
            if(entity->has<cEffect>() && !m_state.brushesVisible)
            {
                // do not draw the brushes if the flag is off
                continue;
            }
            auto& spr = entity->get<cSprite>();
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

    GameMap::MapTile tile{m_globalGrid.getGridAt(pos).midPos,
                          m_state.angle,
                          m_state.tileTexture};
    m_state.map.placeTile(tile);
    // do other stuff
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

    //* maybe combine these calls
    m_state.map.placeEffect(tile.id, m_state.brushType);
    //* this creates 2 effect ids for the one effect
    spawnBrush(tile, m_state.brushType);
}

void Scene_Editor::spawnBrush(const GameMap::MapTile& tile,
                              const TileEffect& effect)
{
    auto entitiy = m_entities.addEntity("Brush");

    if(m_effectTextures.find(effect.effect) == m_effectTextures.end())
    {
        std::cerr << "Could not find effect texture!\n";
        return;
    }

    std::string_view texName = m_effectTextures.at(effect.effect);

    auto& spr = entitiy->add<cSprite>(m_assets.getTexture(texName)).sprite;
    auto& tex = spr.getTexture();
    spr.setOrigin({tex.getSize().x / 2.f, tex.getSize().y / 2.f});
    // lower the opacity so that we can see the original tile
    spr.setColor(sf::Color{255, 255, 255, 150});

    auto& transform
        = entitiy->add<cTransform>(m_globalGrid.getGridAt(tile.pos).midPos);
    transform.scale = {0.8f, 0.8f};

    entitiy->add<cId>(tile.id);
}

void Scene_Editor::select(const sf::Vector2f& pos)
{
    const auto& gridPos = m_globalGrid.getGridAt(pos);
    auto tile = m_state.map.getTileAt(gridPos.midPos);

    if(tile)
    {
        m_state.selectedTile = tile.value();
    }
}

void Scene_Editor::deSelect() { m_state.selectedTile = {}; }
