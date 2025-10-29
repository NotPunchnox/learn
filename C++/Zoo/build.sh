#!/usr/bin/env bash
set -euo pipefail

BUILD_DIR=build
EXE_NAME=ZOO

mkdir -p "$BUILD_DIR"
cd "$BUILD_DIR"

cmake .. 
cmake --build . --config Release

EXEC_PATH=$(find . -type f \( -iname "${EXE_NAME}" -o -iname "${EXE_NAME}.exe" \) -perm /111 -print -quit || true)

if [[ -n "$EXEC_PATH" ]]; then
    EXEC_PATH=${EXEC_PATH#./}
    echo "Running $EXEC_PATH"
    "./$EXEC_PATH"
else
    echo "Executable ${EXE_NAME} not found in ${BUILD_DIR}" >&2
    exit 1
fi