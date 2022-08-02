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
    void createPipelines();

    std::unique_ptr<LkePipeline> lkePipeline;
    std::unique_ptr<LkePipeline> lkePipelineTouchGamepad;

    Kore::Graphics5::ConstantBuffer* modelConstantBuffer;
    Kore::Graphics5::ConstantBuffer* dataConstantBuffer;
    Kore::Graphics5::ConstantBuffer* dataBackgroundConstantBuffer;
    
    Kore::Graphics5::ConstantBuffer* data2ConstantBuffer;
    Kore::Graphics5::ConstantBuffer* data2BackgroundConstantBuffer;
};
}
