#include "UndoStack.h"
UndoStack::~UndoStack(){clear();}
void UndoStack::clear(){while(topNode){Node*n=topNode->next;delete topNode;topNode=n;}count=0;}
void UndoStack::copyFrom(const UndoStack&o){if(!o.topNode)return;Node*src=o.topNode;Node*rev=0;while(src){rev=new Node(src->data,rev);src=src->next;}while(rev){Node*n=rev->next;rev->next=topNode;topNode=rev;rev=n;++count;}}
UndoStack::UndoStack(const UndoStack&o):topNode(0),count(0){copyFrom(o);}
UndoStack&UndoStack::operator=(const UndoStack&o){if(this!=&o){clear();copyFrom(o);}return *this;}
void UndoStack::push(const TopUpTxn&txn,long long&steps){steps=1;topNode=new Node(txn,topNode);++count;}
bool UndoStack::pop(TopUpTxn&txn,long long&steps){steps=1;if(!topNode)return false;Node*n=topNode;txn=n->data;topNode=n->next;delete n;--count;return true;}
bool UndoStack::peek(TopUpTxn&txn,long long&steps)const{steps=1;if(!topNode)return false;txn=topNode->data;return true;}
int UndoStack::size()const{return count;}bool UndoStack::empty()const{return topNode==0;}
