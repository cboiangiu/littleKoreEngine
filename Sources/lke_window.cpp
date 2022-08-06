#include "lke_window.hpp"

#include <Kore/System.h>

// std
#include <stdexcept>

namespace lke
{

LkeWindow::LkeWindow(int w, int h, std::string name, void (*callback)(int x, int y, void *data),
                     void (*callbackPpi)(int ppi, void *data))
    : width{ w }
    , height{ h }
    , windowName{ name }
    , framebufferResizeCallback { callback }
    , ppiChangedCallback { callbackPpi }
{
    initWindow();
}

LkeWindow::~LkeWindow()
{
    Kore::Window::destroy(window);
}

void LkeWindow::initWindow()
{
    Kore::WindowOptions windowOptions;
    windowOptions.title = windowName.c_str();
    windowOptions.width = width;
    windowOptions.height = height;
//    windowOptions.windowFeatures = 0; // disable resize

    Kore::FramebufferOptions framebufferOptions;
//    framebufferOptions.verticalSync = false;

    window = Kore::System::init(windowOptions.title, windowOptions.width, windowOptions.height,
        &windowOptions, &framebufferOptions);
    
    window->setResizeCallback(framebufferResizeCallback);
}
}
