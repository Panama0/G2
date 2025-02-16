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
    Action(std::string n, ActionStatus t)
        :m_name {n}
        ,m_status {t}
    {}
    
    std::string_view name() const { return m_name; }
    ActionStatus status() const { return m_status; }
private:
    std::string m_name;
    ActionStatus m_status;
};