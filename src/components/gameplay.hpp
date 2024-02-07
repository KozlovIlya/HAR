#pragma once

#include "../vector2.hpp"

namespace HAR::Component {

struct Location {
    HAR::Math::Vector2 location;
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


}