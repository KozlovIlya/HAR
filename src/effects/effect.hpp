#pragma once

#include <entt/entt.hpp>

#include <list>

        #include <iostream>

struct EffectData {
};

class Effect {
public:
    enum class Status {
        Init,
        Running,
        Finished,
        Invalid
    };
public:
    Effect(entt::registry& registry, entt::entity entity, const EffectData& data = EffectData()) 
        : m_registry(registry), m_entity(entity), m_status(Status::Init) {
    }
    virtual ~Effect() {}
public:
    Status processEffect(float deltaTime) {
        if (m_status == Status::Init) {
            start();
        }
        if (m_status == Status::Running) {
            update(deltaTime);

        }
        if (m_status == Status::Finished) {
            finish();
        }
        if (m_status == Status::Invalid) {
        }
        return m_status;
    }

    Status getStatus() const {
        return m_status;
    }

protected:
    virtual void update(float deltaTime) {
    }
    virtual void start() {
    }
    virtual void finish() {
    }
protected:
    entt::registry& m_registry;
    entt::entity m_entity;
    Status m_status;
};
