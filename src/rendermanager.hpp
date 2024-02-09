#pragma once

#include "manager.hpp"

#include <glm/ext/vector_float2.hpp>
#include <emscripten/html5.h>
#include <GLES2/gl2.h>

#include <vector>
#include <cmath>
#include <iostream>

class RenderManager : public Manager {
public:
    RenderManager(entt::registry& registry) : Manager(registry) {}
    virtual ~RenderManager() override;
public:
    virtual void tick(float deltaTime) override;
    virtual void init() override;
public:
    glm::vec2 getCanvasSize() const;
protected:
    GLuint compileShader(GLenum type, const char* source);
    GLuint createShaderProgram(const char* vertexSource, const char* fragmentSource);
    void prepareForRendering(float deltaTime);
    void renderEntity(entt::entity entity);
    void renderCircle(entt::entity entity);
    void renderPolyhedron(entt::entity entity);
    void finalizeRendering();

protected:
    void setupVerticesBuffer();
protected:
    void loadPolygonIntoBuffer(float radius, glm::vec2 location , int segments = 3);
private:
    EMSCRIPTEN_WEBGL_CONTEXT_HANDLE m_ctx;
    GLuint m_shaderProgram;
    GLuint m_VBO;
    GLuint m_positionAttrib;
};