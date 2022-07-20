#pragma once

#include "lke_frame_info.hpp"
#include "lke_pipeline.hpp"

// std
#include <memory>
#include <vector>

namespace lke
{
class SimpleRenderSystem
{
public:
    SimpleRenderSystem();
    ~SimpleRenderSystem();

    SimpleRenderSystem(const SimpleRenderSystem&) = delete;
    SimpleRenderSystem& operator=(const SimpleRenderSystem&) = delete;

    void renderGameObjects(FrameInfo& frameInfo);

private:
    void createPipelineLayout();
    void createPipeline();

    std::unique_ptr<LkePipeline> lkePipeline;

    Kore::Graphics5::ConstantBuffer* constantBuffer;
};
}
