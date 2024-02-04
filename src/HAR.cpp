#include <exception>
#include <iostream>

#include "app.hpp"

int main() {
  try {
    Application app;
    app.run();
  } catch (std::exception &e) {
    std::cout << e.what() << '\n';
    return 1;
  }
  return 0;
}




// #include "box2d/b2_body.h"
// #include "box2d/b2_world.h"
// #include "entt/entity/fwd.hpp"
// #include <emscripten/emscripten.h>
// #include <emscripten/html5.h>
// #include <GLES2/gl2.h>
// #include <box2d/box2d.h>
// #include <entt/entt.hpp>

// class World {
// public:
//     World() : m_world(b2Vec2(0.0f, -10.0f)) { // Инициализация мира Box2D с гравитацией
//     }

//     entt::registry& getRegistry() {
//         return m_registry;
//     }

//     b2World& getPhysicsWorld() {
//         return m_world;
//     }

// private:
//     b2World m_world;
//     entt::registry m_registry;
// };




//     void update(float deltaTime) override {
//         // Логика обновления игрока
//     }

//     void draw() override {
//         // Логика рендеринга игрока
//         auto body = m_world.getRegistry().get<b2Body*>(m_entity);
//         b2Vec2 position = body->GetPosition();

//         glMatrixMode(GL_MODELVIEW);
//         glPushMatrix();
//         glTranslatef(position.x, position.y, 0);
//         glColor4f(1.0f, 0.0f, 0.0f, 1.0f); // Красный цвет
//         glBegin(GL_TRIANGLE_FAN);
//         for (int i = 0; i < 360; i += 18) {
//             float rad = i * b2_pi / 180.0f;
//             glVertex2f(cos(rad), sin(rad));
//         }
//         glEnd();
//         glPopMatrix();
//     }
// };





// void gameLoop() {
//     // Обновление физики мира
//     gameWorld.getWorld().Step(1.0f / 60.0f, 6, 2);

//     // Очистка экрана
//     glClearColor(0.5f, 0.0f, 0.5f, 1.0f);
//     glClear(GL_COLOR_BUFFER_BIT);

//     // Рендеринг и обновление акторов
//     // ...

//     // Обработка контактов и событий
//     // ...
// }

// World gameWorld;

// int main() {
//     EmscriptenWebGLContextAttributes attrs;
//     emscripten_webgl_init_context_attributes(&attrs);
//     attrs.alpha = attrs.depth = attrs.stencil = attrs.antialias = attrs.preserveDrawingBuffer = attrs.failIfMajorPerformanceCaveat = 0;
//     attrs.enableExtensionsByDefault = 1;
//     attrs.premultipliedAlpha = 0;
//     attrs.majorVersion = 1;
//     attrs.minorVersion = 0;

//     EMSCRIPTEN_WEBGL_CONTEXT_HANDLE ctx = emscripten_webgl_create_context("#canvas", &attrs);
//     emscripten_webgl_make_context_current(ctx);

//     glClearColor(0.5f, 0.0f, 0.5f, 1.0f);
//     glClear(GL_COLOR_BUFFER_BIT);



    

//     emscripten_set_main_loop(gameLoop, 0, 1);

//     return 0;
// }
