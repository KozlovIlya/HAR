#include "physicsmanager.hpp"

#include "components/gameplay.hpp"
#include "glm/geometric.hpp"

#include <glm/glm.hpp>

#include <iostream>

void PhysicsManager::init() {
}

void PhysicsManager::tick(float deltaTime) {
    auto view = m_registry.view<HAR::Component::Movement, HAR::Component::Location>();
    for (auto& entity : view) {
        auto& location = view.get<HAR::Component::Location>(entity);
        auto& movementComp = view.get<HAR::Component::Movement>(entity);
        auto futureLocation = location.value + movementComp.velocity * deltaTime;
        location.value = futureLocation;
    }
}
