#!/bin/bash
# Outputs the number of times each predicate was hit during a run.
#
# First argument:  testcase file
# Second argument: input C file (will be instrumented)

i=1
./count_true_branch "$2"
gcc "rose_"$(basename $2) &> /dev/null

while IFS='' read -r testcase_line || [[ -n "$testcase_line" ]]; do
    output=$(./a.out $testcase_line)

    echo -e "\nRun $i"
    j=0
    printf '%s\n' "$output" | while IFS= read -r line
    do
	if [[ "$line" =~ \  ]]; then
	    val1=$(expr substr "$line" 1 1)
	    val2=$(expr substr "$line" 2 2)

	    echo "Predicate $j -- True:$val1 False:$val2"
	fi
	((j++))
    done
    ((i++))

done < "$1"
