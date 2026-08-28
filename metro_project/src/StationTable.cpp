#include "StationTable.h"
bool StationTable::addCode(int code){for(int i=0;i<count;++i)if(codes[i]==code)return false;if(count>=30)return false;int pos=0;while(pos<count&&codes[pos]<code)++pos;for(int i=count;i>pos;--i)codes[i]=codes[i-1];codes[pos]=code;++count;return true;}
int StationTable::indexOf(int code,long long&steps)const{steps=0;for(int i=0;i<count;++i){++steps;if(codes[i]==code)return i;}return -1;}
