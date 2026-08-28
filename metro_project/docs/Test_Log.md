# Metro Smart Card System — Test Log

## Test environment

- Language: C++17
- Compiler: `g++`
- Flags: `-std=c++17 -Wall -Wextra -Wpedantic`
- Supplied full dataset: 50,000 cards, 250,000 journeys, 30 stations
- Script mode: `--script`

## Functional test results

| ID | Area | Test | Result | Evidence |
|---|---|---|---|---|
| T01 | Build | `make clean && make` | PASS | Zero compiler warnings/errors |
| T02 | Data load | Load full CSV files | PASS | `cards=50000`, `journeys=250000`, `stations=30` |
| T03 | Card registry | Find an existing active card | PASS | Hash-table lookup returned the correct record |
| T04 | Blocked card | Check blocked card and attempt tap-in | PASS | Gate refused blocked card |
| T05 | Tap-in | Active card at a valid station | PASS | Open journey created |
| T06 | Tap-out | Matching open journey | PASS | Fare calculated and balance reduced |
| T07 | Queue | Enqueue two cards, dequeue twice | PASS | FIFO order preserved |
| T08 | Undo | Top-up followed by `undo 1` | PASS | Balance restored; latest transaction removed from stack |
| T09 | Advanced A | Replay transaction log | PASS | Entries replayed in append order in one pass |
| T10 | Advanced B | Select history, back, forward, delete | PASS | Back/forward one node and middle deletion worked |
| T11 | Duplicate registration | Register existing card | PASS | Duplicate rejected |
| T12 | Invalid card | Register non-16-digit card | PASS | Input rejected without crash |
| T13 | Empty queue | Dequeue from empty queue | PASS | Error returned without crash |
| T14 | Empty undo | Undo with empty stack | PASS | `UNDONE=0` without crash |
| T15 | Invalid journey | Tap-out without an open journey | PASS | Error returned without crash |
| T16 | Block/unblock | Block, reject tap, unblock, tap | PASS | Status transitions worked |

## Scale checks

The card registry was tested at 5,000, 25,000, and 50,000 cards using the same lookup operation. Median step counts remained constant in the recorded runs:

| Cards | Card lookup median time (µs) | Card lookup median steps |
|---:|---:|---:|
| 5,000 | 0 | 2 |
| 25,000 | 0 | 2 |
| 50,000 | 0 | 2 |

The blocked-card lookup also remained a small fixed number of steps because the blocked set is bounded by the problem statement (up to 2,000 blocked cards).

## Full-data save verification

A fresh copy of the full dataset was loaded and exited without changing application state. The resulting `cards.csv` contained exactly 50,000 card records with 48,000 ACTIVE and 2,000 BLOCKED records. Card keys and balances were preserved; the custom hash table may write records in bucket order rather than original CSV order.

## Memory validation

AddressSanitizer was run against the full dataset and the scripted functional session. The run completed with exit status 0 and no sanitizer error report.

Valgrind was not available in the execution environment used for this validation, so this document does **not** claim a Valgrind result. The submission instructions still require the team to run Valgrind locally before final submission if the lab machine provides it.
