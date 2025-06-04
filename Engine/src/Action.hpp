#pragma once

#include "SFML/System/Vector2.hpp"

class Action
{
public:
    enum Status
    {
        null,
        start,
        end,
    };

    Action() = default;
    Action(int idx,
           Status t,
           sf::Vector2f worldPos = {},
           sf::Vector2i windowPos = {})
        : m_typeIndex{idx}, m_status{t}, m_mouseWorldPos{worldPos},
          m_mouseWindowPos{windowPos}
    {
    }

    int type() const { return m_typeIndex; }
    Status status() const { return m_status; }

    const sf::Vector2f& mousePosition() const { return m_mouseWorldPos; }

private:
    int m_typeIndex{0};
    Status m_status;
    sf::Vector2f m_mouseWorldPos;
    sf::Vector2f m_mouseWindowPos;
};
