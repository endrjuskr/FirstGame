//
//  GameSprite.h
//  FirstGame
//
//  Created by Konrad Lisiecki on 29/03/15.
//
//

#ifndef FirstGame_GameSprite_h
#define FirstGame_GameSprite_h

#include <iostream>
#include "cocos2d.h"

using namespace cocos2d;

class GameSprite : public CCSprite {
    CCSize _screenSize;
    bool userData;
    
public:
    GameSprite();
    ~GameSprite();
    
    static GameSprite* gameSpriteWithFile(const char *pszFileName);
    void setUserData(GameSprite *mole, bool flag);
    bool getUserData(GameSprite *mole);
};

#endif
