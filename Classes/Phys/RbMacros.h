#ifndef __RB_MACROS_H__
#define __RB_MACROS_H__

#define RB_PTM 100

#ifdef RB_PTM

//Conversion functions

//Convert b2Vec2 to cocos2d::Vec2 with ptm ratio
#define B2TOCCVEC2(v) cocos2d::Vec2(v.x*RB_PTM,v.y*RB_PTM)

//Convert cocos2d::Vec2 to b2Vec2 with ptm ratio
#define CCTOB2VEC2(v) b2Vec2(v.x/RB_PTM,v.y/RB_PTM)



#endif // RB_PTM


#endif //__RB_MACROS_H__
