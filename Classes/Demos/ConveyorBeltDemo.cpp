#include "Demos/ConveyorBeltDemo.h"
#include "Phys/SuperPhysHelper.h"
#include "Phys/B2_PhysicsBody.h"
#include "Phys/DefHelper.h"
#include "Phys/ShapeHelper.h"
#include "Phys/RbMacros.h"

USING_NS_CC;
using namespace rb;

cocos2d::Scene* ConveyorBeltDemo::createScene()
{
	return ConveyorBeltDemo::create();
}


bool ConveyorBeltDemo::init()
{
    if (!BaseDemo::init())
    {
        return false;
    }

	//Init here

	wN->getB2World()->SetGravity(b2Vec2(0,-10));
	//wN->setStepCallback(CC_CALLBACK_1(BreakableBodyDemo::Step, this));
	
	// Ground
	{
		b2BodyDef bd;
		B2PhysicsBody* ground = wN->createPhysicsBodyComp(&bd);

		b2EdgeShape shape;
		//shape.SetTwoSided(b2Vec2(-20.0f, 0.0f), b2Vec2(20.0f, 0.0f));
		ShapeHelper::initEdgeShapeSetTwoSided(shape, pTM, Vec2(-2000, 0), Vec2(2000, 0));
		ground->createFixture(&shape, 0.0f);

		auto node = Node::create();
		addChild(node);
		node->addComponent(ground);
	}

	// Platform
	{
		b2BodyDef bd;
		//bd.position.Set(-5.0f, 5.0f);
		DefHelper::initWithPos(bd,pTM,Vec2(-500,500));
		B2PhysicsBody* body = wN->createPhysicsBodyComp(&bd);

		b2PolygonShape shape;
		//shape.SetAsBox(10.0f, 0.5f);
		ShapeHelper::initPolygonShapeWithBox(shape, pTM, 1000, 50);
		b2FixtureDef fd;
		fd.shape = &shape;
		fd.friction = 0.8f;
		m_platform = body->createFixture(&fd);

		auto node = Node::create();
		addChild(node);
		node->addComponent(body);
	}

	// Boxes
	for (int32 i = 0; i < 5; ++i)
	{
		b2BodyDef bd;
		bd.type = b2_dynamicBody;
		//bd.position.Set(-10.0f + 2.0f * i, 7.0f);
		DefHelper::initWithPos(bd,pTM,Vec2(-1000.f + 200 * i, 700));
		B2PhysicsBody* body = wN->createPhysicsBodyComp(&bd);

		b2PolygonShape shape;
		shape.SetAsBox(0.5f, 0.5f);
		ShapeHelper::initPolygonShapeWithBox(shape, pTM, 50, 50);
		body->createFixture(&shape, 20.0f);

		auto n = Node::create();
		addChild(n);
		n->addComponent(body);
	}

    
	return true;
}

void ConveyorBeltDemo::PreSolve(b2Contact* contact, const b2Manifold* oldManifold)
{
	BaseDemo::PreSolve(contact, oldManifold);

	b2Fixture* fixtureA = contact->GetFixtureA();
	b2Fixture* fixtureB = contact->GetFixtureB();

	if (fixtureA == m_platform)
	{
		contact->SetTangentSpeed(5.0f);
	}

	if (fixtureB == m_platform)
	{
		contact->SetTangentSpeed(-5.0f);
	}
	
	//CCLOG("Pre Solve");
}

void ConveyorBeltDemo::onRestart()
{
	auto sc = ConveyorBeltDemo::createScene();
	_director->replaceScene(sc);
}

