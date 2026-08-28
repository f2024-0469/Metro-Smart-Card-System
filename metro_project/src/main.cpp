#include "MetroSystem.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstdlib>

static void showMenu() {
    std::cout << "\n===== Metro Smart Card System =====\n"
              << "1. Register card\n"
              << "2. Tap in\n"
              << "3. Tap out\n"
              << "4. Check blocked card\n"
              << "5. Block card\n"
              << "6. Unblock card\n"
              << "7. Enqueue passenger\n"
              << "8. Dequeue passenger\n"
              << "9. Show gate queue\n"
              << "10. Top up\n"
              << "11. Undo last N top-ups\n"
              << "12. End-of-day replay\n"
              << "13. Select journey history\n"
              << "14. Move history back\n"
              << "15. Move history forward\n"
              << "16. Delete current history journey\n"
              << "17. Show card\n"
              << "18. Show system statistics\n"
              << "0. Exit\n"
              << "Choice: ";
}

static std::string readLine(const char* prompt) {
    std::cout << prompt;
    std::string value;
    std::getline(std::cin, value);
    return value;
}

static int readInt(const char* prompt) {
    std::string value = readLine(prompt);
    std::stringstream ss(value);
    int result = 0;
    ss >> result;
    return result;
}

static double readDouble(const char* prompt) {
    std::string value = readLine(prompt);
    std::stringstream ss(value);
    double result = 0.0;
    ss >> result;
    return result;
}

static void runInteractive(MetroSystem& system) {
    while (true) {
        showMenu();
        std::string choice;
        if (!std::getline(std::cin, choice)) break;
        std::stringstream cs(choice);
        int option = -1;
        cs >> option;
        if (option == 0) break;

        if (option == 1) {
            std::string card = readLine("16-digit card: ");
            std::string holder = readLine("Holder name: ");
            std::string cnic = readLine("CNIC: ");
            double balance = readDouble("Balance: ");
            std::string status = readLine("Status (ACTIVE/BLOCKED): ");
            system.registerCard(card, holder, cnic, balance, status == "ACTIVE");
        } else if (option == 2) {
            system.tapIn(readLine("Card: "), readInt("Station code: "));
        } else if (option == 3) {
            system.tapOut(readLine("Card: "), readInt("Station code: "));
        } else if (option == 4) {
            system.showBlocked(readLine("Card: "));
        } else if (option == 5) {
            system.blockCard(readLine("Card: "));
        } else if (option == 6) {
            system.unblockCard(readLine("Card: "));
        } else if (option == 7) {
            system.queueAdd(readLine("Card: "));
        } else if (option == 8) {
            system.queueRemove();
        } else if (option == 9) {
            system.queueShow();
        } else if (option == 10) {
            system.topUp(readLine("Card: "), readDouble("Amount: "));
        } else if (option == 11) {
            system.undoTopUps(readInt("N: "));
        } else if (option == 12) {
            system.replayReport();
        } else if (option == 13) {
            system.execute(std::string("history ") + readLine("Card: "));
        } else if (option == 14) {
            system.historyBack();
        } else if (option == 15) {
            system.historyForward();
        } else if (option == 16) {
            system.historyDelete();
        } else if (option == 17) {
            system.showCard(readLine("Card: "));
        } else if (option == 18) {
            system.stats();
        } else {
            std::cout << "ERROR invalid_menu_choice\n";
        }
    }
}

int main(int argc, char** argv) {
    MetroSystem system("data/cards.csv", "data/journeys.csv");
    if (!system.load()) {
        std::cerr << "Warning: one or more data files could not be loaded.\n";
    }

    if (argc == 3 && std::string(argv[1]) == "--script") {
        std::ifstream script(argv[2]);
        if (!script) {
            std::cerr << "Cannot open script: " << argv[2] << "\n";
            return 1;
        }
        std::string line;
        while (std::getline(script, line)) {
            if (!system.execute(line)) break;
        }
        if (!system.save()) {
            std::cerr << "Warning: could not save cards.csv\n";
            return 1;
        }
        return 0;
    }

    runInteractive(system);
    if (!system.save()) {
        std::cerr << "Warning: could not save cards.csv\n";
        return 1;
    }
    return 0;
}
