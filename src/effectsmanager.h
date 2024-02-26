#pragma once

#include "manager.hpp"
#include "components/gameplay.hpp"
#include "effects/effect.hpp"

#include <entt/entt.hpp>
#include <iostream>

class EffectsManager : public Manager {
public:
    EffectsManager(entt::registry& registry) : Manager(registry) {
    }
    virtual ~EffectsManager() {
    }
public:
    virtual void init() override {
    }
    virtual void tick(float deltaTime) override {
        auto effectBagView = m_registry.view<HAR::Component::EffectBag>();
        for (auto entity : effectBagView) {
            auto& effectBag = effectBagView.get<HAR::Component::EffectBag>(entity);
            for (auto it = effectBag.effectsList.begin(); it != effectBag.effectsList.end(); ++it) {
                if (*it) {
                    if ((*it)->processEffect(deltaTime) == Effect::Status::Finished) {
                        effectBag.effectsList.erase(it);
                    }
                }
            }
        }
    }
};