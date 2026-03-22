// 游戏状态容器
#pragma once
#include "CardModel.h"
#include <vector>

class GameModel
{
public:
    GameModel()=default;
    ~GameModel();
    // 获取主牌区卡牌列表
    const std::vector<CardModel*>& getMainDeck() const { return _mainDeck; }
    // 获取备用牌堆卡牌列表
    const std::vector<CardModel*>& getSideDeck() const { return _sideDeck; }
    // 获取当前底牌
    CardModel* getCurrentCard() const { return _currentCard; }

    // 添加卡牌到主牌区
    void addToMain( CardModel* card);
    // 添加卡牌到备用牌堆
    void addToSide(CardModel* card);
    // 主牌区移除卡牌
    bool removeMainCard(int cardId);
    // 查找主牌区卡牌
    CardModel* findMainCard(int cardId) const;
    // 从备用牌堆取出顶部卡牌
    CardModel* getSideDeckTop();
    // 检查备用牌堆是否为空
    bool isSideEmpty() const { return _sideDeck.empty(); }
    // 设置底牌
    void setCurrentCard(CardModel* card);

    void clear() {
        _mainDeck.clear();
        _sideDeck.clear();
    }

private:
    // 主牌堆 
    std::vector<CardModel*> _mainDeck;
    // 备用牌堆
    std::vector<CardModel*> _sideDeck;
    // 当前底牌
    CardModel* _currentCard;               
};
