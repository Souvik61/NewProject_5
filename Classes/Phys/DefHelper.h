#ifndef __RB_DEFHELPER_H__
#define __RB_DEFHELPER_H__

#include "cocos2d.h"
#include "box2d/include/box2d/box2d.h"
#include "Utilities.h"
#include "Phys/B2_PhysicsBody.h"

USING_NS_CC;

//struct b2BodyDef;
//struct b2DistanceJointDef;
//struct b2RevoluteJointDef;
//struct b2PrismaticJointDef;
//struct b2PulleyJointDef;
//struct b2WheelJointDef;
//struct b2FrictionJointDef;

namespace rb {

    class B2PhysicsBody;

    class DefHelper
    {
    public:
        /*
        *Create a body def with initial position
        */
        static b2BodyDef createWithPos(float ptm_ratio, cocos2d::Vec2 position = cocos2d::Vec2::ZERO);

        //init body def given the reference
        static void initWithPos(b2BodyDef& def, float ptm_ratio, const cocos2d::Vec2& position);

        //Joint helpers

        /*Create a b2DistantJointDef*/
        static b2DistanceJointDef createDistanceJointDef(float ptm_ratio, B2PhysicsBody* bodyA, B2PhysicsBody* bodyB, cocos2d::Vec2 anchorA, cocos2d::Vec2 anchorB);
        static b2RevoluteJointDef createRevoluteJointDef(float ptm_ratio, B2PhysicsBody* bodyA, B2PhysicsBody* bodyB, cocos2d::Vec2 anchor);
        static b2PrismaticJointDef createPrismaticJointDef(float ptm_ratio, B2PhysicsBody* bodyA, B2PhysicsBody* bodyB, cocos2d::Vec2 anchor, cocos2d::Vec2 axis);
        static b2PulleyJointDef createPulleyJointDef(float ptm_ratio, B2PhysicsBody* bodyA, B2PhysicsBody* bodyB, cocos2d::Vec2 groundAnchorA, cocos2d::Vec2 groundAnchorB, cocos2d::Vec2 anchorA, cocos2d::Vec2 anchorB, float ratio);
        static b2WheelJointDef createWheelJointDef(float ptm_ratio, B2PhysicsBody* bodyA, B2PhysicsBody* bodyB, Vec2 worldAnchor, Vec2 worldAxis);

        //static b2FrictionJointDef createFrictionJointDef(float ptm_ratio, B2PhysicsBody* bodyA, B2PhysicsBody* bodyB, Vec2 anchor);

        static inline b2FrictionJointDef rb::DefHelper::createFrictionJointDef(float ptm, B2PhysicsBody* bodyA, B2PhysicsBody* bodyB, Vec2 anch)
        {
            b2FrictionJointDef def;
            def.Initialize(bodyA->getBox2dBody(), bodyB->getBox2dBody(), Utilities::convertToB2Vec2(ptm, anch));
            return def;
        }
    
        //Joint helpers --pass by reference
        
        static void initDistanceJointDef(float ptm_ratio, b2DistanceJointDef& joint, B2PhysicsBody* bodyA, B2PhysicsBody* bodyB, const cocos2d::Vec2& anchorA, const cocos2d::Vec2& anchorB);
        
        static void initRevoluteJointDef(float ptm_ratio,b2RevoluteJointDef& joint, B2PhysicsBody* bodyA, B2PhysicsBody* bodyB, const cocos2d::Vec2& anchor);
        
        static void initPrismaticJointDef(float ptm_ratio, b2PrismaticJointDef& def, B2PhysicsBody* bodyA, B2PhysicsBody* bodyB, cocos2d::Vec2 anchor, cocos2d::Vec2 axis);

        //Others

        //static void setMassDataDef(b2MassData data,);
    
    };
}

#endif //__RB_DEFHELPER_H__
