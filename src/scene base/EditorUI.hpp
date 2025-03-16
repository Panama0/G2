#pragma once

#include "engine/Gui.hpp"
#include "scene base/Assets.hpp"

class Scene;

class EditorUI : public Gui
{
public:
    EditorUI() = default;
    EditorUI(Scene* scene)
        :m_scene {scene}
    {}
    
    void draw() override;
private:
    void drawTilesUI();
    void drawMainMenuBarUI();
    void drawSaveLoadUI();

    sf::RenderTexture m_currentTileRenderTex {{200,200}};
    bool m_showSaveLoad {false};
    bool m_showTilesUI {true};
    bool m_showToolsUI {false};
    std::string m_fileName {"Unnamed Level"};
    std::filesystem::path m_dir {"../../res/saves/"};
    
    Scene* m_scene;
};
