#pragma once

#include "manager.hpp"
#include "vector2.hpp"

#include <emscripten.h>
#include <emscripten/html5.h>
#include <GLES3/gl3.h>

class RenderManager : public Manager {
public:
    RenderManager(entt::registry& registry) : Manager(registry) {}
    virtual void tick(float deltaTime) override;
    virtual void init() override;
    virtual ~RenderManager() override;
public:
    HAR::Math::Vector2 getCanvasSize() const;
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
    void loadPolygonIntoBuffer(float radius, HAR::Math::Vector2 location , int segments = 3);
private:
    EMSCRIPTEN_WEBGL_CONTEXT_HANDLE m_ctx;
    GLuint m_shaderProgram;
    GLuint m_VBO;
    GLuint m_positionAttrib;
};