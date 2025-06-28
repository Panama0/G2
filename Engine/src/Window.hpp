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
    const sf::View& getView() const { return m_view; }
    Vec2u getSize() const { return Vec2u{m_window.getSize()}; }
    std::optional<sf::Event> getEvent() { return m_window.pollEvent(); }
    const sf::RenderWindow& getWindow() { return m_window; }
    Vec2i getMousePos() const
    {
        return static_cast<Vec2i>(sf::Mouse::getPosition(m_window));
    }

    Vec2f pixelToCoords(const Vec2i& pixel) const;
    Vec2i coordsToPixel(const Vec2f& point) const;

    bool isInsideView(const Vec2f& pos) const;

    void toggleFullscreen(bool mode);
    void toggleFullscreen();

private:
    sf::RenderWindow m_window;
    sf::View m_view;

    Vec2u m_renderSpace{1280, 720};

    bool m_isFullscreen{false};
};
