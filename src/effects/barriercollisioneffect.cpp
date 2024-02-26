#include "barriercollisioneffect.hpp"
#include "glm/ext/quaternion_geometric.hpp"
#include "glm/ext/scalar_constants.hpp"

#include <glm/glm.hpp>

BarrierCollisionEffect::BarrierCollisionEffect(entt::registry& registry, entt::entity entity, const EffectData& data) : Effect(registry, entity, data) {
    m_timer = 0.0f;
    m_acceleration = 0.0f;
    if (typeid(data) != typeid(BarrierCollisionEffectData)) [[unlikely]] {
        // TODO: Log error
        // m_status = Status::Invalid;
        // return;
    }
    const auto& barrierCollisionData = static_cast<const BarrierCollisionEffectData&>(data);
    m_acceleration = barrierCollisionData.acceleration;
    m_timer = barrierCollisionData.duration;
    m_accelerationLossFactor = barrierCollisionData.accelerationLossFactor;
    m_ownAccelerationPenalty = barrierCollisionData.ownAccelerationPenalty;
    m_ownAaccelerationPenaltyLooseFactor = barrierCollisionData.ownAcelerationPenaltyLooseFactor;
}


void BarrierCollisionEffect::start() {
    if (!m_registry.all_of<HAR::Component::Movement>(m_entity)) {
        // TODO: Log error
        m_status = Status::Finished;
        return;
    }
    auto& velocity = m_registry.get<HAR::Component::Movement>(m_entity).velocity;
    m_direction = -glm::normalize(velocity);
    m_status = Status::Running;
}

void BarrierCollisionEffect::update(float deltaTime) {
    if (m_status != Status::Running) {
        return;
    }

    if (m_timer > 0.0f) {
        m_timer -= deltaTime;
    } else {
        m_status = Status::Finished;
    }
    std::cout << m_timer << " " << deltaTime << std::endl;

    auto& movement = m_registry.get<HAR::Component::Movement>(m_entity);
    
    movement.velocity *= m_ownAccelerationPenalty;
    movement.velocity += m_direction * m_acceleration * deltaTime;
    if (m_acceleration < glm::epsilon<float>()) {
        m_status = Status::Finished;
    }
    m_ownAccelerationPenalty -= m_ownAaccelerationPenaltyLooseFactor / deltaTime;
    m_acceleration -= m_accelerationLossFactor / deltaTime;
    if (m_acceleration < glm::epsilon<float>()) {
        m_status = Status::Finished;
    }
}

void BarrierCollisionEffect::finish() {
}