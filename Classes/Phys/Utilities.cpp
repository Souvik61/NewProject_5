#include "Utilities.h"
#include "cocos2d.h"
#include "box2d/include/box2d/box2d.h"

using namespace rb;

//cocos2d::Color4F rb::Utilities::convertToCCColor(b2Color col)
//{
//	return cocos2d::Color4F(col.r, col.g, col.b, col.a);
//}
//
//cocos2d::Vec2 rb::Utilities::convertToCCVec2(float ptm_ratio, b2Vec2 vec)
//{
//	return cocos2d::Vec2(vec.x * ptm_ratio, vec.y * ptm_ratio);
//}

//b2Vec2 rb::Utilities::convertToB2Vec2(float ptm_ratio, cocos2d::Vec2 vec)
//{
//	return b2Vec2(vec.x / ptm_ratio, vec.y / ptm_ratio);
//}

cocos2d::Vec2* rb::Utilities::convertToCCVecArr(const b2Vec2* bVec, int size, float ptm_ratio)
{
	cocos2d::Vec2* vec = new cocos2d::Vec2[size]();

	for (size_t i = 0; i < size; i++)
	{
		vec[i] = cocos2d::Vec2(bVec[i].x * ptm_ratio, bVec[i].y * ptm_ratio);
	}
	return vec;
}

b2Vec2* rb::Utilities::convertToB2VecArr(cocos2d::Vec2* inArray, int size, float ptm_ratio)
{
	b2Vec2* vec = new b2Vec2[size];

	for (size_t i = 0; i < size; i++)
	{
		vec[i].Set(inArray[i].x / ptm_ratio, inArray[i].y / ptm_ratio);
	}
	return vec;
}

