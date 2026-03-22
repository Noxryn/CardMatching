#include "ModelGeneratorService.h"

USING_NS_CC;

void ModelGenerator::generateFromLevelConfig(const LevelConfig* levelConfig, GameModel* gameModel)
{
    if (!levelConfig || !gameModel)
    {
        CCLOG("GameModelGenerator::generateFromLevelConfig - Invalid parameters");
        return;
    }

    gameModel->clear();

    const std::vector<CardConfig>& mainDeck = levelConfig->getMainDeck();
    int cardIdCounter = 1;  // 卡牌ID计数器，从1开始

    // 转换牌面区卡牌
    for (const CardConfig& cardData : mainDeck)
    {
        CardModel* card = new CardModel(
            cardIdCounter++,        // 自动生成ID
            cardData.faceType,
            cardData.suitType,
            cardData.position       // 使用position（Vec2类型）
        );
        card->setOpen(false);      // 牌面卡牌默认正面朝上
        card->setClick(true);   // 默认可点击
        gameModel->addToMain(card);
    }

    // 转换牌堆区卡牌
    const std::vector<CardConfig>& sideDeck = levelConfig->getSideDeck();
    for (const CardConfig& cardData : sideDeck)
    {
        CardModel* card = new CardModel(
            cardIdCounter++,        
            cardData.faceType,
            cardData.suitType,
            Vec2(0, 0)             
        );
        card->setOpen(false);    
        card->setClick(false);  
        gameModel->addToSide(card);
    }
}
