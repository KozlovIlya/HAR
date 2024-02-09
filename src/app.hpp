#pragma once

#include "entt/entt.hpp"
#include "manager.hpp"

#include <vector>
#include <memory>

struct ApplicationData {
    entt::registry registry;
    float lastTime;
    std::vector<std::unique_ptr<Manager>> managers;
};

class Application {
public:
    Application() {}
    void run();
protected:
    static void mainLoop(void* arg);
private:
    ApplicationData m_data;
};
