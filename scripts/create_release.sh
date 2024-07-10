#!/bin/bash

SCRIPT_DIR="$(dirname "$(readlink -f "$0")")"
BASE_DIR="${SCRIPT_DIR%/*}"
BUILD_DIR="${BASE_DIR}/releaseBuild"
RELEASE_DIR="${BASE_DIR}/${1:-release}"
OUTPUT_NAME_BASE="Measurement"
TOOLCHAIN_FILE="$2"

# Cleanup previous Release and Build
rm -rf "$RELEASE_DIR"
rm -rf "$BUILD_DIR"

# Create Folders
mkdir -p "${RELEASE_DIR}/lib"
mkdir -p "${RELEASE_DIR}/include"
mkdir -p "$BUILD_DIR"

# Create Release
cd "$BUILD_DIR"
if [ -n "$TOOLCHAIN_FILE" ]; then
  cmake .. -DCMAKE_BUILD_TYPE=RELEASE -DOUTPUT_NAME_BASE=$OUTPUT_NAME_BASE -DCMAKE_TOOLCHAIN_FILE=${TOOLCHAIN_FILE} 
else
  cmake .. -DCMAKE_BUILD_TYPE=RELEASE -DOUTPUT_NAME_BASE=$OUTPUT_NAME_BASE
fi

cmake --build .

# Copy relevant files
cp "${BUILD_DIR}/lib${OUTPUT_NAME_BASE}.a" "${RELEASE_DIR}/lib/"
cp "${BASE_DIR}/src/measurement/measurement.h" "${RELEASE_DIR}/include/"

# Cleanup Build folder
rm -rf "$BUILD_DIR"

