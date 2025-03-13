#pragma once

#include "engine/Entity.hpp"
#include <memory>
#include <map>
#include <vector>

class EntityManager;
using EntityVec = std::vector<std::shared_ptr<Entity>>;
using EntityMap = std::map<std::string, EntityVec>;

class EntityManager
{
public:
    EntityManager() = default;
    
    void update();
    
    const EntityVec& getEntities() { return m_entities; }
    const EntityVec& getEntities(const std::string& tag);
    
    const EntityMap& getEntityMap() {return m_entityMap;}
    
    std::shared_ptr<Entity> addEntity(const std::string& tag);
    
private:
    void removeDeadEntities(EntityVec& vec);

    EntityVec m_entities;
    EntityVec m_entitiesToAdd;
    EntityMap m_entityMap;
    size_t m_totalEntities{};
};