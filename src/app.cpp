#include "app.hpp"

#include "components/common.hpp"
#include "components/render.hpp"
#include "components/geometry.hpp"
#include "components/gameplay.hpp"
#include "rendermanager.hpp"

#include "vector2.hpp"

#include <entt/entt.hpp>

#include <emscripten/emscripten.h>
#include <emscripten/html5.h>

void Application::run() {
    m_data.lastTime = 0.f;
    m_data.managers.emplace_back(std::make_unique<RenderManager>(m_data.registry));
    m_data.managers[0]->init();
    
    auto entityCircle = m_data.registry.create();
    m_data.registry.emplace<HAR::Component::Renderable>(entityCircle, true);
    m_data.registry.emplace<HAR::Component::Circle>(entityCircle, 0.2f);
    m_data.registry.emplace<HAR::Component::Location>(entityCircle, HAR::Math::Vector2(0.0f, 0.0f));

    auto entityPolyhedron = m_data.registry.create();
    m_data.registry.emplace<HAR::Component::Renderable>(entityPolyhedron, true);
    m_data.registry.emplace<HAR::Component::Polyhedron>(entityPolyhedron, std::vector<HAR::Math::Vector2>{
        HAR::Math::Vector2(-0.3f, -0.2f),
        HAR::Math::Vector2(-0.2f, 0.3f),
        HAR::Math::Vector2(0.1f, 0.4f),
        HAR::Math::Vector2(0.4f, 0.1f),
        HAR::Math::Vector2(0.3f, -0.3f),
        HAR::Math::Vector2(0.0f, -0.4f)
    });

    m_data.registry.emplace<HAR::Component::Location>(entityPolyhedron, HAR::Math::Vector2(-0.7f, 0.3f));

    emscripten_set_main_loop_arg(Application::mainLoop, &m_data, 0, 1);
}

double Application::getDevicePixelRatio() const {
    return EM_ASM_DOUBLE({
        return window.devicePixelRatio || 1.0;
    });
}

HAR::Math::Vector2 Application::getScaleFactor() const {
    double x,y;
    emscripten_get_element_css_size("#canvas", &x, &y);
    HAR::Math::Vector2 screenSize(x, y);
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
