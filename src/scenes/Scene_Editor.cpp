#include "scenes/Scene_Editor.hpp"
#include "imgui-SFML.h"
#include "imgui.h"

void Scene_Editor::init()
{
    //* temp
    m_gridSize = {32, 32};
    m_hasGui = true;
    
    m_assets.setResourceDir("../../res/");
    m_globalGrid.init(m_game->getWindow().getSize(), m_gridSize);
    
    registerAction(sf::Keyboard::Key::F, static_cast<int>(ActionTypes::toggleFS));
    registerAction(sf::Keyboard::Key::G, static_cast<int>(ActionTypes::toggleGrid));
    registerAction(sf::Mouse::Button::Left, static_cast<int>(ActionTypes::place));
    registerAction(sf::Mouse::Button::Right, static_cast<int>(ActionTypes::remove));
    registerAction(sf::Keyboard::Key::S, static_cast<int>(ActionTypes::save));
    registerAction(sf::Keyboard::Key::L, static_cast<int>(ActionTypes::load));
    
    m_assets.loadTextureDir("../../res/tiles/");
    
    m_state.selectedTile = m_assets.getTextureList().at(0);
    
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
    m_state.entities = m_entities.getEntities();
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
        
        case static_cast<int>(ActionTypes::place):
            if(action.status() == Action::start)
            {
                switch(m_state.currentMode)
                {
                    case EditorState::Modes::none:
                        break;
                    case EditorState::Modes::tilePlaceTileRemove:
                        placeSelectedTile(action.position());
                        break;
                    case EditorState::Modes::brushPlaceBrushRemove:
                        placeSelectedBrush(action.position());
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
                    auto tiles = m_state.map.getTilesAt(pos.midPos);
                    for(auto& tile : tiles)
                    {
                        remove(tile);
                    }
                }
                else if (m_state.currentMode == EditorState::Modes::brushPlaceBrushRemove)
                {
                    auto pos = m_globalGrid.getGridAt(action.position());
                    auto brushes = m_entities.getEntities("Brush");
                    
                    for(auto brush : brushes)
                    {
                        if(brush->get<cTransform>().pos == pos.midPos)
                        {
                            remove(brush);
                        }
                    }
                    
                }
            }
            break;
            
        case static_cast<int>(ActionTypes::save):
            if(action.status() == Action::end)
            {
                m_state.map.save(m_state.filePath / m_state.fileName);
            }
            break;
            
        case static_cast<int>(ActionTypes::load):
            if(action.status() == Action::end)
            {
                m_state.map.load(m_state.filePath / m_state.fileName);
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
        sf::Sprite grid {m_globalGrid.getTexture()};
        window.draw(grid);
    }
    
    auto tiles = m_state.map.getTiles();
    for(auto& tile : tiles)
    {
        sf::Texture tex { m_assets.getTexture(tile.textureName)};
        sf::Sprite spr {tex};
        spr.setPosition(tile.pos);
        spr.setOrigin({tex.getSize().x / 2.f, tex.getSize().y / 2.f});
        
        window.draw(spr);
    }
    
    for(const auto& entity : m_entities.getEntities())
    {
        if(entity->has<cSprite>())
        {
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
    
    GameMap::MapTile tile {m_globalGrid.getGridAt(pos).midPos, m_state.angle, m_state.selectedTile};
    m_state.map.placeTile(tile);
    // do other stuff 
}

void Scene_Editor::placeSelectedBrush(const sf::Vector2f& pos)
{
    if(!m_game->getWindow().isInsideView(pos))
    {
        return;
    }
    
    auto entitiy = m_entities.addEntity("Brush");
    
    auto& spr = entitiy->add<cSprite>().sprite;
    auto& tex = spr.getTexture();
    spr.setOrigin({tex.getSize().x / 2.f, tex.getSize().y / 2.f});
    
    entitiy->add<cTransform>(m_globalGrid.getGridAt(pos).midPos);
    entitiy->add<cBrush>(m_state.selectedBrush, "a");
}

void Scene_Editor::select(const sf::Vector2f& pos)
{
    
}

void Scene_Editor::remove(const GameMap::MapTile& tile)
{
    m_state.map.remove(tile);
}

void Scene_Editor::remove(std::shared_ptr<Entity> entity)
{
    entity->destroy();
}