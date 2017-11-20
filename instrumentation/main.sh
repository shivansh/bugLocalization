#!/bin/bash
# Driver script.

set -e

serve_dir="server/serve_dir"

# Insert a random bug in client code.
./insert_bug.sh

cd testcases/CS425
make

# Remove stale predicate values (if existent) from previous run.
rm -f predicate_values
touch predicate_values

for file in $serve_dir/*; do
    ./client_exe shivansh:rai@127.0.0.1 6061 $file &>> predicate_values
    sleep .5
done
