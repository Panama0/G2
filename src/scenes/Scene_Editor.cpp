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
}

void Scene_Editor::update()
{
    m_game->getWindow().updateImGui(m_game->getDT());
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
            
        case static_cast<int>(ActionTypes::save):
            if(action.status() == Action::end)
            {
                m_gameMap.save(m_dir / m_fileName);
            }
            break;
            
        case static_cast<int>(ActionTypes::load):
            if(action.status() == Action::end)
            {
                m_gameMap.load(m_dir / m_fileName);
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
    drawMainMenuBarUI();
    if(m_showTilesUI) { drawTilesUI(); }
    
    if(m_showSaveLoad) { drawSaveLoadUI(); }
    
    m_game->getWindow().drawImGui();
}

void Scene_Editor::drawMainMenuBarUI()
{
    ImGui::BeginMainMenuBar();
    
    if(ImGui::BeginMenu("File"))
    {
        if(ImGui::MenuItem("Save", m_fileName.c_str()))
        {
            m_gameMap.save(m_dir / m_fileName);
        }
        if(ImGui::MenuItem("Save As/Load"))
        {
            m_showSaveLoad = true;
        }
        if(ImGui::MenuItem("Exit to Main Menu"))
        {
            
        }
        if(ImGui::MenuItem("Exit"))
        {
            
        }
        
        ImGui::EndMenu();
    }
    
    if(ImGui::BeginMenu("Windows"))
    {
        if(ImGui::MenuItem("Tiles", NULL, m_showTilesUI))
        {
            if(m_showTilesUI) { m_showTilesUI = false; }
            else { m_showTilesUI = true; }
        }
        
        if(ImGui::MenuItem("Tools", NULL, m_showToolsUI))
        {
            if(m_showToolsUI) { m_showToolsUI = false; }
            else { m_showToolsUI = true; }
        }
        
        ImGui::EndMenu();
    }
    
    ImGui::EndMainMenuBar();
}

void Scene_Editor::drawTilesUI()
{
    ImGui::Begin("Tiles");
    if(ImGui::BeginTabBar("Tabs"))
    {
        if(ImGui::BeginTabItem("Tiles"))
        {
            ImGui::SeparatorText("Current Tile");
            
            sf::Sprite currentTile {m_assets.getTexture(m_selectedTile)};
            currentTile.setScale({5.f, 5.f});
            currentTile.setRotation(m_rotation);
            currentTile.setOrigin(currentTile.getLocalBounds().getCenter());
            currentTile.setPosition(m_currentTileRenderTex.getView().getCenter());
            
            if(!m_currentTileRenderTex.resize(m_assets.getTexture(m_selectedTile).getSize() * 5u))
            {
                std::cerr << "Could not Resize!";
            }
            m_currentTileRenderTex.clear();
            m_currentTileRenderTex.draw(currentTile);
            m_currentTileRenderTex.display();
            ImGui::Image(m_currentTileRenderTex);
            
            ImGui::SeparatorText("All Tiles");
            
            size_t tileCount {m_assets.getTextureList().size()};
            int columnCount{5};
            
            if(ImGui::BeginTable("Tile Table", columnCount, ImGuiTableFlags_Borders))
            {
                for(size_t i {}; i < tileCount; i++)
                {
                    ImGui::TableNextColumn();
                    std::string name {m_assets.getTextureList().at(i).c_str()};
                    ImGui::Text("%s", name.c_str());
                    
                    ImGui::SameLine();
                    ImGui::PushID(i);
                    if(ImGui::Button("Select"))
                    {
                        m_selectedTile = name;
                    }
                    ImGui::PopID();
                    
                    ImGui::Image(m_assets.getTexture(name));
                }
                
                ImGui::EndTable();
            }
            
            ImGui::EndTabItem();
        }
        
        if(ImGui::BeginTabItem("Brushes"))
        {
            
            ImGui::EndTabItem();
        }
        
        ImGui::EndTabBar();
    }
    
    ImGui::End();
}

void Scene_Editor::drawSaveLoadUI()
{
    ImGui::Begin("Save/Load");
    
    if(ImGui::BeginTabBar("Save/Load Tabs"))
    {
        if(ImGui::BeginTabItem("Save As"))
        {
            static char folderBuf[128];
            static char fnameBuf[32];
            static bool copied {false};
            
            if(!copied)
            {
                std::copy(m_dir.c_str(), m_dir.c_str() + m_dir.string().length() + 1, folderBuf);
                std::copy(m_fileName.c_str(), m_fileName.c_str() + m_fileName.length() + 1, fnameBuf);
                copied = true;
            }
            
            ImGui::InputText("Folder (path relative to game executable)", folderBuf, 128);
            ImGui::InputText("File Name", fnameBuf, 32);
            
            if(ImGui::Button("Save", ImVec2 {150, 50}))
            {
                m_dir = folderBuf;
                m_fileName = fnameBuf;
                sDoAction(Action {static_cast<int>(ActionTypes::save), Action::end});
                copied = false;
                m_showSaveLoad = false;
            }
            
            ImGui::EndTabItem();
        }
        
        if(ImGui::BeginTabItem("Load"))
        {
            
            ImGui::EndTabItem();
        }
        
        ImGui::EndTabBar();
    }
    
    ImGui::End();
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