#pragma once

#include "manager.hpp"

class MovementManager : public Manager {
public:
    MovementManager(entt::registry& registry) : Manager(registry) {}
public:
    virtual void tick(float deltaTime) override;
    virtual void init() override;
};

