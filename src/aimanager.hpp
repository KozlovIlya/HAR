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
    virtual void tick(float deltaTime) override;
    virtual void init() override {
    }
};