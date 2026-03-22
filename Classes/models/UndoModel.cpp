#include "UndoModel.h"

UndoModel::~UndoModel()
{
    // 释放所有卡牌的克隆对象
    for (CardModel* card : mainDeck)
    {
        delete card;
    }
    mainDeck.clear();

    for (CardModel* card : sideDeck)
    {
        delete card;
    }
    sideDeck.clear();

    if (card)
    {
        delete card;
        card = nullptr;
    }
}


UndoModel* UndoModel::createFromModel(const GameModel* model)
{
    UndoModel* state = new UndoModel();

    // 克隆牌面区卡牌
    const std::vector<CardModel*>& playfieldCards = model->getMainDeck();
    for (const CardModel* card : playfieldCards)
    {
        state->mainDeck.push_back(cloneCard(card));
    }

    // 克隆牌堆区卡牌
    const std::vector<CardModel*>& stackCards = model->getSideDeck();
    for (const CardModel* card : stackCards)
    {
        state->sideDeck.push_back(cloneCard(card));
    }

    // 克隆托盘卡牌
    if (model->getCurrentCard())
    {
        state->card = cloneCard(model->getCurrentCard());
    }

    return state;
}

void UndoModel::restoreToModel(GameModel* model) const
{
    // 清空旧数据
    model->clear();

    // 恢复牌面区卡牌
    for (const CardModel* card : mainDeck)
    {
        model->addToMain(cloneCard(card));
    }

    // 恢复牌堆区卡牌
    for (const CardModel* card : sideDeck)
    {
        model->addToSide(cloneCard(card));
    }

    // 恢复托盘卡牌
    if (card)
    {
        model->setCurrentCard(cloneCard(card));
    }
}

CardModel* UndoModel::cloneCard(const CardModel* card)
{
    if (!card)
    {
        return nullptr;
    }

    // 创建新的卡牌对象，复制所有属性
    CardModel* clone = new CardModel(
        card->getId(),
        card->getFace(),
        card->getSuit(),
        card->getPosition()
    );

    clone->setOpen(card->isOpen());
    clone->setClick(card->isClick());

    return clone;
}
