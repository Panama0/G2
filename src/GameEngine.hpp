#pragma once

#include "Scene.hpp"
#include "Action.hpp"

#include "SFML/Graphics.hpp"
#include <map>

class Scene;

class GameEngine
{
public:
    GameEngine();
    void run();
    void update();
    void quit() { m_running = false; }
    void changeScene(int s);
    
    sf::RenderWindow& getWindow() { return m_window; }
    void sUserInput();
private:
    void init();
    void processKey(sf::Keyboard::Key key, Action::ActionStatus status);
    
    std::shared_ptr<Scene> currentScene() { return m_scenes[m_currentScene]; }
    
    std::map<int, std::shared_ptr<Scene>> m_scenes;
    sf::RenderWindow m_window;
    int m_currentScene;
    bool m_running {false};
};