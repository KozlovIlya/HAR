#pragma once

#include "effect.hpp"

#include "../components/gameplay.hpp"

#include <stdint.h>
#include <entt/entity/fwd.hpp>

#include <glm/vec2.hpp>

struct BarrierCollisionEffectData : public EffectData {
    enum class DurationType : uint_fast8_t {
        Fixed                          = 0,
        UntilAccelerationIsZero        = Fixed + 1,
        UntilAccelerationPenaltyIsZero = UntilAccelerationIsZero + 1
    };
    
    float acceleration;
    float accelerationLossFactor;
    
    float ownAccelerationPenalty;
    float ownAcelerationPenaltyLooseFactor; 

    float duration; // <= 0 means until acceleration is 0 
}; 

class BarrierCollisionEffect : public Effect {
public:
    BarrierCollisionEffect(entt::registry& registry, entt::entity entity, const EffectData& = EffectData());
    virtual ~BarrierCollisionEffect() {
    }
protected:
    virtual void start() override;
    virtual void update(float deltaTime) override;
    virtual void finish() override;
private:
    float m_timer;
    float m_acceleration;
    float m_accelerationLossFactor;
    float m_ownAccelerationPenalty;
    float m_ownAaccelerationPenaltyLooseFactor;

    glm::vec2 m_direction;
    uint_fast8_t m_durationType;
};