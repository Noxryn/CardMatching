#include "GameView.h"
#include "configs/loaders/LevelConfigLoader.h"
#include "utils/CardDefines.h"
#include "controllers/GameController.h"
#include "services/ModelGeneratorService.h"

USING_NS_CC;

// 主牌区
const float GameView::PLAYFIELD_START_X = 0.0f;   
const float GameView::PLAYFIELD_START_Y = 580.0f;  

// 手牌区
const float GameView::HAND_AREA_Y = 0.0f;          
const float GameView::STACK_START_X = 50.0f;       
const float GameView::STACK_START_Y = 150.0f;      
const float GameView::STACK_OFFSET_X = 40.0f;      
const float GameView::TRAY_X = 540.0f;             
const float GameView::TRAY_Y = 150.0f;             
const float GameView::UNDO_BUTTON_X = 880.0f;     
const float GameView::UNDO_BUTTON_Y = 290.0f;       

GameView::GameView()
    : _gameModel(nullptr)
    , _gameController(nullptr)
    , _mainDeckView(nullptr)
    , _undoLabel(nullptr)
{
}

GameView::~GameView()
{
    CC_SAFE_DELETE(_gameModel);
}

GameView* GameView::createWithLevel(const std::string& levelPath)
{
    GameView* ret = new (std::nothrow) GameView();
    if (ret && ret->init())
    {
        ret->autorelease();

        // 加载关卡
        if (!ret->loadLevel(levelPath))
        {
            CCLOG("GameView::createWithLevel - Failed to load level: %s", levelPath.c_str());
            return nullptr;
        }

        return ret;
    }
    else
    {
        CC_SAFE_DELETE(ret);
        return nullptr;
    }
}

bool GameView::init()
{
    if (!Layer::init())
    {
        return false;
    }

    // 创建游戏数据模型
    _gameModel = new GameModel();

    // 创建背景
    createBackground();

    // 创建回退按钮
    createUndoButton();

    // 设置触摸事件监听
    setupTouchListener();

    // 设置键盘事件监听
    setupKeyboardListener();

    return true;
}

bool GameView::loadLevel(const std::string& levelPath)
{
    // 使用LevelConfigLoader加载关卡配置
    LevelConfig* levelConfig = LevelConfigLoader::loadFromFile(levelPath);
    if (!levelConfig)
    {
        CCLOG("GameView::loadLevel - Failed to load level config");
        return false;
    }

    // 静态配置转换为运行时数据
    ModelGenerator::generateFromLevelConfig(levelConfig, _gameModel);

    // 释放关卡配置
    delete levelConfig;

    // 刷新显示
    refreshDisplay();

    return true;
}

void GameView::refreshDisplay()
{
    // 清除旧的CardView
    clearAllCardViews();
    const std::vector<CardModel*>& playfieldCards = _gameModel->getMainDeck();
    for (CardModel* cardModel : playfieldCards)
    {
        CardView* cardView = createCardView(cardModel);
        _mainCardViews[cardModel->getId()] = cardView;
    }
    if (_gameModel->getCurrentCard() != nullptr)
    {
        _mainDeckView = createCardView(_gameModel->getCurrentCard());
    }

    const std::vector<CardModel*>& stackCards = _gameModel->getSideDeck();
    for (size_t i = 0; i < stackCards.size(); i++)
    {
        CardModel* cardModel = stackCards[i];

        CardView* cardView = createCardView(cardModel);
        float xPos = STACK_START_X + i * STACK_OFFSET_X;
        float yPos = STACK_START_Y;
        cardView->setPosition(xPos, yPos);

        // 保存到备用牌堆视图映射表
        _sideCardViews[cardModel->getId()] = cardView;
    }
}

CardView* GameView::createCardView(const CardModel* cardModel)
{
    CardView* cardView = CardView::create(cardModel);
    if (cardView)
    {
        this->addChild(cardView, 1); 
    }
    return cardView;
}

void GameView::clearAllCardViews()
{
    // 移除所有牌面卡牌视图
    for (auto& pair : _mainCardViews)
    {
        this->removeChild(pair.second);
    }
    _mainCardViews.clear();

    // 移除托盘卡牌视图
    if (_mainDeckView)
    {
        this->removeChild(_mainDeckView);
        _mainDeckView = nullptr;
    }

    // 移除备用牌堆卡牌视图
    for (auto& pair : _sideCardViews)
    {
        this->removeChild(pair.second);
    }
    _sideCardViews.clear();
}

void GameView::setupTouchListener()
{
    // 创建单点触摸监听器
    auto listener = EventListenerTouchOneByOne::create();

    // 设置触摸事件回调
    listener->onTouchBegan = [this](Touch* touch, Event* event) -> bool
        {
            Vec2 touchPos = touch->getLocation();

            
            for (auto it = _sideCardViews.rbegin(); it != _sideCardViews.rend(); ++it)
            {
                CardView* cardView = it->second;
                if (cardView->getBoundingBox().containsPoint(touchPos))
                {
                    this->onStackCardClicked(it->first);  // 传递卡牌ID
                    return true;
                }
            }

            
            for (auto& pair : _mainCardViews)
            {
                CardView* cardView = pair.second;
                if (cardView->getBoundingBox().containsPoint(touchPos))
                {
                    this->onCardClicked(pair.first);  // 传递卡牌ID
                    return true;
                }
            }

            return false;
        };

    // 注册监听器
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

void GameView::createBackground()
{
    // 创建主游戏区背景（顶部1080x1500，棕色）
    auto playfieldBg = LayerColor::create(Color4B(139, 90, 43, 255), 1080, 1500);
    playfieldBg->setPosition(Vec2(0, 580));  // 从y=580开始到顶部
    this->addChild(playfieldBg, -1);  // z-order = -1，在最底层

    // 创建手牌区背景（底部1080x580，紫色）
    auto handAreaBg = LayerColor::create(Color4B(148, 0, 211, 255), 1080, 580);
    handAreaBg->setPosition(Vec2(0, 0));  // 从底部开始
    this->addChild(handAreaBg, -1);  // z-order = -1，在最底层
}

void GameView::setupKeyboardListener()
{
    // 键盘监听器已禁用，防止Z键误触发回退
    // 使用回退文字按钮进行回退操作
    CCLOG("GameView::setupKeyboardListener - Keyboard shortcuts disabled, use undo button instead");
}

void GameView::onCardClicked(int cardId)
{
    CCLOG("GameView::onCardClicked - Card %d clicked", cardId);

    // 如果设置了游戏控制器，调用控制器处理逻辑
    if (_gameController)
    {
        _gameController->onCardClicked(cardId);
    }
    else
    {
        CCLOG("GameView::onCardClicked - No controller set");
    }
}

void GameView::onStackCardClicked(int cardId)
{
    CCLOG("GameView::onStackCardClicked - Stack card %d clicked", cardId);

    // 如果设置了游戏控制器，调用控制器处理逻辑
    if (_gameController)
    {
        _gameController->onStackClicked();
    }
    else
    {
        CCLOG("GameView::onStackCardClicked - No controller set");
    }
}

void GameView::onUndoButtonClicked()
{
    CCLOG("GameView::onUndoButtonClicked - Undo button clicked");

    // 如果设置了游戏控制器，调用控制器的回退功能
    if (_gameController)
    {
        _gameController->undo();
    }
    else
    {
        CCLOG("GameView::onUndoButtonClicked - No controller set");
    }
}
char* FontToUTF8(const char* font) {
    int len = MultiByteToWideChar(CP_ACP, 0, font, -1, NULL, 0);
    wchar_t* wstr = new wchar_t[len + 1];
    memset(wstr, 0, len + 1);
    MultiByteToWideChar(CP_ACP, 0, font, -1, wstr, len);
    len = WideCharToMultiByte(CP_UTF8, 0, wstr, -1, NULL, 0, NULL, NULL);
    char* str = new char[len + 1];
    memset(str, 0, len + 1);
    WideCharToMultiByte(CP_UTF8, 0, wstr, -1, str, len, NULL, NULL);
    if (wstr)delete[] wstr;
    return str;
}
void GameView::createUndoButton()
{
    // 创建回退文字标签
    _undoLabel = Label::create(FontToUTF8("回退"),"Arial",50);
    _undoLabel->setColor(Color3B::WHITE);
    _undoLabel->setPosition(Vec2(UNDO_BUTTON_X, UNDO_BUTTON_Y));

    // 添加到场景
    this->addChild(_undoLabel, 10);  

    // 创建触摸监听器（点击回退文字）
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);

    listener->onTouchBegan = [this](Touch* touch, Event* event) -> bool
        {
            Vec2 touchPos = touch->getLocation();

            // 检查是否点击了回退文字
            if (_undoLabel && _undoLabel->getBoundingBox().containsPoint(touchPos))
            {
                this->onUndoButtonClicked();
                return true;
            }

            return false;
        };

    // 注册监听器
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, _undoLabel);

    CCLOG("GameView::createUndoButton - Undo label created at (%.0f, %.0f)", UNDO_BUTTON_X, UNDO_BUTTON_Y);
}
