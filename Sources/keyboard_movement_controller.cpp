#include <Kore/Math/Vector.h>

#include "keyboard_movement_controller.hpp"

// std
#include <limits>

namespace lke
{

    void KeyboardMovementController::moveInPlaneXZ(std::set<Kore::KeyCode> keyCodes, float dt, LkeGameObject &gameObject)
    {
        Kore::vec3 rotate{0, 0, 0};
        if (wasKeyPressed(keys.lookRight, keyCodes))
            rotate.y() += 1.f;
        if (wasKeyPressed(keys.lookLeft, keyCodes))
            rotate.y() -= 1.f;
        if (wasKeyPressed(keys.lookUp, keyCodes))
            rotate.x() -= 1.f;
        if (wasKeyPressed(keys.lookDown, keyCodes))
            rotate.x() += 1.f;

        if (rotate.dot(rotate) > std::numeric_limits<float>::epsilon())
        {
            gameObject.transform.rotation += lookSpeed * dt * rotate.normalize();
        }

        // limit pitch values between about +/- 85ish degrees
        gameObject.transform.rotation.x() = Kore::clamp(gameObject.transform.rotation.x(), -1.5f, 1.5f);

        gameObject.transform.rotation.y() = Kore::mod(gameObject.transform.rotation.y(), Kore::pi * 2.0f);

        float yaw = gameObject.transform.rotation.y();
        const Kore::vec3 forwardDir{Kore::sin(yaw), 0.f, Kore::cos(yaw)};
        const Kore::vec3 rightDir{forwardDir.z(), 0.f, -forwardDir.x()};
        const Kore::vec3 upDir{0.f, -1.f, 0.f};

        Kore::vec3 moveDir{0.f, 0.f, 0.f};
        if (wasKeyPressed(keys.moveForward, keyCodes))
            moveDir += forwardDir;
        if (wasKeyPressed(keys.moveBackward, keyCodes))
            moveDir -= forwardDir;
        if (wasKeyPressed(keys.moveRight, keyCodes))
            moveDir += rightDir;
        if (wasKeyPressed(keys.moveLeft, keyCodes))
            moveDir -= rightDir;
        if (wasKeyPressed(keys.moveUp, keyCodes))
            moveDir += upDir;
        if (wasKeyPressed(keys.moveDown, keyCodes))
            moveDir -= upDir;

        if (moveDir.dot(moveDir) > std::numeric_limits<float>::epsilon())
        {
            gameObject.transform.translation += moveSpeed * dt * moveDir.normalize();
        }
    }

    void KeyboardMovementController::moveInPlaneXZFromGamepad(Kore::vec2 moveAxis, Kore::vec2 lookAxis, float dt, LkeGameObject &gameObject)
    {
        Kore::vec3 rotate{0, 0, 0};
        rotate.x() -= lookAxis.y();
        rotate.y() += lookAxis.x();

        if (rotate.dot(rotate) > std::numeric_limits<float>::epsilon())
        {
            gameObject.transform.rotation += lookSpeed * .55f * dt * rotate;
        }

        // limit pitch values between about +/- 85ish degrees
        gameObject.transform.rotation.x() = Kore::clamp(gameObject.transform.rotation.x(), -1.5f, 1.5f);

        gameObject.transform.rotation.y() = Kore::mod(gameObject.transform.rotation.y(), Kore::pi * 2.0f);

        float yaw = gameObject.transform.rotation.y();
        const Kore::vec3 forwardDir{Kore::sin(yaw), 0.f, Kore::cos(yaw)};
        const Kore::vec3 rightDir{forwardDir.z(), 0.f, -forwardDir.x()};
        const Kore::vec3 upDir{0.f, -1.f, 0.f};

        Kore::vec3 moveDir{0.f, 0.f, 0.f};
        moveDir += forwardDir * moveAxis.y();
        moveDir += rightDir * moveAxis.x();

//        if (wasKeyPressed(keys.moveUp, keyCodes))
//            moveDir += upDir;
//        if (wasKeyPressed(keys.moveDown, keyCodes))
//            moveDir -= upDir;

        if (moveDir.dot(moveDir) > std::numeric_limits<float>::epsilon())
        {
            gameObject.transform.translation += moveSpeed * .55f * dt * moveDir;
        }
    }
}
