// ³·Ïú¿ìÕÕ
#pragma once
#include "models/CardModel.h"
#include "models/GameModel.h"
#include <vector>

struct UndoModel
{
    std::vector<CardModel*> mainDeck;  
    std::vector<CardModel*> sideDeck;      
    CardModel* card;                  

    UndoModel() = default;
    ~UndoModel();

    static UndoModel* createFromModel(const GameModel* model);
    void restoreToModel(GameModel* model) const;
    static CardModel* cloneCard(const CardModel* card);
};



