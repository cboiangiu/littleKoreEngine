#include "lke_camera.hpp"

// std
#include <cassert>
#include <limits>

namespace lke
{

void LkeCamera::setOrthographicProjection(
    float left, float right, float top, float bottom, float near, float far)
{
    projectionMatrix = projectionMatrix.orthogonalProjection(left, right, bottom, top, near, far);
}

void LkeCamera::setPerspectiveProjection(float fovy, float aspect, float near, float far)
{
    assert(Kore::abs(aspect - std::numeric_limits<float>::epsilon()) > 0.0f);
    projectionMatrix = projectionMatrix.Perspective(fovy, aspect, near, far);
}

void LkeCamera::setViewDirection(Kore::vec3 position, Kore::vec3 direction, Kore::vec3 up)
{
    viewMatrix = viewMatrix.lookAlong(direction, position, up);
}

void LkeCamera::setViewTarget(Kore::vec3 position, Kore::vec3 target, Kore::vec3 up)
{
    viewMatrix = viewMatrix.lookAt(position, target, up);
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
    viewMatrix = Kore::mat4::Identity();;
    viewMatrix[0][0] = u.x();
    viewMatrix[0][1] = u.y();
    viewMatrix[0][2] = u.z();
    viewMatrix[1][0] = v.x();
    viewMatrix[1][1] = v.y();
    viewMatrix[1][2] = v.z();
    viewMatrix[2][0] = w.x();
    viewMatrix[2][1] = w.y();
    viewMatrix[2][2] = w.z();
    viewMatrix[0][3] = -u.dot(position);
    viewMatrix[1][3] = -v.dot(position);
    viewMatrix[2][3] = -w.dot(position);
}
}
