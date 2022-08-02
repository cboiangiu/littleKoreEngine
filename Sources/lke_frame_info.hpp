#pragma once

#include <Kore/Graphics5/CommandList.h>

#include "lke_camera.hpp"
#include "lke_game_object.hpp"

namespace lke
{
struct FrameInfo
{
    int frameIndex;
    float frameTime;
    Kore::Graphics5::CommandList* commandList;
    LkeCamera& camera;
    LkeGameObject::Map& gameObjects;
    LkeGameObject& touchGamepadObject;
};
}
