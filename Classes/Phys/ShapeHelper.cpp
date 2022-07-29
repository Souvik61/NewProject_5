#include "Phys/ShapeHelper.h"
#include "box2d/include/box2d/box2d.h"
#include "Phys/Utilities.h"
#include "Phys/B2_PhysicsBody.h"

USING_NS_CC;
using namespace rb;

void rb::ShapeHelper::attachWithEdgeBox(float ptm_ratio, B2PhysicsBody* body, Vec2 size)
{
	b2EdgeShape shape;
	b2Vec2 vec1;
	b2Vec2 vec2;
	// Floor
	vec1.x = Utilities::convertToB2Float(ptm_ratio, -size.x / 2);
	vec1.y = Utilities::convertToB2Float(ptm_ratio, -size.y / 2);

	vec2.x = Utilities::convertToB2Float(ptm_ratio, size.x / 2);
	vec2.y = Utilities::convertToB2Float(ptm_ratio, -size.y / 2);

	shape.SetTwoSided(vec1, vec2);
	body->createFixture(&shape, 0);

	// Left wall
	vec2.x = Utilities::convertToB2Float(ptm_ratio, -size.x / 2);
	vec2.y = Utilities::convertToB2Float(ptm_ratio, size.y / 2);

	shape.SetTwoSided(vec1, vec2);
	body->createFixture(&shape, 0.0f);

	// Right wall
	vec1.x = Utilities::convertToB2Float(ptm_ratio, size.x / 2);
	vec1.y = Utilities::convertToB2Float(ptm_ratio, -size.y / 2);

	vec2.x = Utilities::convertToB2Float(ptm_ratio, size.x / 2);
	vec2.y = Utilities::convertToB2Float(ptm_ratio, size.y / 2);

	shape.SetTwoSided(vec1, vec2);
	body->createFixture(&shape, 0.0f);

	// Roof
	vec1.x = Utilities::convertToB2Float(ptm_ratio, -size.x / 2);
	vec1.y = Utilities::convertToB2Float(ptm_ratio, size.y / 2);
	shape.SetTwoSided(vec1, vec2);
	body->createFixture(&shape, 0.0f);
}

void rb::ShapeHelper::attachWithEdgeBox(float ptm_ratio, B2PhysicsBody* body, cocos2d::Vec2 size, cocos2d::PhysicsMaterial mat)
{
	b2EdgeShape shape;
	b2Vec2 vec1;
	b2Vec2 vec2;
	b2FixtureDef fD;

	fD.density = mat.density;
	fD.friction = mat.friction;
	fD.restitution = mat.restitution;
	// Floor
	vec1.x = Utilities::convertToB2Float(ptm_ratio, -size.x / 2);
	vec1.y = Utilities::convertToB2Float(ptm_ratio, -size.y / 2);
	vec2.x = Utilities::convertToB2Float(ptm_ratio, size.x / 2);
	vec2.y = Utilities::convertToB2Float(ptm_ratio, -size.y / 2);

	shape.SetTwoSided(vec1, vec2);
	fD.shape = &shape;
	body->createFixture(&fD);

	// Left wall
	vec2.x = Utilities::convertToB2Float(ptm_ratio, -size.x / 2);
	vec2.y = Utilities::convertToB2Float(ptm_ratio, size.y / 2);

	shape.SetTwoSided(vec1, vec2);
	body->createFixture(&fD);

	// Right wall
	vec1.x = Utilities::convertToB2Float(ptm_ratio, size.x / 2);
	vec1.y = Utilities::convertToB2Float(ptm_ratio, -size.y / 2);

	vec2.x = Utilities::convertToB2Float(ptm_ratio, size.x / 2);
	vec2.y = Utilities::convertToB2Float(ptm_ratio, size.y / 2);

	shape.SetTwoSided(vec1, vec2);
	body->createFixture(&fD);

	// Roof
	vec1.x = Utilities::convertToB2Float(ptm_ratio, -size.x / 2);
	vec1.y = Utilities::convertToB2Float(ptm_ratio, size.y / 2);
	shape.SetTwoSided(vec1, vec2);
	body->createFixture(&fD);
}

void rb::ShapeHelper::attachWithEdgeChain(float ptm_ratio, B2PhysicsBody* body, cocos2d::Vec2* points, int count)
{
	b2EdgeShape shape;
	b2Vec2 vec1;
	b2Vec2 vec2;

	for (size_t i = 0; i < count - 1; i++)
	{
		auto pt = points[i];
		auto pt1 = points[i+1];
		// An segment
		vec1.x = Utilities::convertToB2Float(ptm_ratio, pt.x);
		vec1.y = Utilities::convertToB2Float(ptm_ratio, pt.y);

		vec2.x = Utilities::convertToB2Float(ptm_ratio, pt1.x);
		vec2.y = Utilities::convertToB2Float(ptm_ratio, pt1.y);

		shape.SetTwoSided(vec1, vec2);
		body->createFixture(&shape, 0.0f);
	}

}

b2PolygonShape rb::ShapeHelper::createBoxShape(float ptm_ratio, cocos2d::Size size)
{
	b2PolygonShape shape;
	shape.SetAsBox(size.width / (2 * ptm_ratio), size.height / (2 * ptm_ratio));
	return shape;
}

b2CircleShape rb::ShapeHelper::createCircleShape(float ptm_ratio, float radius)
{
	b2CircleShape shape;
	shape.m_radius = radius / ptm_ratio;
	return shape;
}

b2PolygonShape rb::ShapeHelper::createPolygonShape(float ptm_ratio, cocos2d::Vec2* points, int count)
{
	b2PolygonShape shape;
	shape.Set(Utilities::convertToB2VecArr(points, count, ptm_ratio), count);
	return shape;
}

b2EdgeShape rb::ShapeHelper::createSegmentShape(float ptm_ratio, Vec2 pointA, Vec2 pointB)
{
	b2EdgeShape shape;
	shape.SetTwoSided(Utilities::convertToB2Vec2(ptm_ratio, pointA), Utilities::convertToB2Vec2(ptm_ratio, pointB));
	return shape;
}

//b2ChainShape rb::ShapeHelper::createEdgePolygonShape2(float ptm_ratio, cocos2d::Vec2* points, int count)
//{
//	b2ChainShape shape;
//	auto s = Utilities::convertToB2VecArr(points, count, ptm_ratio);
//	shape.CreateChain(Utilities::convertToB2VecArr(points, count, ptm_ratio), count);
//	return shape;
//}
