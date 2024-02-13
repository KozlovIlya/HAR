#include "app.hpp"

#include "components/render.hpp"
#include "components/geometry.hpp"
#include "components/gameplay.hpp"

#include "inputmanager.hpp"
#include "rendermanager.hpp"
#include "overlapmanager.hpp"
#include "movementmanager.hpp"
#include "utils.hpp"

#include <entt/entt.hpp>

#include <emscripten/emscripten.h>
#include <emscripten/html5.h>
#include <functional>
#include <iostream>
#include <unordered_map>

void Application::run() {
    m_data.lastTime = 0.f;
    m_data.registry.clear();

    auto entityCircle = m_data.registry.create();
    m_data.registry.emplace<HAR::Component::Renderable>(entityCircle, true);
    m_data.registry.emplace<HAR::Component::Color>(entityCircle, glm::vec4(0.f, 0.f, 0.f, 1.f));
    m_data.registry.emplace<HAR::Component::Circle>(entityCircle, 0.07f);
    m_data.registry.emplace<HAR::Component::Location>(entityCircle, glm::vec2(0.f, 0.f));
    m_data.registry.emplace<HAR::Component::Overlap>(entityCircle, std::unordered_map<entt::entity, HAR::Component::Overlap::OverlapCheckResult>());
    m_data.registry.emplace<HAR::Component::Movement>(entityCircle,
        .0001f,
        .001f,
        0.9988f,
        glm::vec2(0.f, 0.f),
        glm::vec2(0.f, 0.f)
    );

    auto entityPolyhedron = m_data.registry.create();
    m_data.registry.emplace<HAR::Component::Renderable>(entityPolyhedron, true);
    m_data.registry.emplace<HAR::Component::Color>(entityPolyhedron, glm::vec4(1.f, 0.f, 0.f, 1.f));
    m_data.registry.emplace<HAR::Component::Overlap>(entityPolyhedron, std::unordered_map<entt::entity, HAR::Component::Overlap::OverlapCheckResult>());
    m_data.registry.emplace<HAR::Component::Location>(entityPolyhedron, glm::vec2(-0.7f, 0.3f));
    m_data.registry.emplace<HAR::Component::Polyhedron>(entityPolyhedron, std::vector<glm::vec2>{
        glm::vec2(-0.3f, -0.2f),
        glm::vec2(-0.2f, 0.3f),
        glm::vec2(0.1f, 0.4f),
        glm::vec2(0.4f, 0.1f),
        glm::vec2(0.3f, -0.3f),
        glm::vec2(0.0f, -0.4f)
    });

    auto entityDebugPolyhedron = m_data.registry.create();
    m_data.registry.emplace<HAR::Component::Renderable>(entityDebugPolyhedron, true);
    m_data.registry.emplace<HAR::Component::Color>(entityDebugPolyhedron, glm::vec4(0.f, 1.f, 0.f, 1.f));
    m_data.registry.emplace<HAR::Component::Location>(entityDebugPolyhedron, glm::vec2(-0.7f, 0.3f));
    m_data.registry.emplace<HAR::Component::Polyhedron>(entityDebugPolyhedron, HAR::Math::InflatePolygon(std::vector<glm::vec2>{
        glm::vec2(-0.3f, -0.2f),
        glm::vec2(-0.2f, 0.3f),
        glm::vec2(0.1f, 0.4f),
        glm::vec2(0.4f, 0.1f),
        glm::vec2(0.3f, -0.3f),
        glm::vec2(0.0f, -0.4f)
    }, 0.07f));

    // auto entityCircle2 = m_data.registry.create();
    // m_data.registry.emplace<HAR::Component::Renderable>(entityCircle2, true);
    // m_data.registry.emplace<HAR::Component::Circle>(entityCircle2, 0.32f);
    // m_data.registry.emplace<HAR::Component::Color>(entityCircle2, glm::vec4(1.f, 0.f, 0.f, 1.f));
    // m_data.registry.emplace<HAR::Component::Location>(entityCircle2, glm::vec2(-0.7f, 0.3f));
    // m_data.registry.emplace<HAR::Component::Overlap>(entityCircle2, std::unordered_map<entt::entity, HAR::Component::Overlap::OverlapCheckResult>());
    



    m_data.managers.emplace_back(std::make_unique<InputManager>(m_data.registry))->init();
    m_data.managers.emplace_back(std::make_unique<OverlapManager>(m_data.registry))->init();
    // m_data.managers.emplace_back(std::make_unique<MovementManager>(m_data.registry))->init();
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
