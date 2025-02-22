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
    m_windowSize = {1280, 720};
    m_window.create(sf::VideoMode{m_windowSize}, "G2");
    m_view = m_window.getView();
    
    updateView(m_windowSize);
    m_window.setFramerateLimit(60u);

    if(!ImGui::SFML::Init(m_window)) { std::cerr << "Could not init ImGui!\n"; }
    
    //* temp
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

void GameEngine::update()
{
    
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
    while (auto event = m_window.pollEvent())
    {
        ImGui::SFML::ProcessEvent(m_window, event.value());
        if(ImGui::GetIO().WantCaptureMouse) { continue; }
        
        if(event->is<sf::Event::Closed>())
        {
            quit();
        }
        
        // resize view when window changes size
        if(const auto& resized = event->getIf<sf::Event::Resized>())
        {
            updateView(resized->size);
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

void GameEngine::updateView(const sf::Vector2u& size)
{
    float desiredRatio {static_cast<float>(size.x) / static_cast<float>(size.y)};
    float currentRatio {m_view.getSize().x / m_view.getSize().y};
    
    float sizeX {1.f};
    float sizeY {1.f};
    float posX {0.f};
    float posY {0.f};
    
    bool horizontalSpacing {true};
    if(desiredRatio < currentRatio)
    {
        horizontalSpacing = false;
    }
    
    if(horizontalSpacing)
    {
        sizeX = currentRatio / desiredRatio;
        posX = (1 - sizeX) / 2.f;
    }
    else
    {
        sizeY = desiredRatio / currentRatio;
        posY = (1 - sizeY) / 2.f;
    }

    m_view.setViewport(sf::FloatRect({posX, posY}, {sizeX, sizeY}));
    m_window.setView(m_view);
}