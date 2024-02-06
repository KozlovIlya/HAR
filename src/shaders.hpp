#pragma once

namespace HAR::Shader {

// const char* vertexShaderSource = R"glsl(
//     precision mediump float;

//     attribute vec2 position;
//     // attribute vec4 vertexColor;

//     // uniform vec2 u_location;
//     // uniform float u_scale;

//     // varying vec4 fragColor;

//     void main() {
//         // vec2 transformedPosition = (position * u_scale) + u_location;
//         // gl_Position = vec4(transformedPosition, 0.0, 1.0);
//         // fragColor = vertexColor;
//         gl_Position = vec4(position, 0.0, 1.0);
//     }
// )glsl";

// const char* fragmentShaderSource = R"glsl(
//     // precision mediump float;
//     // varying vec4 fragColor;

//     void main() {
//         // gl_FragColor = fragColor;
//         gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);
//     }
//     )glsl";
// }



const char* vertexShaderSource = R"(
    attribute vec2 position;
    void main() {
        gl_Position = vec4(position, 0.0, 1.0);
    }
)";

const char* fragmentShaderSource = R"(
    precision mediump float;
    void main() {
        gl_FragColor = vec4(0.0, 1.0, 0.0, 1.0); // Зелёный цвет
    }
)";

} // namespace HAR::Shader