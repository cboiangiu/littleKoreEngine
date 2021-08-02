#include <Kore/System.h>

#include "lke_window.hpp"

namespace lke{
LkeWindow::LkeWindow(int w, int h, std::string name) : width(w), height(h), windowName(name) {
    initWindow();
}

LkeWindow::~LkeWindow() {
    Kore::Window::destroy(window);
}

void LkeWindow::initWindow() {
    Kore::WindowOptions windowOptions;
    windowOptions.title = windowName.c_str();
    windowOptions.width = width;
    windowOptions.height = height;
    windowOptions.windowFeatures = 0; // disable resize
    
    Kore::FramebufferOptions framebufferOptions;
//    framebufferOptions.verticalSync = false;
    
    window = Kore::System::init(windowOptions.title, windowOptions.width, windowOptions.height, &windowOptions, &framebufferOptions);
}
}
