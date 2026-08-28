# Metro Smart Card System — Validation Report

## Scope

This report validates the final C++17 implementation against the project checklist: clean build, menu/script modes, full data loading/saving, all six core jobs, Advanced A and B, instrumentation, error handling, and memory-safety testing.

## Build

Command:

```text
make clean && make
```

Result: **PASS**. The build completed with `-Wall -Wextra -Wpedantic` and no compiler warnings.

## Full dataset

Startup validation on the supplied files reported:

```text
LOAD cards=50000 journeys=250000 stations=30
```

The dataset therefore loaded with all 50,000 cards, 250,000 journeys, and 30 station codes.

## Script mode

Command:

```text
./app --script tests/commands.txt
```

Result: **PASS**. The script exercised card lookup, blocked-card rejection, tap-in, tap-out, FIFO queue behavior, top-up/undo, transaction replay, and journey history.

## Interactive mode

The executable starts the numbered menu when no `--script` argument is supplied. The menu includes all implemented core and advanced operations.

## Save/load

A fresh copy of the full data files was loaded and exited. The saved card file contained exactly 50,000 records with 48,000 ACTIVE and 2,000 BLOCKED cards. Record ordering can differ because the custom hash table writes records bucket-by-bucket; this does not change the stored card keys, balances, or statuses.

## Complexity evidence

The benchmark at 5,000, 25,000, and 50,000 cards recorded a median of 2 steps for the same card lookup at all three sizes. Queue enqueue/dequeue, undo, and journey-history movement/deletion each used a constant small number of steps in the representative full-data run.

See `docs/Performance_Benchmark.md` for the measured-vs-claimed table.

## Memory validation

AddressSanitizer was executed with:

```text
g++ -std=c++17 -Wall -Wextra -Wpedantic -fsanitize=address -fno-omit-frame-pointer -Iinclude src/*.cpp -o app_asan
./app_asan --script tests/commands.txt
```

Result: **PASS**, exit status 0, with no AddressSanitizer error report.

Valgrind was not installed in the available validation environment. Therefore no Valgrind result is claimed here. Run the final executable under Valgrind on the university/lab machine before submission if available.

## Final status

| Checklist item | Status |
|---|---|
| Build with no warnings | PASS |
| Menu mode | PASS |
| `--script` mode | PASS |
| Six core jobs | PASS |
| Advanced A | PASS |
| Advanced B | PASS |
| Design document included | PASS |
| README included | PASS |
| Test log included | PASS |
| Time and step counts | PASS |
| Measured-vs-claimed table | PASS |
| Full data load/save | PASS |
| AddressSanitizer validation | PASS |
| Valgrind validation | Pending local run |
| 15 GitHub commits | Pending team repository |
| Viva readiness | Requires both team members to review the full code |
