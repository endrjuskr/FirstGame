#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "GameSprite.h"

USING_NS_CC;

class HelloWorld : public cocos2d::Layer
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
    
    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);
    
    CCPoint convertPoint(CCPoint point);
    
    void tryPopMoles(float dt);
    void popMole(GameSprite *mole);
    
    ~HelloWorld();
    Animation* animationFromPlist_delay(const char *animPlist, float delay);
    
    float converFontSize(float fontSize);
    
    
    void setTappable(cocos2d::Node * sender, void * moleObject);
    void unSetTappable(cocos2d::Node * sender, void * moleObject);
    
    bool onTouchBegan (cocos2d::Touch *touch, cocos2d::Event *event);
    
private:
    CCSize _winSize;
    CCArray *_moles;
    
    // Inside head file of HelloWorld
    Animation*	laughAnim;
    Animation*	hitAnim;
    
    CCLabelTTF*     _label;
    int             _score;
    int             _totalSpawns;
    bool            _gameOver;
};

#endif // __HELLOWORLD_SCENE_H__
