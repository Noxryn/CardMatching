// 游戏主界面
#pragma once

#include "cocos2d.h"
#include "models/GameModel.h"
#include "views/CardView.h"
#include <map>

class GameController;

//  游戏主界面类
class GameView : public cocos2d::Layer
{
    friend class GameController;

public:
    // 创建游戏视图并加载指定关卡
    static GameView* createWithLevel(const std::string& levelPath);
    virtual bool init() override;

    //加载关卡数据
    bool loadLevel(const std::string& levelPath);

    // 刷新卡牌的显示
    void refreshDisplay();

    // 设置游戏控制器
    void setGameController(GameController* controller) { _gameController = controller; }

    // 卡牌点击回调
    void onCardClicked(int cardId);

    // 用牌堆中某张牌点击回调

    void onStackCardClicked(int cardId);

    // 回退文字点击回调
    void onUndoButtonClicked();

    // 获取游戏数据模型
    GameModel* getGameModel() const { return _gameModel; }

    // 游戏区域位置常量（公开访问）
    static const float PLAYFIELD_START_X;  // 牌面区起始X坐标
    static const float PLAYFIELD_START_Y;  // 牌面区起始Y坐标

    // 手牌区（底部区域 1080x580）
    static const float HAND_AREA_Y;        // 手牌区起始Y坐标（底部）
    static const float STACK_START_X;      // 备用牌堆起始X坐标（左侧）
    static const float STACK_START_Y;      // 备用牌堆起始Y坐标
    static const float STACK_OFFSET_X;     // 备用牌堆卡牌X偏移（叠放效果）
    static const float TRAY_X;             // 顶部牌X坐标（中央）
    static const float TRAY_Y;             // 顶部牌Y坐标
    static const float UNDO_BUTTON_X;      // 回退文字X坐标（右侧）
    static const float UNDO_BUTTON_Y;      // 回退文字Y坐标

    CREATE_FUNC(GameView);

private:
    GameView();
    virtual ~GameView();

    // 设置触摸事件监听
    void setupTouchListener();

    // 设置键盘事件监听（用于撤销/重做）
    void setupKeyboardListener();

    //  创建背景
    void createBackground();

    //f 创建回退文字标签
    void createUndoButton();

    // 创建一个CardView并添加到场景
    CardView* createCardView(const CardModel* cardModel);

    // 清除所有CardView
    void clearAllCardViews();

    GameModel* _gameModel; 
    GameController* _gameController;

    // 卡牌视图映射表
    std::map<int, CardView*> _mainCardViews;
    CardView* _mainDeckView;

    // 备用牌堆卡牌视图映射表
    std::map<int, CardView*> _sideCardViews;

    // 回退文字标签
    cocos2d::Label* _undoLabel;
};