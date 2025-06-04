#pragma once

#include "Action.hpp"
#include "Buttons.hpp"
#include "SFML/System/Vector2.hpp"
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

    void handleInput();

private:
    void init();
    void processIOEvent(Buttons::Button button, sf::Vector2i mousePos, Action::Status status);

    Scene* currentScene();
    SceneManager m_scenes;

    Window m_window;

    bool m_running{false};
    sf::Clock m_clock;
    sf::Time m_dt;
};
