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
    LkeWindow(int w, int h, std::string name);
    ~LkeWindow();

    LkeWindow(const LkeWindow&) = delete;
    LkeWindow& operator=(const LkeWindow&) = delete;

    LkeExtent2D getExtent()
    {
        return { static_cast<uint32_t>(width), static_cast<uint32_t>(height) };
    }
    //    bool wasWindowResized() { return framebufferResized; }
    //    void resetWindowResizedFlag() { framebufferResized = false; }

private:
    //    static void framebufferResizeCallback(GLFWwindow *window, int width, int height);
    void initWindow();

    int width;
    int height;
    //    bool framebufferResized = false;

    std::string windowName;
    Kore::Window* window;
};
}
