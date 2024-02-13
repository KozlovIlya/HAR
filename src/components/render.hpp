#pragma once

#include <glm/vec4.hpp>

namespace HAR::Component {

struct Renderable {
    bool visible;  
};

struct Color {
    glm::vec4 value;
};


}