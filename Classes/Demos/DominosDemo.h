#ifndef __DOMINOS_DEMO_H__
#define __DOMINOS_DEMO_H__

#include "cocos2d.h"
#include "box2d/include/box2d/box2d.h"
#include "Demos/BaseDemo.h"
#include "Phys/B2_PhysicsBody.h"

USING_NS_CC;

//Dominos demo
class DominosDemo : public BaseDemo
{
	b2Fixture* m_platform;

public:
	static cocos2d::Scene* createScene();

	virtual bool init() override;

	CREATE_FUNC(DominosDemo);

};

#endif // __DOMINOS_DEMO_H__
