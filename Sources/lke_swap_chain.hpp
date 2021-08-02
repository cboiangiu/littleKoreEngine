#pragma once

#include <Kore/Graphics5/Graphics.h>

// std
#include <vector>

namespace lke {
class LkeSwapChain {
public:
    static constexpr int bufferCount = 3;
    
    LkeSwapChain(int windowWidth, int windowHeight);
    ~LkeSwapChain();
    
    LkeSwapChain(const LkeSwapChain &) = delete;
    void operator=(const LkeSwapChain &) = delete;
    
    Kore::Graphics5::RenderTarget* getFrameBuffer(int index) { return swapChainFramebuffers[index]; }
    Kore::Graphics5::RenderTargetFormat getSwapChainFramebufferFormat() { return swapChainFramebufferFormat; }
    int width() { return swapChainWidth; }
    int height() { return swapChainHeight; }

    float aspectRatio() {
        return static_cast<float>(swapChainWidth) / static_cast<float>(swapChainHeight);
    }
    
    bool acquireNextBuffer(int& bufferIndex);
    void submitBuffer();

private:
    void createFramebuffers();

    bool nextBufferAvailable = true;

    int swapChainWidth;
    int swapChainHeight;
    
    Kore::Graphics5::RenderTargetFormat swapChainFramebufferFormat;

    std::vector<Kore::Graphics5::RenderTarget*> swapChainFramebuffers;
    
    int currentBuffer = 0;
};
}
