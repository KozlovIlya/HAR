#pragma once

#include "manager.hpp"

#include <emscripten/html5.h>

class InputManager : public Manager {
public:
    InputManager(entt::registry& registry);
    virtual ~InputManager() override;
public:
    virtual void init() override;
    virtual void tick(float deltaTime) override;
protected:
    virtual void registerInputCallbacks();
    virtual void onKeyChange(const EmscriptenKeyboardEvent* e, bool pressed);
protected:
    static EM_BOOL keydownCallback(int eventType, const EmscriptenKeyboardEvent* e, void* userData);
    static EM_BOOL keyupCallback(int eventType, const EmscriptenKeyboardEvent* e, void* userData);
protected:
    bool m_up, m_down, m_left, m_right;
};
