# Validation Performed

- Supplied `cards.csv`: 50,000 records; 48,000 ACTIVE; 2,000 BLOCKED; all 16 digits; common prefix `627384`.
- Supplied `journeys.csv`: 250,000 records; 14 dates; 30 unique station codes; every journey card reference exists in `cards.csv`.
- Project builds successfully with `g++ -std=c++17 -Wall -Wextra -Wpedantic` with no compiler warnings.
- Forbidden STL container/algorithm scan returned no matches.
- Main script smoke test covers card lookup, tap-in/tap-out, blocked-card rejection, FIFO queue operations, top-up/undo, transaction replay, and journey history navigation/deletion.
- Edge-case script was executed successfully and covers blocked-card rejection, tap-out without an open journey, empty undo stack, empty queue, invalid card input, duplicate registration, block/unblock behavior, tap-in/tap-out, queue enqueue/dequeue, and final system statistics.
- Full dataset loaded successfully during testing:
  - 50,000 cards
  - 250,000 journeys
  - 30 stations
- Final edge-case run completed without crashing and reported:
  `cards=50000 blocked=2000 open_journeys=0 queue=0 undo_stack=0 daily_log=2 stations=30`
- AddressSanitizer test with leak detection completed with exit code 0 on the smoke-test session.

## Important Implementation Note

The supplied data does not include a separate station file. The 30 station codes are therefore derived from the two station-code columns in `journeys.csv` and kept in a fixed 30-entry sorted C array. The supplied fares match `20 + 8 * absolute(position difference)` using that station-code order.