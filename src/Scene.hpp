#pragma once

#include "GameEngine.hpp"
#include "EntityManager.hpp"
#include "Action.hpp"
#include <map>
#include <string>
#include <memory>

class GameEngine;

class Scene
{
public:
    virtual void update() = 0;
    virtual void sDoAction() = 0;
    virtual void sRender() = 0;
    
    void simulate(int iterations);
    void doAction(const Action& a);
    void registerAction(const Action& a);
private:
    std::shared_ptr<GameEngine> m_game;
    EntityManager m_entities;
    int m_currentFrame {};
    //? what is this 
    std::map<Action, std::string> m_actionMap;
    bool m_paused {false};
    bool m_hasEnded {false};
};