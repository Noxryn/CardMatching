// 卡牌美术资源映射
#include "utils/CardDefines.h"
#include <string>

// 卡牌资源配置管理器
class CardResConfig { 
public:
    // 获取卡牌底图路径
    static std::string getCardGeneralPath();

    // 获取指定点数图片路径
    static std::string getNumberPath(CardFaceType face, CardSuitType suit, bool isBig = true);

    // 获取指定花色图片路径
    static std::string getSuitPath(CardSuitType suit);

    // 将点数枚举转换为字符串
    static std::string faceToString(CardFaceType face);

    // 将花色枚举转换为字符串
    static std::string suitToString(CardSuitType suit);
private:
    static const std::string kNumber;       // 点数图片路径
    static const std::string ksuits;    // 花色图片路径
};
