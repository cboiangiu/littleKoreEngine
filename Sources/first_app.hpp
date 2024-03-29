#pragma once

#include <Kore/Input/Keyboard.h>
#include <Kore/Math/Vector.h>

#include "keyboard_movement_controller.hpp"
#include "lke_game_object.hpp"
#include "lke_renderer.hpp"
#include "lke_window.hpp"
#include "lke_camera.hpp"
#include "systems/simple_render_system.hpp"

// std
#include <memory>
#include <vector>
#include <set>
#include <map>
#include <chrono>
#include <queue>

namespace lke
{
class FirstApp
{
public:
    static constexpr int WIDTH = 1600;
    static constexpr int HEIGHT = 900;

    ~FirstApp();

    static std::shared_ptr<FirstApp> instance()
    {
        static std::shared_ptr<FirstApp> s{ new FirstApp };
        return s;
    }

    FirstApp(const FirstApp&) = delete;
    FirstApp& operator=(const FirstApp&) = delete;

    void run();

private:
    FirstApp();
    static void updateCallback();
    static void keyDownCallback(Kore::KeyCode keyCode);
    static void keyUpCallback(Kore::KeyCode keyCode);
    static void pauseCallback();
    static void resumeCallback();
    static void resizeCallback(int x, int y, void *data);
    static void touchStartCallback(int index, int x, int y);
    static void touchMoveCallback(int index, int x, int y);
    static void touchEndCallback(int index, int x, int y);

    void loadGameObjects();
    void update();
    
    bool pause = false;

    LkeWindow lkeWindow{ WIDTH, HEIGHT, "Hello Kode!", resizeCallback };
    LkeRenderer lkeRenderer{ lkeWindow };

    // note: order of declarations matters
    LkeGameObject::Map gameObjects;

    SimpleRenderSystem simpleRenderSystem;
    LkeCamera camera{};
    LkeGameObject viewerObject = LkeGameObject::createGameObject();
    LkeGameObject touchGamepadObject = LkeGameObject::createGameObject();
    KeyboardMovementController cameraController{};

    std::chrono::time_point<std::chrono::high_resolution_clock> currentTime;
    
    std::set<Kore::KeyCode> keysPressed;
    std::map<int, Kore::vec2> fingersPressed;
    Kore::vec2 latestMousePosition;
};
}
