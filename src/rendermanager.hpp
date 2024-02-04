#pragma once

#include "manager.hpp"

class RenderManager : public Manager {
public:
    RenderManager(entt::registry& registry) : Manager(registry) {}
    virtual void tick(float deltaTime) override;
    virtual void init() override;
    virtual ~RenderManager() override {};
};
