#include "Tests/TestScene3.h"
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

cocos2d::Scene* TestScene3::createScene()
{
	return TestScene3::create();
}

bool TestScene3::init()
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
	wN->setTag(1);
	this->addChild(wN, 9999999);

	//Set debug draw node
	auto dDN = DebugDrawNode::create();
	this->addChild(dDN, 55);
	dDN->SetFlags(b2Draw::e_centerOfMassBit | b2Draw::e_shapeBit);
	wN->setDebugDrawNode(dDN);

	Vec2 s_centre = Vec2(visibleSize.width / 2, visibleSize.height / 2);

	auto eComp = SuperPhysHelper::createEdgeBox(wN,visibleSize);
	auto edgeNode = Node::create();
	edgeNode->setPosition(visibleSize / 2);
	addChild(edgeNode);
	edgeNode->addComponent(eComp);

	//New things


	auto sprite1 = addSpriteAtPosition(Vec2(s_centre.x - 150, s_centre.y));

	b2Filter filter;
	filter.categoryBits = 0b10;//0010
	filter.maskBits = 0xFFFF;//1111

	((B2PhysicsBody*)sprite1->getComponent("rb_pb"))->setFilterDataAll(filter);


	sprite1 = addSpriteAtPosition(Vec2(s_centre.x - 150, s_centre.y + 100));

	filter.categoryBits = 0b100;//0100
	filter.maskBits = ~0b10;//1101
	((B2PhysicsBody*)sprite1->getComponent("rb_pb"))->setFilterDataAll(filter);

	auto sprite2 = addSpriteAtPosition(Vec2(s_centre.x + 150, s_centre.y));


	auto sprite3 = addSpriteAtPosition(Vec2(s_centre.x + 150, s_centre.y + 100));

	return true;
}

Sprite* TestScene3::addSpriteAtPosition(cocos2d::Vec2 pos)
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
