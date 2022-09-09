#include "Demos/TheoJansenDemo.h"
#include "Phys/SuperPhysHelper.h"
#include "Phys/B2_PhysicsBody.h"
#include "Phys/DefHelper.h"
#include "Phys/ShapeHelper.h"
#include "Phys/RbMacros.h"

USING_NS_CC;
using namespace rb;

cocos2d::Scene* TheoJansenDemo::createScene()
{
	return TheoJansenDemo::create();
}

bool TheoJansenDemo::init()
{
	if (!BaseDemo::init())
	{
		return false;
	}

	//Init here

	wN->getB2World()->SetGravity(b2Vec2(0, -10));
	//wN->setStepCallback(CC_CALLBACK_1(PinballDemo::stepCallback, this));

	//m_offset.Set(0.0f, 8.0f);
	m_offset.set(0.0f, 800.0f);
	m_motorSpeed = 2.0f;
	m_motorOn = true;
	Vec2 pivot(0.0f, 80.f);

	// Ground
	{
		b2BodyDef bd;
		B2PhysicsBody* ground = wN->createPhysicsBodyComp(&bd);

		b2EdgeShape shape;
		//shape.SetTwoSided(b2Vec2(-50.0f, 0.0f), b2Vec2(50.0f, 0.0f));
		ShapeHelper::initEdgeShapeSetTwoSided(shape, pTM, Vec2(-5000.f, 0), Vec2(5000.f, 0));
		ground->createFixture(&shape, 0.0f);

		//shape.SetTwoSided(b2Vec2(-50.0f, 0.0f), b2Vec2(-50.0f, 10.0f));
		ShapeHelper::initEdgeShapeSetTwoSided(shape, pTM, Vec2(-5000.f, 0), Vec2(-5000.f, 1000));
		ground->createFixture(&shape, 0.0f);

		//shape.SetTwoSided(b2Vec2(50.0f, 0.0f), b2Vec2(50.0f, 10.0f));
		ShapeHelper::initEdgeShapeSetTwoSided(shape, pTM, Vec2(5000.f, 0), Vec2(5000.f, 1000));
		ground->createFixture(&shape, 0.0f);

		auto n = Node::create();
		addChild(n);
		n->addComponent(ground);
	}

	// Balls
	for (int32 i = 0; i < 40; ++i)
	{
		b2CircleShape shape;
		shape.m_radius = 0.25f;

		b2BodyDef bd;
		bd.type = b2_dynamicBody;
		//bd.position.Set(-40.0f + 2.0f * i, 0.5f);
		DefHelper::initWithPos(bd, pTM, Vec2(-4000.f + 200 * i, 50.f));

		B2PhysicsBody* body = wN->createPhysicsBodyComp(&bd);
		body->createFixture(&shape, 1.0f);

		auto n = Node::create();
		addChild(n);
		n->addComponent(body);
	}

	// Chassis
	{
		b2PolygonShape shape;
		//shape.SetAsBox(2.5f, 1.0f);
		ShapeHelper::initPolygonShapeSetAsBox(shape, pTM, 250, 100);

		b2FixtureDef sd;
		sd.density = 1.0f;
		sd.shape = &shape;
		sd.filter.groupIndex = -1;
		b2BodyDef bd;
		bd.type = b2_dynamicBody;

		auto v = pivot + m_offset;

		bd.position = CCTOB2VEC2(v);
		m_chassis = wN->createPhysicsBodyComp(&bd);
		m_chassis->createFixture(&sd);

		auto n = Node::create();
		addChild(n);
		n->addComponent(m_chassis);
	}

	{
		b2CircleShape shape;
		shape.m_radius = 1.6f;

		b2FixtureDef sd;
		sd.density = 1.0f;
		sd.shape = &shape;
		sd.filter.groupIndex = -1;
		b2BodyDef bd;
		bd.type = b2_dynamicBody;
		
		auto v = pivot + m_offset;

		bd.position = CCTOB2VEC2(v);
		m_wheel = wN->createPhysicsBodyComp(&bd);
		m_wheel->createFixture(&sd);

		auto n = Node::create();
		addChild(n);
		n->addComponent(m_wheel);
	}

	{
		b2RevoluteJointDef jd;
		//jd.Initialize(m_wheel, m_chassis, pivot + m_offset);
		DefHelper::initRevoluteJointDef(pTM, jd, m_wheel, m_chassis, pivot + m_offset);
		jd.collideConnected = false;
		jd.motorSpeed = m_motorSpeed;
		jd.maxMotorTorque = 400.0f;
		jd.enableMotor = m_motorOn;
		m_motorJoint = (b2RevoluteJoint*)wN->createJoint(&jd);
	}

	Vec2 wheelAnchor;

	//wheelAnchor = pivot + b2Vec2(0.0f, -0.8f);
	wheelAnchor = pivot + Vec2(0.0f, -80.f);

	createLeg(-1.0f, CCTOB2VEC2(wheelAnchor));
	createLeg(1.0f, CCTOB2VEC2(wheelAnchor));

	m_wheel->setTransform(m_wheel->getPosition(), 120.0f * b2_pi / 180.0f);
	createLeg(-1.0f, CCTOB2VEC2(wheelAnchor));
	createLeg(1.0f, CCTOB2VEC2(wheelAnchor));

	m_wheel->setTransform(m_wheel->getPosition(), -120.0f * b2_pi / 180.0f);
	createLeg(-1.0f, CCTOB2VEC2(wheelAnchor));
	createLeg(1.0f, CCTOB2VEC2(wheelAnchor));

	return true;
}

void TheoJansenDemo::onRestart()
{
	auto sc = TheoJansenDemo::createScene();
	_director->replaceScene(sc);
}

void TheoJansenDemo::createLeg(float s, const b2Vec2& wheelAnchor)
{
	b2Vec2 p1(5.4f * s, -6.1f);
	b2Vec2 p2(7.2f * s, -1.2f);
	b2Vec2 p3(4.3f * s, -1.9f);
	b2Vec2 p4(3.1f * s, 0.8f);
	b2Vec2 p5(6.0f * s, 1.5f);
	b2Vec2 p6(2.5f * s, 3.7f);

	b2FixtureDef fd1, fd2;
	fd1.filter.groupIndex = -1;
	fd2.filter.groupIndex = -1;
	fd1.density = 1.0f;
	fd2.density = 1.0f;

	b2PolygonShape poly1, poly2;

	if (s > 0.0f)
	{
		b2Vec2 vertices[3];

		vertices[0] = p1;
		vertices[1] = p2;
		vertices[2] = p3;
		poly1.Set(vertices, 3);

		vertices[0] = b2Vec2_zero;
		vertices[1] = p5 - p4;
		vertices[2] = p6 - p4;
		poly2.Set(vertices, 3);
	}
	else
	{
		b2Vec2 vertices[3];

		vertices[0] = p1;
		vertices[1] = p3;
		vertices[2] = p2;
		poly1.Set(vertices, 3);

		vertices[0] = b2Vec2_zero;
		vertices[1] = p6 - p4;
		vertices[2] = p5 - p4;
		poly2.Set(vertices, 3);
	}

	fd1.shape = &poly1;
	fd2.shape = &poly2;

	b2BodyDef bd1, bd2;
	//bd1.type = b2_staticBody;
	//bd2.type = b2_staticBody;
	
	bd1.type = b2_dynamicBody;
	bd2.type = b2_dynamicBody;


	//bd1.position = m_offset;
	DefHelper::initWithPos(bd1, pTM, m_offset);
	//bd2.position = p4 + m_offset;
	DefHelper::initWithPos(bd2, pTM, B2TOCCVEC2(p4) + m_offset);


	bd1.angularDamping = 10.0f;
	bd2.angularDamping = 10.0f;

	B2PhysicsBody* body1 = wN->createPhysicsBodyComp(&bd1);
	B2PhysicsBody* body2 = wN->createPhysicsBodyComp(&bd2);

	body1->createFixture(&fd1);
	body2->createFixture(&fd2);

	auto n = Node::create();
	addChild(n);
	n->addComponent(body1);

	auto n1 = Node::create();
	addChild(n1);
	n1->addComponent(body2);

	{
		b2DistanceJointDef jd;

		// Using a soft distance constraint can reduce some jitter.
		// It also makes the structure seem a bit more fluid by
		// acting like a suspension system.
		float dampingRatio = 0.5f;
		float frequencyHz = 10.0f;

		//jd.Initialize(body1, body2, p2 + m_offset, p5 + m_offset);
		auto v = B2TOCCVEC2(p2) + m_offset;
		auto v1 = B2TOCCVEC2(p5) + m_offset;
		DefHelper::initDistanceJointDef(pTM, jd, body1, body2, v, v1);
		b2LinearStiffness(jd.stiffness, jd.damping, frequencyHz, dampingRatio, jd.bodyA, jd.bodyB);
		wN->createJoint(&jd);

		//jd.Initialize(body1, body2, p3 + m_offset, p4 + m_offset);
		auto v2 = B2TOCCVEC2(p3) + m_offset;
		auto v3 = B2TOCCVEC2(p4) + m_offset;
		DefHelper::initDistanceJointDef(pTM, jd, body1, body2, v2, v3);

		b2LinearStiffness(jd.stiffness, jd.damping, frequencyHz, dampingRatio, jd.bodyA, jd.bodyB);
		wN->createJoint(&jd);

		//jd.Initialize(body1, m_wheel, p3 + m_offset, wheelAnchor + m_offset);
		auto v4 = B2TOCCVEC2(p3) + m_offset;
		auto v5 = B2TOCCVEC2(wheelAnchor) + m_offset;
		DefHelper::initDistanceJointDef(pTM, jd, body1, m_wheel, v4, v5);

		b2LinearStiffness(jd.stiffness, jd.damping, frequencyHz, dampingRatio, jd.bodyA, jd.bodyB);
		wN->createJoint(&jd);

		//jd.Initialize(body2, m_wheel, p6 + m_offset, wheelAnchor + m_offset);
		auto v6 = B2TOCCVEC2(p6) + m_offset;
		auto v7 = B2TOCCVEC2(wheelAnchor) + m_offset;
		DefHelper::initDistanceJointDef(pTM, jd, body2, m_wheel, v6, v7);
		b2LinearStiffness(jd.stiffness, jd.damping, frequencyHz, dampingRatio, jd.bodyA, jd.bodyB);
		wN->createJoint(&jd);
	}

	{
		b2RevoluteJointDef jd;
		//jd.Initialize(body2, m_chassis, p4 + m_offset);
		auto v = B2TOCCVEC2(p4) + m_offset;
		DefHelper::initRevoluteJointDef(pTM, jd, body2, m_chassis, v);
		wN->createJoint(&jd);
	}
}

void TheoJansenDemo::onKeyDown(cocos2d::EventKeyboard::KeyCode c, cocos2d::Event* eve)
{
	switch (c)
	{
	case EventKeyboard::KeyCode::KEY_A:
		m_motorJoint->SetMotorSpeed(-m_motorSpeed);
		break;

	case EventKeyboard::KeyCode::KEY_S:
		m_motorJoint->SetMotorSpeed(0.0f);
		break;

	case EventKeyboard::KeyCode::KEY_D:
		m_motorJoint->SetMotorSpeed(m_motorSpeed);
		break;

	case EventKeyboard::KeyCode::KEY_M:
		m_motorJoint->EnableMotor(!m_motorJoint->IsMotorEnabled());
		break;
	}

	BaseDemo::onKeyDown(c, eve);
}
