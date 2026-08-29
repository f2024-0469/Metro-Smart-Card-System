#include "GateQueue.h"
GateQueue::GateQueue(int c):data(new GateEntry[c > 0 ? c : 1]),capacity(c > 0 ? c : 1),head(0),tail(0),count(0){}
GateQueue::~GateQueue(){delete[] data;}
void GateQueue::copyFrom(const GateQueue&o){capacity=o.capacity;data=new GateEntry[capacity];head=0;tail=o.count%capacity;count=o.count;for(int i=0;i<count;++i)data[i]=o.data[(o.head+i)%o.capacity];}
GateQueue::GateQueue(const GateQueue&o):data(0),capacity(0),head(0),tail(0),count(0){copyFrom(o);}
GateQueue& GateQueue::operator=(const GateQueue&o){if(this!=&o){delete[]data;data=0;copyFrom(o);}return *this;}
void GateQueue::resize(int newCapacity){GateEntry*nd=new GateEntry[newCapacity];for(int i=0;i<count;++i)nd[i]=data[(head+i)%capacity];delete[]data;data=nd;capacity=newCapacity;head=0;tail=count;}
bool GateQueue::enqueue(const GateEntry&e,long long&steps){steps=1;if(count==capacity)resize(capacity*2);data[tail]=e;tail=(tail+1)%capacity;++count;return true;}
bool GateQueue::dequeue(GateEntry&e,long long&steps){steps=1;if(count==0)return false;e=data[head];head=(head+1)%capacity;--count;return true;}
bool GateQueue::peek(GateEntry&e,long long&steps)const{steps=1;if(count==0)return false;e=data[head];return true;}
int GateQueue::size()const{return count;} bool GateQueue::empty()const{return count==0;}
