#include <Kore/System.h>

#include "first_app.hpp"
#include "lke_utils.hpp"
#include "lke_frame_info.hpp"

// std
#include <array>
#include <cassert>
#include <stdexcept>

namespace lke
{

FirstApp::FirstApp()
{
    loadGameObjects();

    viewerObject.transform.translation.set(.0f, .0f, -2.5f);
}

FirstApp::~FirstApp()
{
}

void FirstApp::run()
{
    Kore::System::setCallback(updateCallback);
    // set more callbacks for key events here
    currentTime = std::chrono::high_resolution_clock::now();
    Kore::System::start();
}

void FirstApp::updateCallback()
{
    FirstApp::instance()->update();
}

void FirstApp::loadGameObjects()
{
    std::shared_ptr<LkeModel> lkeModel = LkeModel::createModelFromFile(
        "/Users/catalinboiangiu/Documents/Projects/littleKoreEngine/Sources/models/flat_vase.obj");
    auto flatVase = LkeGameObject::createGameObject();
    flatVase.model = lkeModel;
    flatVase.transform.translation = {-.5f, .5f, 0.f};
    flatVase.transform.scale = {3.f, 1.5f, 3.f};
    gameObjects.emplace(flatVase.getId(), std::move(flatVase));
}

void FirstApp::update()
{
    auto newTime = std::chrono::high_resolution_clock::now();
    float frameTime
        = std::chrono::duration<float, std::chrono::seconds::period>(newTime - currentTime).count();
    currentTime = newTime;

    camera.setViewYXZ(viewerObject.transform.translation, viewerObject.transform.rotation);

    float aspect = lkeRenderer.getAspectRatio();
    camera.setPerspectiveProjection(50.f * Kore::pi / 180, aspect, 0.1f, 100.f);

    if (auto commandList = lkeRenderer.beginFrame())
    {
        int frameIndex = lkeRenderer.getFrameIndex();
        FrameInfo frameInfo{ frameIndex, frameTime, commandList, camera, gameObjects };

        // update
        // nothing...

        // render
        lkeRenderer.beginFrameBufferRenderPass(commandList);

        // order here matters
        simpleRenderSystem.renderGameObjects(frameInfo);

        lkeRenderer.endFrameBufferRenderPass(commandList);
        lkeRenderer.endFrame();
    }
}
}
