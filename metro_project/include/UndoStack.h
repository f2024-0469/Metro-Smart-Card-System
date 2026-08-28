#ifndef UNDO_STACK_H
#define UNDO_STACK_H
#include "Types.h"
class UndoStack {
private:
    struct Node{TopUpTxn data;Node*next;Node(const TopUpTxn&d,Node*n=0):data(d),next(n){}}; Node*topNode;int count;
    void clear();void copyFrom(const UndoStack&o);
public:
    UndoStack():topNode(0),count(0){} ~UndoStack(); UndoStack(const UndoStack&o); UndoStack&operator=(const UndoStack&o);
    void push(const TopUpTxn&txn,long long&steps);bool pop(TopUpTxn&txn,long long&steps);bool peek(TopUpTxn&txn,long long&steps)const;int size()const;bool empty()const;
};
#endif
