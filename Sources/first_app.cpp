#include <Kore/System.h>

#include "first_app.hpp"
#include "lke_utils.hpp"

// std
#include <stdexcept>

namespace lke{
FirstApp::FirstApp() {
    createPipeline();
    
    commandList = new Kore::Graphics5::CommandList;
    
    indices = new Kore::Graphics5::IndexBuffer(3, true);
    int* i = indices->lock();
    i[0] = 0; i[1] = 1; i[2] = 2; // for our shader hardcoded vertices
    indices->unlock();
    commandList->upload(indices);
    commandList->setIndexBuffer(*indices);
}

FirstApp::~FirstApp() {
    delete commandList;
    delete indices;
}

void FirstApp::run() {
    Kore::System::setCallback(update);
    // set more callbacks for key events here
    Kore::System::start();
}

void FirstApp::update() {
    FirstApp::instance()->drawFrame();
}

void FirstApp::createPipeline() {
    auto pipelineConfig = LkePipeline::defaultPipelineConfigInfo();
    pipelineConfig.colorAttachmentsFormat = lkeSwapChain.getSwapChainFramebufferFormat();
    lkePipeline = std::make_unique<LkePipeline>("simple_shader.vert", "simple_shader.frag", pipelineConfig);
}

void FirstApp::drawFrame() {
    int bufferIndex;
    if (!lkeSwapChain.acquireNextBuffer(bufferIndex)) {
        throw std::runtime_error("failed to acquire swap chain buffer!");
    }
    
    auto currentBuffer = lkeSwapChain.getFrameBuffer(bufferIndex);
    
    commandList->begin();
    commandList->framebufferToRenderTargetBarrier(currentBuffer);
    commandList->setRenderTargets(&currentBuffer, 1);

    commandList->clear(currentBuffer, Kore::Graphics5::ClearColorFlag, lke::LkeUtils::convertColor(0.1f, 1.0f, 1.0f, 1.0f), 1.0f, 0);
    commandList->setPipeline(lkePipeline->getPipeline());
    commandList->setPipelineLayout();

    commandList->drawIndexedVertices();

    commandList->renderTargetToFramebufferBarrier(currentBuffer);
    commandList->end();
    
    lkeSwapChain.submitBuffer();
}
}
