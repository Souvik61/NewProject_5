#ifndef __WRECKING_BALL_DEMO_H__
#define __WRECKING_BALL_DEMO_H__

#include "cocos2d.h"
#include "box2d/include/box2d/box2d.h"
#include "Demos/BaseDemo.h"
#include "Phys/B2_PhysicsBody.h"

USING_NS_CC;

//Wrecking ball demo
class WreckingBallDemo : public BaseDemo
{
	b2DistanceJointDef m_distanceJointDef;
	b2Joint* m_distanceJoint;
	bool m_stabilize;

public:
	static cocos2d::Scene* createScene();

	virtual bool init() override;

	void onRestart() override;

	void toggleStabilize(bool);

	//Keyboard input
	void onKeyDown(cocos2d::EventKeyboard::KeyCode, cocos2d::Event*) override;
	//void onKeyUp(cocos2d::EventKeyboard::KeyCode, cocos2d::Event*) override;


	CREATE_FUNC(WreckingBallDemo);

};

#endif // __WRECKING_BALL_DEMO_H__
