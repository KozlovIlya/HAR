#pragma once

#include "../vector2.hpp"

namespace HAR::Component {

struct Polygon {
    float radius;
    int vertexCount;
};

struct Circle {
    float radius;
};

struct Quaterion {
    float x, y, z, w;
};

}