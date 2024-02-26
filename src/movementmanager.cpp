#include "movementmanager.hpp"

#include "components/gameplay.hpp"
#include "glm/ext/scalar_constants.hpp"

#include <glm/ext/vector_float2.hpp>
#include <glm/geometric.hpp>

#include <cstdlib>
#include <iostream>

void MovementManager::tick(float deltaTime) {
    auto view = m_registry.view<HAR::Component::Location, HAR::Component::Movement>();
    for (auto& entity : view) {
        auto& location = view.get<HAR::Component::Location>(entity);
        auto& movement = view.get<HAR::Component::Movement>(entity);

        glm::vec2 controlDirection = glm::vec2(0.0f, 0.0f);
        if (m_registry.all_of<HAR::Component::Controlled>(entity)) {
            controlDirection = m_registry.get<HAR::Component::Controlled>(entity).movementDir;
            if (glm::length(controlDirection) > 0.0f) {
                controlDirection = glm::normalize(controlDirection);
            }
        }

        glm::vec2 acceleration = controlDirection * movement.acceleration;

        glm::vec2 velocityChange = acceleration * deltaTime;
        glm::vec2 newVelocity = movement.velocity + velocityChange;


        if ( m_registry.all_of<HAR::Component::PendingMovement>(entity)) {
            auto& pendingMovement = m_registry.get<HAR::Component::PendingMovement>(entity);
            newVelocity = pendingMovement.velocity + velocityChange;
            m_registry.remove<HAR::Component::PendingMovement>(entity);
        }

        if (glm::length(controlDirection) == 0.0f) {
            float frictionEffect = 1.0f - movement.speedLossFactor * deltaTime;
            newVelocity *= frictionEffect;
        }

        if (glm::length(newVelocity) > movement.maxSpeed) {
            newVelocity = glm::normalize(newVelocity) * movement.maxSpeed;
        }

        movement.velocity = newVelocity;
    }
}

void MovementManager::init() {
    auto view = m_registry.view<HAR::Component::Movement>();
    for (auto& entity : view) {
        auto& movement = view.get<HAR::Component::Movement>(entity);
        movement.velocity = glm::vec2(0.0f, 0.0f);
    }
}
