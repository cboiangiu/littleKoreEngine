#include "lke_swap_chain.hpp"

// std
#include <array>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <limits>
#include <set>
#include <stdexcept>

namespace lke {
LkeSwapChain::LkeSwapChain(int windowWidth, int windowHeight) : swapChainWidth{windowWidth}, swapChainHeight{windowHeight} {
    createFramebuffers();
}

LkeSwapChain::~LkeSwapChain() {
    for (auto framebuffer : swapChainFramebuffers) {
        delete framebuffer;
    }
}

bool LkeSwapChain::acquireNextBuffer(int& bufferIndex) {
    if(!nextBufferAvailable) {
        return false;
    }

    Kore::Graphics5::begin(swapChainFramebuffers[currentBuffer]);
    
    bufferIndex = currentBuffer;
    nextBufferAvailable = false;

    return true;
}

void LkeSwapChain::submitBuffer() {
    Kore::Graphics5::end();
    Kore::Graphics5::swapBuffers();

    nextBufferAvailable = true;

    currentBuffer = (currentBuffer + 1) % bufferCount;
}

void LkeSwapChain::createFramebuffers() {
    swapChainFramebuffers.resize(bufferCount);
    swapChainFramebufferFormat = Kore::Graphics5::Target32Bit;
    for (int i = 0; i < bufferCount; ++i) {
        swapChainFramebuffers[i] = new Kore::Graphics5::RenderTarget(swapChainWidth, swapChainHeight, 16, false, swapChainFramebufferFormat,
            -1, -i - 1 /* hack in an index for backbuffer render targets */);
    }
}
}
