// 加载解析配置文件
#pragma once

#include "configs/models/LevelConfig.h"
#include <string>
#include "json/document.h" // 自带的Json解析库

class LevelConfigLoader {
public:
	// 从指定路径加载关卡配置文件，并返回解析后的 LevelConfig 对象
    static LevelConfig* loadFromFile(const std::string& filePath);
	// 解析单张卡牌的 JSON 对象，并返回对应的 CardConfig 结构体
    static CardConfig parseCardConfig(const rapidjson::Value& jsonCard);
};
