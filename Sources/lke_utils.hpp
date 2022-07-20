#pragma once

#include <Kore/Math/Core.h>
#include <Kore/Math/Matrix.h>

#include <functional>

namespace lke
{
struct LkeExtent2D
{
    u_int32_t width = 0;
    u_int32_t height = 0;
};

class LkeUtils
{
public:
    static unsigned int convertColor(float r, float g, float b, float a)
    {
        int red = Kore::clamp((int)Kore::floor(r * 256.0), 0, 255);
        int green = Kore::clamp((int)Kore::floor(g * 256.0), 0, 255);
        int blue = Kore::clamp((int)Kore::floor(b * 256.0), 0, 255);
        int alpha = Kore::clamp((int)Kore::floor(a * 256.0), 0, 255);

        return (alpha << 24) | (red << 16) | (green << 8) | (blue);
    }

    // from: https://stackoverflow.com/a/57595105
    template <typename T, typename... Rest>
    static void hashCombine(std::size_t& seed, const T& v, const Rest&... rest)
    {
        seed ^= std::hash<T>{}(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        (hashCombine(seed, rest), ...);
    };
};
}
