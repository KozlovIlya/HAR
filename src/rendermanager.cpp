#include "rendermanager.hpp"

#include "components/render.hpp"
#include "components/gameplay.hpp"
#include "components/geometry.hpp"

#include "shaders.hpp"

#include <emscripten/emscripten.h>
#include <emscripten/html5.h>
#include <GLES2/gl2.h>
#include <entt/entt.hpp>

#include <vector>
#include <cmath>
#include <iostream>

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
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    auto view = m_registry.view<HAR::Component::Renderable, HAR::Component::Polygon>();
    m_positionAttrib = glGetAttribLocation(m_shaderProgram, "position");
    glEnableVertexAttribArray(m_positionAttrib);
    glVertexAttribPointer(m_positionAttrib, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
    for (auto entity : view) {
        auto& polygon = view.get<HAR::Component::Polygon>(entity);
        loadPolygonIntoBuffer(polygon.radius, HAR::Math::Vector2(0.f, 0.f), polygon.vertexCount);
        glDrawArrays(GL_TRIANGLE_FAN, 0, polygon.vertexCount);
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
    // Initially, we don't load data into the buffer
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

    GLint positionAttribLocation = glGetAttribLocation(m_shaderProgram, "position");
    glEnableVertexAttribArray(positionAttribLocation);
    glVertexAttribPointer(positionAttribLocation, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
}














































































































































































































































































































































































