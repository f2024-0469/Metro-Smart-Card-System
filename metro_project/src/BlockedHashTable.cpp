#include "BlockedHashTable.h"
BlockedHashTable::BlockedHashTable(int initialBuckets):buckets(0),bucketCount(initialBuckets > 0 ? initialBuckets : 3),itemCount(0){buckets=new Node*[bucketCount];for(int i=0;i<bucketCount;++i)buckets[i]=0;}
BlockedHashTable::~BlockedHashTable(){clear();}
void BlockedHashTable::clear(){if(!buckets)return;for(int i=0;i<bucketCount;++i){Node*p=buckets[i];while(p){Node*n=p->next;delete p;p=n;}buckets[i]=0;}delete[] buckets;buckets=0;itemCount=0;}
BlockedHashTable::BlockedHashTable(const BlockedHashTable&o):buckets(0),bucketCount(o.bucketCount),itemCount(0){buckets=new Node*[bucketCount];for(int i=0;i<bucketCount;++i)buckets[i]=0;copyFrom(o);}
void BlockedHashTable::copyFrom(const BlockedHashTable&o){for(int i=0;i<o.bucketCount;++i){Node*s=o.buckets[i];Node**tail=&buckets[i];while(s){*tail=new Node(s->cardNumber);tail=&((*tail)->next);s=s->next;++itemCount;}}}
BlockedHashTable& BlockedHashTable::operator=(const BlockedHashTable&o){if(this!=&o){clear();bucketCount=o.bucketCount;buckets=new Node*[bucketCount];for(int i=0;i<bucketCount;++i)buckets[i]=0;copyFrom(o);}return *this;}
unsigned long long BlockedHashTable::hashKey(const std::string&key)const{unsigned long long v=0;int start=key.length()>10?(int)key.length()-10:0;for(int i=start;i<(int)key.length();++i)v=v*131ULL+(unsigned long long)(key[i]-'0'+1);return v%(unsigned long long)bucketCount;}
bool BlockedHashTable::insert(const std::string&k,long long&steps){steps=0;unsigned long long h=hashKey(k);++steps;Node*p=buckets[h];while(p){++steps;if(p->cardNumber==k)return false;p=p->next;}buckets[h]=new Node(k,buckets[h]);++itemCount;return true;}
bool BlockedHashTable::remove(const std::string&k,long long&steps){steps=0;unsigned long long h=hashKey(k);++steps;Node*p=buckets[h],*prev=0;while(p){++steps;if(p->cardNumber==k){if(prev)prev->next=p->next;else buckets[h]=p->next;delete p;--itemCount;return true;}prev=p;p=p->next;}return false;}
bool BlockedHashTable::contains(const std::string&k,long long&steps)const{steps=0;unsigned long long h=hashKey(k);++steps;Node*p=buckets[h];while(p){++steps;if(p->cardNumber==k)return true;p=p->next;}return false;}
int BlockedHashTable::size()const{return itemCount;}
