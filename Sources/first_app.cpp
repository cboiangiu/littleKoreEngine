#include <Kore/System.h>
#include <Kore/Window.h>
#include <Kore/Log.h>

#include "first_app.hpp"
#include "lke_utils.hpp"
#include "lke_frame_info.hpp"

// std
#include <array>
#include <cassert>
#include <stdexcept>
#include <chrono>
#include <thread>

namespace lke
{

FirstApp::FirstApp()
{
    loadGameObjects();
    viewerObject.transform.translation = {0.f,10.f,-15.f};
    viewerObject.transform.rotation = {.7f,0.f,0.f};
}

FirstApp::~FirstApp()
{
}

void FirstApp::run()
{
    Kore::Keyboard::the()->KeyDown = keyDownCallback;
    Kore::Keyboard::the()->KeyUp = keyUpCallback;
    Kore::System::setCallback(updateCallback);
    Kore::System::setPauseCallback(pauseCallback);
    Kore::System::setResumeCallback(resumeCallback);
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

void FirstApp::pauseCallback()
{
    FirstApp::instance()->pause = true;
    FirstApp::instance()->keysPressed.clear();
}

void FirstApp::resumeCallback()
{
    FirstApp::instance()->pause = false;
}

void FirstApp::resizeCallback(int x, int y, void *data)
{
    Kore::log(Kore::LogLevel::Info, (std::to_string(x) + " " + std::to_string(y)).c_str());
    FirstApp::instance()->lkeWindow.resizeWindow(x,y);
}

void FirstApp::loadGameObjects()
{
    std::shared_ptr<LkeModel> lkeModel = LkeModel::createModelFromFile("models/cube.obj");
    auto obj = LkeGameObject::createGameObject();
    obj.model = lkeModel;
    obj.transform.translation = {.0f, .0f, 2.5f};
    obj.transform.scale = {3.f, 3.f, 3.f};
    gameObjects.emplace(obj.getId(), std::move(obj));
    
    // add touchGamepadObject model
    {
        std::vector<LkeModel::Vertex> vertices;
        std::vector<uint32_t> indices;
        
        LkeModel::Vertex vertex{};
        vertex.color = {1,1,0};
        vertex.position = {-1, 1, 0};
        vertices.push_back(vertex);
        vertex.position = {-1, -1, 0};
        vertices.push_back(vertex);
        vertex.position = {1,  -1, 0};
        vertices.push_back(vertex);
        vertex.position = {1, 1, 0};
        vertices.push_back(vertex);
        
        indices = {0,1,2, // first triangle (bottom left - top left - top right)
            0,2,3};

        touchGamepadObject.model = LkeModel::createFromBuffers(vertices, indices);
    }
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
    
    float joystickSmallRadius = 0.06 * lkeWindow.getExtent().height;
    float joystickBigRadius = 0.16 * lkeWindow.getExtent().height;
    {
        Kore::vec2 center = {lkeWindow.getExtent().width * .22f, lkeWindow.getExtent().height * .74f};
        touchGamepadObject.joystick.background1Position = center;
        touchGamepadObject.joystick.background1Radius = joystickBigRadius;
        touchGamepadObject.joystick.stick1Position = center - Kore::vec2{joystickBigRadius, joystickBigRadius} / 2;
        touchGamepadObject.joystick.stick1Radius = joystickSmallRadius;
    }
    {
        Kore::vec2 center = {lkeWindow.getExtent().width * .78f, lkeWindow.getExtent().height * .74f};
        touchGamepadObject.joystick.background2Position = center;
        touchGamepadObject.joystick.background2Radius = joystickBigRadius;
        touchGamepadObject.joystick.stick2Position = center;
        touchGamepadObject.joystick.stick2Radius = joystickSmallRadius;
    }

    if (auto commandList = lkeRenderer.beginFrame())
    {
        int frameIndex = lkeRenderer.getFrameIndex();
        FrameInfo frameInfo{ frameIndex, frameTime, commandList, camera, gameObjects, touchGamepadObject };

        // update
        // nothing...

        // render
        lkeRenderer.beginFrameBufferRenderPass(commandList);

        // order here matters
        simpleRenderSystem.renderGameObjects(frameInfo);

        lkeRenderer.endFrameBufferRenderPass(commandList);
        lkeRenderer.endFrame();
    }
    
    if (pause) {
        newTime = std::chrono::high_resolution_clock::now();
        int sleepMs = 15 - std::chrono::duration<float, std::chrono::milliseconds::period>(newTime - currentTime).count();
        if(sleepMs < 0) {
            sleepMs = 0;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(sleepMs));
    }
}
}
