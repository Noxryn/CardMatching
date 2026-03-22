#ifndef __CARDMATCHING_SCENE_H__
#define __CARDMATCHING_SCENE_H__

#include "cocos2d.h"
#include "views/GameView.h"
#include "controllers/GameController.h"

class CardMatching : public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();
    void menuCloseCallback(cocos2d::Ref* pSender);

    CREATE_FUNC(CardMatching);
};


#endif // __CARDMATCHING_SCENE_H__
