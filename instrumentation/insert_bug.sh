#!/bin/bash
# Inserts a bug randomly in the client code.

set -e

filename="testcases/CS425/include/standard.h"
bug_identifier=$(tail -1 $filename)
bug_count=5

# Remove any previously inserted directives
if [[ $bug_identifier == *"#define BUG"* ]]; then
    sed -i '$ d' $filename
fi

random_bug_id=$(( ( RANDOM % $bug_count )  + 1 ))
echo "#define BUG$random_bug_id" >> "$filename"
echo "-- Inserted bug $random_bug_id --"
