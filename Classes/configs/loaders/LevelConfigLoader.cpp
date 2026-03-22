#include "LevelConfigLoader.h"
#include "cocos2d.h"
#include <iostream>

USING_NS_CC;

LevelConfig* LevelConfigLoader::loadFromFile(const std::string& filePath) {
    LevelConfig* config = new(std::nothrow) LevelConfig();

    std::string jsonContent = FileUtils::getInstance()->getStringFromFile(filePath);

    if (jsonContent.empty()) {
        CCLOG("Error: Failed to read level config file: %s", filePath.c_str());
        return nullptr; 
    }


    rapidjson::Document d;
    if (d.Parse(jsonContent.c_str()).HasParseError()) {
        std::cout << "无法解析文件中的json" << std::endl;
        return nullptr;
    }


    // 解析主牌堆
    if (d.HasMember("Playfield") && d["Playfield"].IsArray()) {
        const rapidjson::Value& mainDeckArray = d["Playfield"];
        for (rapidjson::SizeType i = 0; i < mainDeckArray.Size(); i++) {
            if (mainDeckArray[i].IsObject()) {
                CardConfig card = parseCardConfig(mainDeckArray[i]);
                config->addToMain(card);
            }
        }
    }
    else {
        std::cout << "无主牌堆" << std::endl;
    }

    // 解析备用牌堆
    if (d.HasMember("Stack") && d["Stack"].IsArray()) {
        const  rapidjson::Value& sideDeckArray = d["Stack"];
        for (rapidjson::SizeType i = 0; i < sideDeckArray.Size(); i++) {
            if (sideDeckArray[i].IsObject()) {
                CardConfig card = parseCardConfig(sideDeckArray[i]);
                config->addToSide(card);
            }
        }
    }
    else {
        std::cout << "无备用牌堆" << std::endl;
    }

    return config;
}

CardConfig LevelConfigLoader::parseCardConfig(const rapidjson::Value& jsonCard) {
    CardConfig card;

    
    // 解析花色
    if (jsonCard.HasMember("CardSuit") && jsonCard["CardSuit"].IsInt()) {
        card.suitType = static_cast<CardSuitType>(jsonCard["CardSuit"].GetInt());
    }

    // 解析点数
    if (jsonCard.HasMember("CardFace") && jsonCard["CardFace"].IsInt()) {
        card.faceType = static_cast<CardFaceType>(jsonCard["CardFace"].GetInt());
    }

    // 解析位置
    if (jsonCard.HasMember("Position") && jsonCard["Position"].IsObject())
    {
        const rapidjson::Value& posObj = jsonCard["Position"];
        float x = posObj.HasMember("x") ? posObj["x"].GetFloat() : 0.0f;
        float y = posObj.HasMember("y") ? posObj["y"].GetFloat() : 0.0f;
        card.position = Vec2(x, y);
    }

    return card;
}

