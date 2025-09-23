#!/bin/bash

# Script to measure performance of different modes in TME3
#
# Usage: ./measurePerf.sh [FILE] [EXE]
#   - FILE: Input text file to parse (default: WarAndPeace.txt)
#   - EXE: Path to the TME3 executable (default: ./build-release/TME3)
#
# Invocation example:
#   ./measurePerf.sh WarAndPeace.txt ./build/TME3 > perf.txt
#
# Description:
#   This script runs the TME3 word frequency counter in various modes and thread counts.
#   It tests single-threaded modes (freqstd, freqstdf, freq) once each.
#   For multi-threaded modes, it loops over thread counts 1, 2, 4, 6, 8, 16, 32, 64.
#   Output includes timing info from 'time' command for single modes, and program output for multi.
#   Redirect stdout to a file (e.g., perf.txt) to capture results for further processing.
#
# I/O:
#   - Input: Reads the specified FILE.
#   - Output: Prints test results and timings to stdout.
#
# Adaptation:
#   - Modify SINGLE_MODES or MULTI_MODES arrays to add/remove modes.
#   - Change THREADS array for different thread counts.
#   - Adjust EXE path if built in a different location (e.g., ./build/TME3).
#   - For release builds, ensure EXE points to optimized binary.

FILE="${1:-WarAndPeace.txt}"
EXE="${2:-./build-release/TME3}"

# Modes that do not use num_threads
SINGLE_MODES=("freqstd" "freqstdf" "freq")

# Modes that use num_threads, skipping naive ones
MULTI_MODES=("partition" "mt_mutex" "mt_hmutex" "mt_hashes" "mt_hhashes" "mt_hfine" "mt_lf" "mt_lfna")

# Thread counts to test
THREADS=(1 2 4 6 8 16 32 64)

echo "Starting performance measurements..."

# Test single-threaded modes
for mode in "${SINGLE_MODES[@]}"; do
    echo "Testing mode: $mode"
    time $EXE $FILE $mode
    echo "--------------------------------"
done

# Test multi-threaded modes with different thread counts
for mode in "${MULTI_MODES[@]}"; do
    for n in "${THREADS[@]}"; do
        echo "Testing mode: $mode with $n threads"
        $EXE $FILE $mode $n
        echo "--------------------------------"
    done
done

echo "All tests completed."
