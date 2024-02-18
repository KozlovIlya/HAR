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

        if (m_registry.all_of<HAR::Component::PhysicalBody, HAR::Component::Overlap>(entity)) {
            auto& overlapComp = m_registry.get<HAR::Component::Overlap>(entity);
            auto& physicalBodyComp = m_registry.get<HAR::Component::PhysicalBody>(entity);
            if (!overlapComp.overlapInfoMap.empty()) {
                for (auto& [overlapsEntity, overlapInfo] : overlapComp.overlapInfoMap) {
                    if (overlapInfo.touchPoint.has_value()) {
                        glm::vec2 collisionNormal = glm::normalize(location.value - overlapInfo.touchPoint.value());
                        movementComp.velocity = glm::reflect(movementComp.velocity, collisionNormal) * physicalBodyComp.hitPower;
                        futureLocation = overlapInfo.touchPoint.value() + collisionNormal * (glm::length(overlapInfo.touchPoint.value() - location.value) + 0.01f);
                    }
                }
                overlapComp.overlapInfoMap.clear();
            }
        }
        location.value = futureLocation;
    }
}
