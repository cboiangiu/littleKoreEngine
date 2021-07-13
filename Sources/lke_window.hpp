#pragma once

#include <Kore/Window.h>

#include <string>

namespace lke {
class LkeWindow {
public:
    LkeWindow(int w, int h, std::string name);
    ~LkeWindow();
    
    LkeWindow(const LkeWindow &) = delete;
    LkeWindow &operator=(const LkeWindow &) = delete;
    
    int getWidth() { return width; }
    int getHeight() { return height; }
        
private:
    void initWindow();
    
    const int width;
    const int height;
    
    std::string windowName;
    Kore::Window *window;
};
} // namespace lke
