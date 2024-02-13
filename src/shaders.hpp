#pragma once

namespace HAR::Shader {

// Vertex Shader
const char* vertexShaderSource = R"glsl(
precision mediump float;

attribute vec2 a_position;

uniform vec2 u_resolution;
uniform vec2 u_location;
uniform vec4 u_color;

// CIRCLE
uniform float u_radius;

void main() {
    gl_Position = vec4(u_location + a_position, 0.0, 1.0);
}
)glsl";

// Fragment Shader
const char* fragmentShaderSource = R"glsl(
precision mediump float;

uniform vec2 u_resolution;
uniform vec2 u_location;
uniform vec4 u_color;

// CIRCLE
uniform float u_radius;

void main() {
    vec2 normalizedCoords = gl_FragCoord.xy / u_resolution.xy;
    vec2 coords = normalizedCoords * 2.0 - 1.0;

    // Вычисляем расстояние от текущего пикселя до центра круга
    float dist = length(u_location - coords);

    if (u_radius <= 0.0) {
        gl_FragColor = u_color;
    } else {
        if (dist < u_radius) {
            gl_FragColor = u_color;
        } else {
            discard;
        }
    }
}
)glsl";

} // namespace HAR::Shader
