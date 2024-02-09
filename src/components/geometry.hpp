#pragma once

#include <glm/ext/vector_float2.hpp>
#include <vector>

namespace HAR::Component {

struct Circle {
    float radius;
};

struct Polyhedron {
    std::vector<glm::vec2> vertices;
};

}