#!/bin/bash
# Generates a CSV file in a format accepted by
# the R script 'correlations.R'
#
# First argument: testcase file
# Second argument: input C file
# Result is stored in the file: output.csv

i=1
initial_csv="initial.csv"
initial_output="initial_output.csv"
line_csv="line.csv"
output_csv="output.csv"

rm -f "$initial_csv" "$initial_output" "$line_csv" "$output_csv"

./count_true_branch "$2"
./gen_line_num "$2" > $line_csv
gcc "rose_"$(basename $2) &> /dev/null

while IFS='' read -r testcase_line || [[ -n "$testcase_line" ]]; do
    output=$(./a.out $testcase_line)

    printf '%s\n' "$output" | while IFS= read -r line
    do
	if [[ "$line" =~ \  ]]; then
	    val1=$(expr substr "$line" 1 1)
	    val2=$(expr substr "$line" 2 2)

	    if [ $val1 -eq 0 ] && [ $val2 -eq 0 ]; then
	        printf "0," >> "$initial_csv"
	    elif [ $val1 -gt 0 ] && [ $val2 -eq 0 ]; then
	        printf "1," >> "$initial_csv"
	    elif [ $val1 -eq 0 ] && [ $val2 -gt 0 ]; then
	        printf "2," >> "$initial_csv"
	    else
	        printf "3," >> "$initial_csv"
	    fi
	fi
    done
    echo "" >> "$initial_csv"
    ((i++))

done < "$1"

# Take a transpose of the generated csv file
csvtool transpose "$initial_csv" > "$initial_output"

sed -i '$d' "$initial_output"
paste -d',' <(cut -d',' -f"$i" "$initial_output") "$line_csv" <(cut -d',' -f1- "$initial_output") > "$initial_csv"

printf "TestCase," > "$output_csv"
for ((j=1; j<$i-1; j++)); do printf $j',' >> "$output_csv"; done
printf $j >> "$output_csv"
echo "" >> "$output_csv"
printf "LineNumber" >> "$output_csv"
for ((j=1; j<$i; j++)); do printf ',' >> "$output_csv"; done
echo "" >> "$output_csv"
cut -d',' -f2- "$initial_csv" >> "$output_csv"
rm -f "$initial_csv" "$initial_output" "$line_csv"
