#include "Demos/PinballDemo.h"
#include "Phys/SuperPhysHelper.h"
#include "Phys/B2_PhysicsBody.h"
#include "Phys/DefHelper.h"
#include "Phys/ShapeHelper.h"
#include "Phys/RbMacros.h"

USING_NS_CC;
using namespace rb;

cocos2d::Scene* PinballDemo::createScene()
{
	return PinballDemo::create();
}


bool PinballDemo::init()
{
    if (!BaseDemo::init())
    {
        return false;
    }

	//Init here

	wN->getB2World()->SetGravity(b2Vec2(0,-10));
	wN->setStepCallback(CC_CALLBACK_1(PinballDemo::stepCallback, this));
	
	// Ground body
	B2PhysicsBody* ground = NULL;
	{
		b2BodyDef bd;
		ground = wN->createPhysicsBodyComp(&bd);

		b2Vec2 vs[5];
		vs[0].Set(-8.0f, 6.0f);
		vs[1].Set(-8.0f, 20.0f);
		vs[2].Set(8.0f, 20.0f);
		vs[3].Set(8.0f, 6.0f);
		vs[4].Set(0.0f, -2.0f);

		b2ChainShape loop;
		loop.CreateLoop(vs, 5);
		

		b2FixtureDef fd;
		fd.shape = &loop;
		fd.density = 0.0f;
		ground->createFixture(&fd);

		auto n = Node::create();
		addChild(n);
		n->addComponent(ground);
	}

	// Flippers
	{
		Vec2 p1(-200.0f, 0.0f), p2(200.0f, 0.0f);

		b2BodyDef bd;
		bd.type = b2_dynamicBody;

		DefHelper::initWithPos(bd, pTM, p1);
		B2PhysicsBody* leftFlipper = wN->createPhysicsBodyComp(&bd);

		auto n = Node::create();
		addChild(n);
		n->addComponent(leftFlipper);

		//bd.position = p2;
		DefHelper::initWithPos(bd, pTM, p2);
		B2PhysicsBody* rightFlipper = wN->createPhysicsBodyComp(&bd);

		auto n1 = Node::create();
		addChild(n1);
		n1->addComponent(rightFlipper);

		b2PolygonShape box;
		//box.SetAsBox(1.75f, 0.1f);
		ShapeHelper::initPolygonShapeWithBox(box, pTM, 175.f, 10.f);

		b2FixtureDef fd;
		fd.shape = &box;
		fd.density = 1.0f;

		leftFlipper->createFixture(&fd);
		rightFlipper->createFixture(&fd);

		b2RevoluteJointDef jd;
		jd.bodyA = ground->getBox2dBody();
		jd.localAnchorB.SetZero();
		jd.enableMotor = true;
		jd.maxMotorTorque = 1000.0f;
		jd.enableLimit = true;

		jd.motorSpeed = 0.0f;
		jd.localAnchorA = CCTOB2VEC2(p1);
		jd.bodyB = leftFlipper->getBox2dBody();
		jd.lowerAngle = -30.0f * b2_pi / 180.0f;
		jd.upperAngle = 5.0f * b2_pi / 180.0f;
		m_leftJoint = (b2RevoluteJoint*)wN->createJoint(&jd);

		jd.motorSpeed = 0.0f;
		jd.localAnchorA = CCTOB2VEC2(p2);
		jd.bodyB = rightFlipper->getBox2dBody();
		jd.lowerAngle = -5.0f * b2_pi / 180.0f;
		jd.upperAngle = 30.0f * b2_pi / 180.0f;
		m_rightJoint = (b2RevoluteJoint*)wN->createJoint(&jd);
	}

	// Circle character
	{
		b2BodyDef bd;
		bd.position.Set(1.0f, 15.0f);
		DefHelper::initWithPos(bd, pTM, Vec2(100.f, 1500.f));
		bd.type = b2_dynamicBody;
		bd.bullet = true;

		m_ball = wN->createPhysicsBodyComp(&bd);

		b2CircleShape shape;
		shape.m_radius = 0.2f;

		b2FixtureDef fd;
		fd.shape = &shape;
		fd.density = 1.0f;
		m_ball->createFixture(&fd);

		auto n = Node::create();
		addChild(n);
		n->addComponent(m_ball);
	}

	m_button = false;
	
	return true;
}

void PinballDemo::onRestart()
{
	auto sc = PinballDemo::createScene();
	_director->replaceScene(sc);
}

void PinballDemo::stepCallback(float dt)
{
	if (m_button)
	{
		m_leftJoint->SetMotorSpeed(20.0f);
		m_rightJoint->SetMotorSpeed(-20.0f);
	}
	else
	{
		m_leftJoint->SetMotorSpeed(-10.0f);
		m_rightJoint->SetMotorSpeed(10.0f);
	}
}

void PinballDemo::onKeyDown(cocos2d::EventKeyboard::KeyCode code, cocos2d::Event* eve)
{
	if (code == EventKeyboard::KeyCode::KEY_A)
	{
		m_button = true;
	}
	BaseDemo::onKeyDown(code,eve);
}

void PinballDemo::onKeyUp(cocos2d::EventKeyboard::KeyCode code, cocos2d::Event* eve)
{
	if (code == EventKeyboard::KeyCode::KEY_A)
	{
		m_button = false;
	}
	BaseDemo::onKeyUp(code,eve);
}
