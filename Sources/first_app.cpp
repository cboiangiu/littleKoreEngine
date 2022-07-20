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
}

FirstApp::~FirstApp()
{
}

void FirstApp::run()
{
    Kore::Keyboard::the()->KeyDown = keyDownCallback;
    Kore::Keyboard::the()->KeyUp = keyUpCallback;
    Kore::System::setCallback(updateCallback);
    // set more callbacks for key events here
    currentTime = std::chrono::high_resolution_clock::now();
    Kore::System::start();
}

void FirstApp::updateCallback()
{
    FirstApp::instance()->update();
}

void FirstApp::keyDownCallback(Kore::KeyCode keyCode)
{
    FirstApp::instance()->keysPressed.insert(keyCode);
}

void FirstApp::keyUpCallback(Kore::KeyCode keyCode)
{
    FirstApp::instance()->keysPressed.erase(keyCode);
}

void FirstApp::loadGameObjects()
{
    std::shared_ptr<LkeModel> lkeModel = LkeModel::createModelFromFile("models/cube.obj");
    auto obj = LkeGameObject::createGameObject();
    obj.model = lkeModel;
    obj.transform.translation = {.0f, .0f, 2.5f};
    obj.transform.scale = {3.f, 3.f, 3.f};
    gameObjects.emplace(obj.getId(), std::move(obj));
}

void FirstApp::update()
{
    auto newTime = std::chrono::high_resolution_clock::now();
    float frameTime
        = std::chrono::duration<float, std::chrono::seconds::period>(newTime - currentTime).count();
    currentTime = newTime;

    cameraController.moveInPlaneXZ(keysPressed, frameTime, viewerObject);
    camera.setViewYXZ(viewerObject.transform.translation, viewerObject.transform.rotation);

    float aspect = lkeRenderer.getAspectRatio();
    camera.setPerspectiveProjection(50.f * Kore::pi / 180, aspect, 0.1f, 500.f);

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
