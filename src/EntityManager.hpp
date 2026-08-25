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
    static void removeDeadEntities(EntityVec& vecE, EntityMap& map)
    {
        // use reset to release ownership of the managed object from the shared_Pointer
        // the release does not delete the managed object
        // the correct approach is to use the "erase_if" c++20 standard
        std::erase_if(vecE,
                      [](const std::shared_ptr<Entity>& e)
                      {
                          return e->isActive();
                          // as the destroy method will make this false
                      });
        // as we access the map itself then make sure that the isAlive is false in each map pair
        for (auto& [tag, entityVec] : map)
        {
            std::erase_if(entityVec,
                          [](const std::shared_ptr<Entity>& e)
                          {
                              return e->isActive();
                          });
        }
        // then acces the same map to remove the first pair
        std::erase_if(map,
                      [](const auto& pair)
                      {
                          return pair.second.empty();
                      });
    }

public:
    EntityManager() = default;

    void update()
    {
        for (auto& entity : m_entitiesToAdd)
        {
            m_entities.push_back(entity);
            // if tag exist then add it to the same vector
            // if new tag , create new vector then add the entity
            m_entityMap[entity->tag()].push_back(entity);
        }

        m_entitiesToAdd.clear();
        removeDeadEntities(m_entities, m_entityMap);

        /*
        for (auto e : m_entities)
        {
            // TODO: remove entities
        }
        */
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