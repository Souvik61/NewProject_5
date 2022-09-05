#include "Demos/BaseDemo.h"
#include "Helpers.h"
#include "Phys/B2_WorldNode.h"
#include "Phys/DebugDrawNode.h"
#include "Phys/ShapeHelper.h"
#include "Phys/DefHelper.h"

USING_NS_CC;
using namespace rb;

Scene* BaseDemo::createScene()
{
    return BaseDemo::create();
}

bool BaseDemo::init()
{
    if ( !Scene::init() )
    {
        return false;
    }
    pTM = 100;
    zoomScale = 1;
    isM1Held = false;
    visibleSize = _director->getVisibleSize(); origin = _director->getVisibleOrigin();
    winSize = _director->getWinSizeInPixels();
    defaultCam = getDefaultCamera();

    camStart = defaultCam->getPosition();//test

    //Add mouse event listener
    mListener = EventListenerMouse::create();
    mListener->onMouseDown = CC_CALLBACK_1(BaseDemo::onMouseDown, this);
    mListener->onMouseUp = CC_CALLBACK_1(BaseDemo::onMouseUp, this);
    mListener->onMouseScroll = CC_CALLBACK_1(BaseDemo::onMouseScroll, this);
    mListener->onMouseMove = CC_CALLBACK_1(BaseDemo::onMouseMove, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(mListener, this);

    //Phys init stuff

    //Create world node
    wN = B2WorldNode::create(pTM, Vec2(0, 0));
    wN->retain();
    wN->getB2World()->SetContactListener(this);
    //this->addChild(wN, 9999999);

    //Set debug draw node
    auto dDN = DebugDrawNode::create();
    this->addChild(dDN, 55);
    dDN->SetFlags(b2Draw::e_shapeBit | b2Draw::e_jointBit);
    wN->setDebugDrawNode(dDN);


    //Create a dummy body
    b2BodyDef def = DefHelper::createWithPos(pTM);
    m_groundBody = wN->createPhysicsBodyComp(def);
    m_groundBody->retain();

    //Test

    /*auto sp = Sprite::create("HelloWorld.png");
    sp->setPosition(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y);
    addChild(sp);


    b2BodyDef bDef;
    DefHelper::initWithPos(bDef, pTM, Vec2(visibleSize.width / 2 + origin.x - 300, visibleSize.height / 2 + origin.y));
    bDef.type = b2_dynamicBody;
    bDef.bullet = true;

    auto bComp = wN->createPhysicsBodyComp(&bDef);

    b2PolygonShape bShape;

    ShapeHelper::initBoxShapeBy(bShape, pTM, 25, 25, Vec2(), 0);

    bComp->createFixture(&bShape, 40);

    auto box = Node::create();
    addChild(box);
    box->addComponent(bComp);*/

    return true;
}

void BaseDemo::onMouseDown(cocos2d::EventMouse* event)
{
    //Camera panning
    if (event->getMouseButton() == EventMouse::MouseButton::BUTTON_RIGHT)
    {
        isM1Held = true;
        prevTouch = touchStart = event->getLocationInView();
    }

    if (event->getMouseButton() == EventMouse::MouseButton::BUTTON_LEFT)
    {
        //Pick objects

        auto p = convertTouchToWorld(event->getLocationInView());
        m_mouseWorld = p;
        if (m_mouseJoint != NULL)
        {
            return;
        }

        // Make a small box.
        b2AABB aabb;
        Vec2 d = Vec2(0.1, 0.1);
        aabb.lowerBound = wN->convertToB2Vector(p - d);
        aabb.upperBound = wN->convertToB2Vector(p + d);

        // Query the world for overlapping shapes.
        QueryCallback callback(wN->convertToB2Vector(p));

        wN->getB2World()->QueryAABB(&callback, aabb);

        if (callback.m_fixture)
        {
            float frequencyHz = 5.0f;
            float dampingRatio = 0.7f;

            b2Body* body = callback.m_fixture->GetBody();
            b2MouseJointDef jd;
            jd.bodyA = m_groundBody->getBox2dBody();
            jd.bodyB = body;
            jd.target = wN->convertToB2Vector(p);
            jd.maxForce = 1000.0f * body->GetMass();

            b2LinearStiffness(jd.stiffness, jd.damping, frequencyHz, dampingRatio, jd.bodyA, jd.bodyB);

            m_mouseJoint = (b2MouseJoint*)wN->getB2World()->CreateJoint(&jd);
            body->SetAwake(true);
        }
    }
}

void BaseDemo::onMouseUp(cocos2d::EventMouse* event)
{
    //Touch handling
    if (event->getMouseButton() == EventMouse::MouseButton::BUTTON_RIGHT)
        isM1Held = false;

    //Mouse joint code
    if (m_mouseJoint)
    {
        wN->getB2World()->DestroyJoint(m_mouseJoint);
        m_mouseJoint = NULL;
    }
}

void BaseDemo::onMouseScroll(cocos2d::EventMouse* event)
{
    //zoomFactor += event->getScrollY();
    //zoomFactor = clampf(zoomFactor, 3.0f, 179.99f);
    //zoomScale = 60.0f / zoomFactor;

    (event->getScrollY() < 0) ? zoomScale /= 1.1f : zoomScale *= 1.1f;
    zoomScale = clampf(zoomScale, 0.1f, 2.66f);
    defaultCam->initPerspective(60 * zoomScale, winSize.width / winSize.height, 1, 1000);
}

void BaseDemo::onMouseMove(cocos2d::EventMouse* event)
{
    //Touch handling
    Vec2 dir, delta;
    if (isM1Held)
    {
        delta = prevTouch - event->getLocationInView();

        defaultCam->setPosition(defaultCam->getPosition() + (delta * zoomScale * zoomScale));
        camOffset = defaultCam->getPosition() - camStart;
        prevTouch = event->getLocationInView();
    }

    //mouse joint move
    Vec2 p = convertTouchToWorld(event->getLocationInView());
    m_mouseWorld = p;

    if (m_mouseJoint)
    {
        m_mouseJoint->SetTarget(Utilities::convertToB2Vec2(100, p));
    }
}