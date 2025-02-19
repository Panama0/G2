#pragma once

#include <string>
#include <string_view>

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
    Action(int idx, ActionStatus t)
        :m_index {idx}
        ,m_status {t}
    {}
    
    int type() const { return m_index; }
    ActionStatus status() const { return m_status; }
private:
    int m_index {0};
    ActionStatus m_status;
};