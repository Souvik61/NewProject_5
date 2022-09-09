#include "Demos/SliderCrank2Demo.h"
#include "Phys/SuperPhysHelper.h"
#include "Phys/B2_PhysicsBody.h"
#include "Phys/DefHelper.h"
#include "Phys/ShapeHelper.h"
#include "Phys/RbMacros.h"

USING_NS_CC;
using namespace rb;

cocos2d::Scene* SliderCrank2Demo::createScene()
{
	return SliderCrank2Demo::create();
}


bool SliderCrank2Demo::init()
{
	if (!BaseDemo::init())
	{
		return false;
	}

	//Init here

	wN->getB2World()->SetGravity(b2Vec2(0, -10));
	//wN->setStepCallback(CC_CALLBACK_1(PinballDemo::stepCallback, this));

	B2PhysicsBody* ground = NULL;
	{
		b2BodyDef bd;
		ground = wN->createPhysicsBodyComp(&bd);

		b2EdgeShape shape;
		//shape.SetTwoSided(b2Vec2(-40.0f, 0.0f), b2Vec2(40.0f, 0.0f));
		ShapeHelper::initEdgeShapeSetTwoSided(shape, pTM, Vec2(-4000.f, 0.f), Vec2(4000.f, 0.f));
		ground->createFixture(&shape, 0.0f);

		auto n = Node::create();
		addChild(n);
		n->addComponent(ground);
	}

	{
		B2PhysicsBody* prevBody = ground;

		// Define crank.
		{
			b2PolygonShape shape;
			//shape.SetAsBox(0.5f, 2.0f);
			ShapeHelper::initPolygonShapeWithBox(shape, pTM, 50.f, 200.f);

			b2BodyDef bd;
			bd.type = b2_dynamicBody;
			//bd.position.Set(0.0f, 7.0f);
			DefHelper::initWithPos(bd, pTM, Vec2(0, 700.f));
			B2PhysicsBody* body = wN->createPhysicsBodyComp(&bd);
			body->createFixture(&shape, 2.0f);

			b2RevoluteJointDef rjd;
			//rjd.Initialize(prevBody, body, b2Vec2(0.0f, 5.0f));
			DefHelper::initRevoluteJointDef(pTM, rjd, prevBody, body, Vec2(0.f, 500.f));
			rjd.motorSpeed = 1.0f * b2_pi;
			rjd.maxMotorTorque = 10000.0f;
			rjd.enableMotor = true;
			m_joint1 = (b2RevoluteJoint*)wN->createJoint(&rjd);

			prevBody = body;

			auto n = Node::create();
			addChild(n);
			n->addComponent(body);
		}

		// Define follower.
		{
			b2PolygonShape shape;
			//shape.SetAsBox(0.5f, 4.0f);
			ShapeHelper::initPolygonShapeSetAsBox(shape, pTM, 50.f, 400.f);

			b2BodyDef bd;
			bd.type = b2_dynamicBody;
			//bd.position.Set(0.0f, 13.0f);
			DefHelper::initWithPos(bd, pTM, Vec2(0.f, 1300.f));
			B2PhysicsBody* body = wN->createPhysicsBodyComp(&bd);
			body->createFixture(&shape, 2.0f);

			b2RevoluteJointDef rjd;
			//rjd.Initialize(prevBody, body, b2Vec2(0.0f, 9.0f));
			DefHelper::initRevoluteJointDef(pTM, rjd, prevBody, body, Vec2(0.f, 900.f));
			rjd.enableMotor = false;
			wN->createJoint(&rjd);

			prevBody = body;

			auto n = Node::create();
			addChild(n);
			n->addComponent(body);
		}

		// Define piston
		{
			b2PolygonShape shape;
			//shape.SetAsBox(1.5f, 1.5f);
			ShapeHelper::initPolygonShapeSetAsBox(shape, pTM, 150.f, 150.f);

			b2BodyDef bd;
			bd.type = b2_dynamicBody;
			bd.fixedRotation = true;
			//bd.position.Set(0.0f, 17.0f);
			DefHelper::initWithPos(bd, pTM, Vec2(0.f, 1700.f));
			B2PhysicsBody* body = wN->createPhysicsBodyComp(&bd);
			body->createFixture(&shape, 2.0f);

			b2RevoluteJointDef rjd;
			//rjd.Initialize(prevBody, body, b2Vec2(0.0f, 17.0f));
			DefHelper::initRevoluteJointDef(pTM, rjd, prevBody, body, Vec2(0.f, 1700.f));
			wN->createJoint(&rjd);

			b2PrismaticJointDef pjd;
			//pjd.Initialize(ground, body, b2Vec2(0.0f, 17.0f), b2Vec2(0.0f, 1.0f));
			DefHelper::initPrismaticJointDef(pTM, pjd, ground, body, Vec2(0.f, 1700.f), Vec2::UNIT_Y);

			pjd.maxMotorForce = 1000.0f;
			pjd.enableMotor = true;

			m_joint2 = (b2PrismaticJoint*)wN->createJoint(&pjd);

			auto n = Node::create();
			addChild(n);
			n->addComponent(body);
		}

		// Create a payload
		{
			b2PolygonShape shape;
			//shape.SetAsBox(1.5f, 1.5f);
			ShapeHelper::initPolygonShapeSetAsBox(shape, pTM, 150.f, 150.f);

			b2BodyDef bd;
			bd.type = b2_dynamicBody;
			//bd.position.Set(0.0f, 23.0f);
			DefHelper::initWithPos(bd, pTM, Vec2(0.f, 2300.f));

			B2PhysicsBody* body = wN->createPhysicsBodyComp(&bd);
			body->createFixture(&shape, 2.0f);

			auto n = Node::create();
			addChild(n);
			n->addComponent(body);
		}
	}
		return true;
}

void SliderCrank2Demo::onRestart()
{
	auto sc = SliderCrank2Demo::createScene();
	_director->replaceScene(sc);
}

void SliderCrank2Demo::onKeyDown(cocos2d::EventKeyboard::KeyCode c, cocos2d::Event* eve)
{
	switch (c)
	{
	case EventKeyboard::KeyCode::KEY_F:
		m_joint2->EnableMotor(!m_joint2->IsMotorEnabled());
		m_joint2->GetBodyB()->SetAwake(true);
		break;

	case EventKeyboard::KeyCode::KEY_M:
		m_joint1->EnableMotor(!m_joint1->IsMotorEnabled());
		m_joint1->GetBodyB()->SetAwake(true);
		break;
	}

	BaseDemo::onKeyDown(c, eve);
}
