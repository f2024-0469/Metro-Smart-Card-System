#include "OpenJourneyTable.h"

OpenJourneyTable::OpenJourneyTable(int initialBuckets) : buckets(0), bucketCount(initialBuckets > 0 ? initialBuckets : 3), itemCount(0) {
    buckets = new Node*[bucketCount];
    for (int i = 0; i < bucketCount; ++i) buckets[i] = 0;
}
OpenJourneyTable::~OpenJourneyTable() { clear(); }
void OpenJourneyTable::clear() {
    if (!buckets) return;
    for (int i=0;i<bucketCount;++i) { Node* p=buckets[i]; while(p){Node* n=p->next; delete p; p=n;} buckets[i]=0; }
    delete[] buckets; buckets=0; itemCount=0;
}
OpenJourneyTable::OpenJourneyTable(const OpenJourneyTable& other) : buckets(0), bucketCount(other.bucketCount), itemCount(0) {
    buckets=new Node*[bucketCount]; for(int i=0;i<bucketCount;++i)buckets[i]=0; copyFrom(other);
}
void OpenJourneyTable::copyFrom(const OpenJourneyTable& other) {
    for(int i=0;i<other.bucketCount;++i){Node* s=other.buckets[i]; Node** tail=&buckets[i]; while(s){*tail=new Node(s->data); tail=&((*tail)->next); s=s->next; ++itemCount;}}
}
OpenJourneyTable& OpenJourneyTable::operator=(const OpenJourneyTable& other){if(this!=&other){clear();bucketCount=other.bucketCount;buckets=new Node*[bucketCount];for(int i=0;i<bucketCount;++i)buckets[i]=0;copyFrom(other);}return *this;}
unsigned long long OpenJourneyTable::hashKey(const std::string& key) const {unsigned long long v=0;int start=key.length()>10?static_cast<int>(key.length())-10:0;for(int i=start;i<(int)key.length();++i)v=v*131ULL+(unsigned long long)(key[i]-'0'+1);return v%(unsigned long long)bucketCount;}
bool OpenJourneyTable::insert(const OpenJourney& journey,long long& steps){steps=0;unsigned long long h=hashKey(journey.cardNumber);++steps;Node* p=buckets[h];while(p){++steps;if(p->data.cardNumber==journey.cardNumber)return false;p=p->next;}buckets[h]=new Node(journey,buckets[h]);++steps;++itemCount;return true;}
OpenJourney* OpenJourneyTable::find(const std::string& cardNumber,long long& steps){steps=0;unsigned long long h=hashKey(cardNumber);++steps;Node* p=buckets[h];while(p){++steps;if(p->data.cardNumber==cardNumber)return &p->data;p=p->next;}return 0;}
bool OpenJourneyTable::remove(const std::string& cardNumber,long long& steps){steps=0;unsigned long long h=hashKey(cardNumber);++steps;Node* p=buckets[h];Node* prev=0;while(p){++steps;if(p->data.cardNumber==cardNumber){if(prev)prev->next=p->next;else buckets[h]=p->next;delete p;--itemCount;return true;}prev=p;p=p->next;}return false;}
int OpenJourneyTable::size() const{return itemCount;}
