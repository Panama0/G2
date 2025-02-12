#include "GameEngine.hpp"

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
    ImGui::SFML::Init(m_window);
    
    // load stuff
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

void GameEngine::quit()
{
    m_running = false;
}

void GameEngine::changeScene(SceneTypes s)
{
    // if the scene already exits, change to it, otherwise, create the scene
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