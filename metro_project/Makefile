CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Wpedantic
TARGET = app
SRC = src/main.cpp src/MetroSystem.cpp src/CardHashTable.cpp src/OpenJourneyTable.cpp src/BlockedHashTable.cpp src/GateQueue.cpp src/UndoStack.cpp src/JourneyHistory.cpp src/TransactionLog.cpp src/StationTable.cpp
OBJ = $(SRC:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) -Iinclude $(OBJ) -o $(TARGET)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -Iinclude -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)

run: $(TARGET)
	./$(TARGET)
