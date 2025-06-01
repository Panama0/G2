#include "GameEngine.hpp"
#include "Action.hpp"
#include "Scene.hpp"

#include "SFML/Graphics.hpp"
#include "imgui-SFML.h"
#include "imgui.h"

#include <iostream>
#include <memory>
#include <optional>

GameEngine::GameEngine() { init(); }

void GameEngine::init() { m_window.init(); }

void GameEngine::run()
{
    m_running = true;

    // load the new scene if one was created before running
    m_scenes.update();

    while(m_running)
    {
        if(m_scenes.empty())
        {
            quit();
        }

        m_dt = m_clock.restart();

        currentScene()->update();

        sUserInput();
        m_scenes.update();
    }
}

void GameEngine::startScene(std::shared_ptr<Scene> scene)
{
    m_scenes.start(scene);
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
                processMousePress(keyP->button, status, keyP->position);
            }
            else if(const auto& keyR
                    = event->getIf<sf::Event::MouseButtonReleased>())
            {
                processMousePress(keyR->button, status, keyR->position);
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
                                   const sf::Vector2i& pos)
{
    const auto& actionMap = currentScene()->getMouseActions();
    if(actionMap.find(button) == actionMap.end())
    {
        // the key is not mapped
        return;
    }
    // create the action and send to the scene for processisng
    Action action{actionMap.at(button), status, m_window.pixelToCoords(pos)};
    currentScene()->sDoAction(action);
}

Scene* GameEngine::currentScene()
{
    return m_scenes.getCurrentScene();
}
