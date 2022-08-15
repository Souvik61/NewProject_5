#include "Phys/DefHelper.h"
#include "Phys/Utilities.h"
#include "box2d/include/box2d/box2d.h"
#include "Phys/B2_PhysicsBody.h"

USING_NS_CC;
using namespace rb;

b2BodyDef rb::DefHelper::createWithPos(float ptm_ratio, cocos2d::Vec2 pos)
{
	b2BodyDef def;
	def.position = Utilities::convertToB2Vec2(ptm_ratio, pos);
	return def;
}

void rb::DefHelper::initWithPos(b2BodyDef& def, float ptm_ratio, const cocos2d::Vec2&  pos)
{
	def.position = Utilities::convertToB2Vec2(ptm_ratio, pos);
}

b2DistanceJointDef rb::DefHelper::createDistanceJointDef(float pTM, B2PhysicsBody* bodyA, B2PhysicsBody* bodyB, Vec2 anchorA, Vec2 anchorB)
{
	b2DistanceJointDef def;
	def.Initialize(bodyA->getBox2dBody(), bodyB->getBox2dBody(), Utilities::convertToB2Vec2(pTM, anchorA), Utilities::convertToB2Vec2(pTM, anchorB));
	return def;
}

b2RevoluteJointDef rb::DefHelper::createRevoluteJointDef(float pTM, B2PhysicsBody* bodyA, B2PhysicsBody* bodyB, cocos2d::Vec2 anchor)
{
	b2RevoluteJointDef jointDef;
	jointDef.Initialize(bodyA->getBox2dBody(), bodyB->getBox2dBody(), Utilities::convertToB2Vec2(pTM, anchor));
	return jointDef;
}

b2PrismaticJointDef rb::DefHelper::createPrismaticJointDef(float pTM, B2PhysicsBody* bodyA, B2PhysicsBody* bodyB, cocos2d::Vec2 anchor, Vec2 axis)
{
	b2PrismaticJointDef def;
	def.Initialize(bodyA->getBox2dBody(), bodyB->getBox2dBody(), Utilities::convertToB2Vec2(pTM, anchor), Utilities::convertToB2Vec2(1, axis));//axis is a unit vector so the conversion factor is 1.
	return def;
}

b2PulleyJointDef rb::DefHelper::createPulleyJointDef(float ptm, B2PhysicsBody* bodyA, B2PhysicsBody* bodyB, cocos2d::Vec2 gancA, cocos2d::Vec2 gancB, Vec2 ancA, Vec2 ancB, float rat)
{
	b2PulleyJointDef def;
	def.Initialize(bodyA->getBox2dBody(), bodyB->getBox2dBody(), Utilities::convertToB2Vec2(ptm, gancA), Utilities::convertToB2Vec2(ptm, gancB), Utilities::convertToB2Vec2(ptm, ancA), Utilities::convertToB2Vec2(ptm, ancB), rat);
	return def;
}

b2WheelJointDef rb::DefHelper::createWheelJointDef(float ptm, B2PhysicsBody* bodyA, B2PhysicsBody* bodyB, Vec2 wAnch, Vec2 wAxis)
{
	b2WheelJointDef def;
	def.Initialize(bodyA->getBox2dBody(), bodyB->getBox2dBody(), Utilities::convertToB2Vec2(ptm, wAnch), Utilities::convertToB2Vec2(1, wAxis));//Conversion factor 1 for axis
	return def;
}

void rb::DefHelper::initRevoluteJointDef(float ptm, b2RevoluteJointDef& j, B2PhysicsBody* bodyA, B2PhysicsBody* bodyB, cocos2d::Vec2 anc)
{
	j.Initialize(bodyA->getBox2dBody(), bodyB->getBox2dBody(), Utilities::convertToB2Vec2(ptm, anc));
}

void rb::DefHelper::initPrismaticJointDef(float ptm, b2PrismaticJointDef& def, B2PhysicsBody* bodyA, B2PhysicsBody* bodyB, cocos2d::Vec2 anc, cocos2d::Vec2 axis)
{
	def.Initialize(bodyA->getBox2dBody(), bodyB->getBox2dBody(), Utilities::convertToB2Vec2(ptm, anc), Utilities::convertToB2Vec2(1, axis));
}
