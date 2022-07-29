#ifndef __RB_SHAPEHELPER_H__
#define __RB_SHAPEHELPER_H__

#include "cocos2d.h"

class b2PolygonShape;
class b2ChainShape;
class b2CircleShape;
class b2EdgeShape;

namespace rb {

    class B2PhysicsBody;

    class ShapeHelper
    {
    public:
        /*
        *Attach shape to a physics body
        */

        /*
        *For complex shapes
        */
        static void attachWithEdgeBox(float ptm_ratio, B2PhysicsBody* body, cocos2d::Vec2 size);
        static void attachWithEdgeBox(float ptm_ratio, B2PhysicsBody* body, cocos2d::Vec2 size, cocos2d::PhysicsMaterial material);
        static void attachWithEdgeChain(float ptm_ratio, B2PhysicsBody* body, cocos2d::Vec2* points, int count);

        /*
        *For simple shapes
        */
        static b2PolygonShape createBoxShape(float ptm_ratio, cocos2d::Size size);
        static b2CircleShape createCircleShape(float ptm_ratio, float radius);
        static b2PolygonShape createPolygonShape(float ptm_ratio, cocos2d::Vec2* points, int count);
        static b2EdgeShape createSegmentShape(float ptm_ratio, cocos2d::Vec2 pointA, cocos2d::Vec2 pointB);
        //static b2ChainShape createEdgePolygonShape2(float ptm_ratio, cocos2d::Vec2* points, int count);
    };
}

#endif //__RB_SHAPEHELPER_H__
