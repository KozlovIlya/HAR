#pragma once

#include "manager.hpp"

class PhysicsManager : public Manager {
public:
    PhysicsManager(entt::registry& registry) : Manager(registry) {}
    virtual ~PhysicsManager() {}
public:
    void tick(float deltaTime) override;
    virtual void init() override;
};