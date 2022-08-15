#ifndef __BODYTYPES_DEMO_H__
#define __BODYTYPES_DEMO_H__

#include "cocos2d.h"
#include "box2d/include/box2d/box2d.h"
#include "Demos/BaseDemo.h"

USING_NS_CC;

//Different box2d body types
class BodyTypesDemo : public BaseDemo
{
public:
	static cocos2d::Scene* createScene();

	virtual bool init() override;


	CREATE_FUNC(BodyTypesDemo);

};

#endif // __BODYTYPES_DEMO_H__
