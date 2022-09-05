#include "Demos/DominosDemo.h"
#include "Phys/SuperPhysHelper.h"
#include "Phys/B2_PhysicsBody.h"
#include "Phys/DefHelper.h"
#include "Phys/ShapeHelper.h"
#include "Phys/RbMacros.h"

USING_NS_CC;
using namespace rb;

cocos2d::Scene* DominosDemo::createScene()
{
	return DominosDemo::create();
}


bool DominosDemo::init()
{
    if (!BaseDemo::init())
    {
        return false;
    }

	//Init here

	wN->getB2World()->SetGravity(b2Vec2(0,-10));
	//wN->setStepCallback(CC_CALLBACK_1(BreakableBodyDemo::Step, this));
	
	B2PhysicsBody* b1;
	{
		b2EdgeShape shape;
		//shape.SetTwoSided(b2Vec2(-40.0f, 0.0f), b2Vec2(40.0f, 0.0f));
		ShapeHelper::initEdgeShapeSetTwoSided(shape, pTM, Vec2(-4000, 0), Vec2(4000, 0));

		b2BodyDef bd;
		b1 = wN->createPhysicsBodyComp(&bd);
		b1->createFixture(&shape, 0.0f);

		auto n = Node::create();
		addChild(n);
		n->addComponent(b1);
	}

	{
		b2PolygonShape shape;
		//shape.SetAsBox(6.0f, 0.25f);
		ShapeHelper::initPolygonShapeWithBox(shape, pTM, 600, 25);

		b2BodyDef bd;
		//bd.position.Set(-1.5f, 10.0f);
		DefHelper::initWithPos(bd, pTM, Vec2(-150, 1000));
		B2PhysicsBody* ground = wN->createPhysicsBodyComp(&bd);
		ground->createFixture(&shape, 0.0f);

		auto n = Node::create();
		addChild(n);
		n->addComponent(ground);

	}

	{
		b2PolygonShape shape;
		//shape.SetAsBox(0.1f, 1.0f);
		ShapeHelper::initPolygonShapeWithBox(shape, pTM, 10, 100);

		b2FixtureDef fd;
		fd.shape = &shape;
		fd.density = 20.0f;
		fd.friction = 0.1f;

		for (int i = 0; i < 10; ++i)
		{
			b2BodyDef bd;
			bd.type = b2_dynamicBody;
			//bd.position.Set(-6.0f + 1.0f * i, 11.25f);
			DefHelper::initWithPos(bd, pTM, Vec2(-600.f + 100.f * i, 1125.f));
			B2PhysicsBody* body = wN->createPhysicsBodyComp(&bd);
			body->createFixture(&fd);

			auto n = Node::create();
			addChild(n);
			n->addComponent(body);
		}
	}

	{
		b2PolygonShape shape;
		//shape.SetAsBox(7.0f, 0.25f, b2Vec2_zero, 0.3f);
		ShapeHelper::initPolygonShapeWithBox(shape, pTM, 700, 25, Vec2::ZERO, 0.3f);

		b2BodyDef bd;
		//bd.position.Set(1.0f, 6.0f);
		DefHelper::initWithPos(bd, pTM, Vec2(100, 600));
		B2PhysicsBody* ground = wN->createPhysicsBodyComp(&bd);
		ground->createFixture(&shape, 0.0f);

		auto n = Node::create();
		addChild(n);
		n->addComponent(ground);
	}

	B2PhysicsBody* b2;
	{
		b2PolygonShape shape;
		//shape.SetAsBox(0.25f, 1.5f);
		ShapeHelper::initPolygonShapeWithBox(shape,pTM,25,150);

		b2BodyDef bd;
		//bd.position.Set(-7.0f, 4.0f);
		DefHelper::initWithPos(bd,pTM,Vec2(-700,400));
		b2 = wN->createPhysicsBodyComp(&bd);
		b2->createFixture(&shape, 0.0f);

		auto n = Node::create();
		addChild(n);
		n->addComponent(b2);
	}

	B2PhysicsBody* b3;
	{
		b2PolygonShape shape;
		//shape.SetAsBox(6.0f, 0.125f);
		ShapeHelper::initPolygonShapeWithBox(shape, pTM, 600, 12.5f);

		b2BodyDef bd;
		bd.type = b2_dynamicBody;
		//bd.position.Set(-0.9f, 1.0f)
		DefHelper::initWithPos(bd, pTM, Vec2(-90, 100.f));
		bd.angle = -0.15f;

		b3 = wN->createPhysicsBodyComp(&bd);
		b3->createFixture(&shape, 10.0f);

		auto n = Node::create();
		addChild(n);
		n->addComponent(b3);
	}

	b2RevoluteJointDef jd;
	Vec2 anchor;

	anchor.set(-200.0f, 100.0f);
	//jd.Initialize(b1, b3, anchor);
	DefHelper::initRevoluteJointDef(pTM, jd, b1, b3, anchor);
	jd.collideConnected = true;
	wN->createJoint(&jd);

	B2PhysicsBody* b4;
	{
		b2PolygonShape shape;
		//shape.SetAsBox(0.25f, 0.25f);
		ShapeHelper::initPolygonShapeWithBox(shape, pTM, 25.f, 25.f);

		b2BodyDef bd;
		bd.type = b2_dynamicBody;
		//bd.position.Set(-10.0f, 15.0f);
		DefHelper::initWithPos(bd, pTM, Vec2(-1000.f, 1500.f));
		b4 = wN->createPhysicsBodyComp(&bd);
		b4->createFixture(&shape, 10.0f);

		auto n = Node::create();
		addChild(n);
		n->addComponent(b4);
	}

	anchor.set(-700.0f, 1500.0f);
	//jd.Initialize(b2, b4, anchor);
	DefHelper::initRevoluteJointDef(pTM, jd, b2, b4, anchor);
	wN->createJoint(&jd);

	B2PhysicsBody* b5;
	{
		b2BodyDef bd;
		bd.type = b2_dynamicBody;
		//bd.position.Set(6.5f, 3.0f);
		DefHelper::initWithPos(bd, pTM, Vec2(650.f, 300.f));
		b5 = wN->createPhysicsBodyComp(&bd);

		auto n = Node::create();
		addChild(n);
		n->addComponent(b5);

		b2PolygonShape shape;
		b2FixtureDef fd;

		fd.shape = &shape;
		fd.density = 10.0f;
		fd.friction = 0.1f;

		//shape.SetAsBox(1.0f, 0.1f, b2Vec2(0.0f, -0.9f), 0.0f);
		ShapeHelper::initPolygonShapeWithBox(shape, pTM, 100, 10, Vec2(0, -90), 0);
		b5->createFixture(&fd);

		//shape.SetAsBox(0.1f, 1.0f, b2Vec2(-0.9f, 0.0f), 0.0f);
		ShapeHelper::initPolygonShapeWithBox(shape, pTM, 10, 100, Vec2(-90.f, 0), 0);
		b5->createFixture(&fd);

		//shape.SetAsBox(0.1f, 1.0f, b2Vec2(0.9f, 0.0f), 0.0f);
		ShapeHelper::initPolygonShapeWithBox(shape, pTM, 10, 100.f, Vec2(90.f, 0), 0);
		b5->createFixture(&fd);
	}

	anchor.set(600.0f, 200.0f);
	//jd.Initialize(b1, b5, anchor);
	DefHelper::initRevoluteJointDef(pTM, jd, b1, b5, anchor);
	wN->createJoint(&jd);

	B2PhysicsBody* b6;
	{
		b2PolygonShape shape;
		//shape.SetAsBox(1.0f, 0.1f);
		ShapeHelper::initPolygonShapeWithBox(shape, pTM, 100.f, 10.f);

		b2BodyDef bd;
		bd.type = b2_dynamicBody;
		//bd.position.Set(6.5f, 4.1f);
		DefHelper::initWithPos(bd, pTM, Vec2(650.f, 410.f));
		b6 = wN->createPhysicsBodyComp(&bd);
		b6->createFixture(&shape, 30.0f);

		auto n = Node::create();
		addChild(n);
		n->addComponent(b6);
	}

	anchor.set(750.f, 400.0f);
	//jd.Initialize(b5, b6, anchor);
	DefHelper::initRevoluteJointDef(pTM, jd, b5, b6, anchor);
	wN->createJoint(&jd);

	B2PhysicsBody* b7;
	{
		b2PolygonShape shape;
		//shape.SetAsBox(0.1f, 1.0f);
		ShapeHelper::initPolygonShapeWithBox(shape, pTM, 10.f, 100.f);

		b2BodyDef bd;
		bd.type = b2_dynamicBody;
		//bd.position.Set(7.4f, 1.0f);
		DefHelper::initWithPos(bd, pTM, Vec2(740.f, 100.f));

		b7 = wN->createPhysicsBodyComp(&bd);
		b7->createFixture(&shape, 10.0f);

		auto n = Node::create();
		addChild(n);
		n->addComponent(b7);
	}

	b2DistanceJointDef djd;
	djd.bodyA = b3->getBox2dBody();
	djd.bodyB = b7->getBox2dBody();
	//djd.localAnchorA.Set(6.0f, 0.0f);
	djd.localAnchorA.Set(6.0f, 0.0f);
	//djd.localAnchorB.Set(0.0f, -1.0f);
	djd.localAnchorB.Set(0.0f, -1.0f);
	b2Vec2 d = djd.bodyB->GetWorldPoint(djd.localAnchorB) - djd.bodyA->GetWorldPoint(djd.localAnchorA);
	djd.length = d.Length();

	b2LinearStiffness(djd.stiffness, djd.damping, 1.0f, 1.0f, djd.bodyA, djd.bodyB);
	wN->createJoint(&djd);

	{
		float radius = 0.2f;

		b2CircleShape shape;
		shape.m_radius = radius;

		for (int32 i = 0; i < 4; ++i)
		{
			b2BodyDef bd;
			bd.type = b2_dynamicBody;
			//bd.position.Set(5.9f + 2.0f * radius * i, 2.4f);
			DefHelper::initWithPos(bd, pTM, Vec2(590.f + 200 * radius * i, 240.f));
			B2PhysicsBody* body = wN->createPhysicsBodyComp(&bd);
			body->createFixture(&shape, 10.0f);

			auto n = Node::create();
			addChild(n);
			n->addComponent(body);
		}
	}
    
	return true;
}
