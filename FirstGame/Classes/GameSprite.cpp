//
//  GameSprite.cpp
//  FirstGame
//
//  Created by Konrad Lisiecki on 29/03/15.
//
//

#include "GameSprite.h"

GameSprite::GameSprite(){
    
}

GameSprite::~GameSprite(){
    
}

GameSprite* GameSprite::gameSpriteWithFile(const char *pszFileName){
    GameSprite *sprite = new GameSprite();
    if (sprite && sprite->initWithSpriteFrameName(pszFileName)) {
        sprite->autorelease();
        return sprite;
    }
    CC_SAFE_DELETE(sprite);
    return NULL;
}

void GameSprite::setUserData(GameSprite *mole, bool flag){
    mole->userData = flag;
}

bool GameSprite::getUserData(GameSprite *mole){
    return mole->userData;
}