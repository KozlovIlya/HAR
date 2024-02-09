#include "app.hpp"

#include "components/render.hpp"
#include "components/geometry.hpp"
#include "components/gameplay.hpp"

#include "inputmanager.hpp"
#include "rendermanager.hpp"
#include "movementmanager.hpp"


#include <entt/entt.hpp>
#include <box2d/box2d.h>

#include <emscripten/emscripten.h>
#include <emscripten/html5.h>
#include <iostream>

void Application::run() {
    m_data.lastTime = 0.f;
    m_data.registry.clear();

    auto entityCircle = m_data.registry.create();
    m_data.registry.emplace<HAR::Component::Renderable>(entityCircle, true);
    m_data.registry.emplace<HAR::Component::Circle>(entityCircle, 0.05f);
    m_data.registry.emplace<HAR::Component::Location>(entityCircle, glm::vec2(0.f, 0.f));
    m_data.registry.emplace<HAR::Component::Movement>(entityCircle,
        glm::vec2(0.f, 0.f),
        glm::vec2(0.f, 0.f),
        .00001f,
        .01f,
        .99f
    );

    auto entityPolyhedron = m_data.registry.create();
    m_data.registry.emplace<HAR::Component::Renderable>(entityPolyhedron, true);
    m_data.registry.emplace<HAR::Component::Polyhedron>(entityPolyhedron, std::vector<glm::vec2>{
        glm::vec2(-0.3f, -0.2f),
        glm::vec2(-0.2f, 0.3f),
        glm::vec2(0.1f, 0.4f),
        glm::vec2(0.4f, 0.1f),
        glm::vec2(0.3f, -0.3f),
        glm::vec2(0.0f, -0.4f)
    });
    m_data.registry.emplace<HAR::Component::Location>(entityPolyhedron, glm::vec2(-0.7f, 0.3f));
    
    m_data.managers.emplace_back(std::make_unique<InputManager>(m_data.registry))->init();
    m_data.managers.emplace_back(std::make_unique<MovementManager>(m_data.registry))->init();
    m_data.managers.emplace_back(std::make_unique<RenderManager>(m_data.registry))->init();
    
    emscripten_set_main_loop_arg(Application::mainLoop, &m_data, 0, 1);
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
