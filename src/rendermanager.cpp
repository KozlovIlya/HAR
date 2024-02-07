#include "rendermanager.hpp"

#include "components/render.hpp"
#include "components/gameplay.hpp"
#include "components/geometry.hpp"

#include "shaders.hpp"
#include "constants.hpp"

#include <emscripten/emscripten.h>
#include <emscripten/html5.h>
#include <GLES2/gl2.h>
#include <entt/entt.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>
#include <cmath>
#include <iostream>


RenderManager::~RenderManager() {
    glDeleteProgram(m_shaderProgram);
    glDeleteBuffers(1, &m_VBO);
    emscripten_webgl_destroy_context(m_ctx);
}

HAR::Math::Vector2 RenderManager::getCanvasSize() const {
    double width, height;
    EM_ASM({
        var canvas = Module['canvas'];
        setValue($0, canvas.width, 'double');
        setValue($1, canvas.height, 'double');
    }, &width, &height);
    return HAR::Math::Vector2(static_cast<float>(width), static_cast<float>(height));
}

void RenderManager::init() {
    EmscriptenWebGLContextAttributes attrs;
    emscripten_webgl_init_context_attributes(&attrs);
    attrs.alpha = attrs.depth = attrs.stencil = attrs.antialias = attrs.preserveDrawingBuffer = attrs.failIfMajorPerformanceCaveat = 0;
    attrs.enableExtensionsByDefault = 1;
    attrs.premultipliedAlpha = 0;
    attrs.majorVersion = 1;
    attrs.minorVersion = 0;

    m_ctx = emscripten_webgl_create_context("#canvas", &attrs);
    emscripten_webgl_make_context_current(m_ctx);

    m_shaderProgram = createShaderProgram(HAR::Shader::vertexShaderSource, HAR::Shader::fragmentShaderSource);

    setupVerticesBuffer();
}

void RenderManager::tick(float deltaTime) {
    glUseProgram(m_shaderProgram);
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    GLint resolutionLoc = glGetUniformLocation(m_shaderProgram, "u_resolution");
    glUniform2fv(resolutionLoc, 1, glm::value_ptr(getCanvasSize().get<glm::vec2>()));

    auto view = m_registry.view<HAR::Component::Renderable, HAR::Component::Location>();
    for (auto entity: view) {
        auto locationVec = view.get<HAR::Component::Location>(view.front()).location.get<glm::vec2>();
        GLint locationLoc = glGetUniformLocation(m_shaderProgram, "u_location");
        if (m_registry.all_of<HAR::Component::Circle>(entity)) {
            auto& circle = m_registry.get<HAR::Component::Circle>(entity);
            GLint radiusLoc = glGetUniformLocation(m_shaderProgram, "u_radius");
            glUniform1f(radiusLoc, circle.radius);
            glBufferData(GL_ARRAY_BUFFER, sizeof(HAR::Constants::SQUARE), HAR::Constants::SQUARE, GL_DYNAMIC_DRAW);
            glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
            glEnableVertexAttribArray(0);
            glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
        } else if (m_registry.all_of<HAR::Component::Polyhedron>(entity)) {
            auto& polyhedron = m_registry.get<HAR::Component::Polyhedron>(entity);
            std::vector<float> vertices;
            for (auto& vertex: polyhedron.vertices) {
                vertices.push_back(vertex.x);
                vertices.push_back(vertex.y);
            }
            GLint radiusLoc = glGetUniformLocation(m_shaderProgram, "u_radius");
            glUniform1f(radiusLoc, 0.0f);
            glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_DYNAMIC_DRAW);
            glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
            glEnableVertexAttribArray(0);
            glDrawArrays(GL_TRIANGLE_FAN, 0, polyhedron.vertices.size());
        }
    }
    emscripten_webgl_commit_frame();
}

GLuint RenderManager::compileShader(GLenum type, const char* source) {
    GLint success;
    GLchar infoLog[512];

    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, nullptr);
    glCompileShader(shader);
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    return shader;
}

GLuint RenderManager::createShaderProgram(const char* vertexSource, const char* fragmentSource) {
    GLint success;
    GLchar infoLog[512];
    GLuint vertexShader = compileShader(GL_VERTEX_SHADER, vertexSource);
    GLuint fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentSource);

    GLuint program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);

    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(program, 512, NULL, infoLog);
        std::cerr << "ERROR::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return program;
}

void RenderManager::setupVerticesBuffer() {
    glGenBuffers(1, &m_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
}

void RenderManager::loadPolygonIntoBuffer(float radius, HAR::Math::Vector2 location, int segments) {
    if (segments < 3) {
        segments = 3;
    }
    std::vector<GLfloat> polygonVertices;

    for (int i = 0; i < segments; ++i) {
        float angle = 2.0f * M_PI * static_cast<float>(i) / static_cast<float>(segments);
        float x = location.x + cos(angle) * radius;
        float y = location.y + sin(angle) * radius;
        polygonVertices.push_back(x);
        polygonVertices.push_back(y);
    }

    polygonVertices.push_back(polygonVertices[0]);
    polygonVertices.push_back(polygonVertices[1]);

    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, polygonVertices.size() * sizeof(GLfloat), polygonVertices.data(), GL_DYNAMIC_DRAW);

    GLint positionAttribLocation = glGetAttribLocation(m_shaderProgram, "a_position");
    glEnableVertexAttribArray(positionAttribLocation);
    glVertexAttribPointer(positionAttribLocation, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
}
