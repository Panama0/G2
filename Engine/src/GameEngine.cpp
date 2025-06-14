#include "GameEngine.hpp"
#include "Action.hpp"
#include "Buttons.hpp"
#include "SFML/System/Vector2.hpp"
#include "SFML/Window/Event.hpp"
#include "Scene.hpp"

#include "SFML/Graphics.hpp"
#include "imgui-SFML.h"
#include "imgui.h"

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

        handleInput();
        m_scenes.update();
    }
}

void GameEngine::startScene(std::shared_ptr<Scene> scene)
{
    m_scenes.start(scene);
}

void GameEngine::handleInput()
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
        if(const auto& keyPressed = event->getIf<sf::Event::KeyPressed>())
        {
            auto button = Buttons::toButton(keyPressed->code);
            processIOEvent(button, m_window.getMousePos(), Action::start);
        }
        if(const auto& keyReleased = event->getIf<sf::Event::KeyReleased>())
        {
            auto button = Buttons::toButton(keyReleased->code);
            processIOEvent(button, m_window.getMousePos(), Action::end);
        }
        if(const auto& mousePressed
           = event->getIf<sf::Event::MouseButtonPressed>())
        {
            auto button = Buttons::toButton(mousePressed->button);
            processIOEvent(button, mousePressed->position, Action::start);
        }
        if(const auto& mouseReleased
           = event->getIf<sf::Event::MouseButtonReleased>())
        {
            auto button = Buttons::toButton(mouseReleased->button);
            processIOEvent(button, mouseReleased->position, Action::end);
        }
    }
}
void GameEngine::processIOEvent(Buttons::Button button,
                                sf::Vector2i mousePos,
                                Action::Status status)
{
    const auto& actionMap = currentScene()->getActions();
    if(actionMap.find(button) == actionMap.end())
    {
        // the key is not mapped
        return;
    }

    auto mouseWorldPos = m_window.pixelToCoords(mousePos);
    if(!m_window.isInsideView(mouseWorldPos) && Buttons::isMouse(button))
    {
        // we have clicked outside the game world
        // and it was really a click and not a keyboard press event
        return;
    }

    // create the action and send to the scene for processing
    Action action{
        actionMap.at(button), status, m_window.pixelToCoords(mousePos)};
    currentScene()->sDoAction(action);
}

Scene* GameEngine::currentScene() { return m_scenes.getCurrentScene(); }
