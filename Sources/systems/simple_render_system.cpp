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
        modelConstantBuffer = new Kore::Graphics5::ConstantBuffer(sizeof(Kore::mat4));
        dataConstantBuffer = new Kore::Graphics5::ConstantBuffer(64);
        dataBackgroundConstantBuffer = new Kore::Graphics5::ConstantBuffer(64);
        
        data2ConstantBuffer = new Kore::Graphics5::ConstantBuffer(64);
        data2BackgroundConstantBuffer = new Kore::Graphics5::ConstantBuffer(64);

        createPipelines();
    }

    SimpleRenderSystem::~SimpleRenderSystem()
    {
        delete modelConstantBuffer;
        delete dataConstantBuffer;
        delete dataBackgroundConstantBuffer;
        
        delete data2ConstantBuffer;
        delete data2BackgroundConstantBuffer;
    }

    void SimpleRenderSystem::createPipelines()
    {
        //    assert(pipelineLayout != nullptr && "Cannot create pipeline before pipeline layout");

        PipelineConfigInfo pipelineConfig{};
        LkePipeline::defaultPipelineConfigInfo(pipelineConfig);
        lkePipeline = std::make_unique<LkePipeline>(
            "simple_shader.vert", "simple_shader.frag", pipelineConfig);
        pipelineConfig.depthMode = Kore::Graphics5::ZCompareAlways;
        LkePipeline::enableAlphaBlending(pipelineConfig);
        lkePipelineTouchGamepad = std::make_unique<LkePipeline>(
            "touch_gamepad_shader.vert", "touch_gamepad_shader.frag", pipelineConfig);
    }

    void SimpleRenderSystem::renderGameObjects(FrameInfo &frameInfo)
    {
        lkePipeline->bind(frameInfo.commandList);

        auto projectionView = frameInfo.camera.getProjection() * frameInfo.camera.getView();

        for (auto &kv : frameInfo.gameObjects)
        {
            auto &obj = kv.second;
            if (obj.model == nullptr)
                continue;

            auto transform = projectionView * obj.transform.mat4();

            modelConstantBuffer->lock();
            modelConstantBuffer->setMatrix(0, transform);
            modelConstantBuffer->unlock();
            frameInfo.commandList->setVertexConstantBuffer(modelConstantBuffer, 0, modelConstantBuffer->size());

            obj.model->bind(frameInfo.commandList);
            obj.model->draw(frameInfo.commandList);
        }
        
        {
            Kore::vec3 colorSmall = {1.0,0.0,1.0};
            Kore::vec3 colorBig = {0.2,0.2,0.2};
            Kore::vec2 windowSize = {frameInfo.windowSize.width + 0.f, frameInfo.windowSize.height + 0.f};
            {
                lkePipelineTouchGamepad->bind(frameInfo.commandList);
                
                // big
                dataConstantBuffer->lock();
                dataConstantBuffer->setFloat2(0, frameInfo.touchGamepadObject.joystick.background1Position);
                dataConstantBuffer->setFloat(8, frameInfo.touchGamepadObject.joystick.background1Radius);
                dataConstantBuffer->setFloat3(16, colorBig);
                dataConstantBuffer->setFloat2(32, windowSize);
                dataConstantBuffer->unlock();
                frameInfo.commandList->setVertexConstantBuffer(dataConstantBuffer, 0, dataConstantBuffer->size());
                frameInfo.touchGamepadObject.model->bind(frameInfo.commandList);
                frameInfo.touchGamepadObject.model->draw(frameInfo.commandList);
                
                //small
                dataBackgroundConstantBuffer->lock();
                dataBackgroundConstantBuffer->setFloat2(0, frameInfo.touchGamepadObject.joystick.stick1Position);
                dataBackgroundConstantBuffer->setFloat(8, frameInfo.touchGamepadObject.joystick.stick1Radius);
                dataBackgroundConstantBuffer->setFloat3(16, colorSmall);
                dataBackgroundConstantBuffer->setFloat2(32, windowSize);
                dataBackgroundConstantBuffer->unlock();
                frameInfo.commandList->setVertexConstantBuffer(dataBackgroundConstantBuffer, 0, dataBackgroundConstantBuffer->size());
                frameInfo.touchGamepadObject.model->bind(frameInfo.commandList);
                frameInfo.touchGamepadObject.model->draw(frameInfo.commandList);
            }
            {
                lkePipelineTouchGamepad->bind(frameInfo.commandList);

                // big
                data2ConstantBuffer->lock();
                data2ConstantBuffer->setFloat2(0, frameInfo.touchGamepadObject.joystick.background2Position);
                data2ConstantBuffer->setFloat(8, frameInfo.touchGamepadObject.joystick.background2Radius);
                data2ConstantBuffer->setFloat3(16, colorBig);
                data2ConstantBuffer->setFloat2(32, windowSize);
                data2ConstantBuffer->unlock();
                frameInfo.commandList->setVertexConstantBuffer(data2ConstantBuffer, 0, data2ConstantBuffer->size());
                frameInfo.touchGamepadObject.model->bind(frameInfo.commandList);
                frameInfo.touchGamepadObject.model->draw(frameInfo.commandList);

                //small
                data2BackgroundConstantBuffer->lock();
                data2BackgroundConstantBuffer->setFloat2(0, frameInfo.touchGamepadObject.joystick.stick2Position);
                data2BackgroundConstantBuffer->setFloat(8, frameInfo.touchGamepadObject.joystick.stick2Radius);
                data2BackgroundConstantBuffer->setFloat3(16, colorSmall);
                data2BackgroundConstantBuffer->setFloat2(32, windowSize);
                data2BackgroundConstantBuffer->unlock();
                frameInfo.commandList->setVertexConstantBuffer(data2BackgroundConstantBuffer, 0, data2BackgroundConstantBuffer->size());
                frameInfo.touchGamepadObject.model->bind(frameInfo.commandList);
                frameInfo.touchGamepadObject.model->draw(frameInfo.commandList);
            }
        }
    }
}
