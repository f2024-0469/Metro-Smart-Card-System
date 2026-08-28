#ifndef GATE_QUEUE_H
#define GATE_QUEUE_H
#include "Types.h"
class GateQueue {
private:
    GateEntry* data; int capacity; int head; int tail; int count;
    void resize(int newCapacity); void copyFrom(const GateQueue& other);
public:
    explicit GateQueue(int initialCapacity=1024);
    ~GateQueue(); GateQueue(const GateQueue& other); GateQueue& operator=(const GateQueue& other);
    bool enqueue(const GateEntry& entry,long long& steps); bool dequeue(GateEntry& entry,long long& steps);
    bool peek(GateEntry& entry,long long& steps) const; int size() const; bool empty() const;
};
#endif
