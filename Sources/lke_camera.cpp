#include "lke_camera.hpp"

// std
#include <cassert>
#include <limits>

namespace lke
{

void LkeCamera::setOrthographicProjection(
    float left, float right, float top, float bottom, float near, float far)
{
    projectionMatrix = LkeUtils::mat4Initialized(1.0f);
    projectionMatrix[0][0] = 2.f / (right - left);
    projectionMatrix[1][1] = 2.f / (bottom - top);
    projectionMatrix[2][2] = 1.f / (far - near);
    projectionMatrix[3][0] = -(right + left) / (right - left);
    projectionMatrix[3][1] = -(bottom + top) / (bottom - top);
    projectionMatrix[3][2] = -near / (far - near);
}

void LkeCamera::setPerspectiveProjection(float fovy, float aspect, float near, float far)
{
    assert(Kore::abs(aspect - std::numeric_limits<float>::epsilon()) > 0.0f);
    const float tanHalfFovy = Kore::tan(fovy / 2.f);
    projectionMatrix = LkeUtils::mat4Initialized(0.0f);
    projectionMatrix[0][0] = 1.f / (aspect * tanHalfFovy);
    projectionMatrix[1][1] = 1.f / (tanHalfFovy);
    projectionMatrix[2][2] = far / (far - near);
    projectionMatrix[2][3] = 1.f;
    projectionMatrix[3][2] = -(far * near) / (far - near);
}

void LkeCamera::setViewDirection(Kore::vec3 position, Kore::vec3 direction, Kore::vec3 up)
{
    Kore::vec3 w{ direction.normalize() };
    Kore::vec3 u{ w.cross(up).normalize() };
    Kore::vec3 v{ w.cross(u) };

    viewMatrix = LkeUtils::mat4Initialized(1.f);
    viewMatrix[0][0] = u.x();
    viewMatrix[1][0] = u.y();
    viewMatrix[2][0] = u.z();
    viewMatrix[0][1] = v.x();
    viewMatrix[1][1] = v.y();
    viewMatrix[2][1] = v.z();
    viewMatrix[0][2] = w.x();
    viewMatrix[1][2] = w.y();
    viewMatrix[2][2] = w.z();
    viewMatrix[3][0] = -u.dot(position);
    viewMatrix[3][1] = -v.dot(position);
    viewMatrix[3][2] = -w.dot(position);

    inverseViewMatrix = LkeUtils::mat4Initialized(1.f);
    inverseViewMatrix[0][0] = u.x();
    inverseViewMatrix[0][1] = u.y();
    inverseViewMatrix[0][2] = u.z();
    inverseViewMatrix[1][0] = v.x();
    inverseViewMatrix[1][1] = v.y();
    inverseViewMatrix[1][2] = v.z();
    inverseViewMatrix[2][0] = w.x();
    inverseViewMatrix[2][1] = w.y();
    inverseViewMatrix[2][2] = w.z();
    inverseViewMatrix[3][0] = position.x();
    inverseViewMatrix[3][1] = position.y();
    inverseViewMatrix[3][2] = position.z();
}

void LkeCamera::setViewTarget(Kore::vec3 position, Kore::vec3 target, Kore::vec3 up)
{
    setViewDirection(position, target - position, up);
}

void LkeCamera::setViewYXZ(Kore::vec3 position, Kore::vec3 rotation)
{
    const float c3 = Kore::cos(rotation.z());
    const float s3 = Kore::sin(rotation.z());
    const float c2 = Kore::cos(rotation.x());
    const float s2 = Kore::sin(rotation.x());
    const float c1 = Kore::cos(rotation.y());
    const float s1 = Kore::sin(rotation.y());
    const Kore::vec3 u{ (c1 * c3 + s1 * s2 * s3), (c2 * s3), (c1 * s2 * s3 - c3 * s1) };
    const Kore::vec3 v{ (c3 * s1 * s2 - c1 * s3), (c2 * c3), (c1 * c3 * s2 + s1 * s3) };
    const Kore::vec3 w{ (c2 * s1), (-s2), (c1 * c2) };
    viewMatrix = LkeUtils::mat4Initialized(1.f);
    viewMatrix[0][0] = u.x();
    viewMatrix[1][0] = u.y();
    viewMatrix[2][0] = u.z();
    viewMatrix[0][1] = v.x();
    viewMatrix[1][1] = v.y();
    viewMatrix[2][1] = v.z();
    viewMatrix[0][2] = w.x();
    viewMatrix[1][2] = w.y();
    viewMatrix[2][2] = w.z();
    viewMatrix[3][0] = -u.dot(position);
    viewMatrix[3][1] = -v.dot(position);
    viewMatrix[3][2] = -w.dot(position);

    inverseViewMatrix = LkeUtils::mat4Initialized(1.f);
    inverseViewMatrix[0][0] = u.x();
    inverseViewMatrix[0][1] = u.y();
    inverseViewMatrix[0][2] = u.z();
    inverseViewMatrix[1][0] = v.x();
    inverseViewMatrix[1][1] = v.y();
    inverseViewMatrix[1][2] = v.z();
    inverseViewMatrix[2][0] = w.x();
    inverseViewMatrix[2][1] = w.y();
    inverseViewMatrix[2][2] = w.z();
    inverseViewMatrix[3][0] = position.x();
    inverseViewMatrix[3][1] = position.y();
    inverseViewMatrix[3][2] = position.z();
}
}
