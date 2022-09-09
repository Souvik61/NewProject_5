#ifndef __SLIDER_CRANK2_DEMO_H__
#define __SLIDER_CRANK2_DEMO_H__

#include "cocos2d.h"
#include "box2d/include/box2d/box2d.h"
#include "Demos/BaseDemo.h"
#include "Phys/B2_PhysicsBody.h"

USING_NS_CC;

//Slider crank demo
class SliderCrank2Demo : public BaseDemo
{
	b2RevoluteJoint* m_joint1;
	b2PrismaticJoint* m_joint2;

public:
	static cocos2d::Scene* createScene();

	virtual bool init() override;

	void onRestart() override;

	//Keyboard input
	void onKeyDown(cocos2d::EventKeyboard::KeyCode, cocos2d::Event*) override;
	//void onKeyUp(cocos2d::EventKeyboard::KeyCode, cocos2d::Event*) override;


	CREATE_FUNC(SliderCrank2Demo);

};

#endif // __SLIDER_CRANK2_DEMO_H__
