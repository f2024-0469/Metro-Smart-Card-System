# Validation performed

- Supplied `cards.csv`: 50,000 records; 48,000 ACTIVE; 2,000 BLOCKED; all 16 digits; common prefix `627384`.
- Supplied `journeys.csv`: 250,000 records; 14 dates; 30 unique station codes; every journey card reference exists in `cards.csv`.
- Project builds with `g++ -std=c++17 -Wall -Wextra -Wpedantic` with no compiler warnings.
- Forbidden STL container/algorithm scan returned no matches.
- Script smoke test covers registration-related lookup, tap-in/tap-out, blocked rejection, FIFO queue, top-up/undo, replay, and journey history.
- AddressSanitizer test with leak detection completed with exit code 0 on the smoke-test session.

## Important implementation note

The supplied data does not include a separate station file. The 30 station codes are therefore derived from the two station-code columns in `journeys.csv` and kept in a fixed 30-entry sorted C array. The supplied fares match `20 + 8 * absolute(position difference)` using that station-code order.
