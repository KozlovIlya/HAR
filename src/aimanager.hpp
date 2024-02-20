#pragma once

#include "manager.hpp"

#include "components/gameplay.hpp"

#include <entt/entity/fwd.hpp>

#include <vector>

class AIManager : public Manager {
public:
    AIManager(entt::registry& registry) : Manager(registry) {}
    virtual ~AIManager() {}
public:
    void tick(float deltaTime) override {
        auto view = m_registry.view<HAR::Component::AI>();
        for (auto& entity : view) {
            auto& ai = m_registry.get<HAR::Component::AI>(entity);
            ai.tick(m_registry, entity, deltaTime);
        }
    }
    virtual void init() override {
    }
};