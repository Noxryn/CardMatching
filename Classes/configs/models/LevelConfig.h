// 定义关卡的静态数据结构
#pragma once

#include "cocos2d.h"
#include <vector>
#include <string>

#include "utils/CardDefines.h"

// 卡牌数据结构
struct CardConfig {
	CardSuitType suitType;        // 花色类型
    CardFaceType faceType;       // 点数类型
    cocos2d::Vec2 position;      // 卡牌位置
    CardConfig()
        : faceType(CFT_NONE)
        , suitType(CST_NONE)
        , position(cocos2d::Vec2::ZERO) {}
};

// 关卡配置类
class LevelConfig {
private:
    // 主牌堆 
    std::vector<CardConfig> _mainDeck; 
    // 备用牌堆
    std::vector<CardConfig> _sideDeck;
public:
    LevelConfig() = default;
	~LevelConfig() = default;

    // 获取主牌区卡牌列表
    const std::vector<CardConfig>& getMainDeck() const { return _mainDeck; }
    // 获取备用牌堆卡牌列表
    const std::vector<CardConfig>& getSideDeck() const { return _sideDeck; }
	// 添加卡牌到主牌区
    void addToMain(const CardConfig& card);
    // 添加卡牌到备用牌堆
    void addToSide(const CardConfig& card);
    void clear() {
        _mainDeck.clear();
        _sideDeck.clear();
    }
};