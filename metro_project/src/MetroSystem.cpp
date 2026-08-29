#include "MetroSystem.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <cstdlib>

MetroSystem::MetroSystem(const std::string&c,const std::string&j):cards(),openJourneys(),blockedCards(),gateQueue(),undoStack(),dailyLog(),stations(),nextJourneyId(1),cardsFile(c),journeysFile(j){}
std::string MetroSystem::now()const{std::time_t t=std::time(0);std::tm*lt=std::localtime(&t);std::ostringstream out;if(lt)out<<std::put_time(lt,"%Y-%m-%d %H:%M:%S");else out<<"1970-01-01 00:00:00";return out.str();}
bool MetroSystem::validCardNumber(const std::string&c)const{if(c.size()!=16)return false;for(std::size_t i=0;i<c.size();++i)if(c[i]<'0'||c[i]>'9')return false;return true;}
bool MetroSystem::validStation(int code)const{long long s=0;return stations.indexOf(code,s)>=0;}
double MetroSystem::calculateFare(int a,int b,long long&steps)const{long long s1=0,s2=0;int ia=stations.indexOf(a,s1),ib=stations.indexOf(b,s2);steps=s1+s2+1;if(ia<0||ib<0)return -1.0;int travelled=ia>ib?ia-ib:ib-ia;return 20.0+8.0*travelled;}
void MetroSystem::printMetric(long long us,long long steps)const{std::cout<<"time_us="<<us<<" steps="<<steps<<"\n";}
bool MetroSystem::parseCardLine(const std::string&line,CardRecord&card)const{std::stringstream ss(line);std::string balance,status,extra;if(!std::getline(ss,card.cardNumber,','))return false;if(!std::getline(ss,card.holderName,','))return false;if(!std::getline(ss,card.cnic,','))return false;if(!std::getline(ss,balance,','))return false;if(!std::getline(ss,status,','))return false;if(status!="ACTIVE"&&status!="BLOCKED")return false;std::stringstream bs(balance);if(!(bs>>card.balance))return false;if(bs>>extra)return false;if(card.balance<0)return false;card.active=(status=="ACTIVE");return validCardNumber(card.cardNumber);}
bool MetroSystem::parseJourneyLine(const std::string&line,JourneyData&j)const{std::stringstream ss(line);std::string id,in,out,fare;if(!std::getline(ss,id,','))return false;if(!std::getline(ss,j.cardNumber,','))return false;if(!std::getline(ss,in,','))return false;if(!std::getline(ss,out,','))return false;if(!std::getline(ss,j.entryTime,','))return false;if(!std::getline(ss,j.exitTime,','))return false;if(!std::getline(ss,fare,','))return false;std::stringstream a(id),b(in),c(out),d(fare);if(!(a>>j.journeyId&&b>>j.entryStation&&c>>j.exitStation&&d>>j.fare))return false;return true;}
bool MetroSystem::loadCardsInternal(long long&loaded){std::ifstream file(cardsFile.c_str());if(!file)return false;std::string line;std::getline(file,line);loaded=0;while(std::getline(file,line)){if(line.empty())continue;CardRecord c;if(!parseCardLine(line,c))continue;long long s=0;if(cards.insert(c,s)){++loaded;if(!c.active){long long bs=0;blockedCards.insert(c.cardNumber,bs);}}}return true;}
bool MetroSystem::loadJourneysInternal(long long&loaded){std::ifstream file(journeysFile.c_str());if(!file)return false;std::string line;std::getline(file,line);loaded=0;while(std::getline(file,line)){if(line.empty())continue;JourneyData j;if(!parseJourneyLine(line,j))continue;stations.addCode(j.entryStation);stations.addCode(j.exitStation);if(j.journeyId>=nextJourneyId)nextJourneyId=j.journeyId+1;++loaded;}return true;}
bool MetroSystem::load(){long long a=0,b=0;bool ok1=loadCardsInternal(a),ok2=loadJourneysInternal(b);std::cout<<"LOAD cards="<<a<<" journeys="<<b<<" stations="<<stations.size()<<"\n";return ok1&&ok2;}
bool MetroSystem::save()const{
    std::ofstream cf(cardsFile.c_str());
    if(!cf)return false;
    cf<<"card_no,holder,cnic,balance,status\n";
    cards.writeCsv(cf);
    return static_cast<bool>(cf);
}

void MetroSystem::registerCard(const std::string&card,const std::string&holder,const std::string&cnic,double balance,bool active){auto start=std::chrono::high_resolution_clock::now();long long steps=0;if(balance<0){std::cout<<"ERROR invalid_balance ";printMetric(0,1);return;}if(!validCardNumber(card)){std::cout<<"ERROR invalid_card ";printMetric(0,1);return;}CardRecord c{card,holder,cnic,balance,active};bool ok=cards.insert(c,steps);if(ok&&!active){long long bs=0;blockedCards.insert(card,bs);steps+=bs;}std::cout<<(ok?"REGISTERED ":"ERROR duplicate_card ");auto end=std::chrono::high_resolution_clock::now();printMetric(std::chrono::duration_cast<std::chrono::microseconds>(end-start).count(),steps);}
void MetroSystem::blockCard(const std::string&card){auto start=std::chrono::high_resolution_clock::now();long long steps=0;CardRecord*c=cards.find(card,steps);if(!c){std::cout<<"ERROR card_not_found ";printMetric(0,steps);return;}c->active=false;long long bs=0;blockedCards.insert(card,bs);steps+=bs;std::cout<<"BLOCKED ";auto end=std::chrono::high_resolution_clock::now();printMetric(std::chrono::duration_cast<std::chrono::microseconds>(end-start).count(),steps);}
void MetroSystem::unblockCard(const std::string&card){auto start=std::chrono::high_resolution_clock::now();long long steps=0;CardRecord*c=cards.find(card,steps);if(!c){std::cout<<"ERROR card_not_found ";printMetric(0,steps);return;}c->active=true;long long bs=0;blockedCards.remove(card,bs);steps+=bs;std::cout<<"UNBLOCKED ";auto end=std::chrono::high_resolution_clock::now();printMetric(std::chrono::duration_cast<std::chrono::microseconds>(end-start).count(),steps);}
void MetroSystem::tapIn(const std::string&card,int station){auto start=std::chrono::high_resolution_clock::now();long long steps=0;long long s=0;CardRecord*c=cards.find(card,s);steps+=s;if(!c){std::cout<<"ERROR card_not_found ";printMetric(0,steps);return;}long long bs=0;if(blockedCards.contains(card,bs)){steps+=bs;std::cout<<"REFUSED blocked_card ";auto end=std::chrono::high_resolution_clock::now();printMetric(std::chrono::duration_cast<std::chrono::microseconds>(end-start).count(),steps);return;}steps+=bs;if(c->balance<20){std::cout<<"REFUSED insufficient_balance ";printMetric(0,steps);return;}if(!validStation(station)){std::cout<<"ERROR invalid_station ";printMetric(0,steps);return;}OpenJourney*existing=openJourneys.find(card,s);steps+=s;if(existing){std::cout<<"ERROR already_tapped_in ";printMetric(0,steps);return;}OpenJourney j{card,station,now()};openJourneys.insert(j,s);steps+=s;TxnLogEntry log{"TAP_IN",card,0.0,station,0,j.entryTime};dailyLog.append(log,s);steps+=s;std::cout<<"TAP_IN accepted station="<<station<<" ";auto end=std::chrono::high_resolution_clock::now();printMetric(std::chrono::duration_cast<std::chrono::microseconds>(end-start).count(),steps);}
void MetroSystem::tapOut(const std::string&card,int station){auto start=std::chrono::high_resolution_clock::now();long long steps=0;long long s=0;CardRecord*c=cards.find(card,s);steps+=s;if(!c){std::cout<<"ERROR card_not_found ";printMetric(0,steps);return;}long long bs=0;if(blockedCards.contains(card,bs)){steps+=bs;std::cout<<"REFUSED blocked_card ";auto end=std::chrono::high_resolution_clock::now();printMetric(std::chrono::duration_cast<std::chrono::microseconds>(end-start).count(),steps);return;}OpenJourney*j=openJourneys.find(card,s);steps+=s;if(!j){std::cout<<"ERROR no_open_journey ";printMetric(0,steps);return;}long long fs=0;double fare=calculateFare(j->entryStation,station,fs);steps+=fs;if(fare<0){std::cout<<"ERROR invalid_station ";printMetric(0,steps);return;}if(c->balance<fare){std::cout<<"ERROR insufficient_balance_for_fare ";printMetric(0,steps);return;}std::string exitTime=now();c->balance-=fare;JourneyData done{nextJourneyId++,card,j->entryStation,station,j->entryTime,exitTime,fare};openJourneys.remove(card,s);steps+=s;std::ofstream jf(journeysFile.c_str(),std::ios::app);if(jf){jf<<done.journeyId<<","<<done.cardNumber<<","<<done.entryStation<<","<<done.exitStation<<","<<done.entryTime<<","<<done.exitTime<<","<<done.fare<<"\n";}TxnLogEntry log{"TAP_OUT",card,fare,done.entryStation,done.exitStation,exitTime};dailyLog.append(log,s);steps+=s;std::cout<<"TAP_OUT accepted fare="<<fare<<" balance="<<c->balance<<" ";auto end=std::chrono::high_resolution_clock::now();printMetric(std::chrono::duration_cast<std::chrono::microseconds>(end-start).count(),steps);}
void MetroSystem::showBlocked(const std::string&card){auto start=std::chrono::high_resolution_clock::now();long long steps=0;bool b=blockedCards.contains(card,steps);std::cout<<(b?"BLOCKED":"NOT_BLOCKED")<<" ";auto end=std::chrono::high_resolution_clock::now();printMetric(std::chrono::duration_cast<std::chrono::microseconds>(end-start).count(),steps);}
void MetroSystem::queueAdd(const std::string&card){auto start=std::chrono::high_resolution_clock::now();long long steps=0;GateEntry e{card,now()};gateQueue.enqueue(e,steps);std::cout<<"QUEUED position="<<gateQueue.size()<<" ";auto end=std::chrono::high_resolution_clock::now();printMetric(std::chrono::duration_cast<std::chrono::microseconds>(end-start).count(),steps);}
void MetroSystem::queueRemove(){auto start=std::chrono::high_resolution_clock::now();long long steps=0;GateEntry e;if(!gateQueue.dequeue(e,steps)){std::cout<<"ERROR queue_empty ";printMetric(0,steps);return;}std::cout<<"SERVED card="<<e.cardNumber<<" ";auto end=std::chrono::high_resolution_clock::now();printMetric(std::chrono::duration_cast<std::chrono::microseconds>(end-start).count(),steps);}
void MetroSystem::queueShow()const{auto start=std::chrono::high_resolution_clock::now();GateEntry e;long long s=0;if(!gateQueue.peek(e,s)){auto end=std::chrono::high_resolution_clock::now();std::cout<<"QUEUE empty ";printMetric(std::chrono::duration_cast<std::chrono::microseconds>(end-start).count(),s);return;}auto end=std::chrono::high_resolution_clock::now();std::cout<<"QUEUE front="<<e.cardNumber<<" size="<<gateQueue.size()<<" ";printMetric(std::chrono::duration_cast<std::chrono::microseconds>(end-start).count(),s);}
void MetroSystem::topUp(const std::string&card,double amount){auto start=std::chrono::high_resolution_clock::now();long long steps=0;if(amount<=0){std::cout<<"ERROR invalid_amount ";printMetric(0,1);return;}CardRecord*c=cards.find(card,steps);if(!c){std::cout<<"ERROR card_not_found ";printMetric(0,steps);return;}if(!c->active){std::cout<<"ERROR blocked_card ";printMetric(0,steps);return;}c->balance+=amount;TopUpTxn t{card,amount,now()};long long s=0;undoStack.push(t,s);steps+=s;TxnLogEntry log{"TOP_UP",card,amount,0,0,t.timestamp};dailyLog.append(log,s);steps+=s;std::cout<<"TOP_UP balance="<<c->balance<<" ";auto end=std::chrono::high_resolution_clock::now();printMetric(std::chrono::duration_cast<std::chrono::microseconds>(end-start).count(),steps);}
void MetroSystem::undoTopUps(int n) {
    auto start = std::chrono::high_resolution_clock::now();
    long long steps = 0;

    if (n < 1) {
        std::cout << "ERROR invalid_n ";
        printMetric(0, 1);
        return;
    }

    int undone = 0;

    while (undone < n) {
        TopUpTxn t;
        long long s = 0;

        if (!undoStack.pop(t, s))
            break;

        steps += s;

        long long fs = 0;
        CardRecord* c = cards.find(t.cardNumber, fs);
        steps += fs;

        if (c)
            c->balance -= t.amount;

        ++undone;
    }

    if (undone < n)
        std::cout << "UNDONE=" << undone << " requested=" << n << " ";
    else
        std::cout << "UNDONE=" << undone << " ";

    auto end = std::chrono::high_resolution_clock::now();

    printMetric(
        std::chrono::duration_cast<std::chrono::microseconds>(
            end - start
        ).count(),
        steps
    );
}
void MetroSystem::replayReport()const{auto start=std::chrono::high_resolution_clock::now();long long steps=0;int n=dailyLog.replay(steps);std::cout<<"REPLAY count="<<n<<"\n";for(int i=0;i<n;++i){const TxnLogEntry&e=dailyLog.at(i);std::cout<<i+1<<" "<<e.type<<" card="<<e.cardNumber;if(e.type=="TOP_UP")std::cout<<" amount="<<e.amount;if(e.type=="TAP_IN")std::cout<<" station="<<e.entryStation;if(e.type=="TAP_OUT")std::cout<<" entry="<<e.entryStation<<" exit="<<e.exitStation<<" fare="<<e.amount;std::cout<<" time="<<e.timestamp<<"\n";}auto end=std::chrono::high_resolution_clock::now();std::cout<<"time_us="<<std::chrono::duration_cast<std::chrono::microseconds>(end-start).count()<<" steps="<<steps<<"\n";}
void MetroSystem::historyShow(const std::string&card)const{
    std::ifstream file(journeysFile.c_str());
    if(!file){std::cout<<"ERROR journeys_file\n";return;}
    std::string line;std::getline(file,line);
    // const method cannot change the active session, so this command prints the last 20 directly.
    JourneyHistory h;
    while(std::getline(file,line)){JourneyData j;if(parseJourneyLine(line,j)&&j.cardNumber==card){long long s=0;h.append(j,s);}}
    std::cout<<"HISTORY card="<<card<<" last="<<h.size()<<"\n";
    const JourneyHistory::Node*n=h.getHead();while(n){std::cout<<n->data.journeyId<<" "<<n->data.entryStation<<"->"<<n->data.exitStation<<" fare="<<n->data.fare<<" "<<n->data.entryTime<<"\n";n=n->next;}
    std::cout<<"steps="<<h.size()<<"\n";
}
void MetroSystem::historyBack(){long long s=0;if(activeHistoryCard.empty()){std::cout<<"ERROR no_history_selected\n";return;}if(!activeHistory.moveBack(s)){std::cout<<"ERROR already_at_oldest ";printMetric(0,s);return;}const JourneyHistory::Node*n=activeHistory.getCurrent();std::cout<<"HISTORY_CURRENT id="<<n->data.journeyId<<" ";printMetric(0,s);}
void MetroSystem::historyForward(){long long s=0;if(activeHistoryCard.empty()){std::cout<<"ERROR no_history_selected\n";return;}if(!activeHistory.moveForward(s)){std::cout<<"ERROR already_at_newest ";printMetric(0,s);return;}const JourneyHistory::Node*n=activeHistory.getCurrent();std::cout<<"HISTORY_CURRENT id="<<n->data.journeyId<<" ";printMetric(0,s);}
void MetroSystem::historyDelete(){auto start=std::chrono::high_resolution_clock::now();long long s=0;if(activeHistoryCard.empty()){std::cout<<"ERROR no_history_selected ";printMetric(0,1);return;}JourneyData removed;if(!activeHistory.deleteCurrent(removed,s)){std::cout<<"ERROR empty_history ";printMetric(0,s);return;}std::cout<<"HISTORY_DELETED id="<<removed.journeyId<<" card="<<removed.cardNumber<<" ";auto end=std::chrono::high_resolution_clock::now();printMetric(std::chrono::duration_cast<std::chrono::microseconds>(end-start).count(),s);}

void MetroSystem::showCard(const std::string&card)const{auto start=std::chrono::high_resolution_clock::now();long long s=0;const CardRecord*c=cards.find(card,s);auto end=std::chrono::high_resolution_clock::now();if(!c){std::cout<<"ERROR card_not_found ";printMetric(std::chrono::duration_cast<std::chrono::microseconds>(end-start).count(),s);return;}std::cout<<"CARD "<<c->cardNumber<<" holder="<<c->holderName<<" cnic="<<c->cnic<<" balance="<<c->balance<<" status="<<(c->active?"ACTIVE":"BLOCKED")<<" ";printMetric(std::chrono::duration_cast<std::chrono::microseconds>(end-start).count(),s);}
void MetroSystem::stats()const{std::cout<<"cards="<<cards.size()<<" blocked="<<blockedCards.size()<<" open_journeys="<<openJourneys.size()<<" queue="<<gateQueue.size()<<" undo_stack="<<undoStack.size()<<" daily_log="<<dailyLog.size()<<" stations="<<stations.size()<<"\n";}
bool MetroSystem::execute(const std::string&line){std::stringstream ss(line);std::string cmd;if(!(ss>>cmd))return true;if(cmd[0]=='#')return true;if(cmd=="register"){std::string c,h,n,b,status;ss>>c>>h>>n>>b>>status;registerCard(c,h,n,std::atof(b.c_str()),status=="ACTIVE");return true;}if(cmd=="block"){std::string c;ss>>c;blockCard(c);return true;}if(cmd=="unblock"){std::string c;ss>>c;unblockCard(c);return true;}if(cmd=="tapin"){std::string c;int st;ss>>c>>st;tapIn(c,st);return true;}if(cmd=="tapout"){std::string c;int st;ss>>c>>st;tapOut(c,st);return true;}if(cmd=="blocked"){std::string c;ss>>c;showBlocked(c);return true;}if(cmd=="enqueue"){std::string c;ss>>c;queueAdd(c);return true;}if(cmd=="dequeue"){queueRemove();return true;}if(cmd=="queue"){queueShow();return true;}if(cmd=="topup"){std::string c,a;ss>>c>>a;topUp(c,std::atof(a.c_str()));return true;}if(cmd=="undo"){int n;ss>>n;undoTopUps(n);return true;}if(cmd=="replay"){replayReport();return true;}if(cmd=="history-back"){historyBack();return true;}if(cmd=="history-forward"){historyForward();return true;}if(cmd=="history-delete"){historyDelete();return true;}if(cmd=="history"){std::string c;ss>>c;std::ifstream file(journeysFile.c_str());if(!file){std::cout<<"ERROR journeys_file\n";return true;}activeHistory=JourneyHistory();activeHistoryCard=c;std::string line;std::getline(file,line);while(std::getline(file,line)){JourneyData j;if(parseJourneyLine(line,j)&&j.cardNumber==c){long long hs=0;activeHistory.append(j,hs);}}std::cout<<"HISTORY_SELECTED card="<<c<<" count="<<activeHistory.size()<<"\n";const JourneyHistory::Node*hn=activeHistory.getCurrent();if(hn)std::cout<<"CURRENT id="<<hn->data.journeyId<<" "<<hn->data.entryStation<<"->"<<hn->data.exitStation<<" fare="<<hn->data.fare<<"\n";return true;}if(cmd=="card"){std::string c;ss>>c;showCard(c);return true;}if(cmd=="stats"){stats();return true;}if(cmd=="help"){std::cout<<"register block unblock tapin tapout blocked enqueue dequeue queue topup undo replay history card stats exit\n";return true;}if(cmd=="exit")return false;std::cout<<"ERROR unknown_command\n";return true;}
