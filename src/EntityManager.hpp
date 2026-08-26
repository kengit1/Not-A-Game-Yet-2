#pragma once

#include "Entity.hpp"

#include <cstddef>
#include <map>
#include <memory>
#include <string>
#include <vector>

using EntityVec = std::vector<std::shared_ptr<Entity>>;
using EntityMap = std::map<std::string, EntityVec>;

class EntityManager
{
    EntityVec m_entities;
    EntityVec m_entitiesToAdd; // for delayed addEntity to avoid iterator invalid
    EntityMap m_entityMap;
    std::size_t m_totalEntities = 0;

    //? does it have to be really static ?
    static void removeDeadEntities(EntityVec& vecE)
    {
        // use reset to release ownership of the managed object from the shared_Pointer
        // the release does not delete the managed object
        // the correct approach is to use the "erase_if" c++20 standard
        std::erase_if(vecE,
                      [](const std::shared_ptr<Entity>& e)
                      {
                          return !e->isActive();
                          // as the destroy method will make this false
                      });
    }

public:
    EntityManager() = default;

    void update()
    {
        for (auto& entity : m_entitiesToAdd)
        {
            m_entities.push_back(entity);
            m_entityMap[entity->tag()].push_back(entity);
        }

        m_entitiesToAdd.clear();

        removeDeadEntities(m_entities);

        for (auto& [tag, entityVec] : m_entityMap)
        {
            removeDeadEntities(entityVec);
        }
    }

    std::shared_ptr<Entity> addEntity(const std::string& tag)
    {
        auto entity = std::shared_ptr<Entity>(new Entity(m_totalEntities++, tag));

        m_entitiesToAdd.push_back(entity);

        return entity;
    }

    const EntityVec& getEntities()
    {
        return m_entities;
    }

    const EntityVec& getEntities(const std::string& tag)
    {
        return m_entityMap[tag];
    }
};