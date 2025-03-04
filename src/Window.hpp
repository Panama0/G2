#pragma once

#include "SFML/Graphics.hpp"

class Window
{
public:
    void init();
    void beginDraw() { m_window.clear(); }
    void draw(sf::Drawable& drawable) { m_window.draw(drawable); }
    void render() { m_window.display(); }
    void updateView(sf::Vector2f size);
    const sf::View& getView() { return m_view; }
    std::optional<sf::Event> getEvent() { return m_window.pollEvent(); }
    const sf::Window& getWindow() { return m_window; }
    void toggleFullscreen(bool mode);
    void toggleFullscreen();
private:
    sf::RenderWindow m_window;
    sf::View m_view;
    
    sf::Vector2u m_renderSpace {1280, 720};
    
    bool m_isFullscreen {false};
};