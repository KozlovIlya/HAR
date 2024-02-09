#pragma once

#include "manager.hpp"
#include "components/gameplay.hpp"

#include <entt/entt.hpp>
#include <emscripten/html5.h>
#include <cstring>

class InputManager : public Manager {
public:
    InputManager(entt::registry& registry) : Manager(registry), up(false), down(false), left(false), right(false) {}

    virtual void tick(float deltaTime) override {
        auto view = m_registry.view<HAR::Component::DesiredMovementDirection>();
        for (auto entity : view) {
            auto& dmd = view.get<HAR::Component::DesiredMovementDirection>(entity);
            HAR::Math::Vector2 direction{0.f, 0.f};

            if (up) direction.y -= 1;
            if (down) direction.y += 1;
            if (left) direction.x -= 1;
            if (right) direction.x += 1;

            dmd.direction = direction;
        }
        up = down = left = right = false;
    }

    virtual void init() override {
        registerInputCallbacks();
    }

private:
    void registerInputCallbacks() {
        emscripten_set_keydown_callback(EMSCRIPTEN_EVENT_TARGET_WINDOW, this, true, keydownCallback);
        emscripten_set_keyup_callback(EMSCRIPTEN_EVENT_TARGET_WINDOW, this, true, keyupCallback);
    }

    static EM_BOOL keydownCallback(int eventType, const EmscriptenKeyboardEvent* e, void* userData) {
        auto* manager = static_cast<InputManager*>(userData);
        manager->onKeyChange(e, true);
        return EM_TRUE;
    }

    static EM_BOOL keyupCallback(int eventType, const EmscriptenKeyboardEvent* e, void* userData) {
        auto* manager = static_cast<InputManager*>(userData);
        manager->onKeyChange(e, false);
        return EM_TRUE;
    }

    void onKeyChange(const EmscriptenKeyboardEvent* e, bool pressed) {
        if (strcmp(e->key, "w") == 0 || strcmp(e->key, "ArrowUp") == 0) up = pressed;
        if (strcmp(e->key, "s") == 0 || strcmp(e->key, "ArrowDown") == 0) down = pressed;
        if (strcmp(e->key, "a") == 0 || strcmp(e->key, "ArrowLeft") == 0) left = pressed;
        if (strcmp(e->key, "d") == 0 || strcmp(e->key, "ArrowRight") == 0) right = pressed;
    }

    bool up, down, left, right;
};
