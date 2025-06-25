#pragma once

#include "SFML/Graphics.hpp"
#include "SFML/Window/Mouse.hpp"
#include "Vec2.hpp"

class Window
{
public:
    friend class Gui;

    void init();

    void beginDraw() { m_window.clear({}); }
    void draw(sf::Drawable& drawable) { m_window.draw(drawable); }
    void render() { m_window.display(); }

    void updateView(Vec2f size);
    const sf::View& getView() { return m_view; }
    const Vec2u& getSize() { return m_renderSpace; }
    std::optional<sf::Event> getEvent() { return m_window.pollEvent(); }
    const sf::RenderWindow& getWindow() { return m_window; }
    Vec2i getMousePos() { return static_cast<Vec2i>(sf::Mouse::getPosition(m_window)); }

    Vec2f pixelToCoords(const Vec2i& pixel);
    Vec2i coordsToPixel(const Vec2f& point);

    bool isInsideView(const Vec2f& pos);

    void toggleFullscreen(bool mode);
    void toggleFullscreen();

private:
    sf::RenderWindow m_window;
    sf::View m_view;

    Vec2u m_renderSpace{1280, 720};

    bool m_isFullscreen{false};
};
