#!/bin/bash
# Instruments the client and server files in testcases/ftp

set -e

echo "-- Instrumenting client and server files --"

# Initial cleanup
(
    cd testcases/ftp/server
    rm -f standard.h rose_server.c
    ln -s ../include/standard.h standard.h

    cd ../client
    rm -f standard.h rose_client.c
    ln -s ../include/standard.h standard.h
)

./count_true_branch testcases/ftp/server/server.c
./count_true_branch testcases/ftp/client/client.c
mv rose_server.c testcases/ftp/server/
mv rose_client.c testcases/ftp/client/

# Final cleanup
rm -f testcases/ftp/client/standard.h testcases/ftp/server/standard.h
