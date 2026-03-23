#include "GameController.h"
#include "views/CardView.h"
#include <algorithm>

USING_NS_CC;

GameController::GameController(GameView* gameView)
    : _gameView(gameView)
    , _gameModel(nullptr)
    , _undoManager(nullptr)
{
    // 获取 GameView 中的 GameModel 引用
    if (_gameView)
    {
        _gameModel = _gameView->getGameModel();
    }

    // 创建撤销管理器
    _undoManager = new UndoManager();

    // 如果没有托盘卡牌且牌堆有牌，自动翻出第一张作为初始托盘卡牌
    if (_gameModel && _gameModel->getCurrentCard() == nullptr && !_gameModel->getSideDeck().empty())
    {
        CCLOG("GameController::GameController - Auto-flipping initial tray card");

        // 弹出牌堆顶部卡牌
        CardModel* initialTrayCard = _gameModel->getSideDeckTop();
        if (initialTrayCard)
        {
            initialTrayCard->setOpen(true);
            initialTrayCard->setClick(false);
            initialTrayCard->setPosition(Vec2(GameView::TRAY_X, GameView::TRAY_Y));
            _gameModel->setCurrentCard(initialTrayCard);

            CCLOG("GameController::GameController - Initial tray card set: face=%d, suit=%d",
                static_cast<int>(initialTrayCard->getFace()),
                static_cast<int>(initialTrayCard->getSuit()));
        }
    }

    // 保存初始状态
    if (_gameModel)
    {
        _undoManager->saveState(_gameModel);
        CCLOG("GameController::GameController - Saved initial state");
    }
}

GameController::~GameController()
{
    // 不拥有 GameView 和 GameModel 的所有权，不需要删除
    // 但需要删除 UndoManager
    if (_undoManager)
    {
        delete _undoManager;
        _undoManager = nullptr;
    }
}

void GameController::onCardClicked(int cardId)
{
    CCLOG("GameController::onCardClicked - Card %d clicked", cardId);

  
    CardModel* clickedCard = _gameModel->findMainCard(cardId);
    if (!clickedCard)
    {
        CCLOG("GameController::onCardClicked - Card %d not found", cardId);
        return;
    }

    if (!clickedCard->isClick())
    {
        CCLOG("GameController::onCardClicked - Card %d is not clickable", cardId);
        return;
    }


    CardModel* card = _gameModel->getCurrentCard();
    if (!card)
    {
        CCLOG("GameController::onCardClicked - No tray card, cannot match");
        // TODO: 可以播放提示动画，提示用户先从牌堆翻牌
        return;
    }

    if (!isMatching(clickedCard,card))
    {
        CCLOG("GameController::onCardClicked - Card %d cannot match with tray card (face diff != 1)", cardId);
        // 播放错误提示动画
        _playErrorAnimation(cardId);
        return;
    }

    // 匹配成功
    CCLOG("GameController::onCardClicked - Match success! Card %d -> Tray", cardId);

    // 播放匹配动画：卡牌移到顶部牌位置并替换它
    _playMatchAnimation(cardId, [this, cardId, clickedCard]() {
        // 动画结束后，执行替换逻辑

        _gameModel->removeMainCard(cardId);


        clickedCard->setPosition(Vec2(GameView::TRAY_X, GameView::TRAY_Y));
        _gameModel->setCurrentCard(clickedCard);



        _gameView->refreshDisplay();

   
        _undoManager->saveState(_gameModel);

        if (checkVictory())
        {
            showVictoryMessage();
        }
        });
}

void GameController::onStackClicked()
{
    CCLOG("GameController::onStackClicked - Stack clicked");

    if (_gameModel->getSideDeck().empty())
    {
        CCLOG("GameController::onStackClicked - Stack is empty");
        return;
    }

    CardModel* newTrayCard = _gameModel->getSideDeckTop();
    if (!newTrayCard)
    {
        CCLOG("GameController::onStackClicked - Failed to pop stack card");
        return;
    }


    newTrayCard->setOpen(true);       // 翻开正面
    newTrayCard->setClick(false);   // 顶部牌不可点击

  
    newTrayCard->setPosition(Vec2(GameView::TRAY_X, GameView::TRAY_Y));
    _gameModel->setCurrentCard(newTrayCard);


    _gameView->refreshDisplay();


    _undoManager->saveState(_gameModel);

    CCLOG("GameController::onStackClicked - New tray card: face=%d, suit=%d",
        static_cast<int>(newTrayCard->getFace()),
        static_cast<int>(newTrayCard->getSuit()));
}

void GameController::undo()
{
    CCLOG("GameController::undo - Attempting to undo");
    clearVictoryMessage();

    if (!_undoManager->canUndo())
    {
        CCLOG("GameController::undo - Cannot undo");
        return;
    }

    // 记录撤销前的状态
    int previousTrayCardId = -1;
    if (_gameModel->getCurrentCard())
    {
        previousTrayCardId = _gameModel->getCurrentCard()->getId();
    }

    // 执行撤销
    if (_undoManager->undo(_gameModel))
    {
        // 撤销成功，先刷新显示
        _gameView->refreshDisplay();

        // 播放反向动画
        _playUndoAnimation(previousTrayCardId);

        CCLOG("GameController::undo - Undo successful");
    }
    else
    {
        CCLOG("GameController::undo - Undo failed");
    }
}

void GameController::_playUndoAnimation(int previousTrayCardId)
{
    if (previousTrayCardId == -1)
    {
        return;
    }

    CardModel* cardInPlayfield = _gameModel->findMainCard(previousTrayCardId);
    if (cardInPlayfield)
    {
        auto& cardViews = _gameView->_mainCardViews;
        auto it = cardViews.find(previousTrayCardId);
        if (it != cardViews.end())
        {
            CardView* cardView = it->second;
            Vec2 targetPos = cardInPlayfield->getPosition();

            // 先将CardView移到顶部牌位置
            cardView->setPosition(GameView::TRAY_X, GameView::TRAY_Y);

            // 播放从顶部牌移回牌面的动画
            auto moveAction = MoveTo::create(0.3f, targetPos);
            auto easeAction = EaseOut::create(moveAction, 2.0f);
            cardView->runAction(easeAction);

            CCLOG("GameController::playUndoAnimation - Card %d moves back to playfield", previousTrayCardId);
        }
        return;
    }

    const std::vector<CardModel*>& stackCards = _gameModel->getSideDeck();
    for (size_t i = 0; i < stackCards.size(); i++)
    {
        if (stackCards[i]->getId() == previousTrayCardId)
        {
            auto& stackCardViews = _gameView->_sideCardViews;
            auto it = stackCardViews.find(previousTrayCardId);
            if (it != stackCardViews.end())
            {
                CardView* cardView = it->second;

                // 计算备用牌堆中的位置
                float xPos = GameView::STACK_START_X + i * GameView::STACK_OFFSET_X;
                float yPos = GameView::STACK_START_Y;
                Vec2 targetPos(xPos, yPos);

                // 先将CardView移到顶部牌位置
                cardView->setPosition(GameView::TRAY_X, GameView::TRAY_Y);

                // 播放从顶部牌移回备用牌堆的动画
                auto moveAction = MoveTo::create(0.3f, targetPos);
                auto easeAction = EaseOut::create(moveAction, 2.0f);
                cardView->runAction(easeAction);

                CCLOG("GameController::playUndoAnimation - Card %d moves back to stack", previousTrayCardId);
            }
            return;
        }
    }
}

void GameController::redo()
{
    CCLOG("GameController::redo - Attempting to redo");

    if (!_undoManager->canRedo())
    {
        CCLOG("GameController::redo - Cannot redo");
        return;
    }

    // 记录重做前牌面区的卡牌ID
    std::vector<int> previousPlayfieldIds;
    for (const CardModel* card : _gameModel->getMainDeck())
    {
        previousPlayfieldIds.push_back(card->getId());
    }

    // 执行重做
    if (_undoManager->redo(_gameModel))
    {
        // 重做成功，刷新显示
        _gameView->refreshDisplay();

        // 检查哪张卡牌从牌面移到了托盘
        CardModel* currentTrayCard = _gameModel->getCurrentCard();
        if (currentTrayCard)
        {
            int trayCardId = currentTrayCard->getId();

            // 检查这张卡牌是否之前在牌面区
            bool wasInPlayfield = std::find(previousPlayfieldIds.begin(), previousPlayfieldIds.end(), trayCardId) != previousPlayfieldIds.end();

            if (wasInPlayfield && _gameView->_mainDeckView)
            {
                
                Vec2 startPos(540.0f, 1200.0f);  // 主牌区中部偏下位置
                Vec2 targetPos(GameView::TRAY_X, GameView::TRAY_Y);

                // 先将托盘CardView移到起始位置
                _gameView->_mainDeckView->setPosition(startPos);

                // 播放移动到托盘的动画
                auto moveAction = MoveTo::create(0.3f, targetPos);
                auto easeAction = EaseOut::create(moveAction, 2.0f);
                _gameView->_mainDeckView ->runAction(easeAction);

                CCLOG("GameController::redo - Playing forward animation for card %d", trayCardId);
            }
        }

        CCLOG("GameController::redo - Redo successful");
    }
    else
    {
        CCLOG("GameController::redo - Redo failed");
    }
}

bool GameController::checkVictory() const
{
    // 如果牌面区的卡牌全部被消除，则胜利
    return _gameModel->getMainDeck().empty();
}

void GameController::showVictoryMessage()
{
    CCLOG("=== VICTORY! ===");
    const int VICTORY_LABEL_TAG = 9999;


    // 创建胜利提示标签
    auto label = Label::createWithSystemFont(
        "Victory!",
        "Arial",
        72
    );

    label->setPosition(Vec2(540, 1040));  // 屏幕中央
    label->setTextColor(Color4B::YELLOW);
    label->setTag(VICTORY_LABEL_TAG);
    _gameView->addChild(label, 100);  

    // 添加缩放动画
    auto scaleUp = ScaleTo::create(0.3f, 1.5f);
    auto scaleDown = ScaleTo::create(0.3f, 1.0f);
    auto sequence = Sequence::create(scaleUp, scaleDown, nullptr);
    auto repeat = RepeatForever::create(sequence);

    label->runAction(repeat);
}

void GameController::clearVictoryMessage()
{
    CCLOG("Clearing victory message...");

    const int VICTORY_LABEL_TAG = 9999;

    if (!_gameView) {
        return;
    }

    
    Node* victoryNode = _gameView->getChildByTag(VICTORY_LABEL_TAG);

    if (victoryNode) {
        
        victoryNode->stopAllActions();


        victoryNode->removeFromParentAndCleanup(true);
        CCLOG("Victory message cleared successfully.");
    }
    else {
        CCLOG("No victory message found to clear.");
    }
}

void GameController::_playMatchAnimation(int cardId, const std::function<void()>& callback)
{
    // 找到对应的 CardView
    auto& cardViews = _gameView->_mainCardViews;  // 需要访问 GameView 的 _cardViews
    auto it = cardViews.find(cardId);
    if (it == cardViews.end())
    {
        CCLOG("GameController::playMatchAnimation - CardView %d not found", cardId);
        if (callback) callback();
        return;
    }

    CardView* cardView = it->second;

    // 创建移动到顶部牌位置的动画
    Vec2 targetPos(GameView::TRAY_X, GameView::TRAY_Y);
    auto moveAction = MoveTo::create(0.3f, targetPos);
    auto easeAction = EaseOut::create(moveAction, 2.0f);

    // 动画结束后执行回调
    auto callbackAction = CallFunc::create(callback);

    // 组合动作序列：移动 -> 回调
    auto sequence = Sequence::create(easeAction, callbackAction, nullptr);

    cardView->runAction(sequence);
}

void GameController::_playErrorAnimation(int cardId)
{
    // 找到对应的 CardView
    auto& cardViews = _gameView->_mainCardViews;
    auto it = cardViews.find(cardId);
    if (it == cardViews.end())
    {
        CCLOG("GameController::playErrorAnimation - CardView %d not found", cardId);
        return;
    }

    CardView* cardView = it->second;

    // 创建摇晃动画
    auto rotateRight = RotateTo::create(0.1f, 10.0f);   // 向右旋转10度
    auto rotateLeft = RotateTo::create(0.1f, -10.0f);   // 向左旋转-10度
    auto rotateBack = RotateTo::create(0.1f, 0.0f);     // 回到0度

    auto sequence = Sequence::create(rotateRight, rotateLeft, rotateRight, rotateLeft, rotateBack, nullptr);

    cardView->runAction(sequence);
}

