#pragma once

#include "entt/entt.hpp"
#include "vector2.hpp"
#include "rendermanager.hpp"
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
    Application() {};
    void run();
// Utils
public:
    class HAR::Math::Vector2 getScaleFactor() const;
    double getDevicePixelRatio() const;
protected:
    static void mainLoop(void* arg);
private:
    ApplicationData m_data;
};
