#ifndef __BREAKABLEBODY_DEMO_H__
#define __BREAKABLEBODY_DEMO_H__

#include "cocos2d.h"
#include "box2d/include/box2d/box2d.h"
#include "Demos/BaseDemo.h"

USING_NS_CC;

//Different box2d body types
class BreakableBodyDemo : public BaseDemo
{
public:
	static cocos2d::Scene* createScene();

	virtual bool init() override;

	void PostSolve(b2Contact* contact, const b2ContactImpulse* impulse) override;

	void Break();

	CREATE_FUNC(BreakableBodyDemo);

};

#endif // __BREAKABLEBODY_DEMO_H__
