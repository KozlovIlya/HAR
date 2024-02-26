#pragma once

#include "entt/entity/entity.hpp"
#include "manager.hpp"

#include "components/gameplay.hpp"
#include "components/geometry.hpp"
#include "components/render.hpp"

#include <array>
#include <entt/entity/fwd.hpp>
#include <stdint.h>
#include <vector>


const std::vector<glm::vec2> possibleSpawnLocation = {
        glm::vec2(.4f, .4f),
        glm::vec2(.8f, 0.5f),
        glm::vec2(-.9f, .0f),
        glm::vec2(-.8f, .5f),
        glm::vec2(-.3f, -.75f),
};

class GameManager : public Manager {
public:
    GameManager(entt::registry& registry);
    virtual ~GameManager() {}
public:
    virtual void tick(float deltaTime) override;
    virtual void init() override;
protected:
    void moveCollectible();
protected:
    entt::entity m_collectible;
    entt::entity m_player;
    std::vector<entt::entity> m_enemies;
    std::vector<entt::entity> m_obstacles;
};
