#include "gamemanager.hpp"

#include "components/gameplay.hpp"

#include <entt/entt.hpp>

#include <iostream>

GameManager::GameManager(entt::registry& registry) : Manager(registry), m_collectible(entt::null), m_score(0) {
}

void GameManager::tick(float deltaTime) {
    if (!m_registry.all_of<HAR::Component::Overlap>(m_collectible)) {
        return;
    }
    auto& overlapComp = m_registry.get<HAR::Component::Overlap>(m_collectible);
    for (auto& [entity, overlapInfo] : overlapComp.overlapInfoMap) {
        if (m_registry.all_of<HAR::Component::Player>(entity)) {
            m_score++;
            moveCollectible();
            std::cout << "Score: " << m_score << std::endl;
            break;
        }
    }
}

void GameManager::init() {
    m_collectible = m_registry.create();
    m_registry.emplace<HAR::Component::Renderable>(m_collectible, true);
    m_registry.emplace<HAR::Component::Color>(m_collectible, glm::vec4(0.f, 0.f, 1.f, 1.f));
    m_registry.emplace<HAR::Component::Circle>(m_collectible, 0.05f);
    m_registry.emplace<HAR::Component::Location>(m_collectible, glm::vec2(0.f, 0.f));
    m_registry.emplace<HAR::Component::Overlap>(m_collectible, std::unordered_map<entt::entity, HAR::Component::Overlap::OverlapInfo>());
    moveCollectible();
}

void GameManager::moveCollectible() { 
    auto& location = m_registry.get<HAR::Component::Location>(m_collectible);
    location.value = glm::vec2((rand() % 100) / 100.f, (rand() % 100) / 100.f);
}