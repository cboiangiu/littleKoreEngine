#pragma once

#include <Kore/Graphics5/CommandList.h>

#include "lke_window.hpp"
#include "lke_pipeline.hpp"
#include "lke_swap_chain.hpp"

// std
#include <memory>

namespace lke {
class FirstApp {
public:
    static constexpr int WIDTH = 800;
    static constexpr int HEIGHT = 600;

    ~FirstApp();
    
    static std::shared_ptr<FirstApp> instance() {
        static std::shared_ptr<FirstApp> s{new FirstApp};
        return s;
    }
        
    FirstApp(const FirstApp &) = delete;
    FirstApp &operator=(const FirstApp &) = delete;

    void run();

private:
    FirstApp();

    void createPipeline();
    void drawFrame();

    LkeWindow lkeWindow{WIDTH, HEIGHT, "Hello Kode!"};
    LkeSwapChain lkeSwapChain{lkeWindow.getWidth(), lkeWindow.getHeight()};
    std::unique_ptr<LkePipeline> lkePipeline;

    Kore::Graphics5::CommandList* commandList;
    Kore::Graphics5::IndexBuffer* indices;
    
    static void update();
};
}
