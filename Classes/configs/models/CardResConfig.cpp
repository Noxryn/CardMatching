#include "CardResConfig.h"
#include <iostream>

const std::string CardResConfig::kNumber = "../Resources/res/number/";
const std::string CardResConfig::ksuits = "../Resources/res/suits/";

std::string CardResConfig::getCardGeneralPath()
{
	return "../Resources/res/card_general.png";
}

std::string CardResConfig::getNumberPath(CardFaceType face, CardSuitType suit, bool isBig)
{
    std::string sizePrefix = isBig ? "big" : "small";
    std::string colorStr = "black";
    if (suit == CST_HEARTS || suit == CST_DIAMONDS) {
        colorStr = "red";
    }
    std::string faceStr = faceToString(face);
    if (faceStr.empty()) {
        std::cout << "error no faceStr" << std::endl;
        return "";
    }
    std::string fileName = sizePrefix + "_" + colorStr + "_" + faceStr + ".png";

    return kNumber+ fileName;
}

std::string CardResConfig::getSuitPath(CardSuitType suit)
{
	return ksuits + suitToString(suit) + ".png";
}

std::string CardResConfig::faceToString(CardFaceType face)
{
    switch (face) {
    case CFT_ACE:   return "A";
    case CFT_TWO:   return "2";
    case CFT_THREE: return "3";
    case CFT_FOUR:  return "4";
    case CFT_FIVE:  return "5";
    case CFT_SIX:   return "6";
    case CFT_SEVEN: return "7";
    case CFT_EIGHT: return "8";
    case CFT_NINE:  return "9";
    case CFT_TEN:   return "10";
    case CFT_JACK:  return "J";
    case CFT_QUEEN: return "Q";
    case CFT_KING:  return "K";
    default:        return "";
    }
}
std::string CardResConfig::suitToString(CardSuitType suit)
 {
        switch (suit) {
        case CST_CLUBS:    return "club";
        case CST_DIAMONDS: return "diamond";
        case CST_HEARTS:   return "heart";
        case CST_SPADES:   return "spade";
        default:           return "";
        }
}