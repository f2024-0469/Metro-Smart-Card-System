#ifndef STATION_TABLE_H
#define STATION_TABLE_H
class StationTable {
private:int codes[30];int count;
public:StationTable():codes{0},count(0){}bool addCode(int code);int indexOf(int code,long long&steps)const;int size()const{return count;}int codeAt(int i)const{return codes[i];}};
#endif
