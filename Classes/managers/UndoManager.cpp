#include "UndoManager.h"


UndoManager::~UndoManager()
{
    clear();
}

void UndoManager::saveState(const GameModel* model)
{
    // 创建当前状态快照
    UndoModel* state = UndoModel::createFromModel(model);

    // 保存到撤销栈
    _undoStack.push_back(state);

    // 清空重做栈（因为执行了新操作，旧的重做历史失效）
    _clearRedoStack();

}

bool UndoManager::undo(GameModel* model)
{
    // 至少需要2个状态才能撤销
    if (!canUndo())
    {
        return false;
    }

    // 弹出当前状态，保存到重做栈
    UndoModel* currentState = _undoStack.back();
    _undoStack.pop_back();
    _redoStack.push_back(currentState);

    // 获取上一个状态并恢复
    UndoModel* previousState = _undoStack.back();
    previousState->restoreToModel(model);

    return true;
}

bool UndoManager::redo(GameModel* model)
{
    if (!canRedo())
    {
     
        return false;
    }

    // 从重做栈弹出状态
    UndoModel* nextState = _redoStack.back();
    _redoStack.pop_back();

    // 恢复状态并保存到撤销栈
    nextState->restoreToModel(model);
    _undoStack.push_back(nextState);

    

    return true;
}

void UndoManager::clear()
{
    _clearUndoStack();
    _clearRedoStack();
}

void UndoManager::_clearUndoStack()
{
    for (UndoModel* state : _undoStack)
    {
        delete state;
    }
    _undoStack.clear();
}

void UndoManager::_clearRedoStack()
{
    for (UndoModel* state : _redoStack)
    {
        delete state;
    }
    _redoStack.clear();
}
