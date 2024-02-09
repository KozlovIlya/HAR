#pragma once

#include <functional>
#include <glm/ext/vector_float2.hpp>


namespace HAR::Component {

struct Location {
    glm::vec2 value;
};

struct Movement {
    glm::vec2 velocity;
    glm::vec2 direction;
    float acceleration;
    float maxSpeed;
    float inertiaFactor;
};



// struct Body {
//     b2BodyDef bodyDef;
//     b2FixtureDef fixtureDef;
//     b2MassData massData;
//     b2Body* body;
//     b2Fixture* fixture;
// };

}
