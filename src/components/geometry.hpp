#pragma once

#include "../vector2.hpp"

#include <vector>

namespace HAR::Component {

struct Circle {
    float radius;
};

struct Polyhedron {
    std::vector<HAR::Math::Vector2> vertices;
};

}