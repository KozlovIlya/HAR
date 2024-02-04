#pragma once

#include <glm/vec2.hpp>
#include <box2d/b2_math.h>
#include <type_traits>

namespace HAR {
class Vector2 {
public:
    float x, y;

    inline Vector2() : x(0), y(0) {}

    inline Vector2(float x, float y) : x(x), y(y) {}

    template<typename T>
    inline Vector2(T vec) : x(vec.x), y(vec.y) {}

    template<typename T>
    inline T get() const {
        return b2Vec2(x, y);
    }
};
} // namespace HAR
