#include "Tests/TestScene2.h"
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

cocos2d::Scene* TestScene2::createScene()
{
	return TestScene2::create();
}

bool TestScene2::init()
{
	if (!Scene::init())
	{
		return false;
	}

	Size visibleSize = _director->getVisibleSize();
	Vec2 origin = _director->getVisibleOrigin();

	setPosition(origin);//offset scene node by visible origin.

	float ptM = 100;

	//Create world node
	auto wN = B2WorldNode::create(100, Vec2(0, -100));
	this->addChild(wN, 9999999);

	//Set debug draw node
	auto dDN = DebugDrawNode::create();
	this->addChild(dDN, 55);
	dDN->SetFlags(b2Draw::e_centerOfMassBit | b2Draw::e_shapeBit);
	wN->setDebugDrawNode(dDN);

	Vec2 s_centre = Vec2(visibleSize.width / 2, visibleSize.height / 2);

	auto bodyComp = SuperPhysHelper::createBox(wN, Size(100, 100));

	//bodyComp->setBodyType(b2BodyType::b2_dynamicBody);

	auto ship = Sprite::create("Sprites/ship.png");
	ship->setScale(0.2f);
	ship->setPosition(visibleSize / 2);
	ship->addComponent(bodyComp);
	addChild(ship);

	// add five dynamic bodies
	for (int i = 0; i < 5; ++i)
	{
		auto bodyComp1 = SuperPhysHelper::createBox(wN, Size(65.0f, 81.0f),
			PhysicsMaterial(0.1f, 1.0f, 0.0f));
		bodyComp1->setBodyType(b2_dynamicBody);

		//set initial velocity of physicsBody
		bodyComp1->setLinearVelocity(Vec2(cocos2d::random(-500, 500),
			cocos2d::random(-500, 500)));

		auto sprite = Sprite::create("Sprites/ship.png");
		sprite->setPosition(Vec2(s_centre.x + cocos2d::random(-300, 300),
			s_centre.y + cocos2d::random(-300, 300)));
		sprite->setScale(0.2f);
		sprite->addComponent(bodyComp1);

		addChild(sprite);
	}

	auto eDef = DefHelper::createWithPos(ptM);

	auto eComp = wN->createPhysicsBodyComp(eDef);

	ShapeHelper::attachWithEdgeBox(ptM, eComp, visibleSize);

	auto edgeNode = Node::create();
	edgeNode->setPosition(visibleSize / 2);
	addChild(edgeNode);
	edgeNode->addComponent(eComp);



	return true;
}
