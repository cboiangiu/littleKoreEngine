#pragma once

#include <Kore/Math/Matrix.h>

#include "lke_utils.hpp"

namespace lke
{

class LkeCamera
{
public:
    void setOrthographicProjection(
        float left, float right, float top, float bottom, float near, float far);
    void setPerspectiveProjection(float fovy, float aspect, float near, float far);

    void setViewDirection(
        Kore::vec3 position, Kore::vec3 direction, Kore::vec3 up = Kore::vec3{ 0.f, -1.f, 0.f });
    void setViewTarget(
        Kore::vec3 position, Kore::vec3 target, Kore::vec3 up = Kore::vec3{ 0.f, -1.f, 0.f });
    void setViewYXZ(Kore::vec3 position, Kore::vec3 rotation);

    const Kore::mat4& getProjection() const
    {
        return projectionMatrix;
    }
    const Kore::mat4& getView() const
    {
        return viewMatrix;
    }

private:
    Kore::mat4 projectionMatrix = Kore::mat4::Identity();
    Kore::mat4 viewMatrix = Kore::mat4::Identity();
};
}
