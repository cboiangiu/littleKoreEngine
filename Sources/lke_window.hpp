#pragma once

#include <Kore/Window.h>

#include "lke_utils.hpp"

// std
#include <string>

namespace lke
{

class LkeWindow
{
public:
    LkeWindow(int w, int h, std::string name, void (*callback)(int x, int y, void *data));
    ~LkeWindow();

    LkeWindow(const LkeWindow&) = delete;
    LkeWindow& operator=(const LkeWindow&) = delete;

    LkeExtent2D getExtent()
    {
        return { static_cast<uint32_t>(width), static_cast<uint32_t>(height) };
    }
    bool wasWindowResized() { return framebufferResized; }
    void resizeWindow(int x, int y) {
        this->width = x;
        this->height = y;
        framebufferResized = true;
    }
    void setResizedFalse() { framebufferResized = false; }

private:
    void (*framebufferResizeCallback)(int x, int y, void *data);
    void initWindow();

    int width;
    int height;
    bool framebufferResized = false;

    std::string windowName;
    Kore::Window* window;
};
}
