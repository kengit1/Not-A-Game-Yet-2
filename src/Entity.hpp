#pragma once

#include "Components.hpp"

#include <cstddef>
#include <string>
#include <tuple>
#include <utility>

using ComponentTuple = std::tuple<CTransform, CShape, CCollision, CInput, CScore, CLifespan>;

class Entity
{
    friend class EntityManager;

    ComponentTuple m_components;
    bool m_alive = true;
    std::size_t m_id = 0;
    std::string m_tag = "default";

    // Constructor
    Entity(const std::size_t id, const std::string& tag)
        : m_id(id),
          m_tag(tag)
    {
    }

public:
    Entity() = default;

    bool isActive() const
    {
        return m_alive;
    }

    const std::string& tag() const
    {
        return m_tag;
    }

    std::size_t id() const
    {
        return m_id;
    }

    void destroy()
    {
        m_alive = false;
    }

    template <typename T, typename... TArgs>
    T& addComponent(TArgs&&... args)
    {
        auto& component = std::get<T>(m_components);

        component = T(std::forward<TArgs>(args)...);
        component.exists = true;

        return component;
    }

    template <typename T>
    T& getComponent()
    {
        return std::get<T>(m_components);
    }

    template <typename T>
    const T& getComponent() const
    {
        return std::get<T>(m_components);
    }
};