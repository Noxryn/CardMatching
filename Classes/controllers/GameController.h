// 用户交互
#pragma once
#include "cocos2d.h"
#include "models/GameModel.h"
#include "views/GameView.h"
#include "managers/UndoManager.h"
#include "utils/MatchRule.h"

// 游戏控制器类
class GameController
{
public:
    GameController(GameView* gameView);
    ~GameController();
    void onCardClicked(int cardId);
    void onStackClicked();
    void undo();
    void redo();

    bool canUndo() const { return _undoManager->canUndo(); }

    bool canRedo() const { return _undoManager->canRedo(); }

    bool checkVictory() const;
    void showVictoryMessage();
    void clearVictoryMessage();

private:
    void _playMatchAnimation(int cardId, const std::function<void()>& callback);
    void _playUndoAnimation(int previousTrayCardId);
    void _playErrorAnimation(int cardId);

    GameView* _gameView;       
    GameModel* _gameModel;     
    UndoManager* _undoManager; 
};