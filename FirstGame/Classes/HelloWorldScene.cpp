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

CCPoint HelloWorld::convertPoint(CCPoint point){
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();

    CCSize frameSize = glview->getFrameSize();
    if ( fabs(frameSize.width - 1024) < FLT_EPSILON) {
        return ccp(0.9 * point.x + 47, point.y + 100);
    }
    if (fabs(frameSize.width - 1136) < FLT_EPSILON) {
        return ccp(point.x, point.y - 18);
    }
    return point;
}

void HelloWorld::popMole(GameSprite *mole){
    CCMoveBy *moveUp = CCMoveBy::create(0.2, ccp(0, _winSize.height * 0.25));
    CCEaseInOut *easeMoveUp = CCEaseInOut::create(moveUp, 3.0);
    CCAnimation *laughN = CCAnimationCache::sharedAnimationCache()->animationByName("laughAnim");
    laughN->setRestoreOriginalFrame(true);
    CCAnimate *laugh = CCAnimate::create(laughN);
    CCAction *easeMoveDown = easeMoveUp->reverse();
    mole->runAction(CCSequence::create(easeMoveUp, laugh, easeMoveDown, NULL));
}

void HelloWorld::tryPopMoles(float dt){
    GameSprite *mole;
    for (int i = 0; i < 3; i++) {
        mole = (GameSprite *)_moles->objectAtIndex(i);
        if (arc4random() % 3 == 0) {
            if (mole->numberOfRunningActions() == 0) {
                this->popMole(mole);
            }
        }
    }
}


Animation* HelloWorld::animationFromPlist_delay(const char *animPlist, float delay)
{
    ValueVector animImages = FileUtils::getInstance()->getValueVectorFromFile(animPlist);
    Animation *moleAnimation = CCAnimation::create();
    
    for (int i = 1 ; i < animImages.size(); ++i )
    {
        std::string name = animImages.at(i).asString();
        SpriteFrame* sprite = SpriteFrameCache::getInstance()->getSpriteFrameByName(name.c_str());
        moleAnimation->addSpriteFrame(sprite);
    }
    moleAnimation->setDelayPerUnit(delay);
    return moleAnimation;
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
    
    
    // load sprites
    CCSpriteBatchNode *spriteNode = CCSpriteBatchNode::create("sprites.pvr.ccz");
    this->addChild(spriteNode, 0);
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("sprites.plist");
    
    float offset = 155;
    float startPoint = 85 + offset;
    
    
    
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    
    
    
    CCSize frameSize = glview->getFrameSize();
    if ( fabs(frameSize.width - 1024) < FLT_EPSILON) {
        offset = offset * (1024 / 480.0);
        startPoint = startPoint * (1024 / 480.0) ;
    }
    
    // Add mole2
    GameSprite *mole2 = GameSprite::gameSpriteWithFile("mole_1.png");
    mole2->setPosition(HelloWorld::convertPoint(ccp(startPoint, 85)));
    spriteNode->addChild(mole2);
    
    // Add mole1
    GameSprite *mole1 = GameSprite::gameSpriteWithFile("mole_1.png");
    mole1->setPosition(HelloWorld::convertPoint(ccpSub(mole2->getPosition(), ccp(offset, mole2->getPositionY() - 85))));
    spriteNode->addChild(mole1);
    
    // Add mole3
    GameSprite *mole3 = GameSprite::gameSpriteWithFile("mole_1.png");
    mole3->setPosition(HelloWorld::convertPoint(ccpAdd(mole2->getPosition(), ccp(offset, 85 - mole2->getPositionY()))));
    spriteNode->addChild(mole3);
    _moles = CCArray::create(mole1, mole2, mole3, NULL);
    _moles->retain();
    
    
    this->schedule(schedule_selector(HelloWorld::tryPopMoles), 0.5);
    
    hitAnim = this->animationFromPlist_delay("hitAnim.plist", 0.02);
    laughAnim = this->animationFromPlist_delay("laughAnim.plist", 0.1);
    
    CCAnimationCache::getInstance()->addAnimation(hitAnim, "hitAnim");
    CCAnimationCache::getInstance()->addAnimation(laughAnim, "laughAnim");;

    return true;
}

HelloWorld::~HelloWorld()
{
    CC_SAFE_RELEASE_NULL(_moles);
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
