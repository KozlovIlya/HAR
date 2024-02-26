#pragma once

#include "../effects/effect.hpp"
#include "glm/ext/quaternion_trigonometric.hpp"

#include <glm/ext/vector_float2.hpp>

#include <entt/entity/entity.hpp>
#include <entt/entity/registry.hpp>
#include <entt/entity/fwd.hpp>

#include <list>
#include <memory>
#include <set>
#include <unordered_map>
#include <functional>
#include <optional>
#include <unordered_set>
#include <vector>
#include <any>


namespace HAR::Component {

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

struct PendingMovement {
    glm::vec2 velocity;
};

struct Controlled {
    glm::vec2 movementDir;

};

struct Overlap {
    struct OverlapInfo {
        std::vector<glm::vec2>   overlapPoints;
        std::optional<glm::vec2> touchPoint;
        std::optional<float>     timeSinceTouch;
        std::optional<glm::vec2> reflectionVector;
        bool                     bFullOverlap;
        bool                     bWasOverlaping;
        
        OverlapInfo() :
            bFullOverlap(false),
            timeSinceTouch(std::nullopt),
            touchPoint(std::nullopt),
            overlapPoints() {
        }
    };
    bool isOverlapping() const {
        return !overlapInfoMap.empty();
    }
    std::unordered_map<entt::entity, OverlapInfo> overlapInfoMap;

    std::optional<std::function<void(entt::registry&, const entt::entity&)>> OnOverlapBegin;
    std::optional<std::function<void(entt::registry&, const entt::entity&)>> OnOverlapEnd;

    std::optional<std::function<void(entt::registry&, const entt::entity&, float)>> OnOverlapUpdate;

    std::any data;
};

struct PhysicalBody {
    float hitPower;
};

struct EffectBag {
    std::list<std::shared_ptr<Effect>> effectsList;
};

struct Player {
    int score;
};


struct AI {
    std::function<void(entt::registry&, const entt::entity&, float)> tick;
    std::any data;
};

struct Collectible {
    bool bCollected;
};

}
