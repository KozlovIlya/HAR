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
        auto& locationComp = view.get<HAR::Component::Location>(entity);
        auto& movementComp = view.get<HAR::Component::Movement>(entity);
        auto futureLocation = locationComp.value + movementComp.velocity * deltaTime;
        if (m_registry.all_of<HAR::Component::PhysicalBody, HAR::Component::Overlap>(entity)) {
            auto& overlapComp = m_registry.get<HAR::Component::Overlap>(entity);
            for (auto& [overlapsEntity, overlapInfo] : overlapComp.overlapInfoMap) {
                // TODO: temp solution. need to be replaced with some kind of collision response
                if (m_registry.any_of<HAR::Component::AI, HAR::Component::Player>(entity) &&
                    m_registry.any_of<HAR::Component::AI, HAR::Component::Player>(overlapsEntity)) {
                    // WOoOp! MaGiC O_o
                } else if (m_registry.all_of<HAR::Component::PhysicalBody, HAR::Component::Location>(overlapsEntity) && overlapInfo.touchPoint.has_value()) {
                    auto collisionNormal = glm::normalize(locationComp.value - overlapInfo.touchPoint.value());
                    futureLocation = locationComp.value;
                }
            }
        }
        locationComp.value = futureLocation;
    }
}
