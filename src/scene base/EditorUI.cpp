#include "EditorUI.hpp"

#include "scenes/Scene_Editor.hpp"

void EditorUI::draw()
{
    drawMainMenuBarUI();
    if(m_showTilesUI) { drawTilesUI(); }
    
    if(m_showSaveLoad) { drawSaveLoadUI(); }
}

void EditorUI::updateState(EditorState* state)
{
    m_state = state;
}

void EditorUI::drawMainMenuBarUI()
{
    ImGui::BeginMainMenuBar();
    
    if(ImGui::BeginMenu("File"))
    {
        if(ImGui::MenuItem("Save", m_state->fileName.c_str()))
        {
            m_scene->sDoAction(Action {static_cast<int>(Scene_Editor::ActionTypes::save), Action::end});
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

void EditorUI::drawTilesUI()
{
    ImGui::Begin("Tiles");
    if(ImGui::BeginTabBar("Tabs"))
    {
        if(ImGui::BeginTabItem("Tiles"))
        {
            ImGui::SeparatorText("Current Tile");
            
            sf::Sprite currentTile {m_scene->getAssets().getTexture(m_state->selectedTile)};
            currentTile.setScale({5.f, 5.f});
            currentTile.setRotation(m_state->angle);
            currentTile.setOrigin(currentTile.getLocalBounds().getCenter());
            currentTile.setPosition(m_currentTileRenderTex.getView().getCenter());
            
            if(!m_currentTileRenderTex.resize(m_scene->getAssets().getTexture(m_state->selectedTile).getSize() * 5u))
            {
                std::cerr << "Could not Resize!";
            }
            m_currentTileRenderTex.clear();
            m_currentTileRenderTex.draw(currentTile);
            m_currentTileRenderTex.display();
            ImGui::Image(m_currentTileRenderTex);
            
            ImGui::SeparatorText("All Tiles");
            
            size_t tileCount {m_scene->getAssets().getTextureList().size()};
            int columnCount{5};
            
            if(ImGui::BeginTable("Tile Table", columnCount, ImGuiTableFlags_Borders))
            {
                for(size_t i {}; i < tileCount; i++)
                {
                    ImGui::TableNextColumn();
                    std::string name {m_scene->getAssets().getTextureList().at(i).c_str()};
                    ImGui::Text("%s", name.c_str());
                    
                    ImGui::SameLine();
                    ImGui::PushID(i);
                    if(ImGui::Button("Select"))
                    {
                        m_state->selectedTile = name;
                    }
                    ImGui::PopID();
                    
                    ImGui::Image(m_scene->getAssets().getTexture(name));
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

void EditorUI::drawSaveLoadUI()
{
    ImGui::Begin("Save/Load");
    
    if(ImGui::BeginTabBar("Save/Load Tabs"))
    {
        if(ImGui::BeginTabItem("Save As"))
        {
            static char folderBuf[128];
            static char fnameBuf[32];
            static bool copied {false};
            
            const auto& path = m_state->filePath;
            const auto& name = m_state->fileName;
            
            if(!copied)
            {
                std::copy(path.c_str(), path.c_str() + path.string().length() + 1, folderBuf);
                std::copy(name.c_str(), name.c_str() + name.length() + 1, fnameBuf);
                copied = true;
            }
            
            ImGui::InputText("Folder (path relative to game executable)", folderBuf, 128);
            ImGui::InputText("File Name", fnameBuf, 32);
            
            if(ImGui::Button("Save", ImVec2 {150, 50}))
            {
                m_state->filePath = folderBuf;
                m_state->fileName = fnameBuf;
                m_scene->sDoAction(Action {static_cast<int>(Scene_Editor::ActionTypes::save), Action::end});
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
