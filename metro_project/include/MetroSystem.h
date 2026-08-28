#ifndef METRO_SYSTEM_H
#define METRO_SYSTEM_H
#include "CardHashTable.h"
#include "OpenJourneyTable.h"
#include "BlockedHashTable.h"
#include "GateQueue.h"
#include "UndoStack.h"
#include "JourneyHistory.h"
#include "TransactionLog.h"
#include "StationTable.h"
#include <string>

class MetroSystem {
private:
    CardHashTable cards;
    OpenJourneyTable openJourneys;
    BlockedHashTable blockedCards;
    GateQueue gateQueue;
    UndoStack undoStack;
    TransactionLog dailyLog;
    StationTable stations;
    JourneyHistory activeHistory;
    std::string activeHistoryCard;
    long long nextJourneyId;
    std::string cardsFile;
    std::string journeysFile;

    std::string now() const;
    bool validCardNumber(const std::string& card) const;
    bool validStation(int code) const;
    double calculateFare(int entryCode, int exitCode, long long& steps) const;
    bool parseCardLine(const std::string& line, CardRecord& card) const;
    bool parseJourneyLine(const std::string& line, JourneyData& journey) const;
    void printMetric(long long microseconds, long long steps) const;
    bool loadCardsInternal(long long& loaded);
    bool loadJourneysInternal(long long& loaded);

public:
    MetroSystem(const std::string& cardsPath="data/cards.csv", const std::string& journeysPath="data/journeys.csv");
    ~MetroSystem() {}
    bool load();
    bool save() const;

    void registerCard(const std::string&card,const std::string&holder,const std::string&cnic,double balance,bool active);
    void blockCard(const std::string&card);void unblockCard(const std::string&card);
    void tapIn(const std::string&card,int station);
    void tapOut(const std::string&card,int station);
    void showBlocked(const std::string&card);
    void queueAdd(const std::string&card);void queueRemove();void queueShow() const;
    void topUp(const std::string&card,double amount);void undoTopUps(int n);
    void replayReport() const;
    void historyShow(const std::string&card) const;
    void historyBack();void historyForward();void historyDelete();
    void showCard(const std::string&card) const;
    void stats() const;
    bool execute(const std::string&line);
};

#endif
