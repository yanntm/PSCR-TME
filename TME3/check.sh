#!/bin/bash

if [ $# -lt 2 ]; then
    echo "Usage: $0 file1 file2 [file3 ...]"
    exit 1
fi

files=("$@")
differ=0

for ((i=0; i<${#files[@]}-1; i++)); do
    for ((j=i+1; j<${#files[@]}; j++)); do
        if ! diff -q "${files[i]}" "${files[j]}" > /dev/null; then
            echo "Files ${files[i]} and ${files[j]} differ"
            differ=1
        fi
    done
done

if [ $differ -eq 0 ]; then
    echo "All files are identical"
else
    exit 1
fi
