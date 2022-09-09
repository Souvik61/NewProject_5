#ifndef __PINBALL_DEMO_H__
#define __PINBALL__DEMO_H__

#include "cocos2d.h"
#include "box2d/include/box2d/box2d.h"
#include "Demos/BaseDemo.h"
#include "Phys/B2_PhysicsBody.h"

USING_NS_CC;

//Pinball demo
class PinballDemo : public BaseDemo
{
	b2RevoluteJoint* m_leftJoint;
	b2RevoluteJoint* m_rightJoint;
	rb::B2PhysicsBody* m_ball;
	bool m_button;

public:
	static cocos2d::Scene* createScene();

	virtual bool init() override;

	void onRestart() override;

	void stepCallback(float);

	//Keyboard input
	void onKeyDown(cocos2d::EventKeyboard::KeyCode, cocos2d::Event*) override;
	void onKeyUp(cocos2d::EventKeyboard::KeyCode, cocos2d::Event*) override;


	CREATE_FUNC(PinballDemo);

};

#endif // __PINBALL__DEMO_H__
