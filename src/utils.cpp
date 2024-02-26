#include "utils.hpp"

#include <any>
#include <glm/glm.hpp>
#include <iostream>

#include "components/gameplay.hpp"
#include "components/geometry.hpp"
#include "components/render.hpp"

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
    glm::vec2 r = fA - sA;
    glm::vec2 s = fB - sB;
    glm::vec2 qp = sB - sA;
    float rxs = glm::cross(glm::vec3(r, 0.0f), glm::vec3(s, 0.0f)).z;
    float qpxr = glm::cross(glm::vec3(qp, 0.0f), glm::vec3(r, 0.0f)).z;

    if (abs(rxs) < 1e-8) {
        return {};
    }

    float t = glm::cross(glm::vec3(qp, 0.0f), glm::vec3(s, 0.0f)).z / rxs;
    float u = qpxr / rxs;
    if (t >= 0.0f && t <= 1.0f && u >= 0.0f && u <= 1.0f) {
        return sA + t * r;
    }

    return {};
}

std::mt19937 rng(std::random_device{}());

float HAR::Math::getRandomFloat(float min, float max) {
    std::uniform_real_distribution<float> dist(min, max);
    return dist(rng);
}

void HAR::AI::chasePlayer(entt::registry& registry, const entt::entity& entity, float deltaTime) {
            if (!!!registry.all_of<HAR::Component::Location, HAR::Component::AI, HAR::Component::Controlled>(entity)) [[unlikely]] {
            return;
        }
        auto& enemyAiComp = registry.get<HAR::Component::AI>(entity);
        auto& enemyLocationComp = registry.get<HAR::Component::Location>(entity);
        auto& enemyControlledComp = registry.get<HAR::Component::Controlled>(entity);

        auto enemyCanSeePlayer = true;       
        
        auto& targetLocation = std::any_cast<glm::vec2&>(enemyAiComp.data);

        auto view = registry.view<HAR::Component::Location, HAR::Component::Player>();
        for (auto& playerEntity : view) {
            auto& playerlocationComp = registry.get<HAR::Component::Location>(playerEntity);
            auto direction = glm::normalize(playerlocationComp.value - enemyLocationComp.value);
            auto visionBarrierView = registry.view<HAR::Component::Renderable, HAR::Component::Location>();
            for (auto& visionBarrierEntity : visionBarrierView) {
                auto visionBarrierLocationComp = registry.get<HAR::Component::Location>(visionBarrierEntity);
                if (registry.any_of<HAR::Component::Player, HAR::Component::AI>(visionBarrierEntity)) {
                    continue;
                }
                if (registry.all_of<HAR::Component::Circle>(visionBarrierEntity)) {
                    // TODO: Check sight overlaps cirlce
                    // refactor overlap manager to use a function that check if line segment intersects with circle
                    // auto circle = registry.get<HAR::Component::Circle>(visionBarrierEntity).radius = 0.1f;
                }
                    
                if (registry.all_of<HAR::Component::Polyhedron>(visionBarrierEntity)) {
                    auto& polyhedron = registry.get<HAR::Component::Polyhedron>(visionBarrierEntity);
                    for (size_t i = 0; i < polyhedron.vertices.size(); ++i) {
                        glm::vec2 start = polyhedron.vertices[i] + visionBarrierLocationComp.value;
                        glm::vec2 end = polyhedron.vertices[(i + 1) % polyhedron.vertices.size()] + visionBarrierLocationComp.value;
                        if (HAR::Math::getIntersection(enemyLocationComp.value, playerlocationComp.value, start, end).has_value()) {
                            enemyCanSeePlayer = false;
                        }
                    }

                    if (!!!enemyCanSeePlayer) {
                        break;
                    }
                }
            }
            
            if (registry.all_of<HAR::Component::Renderable>(entity)) {
                auto& enemyRenderableComp = registry.get<HAR::Component::Renderable>(entity);
                enemyRenderableComp.visible = enemyCanSeePlayer;
            }

            if (enemyCanSeePlayer) {
                targetLocation = playerlocationComp.value;
                break;
            }
        }
        enemyControlledComp.movementDir = glm::normalize(targetLocation - enemyLocationComp.value);
}

void HAR::Collision::pushCollidedComponents(entt::registry& registry, const entt::entity& entity) {
    if (registry.all_of<HAR::Component::PhysicalBody, HAR::Component::Overlap>(entity)) {
        auto& overlapComp = registry.get<HAR::Component::Overlap>(entity);
        auto& physicalBodyComp = registry.get<HAR::Component::PhysicalBody>(entity);
        if (!overlapComp.overlapInfoMap.empty()) {
            for (auto& [overlapsEntity, overlapInfo] : overlapComp.overlapInfoMap) {
                if (registry.all_of<HAR::Component::PhysicalBody, HAR::Component::Location>(overlapsEntity) && overlapInfo.touchPoint.has_value()) {
                    auto& location = registry.get<HAR::Component::Location>(overlapsEntity);
                    auto futureLocation = location.value;
                    if (registry.all_of<HAR::Component::Movement>(overlapsEntity)) {
                        auto& movementComp = registry.get<HAR::Component::Movement>(overlapsEntity);
                        glm::vec2 collisionNormal = glm::normalize(location.value - overlapInfo.touchPoint.value());
                        glm::vec2 reflectionVector = (overlapInfo.reflectionVector ? *overlapInfo.reflectionVector : glm::reflect(movementComp.velocity, collisionNormal)) * physicalBodyComp.hitPower;
                        glm::vec2 futureVelocity = reflectionVector;

                        if (overlapInfo.timeSinceTouch.has_value()) {
                            float time = overlapInfo.timeSinceTouch.value();
                            futureLocation += reflectionVector * time;
                            futureVelocity = reflectionVector;
                        }

                        registry.emplace_or_replace<HAR::Component::PendingMovement>(overlapsEntity, HAR::Component::PendingMovement{ futureVelocity });
                    }
                }
            }
        }
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