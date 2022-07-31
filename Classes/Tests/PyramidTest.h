#ifndef __PYRAMID_TEST_H__
#define __PYRAMID_TEST_H__

#include "cocos2d.h"
#include "box2d/include/box2d/box2d.h"
#include "Phys/B2_PhysicsBody.h"

USING_NS_CC;

namespace rb{

    class B2WorldNode;
    class B2PhysicsBody;
}

class PyramidTest : public cocos2d::Scene
{
private:
    Size visibleSize;
    Vec2 origin;
    cocos2d::Vec2 m_mouseWorld;
    b2MouseJoint* m_mouseJoint;
    b2Body* m_groundBody;
    rb::B2PhysicsBody* m_groundBody1;
    float ptM;
    rb::B2WorldNode* wN;
    rb::B2PhysicsBody* pB;
public:
    static cocos2d::Scene* createScene();

    virtual bool init();
    virtual void update(float delta) override;

    cocos2d::Node* addBoxAtPosition(cocos2d::Vec2 pos);

    //Mouse events

    void onMouseUp(cocos2d::EventMouse*);
    void onMouseMove(cocos2d::EventMouse*);
    void onMouseDown(cocos2d::EventMouse*);

    // implement the "static create()" method manually
    CREATE_FUNC(PyramidTest);
};

#endif // __PYRAMID_TEST_H__
