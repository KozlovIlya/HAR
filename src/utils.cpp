#include "utils.hpp"

#include <any>
#include <glm/glm.hpp>
#include <iostream>

#include "components/gameplay.hpp"
#include "components/geometry.hpp"
#include "point.h"

#include <path_finder.h>
#include <polygon.h>

std::vector<glm::vec2> HAR::Math::InflatePolygon(const std::vector<glm::vec2>& vertices, float radius) {
    std::vector<glm::vec2> inflatedVertices;
    int n = vertices.size();

    for (int i = 0; i < n; i++) {
        glm::vec2 edge = vertices[(i + 1) % n] - vertices[i];
        glm::vec2 normal(-edge.y, edge.x);
        normal = glm::normalize(normal);
        inflatedVertices.push_back(vertices[i] + normal * radius);
        inflatedVertices.push_back(vertices[(i + 1) % n] + normal * radius);
    }

    return inflatedVertices;
}

std::optional<glm::vec2> HAR::Math::getIntersection(
    const glm::vec2& sA, const glm::vec2& fA,
    const glm::vec2& sB, const glm::vec2& fB)
{
    glm::vec2 r = fA - sA; // Вектор отрезка AB
    glm::vec2 s = fB - sB; // Вектор отрезка CD
    glm::vec2 qp = sB - sA; // Вектор между начальными точками AB и CD
    float rxs = glm::cross(glm::vec3(r, 0.0f), glm::vec3(s, 0.0f)).z;
    float qpxr = glm::cross(glm::vec3(qp, 0.0f), glm::vec3(r, 0.0f)).z;

    if (abs(rxs) < 1e-8) {
        return {}; // Отрезки параллельны или лежат на одной прямой
    }

    float t = glm::cross(glm::vec3(qp, 0.0f), glm::vec3(s, 0.0f)).z / rxs;
    float u = qpxr / rxs;
    if (t >= 0.0f && t <= 1.0f && u >= 0.0f && u <= 1.0f) {
        return sA + t * r; // Точка пересечения находится на обоих отрезках
    }

    return {}; // Отрезки не пересекаются
}

void HAR::AI::chasePlayer(entt::registry& registry, const entt::entity& entity, float deltaTime) {
    // if (!registry.all_of<HAR::Component::Location, HAR::Component::AI, HAR::Component::Controlled>(entity)) {
    //     return;
    // }

    // auto& aiLocationComp = registry.get<HAR::Component::Location>(entity);
    // auto& aiControlledComp = registry.get<HAR::Component::Controlled>(entity);
    // auto& aiComp = registry.get<HAR::Component::AI>(entity);
    
    // auto pathFinder = std::any_cast<NavMesh::PathFinder*>(aiComp.data);
    // if (!pathFinder) {
    //     return;
    // }

    // std::vector<NavMesh::Polygon> polygons_to_add;
    // auto polygonView = registry.view<HAR::Component::Location, HAR::Component::PhysicalBody, HAR::Component::Polyhedron>();
    // for (auto& polygonEntity : polygonView) {
    //     NavMesh::Polygon polygon;
    //     auto& polygonLocationComp = polygonView.get<HAR::Component::Location>(polygonEntity);
    //     auto& polygonPolyhedronComp = polygonView.get<HAR::Component::Polyhedron>(polygonEntity);
    //     for (auto& vertex : polygonPolyhedronComp.vertices) {
    //         NavMesh::Point point(vertex.x * 10, vertex.y * 10, 0.0f);
    //         // polygon.AddPoint(vertex.x, vertex.y, 0.0f);
    //     }

    //     polygons_to_add.push_back(NavMesh::Polygon(polygonPolyhedronComp.vertices));
    //     pathFinder->AddPolygons(polygons_to_add, 0);
    // }
    // pathFinder->AddPolygons(const std::vector<Polygon> &polygons_to_add, int inflate_by)

    // auto view = registry.view<HAR::Component::Location, HAR::Component::Player>();
    // for (auto& playerEntity : view) {
    //     auto& playerlocationComp = registry.get<HAR::Component::Location>(playerEntity);
    //     auto direction = glm::normalize(playerlocationComp.value - aiLocationComp.value);
    //     aiControlledComp.movementDir = direction;
    // }
}

void HAR::Collision::pushCollidedComponents(entt::registry& registry, const entt::entity& entity) {
        auto view = registry.view<HAR::Component::Movement, HAR::Component::Location>();
    for (auto& entity : view) {
        auto& location = view.get<HAR::Component::Location>(entity);
        auto& movementComp = view.get<HAR::Component::Movement>(entity);
        auto futureLocation = location.value + movementComp.velocity * 0.1f;

        if (registry.all_of<HAR::Component::PhysicalBody, HAR::Component::Overlap>(entity)) {
            auto& overlapComp = registry.get<HAR::Component::Overlap>(entity);
            auto& physicalBodyComp = registry.get<HAR::Component::PhysicalBody>(entity);
            if (!!!overlapComp.overlapInfoMap.empty()) {
                for (auto& [overlapsEntity, overlapInfo] : overlapComp.overlapInfoMap) {
                    if (registry.all_of<HAR::Component::PhysicalBody>(overlapsEntity) && overlapInfo.touchPoint.has_value()) {
                        glm::vec2 collisionNormal = glm::normalize(location.value - overlapInfo.touchPoint.value());
                        movementComp.velocity = glm::reflect(movementComp.velocity, collisionNormal) * physicalBodyComp.hitPower;
                        futureLocation = overlapInfo.touchPoint.value() + collisionNormal * (glm::length(overlapInfo.touchPoint.value() - location.value) + 0.01f);
                    }
                }
            }
        }
        location.value = futureLocation;
    }
}

template <typename T>
void HAR::Effect::applyEffect(const EffectData &effectData, entt::registry &registry, const entt::entity &entity) {
    if (registry.all_of<HAR::Component::EffectBag>(entity)) [[likely]] {
        auto& effectBag = registry.get<HAR::Component::EffectBag>(entity);
        effectBag.effectsList.push_back(std::make_shared<T>(registry, entity, effectData));
    }
    else {
        // TODO: Add logging
    }
}