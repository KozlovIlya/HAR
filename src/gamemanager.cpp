#include "gamemanager.hpp"

#include "components/gameplay.hpp"
#include "entt/entity/fwd.hpp"
#include "glm/detail/qualifier.hpp"
#include "utils.hpp"

#include <entt/entt.hpp>
#include <glm/glm.hpp>

#include <iostream>
#include <stdlib.h>

GameManager::GameManager(entt::registry& registry) : Manager(registry), m_collectible(entt::null) {
}

void GameManager::tick(float deltaTime) {
    if (m_collectible == entt::null) {
        init();
    }
    
    auto playerView = m_registry.view<HAR::Component::Player>();
    for (auto& playerEntity : playerView) {
        auto& player = m_registry.get<HAR::Component::Player>(playerEntity);
        std::cout << "Player score: " << player.score << std::endl;
        auto enemyView = m_registry.view<HAR::Component::AI, HAR::Component::Movement>();
        for (auto& enemyEntity : enemyView) {
            auto& movement = m_registry.get<HAR::Component::Movement>(enemyEntity);
            movement.acceleration = player.score * 0.0000001f;
            movement.maxSpeed = player.score * 0.00001f; 
        }
    }

    auto collectibleView = m_registry.view<HAR::Component::Collectible, HAR::Component::Location>();
    for (auto& collectibleEntity : collectibleView) {
        auto& collectible = m_registry.get<HAR::Component::Collectible>(collectibleEntity);
        if (collectible.bCollected) {
            moveCollectible();
            collectible.bCollected = false;
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
    m_registry.emplace<HAR::Component::Collectible>(m_collectible, false);
    moveCollectible();
}

void GameManager::moveCollectible() { 
    auto& location = m_registry.get<HAR::Component::Location>(m_collectible);
    location.value = getRandomLocationWithNoise();
};

glm::vec2 GameManager::getRandomLocationWithNoise() {
    if (possibleSpawnLocations.empty()) {
        return glm::vec2(0.0f);
    }

    srand(time(0));
    glm::vec2 location = possibleSpawnLocations[rand() % possibleSpawnLocations.size() - 1];
    location.x += HAR::Math::getRandomFloat(-0.05f, 0.05f);
    location.y += HAR::Math::getRandomFloat(-0.05f, 0.05f);

    return location;
}
