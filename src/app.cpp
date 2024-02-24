#include "app.hpp"

#include "components/render.hpp"
#include "components/geometry.hpp"
#include "components/gameplay.hpp"

#include "glm/geometric.hpp"
#include "inputmanager.hpp"
#include "rendermanager.hpp"
#include "movementmanager.hpp"
#include "overlapmanager.hpp"
#include "physicsmanager.hpp"
#include "aimanager.hpp"
#include "gamemanager.hpp"

#include "utils.hpp"

#include <any>
#include <entt/entt.hpp>
#include <entt/entity/fwd.hpp>
#include <glm/glm.hpp>
#include <emscripten/emscripten.h>
#include <emscripten/html5.h>
#include <functional>
#include <iostream>
#include <stdlib.h>
#include <unordered_map>

void Application::run() {
    m_data.lastTime = 0.f;
    m_data.registry.clear();

    auto entityCircle = m_data.registry.create();
    m_data.registry.emplace<HAR::Component::Renderable>(entityCircle, true);
    m_data.registry.emplace<HAR::Component::Color>(entityCircle, glm::vec4(0.f, 0.f, 0.f, 1.f));
    m_data.registry.emplace<HAR::Component::Circle>(entityCircle, 0.035f);
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
    m_data.registry.emplace<HAR::Component::Color>(entityEnemy, glm::vec4(0.f, 1.f, 0.f, 1.f));
    m_data.registry.emplace<HAR::Component::Circle>(entityEnemy, 0.035f);
    m_data.registry.emplace<HAR::Component::Location>(entityEnemy, glm::vec2(0.3f, 0.3f));
    m_data.registry.emplace<HAR::Component::Overlap>(entityEnemy, std::unordered_map<entt::entity, HAR::Component::Overlap::OverlapInfo>());
    m_data.registry.emplace<HAR::Component::PhysicalBody>(entityEnemy, 1.f);
    m_data.registry.emplace<HAR::Component::Controlled>(entityEnemy, glm::vec2(0.f, 0.f));
    m_data.registry.emplace<HAR::Component::Movement>(entityEnemy,
    .000001f,
    .0001f,
    .001f,
    .001f,
    glm::vec2(0.f, 0.f),
    glm::vec2(0.f, 0.f)
    );
    m_data.registry.emplace<HAR::Component::AI>(entityEnemy, [&](entt::registry& registry, const entt::entity& entity, float deltaTime) {
        if (!!!registry.all_of<HAR::Component::Location, HAR::Component::AI, HAR::Component::Controlled>(entity)) [[unlikely]] {
            return;
        }
        auto& enemyAiComp = registry.get<HAR::Component::AI>(entity);
        auto& enemyLocationComp = registry.get<HAR::Component::Location>(entity);
        auto& enemyControlledComp = registry.get<HAR::Component::Controlled>(entity);

        auto enemyCanSeePlayer = true;       
        
        auto& targetLocation = std::any_cast<glm::vec2&>(enemyAiComp.data);

        auto view = registry.view<HAR::Component::Location, HAR::Component::Player>();
        for (auto& playerEntity : view) {
            auto& playerlocationComp = registry.get<HAR::Component::Location>(playerEntity);
            auto direction = glm::normalize(playerlocationComp.value - enemyLocationComp.value);
            auto visionBarrierView = registry.view<HAR::Component::Renderable, HAR::Component::Location>();
            for (auto& visionBarrierEntity : visionBarrierView) {
                auto visionBarrierLocationComp = registry.get<HAR::Component::Location>(visionBarrierEntity);
                if (registry.any_of<HAR::Component::Player, HAR::Component::AI>(visionBarrierEntity)) {
                    continue;
                }
                if (registry.all_of<HAR::Component::Circle>(visionBarrierEntity)) {
                    // TODO: Check sight overlaps cirlce
                    // refactor overlap manager to use a function that check if line segment intersects with circle
                    // auto circle = registry.get<HAR::Component::Circle>(visionBarrierEntity).radius = 0.1f;
                }
                    
                if (registry.all_of<HAR::Component::Polyhedron>(visionBarrierEntity)) {
                    auto& polyhedron = registry.get<HAR::Component::Polyhedron>(visionBarrierEntity);
                    for (size_t i = 0; i < polyhedron.vertices.size(); ++i) {
                        glm::vec2 start = polyhedron.vertices[i] + visionBarrierLocationComp.value;
                        glm::vec2 end = polyhedron.vertices[(i + 1) % polyhedron.vertices.size()] + visionBarrierLocationComp.value;
                        if (HAR::Math::getIntersection(enemyLocationComp.value, playerlocationComp.value, start, end).has_value()) {
                            enemyCanSeePlayer = false;
                        }
                    }

                    if (!!!enemyCanSeePlayer) {
                        break;
                    }
                }
            }
            
            if (registry.all_of<HAR::Component::Renderable>(entity)) {
                auto& enemyRenderableComp = registry.get<HAR::Component::Renderable>(entity);
                enemyRenderableComp.visible = enemyCanSeePlayer;
            }

            if (enemyCanSeePlayer) {
                targetLocation = playerlocationComp.value;
                std::cout << "Location: " << targetLocation.x << ", " << targetLocation.y << std::endl;
                break;
            }
        }
        enemyControlledComp.movementDir = glm::normalize(targetLocation - enemyLocationComp.value);
    }, glm::vec2(0.f, 0.f));




    auto entityPolyhedron = m_data.registry.create();
    m_data.registry.emplace<HAR::Component::Renderable>(entityPolyhedron, true);
    m_data.registry.emplace<HAR::Component::Color>(entityPolyhedron, glm::vec4(1.f, 0.f, 0.f, 1.f));
    m_data.registry.emplace<HAR::Component::Overlap>(entityPolyhedron, std::unordered_map<entt::entity, HAR::Component::Overlap::OverlapInfo>());
    m_data.registry.emplace<HAR::Component::Location>(entityPolyhedron, glm::vec2(-0.7f, 0.3f));
    m_data.registry.emplace<HAR::Component::PhysicalBody>(entityPolyhedron, 10.f);
    m_data.registry.emplace<HAR::Component::Polyhedron>(entityPolyhedron, std::vector<glm::vec2>{
        glm::vec2(-0.15f, -0.1f),
        glm::vec2(-0.1f, 0.15f),
        glm::vec2(0.05f, 0.2f),
        glm::vec2(0.2f, 0.05f),
        glm::vec2(0.15f, -0.15f),
        glm::vec2(0.0f, -0.2f)
    });

    // auto entityDebugPolyhedron = m_data.registry.create();
    // m_data.registry.emplace<HAR::Component::Renderable>(entityDebugPolyhedron, true);
    // m_data.registry.emplace<HAR::Component::Color>(entityDebugPolyhedron, glm::vec4(0.f, 1.f, 0.f, 1.f));
    // m_data.registry.emplace<HAR::Component::Location>(entityDebugPolyhedron, glm::vec2(-0.7f, 0.3f));
    // m_data.registry.emplace<HAR::Component::Polyhedron>(entityDebugPolyhedron, HAR::Math::InflatePolygon(std::vector<glm::vec2>{
    //     glm::vec2(-0.3f, -0.2f),
    //     glm::vec2(-0.2f, 0.3f),
    //     glm::vec2(0.1f, 0.4f),
    //     glm::vec2(0.4f, 0.1f),
    //     glm::vec2(0.3f, -0.3f),
    //     glm::vec2(0.0f, -0.4f)
    // }, 0.07f));

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
