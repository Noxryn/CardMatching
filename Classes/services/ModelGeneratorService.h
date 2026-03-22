// 模型生成

#pragma once

#include "models/GameModel.h"
#include "configs/models/LevelConfig.h"
#include "views/GameView.h"

class ModelGenerator
{
public:
    static void generateFromLevelConfig(const LevelConfig* levelConfig, GameModel* gameModel);

private:
    
    ModelGenerator() = delete;
    ~ModelGenerator() = delete;
};