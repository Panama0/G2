#pragma once

#include "SFML/System/Vector2.hpp"

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
    Action(int idx,
           ActionStatus t,
           sf::Vector2f worldPos = {},
           sf::Vector2i windowPos = {})
        : m_index{idx}, m_status{t}, m_mouseWorldPos{worldPos},
          m_mouseWindowPos{windowPos}
    {
    }

    int type() const { return m_index; }
    ActionStatus status() const { return m_status; }

    const sf::Vector2f& mousePosition() const { return m_mouseWorldPos; }

private:
    int m_index{0};
    ActionStatus m_status;
    sf::Vector2f m_mouseWorldPos;
    sf::Vector2f m_mouseWindowPos;
};
