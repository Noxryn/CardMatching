#ifndef __CARDMATCHING_SCENE_H__
#define __CARDMATCHING_SCENE_H__

#include "cocos2d.h"

class CardMatching : public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();

    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);

    // implement the "static create()" method manually
    CREATE_FUNC(CardMatching);
};


#endif // __CARDMATCHING_SCENE_H__
