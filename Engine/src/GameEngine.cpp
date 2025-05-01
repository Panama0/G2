#include "GameEngine.hpp"
#include "Action.hpp"
//#include "Scene_Editor.hpp"
//#include "Scene_MainMenu.hpp"

#include "SFML/Graphics.hpp"
#include "imgui-SFML.h"
#include "imgui.h"

#include <iostream>
#include <optional>

GameEngine::GameEngine() { init(); }

void GameEngine::init()
{
    m_window.init();

    // changeScene<Scene_Editor>();
}

void GameEngine::run()
{
    m_running = true;

    while(m_running)
    {
        m_dt = m_clock.restart();

        sUserInput();
        currentScene()->update();

        if(currentScene()->hasEnded())
        {
            uint32_t endedSceneID{currentScene()->id()};
            currentScene()->end();
            // destroy the scene
            sUserInput();
            m_scenes.erase(endedSceneID);
        }
    }
}

void GameEngine::sUserInput()
{
    while(auto event = m_window.getEvent())
    {
        if(currentScene()->hasGUI())
        {
            ImGui::SFML::ProcessEvent(m_window.getWindow(), event.value());
            if(ImGui::GetIO().WantCaptureMouse)
            {
                continue;
            }
        }

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
        if(event->is<sf::Event::KeyPressed>()
           || event->is<sf::Event::KeyReleased>())
        {
            Action::ActionStatus status;
            event->is<sf::Event::KeyPressed>() ? status = Action::start
                                               : status = Action::end;

            if(const auto& keyP = event->getIf<sf::Event::KeyPressed>())
            {
                processKey(keyP->code, status);
            }
            else if(const auto& keyR = event->getIf<sf::Event::KeyReleased>())
            {
                processKey(keyR->code, status);
            }
        }

        if(event->is<sf::Event::MouseButtonPressed>()
           || event->is<sf::Event::MouseButtonReleased>())
        {
            Action::ActionStatus status;
            event->is<sf::Event::MouseButtonPressed>() ? status = Action::start
                                                       : status = Action::end;

            if(const auto& keyP
               = event->getIf<sf::Event::MouseButtonPressed>())
            {
                processMousePress(keyP->button,
                                  status,
                                  m_window.pixelToCoords(keyP->position));
            }
            else if(const auto& keyR
                    = event->getIf<sf::Event::MouseButtonReleased>())
            {
                processMousePress(keyR->button,
                                  status,
                                  m_window.pixelToCoords(keyR->position));
            }
        }
    }
}

void GameEngine::processKey(sf::Keyboard::Key key, Action::ActionStatus status)
{
    const auto& actionMap = currentScene()->getKeyboardActions();
    if(actionMap.find(key) == actionMap.end())
    {
        // the key is not mapped
        return;
    }
    // create the action and send to the scene for processisng
    Action action{actionMap.at(key), status};
    currentScene()->sDoAction(action);
}

void GameEngine::processMousePress(sf::Mouse::Button button,
                                   Action::ActionStatus status,
                                   const sf::Vector2f& pos)
{
    const auto& actionMap = currentScene()->getMouseActions();
    if(actionMap.find(button) == actionMap.end())
    {
        // the key is not mapped
        return;
    }
    // create the action and send to the scene for processisng
    Action action{actionMap.at(button), status, pos};
    currentScene()->sDoAction(action);
}