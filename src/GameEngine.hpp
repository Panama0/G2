#pragma once

#include "Scene.hpp"
#include "Action.hpp"
#include "Window.hpp"
#include "Grid.hpp"

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
    const Grid& getGrid() { return m_grid; }
    
    void sUserInput();
private:
    void init();
    void processKey(sf::Keyboard::Key key, Action::ActionStatus status);
    
    std::shared_ptr<Scene> currentScene() { return m_scenes[m_currentScene]; }
    std::map<int, std::shared_ptr<Scene>> m_scenes;
    
    Window m_window;
    Grid m_grid;
    
    int m_currentScene;
    bool m_running {false};
};