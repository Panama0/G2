#include "GameEngine.hpp"
#include "Scene_MainMenu.hpp"
#include "Action.hpp"

#include "SFML/Graphics.hpp"
#include "imgui.h"
#include "imgui-SFML.h"

#include <iostream>
#include <optional>

GameEngine::GameEngine()
{
    init();
}

void GameEngine::init()
{
    m_window.init();
    //* temp
    m_grid.init({128,128}, {32, 32});
    
    m_scenes[0] = std::make_shared<Scene_MainMenu>(this);
    changeScene(0);
}

void GameEngine::run()
{
    m_running = true;
    
    while(m_running)
    {      
        sUserInput();
        currentScene()->update();
    }
}

void GameEngine::changeScene(int s)
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
    while (auto event = m_window.getEvent())
    {
        ImGui::SFML::ProcessEvent(m_window.getWindow(), event.value());
        if(ImGui::GetIO().WantCaptureMouse) { continue; }
        
        if(event->is<sf::Event::Closed>())
        {
            quit();
        }
        
        // resize view when window changes size
        if(const auto& resized = event->getIf<sf::Event::Resized>())
        {
            m_window.updateView(static_cast<sf::Vector2f>(resized->size));
        }
        
        // main input handling
        
        if(event->is<sf::Event::KeyPressed>() || event->is<sf::Event::KeyReleased>())
        {
            
            Action::ActionStatus status {Action::null};
            event->is<sf::Event::KeyPressed>()? status = Action::start : status = Action::end;
            
            if(const auto& keyP = event->getIf<sf::Event::KeyPressed>())
            {
                processKey(keyP->code, status);
            }
            else if (const auto& keyR = event->getIf<sf::Event::KeyReleased>())
            {
                processKey(keyR->code, status);
            }
        }
    }
}

void GameEngine::processKey(sf::Keyboard::Key key, Action::ActionStatus status)
{
    const auto& actionMap = currentScene()->getActionMap();
    if(actionMap.find(key) == actionMap.end())
    {
        // the key is not mapped
        return;
    }
    // create the action and send to the scene for processisng
    Action action {actionMap.at(key), status};
    currentScene()->sDoAction(action);
}