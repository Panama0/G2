#include "Scene_Editor.hpp"
#include "imgui-SFML.h"
#include "imgui.h"

void Scene_Editor::init()
{
    //* temp
    m_gridSize = {32, 32};
    
    m_assets.setResourceDir("../../res/");
    m_globalGrid.init(m_game->getWindow().getSize(), m_gridSize);
    
    registerAction(sf::Keyboard::Key::F, static_cast<int>(ActionTypes::toggleFS));
    registerAction(sf::Keyboard::Key::G, static_cast<int>(ActionTypes::toggleGrid));
    registerAction(sf::Mouse::Button::Left, static_cast<int>(ActionTypes::place));
    
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
                std::cout << action.position().x << ',' << action.position().y << '\n';
            }
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

    drawUI();
    window.render();
}

void Scene_Editor::drawUI()
{
    ImGui::Begin("Editor");
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
            m_currentTileRenderTex.clear(sf::Color::Red);
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
    m_game->getWindow().drawImGui();
}