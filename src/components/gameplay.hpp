#pragma once

#include "../vector2.hpp"
#include "box2d/b2_world.h"

#include <box2d/box2d.h>

namespace HAR::Component {

struct Location {
    HAR::Math::Vector2 location;
};

struct DesiredMovementDirection {
    HAR::Math::Vector2 direction;
};

struct Velocity {
    HAR::Math::Vector2 velocity;
};

struct Acceleration {
    HAR::Math::Vector2 acceleration;
};

struct Mass {
    float mass;
};

struct Force {
    HAR::Math::Vector2 force;
};

struct PhysicsBody {
    b2Body* body;
};

struct World {
    b2World* world;
};

}