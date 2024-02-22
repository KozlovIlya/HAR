#include "app.hpp"

#include "components/render.hpp"
#include "components/geometry.hpp"
#include "components/gameplay.hpp"

#include "inputmanager.hpp"
#include "rendermanager.hpp"
#include "movementmanager.hpp"
#include "overlapmanager.hpp"
#include "physicsmanager.hpp"
#include "aimanager.hpp"
#include "gamemanager.hpp"

#include "utils.hpp"

#include <entt/entt.hpp>
#include <entt/entity/fwd.hpp>
#include <glm/glm.hpp>
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
    m_data.registry.emplace<HAR::Component::Overlap>(entityCircle, std::unordered_map<entt::entity, HAR::Component::Overlap::OverlapInfo>());
    m_data.registry.emplace<HAR::Component::PhysicalBody>(entityCircle, 1.f);
    m_data.registry.emplace<HAR::Component::Controlled>(entityCircle, glm::vec2(0.f, 0.f));
    m_data.registry.emplace<HAR::Component::Player>(entityCircle);
    m_data.registry.emplace<HAR::Component::Movement>(entityCircle,
        .0001f,
        .002f,
        .01f,
        .01f,
        glm::vec2(0.f, 0.f),
        glm::vec2(0.f, 0.f)
    );

    auto entityEnemy = m_data.registry.create();
    m_data.registry.emplace<HAR::Component::Renderable>(entityEnemy, true);
    m_data.registry.emplace<HAR::Component::Circle>(entityEnemy, 0.07f);
    m_data.registry.emplace<HAR::Component::Color>(entityEnemy, glm::vec4(0.f, 1.f, 0.f, 1.f));
    m_data.registry.emplace<HAR::Component::Location>(entityEnemy, glm::vec2(0.3f, 0.3f));
    m_data.registry.emplace<HAR::Component::Overlap>(entityEnemy, std::unordered_map<entt::entity, HAR::Component::Overlap::OverlapInfo>());
    m_data.registry.emplace<HAR::Component::PhysicalBody>(entityEnemy, 1.f);
    m_data.registry.emplace<HAR::Component::Controlled>(entityEnemy, glm::vec2(0.f, 0.f));
    m_data.registry.emplace<HAR::Component::Movement>(entityEnemy,
    .00002f,
    .001f,
    .00001f,
    .00001f,
    glm::vec2(0.f, 0.f),
    glm::vec2(0.f, 0.f)
    );
    m_data.registry.emplace<HAR::Component::AI>(entityEnemy, [&](entt::registry& registry, const entt::entity& entity, float deltaTime) {
        if (!!!registry.all_of<HAR::Component::Location, HAR::Component::AI, HAR::Component::Controlled>(entity)) [[unlikely]] {
            return;
        }
        auto& aiLocationComp = registry.get<HAR::Component::Location>(entity);
        auto& aiControlledComp = registry.get<HAR::Component::Controlled>(entity);
        auto view = registry.view<HAR::Component::Location, HAR::Component::Player>();
        for (auto& playerEntity : view) {
            auto& playerlocationComp = registry.get<HAR::Component::Location>(playerEntity);
            auto direction = glm::normalize(playerlocationComp.value - aiLocationComp.value);
            // aiControlledComp.movementDir = direction;
            return;
        }
    });




    auto entityPolyhedron = m_data.registry.create();
    m_data.registry.emplace<HAR::Component::Renderable>(entityPolyhedron, true);
    m_data.registry.emplace<HAR::Component::Color>(entityPolyhedron, glm::vec4(1.f, 0.f, 0.f, 1.f));
    m_data.registry.emplace<HAR::Component::Overlap>(entityPolyhedron, std::unordered_map<entt::entity, HAR::Component::Overlap::OverlapInfo>());
    m_data.registry.emplace<HAR::Component::Location>(entityPolyhedron, glm::vec2(-0.7f, 0.3f));
    m_data.registry.emplace<HAR::Component::PhysicalBody>(entityPolyhedron, 10.f);
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

    m_data.managers.emplace_back(std::make_unique<InputManager>(m_data.registry))->init();
    m_data.managers.emplace_back(std::make_unique<AIManager>(m_data.registry))->init();
    m_data.managers.emplace_back(std::make_unique<MovementManager>(m_data.registry))->init();
    m_data.managers.emplace_back(std::make_unique<OverlapManager>(m_data.registry))->init();
    m_data.managers.emplace_back(std::make_unique<PhysicsManager>(m_data.registry))->init();
    m_data.managers.emplace_back(std::make_unique<GameManager>(m_data.registry))->init();
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
