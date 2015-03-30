#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;


/************************************************************************************************
 *                           Function creating scene for game
 ************************************************************************************************/
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


/************************************************************************************************
 *                   Function converts a point in the “playable area”
 *                      to the appropriate screen position on the iPad
 ************************************************************************************************/
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


/************************************************************************************************
 *                              Set that mole can be tapped
 ************************************************************************************************/
void HelloWorld::setTappable(CCNode* sender, void* moleObject){
    GameSprite *mole = (GameSprite *)moleObject;
    mole->setUserData(mole, true);
    CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("laugh.caf");
}


/************************************************************************************************
 *                              Set that mole cannot be tapped
 ************************************************************************************************/
void HelloWorld::unSetTappable(CCNode* sender, void* moleObject){
    GameSprite *mole = (GameSprite *) moleObject;
    mole->setUserData(mole, false);
}

 
/************************************************************************************************
 *                               Function processing user touch
 ************************************************************************************************/
bool HelloWorld::onTouchBegan (cocos2d::Touch *touch, cocos2d::Event *event) {
    printf("Hit!\n");
    CCPoint touchLocation = this->convertToNodeSpace(touch->getLocation());
    for (int i = 0; i < _moles->count(); i++) {
        GameSprite *mole = (GameSprite *)_moles->objectAtIndex(i);
        if (mole->GameSprite::getUserData(mole) == false) {
            continue;
        }
        if (mole->boundingBox().containsPoint(touchLocation)) {
            mole->GameSprite::setUserData(mole, false);
            _score += 10;
            
            mole->stopAllActions();
            CCAnimation *hitN = (CCAnimation *)CCAnimationCache::sharedAnimationCache()->animationByName("hitAnim");
            CCAnimate *hit = CCAnimate::create(hitN);
            CCMoveBy *moveDown = CCMoveBy::create(0.2, ccp(0, -_winSize.height * 0.25));
            CCEaseInOut *easeMoveDown = CCEaseInOut::create(moveDown, 3.0);
            mole->runAction(CCSequence::create(hit, easeMoveDown, NULL));
        }
    }
    
    CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("ow.caf");
    return true;
}


/************************************************************************************************
 *                               Function poping single mole
 ************************************************************************************************/
void HelloWorld::popMole(GameSprite *mole){
    if (_totalSpawns > 50) {
        return;
    }
    _totalSpawns++;
    
    mole->setDisplayFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("mole_1.png"));
    auto *moveUp = CCMoveBy::create(0.2, ccp(0, _winSize.height * 0.25));
    mole->retain();
    auto *setTappable = CCCallFuncND::create(this, callfuncND_selector(HelloWorld::setTappable), mole);
    auto *easeMoveUp = CCEaseInOut::create(moveUp, 3.0);
    //    CCDelayTime *delay = CCDelayTime::create(0.5);
    auto *laughN = CCAnimationCache::sharedAnimationCache()->animationByName("laughAnim");
    laughN->setRestoreOriginalFrame(true);
    auto *laugh = CCAnimate::create(laughN);
    auto *unSetTappable = CCCallFuncND::create(this,callfuncND_selector(HelloWorld::unSetTappable), mole);
    CCAction *easeMoveDown = easeMoveUp->reverse();
    mole->runAction(CCSequence::create(easeMoveUp, setTappable, laugh, unSetTappable, easeMoveDown, NULL));
}


/************************************************************************************************
 *                               Decide if pop moles or not
 ************************************************************************************************/
void HelloWorld::tryPopMoles(float dt){
    if (_gameOver) {
        return;
    }
    _label->setString(CCString::createWithFormat("score: %d", _score)->getCString());
    if (_totalSpawns >= 50) {
        CCSize winSize = CCDirector::sharedDirector()->getWinSize();
        
        CCLabelTTF *goLabel = CCLabelTTF::create("Level Complete", "Verdana",
                                                 converFontSize(48));
        goLabel->setPosition(ccp(winSize.width / 2, winSize.height / 2));
        goLabel->setScale(0.1);
        this->addChild(goLabel, 10);
        goLabel->runAction(CCScaleTo::create(0.5, 1.0));
        
        _gameOver = true;
    }
    
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


/************************************************************************************************
 *                               Convert size of score
 ************************************************************************************************/
float HelloWorld::converFontSize(float fontSize){
    if (_winSize.width >= 1024 ) {
        return fontSize * 2;
    }
    else{
        return fontSize;
    }
}


/************************************************************************************************
 *                               Load animation from plist
 ************************************************************************************************/
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


/************************************************************************************************
 *                               Initialize instance of game
 ************************************************************************************************/
bool HelloWorld::init()
{
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
    
    // Added animation from property list
    hitAnim = this->animationFromPlist_delay("hitAnim.plist", 0.02);
    laughAnim = this->animationFromPlist_delay("laughAnim.plist", 0.1);
    
    CCAnimationCache::getInstance()->addAnimation(hitAnim, "hitAnim");
    CCAnimationCache::getInstance()->addAnimation(laughAnim, "laughAnim");;

    // Adding invoking of single touch
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = CC_CALLBACK_2(HelloWorld::onTouchBegan, this);
    
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    
    // Added label with score in lower right corner
    float margin = 10.0;
    _label = CCLabelTTF::create("score: 0", "Verdana", converFontSize(14.0));
    _label->cocos2d::CCNode::setAnchorPoint(ccp(1, 0));
    _label->setPosition(ccp(_winSize.width - margin, margin));
    this->addChild(_label, 10);
    
    // Added sound to game
    CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect("laugh.caf");
    CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect("ow.caf");
    CocosDenshion::SimpleAudioEngine::sharedEngine()->playBackgroundMusic("whack.caf", true);
    
    return true;
}


/************************************************************************************************
 *                                  Default destructor
 ************************************************************************************************/
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
