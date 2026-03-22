#include "CardMatchingScene.h"
#include "SimpleAudioEngine.h"

USING_NS_CC; 

// 创建场景的静态方法，供外部调用
Scene* CardMatching::createScene()
{
    return CardMatching::create();
}

// 打印错误信息
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

// 初始化函数
bool CardMatching::init()
{
    if (!Scene::init())
    {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    auto closeItem = MenuItemImage::create(
        "CloseNormal.png",
        "CloseSelected.png",
        CC_CALLBACK_1(CardMatching::menuCloseCallback, this));

    if (closeItem == nullptr ||
        closeItem->getContentSize().width <= 0 ||
        closeItem->getContentSize().height <= 0)
    {
        problemLoading("'CloseNormal.png' and 'CloseSelected.png'");
    }
    else
    {
        float x = origin.x + visibleSize.width - closeItem->getContentSize().width / 2;
        float y = origin.y + closeItem->getContentSize().height / 2;
        closeItem->setPosition(Vec2(x, y));
    }

    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Vec2::ZERO); 
    this->addChild(menu, 1); 

    /////////////////////////////
    auto gameView = GameView::createWithLevel("../Resources/res/levels/levels.json");
    if (!gameView)
    {
        CCLOG("创建GameView失败");
        return false;
    }

    // 将游戏视图添加到场景
    this->addChild(gameView);

    // 创建游戏控制器并连接到GameView
    auto gameController = new GameController(gameView);
    gameView->setGameController(gameController);

    // 刷新显示（显示初始托盘卡牌）
    gameView->refreshDisplay();
    
    
    /////////////////////////////

    return true;
}

// 菜单关闭回调函数
void CardMatching::menuCloseCallback(Ref* pSender)
{
    Director::getInstance()->end();
}