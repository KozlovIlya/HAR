#pragma once

#include <glm/vec2.hpp>
#include <box2d/b2_math.h>
#include <type_traits>

namespace HAR::Math {
class Vector2 {
public:
    float x, y;

    inline Vector2() : x(0.f), y(0.f) {}

    inline Vector2(float x, float y) : x(x), y(y) {}

    template<typename T>
    inline T get() const {
        return T(x, y);
    }
};
} // namespace HAR
