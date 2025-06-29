#pragma once

#include "Action.hpp"
#include "Buttons.hpp"
#include "SceneManager.hpp"
#include "Window.hpp"

#include "SFML/Graphics.hpp"
#include <cstdint>
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
    uint32_t getMaxFps() const { return m_maxFps; }

    void handleInput();

private:
    void init();
    void processIOEvent(Buttons::Button button,
                        Vec2i mousePos,
                        Action::Status status);

    Scene* currentScene();
    SceneManager m_scenes;

    Window m_window;

    bool m_running{false};
    sf::Clock m_clock;
    sf::Time m_dt;
    uint32_t m_maxFps{120};
};
