#pragma once

#include "SFML/Graphics.hpp"
#include "imgui-SFML.h"
#include "imgui.h"

class Window
{
public:
    void init();

    void beginDraw() { m_window.clear(); }
    void draw(sf::Drawable& drawable) { m_window.draw(drawable); }
    void updateImGui(const sf::Time& dt) { ImGui::SFML::Update(m_window, dt); }
    void drawImGui() { ImGui::SFML::Render(m_window); }
    void render() { m_window.display(); }

    void updateView(sf::Vector2f size);
    const sf::View& getView() { return m_view; }
    const sf::Vector2u& getSize() { return m_renderSpace; }
    std::optional<sf::Event> getEvent() { return m_window.pollEvent(); }
    const sf::RenderWindow& getWindow() { return m_window; }
    
    sf::Vector2f pixelToCoords(const sf::Vector2i& pixel);

    void toggleFullscreen(bool mode);
    void toggleFullscreen();
private:
    sf::RenderWindow m_window;
    sf::View m_view;
    
    sf::Vector2u m_renderSpace {1280, 720};
    
    bool m_isFullscreen {false};
};