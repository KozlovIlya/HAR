#pragma once

#include <entt/entity/fwd.hpp>
#include <glm/ext/vector_float2.hpp>

#include <unordered_map>
#include <functional>
#include <optional>
#include <unordered_set>
#include <vector>
#include <any>


namespace HAR::Component {

using ComponentTickFunction = std::function<void(entt::registry&, const entt::entity&, float)>;


struct Location {
    glm::vec2 value;
};

struct Movement {

    float acceleration;
    float maxSpeed;
    float inertiaFactor;
    float speedLossFactor;

    glm::vec2 velocity;
    glm::vec2 intertia;
};

struct Controlled {
    glm::vec2 movementDir;

};

struct Overlap {
    struct OverlapInfo {
        std::vector<glm::vec2>   overlapPoints;
        std::optional<glm::vec2> touchPoint;
        std::optional<float>     timeSinceTouch;
        bool                     bFullOverlap;
        
        OverlapInfo() :
            bFullOverlap(false),
            timeSinceTouch(std::nullopt),
            touchPoint(std::nullopt),
            overlapPoints() {
        }
    };
    std::unordered_map<entt::entity, OverlapInfo> overlapInfoMap;
};

struct PhysicalBody {
    float hitPower;
};

struct Player {
};

struct AI {
    ComponentTickFunction tick;
    std::any data;
};
}
