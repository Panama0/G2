#pragma once

#include <string>

struct Action
{
    Action() = default;
    Action(std::string n, std::string t)
        :name {n}
        ,type {t}
    {}
    std::string name;
    std::string type;
};