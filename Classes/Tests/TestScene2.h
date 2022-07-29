#ifndef __TESTSCENE_2_H__
#define __TESTSCENE_2_H__

#include "cocos2d.h"

class TestScene2 : public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();

    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
    
    // implement the "static create()" method manually
    CREATE_FUNC(TestScene2);
};

#endif // __TESTSCENE_2_H__
