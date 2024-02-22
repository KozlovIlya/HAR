#include "aimanager.hpp"

#include "components/gameplay.hpp"

#include <entt/entity/fwd.hpp>

#include <vector>

void AIManager::tick(float deltaTime) {
    auto view = m_registry.view<HAR::Component::AI>();
    for (auto& entity : view) {
        auto& ai = m_registry.get<HAR::Component::AI>(entity);
        ai.tick(m_registry, entity, deltaTime);
    }
}