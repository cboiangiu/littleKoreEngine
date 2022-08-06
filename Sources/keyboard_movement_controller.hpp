#pragma once

#include <Kore/Input/Keyboard.h>

#include "lke_game_object.hpp"
#include "lke_window.hpp"

#include <set>

namespace lke
{
    class KeyboardMovementController
    {
    public:
        struct KeyMappings
        {
            Kore::KeyCode moveLeft = Kore::KeyCode::KeyA;
            Kore::KeyCode moveRight = Kore::KeyCode::KeyD;
            Kore::KeyCode moveForward = Kore::KeyCode::KeyW;
            Kore::KeyCode moveBackward = Kore::KeyCode::KeyS;
            Kore::KeyCode moveUp = Kore::KeyCode::KeyQ;
            Kore::KeyCode moveDown = Kore::KeyCode::KeyE;
            Kore::KeyCode lookLeft = Kore::KeyCode::KeyLeft;
            Kore::KeyCode lookRight = Kore::KeyCode::KeyRight;
            Kore::KeyCode lookUp = Kore::KeyCode::KeyUp;
            Kore::KeyCode lookDown = Kore::KeyCode::KeyDown;
            Kore::KeyCode crouch = Kore::KeyCode::KeyControl;
        };

        void moveInPlaneXZ(std::set<Kore::KeyCode> keyCodes, float dt, LkeGameObject &gameObject);
        void moveInPlaneXZFromGamepad(Kore::vec2 moveAxis, Kore::vec2 lookAxis, float dt, LkeGameObject &gameObject);
        void moveInPlaneXZFromMouse(Kore::vec2 mouseEvent, float dt, LkeGameObject &gameObject);

        KeyMappings keys{};
        float moveSpeed{136.f};
        float moveSpeedCrouched{52.f};
        float lookSpeed{2.f};

    private:
        static bool wasKeyPressed(Kore::KeyCode keyCode, std::set<Kore::KeyCode> keyCodes)
        {
            auto it = keyCodes.find(keyCode);
            if (it != keyCodes.end())
            {
                return true;
            }
            return false;
        }
    };
}
