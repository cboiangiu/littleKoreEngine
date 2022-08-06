#include <Kore/Log.h>

#include "lke_renderer.hpp"

// std
#include <array>
#include <cassert>
#include <stdexcept>

namespace lke
{

LkeRenderer::LkeRenderer(LkeWindow& window)
    : lkeWindow{ window }
{
    recreateFrameBuffers();
    createCommandLists();
}

LkeRenderer::~LkeRenderer()
{
    freeCommandLists();
    for (auto frameBuffer : frameBuffers)
    {
        delete frameBuffer;
    }
}

void LkeRenderer::recreateFrameBuffers()
{
    frameBufferExtent = lkeWindow.getExtent();
    frameBuffers.resize(2);
    for (int i = 0; i < 2; ++i)
    {
        frameBuffers[i] = new Kore::Graphics5::RenderTarget(frameBufferExtent.width,
            frameBufferExtent.height, 16, false, Kore::Graphics5::Target32Bit, -1,
            -i - 1 /* hack in an index for backbuffer render targets */);
    }
}

void LkeRenderer::createCommandLists()
{
    commandLists.resize(MAX_FRAMES_IN_FLIGHT);
    for (int i = 0; i < MAX_FRAMES_IN_FLIGHT; ++i)
    {
        commandLists[i] = new Kore::Graphics5::CommandList;
    }
}

void LkeRenderer::freeCommandLists()
{
    for (auto commandList : commandLists)
    {
        delete commandList;
    }
}

Kore::Graphics5::CommandList* LkeRenderer::beginFrame()
{
    assert(!isFrameStarted && "Can't call beginFrame while already in progress");

    if(lkeWindow.wasWindowResized()) {
        recreateFrameBuffers();
        lkeWindow.setResizedFalse();
    }
    
    isFrameStarted = true;

    auto timeaaaa = std::chrono::high_resolution_clock::now();
    Kore::Graphics5::begin(frameBuffers[currentFrameBufferIndex]);
    auto timeee = std::chrono::duration<float, std::chrono::milliseconds::period>(std::chrono::high_resolution_clock::now() - timeaaaa).count();
    Kore::log(Kore::Info, ("asdasd: " + std::to_string(timeee) + "ms").c_str());
    
    commandLists[currentFrameIndex]->begin();
    commandLists[currentFrameIndex]->framebufferToRenderTargetBarrier(
        frameBuffers[currentFrameBufferIndex]);
    commandLists[currentFrameIndex]->setRenderTargets(&frameBuffers[currentFrameBufferIndex], 1);
    
    return commandLists[currentFrameIndex];
}

void LkeRenderer::endFrame()
{
    assert(isFrameStarted && "Can't call endFrame while frame is not in progress");

    commandLists[currentFrameIndex]->renderTargetToFramebufferBarrier(
        frameBuffers[currentFrameBufferIndex]);
    commandLists[currentFrameIndex]->end();

    Kore::Graphics5::end();
    Kore::Graphics5::swapBuffers();

    isFrameStarted = false;

    currentFrameIndex = (currentFrameIndex + 1) % LkeRenderer::MAX_FRAMES_IN_FLIGHT;
    currentFrameBufferIndex = (currentFrameBufferIndex + 1) % 2;
}

void LkeRenderer::beginFrameBufferRenderPass(Kore::Graphics5::CommandList* commandList)
{
    assert(isFrameStarted && "Can't call beginSwapChainRenderPass if frame is not in progress");
    assert(commandList == commandLists[currentFrameIndex]
        && "Can't begin render pass on command buffer from a different frame");

    commandList->clear(frameBuffers[currentFrameBufferIndex], Kore::Graphics5::ClearColorFlag,
        LkeUtils::convertColor(0.2,0.3,0.6,1.0), 1.0f, 0);
    commandList->viewport(0, 0, frameBufferExtent.width, frameBufferExtent.height);
    commandList->scissor(0, 0, frameBufferExtent.width, frameBufferExtent.height);
}

void LkeRenderer::endFrameBufferRenderPass(Kore::Graphics5::CommandList* commandList)
{
    assert(isFrameStarted && "Can't call endSwapChainRenderPass if frame is not in progress");
    assert(commandList == commandLists[currentFrameIndex]
        && "Can't end render pass on command buffer from a different frame");
}
}
