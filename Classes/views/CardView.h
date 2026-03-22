// 单张卡牌的 UI
#pragma once
#include "cocos2d.h"
#include "models/CardModel.h"

// 卡牌UI界面
class CardView : public cocos2d::Node
{
public:
    // 创建卡牌视图
    static CardView* create(const CardModel* cardModel);
    // 初始化卡牌视图
    bool init(const CardModel* cardModel);
    // 更新卡牌显示
    void updateCard(const CardModel* cardModel);
    // 移动卡牌
    void moveToPosition(const cocos2d::Vec2& targetPos, float duration = 0.3f);

private:
    CardView() = default;
    virtual ~CardView() = default;

    cocos2d::Sprite* _generalPng;
    cocos2d::Sprite* _smallNumberPng;  
    cocos2d::Sprite* _bigNumberPng;
    cocos2d::Sprite* _suitPng;          
    const CardModel* _cardModel;       
};

