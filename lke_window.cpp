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
    Kore::WindowOptions options;
    options.title = windowName.c_str();
    options.width = width;
    options.height = height;
    options.windowFeatures = 0; // disable resize
    
    window = Kore::System::init(options.title, options.width, options.height, &options);
}
}
