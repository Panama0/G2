#pragma once

#include "Vec2.hpp"

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
           Vec2f worldPos = {},
           Vec2i windowPos = {})
        : m_typeIndex{idx}, m_status{t}, m_mouseWorldPos{worldPos},
          m_mouseWindowPos{windowPos}
    {
    }

    int type() const { return m_typeIndex; }
    Status status() const { return m_status; }

    const Vec2f& mousePosition() const { return m_mouseWorldPos; }

private:
    int m_typeIndex{0};
    Status m_status;
    Vec2f m_mouseWorldPos;
    Vec2f m_mouseWindowPos;
};
