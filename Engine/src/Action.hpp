#pragma once

#include "SFML/Graphics.hpp"

class Action
{
public:
    enum ActionStatus
    {
        null,
        start,
        end,
    };

    Action() = default;
    Action(int idx, ActionStatus t, sf::Vector2f pos = {0.f, 0.f})
        : m_index{idx}, m_status{t}, m_mousePos{pos}
    {
    }

    int type() const { return m_index; }
    ActionStatus status() const { return m_status; }


    const sf::Vector2f& mousePosition() const { return m_mousePos; }

private:
    int m_index{0};
    ActionStatus m_status;
    sf::Vector2f m_mousePos;
};
