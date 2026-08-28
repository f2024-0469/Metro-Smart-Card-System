#ifndef JOURNEY_HISTORY_H
#define JOURNEY_HISTORY_H
#include "Types.h"
class JourneyHistory {
public:
    struct Node{JourneyData data;Node*prev;Node*next;Node(const JourneyData&d):data(d),prev(0),next(0){}};
private:
    Node*head;Node*tail;Node*current;int count;
    void clear();void copyFrom(const JourneyHistory&o);
public:
    JourneyHistory():head(0),tail(0),current(0),count(0){}~JourneyHistory();JourneyHistory(const JourneyHistory&o);JourneyHistory&operator=(const JourneyHistory&o);
    void append(const JourneyData&j,long long&steps);bool moveBack(long long&steps);bool moveForward(long long&steps);bool deleteCurrent(JourneyData&removed,long long&steps);
    const Node* getCurrent()const{return current;}const Node* getHead()const{return head;}int size()const{return count;}
};
#endif
