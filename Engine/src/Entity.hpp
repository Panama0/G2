#pragma once

#include "Components.hpp"
#include <map>
#include <string>
#include <tuple>

using ComponentTuple = std::tuple<cSprite,
                                   cTransform,
                                   cInput,
                                   cBoundingBox,
                                   cAnimatedSprite,
                                   cAnimation,
                                cEffect,
                                cId>;

class Entity
{
public:
    friend class EntityManager;

    bool isActive() const { return m_active; }
    void destroy() { m_active = false; }
    std::string tag() { return m_tag; }

    template <typename T> T& get() { return std::get<T>(m_components); }

    template <typename T> const T& get() const
    {
        return std::get<T>(m_components);
    }

    template <typename T> bool has() const { return get<T>().exists; }

    template <typename T, typename... TArgs> T& add(TArgs&&... mArgs)
    {
        auto& component{get<T>()};
        component = T(std::forward<TArgs>(mArgs)...);
        component.exists = true;
        return component;
    }

    template <typename T> void remove() { get<T>() = T(); }

private:
    ComponentTuple m_components;
    bool m_active{true};
    std::string m_tag;

    Entity(std::string tag) : m_tag{tag} {}
};