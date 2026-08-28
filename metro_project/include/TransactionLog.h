#ifndef TRANSACTION_LOG_H
#define TRANSACTION_LOG_H
#include "Types.h"
class TransactionLog {
private:
    TxnLogEntry* data;int count;int capacity;
    void resize(int newCapacity);void clear();void copyFrom(const TransactionLog&o);
public:
    explicit TransactionLog(int initialCapacity=1024);~TransactionLog();TransactionLog(const TransactionLog&o);TransactionLog&operator=(const TransactionLog&o);
    void append(const TxnLogEntry&entry,long long&steps);int replay(long long&steps)const;int size()const;const TxnLogEntry&at(int index)const;
};
#endif
