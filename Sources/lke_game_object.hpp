#pragma once

#include <Kore/Math/Matrix.h>

#include "lke_model.hpp"

// std
#include <memory>
#include <unordered_map>

namespace lke
{
struct TransformComponent
{
    Kore::vec3 translation{};
    Kore::vec3 scale{ 1.f, 1.f, 1.f };
    Kore::vec3 rotation{};

    // Matrix corrsponds to Translate * Ry * Rx * Rz * Scale
    // Rotations correspond to Tait-bryan angles of Y(1), X(2), Z(3)
    // https://en.wikipedia.org/wiki/Euler_angles#Rotation_matrix
    Kore::mat4 mat4();
};

struct JoystickComponent
{
    Kore::vec2 background1Position{};
    Kore::vec2 stick1Position{};
    float background1Radius{};
    float stick1Radius{};
    
    Kore::vec2 background2Position{};
    Kore::vec2 stick2Position{};
    float background2Radius{};
    float stick2Radius{};
};

class LkeGameObject
{
public:
    using id_t = unsigned int;
    using Map = std::unordered_map<id_t, LkeGameObject>;

    static LkeGameObject createGameObject()
    {
        static id_t currentId = 0;
        return LkeGameObject{ currentId++ };
    }

    LkeGameObject(const LkeGameObject&) = delete;
    LkeGameObject& operator=(const LkeGameObject&) = delete;
    LkeGameObject(LkeGameObject&&) = default;
    LkeGameObject& operator=(LkeGameObject&&) = default;

    id_t getId()
    {
        return id;
    }

    Kore::vec3 color{};
    TransformComponent transform{};
    JoystickComponent joystick{};

    // Optional pointer components
    std::shared_ptr<LkeModel> model{};

private:
    LkeGameObject(id_t objId)
        : id{ objId }
    {
    }

    id_t id;
};
}
