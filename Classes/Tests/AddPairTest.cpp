#include "Tests/AddPairTest.h"
#include "Phys/B2_WorldNode.h"
#include "Phys/B2_PhysicsBody.h"
#include "box2d/include/box2d/box2d.h"
#include "Phys/DebugDrawNode.h"
#include "Phys/ShapeHelper.h"
#include "Phys/DefHelper.h"
#include "Phys/Utilities.h"
#include "Phys/SuperPhysHelper.h"
#include "Phys/B2_PhysicsBody.h"

USING_NS_CC;
using namespace rb;

namespace {
	class QueryCallback1 : public b2QueryCallback
	{
	public:
		QueryCallback1(const b2Vec2& point)
		{
			m_point = point;
			m_fixture = NULL;
		}

		bool ReportFixture(b2Fixture* fixture) override
		{
			b2Body* body = fixture->GetBody();
			if (body->GetType() == b2_dynamicBody)
			{
				bool inside = fixture->TestPoint(m_point);
				if (inside)
				{
					m_fixture = fixture;

					// We are done, terminate the query.
					return false;
				}
			}

			// Continue the query.
			return true;
		}

		b2Vec2 m_point;
		b2Fixture* m_fixture;
	};

	class ContactListener :public b2ContactListener
	{
	public:

		std::function<void(uintptr_t)> contactCallback;

		/// Called when two fixtures begin to touch.
		void BeginContact(b2Contact* contact) override;

		/// Called when two fixtures cease to touch.
		void EndContact(b2Contact* contact) override;
	};
}

cocos2d::Scene* AddPairTest::createScene()
{
	return AddPairTest::create();
}

bool AddPairTest::init()
{
	if (!Scene::init())
	{
		return false;
	}

	m_mouseJoint = NULL;

	visibleSize = _director->getVisibleSize();
	origin = _director->getVisibleOrigin();

	//Do not offset scene node
	//setPosition(origin);//offset scene node by visible origin.

	ptM = 100;
	scheduleUpdate();

	//Phys init stuff

	//Create world node
	wN = B2WorldNode::create(100, Vec2(0, 0));
	wN->setTag(1);
	wN->retain();
	//this->addChild(wN, 9999999);

	//Set debug draw node
	auto dDN = DebugDrawNode::create();
	this->addChild(dDN, 55);
	dDN->SetFlags(b2Draw::e_shapeBit | b2Draw::e_jointBit);
	wN->setDebugDrawNode(dDN);

	Vec2 s_centre = Vec2(visibleSize.width / 2, visibleSize.height / 2);

	auto eComp = SuperPhysHelper::createEdgeBox(wN, visibleSize);
	auto edgeNode = Node::create();
	//addChild(edgeNode);
	edgeNode->addComponent(eComp);
	eComp->setPosition(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y);

	//New things


	auto dD = DrawNode::create();
	dD->setName("dD");
	dD->drawPoint(Vec2::ZERO, 10, Color4F::RED);
	addChild(dD, 100);

	//Create a dummy body
	b2BodyDef def = DefHelper::createWithPos(ptM);

	m_groundBody1 = wN->createPhysicsBodyComp(def);
	m_groundBody1->retain();

	//Add mouse touch eventlistener
	auto _mouseListener = EventListenerMouse::create();
	_mouseListener->onMouseMove = CC_CALLBACK_1(AddPairTest::onMouseMove, this);
	_mouseListener->onMouseUp = CC_CALLBACK_1(AddPairTest::onMouseUp, this);
	_mouseListener->onMouseDown = CC_CALLBACK_1(AddPairTest::onMouseDown, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(_mouseListener, this);


	float minX = -100;
	float maxX = 100;
	float minY = -50;
	float maxY = 50;

	for (int32 i = 0; i < 400; ++i)
	{
		Vec2 pos(visibleSize.width / 2 + origin.x + RandomHelper::random_real<float>(minX, maxX), visibleSize.height / 2 + origin.y + RandomHelper::random_real<float>(minY, maxY));

		addCirAtPosition(pos);

	}

	b2BodyDef bDef;
	DefHelper::initWithPos(bDef, ptM, Vec2(visibleSize.width / 2 + origin.x - 300, visibleSize.height / 2 + origin.y));
	bDef.type = b2_dynamicBody;
	bDef.bullet = true;

	auto bComp = wN->createPhysicsBodyComp(&bDef);

	b2PolygonShape bShape;

	ShapeHelper::initBoxShapeBy(bShape, ptM, 25, 25, Vec2(), 0);

	bComp->createFixture(&bShape, 40);

	auto box = Node::create();
	addChild(box);
	box->addComponent(bComp);

	bComp->getBox2dBody()->SetLinearVelocity(b2Vec2(1.0f, 0.0f));

	return true;
}

void AddPairTest::update(float delta)
{
	getChildByName<DrawNode*>("dD")->setPosition(m_mouseWorld - getPosition());
}

Node* AddPairTest::addCirAtPosition(cocos2d::Vec2 pos)
{
	b2CircleShape shape;

	ShapeHelper::initCircleShapeBy(shape, ptM, 4);

	b2FixtureDef fd;
	fd.shape = &shape;
	fd.density = 1.0f;
	fd.friction = 0.3f;

	b2BodyDef bDef;
	DefHelper::initWithPos(bDef, ptM, pos);
	bDef.type = b2_dynamicBody;

	auto comp1 = wN->createPhysicsBodyComp(&bDef);

	comp1->createFixture(&fd);

	auto box = Node::create();
	box->addComponent(comp1);
	addChild(box);
	pB = comp1;
	//comp1->setPosition(pos);
	return box;
}

//----------------
//Mouse events
//----------------

void AddPairTest::onMouseUp(cocos2d::EventMouse* event)
{
	CCLOG("Mouse Up at: x:%f y:%f", event->getCursorX(), event->getCursorY());

	if (m_mouseJoint)
	{
		wN->getB2World()->DestroyJoint(m_mouseJoint);
		m_mouseJoint = NULL;
	}
}

void AddPairTest::onMouseMove(cocos2d::EventMouse* event)
{
	Vec2 p = event->getLocationInView();
	m_mouseWorld = p;

	if (m_mouseJoint)
	{
		m_mouseJoint->SetTarget(Utilities::convertToB2Vec2(100, p));
	}

}

void AddPairTest::onMouseDown(cocos2d::EventMouse* event)
{
	auto p = event->getLocationInView();

	m_mouseWorld = p;

	CCLOG("Mouse Down at: x:%f y:%f", p.x, p.y);

	if (m_mouseJoint != NULL)
	{
		return;
	}

	// Make a small box.
	b2AABB aabb;
	Vec2 d = Vec2(0.1, 0.1);
	aabb.lowerBound = wN->convertToB2Vector(p - d);
	aabb.upperBound = wN->convertToB2Vector(p + d);

	// Query the world for overlapping shapes.
	QueryCallback1 callback(wN->convertToB2Vector(p));

	wN->getB2World()->QueryAABB(&callback, aabb);

	if (callback.m_fixture)
	{
		float frequencyHz = 5.0f;
		float dampingRatio = 0.7f;

		b2Body* body = callback.m_fixture->GetBody();
		b2MouseJointDef jd;
		jd.bodyA = m_groundBody1->getBox2dBody();
		jd.bodyB = body;
		jd.target = wN->convertToB2Vector(p);
		jd.maxForce = 1000.0f * body->GetMass();
		
		b2LinearStiffness(jd.stiffness, jd.damping, frequencyHz, dampingRatio, jd.bodyA, jd.bodyB);

		m_mouseJoint = (b2MouseJoint*)wN->getB2World()->CreateJoint(&jd);
		body->SetAwake(true);
	}
}
