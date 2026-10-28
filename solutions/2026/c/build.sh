#!/bin/bash

DAY="$1"
SOURCE_FILE="$DAY.c"
EXE_NAME="$DAY"

CLANG_FLAGS="-std=gnu23"
CLANG_FLAGS_OPT="-O3 -march=znver5"
CLANG_WARN_FLAGS="-Wall -Wextra -Werror -Wconversion -Wshadow"

if [[ "$2" == "debug" ]]; then
    clang $CLANG_FLAGS -g $CLANG_WARN_FLAGS "$SOURCE_FILE" -o "$EXE_NAME"
else
    clang $CLANG_FLAGS $CLANG_FLAGS_OPT $CLANG_WARN_FLAGS "$SOURCE_FILE" -o "$EXE_NAME"
fi

