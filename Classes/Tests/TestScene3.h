#ifndef __TESTSCENE_3_H__
#define __TESTSCENE_3_H__

#include "cocos2d.h"

class TestScene3 : public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();

    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
    cocos2d::Sprite* addSpriteAtPosition(cocos2d::Vec2 pos);
    
    // implement the "static create()" method manually
    CREATE_FUNC(TestScene3);
};

#endif // __TESTSCENE_3_H__
