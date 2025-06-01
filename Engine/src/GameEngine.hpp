#pragma once

#include "Action.hpp"
#include "SceneManager.hpp"
#include "Window.hpp"

#include "SFML/Graphics.hpp"
#include <memory>

class Scene;

class GameEngine
{
public:
    GameEngine();
    void run();
    void quit() { m_running = false; }

    // Create and open a new scene
    void startScene(std::shared_ptr<Scene> scene);

    Window& getWindow() { return m_window; }

    const sf::Time& getDT() const { return m_dt; }

    void sUserInput();

private:
    void init();
    void processKey(sf::Keyboard::Key key, Action::ActionStatus status);
    void processMousePress(sf::Mouse::Button button,
                           Action::ActionStatus status,
                           const sf::Vector2i& pos);

    Scene* currentScene();
    SceneManager m_scenes;

    Window m_window;

    bool m_running{false};
    sf::Clock m_clock;
    sf::Time m_dt;
};
