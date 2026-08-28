#ifndef OPEN_JOURNEY_TABLE_H
#define OPEN_JOURNEY_TABLE_H

#include "Types.h"
#include <string>

class OpenJourneyTable {
private:
    struct Node {
        OpenJourney data;
        Node* next;
        Node(const OpenJourney& value, Node* nextNode = 0) : data(value), next(nextNode) {}
    };
    Node** buckets;
    int bucketCount;
    int itemCount;
    unsigned long long hashKey(const std::string& key) const;
    void clear();
    void copyFrom(const OpenJourneyTable& other);
public:
    explicit OpenJourneyTable(int initialBuckets = 65537);
    ~OpenJourneyTable();
    OpenJourneyTable(const OpenJourneyTable& other);
    OpenJourneyTable& operator=(const OpenJourneyTable& other);
    bool insert(const OpenJourney& journey, long long& steps);
    OpenJourney* find(const std::string& cardNumber, long long& steps);
    bool remove(const std::string& cardNumber, long long& steps);
    int size() const;
};

#endif
