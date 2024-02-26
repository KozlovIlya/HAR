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
#include "effectsmanager.h"
#include "gamemanager.hpp"

#include "utils.hpp"
#include "effects/barriercollisioneffect.hpp"

#include <any>
#include <entt/entt.hpp>
#include <entt/entity/fwd.hpp>
#include <glm/glm.hpp>
#include <emscripten/emscripten.h>
#include <emscripten/html5.h>
#include <functional>
#include <iostream>
#include <memory>
#include <stdlib.h>
#include <unordered_map>

void Application::run() {
    m_data.lastTime = 0.f;
    m_data.registry.clear();

    auto entityPlayer = m_data.registry.create();
    m_data.registry.emplace<HAR::Component::Renderable>(entityPlayer, true);
    m_data.registry.emplace<HAR::Component::Color>(entityPlayer, glm::vec4(0.f, 1.f, 0.f, 1.f));
    m_data.registry.emplace<HAR::Component::Circle>(entityPlayer, 0.035f);
    m_data.registry.emplace<HAR::Component::Location>(entityPlayer, glm::vec2(0.f, 0.f));
    m_data.registry.emplace<HAR::Component::Overlap>(entityPlayer, std::unordered_map<entt::entity, HAR::Component::Overlap::OverlapInfo>(), [&](entt::registry& registry, entt::entity entity) {
        if (!!!registry.all_of<HAR::Component::Player, HAR::Component::Overlap>(entity)) [[unlikely]] {
            return;
        }

        auto& playerOverlapComp = registry.get<HAR::Component::Overlap>(entity);
        auto& playerPlayerComp = registry.get<HAR::Component::Player>(entity);
        auto collectibleView = m_data.registry.view<HAR::Component::Collectible, HAR::Component::Overlap>();
        for (auto collectibleView : collectibleView) {
            if (playerOverlapComp.overlapInfoMap.find(collectibleView) != playerOverlapComp.overlapInfoMap.end()) {
                auto& collectibleComp = m_data.registry.get<HAR::Component::Collectible>(collectibleView);
                playerPlayerComp.score++;
                collectibleComp.bCollected = true;
            }
        }
    });
    m_data.registry.emplace<HAR::Component::PhysicalBody>(entityPlayer, 1.f);
    m_data.registry.emplace<HAR::Component::Controlled>(entityPlayer, glm::vec2(0.f, 0.f));
    m_data.registry.emplace<HAR::Component::Player>(entityPlayer);
    m_data.registry.emplace<HAR::Component::EffectBag>(entityPlayer, std::list<std::shared_ptr<Effect>>());
    m_data.registry.emplace<HAR::Component::Movement>(entityPlayer,
        .00001f,
        .001f,
        .01f,
        .01f,
        glm::vec2(0.f, 0.f),
        glm::vec2(0.f, 0.f)
    );

    auto entityEnemyGreg = m_data.registry.create();
    m_data.registry.emplace<HAR::Component::Renderable>(entityEnemyGreg, true);
    m_data.registry.emplace<HAR::Component::Color>(entityEnemyGreg, glm::vec4(1.f, 0.f, 0.f, 1.f));
    m_data.registry.emplace<HAR::Component::Circle>(entityEnemyGreg, 0.035f);
    m_data.registry.emplace<HAR::Component::Location>(entityEnemyGreg, glm::vec2(0.3f, 0.3f));
    m_data.registry.emplace<HAR::Component::Overlap>(entityEnemyGreg, std::unordered_map<entt::entity, HAR::Component::Overlap::OverlapInfo>(), [&](entt::registry& registry, entt::entity entity) {
        auto playerView = registry.view<HAR::Component::Player>();
        for (auto& player : playerView) {
            if (!!!registry.all_of<HAR::Component::Overlap>(entity)) [[unlikely]] {
                return;
            }
            auto& enemyOverlapComp = registry.get<HAR::Component::Overlap>(entity);
            if (enemyOverlapComp.overlapInfoMap.find(player) != enemyOverlapComp.overlapInfoMap.end()) {
                auto& playerComp = registry.get<HAR::Component::Player>(player);
                playerComp.score /= 2;
            }
        }
    });
    m_data.registry.emplace<HAR::Component::PhysicalBody>(entityEnemyGreg, 1.f);
    m_data.registry.emplace<HAR::Component::Controlled>(entityEnemyGreg, glm::vec2(0.f, 0.f));
    m_data.registry.emplace<HAR::Component::EffectBag>(entityEnemyGreg, std::list<std::shared_ptr<Effect>>());
    m_data.registry.emplace<HAR::Component::Movement>(entityEnemyGreg,
    .000001f,
    .0001f,
    .06f,
    .06f,
    glm::vec2(0.f, 0.f),
    glm::vec2(0.f, 0.f)
    );
    m_data.registry.emplace<HAR::Component::AI>(entityEnemyGreg, [&](entt::registry& registry, const entt::entity& entity, float deltaTime) {
        HAR::AI::chasePlayer(registry, entity, deltaTime);
    }, glm::vec2(0.f, 0.f));

    auto entityEnemyJosh = m_data.registry.create();
    m_data.registry.emplace<HAR::Component::Renderable>(entityEnemyJosh, true);
    m_data.registry.emplace<HAR::Component::Color>(entityEnemyJosh, glm::vec4(1.f, 0.f, 0.f, 1.f));
    m_data.registry.emplace<HAR::Component::Circle>(entityEnemyJosh, 0.035f);
    m_data.registry.emplace<HAR::Component::Location>(entityEnemyJosh, glm::vec2(-0.3f, 0.3f));
    m_data.registry.emplace<HAR::Component::Overlap>(entityEnemyJosh, std::unordered_map<entt::entity, HAR::Component::Overlap::OverlapInfo>(), [&](entt::registry& registry, entt::entity entity) {
        auto playerView = registry.view<HAR::Component::Player>();
        for (auto& player : playerView) {
            if (!!!registry.all_of<HAR::Component::Overlap>(entity)) [[unlikely]] {
                return;
            }
            auto& enemyOverlapComp = registry.get<HAR::Component::Overlap>(entity);
            if (enemyOverlapComp.overlapInfoMap.find(player) != enemyOverlapComp.overlapInfoMap.end()) {
                auto& playerComp = registry.get<HAR::Component::Player>(player);
                playerComp.score /= 2;
            }
        }
    });
    m_data.registry.emplace<HAR::Component::PhysicalBody>(entityEnemyJosh, 1.f);
    m_data.registry.emplace<HAR::Component::Controlled>(entityEnemyJosh, glm::vec2(0.f, 0.f));
    m_data.registry.emplace<HAR::Component::EffectBag>(entityEnemyJosh, std::list<std::shared_ptr<Effect>>());
    m_data.registry.emplace<HAR::Component::Movement>(entityEnemyJosh,
    .000001f,
    .0001f,
    .06f,
    .06f,
    glm::vec2(0.f, 0.f),
    glm::vec2(0.f, 0.f)
    );
    m_data.registry.emplace<HAR::Component::AI>(entityEnemyJosh, [&](entt::registry& registry, const entt::entity& entity, float deltaTime) {
        HAR::AI::chasePlayer(registry, entity, deltaTime);
    }, glm::vec2(0.f, 0.f));



    auto entityLeftBorder = m_data.registry.create();
    m_data.registry.emplace<HAR::Component::Renderable>(entityLeftBorder, true);
    m_data.registry.emplace<HAR::Component::Color>(entityLeftBorder, glm::vec4(0.f, 0.f, 0.f, 1.f));
    m_data.registry.emplace<HAR::Component::Location>(entityLeftBorder, glm::vec2(0.f, 0.f));
    m_data.registry.emplace<HAR::Component::Polyhedron>(entityLeftBorder, std::vector<glm::vec2>{
        glm::vec2(-1.0f, -1.0f),
        glm::vec2(-1.0f, 1.0f),
        glm::vec2(-.99f, 1.0f),
        glm::vec2(-.99f, -1.0f)
    });
    m_data.registry.emplace<HAR::Component::Overlap>(entityLeftBorder, std::unordered_map<entt::entity, HAR::Component::Overlap::OverlapInfo>(), [&](entt::registry& registry, entt::entity entity) {
        HAR::Collision::pushCollidedComponents(registry, entity);
    });
    m_data.registry.emplace<HAR::Component::PhysicalBody>(entityLeftBorder, 1.23f);

    auto entityRightBorder = m_data.registry.create();
    m_data.registry.emplace<HAR::Component::Renderable>(entityRightBorder, true);
    m_data.registry.emplace<HAR::Component::Color>(entityRightBorder, glm::vec4(0.f, 0.f, 0.f, 1.f));
    m_data.registry.emplace<HAR::Component::Location>(entityRightBorder, glm::vec2(0.f, 0.f));
    m_data.registry.emplace<HAR::Component::Polyhedron>(entityRightBorder, std::vector<glm::vec2>{
        glm::vec2(.99f, -1.0f),
        glm::vec2(.99f, 1.0f),
        glm::vec2(1.0f, 1.0f),
        glm::vec2(1.0f, -1.0f)
    });
    m_data.registry.emplace<HAR::Component::Overlap>(entityRightBorder, std::unordered_map<entt::entity, HAR::Component::Overlap::OverlapInfo>(), [&](entt::registry& registry, entt::entity entity) {
        HAR::Collision::pushCollidedComponents(registry, entity);
    });
    m_data.registry.emplace<HAR::Component::PhysicalBody>(entityRightBorder, 1.23f);

    auto entityTopBorder = m_data.registry.create();
    m_data.registry.emplace<HAR::Component::Renderable>(entityTopBorder, true);
    m_data.registry.emplace<HAR::Component::Color>(entityTopBorder, glm::vec4(0.f, 0.f, 0.f, 1.f));
    m_data.registry.emplace<HAR::Component::Location>(entityTopBorder, glm::vec2(0.f, 0.f));
    m_data.registry.emplace<HAR::Component::Polyhedron>(entityTopBorder, std::vector<glm::vec2>{
        glm::vec2(-1.f, .99f),
        glm::vec2(-1.f, 1.0f),
        glm::vec2(1.0f, 1.0f),
        glm::vec2(1.0f, .99f)
    });
    m_data.registry.emplace<HAR::Component::Overlap>(entityTopBorder, std::unordered_map<entt::entity, HAR::Component::Overlap::OverlapInfo>(), [&](entt::registry& registry, entt::entity entity) {
        HAR::Collision::pushCollidedComponents(registry, entity);
    });
    m_data.registry.emplace<HAR::Component::PhysicalBody>(entityTopBorder, 1.23f);


    auto entityBottomBorder = m_data.registry.create();
    m_data.registry.emplace<HAR::Component::Renderable>(entityBottomBorder, true);
    m_data.registry.emplace<HAR::Component::Color>(entityBottomBorder, glm::vec4(0.f, 0.f, 0.f, 1.f));
    m_data.registry.emplace<HAR::Component::Location>(entityBottomBorder, glm::vec2(0.f, 0.f));
    m_data.registry.emplace<HAR::Component::Polyhedron>(entityBottomBorder, std::vector<glm::vec2>{
        glm::vec2(-1.f, -1.f),
        glm::vec2(-1.f, -.99f),
        glm::vec2(1.0f, -.99f),
        glm::vec2(1.0f, -1.f)
    });
    m_data.registry.emplace<HAR::Component::Overlap>(entityBottomBorder, std::unordered_map<entt::entity, HAR::Component::Overlap::OverlapInfo>(), [&](entt::registry& registry, entt::entity entity) {
        HAR::Collision::pushCollidedComponents(registry, entity);
    });
    m_data.registry.emplace<HAR::Component::PhysicalBody>(entityBottomBorder, 1.23f);



    auto entityObstacleDefault = m_data.registry.create();
    m_data.registry.emplace<HAR::Component::Renderable>(entityObstacleDefault, true);
    m_data.registry.emplace<HAR::Component::Color>(entityObstacleDefault, glm::vec4(0.f, 0.f, 0.f, 1.f));
    m_data.registry.emplace<HAR::Component::Overlap>(entityObstacleDefault, std::unordered_map<entt::entity, HAR::Component::Overlap::OverlapInfo>(), [&](entt::registry& registry, entt::entity entity) {
        HAR::Collision::pushCollidedComponents(registry, entity);
        // BarrierCollisionEffectData data;
        // data.acceleration = 0.00001f;
        // data.accelerationLossFactor = 0.0f;
        // data.ownAccelerationPenalty = 0.0f;
        // data.ownAcelerationPenaltyLooseFactor = 0.0f;
        // data.duration = 1000.f;
        // for (auto& [colidedEntity, overlapInfo] : registry.get<HAR::Component::Overlap>(entity).overlapInfoMap) {
        //     if (registry.all_of<HAR::Component::EffectBag>(colidedEntity)) {
        //         auto& effectBag = registry.get<HAR::Component::EffectBag>(colidedEntity);
        //         BarrierCollisionEffect effect(registry, colidedEntity, data);
        //         effectBag.effectsList.push_back(std::make_shared<BarrierCollisionEffect>(effect));
        //     }
        // }
    });
    m_data.registry.emplace<HAR::Component::Location>(entityObstacleDefault, glm::vec2(-0.1f, 0.5f));
    m_data.registry.emplace<HAR::Component::PhysicalBody>(entityObstacleDefault, 1.23f);
    m_data.registry.emplace<HAR::Component::Polyhedron>(entityObstacleDefault, std::vector<glm::vec2>{
        glm::vec2(-0.1f, -0.17f),
        glm::vec2(-0.7f, 0.35f),
        glm::vec2(0.05f, 0.55f),
        glm::vec2(0.2f, 0.25f),
        glm::vec2(0.15f, -0.15f),
        glm::vec2(0.0f, -0.7f)
    });

    auto entityObstacleBig = m_data.registry.create();
    m_data.registry.emplace<HAR::Component::Renderable>(entityObstacleBig, true);
    m_data.registry.emplace<HAR::Component::Color>(entityObstacleBig, glm::vec4(0.f, 0.f, 0.f, 1.f));
    m_data.registry.emplace<HAR::Component::Overlap>(entityObstacleBig, std::unordered_map<entt::entity, HAR::Component::Overlap::OverlapInfo>(), [&](entt::registry& registry, entt::entity entity) {
        HAR::Collision::pushCollidedComponents(registry, entity);
    });
    m_data.registry.emplace<HAR::Component::Location>(entityObstacleBig, glm::vec2(.7f, -.7f));
    m_data.registry.emplace<HAR::Component::PhysicalBody>(entityObstacleBig, 1.23f);
    m_data.registry.emplace<HAR::Component::Polyhedron>(entityObstacleBig, std::vector<glm::vec2>{
        glm::vec2(-0.7f, -0.09f),
        glm::vec2(-0.2f, 0.65f),
        glm::vec2(0.4f, 0.9f),
        glm::vec2(0.6f, 0.65f),
        glm::vec2(0.7f, -0.35f),
        glm::vec2(0.6f, -0.5f),
        glm::vec2(0.4f, -0.55f),
        glm::vec2(-0.6f, -0.5f)
    });


    auto entityObstacleOneMore = m_data.registry.create();
    m_data.registry.emplace<HAR::Component::Renderable>(entityObstacleOneMore, true);
    m_data.registry.emplace<HAR::Component::Color>(entityObstacleOneMore, glm::vec4(0.f, 0.f, 0.f, 1.f));
    m_data.registry.emplace<HAR::Component::Overlap>(entityObstacleOneMore, std::unordered_map<entt::entity, HAR::Component::Overlap::OverlapInfo>(), [&](entt::registry& registry, entt::entity entity) {
        HAR::Collision::pushCollidedComponents(registry, entity);
    });
    m_data.registry.emplace<HAR::Component::Location>(entityObstacleOneMore, glm::vec2(-.8f, -.2f));
    m_data.registry.emplace<HAR::Component::PhysicalBody>(entityObstacleOneMore, 1.23f);
    m_data.registry.emplace<HAR::Component::Polyhedron>(entityObstacleOneMore, std::vector<glm::vec2>{
        glm::vec2(-0.22f, -1.f),
        glm::vec2(0.3f, 0.3f),
        glm::vec2(0.2f, -1.f)
    }); 

    m_data.managers.emplace_back(std::make_unique<InputManager>(m_data.registry))->init();
    m_data.managers.emplace_back(std::make_unique<AIManager>(m_data.registry))->init();
    m_data.managers.emplace_back(std::make_unique<MovementManager>(m_data.registry))->init();
    m_data.managers.emplace_back(std::make_unique<OverlapManager>(m_data.registry))->init();
    m_data.managers.emplace_back(std::make_unique<GameManager>(m_data.registry))->init();
    m_data.managers.emplace_back(std::make_unique<EffectsManager>(m_data.registry))->init();
    m_data.managers.emplace_back(std::make_unique<PhysicsManager>(m_data.registry))->init();
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
