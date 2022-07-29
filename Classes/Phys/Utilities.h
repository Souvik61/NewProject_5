#ifndef __RB_UTILITIES_H__
#define __RB_UTILITIES_H__

#include "cocos2d.h"

struct b2Color;
struct b2Vec2;
class b2Body;

namespace rb {

    class Utilities
    {
    public:
        static cocos2d::Color4F convertToCCColor(b2Color);
        static float convertToCCFloat(float ptm_ratio, float in) { return in * ptm_ratio; };
        static float convertToB2Float(float ptm_ratio, float in) { return in / ptm_ratio; };
        static cocos2d::Vec2 convertToCCVec2(float, b2Vec2);
        static b2Vec2 convertToB2Vec2(float, cocos2d::Vec2);
        static cocos2d::Vec2* convertToCCVecArr(const b2Vec2*,int size,float ptm_ratio);
        static b2Vec2* convertToB2VecArr(cocos2d::Vec2* inArray, int size, float ptm_ratio);


        //Unknown function
        static void b2LinearStiffness(float& stiffness, float& damping, float frequencyHertz, float dampingRatio, const b2Body* bodyA, const b2Body* bodyB);
    };
}

#endif //__RB_UTILITIES_H__
