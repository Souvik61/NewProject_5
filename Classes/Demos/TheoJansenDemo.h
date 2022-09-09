#ifndef __THEO_JANSEN_DEMO_H__
#define __THEO_JANSEN_DEMO_H__

#include "cocos2d.h"
#include "box2d/include/box2d/box2d.h"
#include "Demos/BaseDemo.h"
#include "Phys/B2_PhysicsBody.h"

USING_NS_CC;

//Theo Jansen demo
class TheoJansenDemo : public BaseDemo
{
	Vec2 m_offset;
	rb::B2PhysicsBody* m_chassis;
	rb::B2PhysicsBody* m_wheel;
	b2RevoluteJoint* m_motorJoint;
	bool m_motorOn;
	float m_motorSpeed;

public:
	static cocos2d::Scene* createScene();

	virtual bool init() override;
	void createLeg(float s, const b2Vec2& wheelAnchor);

	void onRestart() override;

	//Keyboard input
	void onKeyDown(cocos2d::EventKeyboard::KeyCode, cocos2d::Event*) override;
	//void onKeyUp(cocos2d::EventKeyboard::KeyCode, cocos2d::Event*) override;


	CREATE_FUNC(TheoJansenDemo);

};

#endif // __THEO_JANSEN_DEMO_H__
