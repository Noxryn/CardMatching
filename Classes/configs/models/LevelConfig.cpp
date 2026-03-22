#include "LevelConfig.h"

void LevelConfig::addToMain(const CardConfig& card) {
    _mainDeck.push_back(card);
}

void LevelConfig::addToSide(const CardConfig& card) {
    _sideDeck.push_back(card);
}


