#include "Phys/ShapeHelper.h"
#include "box2d/include/box2d/box2d.h"
#include "Phys/Utilities.h"
#include "Phys/B2_PhysicsBody.h"
#include "SuperPhysHelper.h"
#include "Phys/DefHelper.h"
#include "Phys/B2_WorldNode.h"

USING_NS_CC;
using namespace rb;

B2PhysicsBody* rb::SuperPhysHelper::createBox(B2WorldNode* wN, cocos2d::Size size, cocos2d::PhysicsMaterial mat, cocos2d::Vec2 offset,Vec2 initPos)
{
	b2BodyDef def = DefHelper::createWithPos(wN->PTM_RATIO, initPos);
	def.type = b2_dynamicBody;

	B2PhysicsBody* bodyComp = wN->createPhysicsBodyComp(def);

	b2PolygonShape sp = ShapeHelper::createBoxShape(wN->PTM_RATIO, size);

	b2FixtureDef fD;
	fD.shape = &sp;
	fD.density = mat.density;
	fD.friction = mat.friction;
	fD.restitution = mat.restitution;

	bodyComp->createFixture(&fD);

	return bodyComp;
}

B2PhysicsBody* rb::SuperPhysHelper::createCircle(B2WorldNode* wN, float rad, PhysicsMaterial mat, Vec2 offset, Vec2 initPos)
{
	b2BodyDef def = DefHelper::createWithPos(wN->PTM_RATIO, initPos);
	def.type = b2_dynamicBody;
	B2PhysicsBody* bodyComp = wN->createPhysicsBodyComp(def);
	b2CircleShape sp = ShapeHelper::createCircleShape(wN->PTM_RATIO, rad);

	b2FixtureDef fD;
	fD.shape = &sp;
	fD.density = mat.density;
	fD.friction = mat.friction;
	fD.restitution = mat.restitution;
	bodyComp->createFixture(&fD);
	return bodyComp;
}

B2PhysicsBody* rb::SuperPhysHelper::createEdgeBox(B2WorldNode* wN, Size size, PhysicsMaterial mat, Vec2 offset, Vec2 initPos)
{
	b2BodyDef def = DefHelper::createWithPos(wN->PTM_RATIO, initPos);
	B2PhysicsBody* bodyComp = wN->createPhysicsBodyComp(def);

	ShapeHelper::attachWithEdgeBox(wN->PTM_RATIO, bodyComp, size, mat);
	return bodyComp;
}

B2PhysicsBody* rb::SuperPhysHelper::createPolygon(B2WorldNode* wN, Vec2* pts, int count, PhysicsMaterial mat, Vec2 off, Vec2 initPos)
{
	b2BodyDef def = DefHelper::createWithPos(wN->PTM_RATIO, initPos);
	def.type = b2_dynamicBody;

	B2PhysicsBody* bodyComp = wN->createPhysicsBodyComp(def);

	b2PolygonShape sp = ShapeHelper::createPolygonShape(wN->PTM_RATIO, pts, count);

	b2FixtureDef fD;
	fD.shape = &sp;
	fD.density = mat.density;
	fD.friction = mat.friction;
	fD.restitution = mat.restitution;

	bodyComp->createFixture(&fD);

	return bodyComp;
}
