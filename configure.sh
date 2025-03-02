#!/usr/bin/env sh

set -eu

PROJECT_DIR="$(dirname "$(realpath "$0")")"
BUILD_DIR="${PROJECT_DIR}/build"
CMAKE_GENERATOR="Ninja"
CMAKE_BUILD_TYPE="Debug"
CMAKE_CXX_COMPILER_LAUNCHER="$(which ccache)"
CMAKE_CXX_COMPILER="$(which clang++)"
CMAKE_CXX_FLAGS="-fsanitize=address,leak,undefined -fno-omit-frame-pointer" # -fno-exceptions
CMAKE_CXX_FLAGS="-std=c++2a -fno-rtti -Wall -Wextra -Wshadow -Wconversion -Wpedantic -Werror ${CMAKE_CXX_FLAGS:-}"
CMAKE_COMPILE_COMMANDS="ON"

MTP_BUILD_MODULE="ON"
MTP_USE_STD_MODULE="ON"

if ! command -v cmake >/dev/null 2>&1; then
	echo "Error: cmake was not found in PATH!"
	exit 1
fi

cmake \
	-S "$PROJECT_DIR"\
	-B "$BUILD_DIR" \
	-G "$CMAKE_GENERATOR" \
	-DCMAKE_BUILD_TYPE="$CMAKE_BUILD_TYPE" \
	-DCMAKE_CXX_COMPILER_LAUNCHER="$CMAKE_CXX_COMPILER_LAUNCHER" \
	-DCMAKE_CXX_COMPILER="$CMAKE_CXX_COMPILER" \
	-DCMAKE_CXX_FLAGS="$CMAKE_CXX_FLAGS" \
	-DCMAKE_EXPORT_COMPILE_COMMANDS="$CMAKE_COMPILE_COMMANDS" \
	-DMTP_BUILD_MODULE="$MTP_BUILD_MODULE" \
	-DMTP_USE_STD_MODULE="$MTP_USE_STD_MODULE" \
	--fresh
