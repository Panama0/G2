#pragma once

#include "Action.hpp"
#include "Scene.hpp"
#include "Window.hpp"

#include "SFML/Graphics.hpp"
#include <cstdint>
#include <unordered_map>

class Scene;

class GameEngine
{
public:
    GameEngine();
    void run();
    void quit() { m_running = false; }

    void changeScene(uint32_t sceneID) { m_currentSceneID = sceneID; }
    void startScene(std::unique_ptr<Scene> scene);
    uint32_t generateID() { return ++m_idCounter; }

    Window& getWindow() { return m_window; }

    const sf::Time& getDT() { return m_dt; }

    void sUserInput();

private:
    void init();
    void processKey(sf::Keyboard::Key key, Action::ActionStatus status);
    void processMousePress(sf::Mouse::Button button,
                           Action::ActionStatus status,
                           const sf::Vector2f& pos);

    Scene* currentScene() { return m_scenes[m_currentSceneID].get(); }

    std::unordered_map<uint32_t, std::unique_ptr<Scene>> m_scenes;
    uint32_t m_currentSceneID{};

    Window m_window;

    bool m_running{false};
    sf::Clock m_clock;
    sf::Time m_dt;

    uint32_t m_idCounter{0};
};
