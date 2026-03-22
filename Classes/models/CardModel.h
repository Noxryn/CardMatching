// 单张卡牌的数据实体
#pragma once
#include "cocos2d.h"
#include "utils/CardDefines.h"

class CardModel {
public:
    CardModel::CardModel(int id, CardFaceType face, CardSuitType suit, const cocos2d::Vec2& position)
        : _id(id)
        , _face(face)
        , _suit(suit)
        , _position(position)
        , _isOpen(false)
        , _isClick(false)
    {
    }
    ~CardModel() = default;
    int getId() const { return _id; }
    CardFaceType getFace() const { return _face; }
    CardSuitType getSuit() const { return _suit; }
    const cocos2d::Vec2& getPosition() const { return _position; }
    bool isOpen() const { return _isOpen; }
    bool isClick() const { return _isClick; }

    void setPosition(const cocos2d::Vec2& position) { _position = position; }
    void setOpen(bool open) { _isOpen = open; }
    void setClick(bool click) { _isClick = click; }

private:
    int _id;                        // 卡牌ID
    CardFaceType _face;             // 卡牌点数
    CardSuitType _suit;             // 卡牌花色
    cocos2d::Vec2 _position;        // 卡牌位置
    bool _isOpen;                   // 是否翻开
    bool _isClick;                  // 是否可点击

};