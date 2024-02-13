#pragma once

#include <entt/entity/fwd.hpp>
#include <glm/ext/vector_float2.hpp>

#include <unordered_map>
#include <functional>
#include <optional>
#include <unordered_set>
#include <vector>


namespace HAR::Component {

struct Location {
    glm::vec2 value;
};

struct Movement {
    float acceleration;
    float maxSpeed;
    float inertiaFactor;
    // bool  bDynamic;
    glm::vec2 velocity;
    glm::vec2 direction;
};

struct Mass {
    float value;
};

struct Overlap {
    struct OverlapCheckResult {
        std::vector<glm::vec2> overlapPoints;
        std::optional<glm::vec2> touchPoint;
        std::optional<float> timeSinceTouch;
        bool bFullOverlap;
    };

    std::unordered_map<entt::entity, OverlapCheckResult> overlapCheckResultMap;
};

struct PhysicalBody {
    float hitEnergyMultiplier;
};

}
