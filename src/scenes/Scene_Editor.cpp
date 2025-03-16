#include "scenes/Scene_Editor.hpp"
#include "imgui-SFML.h"
#include "imgui.h"

#include <algorithm>

void Scene_Editor::init()
{
    //* temp
    m_gridSize = {32, 32};
    
    m_assets.setResourceDir("../../res/");
    m_globalGrid.init(m_game->getWindow().getSize(), m_gridSize);
    
    registerAction(sf::Keyboard::Key::F, static_cast<int>(ActionTypes::toggleFS));
    registerAction(sf::Keyboard::Key::G, static_cast<int>(ActionTypes::toggleGrid));
    registerAction(sf::Mouse::Button::Left, static_cast<int>(ActionTypes::place));
    registerAction(sf::Mouse::Button::Right, static_cast<int>(ActionTypes::remove));
    registerAction(sf::Keyboard::Key::S, static_cast<int>(ActionTypes::save));
    registerAction(sf::Keyboard::Key::L, static_cast<int>(ActionTypes::load));
    
    registerTexture("t", "start.png");
    m_assets.loadTextureDir("../../res/tiles/");
    
    m_selectedTile = m_assets.getTextureList().at(0);
    
    m_editorUI.init(m_game->getWindow());
}

void Scene_Editor::update()
{
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
                if(m_gridVisible)
                {
                    m_gridVisible = false;
                }
                else
                {
                    m_gridVisible = true;
                }
            }
            break;
        
        case static_cast<int>(ActionTypes::place):
            if(action.status() == Action::start)
            {
                placeSelectedTile(action.position());
            }
            break;
        
        case static_cast<int>(ActionTypes::remove):
            if(action.status() == Action::end)
            {
                const auto& gridLocation = m_globalGrid.getGridAt(action.position());
                const auto& tiles = m_gameMap.getTilesAt(gridLocation.midPos);
                if(!tiles.empty())
                {
                    for(const auto& tile : tiles)
                    {
                        m_gameMap.remove(tile);
                    }
                }
            }
            break;
            
        case static_cast<int>(ActionTypes::save):
            if(action.status() == Action::end)
            {
                m_gameMap.save("u");
            }
            break;
            
        case static_cast<int>(ActionTypes::load):
            if(action.status() == Action::end)
            {
                m_gameMap.load("u");
            }
            break;
    }
}

void Scene_Editor::sRender()
{
    auto& window = m_game->getWindow();
    window.beginDraw();
    
    if(m_gridVisible)
    {
        sf::Sprite grid {m_globalGrid.getTexture()};
        window.draw(grid);
    }
    
    auto tiles = m_gameMap.getTiles();
    for(auto& tile : tiles)
    {
        sf::Texture tex { m_assets.getTexture(tile.textureName)};
        sf::Sprite spr {tex};
        spr.setPosition(tile.pos);
        spr.setOrigin({tex.getSize().x / 2.f, tex.getSize().y / 2.f});
        
        window.draw(spr);
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
    
    GameMap::MapTile tile {m_globalGrid.getGridAt(pos).midPos, m_rotation, m_selectedTile, 0};
    m_gameMap.place(tile);
    // do other stuff 
}