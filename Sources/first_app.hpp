#pragma once

#include "lke_window.hpp"
#include "lke_pipeline.hpp"

namespace lke {
class FirstApp {
public:
    static constexpr int WIDTH = 800;
    static constexpr int HEIGHT = 600;
    
    void run();
private:
    LkeWindow lkeWindow{WIDTH, HEIGHT, "Hello Kode!"};
    LkePipeline lkePipeline{"simple_shader.vert", "simple_shader.frag", LkePipeline::defaultPipelineConfigInfo()};
    
    static void update();
};
}
