#include "Tests/TestScene5.h"
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

cocos2d::Scene* TestScene5::createScene()
{
	return TestScene5::create();
}

bool TestScene5::init()
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
	auto wN = B2WorldNode::create(100, Vec2(0, -100));
	wN->setTag(1);
	this->addChild(wN, 9999999);

	auto cListener = new ContactListener();
	cListener->contactCallback = CC_CALLBACK_1(TestScene5::destroyThisBody, this);
	wN->getB2World()->SetContactListener(cListener);

	//Set debug draw node
	auto dDN = DebugDrawNode::create();
	this->addChild(dDN, 55);
	dDN->SetFlags(b2Draw::e_shapeBit | b2Draw::e_jointBit);
	wN->setDebugDrawNode(dDN);

	Vec2 s_centre = Vec2(visibleSize.width / 2, visibleSize.height / 2);

	auto eComp = SuperPhysHelper::createEdgeBox(wN,visibleSize);
	auto edgeNode = Node::create();
	addChild(edgeNode);
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
	_mouseListener->onMouseMove = CC_CALLBACK_1(TestScene5::onMouseMove, this);
	_mouseListener->onMouseUp = CC_CALLBACK_1(TestScene5::onMouseUp, this);
	_mouseListener->onMouseDown = CC_CALLBACK_1(TestScene5::onMouseDown, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(_mouseListener, this);

	//addCells();

	//Add joints 
	//addDistanceJoint();
	//addRevoluteJoint();

	auto comp1 = SuperPhysHelper::createBox(wN, Size(50, 50));
	auto box = Node::create();
	box->addComponent(comp1);
	addChild(box);

	comp1->setPosition(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y);

	return true;
}

void TestScene5::update(float delta)
{
	getChildByName<DrawNode*>("dD")->setPosition(m_mouseWorld - getPosition());
}

Sprite* TestScene5::addSpriteAtPosition(cocos2d::Vec2 pos)
{
	auto sp = Sprite::create("Sprites/ship.png");
	sp->setScale(0.2f);
	sp->setPosition(pos);
	addChild(sp);
	auto wN = getChildByTag<B2WorldNode*>(1);

	auto bodyComp = SuperPhysHelper::createBox(wN, Size(50, 50));
	sp->addComponent(bodyComp);
	return sp;
}


void TestScene5::addCells()
{
	auto wN = getChildByTag<B2WorldNode*>(1);
	Size cellSize(369, 240);
	float x, y;
	y = cellSize.height / 2;

	for (size_t i = 1; i < 4; i++)//for rows
	{
		x = cellSize.width / 2;
		for (size_t j = 1; j < 4; j++)//for columns
		{
			auto eComp = SuperPhysHelper::createEdgeBox(wN, cellSize);
			auto edgeNode = Node::create();
			addChild(edgeNode);
			edgeNode->addComponent(eComp);
			eComp->setPosition(x + origin.x, y + origin.y);
			x = x + cellSize.width;
		}
		y = y + cellSize.height;
	}
}

void TestScene5::destroyThisBody(uintptr_t body)
{
	destroyBodyInNextLoop(reinterpret_cast<B2PhysicsBody*>(body));
}

void TestScene5::destroyBodyInNextLoop(B2PhysicsBody* body)
{
	_scheduler->schedule([=](float dt) {

		body->getOwner()->removeComponent(body);
		_scheduler->unschedule("a", this);

		}, this, 0, false, "a");
}

//----------------
//Mouse events
//----------------

void TestScene5::onMouseUp(cocos2d::EventMouse* event)
{
	CCLOG("Mouse Up at: x:%f y:%f", event->getCursorX(), event->getCursorY());
	auto wN = getChildByTag<B2WorldNode*>(1);
	if (m_mouseJoint)
	{
		wN->getB2World()->DestroyJoint(m_mouseJoint);
		m_mouseJoint = NULL;
	}
}

void TestScene5::onMouseMove(cocos2d::EventMouse* event)
{
	Vec2 p = event->getLocationInView();
	m_mouseWorld = p;

	if (m_mouseJoint)
	{
		m_mouseJoint->SetTarget(Utilities::convertToB2Vec2(100, p));
	}

}

void TestScene5::onMouseDown(cocos2d::EventMouse* event)
{
	auto p = event->getLocationInView();

	m_mouseWorld = p;

	CCLOG("Mouse Down at: x:%f y:%f", p.x, p.y);

	if (m_mouseJoint != NULL)
	{
		return;
	}

	auto wN = getChildByTag<B2WorldNode*>(1);

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



void ContactListener::BeginContact(b2Contact* contact)
{
	CCLOG("ContactBegin");
	auto ud = contact->GetFixtureA()->GetBody()->GetUserData();
	if (ud.pointer)
	{
		contactCallback(ud.pointer);
	}
}

void ContactListener::EndContact(b2Contact* contact)
{
	CCLOG("ContactEnd");
}
