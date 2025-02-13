#include "GameEngine.hpp"
#include "Scene_MainMenu.hpp"

#include "SFML/Graphics.hpp"
#include "imgui.h"
#include "imgui-SFML.h"

#include <iostream>

GameEngine::GameEngine()
{
    init();
}

void GameEngine::init()
{
    m_window.create(sf::VideoMode{{100,100}}, "G2");
    m_window.setFramerateLimit(60u);
    ImGui::SFML::Init(m_window);
    
    
    //* temp
    m_scenes[SceneTypes::mainmenu] = std::make_shared<Scene_MainMenu>();
    changeScene(SceneTypes::mainmenu);
}

void GameEngine::run()
{
    m_running = true;
    
    while(m_running)
    {
        sUserInput();
        
        // temp
        m_window.display();
    }
}

void GameEngine::update()
{
    
}

void GameEngine::changeScene(SceneTypes s)
{
    if(m_scenes[s] == nullptr)
    {
        std::cerr << "Scene does not exist!\n";
    }
    else
    {
        m_currentScene = s;
    }
}

void GameEngine::sUserInput()
{
    while (auto event = m_window.pollEvent())
    {
        ImGui::SFML::ProcessEvent(m_window, event.value());
        if(ImGui::GetIO().WantCaptureMouse) { continue; }
        
        if(event->is<sf::Event::Closed>())
        {
            quit();
        }
    }
}