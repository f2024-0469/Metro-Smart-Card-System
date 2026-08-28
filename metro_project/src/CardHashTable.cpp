#include "CardHashTable.h"

CardHashTable::CardHashTable(int initialBuckets)
    : buckets(0), bucketCount(initialBuckets), itemCount(0) {
    buckets = new Node*[bucketCount];
    for (int i = 0; i < bucketCount; ++i) buckets[i] = 0;
}

CardHashTable::~CardHashTable() { clear(); }

void CardHashTable::clear() {
    if (!buckets) return;
    for (int i = 0; i < bucketCount; ++i) {
        Node* cur = buckets[i];
        while (cur) {
            Node* next = cur->next;
            delete cur;
            cur = next;
        }
        buckets[i] = 0;
    }
    delete[] buckets;
    buckets = 0;
    itemCount = 0;
}

CardHashTable::CardHashTable(const CardHashTable& other)
    : buckets(0), bucketCount(other.bucketCount), itemCount(0) {
    buckets = new Node*[bucketCount];
    for (int i = 0; i < bucketCount; ++i) buckets[i] = 0;
    copyFrom(other);
}

void CardHashTable::copyFrom(const CardHashTable& other) {
    for (int i = 0; i < other.bucketCount; ++i) {
        Node* src = other.buckets[i];
        Node** tail = &buckets[i];
        while (src) {
            *tail = new Node(src->data);
            tail = &((*tail)->next);
            src = src->next;
            ++itemCount;
        }
    }
}

CardHashTable& CardHashTable::operator=(const CardHashTable& other) {
    if (this != &other) {
        clear();
        bucketCount = other.bucketCount;
        buckets = new Node*[bucketCount];
        for (int i = 0; i < bucketCount; ++i) buckets[i] = 0;
        copyFrom(other);
    }
    return *this;
}

unsigned long long CardHashTable::hashKey(const std::string& key) const {
    unsigned long long value = 0;
    const int start = key.length() > 10 ? static_cast<int>(key.length()) - 10 : 0;
    for (int i = start; i < static_cast<int>(key.length()); ++i) {
        value = value * 131ULL + static_cast<unsigned long long>(key[i] - '0' + 1);
    }
    return value % static_cast<unsigned long long>(bucketCount);
}

void CardHashTable::rehash(int newBucketCount) {
    Node** newBuckets = new Node*[newBucketCount];
    for (int i = 0; i < newBucketCount; ++i) newBuckets[i] = 0;

    for (int i = 0; i < bucketCount; ++i) {
        Node* cur = buckets[i];
        while (cur) {
            Node* next = cur->next;
            unsigned long long value = 0;
            const std::string& key = cur->data.cardNumber;
            const int start = key.length() > 10 ? static_cast<int>(key.length()) - 10 : 0;
            for (int j = start; j < static_cast<int>(key.length()); ++j)
                value = value * 131ULL + static_cast<unsigned long long>(key[j] - '0' + 1);
            int index = static_cast<int>(value % static_cast<unsigned long long>(newBucketCount));
            cur->next = newBuckets[index];
            newBuckets[index] = cur;
            cur = next;
        }
    }
    delete[] buckets;
    buckets = newBuckets;
    bucketCount = newBucketCount;
}

bool CardHashTable::insert(const CardRecord& card, long long& steps) {
    steps = 0;
    unsigned long long h = hashKey(card.cardNumber); ++steps;
    Node* cur = buckets[h];
    while (cur) {
        ++steps;
        if (cur->data.cardNumber == card.cardNumber) return false;
        cur = cur->next;
    }
    buckets[h] = new Node(card, buckets[h]); ++steps;
    ++itemCount;
    if (itemCount * 4 > bucketCount * 3) rehash(bucketCount * 2 + 1);
    return true;
}

CardRecord* CardHashTable::find(const std::string& cardNumber, long long& steps) {
    steps = 0;
    unsigned long long h = hashKey(cardNumber); ++steps;
    Node* cur = buckets[h];
    while (cur) {
        ++steps;
        if (cur->data.cardNumber == cardNumber) return &cur->data;
        cur = cur->next;
    }
    return 0;
}

const CardRecord* CardHashTable::find(const std::string& cardNumber, long long& steps) const {
    steps = 0;
    unsigned long long h = hashKey(cardNumber); ++steps;
    Node* cur = buckets[h];
    while (cur) {
        ++steps;
        if (cur->data.cardNumber == cardNumber) return &cur->data;
        cur = cur->next;
    }
    return 0;
}

bool CardHashTable::remove(const std::string& cardNumber, long long& steps) {
    steps = 0;
    unsigned long long h = hashKey(cardNumber); ++steps;
    Node* cur = buckets[h];
    Node* prev = 0;
    while (cur) {
        ++steps;
        if (cur->data.cardNumber == cardNumber) {
            if (prev) prev->next = cur->next;
            else buckets[h] = cur->next;
            delete cur;
            --itemCount;
            return true;
        }
        prev = cur;
        cur = cur->next;
    }
    return false;
}

int CardHashTable::size() const { return itemCount; }

void CardHashTable::writeCsv(std::ofstream& out) const {
    for (int i = 0; i < bucketCount; ++i) {
        Node* cur = buckets[i];
        while (cur) {
            out << cur->data.cardNumber << "," << cur->data.holderName << ","
                << cur->data.cnic << "," << cur->data.balance << ","
                << (cur->data.active ? "ACTIVE" : "BLOCKED") << "\n";
            cur = cur->next;
        }
    }
}
