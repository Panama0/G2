#pragma once

#include "engine/Window.hpp"

#include "SFML/Graphics.hpp"
#include "imgui.h"
#include "imgui-SFML.h"

#include <iostream>

class Gui
{
public:
    Gui() = default;
    Gui(sf::RenderWindow* window)
        :m_windowPtr {window}
    {}

    void init(Window& window)
    {
        m_windowPtr = &window.m_window;
        if(!ImGui::SFML::Init(*m_windowPtr)) { std::cerr << "Could init ImGui!\n"; };
    }
    
    void display() { ImGui::SFML::Render(*m_windowPtr); }
    void update(const sf::Time& dt) { ImGui::SFML::Update(*m_windowPtr, dt); }
    virtual void draw() = 0;
private:
    sf::RenderWindow* m_windowPtr;
};