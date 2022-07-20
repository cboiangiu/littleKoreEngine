#pragma once

#include <Kore/Graphics5/Graphics.h>
#include <Kore/Graphics5/CommandList.h>

#include "lke_window.hpp"

// std
#include <cassert>
#include <memory>
#include <vector>

namespace lke
{
class LkeRenderer
{
public:
    static constexpr int MAX_FRAMES_IN_FLIGHT = 1;

    LkeRenderer(LkeWindow& window);
    ~LkeRenderer();

    LkeRenderer(const LkeRenderer&) = delete;
    LkeRenderer& operator=(const LkeRenderer&) = delete;

    uint32_t getFrameIndex()
    {
        return currentFrameIndex;
    }
    float getAspectRatio() const
    {
        return static_cast<float>(frameBufferExtent.width)
            / static_cast<float>(frameBufferExtent.height);
    }

    Kore::Graphics5::CommandList* beginFrame();
    void endFrame();

    void beginFrameBufferRenderPass(Kore::Graphics5::CommandList* commandList);
    void endFrameBufferRenderPass(Kore::Graphics5::CommandList* commandList);

private:
    void createCommandLists();
    void freeCommandLists();
    void recreateFrameBuffers();

    LkeWindow& lkeWindow;
    LkeExtent2D frameBufferExtent;
    std::vector<Kore::Graphics5::RenderTarget*> frameBuffers;
    std::vector<Kore::Graphics5::CommandList*> commandLists;

    uint32_t currentFrameBufferIndex{ 0 };
    uint32_t currentFrameIndex{ 0 };
    bool isFrameStarted{ false };
};
}
