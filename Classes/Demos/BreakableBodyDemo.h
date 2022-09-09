#ifndef __BREAKABLEBODY_DEMO_H__
#define __BREAKABLEBODY_DEMO_H__

#include "cocos2d.h"
#include "box2d/include/box2d/box2d.h"
#include "Demos/BaseDemo.h"
#include "Phys/B2_PhysicsBody.h"

USING_NS_CC;

//Different box2d body types
class BreakableBodyDemo : public BaseDemo
{
	rb::B2PhysicsBody* m_body1;
	b2Vec2 m_velocity;
	float m_angularVelocity;
	b2PolygonShape m_shape1;
	b2PolygonShape m_shape2;
	b2Fixture* m_piece1;
	b2Fixture* m_piece2;
	bool m_broke;
	bool m_break;
public:
	static cocos2d::Scene* createScene();

	virtual bool init() override;

	void Step(float);
	virtual void PostSolve(b2Contact* contact, const b2ContactImpulse* impulse) override;

	void Break();

	void onRestart() override;

	CREATE_FUNC(BreakableBodyDemo);

};

#endif // __BREAKABLEBODY_DEMO_H__
