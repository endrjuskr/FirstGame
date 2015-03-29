#include "AppDelegate.h"
#include "HelloWorldScene.h"

USING_NS_CC;

AppDelegate::AppDelegate() {

}

AppDelegate::~AppDelegate() 
{
}

//if you want a different context,just modify the value of glContextAttrs
//it will takes effect on all platforms
void AppDelegate::initGLContextAttrs()
{
    //set OpenGL context attributions,now can only set six attributions:
    //red,green,blue,alpha,depth,stencil
    GLContextAttrs glContextAttrs = {8, 8, 8, 8, 24, 8};

    GLView::setGLContextAttrs(glContextAttrs);
}

bool AppDelegate::applicationDidFinishLaunching() {
    // initialize director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if(!glview) {
        glview = GLViewImpl::create("My Game");
        director->setOpenGLView(glview);
    }

    
    auto screenSize = glview->getFrameSize();
    auto *pFileUtils = CCFileUtils::sharedFileUtils();
    std::vector<std::string> searchPaths;
    
    auto designSize = CCSizeMake(480, 320);
    CCSize resourceSize;
    
    // if the device is iPad
    if (screenSize.height >= 768) {
        searchPaths.push_back("hd");
        searchPaths.push_back("sd");
        
        resourceSize = CCSizeMake(1024, 768);
        designSize = CCSizeMake(1024, 768);
    }
    // if the device is iPhone
    else{
        // for retina iPhone
        if (screenSize.height > 320) {
            searchPaths.push_back("hd");
            searchPaths.push_back("sd");
            resourceSize = CCSizeMake(960, 640);
        }
        else{
            searchPaths.push_back("sd");
            resourceSize = CCSizeMake(480, 320);
        }
    }
    searchPaths.push_back("FirstGame");
    pFileUtils->setSearchPaths(searchPaths);
    director->setContentScaleFactor(resourceSize.width / designSize.width);
    
    glview->setDesignResolutionSize(designSize.width, designSize.height, kResolutionFixedWidth);

    
    // turn on display FPS
    director->setDisplayStats(true);

    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0 / 60);

    // create a scene. it's an autorelease object
    auto scene = HelloWorld::createScene();

    // run
    director->runWithScene(scene);

    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();

    // if you use SimpleAudioEngine, it must be pause
    // SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();

    // if you use SimpleAudioEngine, it must resume here
    // SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}
