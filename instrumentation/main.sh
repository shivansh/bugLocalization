#!/bin/bash
# Driver script.
# Argument 1: Server IP
# Argument 2: Server port

set -euo pipefail

serve_dir="server/serve_dir"

main() {
    cd testcases/ftp
    make

    # Remove stale predicate values (if existent) from previous run.
    rm -f predicate_values
    touch predicate_values

    for file in $serve_dir/*; do
        echo "~~ Requesting file $file ~~"
        ./client_exe shivansh:rai@"$1" "$2" $(basename "$file") &>> predicate_values
        sleep .5
    done

    # Add an instance which is guaranteed to fail.
    echo "~~ Requesting an invalid file ~~"
    ./client_exe shivansh:rai@"$1" "$2" "pingu" &>> predicate_values

    cd -
}


./insert_bug.sh      # Insert a random bug in client code.
./instrument_ftp.sh  # Instrument client and server files.
main "$@"

# Collect the generated values of predicates in different runs appropriately.
mv testcases/ftp/predicate_values ../output_data/output.txt
