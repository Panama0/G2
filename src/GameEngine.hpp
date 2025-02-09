#pragma once

#include "Assets.hpp"
#include "Scene.hpp"
#include "SFML/Graphics.hpp"
#include <map>

enum class SceneTypes
{
    mainmenu,
    gameplay,
};

class Scene;

class GameEngine
{
public:
    void run();
    void update();
    void quit();
    void changeScene(SceneTypes s);
    
    const Assets& getAssets();
    const sf::RenderWindow& getWindow();
    void sUserInput();
private:
    void init();
    
    std::shared_ptr<Scene> currentScene();
    
    std::map<SceneTypes, std::shared_ptr<Scene>> m_scenes;
    sf::RenderWindow m_window;
    Assets m_assets;
    SceneTypes m_currentScene;
    bool m_running {false};
};