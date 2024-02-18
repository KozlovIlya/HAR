#pragma once

#include "manager.hpp"
#include "components/gameplay.hpp"

#include <entt/entity/entity.hpp>
#include <glm/ext/vector_float2.hpp>

class OverlapManager : public Manager {
public:
    OverlapManager(entt::registry& registry) : Manager(registry) {}
    virtual ~OverlapManager() {}
public:
    void tick(float deltaTime) override;
    virtual void init() override;
protected:
    HAR::Component::Overlap::OverlapInfo findOverlap(const entt::entity& a, const entt::entity& b,
    const glm::vec2& currentlocationA, const glm::vec2& currentlocationB,
    const glm::vec2& potentialLocationA, const glm::vec2& potentialLocationB,
    float deltaTime);

protected:
    HAR::Component::Overlap::OverlapInfo findOverlap_Circle_x_Circle(const entt::entity& a, const entt::entity& b,
        const glm::vec2& currentLocationA, const glm::vec2& currentLocationB,
        const glm::vec2& potentialLocationA, const glm::vec2& potentialLocationB,
        float deltaTime);
    HAR::Component::Overlap::OverlapInfo findOverlap_Circle_x_Polyhedron(
        const entt::entity& a, const entt::entity& b,
        const glm::vec2& currentLocationA, const glm::vec2& currentLocationB,
        const glm::vec2& potentialLocationA, const glm::vec2& potentialLocationB,
        float deltaTime);
    HAR::Component::Overlap::OverlapInfo findOverlap_Polyhedron_x_Polyhedron(
        const entt::entity& a, const entt::entity& b,
        const glm::vec2& currentLocationA, const glm::vec2& currentLocationB,
        const glm::vec2& potentialLocationA, const glm::vec2& potentialLocationB,
        float deltaTime);
};