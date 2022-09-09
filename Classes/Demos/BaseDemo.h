#ifndef __BASE_DEMO_H__
#define __BASE_DEMO_H__

#include "cocos2d.h"
#include "box2d/include/box2d/box2d.h"

USING_NS_CC;

namespace rb{
    class B2WorldNode;
	class B2PhysicsBody;
}

//Base class for demos
//Includes camera panning,zooming
//Click to drag bodies
class BaseDemo : public cocos2d::Scene, public b2ContactListener
{
	//Mouse joint query callback class
	class QueryCallback : public b2QueryCallback
	{
	public:
		QueryCallback(const b2Vec2& point)
		{
			m_point = point;
			m_fixture = NULL;
		}

		bool ReportFixture(b2Fixture* fixture) override
		{
			b2Body* body = fixture->GetBody();
			if (body->GetType() == b2_dynamicBody)
			{
				bool inside = fixture->TestPoint(m_point);
				if (inside)
				{
					m_fixture = fixture;

					// We are done, terminate the query.
					return false;
				}
			}
			// Continue the query.
			return true;
		}

		b2Vec2 m_point;
		b2Fixture* m_fixture;
	};

private:
	Vec2 m_mouseWorld;
	b2MouseJoint* m_mouseJoint;
	rb::B2PhysicsBody* m_groundBody;
protected:
	int pTM;
	bool isM1Held;
	float zoomScale;
	Camera* defaultCam;
	Vec2 touchStart, prevTouch, origin;
	Size visibleSize, winSize;
	EventListenerMouse* mListener;
	EventListenerKeyboard* kListener;
	rb::B2WorldNode* wN;

	//test
	Vec2 camStart, camOffset;
public:
	~BaseDemo();

	static cocos2d::Scene* createScene();

	virtual bool init();

	virtual void onRestart() {};

	// implement the "static create()" method manually
	CREATE_FUNC(BaseDemo);


	//Input functions

	//Mouse Inputs
	void onMouseDown(cocos2d::EventMouse* event);
	void onMouseMove(cocos2d::EventMouse* event);
	void onMouseUp(cocos2d::EventMouse* event);
	void onMouseScroll(cocos2d::EventMouse* event);

	//Keyboard inputs
	virtual void onKeyDown(cocos2d::EventKeyboard::KeyCode,cocos2d::Event*);
	virtual void onKeyUp(cocos2d::EventKeyboard::KeyCode,cocos2d::Event*);

	//test

	Vec2 convertTouchToWorld(const Vec2& inp)
	{
		return inp + camOffset;
	}

};

#endif // __BASE_DEMO_H__
