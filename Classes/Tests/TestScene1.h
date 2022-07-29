#ifndef __TESTSCENE_1_H__
#define __TESTSCENE_1_H__

#include "cocos2d.h"

class TestScene1 : public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();
    void AddCloseButton();
    void addPhysicsShape();
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
    
    // implement the "static create()" method manually
    CREATE_FUNC(TestScene1);
};

#endif // __TESTSCENE_1_H__
