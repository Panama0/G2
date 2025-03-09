#pragma once

#include "Scene.hpp"
#include "Action.hpp"
#include "Window.hpp"

#include "SFML/Graphics.hpp"
#include <map>

class Scene;

class GameEngine
{
public:
    GameEngine();
    void run();
    void quit() { m_running = false; }
    void changeScene(int s);
    
    Window& getWindow() { return m_window; }
    
    sf::Time getDT() { return m_dt; }
    
    void sUserInput();
private:
    void init();
    void processKey(sf::Keyboard::Key key, Action::ActionStatus status);
    void processMousePress(sf::Mouse::Button button, Action::ActionStatus status, const sf::Vector2f& pos);
    
    std::shared_ptr<Scene> currentScene() { return m_scenes[m_currentScene]; }
    std::map<int, std::shared_ptr<Scene>> m_scenes;
    
    Window m_window;
    
    int m_currentScene;
    bool m_running {false};
    sf::Clock m_clock;
    sf::Time m_dt;
};