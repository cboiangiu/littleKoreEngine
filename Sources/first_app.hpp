#pragma once

#include "lke_window.hpp"

namespace lke {
class FirstApp {
public:
    static constexpr int WIDTH = 800;
    static constexpr int HEIGHT = 600;
    
    void run();
private:
    LkeWindow lkeWindow{WIDTH, HEIGHT, "Hello Kode!"};
    
    static void update();
};
}
