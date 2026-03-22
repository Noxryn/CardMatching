#include "MatchRule.h"
bool isMatching(const CardModel* Card, const CardModel* otherCard)
{
    if (!Card || !otherCard)
    {
        return false;
    }

    int faceDiff = std::abs(static_cast<int>(Card->getFace()) - static_cast<int>(otherCard->getFace()));

    return faceDiff == 1;
}