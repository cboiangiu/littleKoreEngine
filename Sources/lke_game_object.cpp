#include <Kore/Math/Core.h>

#include "lke_game_object.hpp"

namespace lke
{

Kore::mat4 TransformComponent::mat4()
{
    const float c3 = Kore::cos(rotation.z());
    const float s3 = Kore::sin(rotation.z());
    const float c2 = Kore::cos(rotation.x());
    const float s2 = Kore::sin(rotation.x());
    const float c1 = Kore::cos(rotation.y());
    const float s1 = Kore::sin(rotation.y());
    auto mat = Kore::mat4();
    mat.Set(0, 0, scale.x() * (c1 * c3 + s1 * s2 * s3));
    mat.Set(0, 1, scale.x() * (c2 * s3));
    mat.Set(0, 2, scale.x() * (c1 * s2 * s3 - c3 * s1));
    mat.Set(0, 3, 0.0f);

    mat.Set(1, 0, scale.y() * (c3 * s1 * s2 - c1 * s3));
    mat.Set(1, 1, scale.y() * (c2 * c3));
    mat.Set(1, 2, scale.y() * (c1 * c3 * s2 + s1 * s3));
    mat.Set(1, 3, 0.0f);

    mat.Set(2, 0, scale.z() * (c2 * s1));
    mat.Set(2, 1, scale.z() * (-s2));
    mat.Set(2, 2, scale.z() * (c1 * c2));
    mat.Set(2, 3, 0.0f);

    mat.Set(3, 0, translation.x());
    mat.Set(3, 1, translation.y());
    mat.Set(3, 2, translation.z());
    mat.Set(3, 3, 1.0f);

    return mat;
}
}
