#include "movementmanager.hpp"

#include "components/gameplay.hpp"

#include <glm/ext/vector_float2.hpp>
#include <glm/geometric.hpp>

#include <cstdlib>

void MovementManager::tick(float deltaTime) {
    auto view = m_registry.view<HAR::Component::Location, HAR::Component::Movement>();
    for (auto& entity : view) {
        auto& location = view.get<HAR::Component::Location>(entity);
        auto& movement = view.get<HAR::Component::Movement>(entity);
        glm::vec2 collisionFactor = glm::vec2(1.0f, 1.0f);
        // if (m_registry.all_of<HAR::Component::PhysicalBody>()
        glm::vec2 accelerationVector = movement.direction * movement.acceleration;
        movement.velocity += accelerationVector * deltaTime;
        float speed = glm::length(movement.velocity);
        if (speed > movement.maxSpeed) {
            movement.velocity = glm::normalize(movement.velocity) * movement.maxSpeed;
        }
        if (glm::length(movement.direction) == 0.0f) {
            movement.velocity *= pow(movement.inertiaFactor, deltaTime);
        }
        location.value += movement.velocity * deltaTime;
    }
}

void MovementManager::init() {
    auto view = m_registry.view<HAR::Component::Movement>();
    for (auto& entity : view) {
        auto& movement = view.get<HAR::Component::Movement>(entity);
        movement.velocity = glm::vec2(0.0f, 0.0f);
        movement.direction = glm::vec2(0.0f, 0.0f);
    }
}
