#include "TransactionLog.h"
TransactionLog::TransactionLog(int c):data(new TxnLogEntry[c]),count(0),capacity(c){}
TransactionLog::~TransactionLog(){clear();}
void TransactionLog::clear(){delete[]data;data=0;count=capacity=0;}
void TransactionLog::copyFrom(const TransactionLog&o){capacity=o.capacity;count=o.count;data=new TxnLogEntry[capacity];for(int i=0;i<count;++i)data[i]=o.data[i];}
TransactionLog::TransactionLog(const TransactionLog&o):data(0),count(0),capacity(0){copyFrom(o);}
TransactionLog&TransactionLog::operator=(const TransactionLog&o){if(this!=&o){clear();copyFrom(o);}return *this;}
void TransactionLog::resize(int c){TxnLogEntry*nd=new TxnLogEntry[c];for(int i=0;i<count;++i)nd[i]=data[i];delete[]data;data=nd;capacity=c;}
void TransactionLog::append(const TxnLogEntry&e,long long&steps){steps=1;if(count==capacity)resize(capacity*2);data[count++]=e;}
int TransactionLog::replay(long long&steps)const{steps=0;for(int i=0;i<count;++i)++steps;return count;}
int TransactionLog::size()const{return count;}const TxnLogEntry&TransactionLog::at(int i)const{return data[i];}
