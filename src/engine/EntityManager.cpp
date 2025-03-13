#include "engine/EntityManager.hpp"

#include <algorithm>

void EntityManager::update()
{
    // add entity
    for(const auto& entitiy : m_entitiesToAdd)
    {
        m_entities.push_back(entitiy);
    }

    // removing entities
    removeDeadEntities(m_entities);
    for(auto& [tags, entities] : m_entityMap)
    {
        removeDeadEntities(entities);
    }
    m_entitiesToAdd.clear();
}

const EntityVec& EntityManager::getEntities(const std::string& tag)
{
    if(m_entityMap.find(tag) == m_entityMap.end())  // if it is empty
    {
        m_entityMap[tag] = EntityVec();
    }
    return m_entityMap[tag];
}

std::shared_ptr<Entity> EntityManager::addEntity(const std::string& tag)
{
    auto entity = std::shared_ptr<Entity>(new Entity(m_totalEntities++, tag));
    
    m_entitiesToAdd.push_back(entity);
    
    if(m_entityMap.find(tag) == m_entityMap.end())  // if it is empty
    {
        m_entityMap[tag] = EntityVec();
    }
    
    m_entityMap[tag].push_back(entity);
    
    return entity;
}
    
void EntityManager::removeDeadEntities(EntityVec& vec)
{
    vec.erase(std::remove_if(vec.begin(), vec.end(), 
    [](auto& ent)
    {return !ent->isActive();}), vec.end());
}
