#ifndef TYPES_H
#define TYPES_H

#include <string>

struct CardRecord {
    std::string cardNumber;
    std::string holderName;
    std::string cnic;
    double balance;
    bool active;
};

struct OpenJourney {
    std::string cardNumber;
    int entryStation;
    std::string entryTime;
};

struct GateEntry {
    std::string cardNumber;
    std::string arrivalTime;
};

struct TopUpTxn {
    std::string cardNumber;
    double amount;
    std::string timestamp;
};

struct JourneyData {
    long long journeyId;
    std::string cardNumber;
    int entryStation;
    int exitStation;
    std::string entryTime;
    std::string exitTime;
    double fare;
};

struct TxnLogEntry {
    std::string type;
    std::string cardNumber;
    double amount;
    int entryStation;
    int exitStation;
    std::string timestamp;
};

#endif
