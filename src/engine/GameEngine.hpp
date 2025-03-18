#pragma once

#include "scene base/Scene.hpp"
#include "engine/Action.hpp"
#include "engine/Window.hpp"

#include "SFML/Graphics.hpp"
#include <map>

class Scene;

class GameEngine
{
public:
    GameEngine();
    void run();
    void quit() { m_running = false; }
    
    template <typename T>
    void changeScene(uint32_t sceneID)
    {
        if(m_scenes2.find(sceneID) == nullptr)
        {
            m_scenes2.emplace(sceneID, std::make_unique<T>(this, sceneID));
        }
        m_currentSceneID = sceneID;
    }
    
    Window& getWindow() { return m_window; }
    
    sf::Time getDT() { return m_dt; }
    
    void sUserInput();
private:
    void init();
    void processKey(sf::Keyboard::Key key, Action::ActionStatus status);
    void processMousePress(sf::Mouse::Button button, Action::ActionStatus status, const sf::Vector2f& pos);
    
    Scene* currentScene() { return m_scenes2.find(m_currentSceneID)->second.get(); }
    std::unordered_map<uint32_t, std::unique_ptr<Scene>> m_scenes2;
    
    Window m_window;
    
    uint32_t m_currentSceneID {};
    bool m_running {false};
    sf::Clock m_clock;
    sf::Time m_dt;
    
    uint32_t m_idCounter {0};
};