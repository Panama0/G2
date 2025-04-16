#pragma once

#include "scene base/Scene.hpp"

class Scene_MainMenu : public Scene
{
public:
    enum ActionTypes
    {
        null,
        FS,
        launchEditor,
    };

    Scene_MainMenu() = default;
    Scene_MainMenu(GameEngine* game, uint32_t id) : Scene(game, id) { init(); }

    void update() override;
    void sDoAction(const Action& action) override;
    void sRender() override;
    void sAnimation();
    void end() override { m_game->quit(); }

    void init() override;

private:
    void spawnMainMenu();
    void spawnButton(const std::string& name,
                     std::string_view tex,
                     sf::Vector2f worldPos);

    sf::View m_view;
};