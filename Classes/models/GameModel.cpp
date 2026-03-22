#include "GameModel.h"
#include <algorithm> 

void GameModel::addToMain(CardModel* card)
{
    if(card)
        _mainDeck.push_back(card);
}

void GameModel::addToSide(CardModel* card)
{
    if(card)
    _sideDeck.push_back(card);
}

bool GameModel::removeMainCard(int cardId)
{
    for (auto it = _mainDeck.begin(); it != _mainDeck.end(); ++it)
    {
        if (*it && (*it)->getId() == cardId)
        {

            _mainDeck.erase(it);
            return true;
        }
    }
    return false;
}

CardModel* GameModel::findMainCard(int cardId) const
{
    for (const auto& card : _mainDeck)
    {
        if (card && card->getId() == cardId) 
        {
            return card;
        }
    }
    return nullptr;
}

CardModel* GameModel::getSideDeckTop()
{
    if (_sideDeck.empty())
    {
        return nullptr;
    }
    CardModel* topCard = _sideDeck.back();
    _sideDeck.pop_back();

    return topCard;
}

GameModel::~GameModel()
{
    for (auto card : _mainDeck)
    {
        delete card;
    }
    _mainDeck.clear();

    for (auto card : _sideDeck)
    {
        delete card;
    }
    _sideDeck.clear();

    if (_currentCard)
    {
        delete _currentCard;
        _currentCard = nullptr;
    }
}

void GameModel::setCurrentCard(CardModel* card)
{
    if (_currentCard && _currentCard != card)
    {
        delete _currentCard;
    }
    _currentCard = card;
}