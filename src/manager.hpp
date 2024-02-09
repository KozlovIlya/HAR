#pragma once

#include <entt/entt.hpp>
#include <box2d/box2d.h>
#include <emscripten.h>
#include <memory>

class Manager {
public:
    Manager(entt::registry& registry) : m_registry(registry) {}
    virtual void tick(float deltaTime) {}
    virtual void init() {}
    virtual ~Manager() {}
protected:
    entt::registry& m_registry;
};
