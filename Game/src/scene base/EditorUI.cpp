#include "EditorUI.hpp"

#include "imgui.h"
#include "scene base/TileEffect.hpp"
#include "scenes/Scene_Editor.hpp"
#include <filesystem>

void EditorUI::draw()
{
    drawMainMenuBarUI();
    if(m_showTilesUI)
    {
        drawTilesUI();
    }
    if(m_showToolsUI)
    {
        drawToolsUI();
    }
    if(m_showSaveLoad)
    {
        drawSaveLoadUI();
    }

    if(m_state->selectedTile)
    {
        drawSelectedInfo();
    }
}

void EditorUI::updateState(EditorState* state) { m_state = state; }

void EditorUI::toggleSelect()
{
    m_selected ? m_selected = false : m_selected = true;
}

void EditorUI::drawMainMenuBarUI()
{
    ImGui::BeginMainMenuBar();

    if(ImGui::BeginMenu("File"))
    {
        if(ImGui::MenuItem("Save", m_state->saveName.c_str()))
        {
            m_scene->sDoAction(
                Action{static_cast<int>(Scene_Editor::ActionTypes::save),
                       Action::end});
        }
        if(ImGui::MenuItem("Save As/Load"))
        {
            m_showSaveLoad = true;
        }
        if(ImGui::MenuItem("Exit to Main Menu"))
        {
            m_scene->sDoAction(
                Action{static_cast<int>(Scene_Editor::ActionTypes::endScene),
                       Action::end});
        }
        if(ImGui::MenuItem("Exit"))
        {
            m_scene->sDoAction(
                Action{static_cast<int>(Scene_Editor::ActionTypes::exit),
                       Action::end});
        }

        ImGui::EndMenu();
    }

    if(ImGui::BeginMenu("Windows"))
    {
        if(ImGui::MenuItem("Tiles", NULL, m_showTilesUI))
        {
            if(m_showTilesUI)
            {
                m_showTilesUI = false;
            }
            else
            {
                m_showTilesUI = true;
            }
        }

        if(ImGui::MenuItem("Tools", NULL, m_showToolsUI))
        {
            if(m_showToolsUI)
            {
                m_showToolsUI = false;
            }
            else
            {
                m_showToolsUI = true;
            }
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

            sf::Sprite currentTile{
                m_scene->getAssets().getTexture(m_state->tileTexture)};
            currentTile.setScale({5.f, 5.f});
            currentTile.setRotation(m_state->angle);
            currentTile.setOrigin(currentTile.getLocalBounds().getCenter());
            currentTile.setPosition(
                m_currentTileRenderTex.getView().getCenter());

            if(!m_currentTileRenderTex.resize(
                   m_scene->getAssets()
                       .getTexture(m_state->tileTexture)
                       .getSize()
                   * 5u))
            {
                std::cerr << "Could not Resize!";
            }
            m_currentTileRenderTex.clear();
            m_currentTileRenderTex.draw(currentTile);
            m_currentTileRenderTex.display();
            ImGui::Image(m_currentTileRenderTex);

            ImGui::SeparatorText("All Tiles");

            size_t tileCount{m_scene->getAssets().getTextureList().size()};
            const int columnCount{5};

            if(ImGui::BeginTable(
                   "Tile Table", columnCount, ImGuiTableFlags_Borders))
            {
                for(size_t i{}; i < tileCount; i++)
                {
                    ImGui::TableNextColumn();
                    std::string name{
                        m_scene->getAssets().getTextureList().at(i).c_str()};
                    ImGui::Text("%s", name.c_str());

                    ImGui::SameLine();
                    ImGui::PushID(i);
                    if(ImGui::Button("Select"))
                    {
                        m_state->tileTexture = name;
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
            if(ImGui::BeginListBox("Brushes"))
            {
                for(int i{}; i < TileEffect::count; i++)
                {
                    TileEffect::Effects effect
                        = static_cast<TileEffect::Effects>(i);
                    auto name = TileEffect::toString(effect);

                    if(ImGui::Selectable(name.data(),
                                         effect == m_state->brushType))
                    {
                        m_state->brushType = effect;
                    }
                }
                ImGui::EndListBox();
            }

            ImGui::EndTabItem();
        }

        ImGui::EndTabBar();
    }

    ImGui::End();
}

void EditorUI::drawToolsUI()
{
    if(ImGui::Begin("Tools"))
    {
        ImGui::Text("Current Tool: %s", getToolName(m_state->currentMode));

        if(ImGui::Button(getToolName(EditorState::Mode::none)))
        {
            m_state->currentMode = EditorState::Mode::none;
        }
        ImGui::SameLine();
        if(ImGui::Button(getToolName(EditorState::Mode::tilePlaceTileRemove)))
        {
            m_state->currentMode = EditorState::Mode::tilePlaceTileRemove;
        }
        ImGui::SameLine();
        if(ImGui::Button(
               getToolName(EditorState::Mode::brushPlaceBrushRemove)))
        {
            m_state->currentMode = EditorState::Mode::brushPlaceBrushRemove;
        }
        ImGui::SameLine();
        if(ImGui::Button(getToolName(EditorState::Mode::selectNone)))
        {
            m_state->currentMode = EditorState::Mode::selectNone;
        }

        ImGui::End();
    }
}

const char* EditorUI::getToolName(EditorState::Mode mode)
{
    switch(mode)
    {
    case EditorState::Mode::none:
        return "View";
    case EditorState::Mode::tilePlaceTileRemove:
        return "Place and Remove Tiles";
    case EditorState::Mode::brushPlaceBrushRemove:
        return "Place and Remove Brushes";
    case EditorState::Mode::selectNone:
        return "Select";
    }
    return "Invalid\n";
}

void EditorUI::drawSaveLoadUI()
{
    ImGuiWindowFlags flags = ImGuiWindowFlags_AlwaysAutoResize
                             | ImGuiWindowFlags_NoResize
                             | ImGuiWindowFlags_NoCollapse;

    ImGui::Begin("Save/Load", &m_showSaveLoad, flags);

    if(ImGui::BeginTabBar("Save/Load Tabs"))
    {
        if(ImGui::BeginTabItem("Save As"))
        {
            static char folderBuf[128];
            static char fnameBuf[32];
            static bool copied{false};

            const auto& path = m_state->savePath;
            const auto& name = m_state->saveName.append(".sff");

            if(!copied)
            {
                std::copy(path.c_str(),
                          path.c_str() + path.string().length() + 1,
                          folderBuf);
                std::copy(
                    name.c_str(), name.c_str() + name.length() + 1, fnameBuf);
                copied = true;
            }

            ImGui::InputText(
                "Folder (path relative to game executable)", folderBuf, 128);
            ImGui::InputText("File Name", fnameBuf, 32);

            if(ImGui::Button("Save", ImVec2{150, 50}))
            {
                m_state->savePath = folderBuf;
                m_state->saveName = std::string(fnameBuf);
                m_scene->sDoAction(
                    Action{static_cast<int>(Scene_Editor::ActionTypes::save),
                           Action::end});
                copied = false;
                m_showSaveLoad = false;
            }

            ImGui::EndTabItem();
        }

        if(ImGui::BeginTabItem("Load"))
        {
            static char buf[128];
            static bool copied{false};

            const auto& startingDir = m_state->savePath;

            if(!copied)
            {
                std::copy(startingDir.c_str(),
                          startingDir.c_str() + startingDir.string().length()
                              + 1,
                          buf);
                copied = true;
            }

            if(ImGui::InputText("Path to Saves Folder", buf, 128))
            {
                std::filesystem::path path{buf};
                if(std::filesystem::exists(path))
                {
                    m_state->savePath = buf;
                }
                else
                {
                    std::cerr << "Path does not exist!\n";
                }
            }

            // construct a list of the files in the directory
            std::vector<std::string> files;

            if(std::filesystem::exists(startingDir))
            {
                for(const auto& file :
                    std::filesystem::directory_iterator{startingDir})
                {
                    files.emplace_back(file.path().filename());
                }
            }
            else
            {
                std::cerr << "Path does not exist!\n";
            }

            static std::string selectedItem;

            if(ImGui::BeginListBox("Files"))
            {
                for(auto& file : files)
                {
                    if(ImGui::Selectable(file.c_str(), file == selectedItem))
                    {
                        selectedItem = file;
                    }
                }
                ImGui::EndListBox();
            }

            if(ImGui::Button("Load", ImVec2{150, 50}))
            {
                m_state->saveName = selectedItem;
                m_scene->sDoAction(
                    Action{static_cast<int>(Scene_Editor::ActionTypes::load),
                           Action::end});
                copied = false;
                m_showSaveLoad = false;
            }

            ImGui::EndTabItem();
        }

        ImGui::EndTabBar();
    }

    ImGui::End();
}

void EditorUI::drawSelectedInfo()
{
    ImGuiWindowFlags flags = ImGuiWindowFlags_AlwaysAutoResize
                             | ImGuiWindowFlags_NoResize
                             | ImGuiWindowFlags_NoCollapse;

    if(ImGui::Begin("Selection", nullptr, flags))
    {
        auto& tile = m_state->selectedTile;
        ImGui::SetWindowPos(m_state->selectedTilePos);
        ImGui::Text("Texture Name: %s", tile->textureName.c_str());
        ImGui::Text("Position: %u, %u", tile->pos.x, tile->pos.y);
        ImGui::Text("Rotation: %.2f", tile->rotation.asDegrees());

        if(!tile->staticEffects.empty())
        {
            ImGui::SeparatorText("Effects");

            int count{1};
            for(auto& effect : tile->staticEffects)
            {
                ImGui::Text("Effect %d: %s",
                            count,
                            TileEffect::toString(effect).data());
                count++;
            }
        }

        ImGui::End();
    }
}
