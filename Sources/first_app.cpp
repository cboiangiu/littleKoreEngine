#include <Kore/System.h>
#include <Kore/Window.h>
#include <Kore/Log.h>
#include <Kore/Display.h>
#include <Kore/Input/Surface.h>

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
#if defined(KORE_ANDROID) || defined(KORE_IOS)
    Kore::Display::init();
    auto primaryDisplay = Kore::Display::primary();
    auto width = primaryDisplay->width();
    auto height = primaryDisplay->height();
    
    lkeWindow.resizeWindow(width, height);
#endif
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
    Kore::Surface::the()->TouchStart = touchStartCallback;
    Kore::Surface::the()->Move = touchMoveCallback;
    Kore::Surface::the()->TouchEnd = touchEndCallback;
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
    FirstApp::instance()->lkeWindow.resizeWindow(x,y);
}

void FirstApp::touchStartCallback(int index, int x, int y)
{
    FirstApp::instance()->fingersPressed[index].x() = x+0.f;
    FirstApp::instance()->fingersPressed[index].y() = y+0.f;
}

void FirstApp::touchMoveCallback(int index, int x, int y)
{
    FirstApp::instance()->fingersPressed[index].x() = x+0.f;
    FirstApp::instance()->fingersPressed[index].y() = y+0.f;
}

void FirstApp::touchEndCallback(int index, int x, int y)
{
    FirstApp::instance()->fingersPressed.erase(index);
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
    
#if defined(KORE_ANDROID) || defined(KORE_IOS)
    float joystickSmallRadius = 0.06 * Kore::min(lkeWindow.getExtent().width, lkeWindow.getExtent().height);
    float joystickBigRadius = 0.16 * Kore::min(lkeWindow.getExtent().width, lkeWindow.getExtent().height);
    
    auto joystick1Axis = Kore::vec2{};
    auto joystick2Axis = Kore::vec2{};
    {
        Kore::vec2 center = {lkeWindow.getExtent().width * .22f, lkeWindow.getExtent().height * .74f};
        Kore::vec2 stickCenter = center;
        
        for(auto finger : fingersPressed)
        {
            auto distance = center.distance(finger.second);
            if(distance < joystickBigRadius)
            {
                stickCenter = finger.second;

                joystick1Axis.x() = LkeUtils::smoothstep(0.0, joystickBigRadius, Kore::abs(center.x() - stickCenter.x()));
                joystick1Axis.y() = LkeUtils::smoothstep(0.0, joystickBigRadius, Kore::abs(center.y() - stickCenter.y()));
                if(stickCenter.x() - center.x() < 0)
                {
                    joystick1Axis.x() *= -1.f;
                }
                if(center.y() - stickCenter.y() < 0)
                {
                    joystick1Axis.y() *= -1.f;
                }
                Kore::log(Kore::LogLevel::Info, ("Touch Move: " + std::to_string(0) + " " + std::to_string(joystick1Axis.x()) + " " + std::to_string(joystick1Axis.y())).c_str());
            }
        }
        
        touchGamepadObject.joystick.background1Position = center;
        touchGamepadObject.joystick.background1Radius = joystickBigRadius;
        touchGamepadObject.joystick.stick1Position = stickCenter;
        touchGamepadObject.joystick.stick1Radius = joystickSmallRadius;
    }
    {
        Kore::vec2 center = {lkeWindow.getExtent().width * .78f, lkeWindow.getExtent().height * .74f};
        Kore::vec2 stickCenter = center;
        
        for(auto finger : fingersPressed)
        {
            auto distance = center.distance(finger.second);
            if(distance < joystickBigRadius)
            {
                stickCenter = finger.second;

                joystick2Axis.x() = LkeUtils::smoothstep(0.0, joystickBigRadius, Kore::abs(center.x() - stickCenter.x()));
                joystick2Axis.y() = LkeUtils::smoothstep(0.0, joystickBigRadius, Kore::abs(center.y() - stickCenter.y()));
                if(stickCenter.x() - center.x() < 0)
                {
                    joystick2Axis.x() *= -1.f;
                }
                if(center.y() - stickCenter.y() < 0)
                {
                    joystick2Axis.y() *= -1.f;
                }
                Kore::log(Kore::LogLevel::Info, ("Touch Move: " + std::to_string(1) + " " + std::to_string(joystick2Axis.x()) + " " + std::to_string(joystick2Axis.y())).c_str());
            }
        }
        
        touchGamepadObject.joystick.background2Position = center;
        touchGamepadObject.joystick.background2Radius = joystickBigRadius;
        touchGamepadObject.joystick.stick2Position = stickCenter;
        touchGamepadObject.joystick.stick2Radius = joystickSmallRadius;
    }
    
    cameraController.moveInPlaneXZFromGamepad(joystick1Axis, joystick2Axis, frameTime, viewerObject);
#endif
    
    if (auto commandList = lkeRenderer.beginFrame())
    {
        int frameIndex = lkeRenderer.getFrameIndex();
        FrameInfo frameInfo{ frameIndex, frameTime, commandList, camera, gameObjects, touchGamepadObject, lkeWindow.getExtent() };

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
