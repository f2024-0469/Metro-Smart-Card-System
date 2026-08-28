#include "JourneyHistory.h"
JourneyHistory::~JourneyHistory(){clear();}
void JourneyHistory::clear(){Node*p=head;while(p){Node*n=p->next;delete p;p=n;}head=tail=current=0;count=0;}
void JourneyHistory::copyFrom(const JourneyHistory&o){Node*p=o.head;while(p){long long s=0;append(p->data,s);if(o.current&&p==o.current)current=tail;p=p->next;}}
JourneyHistory::JourneyHistory(const JourneyHistory&o):head(0),tail(0),current(0),count(0){copyFrom(o);}
JourneyHistory&JourneyHistory::operator=(const JourneyHistory&o){if(this!=&o){clear();copyFrom(o);}return *this;}
void JourneyHistory::append(const JourneyData&j,long long&steps){steps=1;Node*n=new Node(j);if(!head){head=tail=current=n;}else{n->prev=tail;tail->next=n;tail=n;current=n;}++count;if(count>20){Node*old=head;head=head->next;head->prev=0;delete old;--count;} }
bool JourneyHistory::moveBack(long long&steps){steps=1;if(!current||!current->prev)return false;current=current->prev;return true;}
bool JourneyHistory::moveForward(long long&steps){steps=1;if(!current||!current->next)return false;current=current->next;return true;}
bool JourneyHistory::deleteCurrent(JourneyData&removed,long long&steps){steps=1;if(!current)return false;Node*n=current;removed=n->data;if(n->prev)n->prev->next=n->next;else head=n->next;if(n->next)n->next->prev=n->prev;else tail=n->prev;current=n->next?n->next:n->prev;delete n;--count;return true;}
