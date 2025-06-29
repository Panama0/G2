#pragma once

#include "Scene.hpp"
#include "Vec2.hpp"

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
    Scene_MainMenu(GameEngine* game) : Scene(game) { init(); }

    void update() override;
    void sDoAction(const Action& action) override;
    void sRender() override;
    void sAnimation();
    void onExit() override { m_game->quit(); }

    void init() override;

private:
    void spawnMainMenu();
    void spawnButton(const std::string& name,
                     std::string_view tex,
                     Vec2f worldPos);

    sf::View m_view;
};
