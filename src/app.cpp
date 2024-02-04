#include "app.hpp"
#include "rendermanager.hpp"
#include "vector2.hpp"

#include <emscripten/emscripten.h>
#include <emscripten/html5.h>

void Application::run() {
    m_data.lastTime = 0.f;
    m_data.managers.emplace_back(std::make_unique<RenderManager>(m_data.registry));
    m_data.managers[0]->init();
    emscripten_set_main_loop_arg(Application::mainLoop, &m_data, 0, 1);
}

double Application::getDevicePixelRatio() const {
    return EM_ASM_DOUBLE({
        return window.devicePixelRatio || 1.0;
    });
}

HAR::Vector2 Application::getScaleFactor() const {
    double x,y;
    emscripten_get_element_css_size("#canvas", &x, &y);
    HAR::Vector2 screenSize(x, y);
    return screenSize;
}

void Application::mainLoop(void* arg) {
    ApplicationData& data = *static_cast<ApplicationData*>(arg);
    float currentTime = emscripten_get_now();
    float deltaTime = currentTime - data.lastTime;
    data.lastTime = currentTime;

    for (auto& manager : data.managers) {
        manager->tick(deltaTime);
    }
}