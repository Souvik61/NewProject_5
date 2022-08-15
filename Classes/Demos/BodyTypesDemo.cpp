#include "Demos/BodyTypesDemo.h"
#include "Phys/SuperPhysHelper.h"
#include "Phys/B2_PhysicsBody.h"
#include "Phys/DefHelper.h"
#include "Phys/ShapeHelper.h"

USING_NS_CC;
using namespace rb;

Scene* BodyTypesDemo::createScene()
{
    return BodyTypesDemo::create();
}

bool BodyTypesDemo::init()
{
    if (!BaseDemo::init())
    {
        return false;
    }

    //auto s1 = _director->getVisibleSize();
    //auto s2 = _director->getWinSizeInPixels();
    //auto s3 = _director->getWinSize();
    //auto s4 = _director->getOpenGLView()->getDesignResolutionSize();
    //
    //CCLOG("Visible size:x:%f, y:%f", s1.width, s1.height);
    //CCLOG("WinSize in pixels:x:%f, y:%f", s2.width, s2.height);
    //CCLOG("WinSize size:x:%f, y:%f", s3.width, s3.height);
    //CCLOG("Design size:x:%f, y:%f", s4.width, s4.height);

    wN->getB2World()->SetGravity(b2Vec2(0, -9));

    Node* attachNode = Node::create();
    addChild(attachNode);
    B2PhysicsBody* attachComp, * groundComp;

    //Create ground
    {
        b2BodyDef bd;
        groundComp = wN->createPhysicsBodyComp(&bd);

        b2EdgeShape eshape;
        ShapeHelper::initEdgeShapeSetTwoSided(eshape, pTM, Vec2(-500, 0), Vec2(500, 0));

        b2FixtureDef fd;
        fd.shape = &eshape;

        groundComp->createFixture(&fd);

        auto ground = Node::create();
        addChild(ground);
        ground->addComponent(groundComp);

        groundComp->setPosition(visibleSize.width / 2 + origin.x, origin.y);
    }

    //Create attachment
    {
        b2BodyDef bd;
        DefHelper::initWithPos(bd, pTM, Vec2(visibleSize.width / 2 + origin.x, 300));
        bd.type = b2_dynamicBody;
        attachComp = wN->createPhysicsBodyComp(&bd);
        attachNode->addComponent(attachComp);

        b2PolygonShape shape;
        ShapeHelper::initPolygonShapeWithBox(shape, pTM, 50, 200);
        attachComp->createFixture(&shape, 2.0f);
    }

    //Create platform
    {
        b2BodyDef bd1;
        DefHelper::initWithPos(bd1, pTM, Vec2((visibleSize.width / 2 + origin.x) - 400, origin.y + 500));
        bd1.type = b2_dynamicBody;
        auto platComp = wN->createPhysicsBodyComp(&bd1);//Creates the body

        b2PolygonShape shape;
        ShapeHelper::initPolygonShapeWithBox(shape, pTM, 50, 400, Vec2(400, 0), 0.5 * b2_pi);

        b2FixtureDef fd1;
        fd1.shape = &shape;
        fd1.friction = 0.6f;
        fd1.density = 2.0f;
        platComp->createFixture(&fd1);

        auto box = Node::create();
        box->addComponent(platComp);
        addChild(box);
        //platComp->setPosition(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y);

        //Create joints

        b2RevoluteJointDef rjd;
        DefHelper::initRevoluteJointDef(pTM, rjd, attachComp, platComp, Vec2(visibleSize.width / 2 + origin.x, origin.y + 500));
        rjd.maxMotorTorque = 100;
        rjd.enableMotor = true;
        wN->createJoint(&rjd);


        b2PrismaticJointDef pjd;
        //pjd.Initialize(ground, m_platform, b2Vec2(0.0f, 5.0f), b2Vec2(1.0f, 0.0f));

        DefHelper::initPrismaticJointDef(pTM, pjd, groundComp, platComp, Vec2(visibleSize.width / 2 + origin.x, origin.y + 500), Vec2::UNIT_X);
        pjd.maxMotorForce = 1000.0f;
        pjd.enableMotor = true;
        pjd.lowerTranslation = Utilities::convertToB2Float(100, -1000.0f);
        pjd.upperTranslation = Utilities::convertToB2Float(100, 1000.0f);
        pjd.enableLimit = true;

        wN->createJoint(&pjd);
    }


    return true;
}