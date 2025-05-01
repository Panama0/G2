#pragma once

#include "Action.hpp"
#include "Window.hpp"
#include "Scene.hpp"

#include "SFML/Graphics.hpp"
#include <iostream>
#include <unordered_map>

#include "imgui-SFML.h"
#include "imgui.h"

class Scene;

class GameEngine
{
public:
    GameEngine();
    void run();
    void quit() { m_running = false; }

    template <typename T> void changeScene(uint32_t sceneID)
    {
        if(m_scenes.find(sceneID) == nullptr)
        {
            std::cerr << "Could not find the specified scene!\n";
        }
        else
        {
            m_currentSceneID = sceneID;
        }
    }
    template <typename T> void changeScene()
    {
        if(m_scenes.find(m_idCounter) == nullptr)
        {
            m_window.init();
            m_scenes.emplace(m_idCounter,
                             std::make_unique<T>(this, m_idCounter));
        }
        m_currentSceneID = m_idCounter;
        m_idCounter++;
    }

    Window& getWindow() { return m_window; }

    sf::Time getDT() { return m_dt; }

    void sUserInput();

private:
    void init();
    void processKey(sf::Keyboard::Key key, Action::ActionStatus status);
    void processMousePress(sf::Mouse::Button button,
                           Action::ActionStatus status,
                           const sf::Vector2f& pos);

    Scene* currentScene()
    {
        return m_scenes.find(m_currentSceneID)->second.get();
    }
    std::unordered_map<uint32_t, std::unique_ptr<Scene> > m_scenes;

    Window m_window;

    uint32_t m_currentSceneID{};
    bool m_running{false};
    sf::Clock m_clock;
    sf::Time m_dt;

    uint32_t m_idCounter{0};
};