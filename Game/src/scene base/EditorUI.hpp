#pragma once

#include "Gui.hpp"
#include "EditorState.hpp"

class Scene_Editor;

class EditorUI : public Gui
{
public:
    EditorUI() = delete;
    EditorUI(Scene_Editor* scene) : m_scene{scene} {}

    void draw() override;
    void updateState(EditorState* state);
    void toggleSelect();

private:
    void drawTilesUI();
    void drawMainMenuBarUI();
    void drawSaveLoadUI();
    void drawToolsUI();
    void drawSelectedInfo();
    const char* getToolName(EditorState::Modes mode);

    sf::RenderTexture m_currentTileRenderTex{{200, 200}};
    bool m_showSaveLoad{false};
    bool m_showTilesUI{true};
    bool m_showToolsUI{true};

    bool m_selected {false};

    Scene_Editor* m_scene;
    EditorState* m_state;
};
