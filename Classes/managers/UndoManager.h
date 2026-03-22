// ³·Ïú¹¦ÄÜ
#pragma once

#include "models/GameModel.h"
#include "models/UndoModel.h"
#include <vector>
#include <stack>

class UndoManager
{
public:
    UndoManager() = default;
    ~UndoManager();

    void saveState(const GameModel* model);
    bool undo(GameModel* model);
    bool redo(GameModel* model);
    bool canUndo() const { return _undoStack.size() > 1; }
    bool canRedo() const { return !_redoStack.empty(); }
    void clear();

private:
    void _clearUndoStack();
    void _clearRedoStack();
    std::vector<UndoModel*> _undoStack; 
    std::vector<UndoModel*> _redoStack;  
};