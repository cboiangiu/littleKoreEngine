#include "lke_utils.hpp"

#include <Kore/Math/Core.h>

namespace lke {
unsigned int LkeUtils::convertColor(float r, float g, float b, float a) {
    int red = Kore::clamp((int)Kore::floor(r * 256.0), 0, 255);
    int green = Kore::clamp((int)Kore::floor(g * 256.0), 0, 255);
    int blue = Kore::clamp((int)Kore::floor(b * 256.0), 0, 255);
    int alpha = Kore::clamp((int)Kore::floor(a * 256.0), 0, 255);

    return (alpha << 24) | (red << 16) | (green << 8) | (blue);
}
}
