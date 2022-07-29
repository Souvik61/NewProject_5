#include "Tests/TestScene4.h"
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


class QueryCallback : public b2QueryCallback
{
public:
	QueryCallback(const b2Vec2& point)
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

cocos2d::Scene* TestScene4::createScene()
{
	return TestScene4::create();
}

bool TestScene4::init()
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
	_mouseListener->onMouseMove = CC_CALLBACK_1(TestScene4::onMouseMove, this);
	_mouseListener->onMouseUp = CC_CALLBACK_1(TestScene4::onMouseUp, this);
	_mouseListener->onMouseDown = CC_CALLBACK_1(TestScene4::onMouseDown, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(_mouseListener, this);

	addCells();

	//Add joints 
	//addDistanceJoint();
	//addRevoluteJoint();

	drawDots();
	addAllJoints();

	return true;
}

void TestScene4::update(float delta)
{
	getChildByName<DrawNode*>("dD")->setPosition(m_mouseWorld - getPosition());
}

Sprite* TestScene4::addSpriteAtPosition(cocos2d::Vec2 pos)
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


void TestScene4::addCells()
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

void TestScene4::addAllJoints()
{
	float width = 369, height = 240;
	auto wN = getChildByTag<B2WorldNode*>(1);

	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 3; ++j)
		{
			Vec2 offset(origin.x + j * width + width / 2, origin.y + i * height + height / 2);

			/*auto fg = DrawNode::create();
			fg->drawPoint(Vec2::ZERO, 10, Color4F::RED);
			fg->setPosition(offset);
			addChild(fg);*/

			switch (i * 3 + j) {

			case 0:
			{
				//Add distance joint
			
				//Add box1
				auto comp1 = SuperPhysHelper::createBox(wN, Size(50, 50));

				auto box1 = Node::create();
				addChild(box1);
				box1->addComponent(comp1);
				comp1->setPosition(offset - Vec2(45, 45));

				//Add box2
				auto comp2 = SuperPhysHelper::createBox(wN, Size(50, 50));

				auto box2 = Node::create();
				box2->setPosition(200, 200);
				addChild(box2);
				box2->addComponent(comp2);
				comp2->setPosition(offset + Vec2(45, 45));

				//Add a distance joint

				float m_hertz = 1.0f;
				float m_dampingRatio = 0.7f;

				b2DistanceJointDef jointDef = DefHelper::createDistanceJointDef(ptM, comp1, comp2, comp1->getPosition(), comp2->getPosition());
				//jointDef.dampingRatio = 0.5f;
				//jointDef.frequencyHz = 4.0f;

				b2LinearStiffness(jointDef.stiffness, jointDef.damping, m_hertz, m_dampingRatio, comp1->getBox2dBody(), comp2->getBox2dBody());

				wN->createJoint(&jointDef);

			}
			break;
			case 1:
			{
				//Add revolute joint

				//Add triangle

				Vec2 pointArr[] = {
					Vec2(-100,0),Vec2(100,0),Vec2(0,100)
				};

				auto comp1 = SuperPhysHelper::createPolygon(wN, pointArr, 3);
				auto box1 = Node::create();
				addChild(box1);
				box1->addComponent(comp1);
				comp1->setPosition(offset + Vec2());

				//Add box2
				auto comp2 = SuperPhysHelper::createBox(wN, Size(50, 50));

				auto box2 = Node::create();
				addChild(box2);
				box2->addComponent(comp2);
				comp2->setPosition(offset);

				//Add a distance joint

				b2RevoluteJointDef jointDef = DefHelper::createRevoluteJointDef(ptM, comp1, comp2, comp1->getPosition());
				jointDef.lowerAngle = -0.5f * b2_pi; // -90 degrees
				jointDef.upperAngle = 0.25f * b2_pi; // 45 degrees
				jointDef.maxMotorTorque = 10.0f;
				jointDef.motorSpeed = 10.f;
				jointDef.enableMotor = true;

				wN->createJoint(&jointDef);
			}
			break;
			case 2:
			{
				//Add revolute joint

				//Add triangle

				Vec2 pointArr[] = {
					Vec2(-100,0),Vec2(100,0),Vec2(0,100)
				};

				auto comp1 = SuperPhysHelper::createPolygon(wN, pointArr, 3);
				auto box1 = Node::create();
				addChild(box1);
				box1->addComponent(comp1);
				comp1->setPosition(offset + Vec2(0, -25));
				//Add box2
				auto comp2 = SuperPhysHelper::createBox(wN, Size(50, 50));

				auto box2 = Node::create();
				addChild(box2);
				box2->addComponent(comp2);
				comp2->setPosition(offset + Vec2(0, 50));

				//Add a prismatic joint
				b2PrismaticJointDef jointDef = DefHelper::createPrismaticJointDef(ptM, comp1, comp2, comp2->getPosition(), Vec2::UNIT_X);
				jointDef.lowerTranslation = -5.0f;
				jointDef.upperTranslation = 2.5f;
				jointDef.enableLimit = true;

				wN->createJoint(&jointDef);

			}
			break;
			case 3:
			{
				//Add pulley joint

				float y = 16.0f;
				float L = 100.0f;
				float a = 1.0f;
				float b = 2.0f;

				//Add box1
				auto comp1 = SuperPhysHelper::createBox(wN, Size(50, 50));

				auto box1 = Node::create();
				addChild(box1);
				box1->addComponent(comp1);
				comp1->setPosition(offset + Vec2(-45, 0));

				//Add box2
				auto comp2 = SuperPhysHelper::createBox(wN, Size(50, 50));

				auto box2 = Node::create();
				addChild(box2);
				box2->addComponent(comp2);
				comp2->setPosition(offset + Vec2(45, 0));


				/*{
					b2BodyDef bd;
					ground = m_world->CreateBody(&bd);

					b2CircleShape circle;
					circle.m_radius = 2.0f;

					circle.m_p.Set(-10.0f, y + b + L);
					ground->CreateFixture(&circle, 0.0f);

					circle.m_p.Set(10.0f, y + b + L);
					ground->CreateFixture(&circle, 0.0f);
				}*/

				{

					Vec2 anchor1(offset.x-45, offset.y - 25);
					Vec2 anchor2(offset.x + 45, offset.y + 25);
					Vec2 groundAnchor1(offset.x - 45.0f, offset.y + b + L);
					Vec2 groundAnchor2(offset.x + 45.0f, offset.y + b + L);
					
					b2PulleyJointDef def = DefHelper::createPulleyJointDef(ptM, comp1, comp2, groundAnchor1, groundAnchor2, anchor1, anchor2, 1);

					wN->createJoint(&def);
				}

				
			}
			break;
			case 4:
			{
				//Gear joint

				//Add a ground
				auto gComp = wN->createPhysicsBodyComp(b2BodyDef());
				auto gNode = Node::create();
				addChild(gNode);
				gNode->addComponent(gComp);
				gComp->setPosition(offset - Vec2(0, 120));
				//Add a circle
				auto comp1 = SuperPhysHelper::createCircle(wN, 25);
				auto cir = Node::create();
				addChild(cir);
				cir->addComponent(comp1);
				comp1->setPosition(offset + Vec2(-30.0f, 12.0f));

				b2PolygonShape box;
				box.SetAsBox(0.5f, 5.0f);

				b2RevoluteJointDef jd1 = DefHelper::createRevoluteJointDef(ptM, gComp, comp1, Vec2::ZERO);
				jd1.localAnchorA = gComp->getBox2dBody()->GetLocalPoint(comp1->getBox2dBody()->GetPosition());
				jd1.localAnchorB = comp1->getBox2dBody()->GetLocalPoint(comp1->getBox2dBody()->GetPosition());
				jd1.referenceAngle = comp1->getBox2dBody()->GetAngle() - gComp->getBox2dBody()->GetAngle();
				auto m_joint1 = (b2RevoluteJoint*)wN->createJoint(&jd1);


				b2BodyDef bd2;
				auto comp2 = SuperPhysHelper::createBox(wN, Size(25, 50));
				auto boxL = Node::create();
				addChild(boxL);
				boxL->addComponent(comp2);
				comp2->setPosition(offset + Vec2(7.0f, 12.0f));
				
				b2PrismaticJointDef jd3 = DefHelper::createPrismaticJointDef(ptM, gComp, comp2, comp2->getPosition(), Vec2::UNIT_Y);
				jd3.lowerTranslation = -0.5f;
				jd3.upperTranslation = 0.5f;
				jd3.enableLimit = true;

				auto m_joint3 = (b2PrismaticJoint*)wN->createJoint(&jd3);

				b2GearJointDef jd5;
				jd5.bodyA = comp1->getBox2dBody();
				jd5.bodyB = comp2->getBox2dBody();
				jd5.joint1 = m_joint1;
				jd5.joint2 = m_joint3;
				jd5.ratio = -1.0f / 1;
				auto m_joint5 = (b2GearJoint*)wN->createJoint(&jd5);
			}
			break;
			case 5:
			{
				//Add wheel joint

				//Add a ground
				auto gComp = wN->createPhysicsBodyComp(b2BodyDef());
				auto gNode = Node::create();
				addChild(gNode);
				gNode->addComponent(gComp);
				gComp->setPosition(offset - Vec2(0, 120));

				auto cirComp = SuperPhysHelper::createCircle(wN, 25);
				auto circle = Node::create();
				addChild(circle);
				circle->addComponent(cirComp);
				cirComp->setPosition(offset);

				b2WheelJointDef jd = DefHelper::createWheelJointDef(ptM, gComp, cirComp, cirComp->getPosition(), Vec2::UNIT_Y);
				jd.motorSpeed = 10;
				jd.maxMotorTorque = 10000.0f;
				jd.enableMotor = true;
				jd.upperTranslation = 50 / ptM;
				jd.lowerTranslation = -50 / ptM;
				jd.enableLimit = true;
				
				float hertz = 1.0f;
				float dampingRatio = 0.7;

				b2LinearStiffness(jd.stiffness, jd.damping, hertz, dampingRatio, gComp->getBox2dBody(), cirComp->getBox2dBody());

				auto m_joint = (b2WheelJoint*)wN->createJoint(&jd);
			}
			break;
			case 6:
			{
				//Add friction joint
				// 
				//Add a ground
				auto gComp = wN->createPhysicsBodyComp(b2BodyDef());
				auto gNode = Node::create();
				addChild(gNode);
				gNode->addComponent(gComp);
				gComp->setPosition(offset);

				//Add box1
				auto comp1 = SuperPhysHelper::createBox(wN, Size(50, 50));

				auto box1 = Node::create();
				addChild(box1);
				box1->addComponent(comp1);
				comp1->setPosition(offset + Vec2(0, 0));

				b2FrictionJointDef jd = DefHelper::createFrictionJointDef(ptM, gComp, comp1, offset);
				jd.maxForce = 1;
				jd.maxTorque = 0.1f;

				wN->createJoint(&jd);
			}
			break;
			default:
				break;
			};

		}
	}
}

void TestScene4::addDistanceJoint()
{
	auto wN = getChildByTag<B2WorldNode*>(1);

	//Add box1
	auto comp1 = SuperPhysHelper::createBox(wN, Size(50, 50));

	auto box1 = Node::create();
	addChild(box1);
	box1->addComponent(comp1);
	comp1->setPosition(visibleSize / 2);

	//Add box2
	auto comp2 = SuperPhysHelper::createBox(wN, Size(50, 50));

	auto box2 = Node::create();
	box2->setPosition(200, 200);
	addChild(box2);
	box2->addComponent(comp2);
	comp2->setPosition(Vec2(200, 200));

	//Add a distance joint

	b2DistanceJointDef jointDef = DefHelper::createDistanceJointDef(ptM, comp1, comp2, comp1->getPosition(), comp2->getPosition());
	/*jointDef.dampingRatio = 0.5f;
	jointDef.frequencyHz = 4.0f;*/

	wN->createJoint(&jointDef);
}

void TestScene4::addRevoluteJoint()
{
	auto wN = getChildByTag<B2WorldNode*>(1);

	//Add box1
	auto comp1 = SuperPhysHelper::createBox(wN, Size(50, 50));
	auto box1 = Node::create();
	addChild(box1);
	box1->addComponent(comp1);
	comp1->setPosition(300,200);

	//Add box2
	auto comp2 = SuperPhysHelper::createBox(wN, Size(50, 50));

	auto box2 = Node::create();
	box2->setPosition(200, 200);
	addChild(box2);
	box2->addComponent(comp2);
	comp2->setPosition(300, 210);

	//Add a distance joint

	b2RevoluteJointDef jointDef = DefHelper::createRevoluteJointDef(ptM, comp1, comp2, comp1->getPosition());
	jointDef.lowerAngle = -0.5f * b2_pi; // -90 degrees
	jointDef.upperAngle = 0.25f * b2_pi; // 45 degrees
	jointDef.maxMotorTorque = 10.0f;
	jointDef.motorSpeed = 10.f;
	jointDef.enableMotor = true;

	wN->createJoint(&jointDef);
}


void TestScene4::addMouseDrag()
{

}

void TestScene4::drawDots()
{
	float width = 369, height = 240;

	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 3; ++j)
		{
			Vec2 offset(origin.x + j * width + width / 2, origin.y + i * height + height / 2);

			auto fg = DrawNode::create();
			fg->drawPoint(Vec2::ZERO, 10, Color4F::RED);
			fg->setPosition(offset);
			addChild(fg);
		}
	}
}

//----------------
//Mouse events
//----------------

void TestScene4::onMouseUp(cocos2d::EventMouse* event)
{
	CCLOG("Mouse Up at: x:%f y:%f", event->getCursorX(), event->getCursorY());
	auto wN = getChildByTag<B2WorldNode*>(1);
	if (m_mouseJoint)
	{
		wN->getB2World()->DestroyJoint(m_mouseJoint);
		m_mouseJoint = NULL;
	}
}

void TestScene4::onMouseMove(cocos2d::EventMouse* event)
{
	Vec2 p = event->getLocationInView();
	m_mouseWorld = p;

	if (m_mouseJoint)
	{
		m_mouseJoint->SetTarget(Utilities::convertToB2Vec2(100, p));
	}

}

void TestScene4::onMouseDown(cocos2d::EventMouse* event)
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
	QueryCallback callback(wN->convertToB2Vector(p));

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
