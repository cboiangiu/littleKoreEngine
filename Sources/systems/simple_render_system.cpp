#include "simple_render_system.hpp"

#include <Kore/Math/Matrix.h>
#include <Kore/Graphics5/ConstantBuffer.h>

#include "lke_utils.hpp"

// std
#include <array>
#include <cassert>
#include <stdexcept>

namespace lke
{

SimpleRenderSystem::SimpleRenderSystem()
{
    constantBuffer = new Kore::Graphics5::ConstantBuffer(sizeof(Kore::mat4));
    createPipeline();
}

SimpleRenderSystem::~SimpleRenderSystem()
{
    delete constantBuffer;
}

void SimpleRenderSystem::createPipeline()
{
//    assert(pipelineLayout != nullptr && "Cannot create pipeline before pipeline layout");

    PipelineConfigInfo pipelineConfig{};
    LkePipeline::defaultPipelineConfigInfo(pipelineConfig);
    lkePipeline = std::make_unique<LkePipeline>(
        "simple_shader.vert", "simple_shader.frag", pipelineConfig);
}

void SimpleRenderSystem::renderGameObjects(FrameInfo& frameInfo)
{
    lkePipeline->bind(frameInfo.commandList);

    auto projectionView = frameInfo.camera.getProjection() * frameInfo.camera.getView();

    for (auto& kv : frameInfo.gameObjects)
    {
        auto& obj = kv.second;
        if (obj.model == nullptr)
            continue;

        auto transform = projectionView * obj.transform.mat4();

        // FIXME: investigate projectionView * obj.transform.mat4();
        
        transform.Set(0, 0, 1.0f);
        transform.Set(0, 1, 0.f);
        transform.Set(0, 2, 0.f);
        transform.Set(0, 3, 0.f);

        transform.Set(1, 0, 0.f);
        transform.Set(1, 1, 1.0f);
        transform.Set(1, 2, 0.f);
        transform.Set(1, 3, 0.f);

        transform.Set(2, 0, 0.f);
        transform.Set(2, 1, 0.f);
        transform.Set(2, 2, 1.0f);
        transform.Set(2, 3, 0.f);

        transform.Set(3, 0, 0.f);
        transform.Set(3, 1, 0.f);
        transform.Set(3, 2, 0.f);
        transform.Set(3, 3, 1.0f);

        constantBuffer->lock();
        constantBuffer->setMatrix(0, transform);
        constantBuffer->unlock();
        frameInfo.commandList->setVertexConstantBuffer(constantBuffer, 0, sizeof(Kore::mat4));

        obj.model->bind(frameInfo.commandList);
        obj.model->draw(frameInfo.commandList);
    }
}
}
