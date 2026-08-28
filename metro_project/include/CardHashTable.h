#ifndef CARD_HASH_TABLE_H
#define CARD_HASH_TABLE_H

#include "Types.h"
#include <string>
#include <fstream>

class CardHashTable {
private:
    struct Node {
        CardRecord data;
        Node* next;
        Node(const CardRecord& value, Node* nextNode = 0) : data(value), next(nextNode) {}
    };

    Node** buckets;
    int bucketCount;
    int itemCount;

    unsigned long long hashKey(const std::string& key) const;
    void rehash(int newBucketCount);
    void clear();
    void copyFrom(const CardHashTable& other);

public:
    explicit CardHashTable(int initialBuckets = 65537);
    ~CardHashTable();
    CardHashTable(const CardHashTable& other);
    CardHashTable& operator=(const CardHashTable& other);

    bool insert(const CardRecord& card, long long& steps);
    CardRecord* find(const std::string& cardNumber, long long& steps);
    const CardRecord* find(const std::string& cardNumber, long long& steps) const;
    bool remove(const std::string& cardNumber, long long& steps);
    int size() const;
    void writeCsv(std::ofstream& out) const;
};

#endif
