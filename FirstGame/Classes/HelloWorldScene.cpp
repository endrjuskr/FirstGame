#include "HelloWorldScene.h"

USING_NS_CC;

Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    _winSize = CCDirector::sharedDirector()->getWinSize();
    
    // Load background
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("background.plist");
    
    // Load foreround
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("foreground.plist");
    
    // Add background
    CCSprite *dirt = CCSprite::createWithSpriteFrameName("bg_dirt.png");
    dirt->setScale(2.0);
    dirt->setPosition(ccp(_winSize.width * 0.5, _winSize.height * 0.5));
    this->addChild(dirt, -2);
    
    // Add foreground
    CCSprite *lower = CCSprite::createWithSpriteFrameName("grass_lower.png");
    lower->setAnchorPoint(ccp(0.5, 1));
    lower->setPosition(ccp(_winSize.width * 0.5, _winSize.height * 0.5));
    lower->getTexture()->setAliasTexParameters();
    this->addChild(lower, 1);
    
    CCSprite *upper = CCSprite::createWithSpriteFrameName("grass_upper.png");
    upper->setAnchorPoint(ccp(0.5, 0));
    upper->setPosition(ccp(_winSize.width * 0.5, _winSize.height * 0.5));
    upper->getTexture()->setAliasTexParameters();
    this->addChild(upper, -1);
    
    // Add more here later...
    
    return true;
}


void HelloWorld::menuCloseCallback(Ref* pSender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
	MessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
    return;
#endif

    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}
