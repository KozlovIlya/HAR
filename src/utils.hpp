#pragma once

#include "components/gameplay.hpp"
#include "effects/effect.hpp"

#include <glm/ext/vector_float2.hpp>
#include <glm/vec2.hpp>

#include <entt/entity/registry.hpp>

#include <optional>
#include <vector>




namespace HAR {
    namespace Math {
glm::vec2 const DEFAULT_LOCATION = {0, 0};

const float SQUARE[] = {
-1.0f,  1.0f,
-1.0f, -1.0f,
1.0f, -1.0f,
1.0f,  1.0f
};

std::vector<glm::vec2> InflatePolygon(const std::vector<glm::vec2>& vertices, float radius);
std::optional<glm::vec2> getIntersection(
    const glm::vec2& A, const glm::vec2& B,
    const glm::vec2& C, const glm::vec2& D);
    }

    namespace AI {
void chasePlayer(entt::registry& registry, const entt::entity& entity, float deltaTime);
    }

    namespace Collision {
void pushCollidedComponents(entt::registry& registry, const entt::entity& entity);
    }

    namespace Effect {
template <typename T>
void applyEffect(const EffectData& effectData, entt::registry& registry, const entt::entity& entity);
    }

}
