#include "inputmanager.hpp"
#include "components/gameplay.hpp"
#include "glm/ext/vector_float2.hpp"

#include <emscripten/html5.h>

#include <cstring>
#include <iostream>

InputManager::InputManager(entt::registry& registry) : Manager(registry),
    m_up(false), m_down(false), m_left(false), m_right(false) {
}

InputManager::~InputManager() {
    emscripten_set_keydown_callback(EMSCRIPTEN_EVENT_TARGET_WINDOW, this, true, nullptr);
    emscripten_set_keyup_callback(EMSCRIPTEN_EVENT_TARGET_WINDOW, this, true, nullptr);
}

void InputManager::init() {
    registerInputCallbacks();
}

void InputManager::tick(float deltaTime) {
    auto view = m_registry.view<HAR::Component::Movement>();
    for (auto entity : view) {
        auto& direction = view.get<HAR::Component::Movement>(entity).direction;
        auto updatedDiretion = glm::vec2(0.f,0.f);
        if (m_up) updatedDiretion.y += 1;
        if (m_down) updatedDiretion.y -= 1;
        if (m_left) updatedDiretion.x -= 1;
        if (m_right) updatedDiretion.x += 1;
        direction = updatedDiretion;
    }
}


void InputManager::registerInputCallbacks() {
    emscripten_set_keydown_callback(EMSCRIPTEN_EVENT_TARGET_WINDOW, this, true, keydownCallback);
    emscripten_set_keyup_callback(EMSCRIPTEN_EVENT_TARGET_WINDOW, this, true, keyupCallback);
}

EM_BOOL InputManager::keydownCallback(int eventType, const EmscriptenKeyboardEvent* e, void* userData) {
    auto* manager = static_cast<InputManager*>(userData);
    manager->onKeyChange(e, true);
    return EM_TRUE;
}

EM_BOOL InputManager::keyupCallback(int eventType, const EmscriptenKeyboardEvent* e, void* userData) {
    auto* manager = static_cast<InputManager*>(userData);
    manager->onKeyChange(e, false);
    return EM_TRUE;
}

void InputManager::onKeyChange(const EmscriptenKeyboardEvent* e, bool pressed) {
    if (std::strcmp(e->key, "ArrowUp") == 0) m_up = pressed;
    if (std::strcmp(e->key, "ArrowDown") == 0) m_down = pressed;
    if (std::strcmp(e->key, "ArrowLeft") == 0) m_left = pressed;
    if (std::strcmp(e->key, "ArrowRight") == 0) m_right = pressed;
}
