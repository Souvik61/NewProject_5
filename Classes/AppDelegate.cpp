#include "AppDelegate.h"
#include "HelloWorldScene.h"
#include "Tests/TestScene1.h"
#include "Tests/TestScene4.h"
#include "Tests/TestScene5.h"
#include "Tests/TestScene6.h"
#include "Tests/PyramidTest.h"
#include "Tests/BoxStackTest.h"
#include "Tests/CircleStackTest.h"
#include "Tests/AddPairTest.h"
#include "Demos/BaseDemo.h"
#include "Demos/BodyTypesDemo.h"
#include "Demos/BreakableBodyDemo.h"
#include "Demos/ConveyorBeltDemo.h"
#include "Demos/DominosDemo.h"
#include "Demos/PinballDemo.h"
#include "Demos/SliderCrank2Demo.h"
#include "Demos/TheoJansenDemo.h"
#include "Demos/WreckingBallDemo.h"

// #define USE_AUDIO_ENGINE 1

#if USE_AUDIO_ENGINE
#include "audio/include/AudioEngine.h"
using namespace cocos2d::experimental;
#endif

USING_NS_CC;

static cocos2d::Size designResolutionSize = cocos2d::Size(480, 320);
static cocos2d::Size smallResolutionSize = cocos2d::Size(480, 320);
static cocos2d::Size mediumResolutionSize = cocos2d::Size(924, 600);
static cocos2d::Size largeResolutionSize = cocos2d::Size(2048, 1536);
static cocos2d::Size hdResolutionSize = cocos2d::Size(1280, 720);

AppDelegate::AppDelegate()
{
}

AppDelegate::~AppDelegate() 
{
#if USE_AUDIO_ENGINE
    AudioEngine::end();
#endif
}

// if you want a different context, modify the value of glContextAttrs
// it will affect all platforms
void AppDelegate::initGLContextAttrs()
{
    // set OpenGL context attributes: red,green,blue,alpha,depth,stencil,multisamplesCount
    GLContextAttrs glContextAttrs = {8, 8, 8, 8, 24, 8, 0};

    GLView::setGLContextAttrs(glContextAttrs);
}

// if you want to use the package manager to install more packages,  
// don't modify or remove this function
static int register_all_packages()
{
    return 0; //flag for packages manager
}

bool AppDelegate::applicationDidFinishLaunching() {
    // initialize director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if (!glview) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) || (CC_TARGET_PLATFORM == CC_PLATFORM_MAC) || (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)
        glview = GLViewImpl::createWithRect("NewProject_5", cocos2d::Rect(0, 0, mediumResolutionSize.width, mediumResolutionSize.height));
#else
        glview = GLViewImpl::create("NewProject_5");
#endif
        director->setOpenGLView(glview);
    }

    // turn on display FPS
    director->setDisplayStats(true);

    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0f / 60);

    // Set the design resolution
    glview->setDesignResolutionSize(hdResolutionSize.width, hdResolutionSize.height, ResolutionPolicy::NO_BORDER);

    register_all_packages();

    // create a scene. it's an autorelease object
    //auto scene = HelloWorld::createScene();
    //auto scene = TestScene1::createScene();
    //auto scene = TestScene2::createScene();
    //auto scene = TestScene3::createScene();
    //auto scene = TestScene4::createScene();
    //auto scene = TestScene5::createScene();
    //auto scene = PyramidTest::createScene();
    //auto scene = BoxStackTest::createScene();
    //auto scene = CircleStackTest::createScene();
    //auto scene = AddPairTest::createScene();
    //auto scene = BaseDemo::createScene();
    //auto scene = BodyTypesDemo::createScene();
    //auto scene = BreakableBodyDemo::createScene();
    //auto scene = ConveyorBeltDemo::createScene();
    //auto scene = DominosDemo::createScene();
    //auto scene = PinballDemo::createScene();
    //auto scene = SliderCrank2Demo::createScene();
    //auto scene = TheoJansenDemo::createScene();
    auto scene = WreckingBallDemo::createScene();

    // run
    director->runWithScene(scene);

    return true;
}

// This function will be called when the app is inactive. Note, when receiving a phone call it is invoked.
void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();

#if USE_AUDIO_ENGINE
    AudioEngine::pauseAll();
#endif
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();

#if USE_AUDIO_ENGINE
    AudioEngine::resumeAll();
#endif
}
