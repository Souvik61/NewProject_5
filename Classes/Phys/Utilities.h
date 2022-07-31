#ifndef __RB_UTILITIES_H__
#define __RB_UTILITIES_H__

#include "cocos2d.h"
#include "box2d/include/box2d/b2_draw.h"

struct b2Color;
struct b2Vec2;
class b2Body;

namespace rb {

    class Utilities
    {
    public:
        //static cocos2d::Color4F convertToCCColor(b2Color);

        static inline cocos2d::Color4F rb::Utilities::convertToCCColor(b2Color col)
        {
            return cocos2d::Color4F(col.r, col.g, col.b, col.a);
        }

        static float convertToCCFloat(float ptm_ratio, float in) { return in * ptm_ratio; };
        static float convertToB2Float(float ptm_ratio, float in) { return in / ptm_ratio; };
        //static cocos2d::Vec2 convertToCCVec2(float, b2Vec2);
        
        static inline cocos2d::Vec2 rb::Utilities::convertToCCVec2(float ptm_ratio, b2Vec2 vec)
        {
            return cocos2d::Vec2(vec.x * ptm_ratio, vec.y * ptm_ratio);
        }

        static inline b2Vec2 rb::Utilities::convertToB2Vec2(float ptm_ratio, cocos2d::Vec2 vec)
        {
            return b2Vec2(vec.x / ptm_ratio, vec.y / ptm_ratio);
        }

        //static b2Vec2 convertToB2Vec2(float, cocos2d::Vec2);
        static cocos2d::Vec2* convertToCCVecArr(const b2Vec2*,int size,float ptm_ratio);
        static b2Vec2* convertToB2VecArr(cocos2d::Vec2* inArray, int size, float ptm_ratio);


        //Unknown function
        
    };
}

#endif //__RB_UTILITIES_H__
