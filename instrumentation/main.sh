#!/bin/bash
# Driver script.

set -e

serve_dir="server/serve_dir"

# Insert a random bug in client code.
./insert_bug.sh

(
    cd testcases/CS425
    make

    # Remove stale predicate values (if existent) from previous run.
    rm -f predicate_values
    touch predicate_values

    for file in $serve_dir/*; do
        ./client_exe shivansh:rai@127.0.0.1 6061 $(basename $file) &>> predicate_values
        sleep .5
    done
)

# Collect the generated values of predicates in different runs appropriately.
mv testcases/CS425/predicate_values ../output_data/output.txt
