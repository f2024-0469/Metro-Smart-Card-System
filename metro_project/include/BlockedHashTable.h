#ifndef BLOCKED_HASH_TABLE_H
#define BLOCKED_HASH_TABLE_H

#include <string>

class BlockedHashTable {
private:
    struct Node { std::string cardNumber; Node* next; Node(const std::string& k, Node* n=0):cardNumber(k),next(n){} };
    Node** buckets; int bucketCount; int itemCount;
    unsigned long long hashKey(const std::string& key) const;
    void clear(); void copyFrom(const BlockedHashTable& other);
public:
    explicit BlockedHashTable(int initialBuckets=8191);
    ~BlockedHashTable();
    BlockedHashTable(const BlockedHashTable& other);
    BlockedHashTable& operator=(const BlockedHashTable& other);
    bool insert(const std::string& cardNumber, long long& steps);
    bool remove(const std::string& cardNumber, long long& steps);
    bool contains(const std::string& cardNumber, long long& steps) const;
    int size() const;
};

#endif
