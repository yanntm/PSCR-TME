#!/bin/bash

# Script to convert TME3 performance log to CSV format
#
# Usage: ./perf2csv.sh [INPUT_FILE]
#   - INPUT_FILE: Path to the performance log file (default: perf.txt)
#
# Invocation example:
#   ./perf2csv.sh perf.txt > perfs.csv
#
# Description:
#   Parses the output from measurePerf.sh (or similar log) to extract mode, thread count, and runtime.
#   Uses sed to match "Preparing to parse" lines for mode/N, and "Total runtime" for time.
#   Outputs CSV with columns: mode,threads,runtime
#
# I/O:
#   - Input: Reads from INPUT_FILE (expects lines like "Preparing to parse... (mode=X N=Y)" and "Total runtime... Z ms")
#   - Output: Prints CSV header and rows to stdout.
#
# Adaptation:
#   - Change INPUT_FILE if log is named differently.
#   - Modify sed regex if log format changes (e.g., different keywords or formats).
#   - To filter modes/threads, pipe output through grep/awk (e.g., | grep -v "mode_to_exclude").

echo "mode,threads,runtime"

sed -n '/^Preparing to parse/ {
    s/.*(mode=\([^ ]*\) N=\([0-9]*\)).*/\1,\2/
    N
    s/\n.* \([0-9]*\) ms/,\1/
    p
}' ${1:-perf.txt}
