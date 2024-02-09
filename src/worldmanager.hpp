// #pragma once

// #include "manager.hpp"

// #include "components/gameplay.hpp"

// #include <entt/entt.hpp>
// #include <emscripten.h>
// #include <emscripten/html5.h>

// #include <iostream>

// class WorldManager : public Manager {
// public:
//     WorldManager(entt::registry& registry, b2World& world) : Manager(registry, world) {}
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
//             auto& bodyComp = view.get<HAR::Component::Body>(entity);
//             if (bodyComp.body != nullptr || bodyComp.body->GetWorld() != nullptr && bodyComp.body->GetWorld() != &m_world) {
//                 continue;
//             }
//             bodyComp.body = m_world.CreateBody(&bodyComp.bodyDef);
//             bodyComp.body->SetMassData(&bodyComp.massData);
//             bodyComp.fixture = bodyComp.body->CreateFixture(&bodyComp.fixtureDef);
//             bodyComp.body->SetLinearDamping(bodyComp.bodyDef.linearDamping);
//         }
//     }
//     virtual void tick(float deltaTime) override {
//         // TODO: clear registry from the bodies that are not in the world
//         m_world.Step(deltaTime, 8, 3);
//     }
// };
