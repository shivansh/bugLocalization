#!/bin/bash
# Instruments the client and server files in testcases/CS425

set -e

# Initial cleanup
(
    cd testcases/CS425/server
    rm -f standard.h rose_server.c
    ln -s ../include/standard.h standard.h

    cd ../client
    rm -f standard.h rose_client.c
    ln -s ../include/standard.h standard.h
)

./count_true_branch testcases/CS425/server/server.c
./count_true_branch testcases/CS425/client/client.c
mv rose_server.c testcases/CS425/server/
mv rose_client.c testcases/CS425/client/

# Final cleanup
rm -f testcases/CS425/client/standard.h testcases/CS425/server/standard.h
