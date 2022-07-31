#ifndef __B2_WORLDNODE_H__
#define __B2_WORLDNODE_H__

#include "cocos2d.h"
#include "box2d/include/box2d/b2_math.h"

USING_NS_CC;

class b2Body;
class b2World;
struct b2BodyDef;
struct b2JointDef;
class b2Joint;

namespace rb {

    class B2PhysicsBody;
    class DebugDrawNode;
 
    /// <summary>
    /// Cocos2d delegate for b2World.
    /// This is required in the scene graph to use box2d in cocos2d.
    /// Create physics components using this class only.
    /// </summary>
    class B2WorldNode : public cocos2d::Node
    {
    private:
        b2World* _b2World;
        cocos2d::Scheduler* custScheduler;

    public:
        static float PTM_RATIO;
        int velIters = 6;
        int posIters = 2;
        float timeStep = 1 / 60.f;

        B2WorldNode();
        ~B2WorldNode();
        virtual bool init();
        virtual void setPosition(float x, float y) override;
        virtual void updateWorld(float);

        inline b2Vec2 convertToB2Vector(cocos2d::Vec2 vec) { return b2Vec2(vec.x / PTM_RATIO, vec.y / PTM_RATIO); };

        inline cocos2d::Vec2 convertToCCVector(b2Vec2 vec) { return cocos2d::Vec2(vec.x * PTM_RATIO, vec.y * PTM_RATIO); };

        inline b2World* getPhysicsWorld() { return _b2World; };
        inline b2World* getB2World() { return _b2World; };

        void setDebugDrawNode(DebugDrawNode* dN);

        //static B2WorldNode* create(Vec2 ptm_ratio);
        static B2WorldNode* create(float ptm_ratio, Vec2 gravity);

        //Factory functions
        rb::B2PhysicsBody* createPhysicsBodyComp(b2BodyDef bodyDef);
        rb::B2PhysicsBody* createPhysicsBodyComp(const b2BodyDef* bodyDef);
        b2Joint* createJoint(b2JointDef* jointDef);

    };
}

#endif // __B2_WORLDNODE_H__
