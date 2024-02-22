#pragma once

#include "entt/entity/entity.hpp"
#include "manager.hpp"

#include "components/gameplay.hpp"
#include "components/geometry.hpp"
#include "components/render.hpp"

#include <entt/entity/fwd.hpp>
#include <stdint.h>

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
    int m_score;
};
