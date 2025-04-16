#pragma once

#include "engine/Gui.hpp"
#include "scene base/EditorState.hpp"

class Scene_Editor;

class EditorUI : public Gui
{
public:
    EditorUI() = delete;
    EditorUI(Scene_Editor* scene) : m_scene{scene} {}

    void draw() override;
    void updateState(EditorState* state);

private:
    void drawTilesUI();
    void drawMainMenuBarUI();
    void drawSaveLoadUI();
    void drawToolsUI();
    const char* getToolName(EditorState::Modes mode);

    sf::RenderTexture m_currentTileRenderTex{{200, 200}};
    bool m_showSaveLoad{false};
    bool m_showTilesUI{true};
    bool m_showToolsUI{true};

    Scene_Editor* m_scene;
    EditorState* m_state;
};
