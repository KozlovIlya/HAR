// #include "worldmanager.hpp"

// #pragma once

// #include "manager.hpp"

// #include "components/gameplay.hpp"

// #include <entt/entt.hpp>
// #include <emscripten.h>
// #include <emscripten/html5.h>


// class WorldManager : public Manager {
// public:
//     WorldManager(entt::registry& registry, b2World& world);
//     virtual ~WorldManager() {
//         auto view = m_registry.view<HAR::Component::Body>();
//         for (auto& entity : view) {
//             auto& body = view.get<HAR::Component::Body>(entity);
//             if (body.body && body.body->GetWorld() == &m_world) {
//                 m_world.DestroyBody(body.body);
//             }
//         };
//         // TODO: think bout
//     }
// public:
//     virtual void init() override {
//         auto view = m_registry.view<HAR::Component::Body>();
//         for (auto& entity : view) {
//             auto& body = view.get<HAR::Component::Body>(entity);
//             if (body.body || body.body->GetWorld() != &m_world) {
//                 continue;
//             }
//             body.body = m_world.CreateBody(&body.bodyDef);
//         }
//     }
//     virtual void tick(float deltaTime) override {
//         // TODO: clear registry from the bodies that are not in the world
//         m_world.Step(deltaTime, 6, 2);
//     }
// };
