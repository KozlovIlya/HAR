#include "rendermanager.hpp"

#include <emscripten/emscripten.h>
#include <emscripten/html5.h>
#include <GLES2/gl2.h>

void RenderManager::init() {
    EmscriptenWebGLContextAttributes attrs;
    emscripten_webgl_init_context_attributes(&attrs);
    attrs.alpha = attrs.depth = attrs.stencil = attrs.antialias = attrs.preserveDrawingBuffer = attrs.failIfMajorPerformanceCaveat = 0;
    attrs.enableExtensionsByDefault = 1;
    attrs.premultipliedAlpha = 0;
    attrs.majorVersion = 1;
    attrs.minorVersion = 0;

    EMSCRIPTEN_WEBGL_CONTEXT_HANDLE ctx = emscripten_webgl_create_context("#canvas", &attrs);
    emscripten_webgl_make_context_current(ctx);

    glClearColor(0.5f, 0.0f, 0.5f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}

void RenderManager::tick(float deltaTime) {
    glClearColor(0.5f, 0.0f, 0.5f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}
