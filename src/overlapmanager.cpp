#include "overlapmanager.hpp"

#include "components/geometry.hpp"
#include "utils.hpp"

#include <entt/entt.hpp>
#include <glm/geometric.hpp>
#include <glm/ext/vector_float2.hpp>

#include <iostream>
#include <optional>

void OverlapManager::tick(float deltaTime) {
    auto view = m_registry.view<HAR::Component::Location, HAR::Component::Overlap>();
    for (auto& entity1 : view) {
        auto& overlapComp1 = m_registry.get<HAR::Component::Overlap>(entity1);
        overlapComp1.overlapInfoMap.clear();
        for (auto& entity2 : view) {
            auto& overlapComp2 = m_registry.get<HAR::Component::Overlap>(entity2);

            if (entity1 == entity2 || entity1 > entity2) {
                continue;
            }
            overlapComp2.overlapInfoMap.clear();

            auto& loc1 = m_registry.get<HAR::Component::Location>(entity1);
            auto& loc2 = m_registry.get<HAR::Component::Location>(entity2);
            auto futureLoc1 = loc1.value; 
            auto futureLoc2 = loc2.value;
            if (m_registry.all_of<HAR::Component::Movement>(entity1)) {
                auto& movement = m_registry.get<HAR::Component::Movement>(entity1);
                futureLoc1 += movement.velocity * deltaTime;
            }

            if (m_registry.all_of<HAR::Component::Movement>(entity2)) {
                auto& movement = m_registry.get<HAR::Component::Movement>(entity2);
                futureLoc1 += movement.velocity * deltaTime;
            }

            auto overlapInfo = findOverlap(entity1, entity2, loc1.value, loc2.value, futureLoc1, futureLoc2, deltaTime);   
            if (overlapInfo.touchPoint.has_value() || overlapInfo.bFullOverlap || overlapInfo.overlapPoints.size() > 0) {
                overlapComp1.overlapInfoMap.emplace(entity2, overlapInfo);
                overlapComp2.overlapInfoMap.emplace(entity1, overlapInfo);
                std::cout << "overlap1 size: " << overlapComp1.overlapInfoMap.size() << std::endl <<
                    "overlap2 size: " << overlapComp2.overlapInfoMap.size() << std::endl;
                std::cout << "Overlap detected between " << std::endl;
            }
        }
    }
}

void OverlapManager::init() {
}

HAR::Component::Overlap::OverlapInfo OverlapManager::findOverlap(const entt::entity& a, const entt::entity& b,
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

HAR::Component::Overlap::OverlapInfo OverlapManager::findOverlap_Circle_x_Circle(const entt::entity& a, const entt::entity& b,
    const glm::vec2& currentLocationA, const glm::vec2& currentLocationB,
    const glm::vec2& potentialLocationA, const glm::vec2& potentialLocationB,
    float deltaTime) {

    auto& circleA = m_registry.get<HAR::Component::Circle>(a);
    auto& circleB = m_registry.get<HAR::Component::Circle>(b);

    float distanceCurrent = glm::distance(currentLocationA, currentLocationB);
    float distancePotential = glm::distance(potentialLocationA, potentialLocationB);
    float radiusSum = circleA.radius + circleB.radius;

    HAR::Component::Overlap::OverlapInfo overlapInfo;
    bool overlapsCurrent = distanceCurrent < radiusSum;
    bool overlapsPotential = distancePotential < radiusSum;


    if (overlapsCurrent) {
        overlapInfo.bFullOverlap = distancePotential < std::abs(circleA.radius - circleB.radius);
        if (overlapInfo.bFullOverlap) {
            return overlapInfo;
        }
        // TODO: Implement the logic of finding overlap points
        glm::vec2 direction = glm::normalize(potentialLocationB - potentialLocationA);
        float overlapDistance = radiusSum - distancePotential;
        glm::vec2 touchPoint = potentialLocationA + direction * (circleA.radius - overlapDistance / 2.0f);
        overlapInfo.touchPoint = touchPoint;
        float speed = glm::length(potentialLocationA - currentLocationA) / deltaTime;
        float timeSinceTouch = overlapDistance / speed;
        overlapInfo.timeSinceTouch = std::max(0.0f, deltaTime - timeSinceTouch);
    }
    return overlapInfo;
}

HAR::Component::Overlap::OverlapInfo OverlapManager::findOverlap_Circle_x_Polyhedron(
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

    HAR::Component::Overlap::OverlapInfo overlapInfo;

    // TODO: Move to collision body
    for (size_t i = 0; i < minkowskiVertices.size(); ++i) {
        glm::vec2 start = minkowskiVertices[i] + potentialLocationB;
        glm::vec2 end = minkowskiVertices[(i + 1) % minkowskiVertices.size()] + potentialLocationB;

        if (auto intersection = HAR::Math::getIntersection(start, end, currentLocationA, potentialLocationA);
        intersection.has_value() && glm::length(intersection.value() - currentLocationA) <= circle.radius) {
            if (!firstIntersection.has_value() || glm::length(intersection.value() - currentLocationA) < glm::length(firstIntersection.value() - currentLocationA)) {
                firstIntersection = intersection;
                break;
            }
        }
    }

    if (firstIntersection.has_value()) {
        overlapInfo.touchPoint = firstIntersection.value();
        float distanceToIntersection = glm::length(firstIntersection.value() - currentLocationA);
        float totalMovementLength = glm::length(movementVector);
        if (totalMovementLength > 0.0f) {
            float timeSinceTouch = (totalMovementLength - distanceToIntersection) / totalMovementLength * deltaTime;
            overlapInfo.timeSinceTouch = std::max(0.0f, deltaTime - timeSinceTouch);
        }
        intersects = true;
    }

    return overlapInfo;
}

HAR::Component::Overlap::OverlapInfo OverlapManager::findOverlap_Polyhedron_x_Polyhedron(
    const entt::entity& a, const entt::entity& b,
    const glm::vec2& currentLocationA, const glm::vec2& currentLocationB,
    const glm::vec2& potentialLocationA, const glm::vec2& potentialLocationB,
    float deltaTime) {
    // TODO: Implement
    return HAR::Component::Overlap::OverlapInfo();
}