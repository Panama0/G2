#include "Window.hpp"

#include "imgui-SFML.h"
#include "imgui.h"

#include <iostream>

void Window::init()
{
    sf::State state = static_cast<sf::State>(m_isFullscreen);

    m_window.create(sf::VideoMode{m_renderSpace}, "G2", state);
    m_view = m_window.getView();

    updateView(static_cast<sf::Vector2f>(m_renderSpace));
    m_window.setFramerateLimit(60u);
    if(!ImGui::SFML::Init(m_window))
    {
        std::cerr << "Could not init ImGui!\n";
    }
}

void Window::updateView(sf::Vector2f size)
{
    float desiredRatio{size.x / size.y};
    float currentRatio{m_view.getSize().x / m_view.getSize().y};

    sf::Vector2f newSize{1.f, 1.f};
    sf::Vector2f newPos{0.f, 0.f};

    bool horizontalSpacing{true};
    if(desiredRatio < currentRatio)
    {
        horizontalSpacing = false;
    }

    if(horizontalSpacing)
    {
        newSize.x = currentRatio / desiredRatio;
        newPos.x = (1 - newSize.x) / 2.f;
    }
    else
    {
        newSize.y = desiredRatio / currentRatio;
        newPos.y = (1 - newSize.y) / 2.f;
    }

    m_view.setViewport(sf::FloatRect(newPos, newSize));
    m_window.setView(m_view);
}

sf::Vector2f Window::pixelToCoords(const sf::Vector2i& pixel)
{
    return m_window.mapPixelToCoords(pixel, m_view);
}

sf::Vector2i Window::coordsToPixel(const sf::Vector2f& point)
{
    return m_window.mapCoordsToPixel(point, m_view);
}

bool Window::isInsideView(const sf::Vector2f& pos)
{
    auto viewSize = m_view.getSize();
    auto viewCenter = m_view.getCenter();

    sf::FloatRect viewRect{viewCenter - viewSize / 2.f, viewSize};

    if(!viewRect.contains(pos))
    {
        // we have clicked outside the view area
        return false;
    }
    return true;
}

void Window::toggleFullscreen(bool mode)
{
    if(m_isFullscreen == mode)
    {
        return;
    }

    m_isFullscreen = mode;

    m_window.close();
    init();
}

void Window::toggleFullscreen()
{
    if(m_isFullscreen)
    {
        m_isFullscreen = false;
    }
    else
    {
        m_isFullscreen = true;
    }
    m_window.close();
    init();
}
