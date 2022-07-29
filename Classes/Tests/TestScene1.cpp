#include "Tests/TestScene1.h"
#include "Phys/B2_WorldNode.h"
#include "Phys/B2_PhysicsBody.h"
#include "box2d/include/box2d/box2d.h"
#include "Phys/DebugDrawNode.h"
#include "Phys/ShapeHelper.h"
#include "Phys/DefHelper.h"
#include "Phys/Utilities.h"

USING_NS_CC;
using namespace rb;

cocos2d::Scene* TestScene1::createScene()
{
	return TestScene1::create();
}

bool TestScene1::init()
{
	if (!Scene::init())
	{
		return false;
	}

	Size visibleSize = _director->getVisibleSize();
	Vec2 origin = _director->getVisibleOrigin();

	float ptM = 100;

	//Create world node
	auto wN = B2WorldNode::create(100, Vec2(0, -100));
	this->addChild(wN, 9999999);

	//Set debug draw node
	auto dDN = DebugDrawNode::create();
	this->addChild(dDN, 55);
	dDN->SetFlags(b2Draw::e_centerOfMassBit | b2Draw::e_shapeBit);
	wN->setDebugDrawNode(dDN);


	//auto edgeBody = PhysicsBody::createEdgeBox(visibleSize, PHYSICSBODY_MATERIAL_DEFAULT, 3);
	b2BodyDef edgeBodyDef = DefHelper::createWithPos(wN->PTM_RATIO, Vec2(visibleSize.width / 2, visibleSize.height / 2));

	//edgeBodyDef.position = wN->convertToB2Vector(Vec2(visibleSize));

	auto edgeBody = wN->createPhysicsBodyComp(edgeBodyDef);

	//ShapeHelper::attachWithEdgeBox(wN->PTM_RATIO, edgeBody, visibleSize);

	//auto boxShape = ShapeHelper::createBoxShape(wN->PTM_RATIO, Size(100, 100));
	//auto cirShape = ShapeHelper::createCircleShape(wN->PTM_RATIO, 100);

	Vec2 pointArr[4];
	pointArr[0] = Vec2(0, 0);
	pointArr[1] = Vec2(100, 100);
	pointArr[2] = Vec2(200, 0);
	pointArr[3] =  Vec2(300, 100);

	auto chainShape = ShapeHelper::createSegmentShape(ptM, pointArr[0], pointArr[1]);

	edgeBody->createFixture(&chainShape, 0);

	//ShapeHelper::attachWithEdgeChain(ptM, edgeBody, pointArr, 4);
	auto edgeNode = Node::create();
	edgeNode->addComponent(edgeBody);
	addChild(edgeNode);


	//Add a circle
	b2BodyDef cirBodyDef;
	cirBodyDef.type = b2BodyType::b2_dynamicBody;

	auto cirBody = wN->createPhysicsBodyComp(cirBodyDef);

	b2CircleShape cShape;
	cShape.m_radius = 1;

	cirBody->createFixture(&cShape, 1);

	auto circle = Node::create();
	circle->addComponent(cirBody);
	cirBody->setPosition(Vec2(500,500));
	//addChild(circle);


	return true;
}
