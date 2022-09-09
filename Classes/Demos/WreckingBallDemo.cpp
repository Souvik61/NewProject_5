#include "Demos/WreckingBallDemo.h"
#include "Phys/SuperPhysHelper.h"
#include "Phys/B2_PhysicsBody.h"
#include "Phys/DefHelper.h"
#include "Phys/ShapeHelper.h"
#include "Phys/RbMacros.h"

USING_NS_CC;
using namespace rb;

cocos2d::Scene* WreckingBallDemo::createScene()
{
	return WreckingBallDemo::create();
}

bool WreckingBallDemo::init()
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
		ShapeHelper::initEdgeShapeSetTwoSided(shape, pTM, Vec2(-4000.f, 0.f), Vec2(4000.f, 0));
		ground->createFixture(&shape, 0.0f);

		auto n = Node::create();
		addChild(n);
		n->addComponent(ground);

	}

	{
		b2PolygonShape shape;
		//shape.SetAsBox(0.5f, 0.125f);
		ShapeHelper::initPolygonShapeSetAsBox(shape, pTM, 50.f, 12.5f);

		b2FixtureDef fd;
		fd.shape = &shape;
		fd.density = 20.0f;
		fd.friction = 0.2f;
		fd.filter.categoryBits = 0x0001;
		fd.filter.maskBits = 0xFFFF & ~0x0002;

		b2RevoluteJointDef jd;
		jd.collideConnected = false;

		const int32 N = 10;
		const float y = 15.0f;
		m_distanceJointDef.localAnchorA.Set(0.0f, y);

		B2PhysicsBody* prevBody = ground;
		for (int32 i = 0; i < N; ++i)
		{
			b2BodyDef bd;
			bd.type = b2_dynamicBody;
			//bd.position.Set(0.5f + 1.0f * i, y);
			DefHelper::initWithPos(bd, pTM, Vec2(50.f + 100.f * i, y * pTM));
			if (i == N - 1)
			{
				//bd.position.Set(1.0f * i, y);
				DefHelper::initWithPos(bd, pTM, Vec2(100.f * i, y * pTM));
				bd.angularDamping = 0.4f;
			}

			B2PhysicsBody* body = wN->createPhysicsBodyComp(&bd);

			auto n = Node::create();
			addChild(n);
			n->addComponent(body);

			if (i == N - 1)
			{
				b2CircleShape circleShape;
				circleShape.m_radius = 1.5f;
				b2FixtureDef sfd;
				sfd.shape = &circleShape;
				sfd.density = 100.0f;
				sfd.filter.categoryBits = 0x0002;
				body->createFixture(&sfd);
			}
			else
			{
				body->createFixture(&fd);
			}

			Vec2 anchor(float(i) * pTM, y * pTM);
			//jd.Initialize(prevBody, body, anchor);
			DefHelper::initRevoluteJointDef(pTM, jd, prevBody, body, anchor);

			wN->createJoint(&jd);

			prevBody = body;
		}

		m_distanceJointDef.localAnchorB.SetZero();

		float extraLength = 0.01f;
		m_distanceJointDef.minLength = 0.0f;
		m_distanceJointDef.maxLength = N - 1.0f + extraLength;
		m_distanceJointDef.bodyB = prevBody->getBox2dBody();
	}

	{
		m_distanceJointDef.bodyA = ground->getBox2dBody();
		m_distanceJoint = wN->createJoint(&m_distanceJointDef);
		m_stabilize = true;
	}
	
	return true;
}

void WreckingBallDemo::onRestart()
{
	auto sc = WreckingBallDemo::createScene();
	_director->replaceScene(sc);
}

void WreckingBallDemo::toggleStabilize(bool s)
{
	m_stabilize = !m_stabilize;

	if (m_stabilize == true && m_distanceJoint == nullptr)
	{
		m_distanceJoint = wN->createJoint(&m_distanceJointDef);
	}
	else if (m_stabilize == false && m_distanceJoint != nullptr)
	{
		wN->getB2World()->DestroyJoint(m_distanceJoint);// Impl. destroy joint
		m_distanceJoint = nullptr;
	}

}

void WreckingBallDemo::onKeyDown(cocos2d::EventKeyboard::KeyCode c, cocos2d::Event* eve)
{
	if (c == EventKeyboard::KeyCode::KEY_S)
	{
		toggleStabilize(true);
	}

	BaseDemo::onKeyDown(c, eve);
}
