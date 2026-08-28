# Metro Smart Card System — Problem 2

C++17 console implementation for the Beaconhouse National University Data Structures + Lab semester project.

## Project scope

This project implements all six Core jobs and the two selected Advanced items from Problem 2:

- **Advanced A:** end-of-day transaction replay
- **Advanced B:** journey history with back/forward movement and middle deletion

The implementation follows the team's design document: custom hash tables, a circular-array queue, a custom stack, a doubly linked list, and an append-only dynamic transaction log.

## Requirements

- C++17 compiler with `g++`
- `make` for the Makefile workflow
- Console only; no GUI, website, database, or external C++ libraries

## Project structure

```text
metro_project/
├── include/                       # Header files
│   ├── BlockedHashTable.h
│   ├── CardHashTable.h
│   ├── GateQueue.h
│   ├── JourneyHistory.h
│   ├── MetroSystem.h
│   ├── OpenJourneyTable.h
│   ├── StationTable.h
│   ├── TransactionLog.h
│   ├── Types.h
│   └── UndoStack.h
├── src/                           # C++ implementations
├── tests/
│   ├── commands.txt               # Main smoke/functional script
│   └── edge_cases.txt             # Error/edge-case script
├── data/
│   ├── cards.csv                  # Supplied 50,000-card dataset
│   └── journeys.csv               # Supplied 250,000-journey dataset
├── docs/
│   ├── Proposal_Metro_Smart_Card.docx
│   ├── Design_Document_Metro_Smart_Card.docx
│   ├── Test_Log.md
│   ├── Performance_Benchmark.md
│   └── Validation_Report.md
├── Makefile
└── README.md
```

## Build

From the `metro_project` directory:

```bash
make clean
make
```

The Makefile uses:

```text
g++ -std=c++17 -Wall -Wextra -Wpedantic
```

## Run — menu mode

```bash
./app
```

On Windows with a MinGW build the executable is normally `app.exe`, so use:

```text
app.exe
```

The menu provides operations for registration, tap-in/out, blocked-card checks, queue operations, top-up/undo, replay, journey history, and statistics.

## Run — script mode

The assignment requires a command-file mode:

```bash
./app --script tests/commands.txt
```

A second script is available for edge cases:

```bash
./app --script tests/edge_cases.txt
```

Run scripts on a **copy** of the data directory if you want to preserve the original CSV files, because tap-out appends completed journeys and normal program exit saves the card registry.

## Command reference

```text
register <16-digit-card> <holder-no-spaces> <cnic> <balance> <ACTIVE|BLOCKED>
block <card>
unblock <card>
tapin <card> <station-code>
tapout <card> <station-code>
blocked <card>
enqueue <card>
dequeue
queue
topup <card> <amount>
undo <N>
replay
history <card>
history-back
history-forward
history-delete
card <card>
stats
help
exit
```

## Data structures and speed rules

### 1. Card registry — custom separate-chaining hash table

Card numbers are 16 digits and share the same first six digits. The hash uses the varying last ten digits. Average insert/search/delete is O(1); worst case is O(n) under extreme collisions. The table rehashes to keep its load factor around the required range.

### 2. Open-journey table — custom hash table

A card number maps directly to its open tap-in. Tap-out therefore finds and removes the open journey in O(1) average time. Fare calculation uses the fixed 30-station table and direct arithmetic over station positions, so it is O(1) for this problem.

### 3. Gate queue — custom circular array

Enqueue at the tail and dequeue at the head are O(1). Resizing is occasional/amortised and does not occur on every normal queue operation.

### 4. Undo stack — custom linked-list stack

Each top-up is pushed. An undo pops the most recent transaction, so each individual undo is O(1).

### 5. Journey history — custom doubly linked list

The selected card's history is capped at the last 20 journeys. Moving one journey back/forward follows one pointer, O(1). Deleting the current node rewires neighboring pointers without shifting the other journeys, O(1) when the node pointer is already known.

### 6. Daily transaction log — custom dynamic array

Transactions are appended in occurrence order. Append is O(1) amortised and end-of-day replay is one sequential O(k) pass over k transactions.

## Instrumentation

The operational commands print:

```text
time_us=<microseconds> steps=<count>
```

The step counter is intended to provide a machine-independent comparison alongside the environment-dependent microsecond timing.

## Validation documents

- `docs/Test_Log.md` — functional and scale test results
- `docs/Performance_Benchmark.md` — measured-vs-claimed complexity table
- `docs/Validation_Report.md` — final checklist and memory-validation status

## Memory management

The custom structures that own dynamic memory implement destructors, copy constructors, and copy-assignment operators. The implementation uses explicit `new`/`delete` as required by the project rules and avoids the forbidden STL containers/algorithms listed in the handout.

## Important final-submission checks

Before handing in the project:

1. Run `make clean && make` and confirm no warnings.
2. Run both menu mode and `--script` mode.
3. Test using the full data files.
4. Run Valgrind on the university/lab machine if available and include the clean report.
5. Keep the measured-vs-claimed table with your final report.
6. Ensure the GitHub repository has at least 15 meaningful commits spread across the semester.
7. Both team members should be able to explain the entire codebase during the viva.

## Academic honesty

Follow the exact disclosure/attribution rule in the course handout for any external or AI assistance. The final submission should also reflect the team's own understanding because the viva covers the whole codebase.

## How to Run

Build the project:

make

Run the interactive menu:

./metro

Run command-file mode:

./metro --script tests/commands.txt
