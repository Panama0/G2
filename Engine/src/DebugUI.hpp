#pragma once

#include "Assets.hpp"
#include "EntityManager.hpp"
#include "Gui.hpp"
#include "Window.hpp"
#include <cstdint>

struct EngineState
{
    const sf::Time& dt;
    uint32_t maxFps{};
    const Window& window;
    const Assets& assets;
    const SECS::EntityManager& entities;
};

class DebugUI : public Gui
{
public:
    DebugUI(const sf::Time& _dt, uint32_t _maxFps,
            Window& _window,
            const Assets& _assets,
            const SECS::EntityManager& _entities)
        : m_state{_dt,_maxFps, _window, _assets, _entities}
    {
        init(_window);
    }

    void draw() override;
    EngineState& getState() { return m_state; }

private:
    EngineState m_state;
};
