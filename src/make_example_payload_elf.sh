#!/bin/sh

TEST_DIR=/tmp/loadertests
TEST_SRC=${TEST_DIR}/test.c
CFLAGS="-Wall -Wextra ${M32+-m32} -static "
CC=${GCC:-${CC:-gcc}}

set -eu

trap 'rm -rf $TEST_DIR' EXIT

PATH=.:$PATH

mkdir -p $TEST_DIR

cat > $TEST_SRC << EOF
#include <stdio.h>

int main(int argc, char *argv[])
{
    printf("Hello %s!\n", "world");
    for (int i = 0; i < argc; i++)
        printf("argv[%d] = %s\n", i, argv[i]);
    return 0;
}
EOF

$CC $CFLAGS "$TEST_SRC" -o payload.elf

