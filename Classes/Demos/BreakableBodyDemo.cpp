#include "Demos/BreakableBodyDemo.h"
#include "Phys/SuperPhysHelper.h"
#include "Phys/B2_PhysicsBody.h"
#include "Phys/DefHelper.h"
#include "Phys/ShapeHelper.h"
#include "Phys/RbMacros.h"

USING_NS_CC;
using namespace rb;

Scene* BreakableBodyDemo::createScene()
{
    return BreakableBodyDemo::create();
}

bool BreakableBodyDemo::init()
{
    if (!BaseDemo::init())
    {
        return false;
    }

	wN->getB2World()->SetGravity(b2Vec2(0,-10));
	wN->setStepCallback(CC_CALLBACK_1(BreakableBodyDemo::Step, this));

	// Ground body
	{
		b2BodyDef bd;
		auto groundComp = wN->createPhysicsBodyComp(&bd);

		b2EdgeShape shape;
		ShapeHelper::initEdgeShapeSetTwoSided(shape, 100, Vec2(-4000, 0), Vec2(4000, 0));
		groundComp->createFixture(&shape, 0.0f);

		auto node = Node::create();
		addChild(node);
		node->addComponent(groundComp);
	}

	// Breakable dynamic body
	{
		b2BodyDef bd;
		bd.type = b2_dynamicBody;
		bd.angle = 0.25f * b2_pi;
		DefHelper::initWithPos(bd, pTM, Vec2(0, 4000));
		m_body1 = wN->createPhysicsBodyComp(&bd);
	
		//m_shape1.SetAsBox(0.5f, 0.5f, b2Vec2(-0.5f, 0.0f), 0.0f);
		ShapeHelper::initPolygonShapeWithBox(m_shape1, pTM, 50, 50, Vec2(-50, 0), 0);
		m_piece1 = m_body1->createFixture(&m_shape1, 1.0f);
	
		//m_shape2.SetAsBox(0.5f, 0.5f, b2Vec2(0.5f, 0.0f), 0.0f);
		ShapeHelper::initPolygonShapeWithBox(m_shape2, pTM, 50, 50, Vec2(50, 0), 0);
		m_piece2 = m_body1->createFixture(&m_shape2, 1.0f);

		auto n = Node::create();
		addChild(n);
		n->addComponent(m_body1);

	}
	
	m_break = false;
	m_broke = false;

	//auto comp = SuperPhysHelper::createBox(wN, Size(100, 100));
	//auto node = Node::create();
	//addChild(node);
	//node->addComponent(comp);
	//comp->setPosition(0,100);
    
	return true;
}

void BreakableBodyDemo::PostSolve(b2Contact* contact, const b2ContactImpulse* impulse)
{
	if (m_broke)
	{
		// The body already broke.
		return;
	}
	
	// Should the body break?
	int32 count = contact->GetManifold()->pointCount;
	
	float maxImpulse = 0.0f;
	for (int32 i = 0; i < count; ++i)
	{
		maxImpulse = b2Max(maxImpulse, impulse->normalImpulses[i]);
	}
	
	if (maxImpulse > 40.0f)
	{
		// Flag the body for breaking.
		m_break = true;
	}
	CCLOG("Post Solve");
}

void BreakableBodyDemo::Break()
{
	//Create two bodies from one.
	b2Body* body1 = m_piece1->GetBody();
	b2Vec2 center = body1->GetWorldCenter();
	
	body1->DestroyFixture(m_piece2);
	m_piece2 = NULL;
	
	b2BodyDef bd;
	bd.type = b2_dynamicBody;
	bd.position = body1->GetPosition();
	bd.angle = body1->GetAngle();
	
	B2PhysicsBody* body2 = wN->createPhysicsBodyComp(&bd);
	m_piece2 = body2->createFixture(&m_shape2, 1.0f);
	auto n = Node::create();
	addChild(n);
	n->addComponent(body2);
	
	// Compute consistent velocities for new bodies based on
	// cached velocity.
	b2Vec2 center1 = body1->GetWorldCenter();
	b2Vec2 center2 = body2->getBox2dBody()->GetWorldCenter();
	
	b2Vec2 velocity1 = m_velocity + b2Cross(m_angularVelocity, center1 - center);
	b2Vec2 velocity2 = m_velocity + b2Cross(m_angularVelocity, center2 - center);
	
	body1->SetAngularVelocity(m_angularVelocity);
	body1->SetLinearVelocity(velocity1);
	
	body2->setAngularVelocity(m_angularVelocity);
	body2->setLinearVelocity(B2TOCCVEC2(velocity2));

}

void BreakableBodyDemo::Step(float)
{
	if (m_break)
	{
		Break();
		m_broke = true;
		m_break = false;
	}
	
	// Cache velocities to improve movement on breakage.
	if (m_broke == false)
	{
		m_velocity = m_body1->getBox2dBody()->GetLinearVelocity();
		m_angularVelocity = m_body1->getAngularVelocity();
	}
}