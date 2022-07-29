#ifndef __TESTSCENE_5_H__
#define __TESTSCENE_5_H__

#include "cocos2d.h"
#include "box2d/include/box2d/box2d.h"
#include "Phys/B2_PhysicsBody.h"

USING_NS_CC;


class TestScene5 : public cocos2d::Scene
{
private:
    Size visibleSize;
    Vec2 origin;
    cocos2d::Vec2 m_mouseWorld;
    b2MouseJoint* m_mouseJoint;
    b2Body* m_groundBody;
    rb::B2PhysicsBody* m_groundBody1;
    float ptM;

public:
    static cocos2d::Scene* createScene();

    virtual bool init();
    virtual void update(float delta) override;

    cocos2d::Sprite* addSpriteAtPosition(cocos2d::Vec2 pos);

    void addCells();
    void addAllJoints();

    void destroyThisBody(uintptr_t bodyAdd);
    void destroyBodyInNextLoop(rb::B2PhysicsBody* body);

    //Mouse events

    void onMouseUp(cocos2d::EventMouse*);
    void onMouseMove(cocos2d::EventMouse*);
    void onMouseDown(cocos2d::EventMouse*);

    // implement the "static create()" method manually
    CREATE_FUNC(TestScene5);
};

#endif // __TESTSCENE_5_H__
