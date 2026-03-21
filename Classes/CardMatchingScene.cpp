#include "CardMatchingScene.h"
#include "SimpleAudioEngine.h"

USING_NS_CC; // 使用 Cocos2d-x 命名空间 (cocos2d)

// 创建场景的静态方法，供外部调用
Scene* CardMatching::createScene()
{
    return CardMatching::create();
}

// 当资源文件加载失败时，打印有用的错误信息，防止程序直接崩溃 (segfault)
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

// 初始化函数：在场景创建时自动调用，用于设置场景内容
bool CardMatching::init()
{
    //////////////////////////////
    // 1. 首先初始化父类 (Scene)
    if (!Scene::init())
    {
        return false;
    }

    // 获取可见屏幕的大小和原点坐标（适配不同分辨率）
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    /////////////////////////////
    // 2. 添加一个带有 "X" 图片的菜单项，点击后退出程序
    //    你可以根据需要修改这部分代码

    // 创建一个自动释放 (autorelease) 的关闭图标菜单项
    // 参数分别是：正常状态图片，选中状态图片，回调函数
    auto closeItem = MenuItemImage::create(
        "CloseNormal.png",
        "CloseSelected.png",
        CC_CALLBACK_1(CardMatching::menuCloseCallback, this));

    // 检查图片是否加载成功
    if (closeItem == nullptr ||
        closeItem->getContentSize().width <= 0 ||
        closeItem->getContentSize().height <= 0)
    {
        problemLoading("'CloseNormal.png' and 'CloseSelected.png'");
    }
    else
    {
        // 计算位置：将按钮放置在屏幕右上角
        float x = origin.x + visibleSize.width - closeItem->getContentSize().width / 2;
        float y = origin.y + closeItem->getContentSize().height / 2;
        closeItem->setPosition(Vec2(x, y));
    }

    // 创建菜单对象，将关闭按钮加入其中，设置为自动释放
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Vec2::ZERO); // 菜单自身位置设为原点，子项位置相对菜单计算
    this->addChild(menu, 1); // 将菜单添加到当前场景，层级为 1

    /////////////////////////////
    // 3. 在这里添加你的游戏代码...

    // 添加一个显示标签
    // 创建并初始化标签，使用 TTF 字体
    auto label = Label::createWithTTF("CardMatching", "fonts/Marker Felt.ttf", 24);
    if (label == nullptr)
    {
        problemLoading("'fonts/Marker Felt.ttf'");
    }
    else
    {
        // 将标签定位在屏幕顶部中心
        label->setPosition(Vec2(origin.x + visibleSize.width / 2,
            origin.y + visibleSize.height - label->getContentSize().height));

        // 将标签作为子节点添加到当前层
        this->addChild(label, 1);
    }

    //// 添加一张名为 "3.png" 的图片作为背景或展示图 (原注释说是 splash screen)
    //auto sprite = Sprite::create("3.png");
    //if (sprite == nullptr)
    //{
    //    problemLoading("'3.png'");
    //}
    //else
    //{
    //    // 将图片定位在屏幕正中心
    //    sprite->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));

    //    // 将图片作为子节点添加到当前层，层级为 0 (在标签和菜单下方)
    //    this->addChild(sprite, 0);
    //}

    // 初始化成功
    return true;
}

// 菜单关闭回调函数：当用户点击关闭按钮时触发
void CardMatching::menuCloseCallback(Ref* pSender)
{
    // 结束 Cocos2d-x 游戏场景并退出应用程序
    Director::getInstance()->end();

}