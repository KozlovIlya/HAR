#pragma once

#include "entt/entity/fwd.hpp"
#include "manager.hpp"
#include "components/geometry.hpp"
#include "components/gameplay.hpp"
#include "utils.hpp"

#include <entt/entity/entity.hpp>
#include <glm/geometric.hpp>
#include <glm/ext/vector_float2.hpp>

#include <iostream>
#include <optional>
#include <stdexcept>
#include <unordered_map>
#include <unordered_set>
#include <vector>


class OverlapManager : public Manager {

public:
    OverlapManager(entt::registry& registry) : Manager(registry) {}
public:
    void tick(float deltaTime) override {
        auto view = m_registry.view<HAR::Component::Location, HAR::Component::Overlap>();
        for (auto& entity1 : view) {
            for (auto& entity2 : view) {
                if (entity1 == entity2) continue;
                if (entity1 > entity2) continue;
                auto& loc1 = m_registry.get<HAR::Component::Location>(entity1);
                auto& loc2 = m_registry.get<HAR::Component::Location>(entity2);


                glm::vec2 futureLoc1 = updateFutureLocation(entity1, deltaTime, loc1.value);
                glm::vec2 futureLoc2 = updateFutureLocation(entity2, deltaTime, loc2.value);
                auto result = findOverlap(entity1, entity2, loc1.value, loc2.value, futureLoc1, futureLoc2, deltaTime);   
                if (result.touchPoint.has_value()) {
                    std::cout << "Touch point: " << result.touchPoint.value_or(glm::vec2(0.0f, 0.0f)).x << " " << result.touchPoint.value_or(glm::vec2(0.0f, 0.0f)).y << std::endl <<
                    "location: " << loc1.value.x << " " << loc1.value.y << " " << loc2.value.x << " " << loc2.value.y << std::endl <<
                    "future location: " << futureLoc1.x << " " << futureLoc1.y << " " << futureLoc2.x << " " << futureLoc2.y << std::endl <<
                    "distance between touch point and location" << glm::distance(result.touchPoint.value_or(glm::vec2(0.0f, 0.0f)), loc1.value) << " " << glm::distance(result.touchPoint.value_or(glm::vec2(0.0f, 0.0f)), loc2.value) << std::endl;
                    
                        #include "components/render.hpp"
                        auto entityNewS = m_registry.create();
                        m_registry.emplace<HAR::Component::Renderable>(entityNewS, true);
                        m_registry.emplace<HAR::Component::Color>(entityNewS, glm::vec4(0.f, 0.f, 1.f, 1.f));
                        m_registry.emplace<HAR::Component::Circle>(entityNewS, 0.05f);
                        m_registry.emplace<HAR::Component::Location>(entityNewS, loc1.value);
                        
                        auto entityNewF = m_registry.create();
                        m_registry.emplace<HAR::Component::Renderable>(entityNewF, true);
                        m_registry.emplace<HAR::Component::Color>(entityNewF, glm::vec4(1.f, 0.f, 0.f, 1.f));
                        m_registry.emplace<HAR::Component::Circle>(entityNewF, 0.05f);
                        m_registry.emplace<HAR::Component::Location>(entityNewF, futureLoc1);



                    if (m_registry.all_of<HAR::Component::Movement>(entity1)) {
                        auto& movement = m_registry.get<HAR::Component::Movement>(entity1);
                            auto& movement1 = m_registry.get<HAR::Component::Movement>(entity1);
                            glm::vec2 collisionNormal = glm::normalize(loc1.value - result.touchPoint.value());
                            float restitutionCoefficient = 10.5f; // Коэффициент отражения (0 - полное затухание, 1 - идеальное отражение)
                            movement1.velocity = glm::reflect(movement1.velocity, collisionNormal) * restitutionCoefficient;
                            futureLoc1 = (loc1.value - movement1.velocity * 0.01f) + movement1.velocity * deltaTime;                    }
                    if (m_registry.all_of<HAR::Component::Movement>(entity2)) {
                        auto& movement = m_registry.get<HAR::Component::Movement>(entity2);
                    }
                }
                loc1.value = futureLoc1;
                loc2.value = futureLoc2;
            }
        }
    }

    virtual void init() override {
    }
protected:
    HAR::Component::Overlap::OverlapCheckResult findOverlap(const entt::entity& a, const entt::entity& b,
    const glm::vec2& currentlocationA, const glm::vec2& currentlocationB,
    const glm::vec2& potentialLocationA, const glm::vec2& potentialLocationB,
    float deltaTime) {
        if (m_registry.all_of<HAR::Component::Circle>(a) && m_registry.all_of<HAR::Component::Circle>(b)) {
            return findOverlap_Circle_x_Circle(a, b,
            currentlocationA, currentlocationB,
            potentialLocationA, potentialLocationB,
            deltaTime);
        }
        if (m_registry.all_of<HAR::Component::Circle>(a) && m_registry.all_of<HAR::Component::Polyhedron>(b)) {
            return findOverlap_Circle_x_Polyhedron(a, b,
            currentlocationA, currentlocationB,
            potentialLocationA, potentialLocationB,
            deltaTime);
        }
        if (m_registry.all_of<HAR::Component::Polyhedron>(a) && m_registry.all_of<HAR::Component::Polyhedron>(b)) {
            return findOverlap_Polyhedron_x_Polyhedron(a, b,
            currentlocationA, currentlocationB,
            potentialLocationA, potentialLocationB,
            deltaTime);
        }
        return {};
    }
protected:
    HAR::Component::Overlap::OverlapCheckResult findOverlap_Circle_x_Circle(const entt::entity& a, const entt::entity& b,
        const glm::vec2& currentLocationA, const glm::vec2& currentLocationB,
        const glm::vec2& potentialLocationA, const glm::vec2& potentialLocationB,
        float deltaTime) {

        auto& circleA = m_registry.get<HAR::Component::Circle>(a);
        auto& circleB = m_registry.get<HAR::Component::Circle>(b);

        float distanceCurrent = glm::distance(currentLocationA, currentLocationB);
        float distancePotential = glm::distance(potentialLocationA, potentialLocationB);
        float radiusSum = circleA.radius + circleB.radius;

        HAR::Component::Overlap::OverlapCheckResult checkResult;
        bool overlapsCurrent = distanceCurrent < radiusSum;
        bool overlapsPotential = distancePotential < radiusSum;

        if (overlapsCurrent) {
            checkResult.bFullOverlap = distancePotential < std::abs(circleA.radius - circleB.radius);
            if (checkResult.bFullOverlap) {
                return checkResult;
            }
            // TODO: Implement the logic of finding overlap points
            glm::vec2 direction = glm::normalize(potentialLocationB - potentialLocationA);
            float overlapDistance = radiusSum - distancePotential;
            glm::vec2 touchPoint = potentialLocationA + direction * (circleA.radius - overlapDistance / 2.0f);
            checkResult.touchPoint = touchPoint;
            float speed = glm::length(potentialLocationA - currentLocationA) / deltaTime;
            float timeSinceTouch = overlapDistance / speed;
            checkResult.timeSinceTouch = std::max(0.0f, deltaTime - timeSinceTouch);
        }
        return checkResult;
    }
    HAR::Component::Overlap::OverlapCheckResult findOverlap_Circle_x_Polyhedron(
        const entt::entity& a, const entt::entity& b,
        const glm::vec2& currentLocationA, const glm::vec2& currentLocationB,
        const glm::vec2& potentialLocationA, const glm::vec2& potentialLocationB,
        float deltaTime) {
        
        auto& circle = m_registry.get<HAR::Component::Circle>(a);
        auto& polyhedron = m_registry.get<HAR::Component::Polyhedron>(b);
        auto minkowskiVertices = HAR::Math::InflatePolygon(polyhedron.vertices, circle.radius);
        glm::vec2 movementVector = potentialLocationA - currentLocationA;
        bool intersects = false;
        std::optional<glm::vec2> firstIntersection;

        HAR::Component::Overlap::OverlapCheckResult checkResult;

        for (size_t i = 0; i < minkowskiVertices.size(); ++i) {
            glm::vec2 start = minkowskiVertices[i] + potentialLocationB;
            glm::vec2 end = minkowskiVertices[(i + 1) % minkowskiVertices.size()] + potentialLocationB;

            if (auto intersection = HAR::Math::getIntersection(start, end, currentLocationA, potentialLocationA) ;
            intersection.has_value() && glm::length(intersection.value() - currentLocationA) <= circle.radius) {
                std::cout << "difference: " << glm::length(intersection.value() - currentLocationA) << " " << circle.radius << std::endl;
                if (!firstIntersection.has_value() || glm::length(intersection.value() - currentLocationA) < glm::length(firstIntersection.value() - currentLocationA)) {
                    firstIntersection = intersection;
                    break;
                }
            }
        }

        if (firstIntersection.has_value()) {
            checkResult.touchPoint = firstIntersection.value();
            float distanceToIntersection = glm::length(firstIntersection.value() - currentLocationA);
            float totalMovementLength = glm::length(movementVector);
            if (totalMovementLength > 0.0f) {
                float timeSinceTouch = (totalMovementLength - distanceToIntersection) / totalMovementLength * deltaTime;
                checkResult.timeSinceTouch = std::max(0.0f, deltaTime - timeSinceTouch);
                std::cout << "Touch point: " << checkResult.touchPoint.value_or(glm::vec2(0.0f, 0.0f)).x << " " << checkResult.touchPoint.value_or(glm::vec2(0.0f, 0.0f)).y << std::endl <<
                "distance : " << distanceToIntersection << " " << totalMovementLength << " " << timeSinceTouch << std::endl <<
                "totamovelen" << totalMovementLength - distanceToIntersection << std::endl;
            }
            intersects = true;
        }

        return checkResult;
    }


    HAR::Component::Overlap::OverlapCheckResult findOverlap_Polyhedron_x_Polyhedron(
        const entt::entity& a, const entt::entity& b,
        const glm::vec2& currentLocationA, const glm::vec2& currentLocationB,
        const glm::vec2& potentialLocationA, const glm::vec2& potentialLocationB,
        float deltaTime) {
        // TODO: Implement
        return HAR::Component::Overlap::OverlapCheckResult();
    }

protected:

    glm::vec2 updateFutureLocation(const entt::entity& entity, float deltaTime, glm::vec2 futureLoc) {
        if (m_registry.all_of<HAR::Component::Movement>(entity)) {
            auto& movement = m_registry.get<HAR::Component::Movement>(entity);
            glm::vec2 accelerationVector = movement.direction * movement.acceleration;
            movement.velocity += accelerationVector * deltaTime;
            float speed = glm::length(movement.velocity);
            if (speed > movement.maxSpeed) {
                movement.velocity = glm::normalize(movement.velocity) * movement.maxSpeed;
            }
            if (glm::length(movement.direction) < std::numeric_limits<float>::epsilon()) {
                movement.velocity *= pow(movement.inertiaFactor, deltaTime);
            }
            futureLoc += movement.velocity * deltaTime;
        }
        return futureLoc;
    }

};